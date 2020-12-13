# VS Code Extensions

- C/C++ (from Microsoft)
- CMake Tools
- Native Debug

# Building

## Basic Prerequisites

1. CMake 3.16+.
2. SDL2, including supplemental libraries: (platform-specific instructions for acquiring are given below)
   - SDL2_Image
3. C++ compiler that is highly compliant with the C++14 or higher standard.
   - Need at minimum `std::make_unique` support.

## Windows

### Get SDL2 

SDL2 version 2.0.10 is already version-controlled in this repo, so nothing further needs to be done. If a different version is desired or you simply want to redownload the libraries, keep reading.

1. Download the _MSVC (Visual C++)_ **development libraries** from the SDL2 [download page](https://www.libsdl.org/download-2.0.php) to an arbitrary location. MinGW is not currently supported.
2. Unzip the zip archive and copy the `lib`, `include` and `COPYING.txt` files/folders to `3rdParty/SDL2/Windows/`. This is the location in which CMake searches to find SDL2 on Windows.

#### TODO: SDL2_Image

### Get LuaJIT

LuaJIT binaries are already version-controlled but just in case you need to do this again, here are the steps.

1. Download the latest stable release ZIP of LuaJIT from [the website](https://luajit.org/download.html). At the time of writing, the version used was [2.0.5](https://luajit.org/download/LuaJIT-2.0.5.zip).
2. Extract the archive and open `src/msvcbuilt.bat` in your favourite text editor.
3. Append `/DLUAJIT_ENABLE_LUA52COMPAT` to the `@set LJCOMPILE=` line.
4. Open up a tools-enabled cmd prompt targeting either x86 or x64 in the `src` folder and run `msvcbuild.bat`.
  - For a 32-bit build (x86), use the "x64_x86 Cross Tools Command Prompt for Visual Studio" cmd prompt.
  - For a 64-bit build (x64), use the "x64 Native Tools Command Prompt for Visual Studio" cmd prompt.
5. Copy the lua51.dll, lua51.lib and lua51.exp files to `3rdParty/lua/Windows/lib/x64` folder (do the corresponding step for x86).
6. Copy the lua.h, lualib.h, lauxlib.h, luajit.h files to `3rdParty/lua/Windows/include`.

You should be good to go.

### Build & Run

From the project root, run: `cmake -B build`. If you want to use a specific MSVC IDE version, do `cmake -B build -G <VS Version>`, for example for VS 2017, `cmake -B build -G "Visual Studio 15 2017"`.

1. Open up the generated solution file `build\pen31ope.sln` in the correct Visual Studio IDE version that corresponds to the version you used to generate build files in the step above.
2. Right-click on the `pen31ope` entry in the Solution Explorer and set it as the default startup target.
3. Press F5.

## macOS

### Get SDL2

1. Download the  **development libraries** from the SDL2 [download page](https://www.libsdl.org/download-2.0.php) to an arbitrary location. This is normally a `.dmg` file.
2. Open the `.dmg` and copy the `SDL2.framework` package to your desired macOS frameworks directory. Normally, this would be `/Library/Frameworks` but user-specific locations like `~/Library/Frameworks` are fine as well.

#### TODO: SDL2_Image

1. Download the **development libraries** from the SDL2_Image [download page](https://www.libsdl.org/projects/SDL_image/) to an arbitrary location. This should also be a `.dmg` file.
2. Open the `.dmg` and copy the `SDL2_Image.framework` package to the same macOS frameworks directory as done earlier.

### Build

1. From the project root, run: `cmake -B build`.
2. Change to the `build` directory and run `make`.

### Run

1. If not already in `build` directory, change to it: `cd build`.
2. Run `./pen31ope [params]`.

## Linux (Ubuntu)

### Get SDL2

```
sudo apt-get install libsdl2-dev
```

### C++ compiler

As of time of writing, your default g++ compiler might be version 4.8.x which doesn't appear to have complete C++14 support, such as for `std::make_unique` which is used a lot in the project. It is recommended to use g++ version 5+ or clang version 3.4+.

To obtain g++ 5+, if your Ubuntu distro is too old you'll have to install it from a PPA.
```
sudo add-apt-repository ppa:ubuntu-toolchain-r/test
sudo apt-get update
sudo apt-get install gcc-5 g++-5

# To set first priority to gcc-5/g++-5
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-5 60 --slave /usr/bin/g++ g++ /usr/bin/g++-5
```

### Build

1. For GCC, run `cmake -B build`. For clang, run `CC=/usr/bin/clang CXX=/usr/bin/clang++ cmake -B build`.
2. Change to the `build` directory and run `make`.

### Run

1. If not already in `build` directory, change to it: `cd build`.
2. Run `./pen31ope [params]`.

