# Contributing to ClipboardManager

Thank you for your interest in contributing to ClipboardManager! This document provides guidelines and information for contributors.

## Getting Started

### Prerequisites
- Qt 5.12 or later (Qt 6.x recommended)
- C++17 compatible compiler
- Git

### Setting up the development environment

1. **Fork and clone the repository**
   ```bash
   git clone https://github.com/your-username/ClipboardManager.git
   cd ClipboardManager
   ```

2. **Install dependencies**
   ```bash
   # macOS
   brew install qt cmake
   
   # Ubuntu/Debian
   sudo apt-get install qt5-default qtbase5-dev qtchooser qt5-qmake build-essential cmake
   
   # Windows
   # Download Qt from https://www.qt.io/download
   ```

3. **Build the project**
   ```bash
   chmod +x scripts/*.sh
   ./scripts/dev.sh build
   ```

## Development Workflow

### Code Style Guidelines

- Follow Qt coding conventions
- Use meaningful variable and function names
- Add comments for complex logic
- Keep functions focused and small
- Use consistent indentation (4 spaces)
- Prefer `const` references for parameters
- Use `nullptr` instead of `NULL`

### File Organization

- Source files go in `src/`
- Header files go in `include/`
- Resources go in `resources/`
- Scripts go in `scripts/`
- Documentation goes in `docs/`

### Making Changes

1. **Create a feature branch**
   ```bash
   git checkout -b feature/your-feature-name
   ```

2. **Make your changes**
   - Follow the code style guidelines
   - Add tests if applicable
   - Update documentation if needed

3. **Test your changes**
   ```bash
   ./scripts/dev.sh run
   ```

4. **Commit your changes**
   ```bash
   git add .
   git commit -m "Add feature: brief description"
   ```

5. **Push and create a pull request**
   ```bash
   git push origin feature/your-feature-name
   ```

### Commit Message Guidelines

Use clear, descriptive commit messages:

- Use present tense ("Add feature" not "Added feature")
- Use imperative mood ("Move cursor to..." not "Moves cursor to...")
- Limit the first line to 72 characters
- Reference issues and pull requests after the first line

Examples:
```
Add search functionality to history window

- Implement real-time search filtering
- Add search box with placeholder text
- Update UI to show filtered results

Fixes #123
```

## Testing

### Running Tests
```bash
./scripts/dev.sh test
```

### Manual Testing Checklist
- [ ] Application starts without errors
- [ ] Clipboard monitoring works correctly
- [ ] History window displays properly
- [ ] Search functionality works
- [ ] Settings are saved and loaded
- [ ] System tray integration works
- [ ] Application quits cleanly

## Documentation

### Code Documentation
- Document all public APIs
- Use Doxygen-style comments for complex functions
- Keep README.md up to date
- Update CONTRIBUTING.md when needed

### User Documentation
- Update user-facing documentation for new features
- Include screenshots for UI changes
- Document any new configuration options

## Pull Request Process

1. **Ensure your code follows the style guidelines**
2. **Add tests for new functionality**
3. **Update documentation**
4. **Ensure all tests pass**
5. **Create a pull request with a clear description**

### Pull Request Template
```markdown
## Description
Brief description of the changes

## Type of Change
- [ ] Bug fix
- [ ] New feature
- [ ] Breaking change
- [ ] Documentation update

## Testing
- [ ] Manual testing completed
- [ ] Unit tests added/updated
- [ ] All tests pass

## Checklist
- [ ] Code follows style guidelines
- [ ] Documentation updated
- [ ] No new warnings
- [ ] Self-review completed
```

## Reporting Issues

### Bug Reports
When reporting bugs, please include:
- Operating system and version
- Qt version
- Steps to reproduce
- Expected behavior
- Actual behavior
- Error messages or logs

### Feature Requests
When requesting features, please include:
- Description of the feature
- Use case/justification
- Proposed implementation (if applicable)

## Getting Help

- Check existing issues and pull requests
- Search the documentation
- Ask questions in discussions
- Join our community chat (if available)

## License

By contributing to ClipboardManager, you agree that your contributions will be licensed under the MIT License.

Thank you for contributing to ClipboardManager!
