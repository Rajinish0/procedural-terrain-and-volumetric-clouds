Thesis project.

## Build Instructions

- install `vcpkg` (add it to your path too)
- run `vcpkg integrate install`
- run `./install_dependencies.ps1`
- run `./build_for_vs.ps1`
- go to build, open .sln, build with vs.
- to run, you might need to reconfigure startup project in vs.

If you don't want to use the script you can do the following:
```shell
cmake .. -G " Visual Studio 17 2022 " -A x64 - DCMAKE_TOOLCHAIN_FILE =%PATH_TO_VCPKG%/scripts/buildsystems/vcpkg.cmake 
```
This way is better because your path to vcpkg could be different.

The other branch builds with MinGW (deprecated).

## Testing
- run `\build_with_tests.ps1`
- go to build, open .sln, build with vs.
- run the tests.

To manually build the project with testing included use the `-DBUILD_TESTS=TRUE` flag for cmake.

## Building on Linux/Mac:
- install dependencies which are listed in `./install_dependencies.ps1`.
- build with cmake, use the generator of your preference.

You might need to fiddle with the CMakeLists.txt to include the path to your package manager's include directories, for instance. I have not tested this on another OS yet.
