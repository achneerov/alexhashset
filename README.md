# Alex's Custom HashSet Implementation

A custom HashSet implementation in C with Python bindings, providing an alternative to Python's built-in set data structure.

## Features

- Custom C-based HashSet implementation
- Python bindings for easy use in Python code
- Basic set operations (add, remove, contains)
- Performance benchmarking tools

## Prerequisites

- Python 3.13 or later
- A C compiler (gcc/clang)
- Python development headers
- setuptools

For macOS:
```bash
brew install python@3.13
```

## Installation

1. Clone the repository:
```bash
git clone [your-repo-url]
cd [repo-name]
```

2. Build and install the package:
```bash
python -m pip install .
```

Or use the provided rebuild script:
```bash
python rebuild.py
```

## Usage

Basic usage in Python:
```python
from alexhashset import HashSet

# Create a new hashset
s = HashSet()

# Add elements
s.add(42)
s.add(100)

# Check if elements exist
print(s.contains(42))  # True
print(s.contains(101))  # False

# Remove elements
s.remove(42)
```

## Development

When making changes to the C code:

1. Edit the source files:
   - `hashset.h` - Header file with function declarations
   - `hashset.c` - Core HashSet implementation
   - `hashset_module.c` - Python bindings

2. Rebuild the package using rebuild.py:
```bash
python rebuild.py
```

## Performance Testing

Run the benchmark script to compare performance with Python's built-in set:
```bash
python benchmark.py
```

This will test both insertion and lookup operations with different data sizes.

## Project Structure

```
.
├── README.md
├── setup.py              # Build configuration
├── hashset.h            # C header file
├── hashset.c            # C implementation
├── hashset_module.c     # Python bindings
├── rebuild.py           # Build automation script
├── benchmark.py         # Performance testing
└── test.py             # Basic functionality tests
```

## Troubleshooting

### VSCode Configuration

If you're using VSCode and seeing include errors, ensure your `.vscode/c_cpp_properties.json` is configured correctly:

```json
{
    "configurations": [
        {
            "name": "Mac",
            "includePath": [
                "${workspaceFolder}/**",
                "/opt/homebrew/opt/python@3.13/Frameworks/Python.framework/Versions/3.13/include/python3.13"
            ],
            "defines": [],
            "macFrameworkPath": [
                "/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/System/Library/Frameworks"
            ],
            "compilerPath": "/usr/bin/clang",
            "cStandard": "c17",
            "cppStandard": "c++17",
            "intelliSenseMode": "macos-clang-arm64"
        }
    ],
    "version": 4
}
```

### Common Issues

1. **Python.h not found**: Make sure you have Python development headers installed and the include path is correct in your setup.py.

2. **Build fails**: Run the rebuild script with verbose output:
```bash
python rebuild.py --verbose
```

3. **Import errors**: Verify the installation location:
```python
import alexhashset
print(alexhashset.__file__)
```

## License

MIT

## Contributing

Message me!