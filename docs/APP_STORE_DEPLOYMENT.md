# Mac App Store Deployment Guide for Xclipy

This guide covers the steps required to deploy Xclipy to the Mac App Store.

## Prerequisites

1. **Apple Developer Account** ($99/year)
2. **Xcode** (latest version)
3. **App Store Connect** access
4. **Code Signing Certificates**
5. **Provisioning Profiles**

## Step 1: Prepare Your App

### 1.1 Update Info.plist for App Store

```xml
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
    <key>CFBundleExecutable</key>
    <string>Xclipy</string>
    <key>CFBundleIdentifier</key>
    <string>com.xclipy.app</string>
    <key>CFBundleName</key>
    <string>Xclipy</string>
    <key>CFBundleDisplayName</key>
    <string>Xclipy</string>
    <key>CFBundleVersion</key>
    <string>1.0</string>
    <key>CFBundleShortVersionString</key>
    <string>1.0</string>
    <key>CFBundlePackageType</key>
    <string>APPL</string>
    <key>CFBundleSignature</key>
    <string>????</string>
    <key>CFBundleInfoDictionaryVersion</key>
    <string>6.0</string>
    <key>CFBundleIconFile</key>
    <string>clipboard.icns</string>
    <key>LSMinimumSystemVersion</key>
    <string>10.15</string>
    <key>NSHighResolutionCapable</key>
    <true/>
    <key>LSUIElement</key>
    <false/>
    <key>NSAppleEventsUsageDescription</key>
    <string>Xclipy needs access to Apple Events to monitor clipboard changes and provide global hotkey functionality.</string>
    <key>NSSystemAdministrationUsageDescription</key>
    <string>Xclipy needs access to system administration to monitor clipboard changes across applications.</string>
    <key>ITSAppUsesNonExemptEncryption</key>
    <false/>
</dict>
</plist>
```

### 1.2 Create App Store Icons

- 16x16, 32x32, 64x64, 128x128, 256x256, 512x512, 1024x1024 pixels
- Save as PNG format
- Use `iconutil` to create .icns file

### 1.3 Update Entitlements

Create `Xclipy.entitlements`:

```xml
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
    <key>com.apple.security.app-sandbox</key>
    <true/>
    <key>com.apple.security.network.client</key>
    <true/>
    <key>com.apple.security.files.user-selected.read-write</key>
    <true/>
    <key>com.apple.security.automation.apple-events</key>
    <true/>
</dict>
</plist>
```

## Step 2: Code Signing Setup

### 2.1 Create Certificates in Xcode

1. Open Xcode → Preferences → Accounts
2. Add your Apple ID
3. Click "Manage Certificates"
4. Click "+" → "Apple Development" or "Apple Distribution"

### 2.2 Create Provisioning Profile

