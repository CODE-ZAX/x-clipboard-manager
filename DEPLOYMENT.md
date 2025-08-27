# 🚀 ClipboardManager Deployment Guide

This guide provides step-by-step instructions for deploying ClipboardManager to production and setting up version control.

## 📋 **Prerequisites**

- Qt 6.2+ installed
- Git installed
- GitHub account (for repository hosting)
- macOS, Windows, or Linux development environment

## 🔧 **Production Build Steps**

### **Step 1: Verify Build Environment**
```bash
# Check Qt installation
qmake --version

# Check build tools
make --version
gcc --version  # or clang --version
```

### **Step 2: Build Production Version**
```bash
# Clean and build
./scripts/build.sh

# Verify build success
ls -la build/ClipboardManager
```

### **Step 3: Create Distribution Packages**
```bash
# Create packages for all platforms
./scripts/package.sh

# Check created packages
ls -la packages/
```

## 📦 **Package Contents**

The packaging script creates:
- **macOS**: `ClipboardManager-{version}-macOS.zip`
- **Linux**: `ClipboardManager-{version}-Linux.tar.gz`
- **Windows**: `ClipboardManager-{version}-Windows.zip`

Each package contains:
- Executable application
- Documentation (README, LICENSE)
- Resources (icons, styles)
- Build configuration files

## 🐙 **Git Repository Setup**

### **Step 1: Initialize Git Repository**
```bash
# Initialize Git repository
git init

# Add all source files (excluding build artifacts)
git add .

# Create initial commit
git commit -m "Initial commit: ClipboardManager v1.2.0"
```

### **Step 2: Configure Git User**
```bash
# Set your name and email
git config --global user.name "Your Name"
git config --global user.email "your.email@example.com"
```

### **Step 3: Create GitHub Repository**

1. **Go to GitHub.com** and sign in
2. **Click "New repository"**
3. **Repository settings:**
   - Name: `ClipboardManager`
   - Description: `Cross-platform clipboard manager with global hotkey support`
   - Visibility: Public or Private
   - Initialize with: None (we'll push existing code)
4. **Click "Create repository"**

### **Step 4: Connect and Push to GitHub**
```bash
# Add remote repository
git remote add origin https://github.com/YOUR_USERNAME/ClipboardManager.git

# Push to GitHub
git push -u origin main
```

## 🌐 **GitHub Repository Configuration**

### **Step 1: Enable GitHub Actions**
The repository includes CI/CD configuration in `.github/workflows/ci.yml`:
- **Automatic builds** on push to main branch
- **Cross-platform testing** (Ubuntu, macOS)
- **Release creation** for tagged versions

### **Step 2: Configure Repository Settings**
1. **Go to Settings → Pages**
   - Source: GitHub Actions
   - This will enable automatic documentation deployment

2. **Go to Settings → Actions → General**
   - Actions permissions: Allow all actions
   - Workflow permissions: Read and write permissions

### **Step 3: Set Up Branch Protection (Optional)**
1. **Go to Settings → Branches**
2. **Add rule for `main` branch:**
   - Require pull request reviews
   - Require status checks to pass
   - Include administrators

## 🏷️ **Version Management**

### **Creating a Release**
```bash
# Create and push a tag
git tag -a v1.2.0 -m "Release version 1.2.0"
git push origin v1.2.0

# GitHub Actions will automatically create a release
```

### **Version Bumping**
```bash
# Update version in files
# - README.md
# - ClipboardManager.pro
# - CMakeLists.txt

# Commit version bump
git add .
git commit -m "Bump version to 1.2.1"
git push origin main
```

## 📤 **Distribution Methods**

### **Method 1: GitHub Releases**
1. **Create a release on GitHub**
2. **Upload package files** to the release
3. **Users can download** directly from GitHub

### **Method 2: Homebrew (macOS)**
```bash
# Create Homebrew formula
# File: Formula/clipboard-manager.rb
class ClipboardManager < Formula
  desc "Cross-platform clipboard manager with global hotkey support"
  homepage "https://github.com/YOUR_USERNAME/ClipboardManager"
  url "https://github.com/YOUR_USERNAME/ClipboardManager/releases/download/v1.2.0/ClipboardManager-1.2.0-macOS.zip"
  sha256 "YOUR_SHA256_HASH"
  
  depends_on "qt@6"
  
  def install
    bin.install "ClipboardManager"
  end
end
```

### **Method 3: Package Managers**
- **macOS**: Homebrew, MacPorts
- **Linux**: Snap, Flatpak, AppImage
- **Windows**: Chocolatey, Scoop

## 🔒 **Security Considerations**

### **Code Signing (macOS)**
```bash
# Sign the application
codesign --force --deep --sign "Developer ID Application: Your Name" build/ClipboardManager.app

# Verify signature
codesign --verify --verbose build/ClipboardManager.app
```

### **Notarization (macOS)**
```bash
# Notarize for distribution
xcrun altool --notarize-app --primary-bundle-id "com.yourcompany.clipboardmanager" \
  --username "your-apple-id@example.com" --password "@keychain:AC_PASSWORD" \
  --file build/ClipboardManager.app.zip
```

## 📊 **Monitoring and Analytics**

### **GitHub Insights**
- **Traffic**: Repository views and clones
- **Contributors**: Code contributions
- **Issues**: Bug reports and feature requests

### **Application Analytics (Optional)**
Consider adding:
- Usage statistics (anonymized)
- Error reporting
- Performance metrics

## 🚀 **Deployment Checklist**

### **Pre-Deployment**
- [ ] All tests pass
- [ ] Build succeeds on all platforms
- [ ] Documentation is up to date
- [ ] Version numbers are consistent
- [ ] License and copyright are correct

### **GitHub Setup**
- [ ] Repository created
- [ ] Code pushed to GitHub
- [ ] GitHub Actions enabled
- [ ] Branch protection configured
- [ ] Release created

### **Distribution**
- [ ] Packages created for all platforms
- [ ] Release notes written
- [ ] Downloads tested
- [ ] Installation instructions verified

### **Post-Deployment**
- [ ] Monitor for issues
- [ ] Respond to user feedback
- [ ] Plan next release
- [ ] Update documentation as needed

## 🆘 **Troubleshooting**

### **Build Issues**
```bash
# Clean build
./scripts/dev.sh clean
./scripts/build.sh

# Check Qt installation
qmake --version
```

### **Git Issues**
```bash
# Reset repository
git reset --hard HEAD
git clean -fd

# Re-add files
git add .
git commit -m "Fix: Re-add files"
```

### **Package Issues**
```bash
# Recreate packages
rm -rf packages/
./scripts/package.sh
```

## 📞 **Support**

For deployment issues:
1. Check the [GitHub Issues](https://github.com/YOUR_USERNAME/ClipboardManager/issues)
2. Review the [CONTRIBUTING.md](CONTRIBUTING.md) guide
3. Create a new issue with detailed information

---

**Happy Deploying! 🎉**
