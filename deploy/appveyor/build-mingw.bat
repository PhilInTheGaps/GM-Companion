cd ..\..

qmake DEFINES-=NO_UPDATE_CHECK CONFIG+=qtquickcompiler CONFIG+=release
mingw32-make -j%NUMBER_OF_PROCESSORS%
