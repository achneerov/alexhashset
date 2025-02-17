from setuptools import setup, Extension
import os

# Homebrew Python 3.13 include path
python_include = "/opt/homebrew/opt/python@3.13/Frameworks/Python.framework/Versions/3.13/include/python3.13"

# Verify the path exists
if not os.path.exists(python_include):
    raise RuntimeError(f"Python include path not found: {python_include}")

module = Extension('alexhashset',  # Changed name here
                  sources=['hashset.c', 'hashset_module.c'],
                  include_dirs=[python_include],
                  extra_compile_args=['-Wall', '-fPIC'])

setup(name='alexhashset',  # And here
      version='1.0',
      description='Alex\'s custom hashset implementation in C',
      ext_modules=[module])