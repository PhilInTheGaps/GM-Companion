cd ..\..

mkdir build
cd build

cget init --shared
cget install -G "MinGW Makefiles" --release -DCMAKE_C_FLAGS="-fno-asynchronous-unwind-tables" -f ../requirements.txt || EXIT /B 1

cmake .. -G "MinGW Makefiles" -DCMAKE_INSTALL_PREFIX=install || EXIT /B 1

mingw32-make -j%NUMBER_OF_PROCESSORS% || EXIT /B 1
