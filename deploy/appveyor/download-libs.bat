cd ..\..\lib
mkdir taglib
cd taglib

set TAGLIB_VERSION=1.12-beta-1
set POPPLER_VERSION=0.82.0-13
set QT_KEYCHAIN_VERSION=v0.10.0
set QT_VERSION=qt5-12

echo "Downloading taglib ..."

if "%COMPILER_NAME%" == "mingw-64" (

    echo https://github.com/PhilInTheGaps/taglib-bin/releases/download/%TAGLIB_VERSION%/taglib-mingw-w64-7.3.0.zip

	curl -L -o taglib.zip https://github.com/PhilInTheGaps/taglib-bin/releases/download/%TAGLIB_VERSION%/taglib-mingw-w64-7.3.0.zip || EXIT /B 1
	7z x taglib.zip || EXIT /B 1
	
) else (

    echo https://github.com/PhilInTheGaps/taglib-bin/releases/download/%TAGLIB_VERSION%/taglib-mingw-32-7.3.0.zip

	curl -L -o taglib.zip https://github.com/PhilInTheGaps/taglib-bin/releases/download/%TAGLIB_VERSION%/taglib-mingw-32-7.3.0.zip || EXIT /B 1
	7z x taglib.zip || EXIT /B 1
) 

del taglib.zip
cd ..

echo "Downloading poppler ..."

mkdir poppler
cd poppler

if "%COMPILER_NAME%" == "mingw-64" (

    echo https://github.com/PhilInTheGaps/poppler-bin/releases/download/%POPPLER_VERSION%/poppler-mingw-w64-7.3.0-%QT_VERSION%.zip

	curl -L -o poppler.zip https://github.com/PhilInTheGaps/poppler-bin/releases/download/%POPPLER_VERSION%/poppler-mingw-w64-7.3.0-%QT_VERSION%.zip || EXIT /B 1
	7z x poppler.zip || EXIT /B 1
	
) else (
    
	echo https://github.com/PhilInTheGaps/poppler-bin/releases/download/%POPPLER_VERSION%/poppler-mingw-32-7.3.0-%QT_VERSION%.zip

	curl -L -o poppler.zip https://github.com/PhilInTheGaps/poppler-bin/releases/download/%POPPLER_VERSION%/poppler-mingw-32-7.3.0-%QT_VERSION%.zip || EXIT /B 1
	7z x poppler.zip || EXIT /B 1
) 

del poppler.zip
cd ..

echo "Downloading qt5keychain ..."

mkdir qt5keychain
cd qt5keychain

if "%COMPILER_NAME%" == "mingw-64" (

    echo https://github.com/PhilInTheGaps/qtkeychain-bin/releases/download/%QT_KEYCHAIN_VERSION%/qt5keychain-mingw-w64-7.3.0-%QT_VERSION%.zip

	curl -L -o qt5keychain.zip https://github.com/PhilInTheGaps/qtkeychain-bin/releases/download/%QT_KEYCHAIN_VERSION%/qt5keychain-mingw-w64-7.3.0-%QT_VERSION%.zip || EXIT /B 1
	7z x qt5keychain.zip || EXIT /B 1
	
) else (
    
	echo https://github.com/PhilInTheGaps/qtkeychain-bin/releases/download/%QT_KEYCHAIN_VERSION%/qt5keychain-mingw-w64-7.3.0-%QT_VERSION%.zip

	curl -L -o qt5keychain.zip https://github.com/PhilInTheGaps/qtkeychain-bin/releases/download/%QT_KEYCHAIN_VERSION%/qt5keychain-mingw-w64-7.3.0-%QT_VERSION%.zip || EXIT /B 1
	7z x qt5keychain.zip || EXIT /B 1
) 

del qt5keychain.zip

cd ..\..
