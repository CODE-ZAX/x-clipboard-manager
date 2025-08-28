#!/bin/bash

# Deployment script for Xclipy
echo "Deploying Xclipy..."

# Build the application
./scripts/build.sh

# Get version from CMakeLists.txt
VERSION=$(grep "XCLIPY_VERSION" CMakeLists.txt | head -1 | sed 's/.*XCLIPY_VERSION "\([0-9.]*\)".*/\1/' || echo "1.0.0")
echo "Building version: $VERSION"

# Create app bundle for macOS
if [[ "$OSTYPE" == "darwin"* ]]; then
    echo "Creating macOS app bundle..."
    
    # The build script already creates Xclipy.app, so we just need to ensure it's complete
    if [ ! -d "build/Xclipy.app" ]; then
        echo "Error: Xclipy.app not found. Build may have failed."
        exit 1
    fi
    
    # Ensure Info.plist and PkgInfo are in the app bundle
    if [ ! -f "build/Xclipy.app/Contents/Info.plist" ]; then
        echo "Copying Info.plist to app bundle..."
        cp Info.plist build/Xclipy.app/Contents/
    fi
    
    if [ ! -f "build/Xclipy.app/Contents/PkgInfo" ]; then
        echo "Copying PkgInfo to app bundle..."
        cp PkgInfo build/Xclipy.app/Contents/
    fi
    
    # Ensure resources are in the app bundle
    if [ ! -d "build/Xclipy.app/Contents/Resources" ]; then
        echo "Creating Resources directory..."
        mkdir -p build/Xclipy.app/Contents/Resources
    fi
    
    # Copy resources if they don't exist
    if [ ! -f "build/Xclipy.app/Contents/Resources/clipboard.icns" ]; then
        echo "Copying resources to app bundle..."
        cp -r resources/* build/Xclipy.app/Contents/Resources/ 2>/dev/null || true
    fi
    
    echo "macOS app bundle ready: build/Xclipy.app"
    
    # Create DMG
    if command -v create-dmg &> /dev/null; then
        echo "Creating DMG..."
        create-dmg \
            --volname "Xclipy $VERSION" \
            --window-pos 200 120 \
            --window-size 600 300 \
            --icon-size 100 \
            --icon "Xclipy.app" 175 120 \
            --hide-extension "Xclipy.app" \
            --app-drop-link 425 120 \
            "build/Xclipy-$VERSION-macOS.dmg" \
            "build/Xclipy.app"
        echo "DMG created: build/Xclipy-$VERSION-macOS.dmg"
    else
        echo "create-dmg not found. Creating ZIP archive instead..."
        cd build
        zip -r "Xclipy-$VERSION-macOS.zip" "Xclipy.app"
        cd ..
        echo "ZIP created: build/Xclipy-$VERSION-macOS.zip"
    fi
    
    # Create macOS app bundle tarball
    echo "Creating macOS app bundle tarball..."
    cd build
    tar -czf "Xclipy-$VERSION-macOS.tar.gz" "Xclipy.app"
    cd ..
    echo "macOS tarball created: build/Xclipy-$VERSION-macOS.tar.gz"
fi

# Create source distribution
echo "Creating source distribution..."
tar -czf "build/xclipy-$VERSION-source.tar.gz" \
    --exclude=build \
    --exclude=.git \
    --exclude=*.o \
    --exclude=*.app \
    --exclude=packages \
    --exclude=.DS_Store \
    --exclude=.qmake.stash \
    --exclude=Makefile \
    --exclude=qrc_resources.cpp \
    .
echo "Source tarball created: build/xclipy-$VERSION-source.tar.gz"

# Create Linux distribution (if not on macOS)
if [[ "$OSTYPE" != "darwin"* ]]; then
    echo "Creating Linux distribution..."
    cd build
    tar -czf "xclipy-$VERSION-Linux.tar.gz" "Xclipy"
    cd ..
    echo "Linux tarball created: build/xclipy-$VERSION-Linux.tar.gz"
fi

# Create Windows distribution (if on Windows or cross-compiling)
if [[ "$OSTYPE" == "msys"* ]] || [[ "$OSTYPE" == "cygwin"* ]]; then
    echo "Creating Windows distribution..."
    cd build
    zip -r "xclipy-$VERSION-Windows.zip" "Xclipy.exe"
    cd ..
    echo "Windows ZIP created: build/xclipy-$VERSION-Windows.zip"
fi

echo "Deployment completed!"
echo "Files created in build/ directory:"
ls -la build/*.tar.gz build/*.dmg build/*.zip 2>/dev/null || true
echo ""
echo "Distribution packages ready:"
echo "- Source: xclipy-$VERSION-source.tar.gz"
if [[ "$OSTYPE" == "darwin"* ]]; then
    echo "- macOS DMG: Xclipy-$VERSION-macOS.dmg"
    echo "- macOS Tarball: Xclipy-$VERSION-macOS.tar.gz"
    echo "- macOS ZIP: Xclipy-$VERSION-macOS.zip"
fi
if [[ "$OSTYPE" != "darwin"* ]]; then
    echo "- Linux: xclipy-$VERSION-Linux.tar.gz"
fi
if [[ "$OSTYPE" == "msys"* ]] || [[ "$OSTYPE" == "cygwin"* ]]; then
    echo "- Windows: xclipy-$VERSION-Windows.zip"
fi
