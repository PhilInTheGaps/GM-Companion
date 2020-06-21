cd ..\..

mkdir build
cd build

echo "Removing sh.exe from PATH ..."
set PATH=%PATH:C:\Program Files\Git\usr\bin;=%
echo %PATH%

cget init --shared -DCMAKE_SH="CMAKE_SH-NOTFOUND"
cget install pfultz2/pkgconfig
cget install -G "MinGW Makefiles" --release -DCMAKE_C_FLAGS="-fno-asynchronous-unwind-tables" -f ../requirements.txt || EXIT /B 1

cmake .. -G "MinGW Makefiles" -DCMAKE_INSTALL_PREFIX=install || EXIT /B 1

mingw32-make -j%NUMBER_OF_PROCESSORS% || EXIT /B 1
