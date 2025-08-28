#!/bin/bash
set -euo pipefail

echo "🚀 Deploying Xclipy..."

# --- Config ---
QT_VERSION="6.9.2"
QT_ROOT="$HOME/Qt/${QT_VERSION}/macos"
APP_NAME="Xclipy"

# --- Helpers ---
die() { echo "❌ $*" >&2; exit 1; }

# Extract version from CMakeLists.txt
VERSION=$(grep -Eo 'set\(XCLIPY_VERSION "[0-9]+\.[0-9]+\.[0-9]+"\)' CMakeLists.txt \
  | head -1 | sed -E 's/.*"([0-9]+\.[0-9]+\.[0-9]+)".*/\1/') || true
[[ -z "${VERSION:-}" ]] && VERSION="1.0.0"
echo "📦 Building version: $VERSION"

# Check for CMake and add to PATH
CMAKE_PATHS=(
    "/opt/homebrew/bin"
    "/usr/local/bin"
    "$HOME/Qt/Tools/CMake/bin"
    "$HOME/Qt/Tools/Ninja"
)

for cmake_path in "${CMAKE_PATHS[@]}"; do
    if [[ -d "$cmake_path" ]]; then
        export PATH="$cmake_path:$PATH"
    fi
done

if ! command -v cmake >/dev/null 2>&1; then
    die "CMake not found. Please install CMake via Homebrew (brew install cmake) or Qt Maintenance Tool"
fi
echo "✅ CMake found: $(which cmake)"
echo "ℹ️ CMake version: $(cmake --version | head -n 1)"

# Sanity: Qt present
[[ -d "$QT_ROOT" ]] || die "Qt not found at $QT_ROOT"

# Clean environment
unset PKG_CONFIG_PATH QTDIR QT_PLUGIN_PATH QT_QPA_PLATFORM_PLUGIN_PATH \
      DYLD_FRAMEWORK_PATH DYLD_LIBRARY_PATH
export PATH="${QT_ROOT}/bin:/usr/bin:/bin:/usr/sbin:/sbin:$PATH"

