# RPGMSPlayer

Ein Informatik Projekt von Phil Hoffmann  und Niklas Lüdtke.

##Welcome to the RPGMSPlayer!
###Info
The RPGMSPlayer is a program for playing music playlists and sounds that fit a specific situation in a tabletop RPG.

Currently it supports files hosted locally or on a server (only .mp3 files at the moment).
Also, the Linux version currently does not support locally hosted files, it will be implemented in the future.

###Setup
The RPGMSPlayer is easy to set up:

Download the .jar from the git

####Local files
In the same folder you downloaded the .jar into, create new folders in an order that looks like this:

 * Music
   * Scenario1
     * Situation1
      * Situation2
       * Situation3
    * Scenario2
      * Situation1
       * Situation2
 * Sounds
   * Situation1
    * Situation2

The music and sounds folders have to be named "Music" and "Sounds".

All the other folder names are not important.

####Server hosted files

You can host your files on a html server.

1. In the HTML folder, or a sub-folder, put your files in the correct structure as shown above.

2. Simply type your IP or domain into the text field at the top-right of the program:

   `http://yourserverdomain.com/some_folder/`

3. Click on "Set URL", then "Use Server Files" and when you are ready, click "Update".

4. Your uploaded files should appear on screen.
