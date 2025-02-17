import subprocess
import shutil
import os
import sys

def rebuild():
    print("🧹 Cleaning old build files...")
    # Remove old build directories
    dirs_to_remove = ['build', 'alexhashset.egg-info']
    for dir_name in dirs_to_remove:
        if os.path.exists(dir_name):
            shutil.rmtree(dir_name)
            print(f"   Removed {dir_name}/")

    print("\n🔨 Building and installing package...")
    try:
        # Run pip install
        subprocess.run([sys.executable, '-m', 'pip', 'install', '.'], check=True)
        print("\n✅ Build successful!")
        
        # Test import
        print("\n🧪 Testing import...")
        import alexhashset
        print(f"   Module location: {alexhashset.__file__}")
        
        # Quick functionality test
        s = alexhashset.HashSet()
        s.add(42)
        assert s.contains(42), "Basic functionality test failed!"
        print("   Basic functionality test passed!")
        
    except subprocess.CalledProcessError as e:
        print(f"\n❌ Build failed with error code {e.returncode}")
        sys.exit(1)
    except ImportError:
        print("\n❌ Build succeeded but module import failed!")
        sys.exit(1)
    except AssertionError:
        print("\n❌ Build succeeded but functionality test failed!")
        sys.exit(1)

if __name__ == "__main__":
    rebuild()