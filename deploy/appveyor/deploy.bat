cd ..\build

mingw32-make install

move cget\*\*.dll install\bin\
copy /y "%OPENSSL%\bin\*eay32.dll" install\bin
copy /y "%OPENSSL%\bin\lib*.dll" install\bin

windeployqt.exe -xml --release --qmldir %QT_PATH%\qml install\bin\gm-companion.exe || EXIT /B 1

echo "Downloading librespot ..."
curl -L -o install\bin\librespot.exe https://github.com/PhilInTheGaps/librespot-bin/releases/download/v0.1.1/librespot-x86_64.exe || EXIT /B 1

xcopy install\bin ..\deploy\installer\packages\lol.rophil.gm-companion\data /s /e /i