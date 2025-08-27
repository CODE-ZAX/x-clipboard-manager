#!/bin/bash

# Test script for global hotkey functionality
# This script helps verify that the global hotkey is working

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

echo -e "${BLUE}ClipboardManager Global Hotkey Test${NC}"
echo "=========================================="
echo ""

# Check if ClipboardManager is running
if pgrep -f "ClipboardManager" > /dev/null; then
    echo -e "${GREEN}✓ ClipboardManager is running${NC}"
else
    echo -e "${RED}✗ ClipboardManager is not running${NC}"
    echo "Please start ClipboardManager first:"
    echo "  ./build/ClipboardManager"
    exit 1
fi

echo ""
echo -e "${YELLOW}Testing Global Hotkey Functionality:${NC}"
echo ""
echo "1. Make sure ClipboardManager is running in the background"
echo "2. Copy some text to clipboard (Ctrl+C)"
echo "3. Press the global hotkey (default: Ctrl+Shift+V)"
echo "4. The clipboard history window should appear"
echo ""
echo -e "${BLUE}Test Steps:${NC}"
echo "1. Copy this text: 'Test clipboard entry'"
echo "2. Press Ctrl+Shift+V (or your configured hotkey)"
echo "3. Verify the history window shows the copied text"
echo "4. Try copying multiple items and check the history"
echo ""
echo -e "${YELLOW}Troubleshooting:${NC}"
echo "- If hotkey doesn't work, check Preferences → Global Hotkey settings"
echo "- Make sure the hotkey is enabled in preferences"
echo "- Try a different hotkey combination if there are conflicts"
echo "- Check system permissions for accessibility/input monitoring"
echo ""
echo -e "${GREEN}Test completed!${NC}"
