cd ..\..\lib
mkdir taglib
cd taglib

echo "Downloading taglib ..."

if "%COMPILER_NAME%" == "mingw-64" (

    echo "https://github.com/PhilInTheGaps/taglib-bin/releases/download/1.11.1/taglib-mingw-w64-7.3.0.zip"

	curl -L -o taglib.zip https://github.com/PhilInTheGaps/taglib-bin/releases/download/1.11.1/taglib-mingw-w64-7.3.0.zip
	7z x taglib.zip
	
) else (

    echo "https://github.com/PhilInTheGaps/taglib-bin/releases/download/1.11.1/taglib-mingw-32-7.3.0.zip"

	curl -L -o taglib.zip https://github.com/PhilInTheGaps/taglib-bin/releases/download/1.11.1/taglib-mingw-32-7.3.0.zip
	7z x taglib.zip
) 

del taglib.zip
cd ..

echo "Downloading poppler ..."

mkdir poppler
cd poppler

if "%COMPILER_NAME%" == "mingw-64" (

    echo "https://github.com/PhilInTheGaps/poppler-bin/releases/download/0.82.0-13/poppler-mingw-w64-7.3.0-qt5-13.zip"

	curl -L -o poppler.zip https://github.com/PhilInTheGaps/poppler-bin/releases/download/0.82.0-13/poppler-mingw-w64-7.3.0-qt5-13.zip
	7z x poppler.zip
	
) else (
    
	echo "https://github.com/PhilInTheGaps/poppler-bin/releases/download/0.82.0-13/poppler-mingw-32-7.3.0-qt5-12.zip"

	curl -L -o poppler.zip https://github.com/PhilInTheGaps/poppler-bin/releases/download/0.82.0-13/poppler-mingw-32-7.3.0-qt5-12.zip
	7z x poppler.zip
) 

del poppler.zip

cd ..\..

