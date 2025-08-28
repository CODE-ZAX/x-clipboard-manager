#!/bin/bash

# Xclipy Packaging Script
# This script creates distributable packages

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Script directory
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"

# Change to project root
cd "$PROJECT_ROOT"

# Version from CMakeLists.txt or default
VERSION=$(grep "XCLIPY_VERSION" CMakeLists.txt | head -1 | sed 's/.*XCLIPY_VERSION "\([0-9.]*\)".*/\1/' || echo "1.0.0")
PACKAGE_NAME="Xclipy-${VERSION}"

echo -e "${GREEN}Packaging Xclipy v${VERSION}...${NC}"

# Ensure we have a clean build
echo -e "${YELLOW}Building application...${NC}"
./scripts/build.sh

# Create package directory
PACKAGE_DIR="packages/${PACKAGE_NAME}"
mkdir -p "$PACKAGE_DIR"

# Copy application
echo -e "${YELLOW}Copying application...${NC}"
cp -r build/Xclipy* "$PACKAGE_DIR/"

# Copy documentation
echo -e "${YELLOW}Copying documentation...${NC}"
mkdir -p "$PACKAGE_DIR/docs"
cp README.md "$PACKAGE_DIR/docs/"
cp LICENSE "$PACKAGE_DIR/docs/"

# Copy resources
echo -e "${YELLOW}Copying resources...${NC}"
mkdir -p "$PACKAGE_DIR/resources"
cp -r resources/* "$PACKAGE_DIR/resources/" 2>/dev/null || true

# Create platform-specific packages
if [[ "$OSTYPE" == "darwin"* ]]; then
    echo -e "${YELLOW}Creating macOS package...${NC}"
    
    # Create DMG
    if command -v create-dmg &> /dev/null; then
        create-dmg \
            --volname "Xclipy ${VERSION}" \
            --window-pos 200 120 \
            --window-size 600 300 \
            --icon-size 100 \
            --icon "Xclipy.app" 175 120 \
            --hide-extension "Xclipy.app" \
            --app-drop-link 425 120 \
            "packages/Xclipy-${VERSION}-macOS.dmg" \
            "$PACKAGE_DIR/"
    else
        echo -e "${YELLOW}create-dmg not found. Creating zip archive...${NC}"
        cd packages
        zip -r "${PACKAGE_NAME}-macOS.zip" "$PACKAGE_NAME"
        cd ..
    fi
    
elif [[ "$OSTYPE" == "linux-gnu"* ]]; then
    echo -e "${YELLOW}Creating Linux package...${NC}"
    
    # Create AppImage or tar.gz
    cd packages
    tar -czf "${PACKAGE_NAME}-Linux.tar.gz" "$PACKAGE_NAME"
    cd ..
    
else
    echo -e "${YELLOW}Creating generic package...${NC}"
    cd packages
    zip -r "${PACKAGE_NAME}.zip" "$PACKAGE_NAME"
    cd ..
fi

echo -e "${GREEN}Packaging complete!${NC}"
echo -e "${GREEN}Package location: packages/${NC}"

# List created packages
echo -e "${BLUE}Created packages:${NC}"
ls -la packages/*.zip packages/*.tar.gz packages/*.dmg 2>/dev/null || true
