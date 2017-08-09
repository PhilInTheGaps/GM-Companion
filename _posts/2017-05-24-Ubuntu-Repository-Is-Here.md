---
title: Ubuntu ppa is now available!
date: 2017-05-24 00:00:00 Z
categories:
- docs
- info
layout: media
modified: 
excerpt: 
image:
  feature: 
  teaser: siteLabel.png
  thumb: icon128.png
---

I finally did it.

Ok I know. I said it would take some time until the GM-Companion is released as a source package in a repository for GNU/Linux systems.  
But investing so much time in trying to do it and then giving up was somehow not acceptable for me. 
So I simply invested even more time and now finally it is here. At least for Ubuntu 17.04 amd64 systems. I proudly present:  
_ppa:rophil/gm-companion_  

To add it to your system, open the terminal and type  
_sudo add-apt-repository ppa:rophil/gm-companion_  
Then to install the program type  
_sudo apt-get update_  
This updates the list of available software, so that the GM-Companion is available.  
_sudo apt-get install gm-companion_ Should install the program.  

To start it, simply type _gm-companion_ in the terminal and everything should (!) work.  
Maybe it does not. If it doesn't please open a GitHub Issue and report the problem.  

If _sudo apt-get update_ leads to an error regarding the public key, visit the wiki installation page.  
I will add a possible solution there.