1. Go to [Apple Developer Portal](https://developer.apple.com)
2. Certificates, Identifiers & Profiles
3. Profiles → "+"
4. Select "Mac App Store"
5. Select your App ID
6. Select your certificate
7. Download and install the profile

## Step 3: Build for App Store

### 3.1 Update CMakeLists.txt

```cmake
# Add code signing
set_target_properties(Xclipy PROPERTIES
    MACOSX_BUNDLE TRUE
    MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/Info.plist"
    MACOSX_BUNDLE_ICON_FILE "${CMAKE_CURRENT_SOURCE_DIR}/resources/clipboard.icns"
    MACOSX_BUNDLE_GUI_IDENTIFIER "com.xclipy.app"
)

# Code signing
if(APPLE)
    set_target_properties(Xclipy PROPERTIES
        XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "Mac Developer"
        XCODE_ATTRIBUTE_DEVELOPMENT_TEAM "YOUR_TEAM_ID"
        XCODE_ATTRIBUTE_PROVISIONING_PROFILE "YOUR_PROFILE_NAME"
    )
endif()
```

### 3.2 Build Commands

```bash
mkdir build-appstore
cd build-appstore
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```

## Step 4: Create App Store Package

### 4.1 Create .pkg file

```bash
# Install pkgbuild
sudo xcode-select --install

# Create package
pkgbuild --component build/Xclipy.app \
         --install-location /Applications \
         --identifier com.xclipy.app \
         --version 1.0.0 \
         --scripts scripts/ \
         Xclipy.pkg
```

### 4.2 Create .dmg file

```bash
# Install create-dmg
brew install create-dmg

# Create DMG
create-dmg \
    --volname "Xclipy" \
    --window-pos 200 120 \
    --window-size 600 300 \
    --icon-size 100 \
    --icon "Xclipy.app" 175 120 \
    --hide-extension "Xclipy.app" \
    --app-drop-link 425 120 \
    "Xclipy-1.0.0.dmg" \
    "build/Xclipy.app"
```

## Step 5: App Store Connect Setup

### 5.1 Create App Record

1. Go to [App Store Connect](https://appstoreconnect.apple.com)
2. My Apps → "+" → New App
3. Fill in app information:
   - Name: Xclipy
   - Bundle ID: com.xclipy.app
   - SKU: xclipy-mac
   - User Access: Full Access

### 5.2 App Information

- **App Name**: Xclipy
- **Subtitle**: Cross Platform Clipboard Manager
- **Keywords**: clipboard,manager,productivity,hotkeys
- **Description**: A powerful clipboard manager with global hotkeys
- **Category**: Productivity
- **Content Rights**: Check if you have rights to use all content

### 5.3 Pricing and Availability

- **Price**: Free or set your price
- **Availability**: Select countries
- **Release Type**: Manual or Automatic

## Step 6: Submit for Review

### 6.1 Upload Build

1. In Xcode, select "Any Mac App Store (arm64)" scheme
2. Product → Archive
3. In Organizer, click "Distribute App"
4. Select "App Store Connect"
5. Upload your build

### 6.2 Submit for Review

1. In App Store Connect, go to your app
2. TestFlight → Builds → Select your build
3. Click "Submit to App Review"
4. Fill out review information
5. Submit

## Step 7: Review Process

### 7.1 Common Rejection Reasons

- **Accessibility Permissions**: Ensure clear explanation of why permissions are needed
- **Global Hotkeys**: Document how they work and why they're necessary
- **Clipboard Access**: Explain the legitimate use case
- **Sandboxing**: Ensure your app follows sandbox requirements

### 7.2 Review Timeline

- **Initial Review**: 1-3 days
- **Re-review** (if rejected): 1-2 days
- **Total Time**: Usually 1-5 days

## Step 8: Post-Launch

### 8.1 Monitor Performance

- App Store Connect Analytics
- User reviews and ratings
- Crash reports
- Performance metrics

### 8.2 Updates

- Submit updates through the same process
- Increment version numbers
- Test thoroughly before submission

## Troubleshooting

### Common Issues

1. **Code Signing Errors**: Check certificates and provisioning profiles
2. **Sandbox Violations**: Review entitlements and permissions
3. **Rejection**: Address Apple's feedback and resubmit
4. **Build Failures**: Check Xcode and CMake configurations

### Resources

- [Apple Developer Documentation](https://developer.apple.com/documentation/)
- [App Store Review Guidelines](https://developer.apple.com/app-store/review/guidelines/)
- [Mac App Programming Guide](https://developer.apple.com/macos/)

## Cost Breakdown

- **Apple Developer Program**: $99/year
- **App Store Commission**: 30% of sales (15% for small developers)
- **Development Tools**: Free (Xcode)
- **Testing Devices**: Free (use your own Mac)

## Timeline

- **Development**: 2-4 weeks
- **App Store Review**: 1-5 days
- **Total Time to Launch**: 3-5 weeks

This guide should help you successfully deploy Xclipy to the Mac App Store. Make sure to follow Apple's guidelines and test thoroughly before submission.
