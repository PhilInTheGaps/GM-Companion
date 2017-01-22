package application;

import java.io.File;

import javafx.scene.media.Media;
import javafx.scene.media.MediaPlayer;

public class SPlayer {
	public static void play(){
		
		//Like MPlayer.java, only for sounds
		
		System.out.println("Converting File Path...");
		Main.soundPath = Main.soundPathList[Main.currentSoundID];
		Main.soundPath = Main.soundPath.replace("\\", "/");
		
		if(Main.onlineMode){
			Main.sMedia = new Media(Main.soundPath);
		}
		else{
			Main.sMedia = new Media(new File(Main.soundPath).toURI().toString());
		}
		
		System.out.println("Creating Sound MediaPlayer...");
		Main.soundPlayer = new MediaPlayer(Main.sMedia);
		Main.soundPlayer.setAutoPlay(Main.autoplay);
		Main.soundPlayer.setVolume(Main.soundVolume);
		
		if (Main.autoplay == true){
			Main.soundIsPlaying = true;
		}
        
		//If the sound folder button was pressed, generate a new random playlist
	    if(Main.initialPress){
	    	Main.initialPress = false;
	    	Main.currentSoundID = Main.maxSoundCount-1;
	    	nextSoundFile.next();
	    }
		
		Main.soundPlayer.setOnEndOfMedia(new Runnable() {
            @Override public void run() {
            	if (Main.singleTrack == false){
            		nextSoundFile.next();
            	}
            }
        });
	}
}
