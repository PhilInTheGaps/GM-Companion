#!/bin/bash

echo "Building AppImage ..."

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

export QML_SOURCES_PATHS=../app/ui/
export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:cget/lib:cget/lib/x86_64-linux-gnu:thirdparty/sentry-native"

# Build AppImage
./linuxdeploy-x86_64.AppImage \
  --appdir AppDir \
  -e AppDir/usr/bin/librespot \
  -d AppDir/usr/share/applications/lol.rophil.gm_companion.desktop \
  --plugin qt \
  --output appimage
