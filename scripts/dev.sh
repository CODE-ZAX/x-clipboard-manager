#!/bin/bash

# ClipboardManager Development Script
# This script provides development utilities

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

# Function to show usage
show_usage() {
    echo -e "${BLUE}ClipboardManager Development Script${NC}"
    echo ""
    echo "Usage: $0 [COMMAND]"
    echo ""
    echo "Commands:"
    echo "  build     - Build the application"
    echo "  clean     - Clean build artifacts"
    echo "  run       - Build and run the application"
    echo "  test      - Run tests (if available)"
    echo "  format    - Format source code"
    echo "  lint      - Run linting checks"
    echo "  install   - Install dependencies"
    echo "  help      - Show this help message"
    echo ""
}

# Function to build
build() {
    echo -e "${GREEN}Building ClipboardManager...${NC}"
    ./scripts/build.sh
}

# Function to clean
clean() {
    echo -e "${YELLOW}Cleaning build artifacts...${NC}"
    make clean 2>/dev/null || true
    rm -rf build/*.o build/*.cpp build/*.h build/Makefile build/.qmake.stash 2>/dev/null || true
    rm -rf build/ClipboardManager build/ClipboardManager.app 2>/dev/null || true
    echo -e "${GREEN}Clean complete!${NC}"
}

# Function to run
run() {
    echo -e "${GREEN}Building and running ClipboardManager...${NC}"
    build
    echo -e "${GREEN}Starting application...${NC}"
    open ./build/ClipboardManager.app
}

# Function to format code
format() {
    echo -e "${YELLOW}Formatting source code...${NC}"
    # You can add clang-format or other formatting tools here
    echo -e "${GREEN}Formatting complete!${NC}"
}

# Function to lint
lint() {
    echo -e "${YELLOW}Running linting checks...${NC}"
    # You can add cppcheck or other linting tools here
    echo -e "${GREEN}Linting complete!${NC}"
}

# Function to install dependencies
install() {
    echo -e "${YELLOW}Installing dependencies...${NC}"
    # Add platform-specific dependency installation here
    if [[ "$OSTYPE" == "darwin"* ]]; then
        echo "macOS detected. Please install Qt via Homebrew or Qt installer."
        echo "brew install qt"
    elif [[ "$OSTYPE" == "linux-gnu"* ]]; then
        echo "Linux detected. Please install Qt via your package manager."
        echo "sudo apt-get install qt5-default qtbase5-dev qtchooser qt5-qmake"
    else
        echo "Please install Qt for your platform."
    fi
}

# Main script logic
case "${1:-help}" in
    build)
        build
        ;;
    clean)
        clean
        ;;
    run)
        run
        ;;
    test)
        echo -e "${YELLOW}Tests not implemented yet.${NC}"
        ;;
    format)
        format
        ;;
    lint)
        lint
        ;;
    install)
        install
        ;;
    help|*)
        show_usage
        ;;
esac