# --- macOS Universal Build ---
if [[ "$OSTYPE" == darwin* ]]; then
  echo "🍎 Creating macOS Universal build…"
  rm -rf build
  mkdir -p build/arm64 build/x86_64 build/universal

  # Common cmake args
  cmake_common=(
    -DCMAKE_PREFIX_PATH="$QT_ROOT"
    -DCMAKE_FIND_FRAMEWORK="FIRST"
    -DCMAKE_BUILD_TYPE="Release"
  )

  echo "🔨 Building arm64…"
  cmake -S . -B build/arm64 -DCMAKE_OSX_ARCHITECTURES=arm64 "${cmake_common[@]}"
  cmake --build build/arm64 -j $(sysctl -n hw.ncpu)

  echo "🔨 Building x86_64…"
  cmake -S . -B build/x86_64 -DCMAKE_OSX_ARCHITECTURES=x86_64 "${cmake_common[@]}"
  cmake --build build/x86_64 -j $(sysctl -n hw.ncpu)

  # --- CRITICAL FIX: Create a fresh app bundle instead of copying ---
  echo "📦 Creating fresh universal app bundle..."
  
  # Create app bundle structure
  mkdir -p "build/universal/${APP_NAME}.app/Contents/MacOS"
  mkdir -p "build/universal/${APP_NAME}.app/Contents/Resources"
  
  # Merge the main executable with lipo
  echo "🧬 Merging Mach-O slices with lipo…"
  lipo -create \
    "build/arm64/${APP_NAME}.app/Contents/MacOS/${APP_NAME}" \
    "build/x86_64/${APP_NAME}.app/Contents/MacOS/${APP_NAME}" \
    -output "build/universal/${APP_NAME}.app/Contents/MacOS/${APP_NAME}"
  
  # Make executable
  chmod +x "build/universal/${APP_NAME}.app/Contents/MacOS/${APP_NAME}"

  # --- FIX: Copy ALL necessary files from BOTH builds ---
  echo "📁 Copying app bundle resources..."
  
  # Copy Info.plist and PkgInfo (prefer arm64 version, fallback to source)
  if [[ -f "build/arm64/${APP_NAME}.app/Contents/Info.plist" ]]; then
    cp "build/arm64/${APP_NAME}.app/Contents/Info.plist" "build/universal/${APP_NAME}.app/Contents/"
  elif [[ -f "Info.plist" ]]; then
    cp "Info.plist" "build/universal/${APP_NAME}.app/Contents/"
  fi
  
  if [[ -f "build/arm64/${APP_NAME}.app/Contents/PkgInfo" ]]; then
    cp "build/arm64/${APP_NAME}.app/Contents/PkgInfo" "build/universal/${APP_NAME}.app/Contents/"
  elif [[ -f "PkgInfo" ]]; then
    cp "PkgInfo" "build/universal/${APP_NAME}.app/Contents/"
  else
    echo "APPL????" > "build/universal/${APP_NAME}.app/Contents/PkgInfo"
  fi

  # --- FIX: Copy ALL resources properly ---
  echo "🎨 Copying icon resources..."
  
  # First, try to copy from the arm64 build (which should have processed resources)
  if [[ -d "build/arm64/${APP_NAME}.app/Contents/Resources" ]]; then
    cp -R "build/arm64/${APP_NAME}.app/Contents/Resources/"* "build/universal/${APP_NAME}.app/Contents/Resources/" 2>/dev/null || true
  fi
  
  # Then supplement with any additional resources from source
  if [[ -d "resources" ]]; then
    cp -R resources/* "build/universal/${APP_NAME}.app/Contents/Resources/" 2>/dev/null || true
  fi
  
  # --- FIX: Explicitly ensure icon files exist ---
  # Check for common icon file patterns and copy them
  ICON_PATTERNS=("*.icns" "*.png" "*.jpg" "*.jpeg" "*.iconset")
  for pattern in "${ICON_PATTERNS[@]}"; do
    find . -name "$pattern" | while read icon_file; do
      if [[ -f "$icon_file" || -d "$icon_file" ]]; then
        echo "📄 Copying icon resource: $icon_file"
        cp -R "$icon_file" "build/universal/${APP_NAME}.app/Contents/Resources/" 2>/dev/null || true
      fi
    done
  done

  # --- FIX: Verify icon is properly referenced in Info.plist ---
  if [[ -f "build/universal/${APP_NAME}.app/Contents/Info.plist" ]]; then
    echo "🔍 Checking Info.plist for icon reference..."
    
    # Look for any .icns file in resources
    ICNS_FILE=$(find "build/universal/${APP_NAME}.app/Contents/Resources" -name "*.icns" | head -1)
    if [[ -n "$ICNS_FILE" ]]; then
      ICON_NAME=$(basename "$ICNS_FILE")
      echo "🎯 Found icon file: $ICON_NAME"
      
      # Check if CFBundleIconFile is already set correctly
      if command -v plutil >/dev/null 2>&1; then
        CURRENT_ICON=$(plutil -extract CFBundleIconFile xml1 -o - "build/universal/${APP_NAME}.app/Contents/Info.plist" 2>/dev/null | grep string | sed 's/.*<string>\(.*\)<\/string>.*/\1/' || echo "")
        
        if [[ "$CURRENT_ICON" != "$ICON_NAME" ]]; then
          echo "✏️  Setting CFBundleIconFile to: $ICON_NAME"
          plutil -replace CFBundleIconFile -string "$ICON_NAME" "build/universal/${APP_NAME}.app/Contents/Info.plist"
        else
          echo "✅ CFBundleIconFile already correctly set to: $CURRENT_ICON"
        fi
      fi
    else
      echo "⚠️  No .icns file found in Resources directory"
    fi
  fi

  echo "✅ Universal app: build/universal/${APP_NAME}.app"

  # Deploy Qt frameworks
  if command -v macdeployqt >/dev/null 2>&1; then
    echo "📦 Running macdeployqt…"
    macdeployqt "build/universal/${APP_NAME}.app" -always-overwrite -verbose=1 || true
  fi

  # Create DMG (or ZIP fallback)
  if command -v create-dmg >/dev/null 2>&1; then
    echo "📀 Creating DMG…"
    create-dmg \
      --volname "${APP_NAME} ${VERSION}" \
      --window-pos 200 120 \
      --window-size 600 300 \
      --icon-size 100 \
      --icon "${APP_NAME}.app" 175 120 \
      --hide-extension "${APP_NAME}.app" \
      --app-drop-link 425 120 \
      "build/${APP_NAME}-${VERSION}-macOS-universal.dmg" \
      "build/universal/${APP_NAME}.app"
    echo "✅ DMG: build/${APP_NAME}-${VERSION}-macOS-universal.dmg"
  else
    echo "📦 create-dmg not found; zipping…"
    (cd build/universal && zip -r "../${APP_NAME}-${VERSION}-macOS-universal.zip" "${APP_NAME}.app")
    echo "✅ ZIP: build/${APP_NAME}-${VERSION}-macOS-universal.zip"
  fi
fi

# Source tarball
echo "📚 Creating source distribution…"
mkdir -p build
tar -czf "build/xclipy-${VERSION}-source.tar.gz" \
  --exclude=build --exclude=.git --exclude=*.o --exclude=*.app \
  --exclude=packages --exclude=.DS_Store --exclude=.qmake.stash \
  --exclude=Makefile --exclude=qrc_resources.cpp .
echo "✅ Source: build/xclipy-${VERSION}-source.tar.gz"

# Other platform packaging...
if [[ "$OSTYPE" != darwin* && "$OSTYPE" != msys* && "$OSTYPE" != cygwin* ]]; then
  echo "🐧 Creating Linux distribution…"
  (cd build && tar -czf "xclipy-${VERSION}-Linux.tar.gz" "Xclipy" 2>/dev/null || true)
  echo "✅ Linux: build/xclipy-${VERSION}-Linux.tar.gz"
fi

if [[ "$OSTYPE" == msys* || "$OSTYPE" == cygwin* ]]; then
  echo "🪟 Creating Windows distribution…"
  (cd build && zip -r "xclipy-${VERSION}-Windows.zip" "Xclipy.exe" 2>/dev/null || true)
  echo "✅ Windows: build/xclipy-${VERSION}-Windows.zip"
fi

echo "🎉 Deployment completed!"
echo "Artifacts in build/:"
ls -la build/*.tar.gz build/*.dmg build/*.zip 2>/dev/null || true