#!/bin/bash

# Comprehensive test script for ClipboardManager features
# This script helps verify all functionality including new features

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

echo -e "${BLUE}ClipboardManager Feature Test${NC}"
echo "================================"
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
echo -e "${YELLOW}Testing Features:${NC}"
echo ""

echo -e "${BLUE}1. Global Hotkey Toggle Test:${NC}"
echo "   - Press Ctrl+Shift+V (or your configured hotkey)"
echo "   - History window should appear"
echo "   - Press the same hotkey again"
echo "   - History window should hide"
echo "   - Press again to show it"
echo ""

echo -e "${BLUE}2. Text Clipboard Test:${NC}"
echo "   - Copy this text: 'Test clipboard entry'"
echo "   - Check that it appears in the history window"
echo "   - Copy multiple different text items"
echo "   - Verify they all appear in history"
echo ""

echo -e "${BLUE}3. File/Folder Clipboard Test:${NC}"
echo "   - Select a file in Finder (Cmd+C)"
echo "   - Check that it appears in history with file icon"
echo "   - Select a folder in Finder (Cmd+C)"
echo "   - Check that it appears in history with folder icon"
echo "   - Select multiple files/folders"
echo "   - Verify they appear as a single entry"
echo ""

echo -e "${BLUE}4. Copy Actions Test:${NC}"
echo "   - Right-click on a text entry"
echo "   - Choose 'Copy as Text' - should copy as text"
echo "   - Right-click on a file entry"
echo "   - Choose 'Copy as Files' - should copy as file paths"
echo "   - Double-click text entries - should copy as text"
echo "   - Double-click file entries - should copy as files"
echo ""

echo -e "${BLUE}5. Search Functionality Test:${NC}"
echo "   - Type in the search box"
echo "   - Verify that only matching entries are shown"
echo "   - Clear the search box"
echo "   - Verify all entries are shown again"
echo ""

echo -e "${BLUE}6. Context Menu Test:${NC}"
echo "   - Right-click on any entry"
echo "   - Verify menu shows: Copy as Text, Copy as Files, Remove, Clear All"
echo "   - Test 'Remove' on an entry"
echo "   - Test 'Clear All' (be careful!)"
echo ""

echo -e "${BLUE}7. Settings Test:${NC}"
echo "   - Open Preferences (tray icon → Preferences)"
echo "   - Test changing history size"
echo "   - Test enabling/disabling global hotkey"
echo "   - Test changing global hotkey combination"
echo "   - Save settings and verify they persist"
echo ""

echo -e "${BLUE}8. Duplicate Prevention Test:${NC}"
echo "   - Copy the same text multiple times"
echo "   - Verify it only appears once in history"
echo "   - Copy the same file multiple times"
echo "   - Verify it only appears once in history"
echo ""

echo -e "${YELLOW}Expected Behavior:${NC}"
echo "- Global hotkey toggles history window on/off"
echo "- File entries show folder/file icons"
echo "- Text entries show document icons"
echo "- Right-click provides appropriate copy options"
echo "- Search filters entries in real-time"
echo "- Settings are saved and restored"
echo "- Duplicates are prevented"
echo ""

echo -e "${YELLOW}Troubleshooting:${NC}"
echo "- If hotkey doesn't work, check Preferences → Global Hotkey settings"
echo "- If files don't appear, check system permissions"
echo "- If icons don't show, this is normal on some systems"
echo "- If search doesn't work, try restarting the application"
echo ""

echo -e "${GREEN}Test completed!${NC}"
echo ""
echo -e "${BLUE}To run individual tests:${NC}"
echo "  ./scripts/test_hotkey.sh    # Global hotkey test"
echo "  ./scripts/dev.sh run        # Build and run"
echo "  ./scripts/dev.sh clean      # Clean build artifacts"
