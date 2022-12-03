# Build:
cmake -B my_build\
cmake --build my_build --parallel <NumerOfParallelBuilds>

## Build requires
cmake 3.24 (not tested with older one)\
gcc-c++ 12.2.1 (not tested with olders)

###  Build Windows (See TODO)
cmake -B my_build -DCMAKE_TOOLCHAIN_FILE=/usr/share/mingw/toolchain-mingw64.cmake\
cmake --build my_build --parallel <NumerOfParallelBuilds>

#### Build requires
cmake 3.24 (not tested with older one)\
mingw64-gcc-c++ 11.2.1 (not tested with olders)
