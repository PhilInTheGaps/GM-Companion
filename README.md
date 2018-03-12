# GM-Companion

A tool for the gamemaster of a tabletop RPG session.

[![Build Status](https://travis-ci.org/PhilInTheGaps/GM-Companion.svg?branch=ui-update)](https://travis-ci.org/PhilInTheGaps/GM-Companion)
[![GitHub (pre-)release](https://img.shields.io/github/release/PhilInTheGaps/GM-Companion/all.svg)](https://github.com/PhilInTheGaps/GM-Companion/releases)
[![GPL Licence](https://badges.frapsoft.com/os/gpl/gpl.svg?v=103)](https://opensource.org/licenses/GPL-3.0/)

[Official Website](https://gm-companion.github.io/)  
[Launchpad Repository](https://launchpad.net/~rophil/+archive/ubuntu/gm-companion)  
[GM-Companion at itch.io](https://philinthegaps.itch.io/gm-companion)  

[Documentation](https://github.com/PhilInTheGaps/GM-Companion/wiki)


# Installation

## Windows

For Windows there are 64-bit and 32-bit installers available:

[Beta 3.3 (Win x64 Installer)](https://github.com/PhilInTheGaps/GM-Companion/releases/download/0.3.3.0/gm-companion_0.3.3_win64_setup.exe)  
[Beta 3.3 (Win x86 Installer)](https://github.com/PhilInTheGaps/GM-Companion/releases/download/0.3.3.0/gm-companion_0.3.3_win32_setup.exe)  
	
If you don't want an installer and instead want a zipped version that runs without an installer:

[Beta 3.3 (Win x64)](https://github.com/PhilInTheGaps/GM-Companion/releases/download/0.3.3.0/gm-companion_0.3.3_win64.zip)  
[Beta 3.3 (Win x86)](https://github.com/PhilInTheGaps/GM-Companion/releases/download/0.3.3.0/gm-companion_0.3.3_win32.zip)  
	
Older versions are available at the [GitHub Release Page](https://github.com/PhilInTheGaps/GM-Companion/releases)  


## Linux

Currently only Ubuntu is officially supported, but an Arch Linux package will be available in the future.

### Ubuntu

For Ubuntu the easiest way is to add the gm-companion software repository and install the gm-companion from there.

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

### Other Distros

Currently your only option is to compile the code yourself.  


GM-Companion requires Qt5 to build.  

So the build steps would look something like this:  
1. Clone the [GitHub repository](https://github.com/PhilInTheGaps/GM-Companion)  
2. Install Qt5  
3. Go to the GM-Companion folder
4. Run qmake
5. Run make

## MacOS

As the GM-Companion is Qt5 based, it should run on MacOS.  
Though I don't use a mac and can't confirm it.  
You could try the same steps I wrote above for other Linux distros and compile everything yourself.

Maybe it works, good luck!



Have fun!
