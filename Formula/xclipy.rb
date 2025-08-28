class Xclipy < Formula
  desc "Cross Platform Clipboard Manager"
  homepage "https://github.com/yourusername/xclipy"
  url "https://github.com/yourusername/xclipy/releases/download/v1.0.0/xclipy-1.0.0-macOS.tar.gz"
  sha256 "YOUR_SHA256_HERE"
  license "MIT"
  
  depends_on "qt@6"
  
  def install
    bin.install "Xclipy"
    
    # Create app bundle
    app = prefix/"Xclipy.app"
    app.mkpath
    
    # Copy executable to app bundle
    (app/"Contents/MacOS").install bin/"Xclipy"
    
    # Create Info.plist
    (app/"Contents").install "Info.plist"
    
    # Create PkgInfo
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
    system "#{bin}/Xclipy", "--version"
  end
end
