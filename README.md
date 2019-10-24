# GM-Companion | [![GitHub (pre-)release](https://img.shields.io/github/release/PhilInTheGaps/GM-Companion/all.svg)](https://github.com/PhilInTheGaps/GM-Companion/releases) [![GPL Licence](https://badges.frapsoft.com/os/gpl/gpl.svg?v=103)](https://opensource.org/licenses/GPL-3.0/)

A tool for the gamemaster of a tabletop RPG session.


**[Website](https://gm-companion.github.io/) | [Documentation](https://gm-companion.github.io/documentation.html)**

![Screenshot](https://gm-companion.github.io/assets/images/screenshots/audio-tool-01.png)

## Build Status

Travis-CI | AppVeyor | Jenkins
--|---|--
 [![travis](https://travis-ci.org/PhilInTheGaps/GM-Companion.svg?branch=ui-update)](https://travis-ci.org/PhilInTheGaps/GM-Companion) | [![appveyor](https://ci.appveyor.com/api/projects/status/8q56pf3cnbtyp6f3?svg=true)](https://ci.appveyor.com/project/PhilInTheGaps/gm-companion) |  [![jenkins](http://jenkins.rophil.lol/buildStatus/icon?job=GM-Companion)](http://jenkins.rophil.lol/job/GM-Companion/)

## Credits

- Dice and effect icons by [Skoll](http://game-icons.net/), [Delapouite](http://delapouite.com/), [Lorc](http://lorcblog.blogspot.com/) and [Cathelineau](https://game-icons.net/) under the [CC BY 3.0](http://creativecommons.org/licenses/by/3.0/)  
- Default music, sound and radio icons by [Marius Masalar](https://unsplash.com/@marius), [Davin Martin Jr.](https://unsplash.com/@davidmartinjr) and [Alex Blăjan](https://unsplash.com/@alexb) from [Unsplash](https://unsplash.com/)
- [FontAwesome](https://fontawesome.com/)  
- [FontAwesome QML Implementation](https://github.com/PhilInTheGaps/fontawesome.pri) fork of [FontAwesome.pri](https://github.com/benlau/fontawesome.pri) by [Ben Lau](https://github.com/benlau)  
- [o2 (OAuth2 library)](https://github.com/pipacs/o2) by [pipacs](https://github.com/pipacs)  
- Spotify playlists by [Brian Davis (Bezoing)](https://open.spotify.com/user/bezoing?si=acN6RQebQTS2iZEUWDKRKA) and [gentilpuck](https://open.spotify.com/user/gentilpuck?si=nZpk5I8wTBuWdcBrrix_qg)

# Installation

## Linux

### AppImage

[Download 64-bit](https://github.com/PhilInTheGaps/GM-Companion/releases/download/1.1.0/gm-companion-1.1.0_amd64.AppImage)  
The AppImage is built using Ubuntu 16.04 LTS.

You might have to install gstreamer 1.0 otherwise audio cannot be played. For me this version works on Ubuntu but not on Manjaro.

### Arch Linux / Manjaro

Install from [AUR](https://aur.archlinux.org/packages/gm-companion/):

```
yay -S gm-companion
```

### Ubuntu

Currently your best option is to build from source:  
(I am working on a ppa)

#### Ubuntu 18.10

```
git clone --recursive https://github.com/PhilInTheGaps/GM-Companion

sudo apt-get install qtbase5-dev qt5-default qttools5-dev qtmultimedia5-dev libqt5opengl5-dev libqt5x11extras5-dev libqt5xmlpatterns5-dev qtdeclarative5-dev libqt5network5 libqt5webengine5 libqt5webenginecore5 libqt5networkauth5-dev libqt5core5a libqt5multimedia5 libqt5multimedia5-plugins libqt5network5 libqt5networkauth5 libqt5opengl5 libqt5script5 libqt5widgets5 libqt5xml5 libqt5xmlpatterns5 libqt5gui5 libqt5qml5 libqt5quick5 qml-module-qtquick-layouts qml-module-qtquick-window2 libqt5quickcontrols2-5 qml-module-qtquick2 qml-module-qtquick-controls libqt5webengine5 libqt5webenginecore5 libqt5multimediaquick5 libqt5quickwidgets5 libpoppler-qt5-dev

cd GM-Companion
qmake
make
```
run: `./gm-companion`  
or install:
```
make install
gm-companion
```

#### Ubuntu 18.04

```
git clone --recursive https://github.com/PhilInTheGaps/GM-Companion

sudo add-apt-repository ppa:beineri/opt-qt-5.12.1-bionic
sudo apt-get update
sudo apt-get install binutils libpulse-dev qt512base qt512declarative qt512graphicaleffects qt512imageformats qt512multimedia qt512networkauth-no-lgpl qt512quickcontrols qt512quickcontrols2 qt512script qt512tools qt512translations qt512webengine libegl1-mesa-dev libglu1-mesa-dev mesa-utils libpoppler-qt5-dev

source /opt/qt512/bin/qt512-env.sh

cd GM-Companion
qmake
make
```
run: `./gm-companion`  
or install:
```
make install
gm-companion
```

#### Ubuntu 16.04

```
git clone --recursive https://github.com/PhilInTheGaps/GM-Companion

sudo add-apt-repository ppa:beineri/opt-qt-5.12.1-xenial
sudo apt-get update
sudo apt-get install binutils libpulse-dev qt512base qt512declarative qt512graphicaleffects qt512imageformats qt512multimedia qt512networkauth-no-lgpl qt512quickcontrols qt512quickcontrols2 qt512script qt512tools qt512translations qt512webengine libegl1-mesa-dev libglu1-mesa-dev mesa-utils libpoppler-qt5-dev

source /opt/qt512/bin/qt512-env.sh

cd GM-Companion
qmake
make
```
run: `./gm-companion`  
or install:
```
make install
gm-companion
```

### Debian

#### Buster

```
git clone --recursive https://github.com/philinthegaps/gm-companion

sudo apt install qt5-default qtmultimedia5-dev libqt5quick5 qtquickcontrols2-5-dev qml-module-qtquick-controls qml-module-qtquick-controls2 qml-module-qtquick-dialogs qml-module-qtquick-extras qml-module-qtquick-window2 qml-module-qt-labs-folderlistmodel qml-module-qt-labs-settings qml-module-qtquick2 libqt5networkauth5-dev qtdeclarative5-dev libtag1-dev libpoppler-qt5-dev

cd gm-companion
qmake
make
```
run: `./gm-companion`  
or install:
```
make install
gm-companion
```

### Other Distros

1. `git clone --recursive https://github.com/PhilInTheGaps/GM-Companion`
2. Install [Qt5](https://www.qt.io/) >=5.10  
3. Install [Poppler-Qt5](https://poppler.freedesktop.org/)  
4. Build the program:
```
cd GM-Companion
qmake
make
```
run: `./gm-companion`  
or install:
```
make install
gm-companion
```

## Windows

[Release 1.1.0 (Win x64)](https://github.com/PhilInTheGaps/GM-Companion/releases/download/1.1.0/gm-companion_1.1.0_win64.zip)  

The program works without an installer. Simply extract all the files from the .zip archive to a folder and run _gm-companion.exe_.

# Usage / Quickstart

For detailed instructions see the [Documentation](https://gm-companion.github.io/documentation.html).

## Audio Tool

- Go to settings, set path of music and sound files
- Open audio editor, create new project
- Add a category and a scenario
- Create an element, add music/sound files, radio playlist file or URL
- Optionally create a thumbnail image, place it in your resources path, add image to element

### Spotify

Using Spotify with GM-Companion requires some additional steps first:

- Go to [Spotify developer page](https://developer.spotify.com/dashboard)
- "Create an App"
- In app settings add _http://127.0.0.1:1965/_ as a redirect URI
- Go to GM-Companion settings, add "Client ID" and "Client Secret" of your app
- Restart GM-Companion
- You can now add Spotify playlists and albums as elements, the "_Audio Addon_" has some great RPG related playlists. You can add by adding a Spotify element and pressing "_Add from Addons_"

Note that playing Spotify playlists requires the Spotify app running in the background.

## Map Tool

- Open your maps folder (see "Paths" settings)
- Create a new folder
- Place map images inside

## Character Tool

- Open your characters folder (see "Paths" settings)
- Create a new folder and name it after the character
- Place images of the character sheet inside

Have fun!
