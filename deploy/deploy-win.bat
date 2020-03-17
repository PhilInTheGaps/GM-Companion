cd ..

mkdir release\deploy

copy /y src\gm-companion.exe release\deploy

windeployqt.exe --release --qmldir %QT_PATH%\qml release\deploy\gm-companion.exe

copy /y "%OPENSSL%\bin\*eay32.dll" release\deploy

echo "Downloading librespot ..."
curl -L -o release\deploy\librespot.exe https://github.com/PhilInTheGaps/librespot-bin/releases/download/v0.1.1/librespot-x86_64.exe

xcopy lib\poppler\bin release\deploy /s /e
xcopy lib\taglib\bin release\deploy /s /e
xcopy lib\qt5keychain\bin release\deploy /s /e

xcopy release\deploy deploy\installer\packages\lol.rophil.gm-companion\data /s /e /i