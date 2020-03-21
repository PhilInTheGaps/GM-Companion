#!/bin/bash

sudo add-apt-repository ppa:beineri/opt-qt-5.12.7-xenial -y
sudo apt-get update -q
sudo apt-get install qt512base qt512declarative qt512graphicaleffects qt512imageformats qt512multimedia qt512networkauth-no-lgpl qt512quickcontrols qt512quickcontrols2 qt512script qt512tools qt512translations qt512webengine -y
source /opt/qt512/bin/qt512-env.sh
