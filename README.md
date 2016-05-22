
### LLVM Hello World

This repository is the testing bed for a Hello World example of LLVM. This repository
also contains the steps for installing LLVM on a Mac OS, which seems like a daunting
task at first.

### Specifications

These are the following specifications regaurding the install.
  - Assuming Mac OSX 10.10 or greater is installed.
  - LLVM v3.8.0 will be installed.
  - OSX SDK version 10.10 is assumed to be installed (install steps provided if needed).

### Installing LLVM

1. Download the [prebuilt LLVM binaries](http://llvm.org/releases/3.8.0/clang+llvm-3.8.0-x86_64-apple-darwin.tar.xz).
2. Extract the contents of the download to any folder location.
3. Create the following folder: `/usr/local/llvm`.
4. Copy the extracted contents into the newly created llvm folder.
5. Add the `/usr/local/llvm/bin` path to your shell profile with the following: 
  
  ```bash
  export PATH = "$PATH:/usr/local/llvm/bin"
  ``` 
6. Next, reload your user profile by running `source ~/.bashrc`.

### Trying it out

To try and build this test application, run the make command.
If make complains about a file (see below) as missing/skipped you must install the correct SDK.
```
/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.10.sdk
```
To install the missing SDK, clone the [SDK Repository](https://github.com/phracker/MacOSX-SDKs) and change directories into the newly cloned folder. Next, run the following command to install it to the appropriate location:

```bash
sudo cp -r MacOSX10.10.sdk /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.10.sdk
```

Now change back to the test repository folder and try make again. Hopefully this time it will build!
