#!/bin/bash

echo "Building AppImage ..."

# build librespot
echo "Building librespot ..."
git clone https://github.com/librespot-org/librespot
cd librespot
cargo build --release
cp target/release/librespot ../AppDir/usr/bin/librespot
cd ..

# Remove include dir
echo "Removing include dir ..."
rm -rf AppDir/usr/include

# Remove static libraries
echo "Removing static libraries ..."
rm AppDir/usr/lib/*.a

# Download linuxdeploy
echo "Downloading linuxdeploy ..."
if [ ! -f linuxdeploy-x86_64.AppImage ]; then
    wget https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-x86_64.AppImage
    chmod u+x linuxdeploy-x86_64.AppImage
fi

# Download linuxdeploy qt plugin
echo "Downloading linuxdeploy qt plugin ..."
if [ ! -f linuxdeploy-plugin-qt-x86_64.AppImage ]; then
    wget https://github.com/linuxdeploy/linuxdeploy-plugin-qt/releases/download/continuous/linuxdeploy-plugin-qt-x86_64.AppImage
    chmod u+x linuxdeploy-plugin-qt-x86_64.AppImage
fi

# Download linuxdeploy gstreamer plugin
echo "Downloading linuxdeploy gstreamer plugin ..."
if [ ! -f linuxdeploy-plugin-gstreamer.sh ]; then
    wget https://raw.githubusercontent.com/linuxdeploy/linuxdeploy-plugin-gstreamer/master/linuxdeploy-plugin-gstreamer.sh
    chmod u+x linuxdeploy-plugin-gstreamer.sh
fi

export EXTRA_QT_PLUGINS=audio,imageformats,mediaservice,playlistformats,gui
export QML_MODULES_PATHS=/opt/qt512/qml/
export QML_SOURCES_PATHS=../app/ui/
export LD_LIBRARY_PATH="$LD_LIBRARY_PATH;cget/lib;cget/lib/x86_64-linux-gnu"

# Build AppImage
./linuxdeploy-x86_64.AppImage \
  --appdir AppDir \
  -e AppDir/usr/bin/librespot \
  -d AppDir/usr/share/applications/lol.rophil.gm_companion.desktop \
  --plugin gstreamer \
  --plugin qt \
  --output appimage
