# GM-Companion

A tool for the gamemaster of a tabletop RPG session.

[![Build Status](https://travis-ci.org/PhilInTheGaps/GM-Companion.svg?branch=ui-update)](https://travis-ci.org/PhilInTheGaps/GM-Companion)
[![Build status](https://ci.appveyor.com/api/projects/status/8q56pf3cnbtyp6f3?svg=true)](https://ci.appveyor.com/project/PhilInTheGaps/gm-companion)
[![GitHub (pre-)release](https://img.shields.io/github/release/PhilInTheGaps/GM-Companion/all.svg)](https://github.com/PhilInTheGaps/GM-Companion/releases)
[![GPL Licence](https://badges.frapsoft.com/os/gpl/gpl.svg?v=103)](https://opensource.org/licenses/GPL-3.0/)

[Official Website](https://gm-companion.github.io/)  
[Launchpad Repository](https://launchpad.net/~rophil/+archive/ubuntu/gm-companion)  
[GM-Companion at itch.io](https://philinthegaps.itch.io/gm-companion)  

[Documentation](https://github.com/PhilInTheGaps/GM-Companion/wiki)

## Credits
 
Dice icons by [Skoll](http://game-icons.net/) and [Delapouite](http://delapouite.com/) under the [CC BY 3.0](http://creativecommons.org/licenses/by/3.0/)  
[FontAwesome](https://fontawesome.com/)  
[FontAwesome 5.1 QML Implementation](https://gitlab.com/Rophil/qml-fontawesome-5) based on [QMLFontAwesome](https://github.com/qCring/QMLFontAwesome) by [qCring](https://github.com/qCring)  

# Installation

## Windows

For Windows there is only a 64 bits version available. If you want to use the GM-Companion on a 32 bits machine, you have to build the program from source. 

[Release 1.0 (Win x64)](https://github.com/PhilInTheGaps/GM-Companion/releases/download/1.0.0.0/gm-companion_1.0.0.0_win64.zip)  

The program works without an installer. Simply extract all the files from the .zip archive to a folder and run _gm-companion.exe_

Older versions are available at the [GitHub Release Page](https://github.com/PhilInTheGaps/GM-Companion/releases)  


## Linux

Currently only Ubuntu is officially supported, but an Arch Linux package will be available in the future.

### Ubuntu

For Ubuntu the easiest way is to add the gm-companion software repository and install the gm-companion from there.  
Note that the GM-Companion requires at least Qt 5.10, currently only Ubuntu 18.10 meets that requirement.

```
sudo add-apt-repository ppa:rophil/gm-companion  
sudo apt-get update  
sudo apt-get install gm-companion  
```

I also have a daily-build repsository that always has the newest development version.  
I do not recommend using it, but if for some reason you absolutely want to use it, here:  


```
sudo add-apt-repository ppa:rophil/gm-companion-daily  
sudo apt-get update  
sudo apt-get install gm-companion  
```

## Building From Source

You can also build the program from source. This should work for Windows, Linux and Mac.  
Even though the travis.ci build for MacOS compiles without errors, I don't know if that version works 100% as I don't own a Mac to test this on.

GM-Companion requires Qt5 to build.  

So the build steps would look something like this:  
1. Clone the [GitHub repository](https://github.com/PhilInTheGaps/GM-Companion)  
2. Install [Qt5](https://www.qt.io/) (GM-Companion requires at least version 5.10)  
3. If you are on Linux install [TagLib](http://taglib.org/)  
4. Switch to the GM-Companion folder  
5. Run qmake  
6. Run make  



Have fun!
