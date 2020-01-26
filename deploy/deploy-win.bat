cd ..

mkdir release\deploy

copy /y release\gm-companion.exe release\deploy

windeployqt.exe --release --qmldir %QT_PATH%\qml release\deploy\gm-companion.exe

copy /y "%OPENSSL%\bin\*eay32.dll" release\deploy

xcopy lib\poppler\bin release\deploy /s /e
xcopy lib\taglib\bin release\deploy /s /e

xcopy release\deploy deploy\installer\packages\lol.rophil.gm-companion\data /s /e

cd deploy\installer
build-offline-v3-1-x64.bat
cd ..\..