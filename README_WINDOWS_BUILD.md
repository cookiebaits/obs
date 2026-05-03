# How to Build the OBS Studio Windows Installer (.exe)

This repository has been configured to natively produce an `.exe` installer using CPack and NSIS.

To generate the `.exe` installer locally on a Windows machine, ensure you have the following prerequisites installed:
1. **Visual Studio 2022** (with C++ Desktop Development tools)
2. **CMake** (v3.24+)
3. **NSIS** (Nullsoft Scriptable Install System) for installer generation.

### Build Instructions

Open a Command Prompt or PowerShell terminal in the root of the cloned repository and execute the following commands:

```cmd
# Create a build directory
mkdir build
cd build

# Configure the project for Windows using the Visual Studio generator
cmake -G "Visual Studio 17 2022" -A x64 ..

# Build the project in Release mode
cmake --build . --config Release --parallel

# Generate the .exe installer using CPack
cpack -G NSIS -C Release
```

After CPack finishes running, the `obs-studio-30.0.0-modified-windows.exe` file will be generated and available in your `build` directory.
