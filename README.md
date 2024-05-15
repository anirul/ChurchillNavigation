# ChurchillNavigation
Churchill Navigation test

## How to build

It use vcpkg to import `gtest` and other important subprojects.

```pwsh
mkdir build
cd build
cmake .. --DCMAKE_TOOLCHAIN_FILE="..\..\vcpkg\scripts\buildsystems\vcpkg.cmake"
cmake --build .
```

You should get a `.dll` and a test `.exe`. 

## How to test

You can execute the test that are in the `churchill_test` directory. 
There is also a simple way to test the `.dll` by using the `point_search` and passing it the path to the `.dll`.

```pwsh
cd ./build
./churchill_exe/Debug/point_search.exe ./churchill_library/Debug/churchill_library.dll
```