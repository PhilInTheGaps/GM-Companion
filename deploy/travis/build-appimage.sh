#!/bin/bash

echo "Building AppImage ..."

make INSTALL_ROOT=../AppDir -j$(nproc) install ; find AppDir/
make clean

if [ ! -f linuxdeploy-x86_64.AppImage ]; then
    wget https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-x86_64.AppImage
    chmod u+x linuxdeploy-x86_64.AppImage
fi

if [ ! -f linuxdeploy-plugin-qt-x86_64.AppImage ]; then
    wget https://github.com/linuxdeploy/linuxdeploy-plugin-qt/releases/download/continuous/linuxdeploy-plugin-qt-x86_64.AppImage
    chmod u+x linuxdeploy-plugin-qt-x86_64.AppImage
fi

if [ ! -f linuxdeploy-plugin-gstreamer.sh ]; then
    wget https://raw.githubusercontent.com/linuxdeploy/linuxdeploy-plugin-gstreamer/master/linuxdeploy-plugin-gstreamer.sh
    chmod u+x linuxdeploy-plugin-gstreamer.sh
fi

export EXTRA_QT_PLUGINS=audio,imageformats,mediaservice,playlistformats,gui
export QML_MODULES_PATHS=/opt/qt512/qml/
export QML_SOURCES_PATHS=src/ui/

cp $HOME/.cargo/bin/librespot AppDir/usr/bin/

./linuxdeploy-x86_64.AppImage \
  --appdir AppDir \
  -e AppDir/usr/bin/librespot \
  -d AppDir/usr/share/applications/lol.rophil.gm_companion.desktop \
  --plugin gstreamer \
  --plugin qt \
  --output appimage
