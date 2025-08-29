# CI/CD Setup Documentation

## Overview

This document describes the GitHub Actions CI/CD pipeline for Xclipy that automatically builds and deploys releases when code is pushed to the main branch.

## Workflow Features

### 🔄 **Automated Pipeline**

- **Triggers on**: Push to `main` or `develop` branches, Pull Requests to `main`
- **Platforms**: macOS (Universal) and Linux builds
- **Qt Versions**: Tests against Qt 6.2 and 6.5, deploys with Qt 6.9.2
- **Release**: Automatic GitHub release creation with distribution files

### 🏗️ **Build Process**

#### **1. Test Phase**

- Runs on both Ubuntu and macOS
- Tests multiple Qt versions (6.2, 6.5)
- Validates both qmake and CMake builds
- Must pass before deployment

#### **2. macOS Universal Build**

- Only runs on `main` branch pushes
- Creates Universal binaries (Intel + Apple Silicon)
- Uses your existing `deploy.sh` script
- Generates DMG and ZIP distributions
- Includes source tarball

#### **3. Linux Build**

- Only runs on `main` branch pushes
- Creates portable Linux binary
- Generates compressed tarball

#### **4. Release Creation**

- Automatically creates GitHub release
- Uses version from `CMakeLists.txt`
- Extracts changelog from `CHANGELOG.md`
- Uploads all distribution files
- Updates existing releases if version exists

## File Structure

```
.github/workflows/ci.yml    # Main CI/CD pipeline
scripts/deploy.sh           # Universal macOS build script (CI-compatible)
scripts/test-ci.sh          # Local testing script
docs/CI_CD_SETUP.md         # This documentation
```

## How It Works

### **Version Detection**

The pipeline automatically extracts the version from your `CMakeLists.txt`:

```cmake
set(XCLIPY_VERSION "1.1.0")
```

### **Universal macOS Builds**

The workflow uses your existing `deploy.sh` script which:

1. Builds separate ARM64 and x86_64 binaries
2. Merges them with `lipo` into a universal binary
3. Creates proper app bundle with resources
4. Generates DMG using `create-dmg`
5. Falls back to ZIP if DMG creation fails

### **Changelog Integration**

The workflow automatically extracts release notes from `CHANGELOG.md`:

- Looks for section matching current version
- Includes content in GitHub release description
- Falls back to generic description if no changelog found

### **Release Management**

- Creates releases with tag format: `v{version}` (e.g., `v1.1.0`)
- Overwrites existing releases if version already exists
- Marks releases as "latest" automatically
- Includes installation instructions

## Distribution Files

Each release includes:

### **macOS (Universal)**

- `Xclipy-{version}-macOS-universal.dmg` - Installer with drag-drop interface
- `Xclipy-{version}-macOS-universal.zip` - Portable archive (fallback)

### **Linux**

- `xclipy-{version}-Linux.tar.gz` - Portable binary archive

### **Source**

- `xclipy-{version}-source.tar.gz` - Complete source distribution

## Requirements

### **Repository Secrets**

- `GITHUB_TOKEN` - Automatically provided by GitHub Actions
- No additional secrets required

### **Repository Permissions**

The workflow needs:

- **Contents**: Write (for creating releases)
- **Metadata**: Read (for repository access)

## Local Testing

Test the CI/CD pipeline locally before pushing:

```bash
# Run local CI tests
./scripts/test-ci.sh

# Test universal build manually
./scripts/deploy.sh
```

## Triggering a Release

### **Method 1: Direct Push**

```bash
# Update version in CMakeLists.txt
# Update CHANGELOG.md with new version
git add -A
git commit -m "Release v1.2.0: New features and improvements"
git push origin main
```

### **Method 2: Manual Release**

```bash
# Create and push tag manually
git tag -a v1.2.0 -m "Release v1.2.0"
git push origin v1.2.0
```

## Configuration

### **Customizing Qt Version**

Edit `.github/workflows/ci.yml`:

```yaml
- name: Install Qt 6.9
  uses: jurplel/install-qt-action@v3
  with:
    version: "6.9.2" # Change this version
```

### **Customizing Build Matrix**

Edit the test matrix:

```yaml
strategy:
  matrix:
    os: [ubuntu-latest, macos-latest]
    qt-version: ["6.2", "6.5"] # Add/remove versions
```

### **Customizing Release Notes**

The workflow automatically generates release notes, but you can customize the template in the workflow file.

## Troubleshooting

### **Build Failures**

1. Check the Actions tab in your GitHub repository
2. Look for specific error messages in failed jobs
3. Test locally with `./scripts/test-ci.sh`

### **Missing Distribution Files**

- Ensure `deploy.sh` has execute permissions
- Check for Qt installation issues in CI logs
- Verify `create-dmg` is installed on macOS runners

### **Release Not Created**

- Ensure you're pushing to `main` branch
- Check repository permissions
- Verify version format in `CMakeLists.txt`

### **Universal Build Issues**

- Check that both ARM64 and x86_64 builds succeed
- Verify `lipo` command availability
- Ensure app bundle structure is correct

## Monitoring

### **Build Status**

- Check the "Actions" tab in your GitHub repository
- Each push to `main` will trigger the full pipeline
- Look for green checkmarks or red X's

### **Release Assets**

- Releases appear in the "Releases" section
- Each release should have 3-4 attached files
- Download counts are tracked automatically

## Security

### **Code Signing**

The current setup creates unsigned binaries. For production releases, consider:

- macOS: Apple Developer ID signing
- Windows: Code signing certificates
- Linux: GPG signatures

### **Notarization**

For macOS App Store or Gatekeeper compatibility:

- Add notarization steps to the workflow
- Requires Apple Developer account

## Performance

### **Build Times**

- macOS Universal: ~10-15 minutes
- Linux: ~5-10 minutes
- Total pipeline: ~20-30 minutes

### **Optimization**

- Qt installation is cached between runs
- Parallel builds use all available CPU cores
- Artifacts are compressed for faster uploads

## Future Enhancements

Potential improvements:

- Windows builds with Qt
- AppImage creation for Linux
- Automatic Homebrew formula updates
- Code signing and notarization
- Performance benchmarking
- Automated testing
