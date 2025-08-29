#!/bin/bash

# Test CI/CD Pipeline Locally
# This script simulates what the GitHub Actions workflow will do

set -euo pipefail

echo "🧪 Testing CI/CD Pipeline Locally..."

# Colors
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
NC='\033[0m'

# Helper function
log() {
    echo -e "${GREEN}[TEST]${NC} $1"
}

warn() {
    echo -e "${YELLOW}[WARN]${NC} $1"
}

error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Check prerequisites
log "Checking prerequisites..."

if ! command -v qmake >/dev/null 2>&1; then
    error "qmake not found. Please install Qt"
    exit 1
fi

if ! command -v cmake >/dev/null 2>&1; then
    error "cmake not found. Please install CMake"
    exit 1
fi

log "✅ Qt version: $(qmake -query QT_VERSION)"
log "✅ CMake version: $(cmake --version | head -n 1)"

# Extract version
VERSION=$(grep -Eo 'set\(XCLIPY_VERSION "[0-9]+\.[0-9]+\.[0-9]+"\)' CMakeLists.txt | head -1 | sed -E 's/.*"([0-9]+\.[0-9]+\.[0-9]+)".*/\1/')
log "📦 Version: $VERSION"

# Test builds
log "🔨 Testing qmake build..."

# Clean any existing artifacts
make clean >/dev/null 2>&1 || true
rm -f .qmake.stash Makefile

# Configure qmake (disable SDK version check on macOS)
if [[ "$OSTYPE" == darwin* ]]; then
    qmake ClipboardManager.pro CONFIG+=sdk_no_version_check
else
    qmake ClipboardManager.pro
fi

# Show available targets for debugging
log "📋 Available Makefile targets:"
make help 2>/dev/null || grep "^[a-zA-Z]" Makefile | head -5 || true

# Build the application
if make -j$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4); then
    log "✅ qmake build successful"
else
    error "❌ qmake build failed"
    exit 1
fi

log "🔨 Testing CMake build..."
mkdir -p build-test
cd build-test
if cmake .. -DCMAKE_BUILD_TYPE=Release && make -j$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4); then
    log "✅ CMake build successful"
else
    error "❌ CMake build failed"
    exit 1
fi
cd ..

# Test deployment script
if [[ "$OSTYPE" == darwin* ]]; then
    log "🍎 Testing macOS universal build..."
    if [[ -f "scripts/deploy.sh" ]]; then
        chmod +x scripts/deploy.sh
        if ./scripts/deploy.sh; then
            log "✅ Universal build successful"
            
            # Check for expected artifacts
            if [[ -f "build/Xclipy-${VERSION}-macOS-universal.dmg" ]] || [[ -f "build/Xclipy-${VERSION}-macOS-universal.zip" ]]; then
                log "✅ Distribution package created"
            else
                warn "⚠️  Distribution package not found"
            fi
        else
            error "❌ Universal build failed"
            exit 1
        fi
    else
        warn "⚠️  deploy.sh not found, skipping universal build test"
    fi
fi

# Check for release artifacts
log "📦 Checking release artifacts..."
find build/ -name "*.dmg" -o -name "*.zip" -o -name "*.tar.gz" 2>/dev/null | while read -r artifact; do
    log "📁 Found: $(basename "$artifact")"
done

# Simulate changelog extraction
if [[ -f "CHANGELOG.md" ]]; then
    log "📄 Testing changelog extraction..."
    CHANGELOG=$(awk '/^## \['"$VERSION"'\]/, /^## \[/ {if (/^## \[/ && !/^## \['"$VERSION"'\]/) exit; print}' CHANGELOG.md | head -n -1 || true)
    if [[ -n "$CHANGELOG" ]]; then
        log "✅ Changelog extracted for version $VERSION"
        echo "Preview:"
        echo "$CHANGELOG" | head -5
        [[ $(echo "$CHANGELOG" | wc -l) -gt 5 ]] && echo "..."
    else
        warn "⚠️  No changelog found for version $VERSION"
    fi
fi

log "🎉 CI/CD test completed successfully!"
log "Your workflow should work when pushed to GitHub"

# Cleanup
make clean >/dev/null 2>&1 || true
rm -rf build-test
