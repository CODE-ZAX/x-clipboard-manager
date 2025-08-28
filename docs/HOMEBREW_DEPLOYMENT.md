# Homebrew Deployment Guide for Xclipy

This guide covers the steps required to deploy Xclipy to Homebrew, making it easily installable for macOS users.

## Prerequisites

1. **GitHub Repository** with Xclipy source code
2. **GitHub Releases** for versioned downloads
3. **Homebrew** installed on your system
4. **Ruby** knowledge (basic)

## Step 1: Prepare Your Repository

### 1.1 Create GitHub Releases

1. Go to your GitHub repository
2. Click "Releases" → "Create a new release"
3. Tag version: `v1.0.0`
4. Release title: `Xclipy v1.0.0`
5. Description: Release notes
6. Upload assets:
   - `xclipy-1.0.0-macOS.tar.gz` (source tarball)
   - `Xclipy-1.0.0-macOS.dmg` (macOS app bundle)

### 1.2 Source Tarball Structure

```bash
# Create source tarball
tar -czf xclipy-1.0.0-source.tar.gz \
    --exclude=build \
    --exclude=.git \
    --exclude=*.o \
    --exclude=*.app \
    --exclude=Formula \
    .
```

## Step 2: Create Homebrew Formula

### 2.1 Basic Formula Structure

```ruby
class Xclipy < Formula
  desc "Cross Platform Clipboard Manager"
  homepage "https://github.com/yourusername/xclipy"
  url "https://github.com/yourusername/xclipy/releases/download/v1.0.0/xclipy-1.0.0-source.tar.gz"
  sha256 "YOUR_SHA256_HERE"
  license "MIT"

  depends_on "qt@6"

  def install
    # Build instructions
  end

  def caveats
    # Installation notes
  end

  test do
    # Test commands
  end
end
```

### 2.2 Complete Formula

```ruby
class Xclipy < Formula
  desc "Cross Platform Clipboard Manager"
  homepage "https://github.com/yourusername/xclipy"
  url "https://github.com/yourusername/xclipy/releases/download/v1.0.0/xclipy-1.0.0-source.tar.gz"
  sha256 "YOUR_SHA256_HERE"
  license "MIT"

  depends_on "qt@6"
  depends_on "cmake" => :build

  def install
    # Create build directory
    mkdir "build" do
      system "cmake", "..", *std_cmake_args
      system "make", "install"
    end

    # Create app bundle
    app = prefix/"Xclipy.app"
    app.mkpath

    # Copy executable
    (app/"Contents/MacOS").install bin/"xclipy"

    # Copy Info.plist and PkgInfo
    (app/"Contents").install "Info.plist"
    (app/"Contents").install "PkgInfo"

    # Copy resources
    (app/"Contents/Resources").install Dir["resources/*"]

    # Create symlink in Applications
    prefix.install_symlink app => "../Applications/Xclipy.app"
  end

  def caveats
    <<~EOS
      Xclipy has been installed as a macOS application.

      To use global hotkeys, you may need to grant accessibility permissions:
      1. Go to System Preferences > Security & Privacy > Privacy > Accessibility
      2. Add Xclipy to the list of allowed applications

      You can also launch Xclipy from Applications folder or use:
        open /Applications/Xclipy.app
    EOS
  end

  test do
    system "#{bin}/xclipy", "--version"
  end
end
```

## Step 3: Calculate SHA256

### 3.1 Download and Calculate Hash

```bash
# Download your release tarball
curl -L -o xclipy-1.0.0-source.tar.gz \
  "https://github.com/yourusername/xclipy/releases/download/v1.0.0/xclipy-1.0.0-source.tar.gz"

# Calculate SHA256
shasum -a 256 xclipy-1.0.0-source.tar.gz
```

### 3.2 Update Formula

Replace `YOUR_SHA256_HERE` with the actual SHA256 hash.

## Step 4: Test Your Formula

### 4.1 Local Testing

```bash
# Test formula locally
brew install --build-from-source ./Formula/xclipy.rb

# Test uninstall
brew uninstall xclipy
```

### 4.2 Fix Issues

Common issues and solutions:

