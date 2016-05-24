### LLVM Hello World

[![Build Status](https://travis-ci.org/tvand7093/LLVM-HelloWorld.svg?branch=master)](https://travis-ci.org/tvand7093/LLVM-HelloWorld)

This repository is the testing bed for a Hello World example of LLVM. This repository
also contains the steps for installing LLVM on a Mac OS and Ubuntu, which seems like a daunting
task at first.

### Specifications

These are the following specifications regaurding the install.
  - Assuming Mac OSX 10.10 or greater is installed or Ubuntu 14.04.
  - LLVM v3.8.0 will be installed.
  - (OSX Only) OSX SDK version 10.10 is assumed to be installed (install steps provided if needed).
  - (Ubuntu Only) zlib library is installed.

### Installing LLVM

1. Download the [prebuilt binaries](http://llvm.org/releases/download.html#3.8.0) for the appropriate platform.
2. Extract the contents of the download to any folder location.
3. Create the following folder: `/usr/local/llvm`.
4. Copy the extracted contents into the newly created llvm folder.
5. Add the `/usr/local/llvm/bin` as well as make sure that the default compiler is clang using the following: 
  
  ```bash
  export PATH = "$PATH:/usr/local/llvm/bin"
  # If on Ubuntu, you need to set clang as the default compiler. g++ will not do!
  export CXX = clang++
  ``` 
6. Next, reload your user profile by running `source ~/.bashrc`.

### Running the application.

To try and build this test application, run the make command. This should output a binary file called `hello-llvm` that you can run and should 
print the value: `value = 3`.

### Inspecting simple LLVM IR
If you simply want to see what a small program looks like in IR, run `make test`. This will dump the IR created for the `bitcode-test.cpp` file. The output file name will be called `bitcode-test.bc`, just open in a text editor and browse!

### Troubleshooting (OSX Only)

If make complains about a file (see below) as missing/skipped you must install the correct SDK.
```
/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.10.sdk
```
To install the missing SDK, clone the [SDK Repository](https://github.com/phracker/MacOSX-SDKs) and change directories into the newly cloned folder. Next, run the following command to install it to the appropriate location:

```bash
sudo cp -r MacOSX10.10.sdk /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.10.sdk
```

Now change back to the test repository folder and try make again. Hopefully this time it will build!
