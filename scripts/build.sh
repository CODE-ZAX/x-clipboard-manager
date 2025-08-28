#!/bin/bash

# Build script for Xclipy
echo "Building Xclipy..."

# Clean previous build
rm -rf build/
mkdir -p build

# Build using qmake
qmake ClipboardManager.pro -o build/Makefile
cd build
make -j$(nproc)

echo "Build completed! Executable is in build/Xclipy"
