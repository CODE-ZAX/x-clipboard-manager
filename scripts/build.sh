#!/bin/bash

# ClipboardManager Build Script
# This script builds the ClipboardManager application

set -e  # Exit on any error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Script directory
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"

echo -e "${GREEN}Building ClipboardManager with improvements...${NC}"
echo "Project root: $PROJECT_ROOT"

# Change to project root
cd "$PROJECT_ROOT"

# Clean previous build
echo -e "${YELLOW}Cleaning previous build...${NC}"
make clean 2>/dev/null || true
rm -rf build/*.o build/*.cpp build/*.h build/Makefile build/.qmake.stash 2>/dev/null || true

# Run qmake
echo -e "${YELLOW}Running qmake...${NC}"
qmake ClipboardManager.pro

# Build the application
echo -e "${YELLOW}Building application...${NC}"
make -j$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)

if [ $? -eq 0 ]; then
    echo -e "${GREEN}Build successful!${NC}"
    echo -e "${GREEN}Executable location: build/ClipboardManager${NC}"
    echo -e "${YELLOW}You can now run: ./build/ClipboardManager${NC}"
else
    echo -e "${RED}Build failed. Please check the error messages above.${NC}"
    exit 1
fi
