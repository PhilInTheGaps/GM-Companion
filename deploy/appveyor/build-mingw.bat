cd ..\..

qmake DEFINES-=NO_UPDATE_CHECK CONFIG+=qtquickcompiler CONFIG+=release || EXIT /B 1
mingw32-make -j%NUMBER_OF_PROCESSORS% || EXIT /B 1