- **Build failures**: Check dependencies and build commands
- **Installation errors**: Verify paths and permissions
- **Runtime errors**: Check executable paths and resources

## Step 5: Submit to Homebrew

### 5.1 Fork Homebrew Core

1. Go to [homebrew-core](https://github.com/Homebrew/homebrew-core)
2. Click "Fork"
3. Clone your fork locally

### 5.2 Add Your Formula

```bash
# Clone your fork
git clone https://github.com/yourusername/homebrew-core.git
cd homebrew-core

# Add your formula
cp /path/to/your/Formula/xclipy.rb Formula/

# Commit and push
git add Formula/xclipy.rb
git commit -m "xclipy 1.0.0 (new formula)"
git push origin master
```

### 5.3 Create Pull Request

1. Go to your fork on GitHub
2. Click "Compare & pull request"
3. Fill in PR description:
   ```
   - [x] Have you followed the [guidelines for contributing](https://github.com/Homebrew/homebrew-core/blob/master/CONTRIBUTING.md)?
   - [x] Have you checked that there aren't other open [pull requests](https://github.com/Homebrew/homebrew-core/pulls) for the same formula update/change?
   - [x] Have you built your formula locally with `brew install --build-from-source <formula>`, where `<formula>` is the name of the formula you're submitting?
   - [x] Is your test running fine `brew test <formula>`, where `<formula>` is the name of the formula you're submitting?
   - [x] Does your build pass `brew audit --strict <formula>` (after doing `brew install <formula>`)?
   ```

## Step 6: Alternative: Homebrew Tap

### 6.1 Create Personal Tap

If you prefer not to submit to homebrew-core, create your own tap:

```bash
# Create tap repository
mkdir homebrew-xclipy
cd homebrew-xclipy

# Initialize git
git init
git remote add origin https://github.com/yourusername/homebrew-xclipy.git

# Add formula
mkdir Formula
cp /path/to/your/Formula/xclipy.rb Formula/

# Commit and push
git add .
git commit -m "Add Xclipy formula"
git push -u origin master
```

### 6.2 Install from Tap

```bash
# Add your tap
brew tap yourusername/xclipy

# Install Xclipy
brew install xclipy
```

## Step 7: Maintain Your Formula

### 7.1 Update for New Versions

1. Update URL in formula
2. Calculate new SHA256
3. Test locally
4. Submit PR or update tap

### 7.2 Monitor Issues

- Watch for user reports
- Fix build issues
- Update dependencies
- Maintain compatibility

## Step 8: Best Practices

### 8.1 Formula Guidelines

- Follow [Homebrew formula guidelines](https://docs.brew.sh/Formula-Cookbook)
- Use standard dependencies when possible
- Provide clear installation instructions
- Include helpful caveats

### 8.2 Testing

- Test on multiple macOS versions
- Verify all dependencies work
- Test installation and uninstallation
- Verify app functionality

## Troubleshooting

### Common Issues

1. **Build Failures**: Check Qt6 installation and CMake configuration
2. **Permission Errors**: Ensure proper file permissions
3. **Resource Missing**: Verify all resources are included
4. **Dependency Issues**: Check Homebrew dependencies

### Resources

- [Homebrew Documentation](https://docs.brew.sh/)
- [Formula Cookbook](https://docs.brew.sh/Formula-Cookbook)
- [Contributing Guidelines](https://github.com/Homebrew/homebrew-core/blob/master/CONTRIBUTING.md)

## Timeline

- **Formula Creation**: 1-2 hours
- **Local Testing**: 2-4 hours
- **PR Submission**: 1 day
- **Review Process**: 1-7 days
- **Total Time**: 1-2 weeks

## Benefits of Homebrew Distribution

1. **Easy Installation**: `brew install xclipy`
2. **Automatic Updates**: `brew upgrade xclipy`
3. **Dependency Management**: Homebrew handles Qt6 and other deps
4. **Wide Distribution**: Available to all Homebrew users
5. **Professional Credibility**: Official Homebrew formula

This guide should help you successfully deploy Xclipy to Homebrew, making it easily accessible to macOS users worldwide.
