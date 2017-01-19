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
		
		Main.sMedia = new Media(new File(Main.soundPath).toURI().toString());
		
		System.out.println("Creating Sound MediaPlayer...");
		Main.soundPlayer = new MediaPlayer(Main.sMedia);
		Main.soundPlayer.setAutoPlay(Main.autoplay);
		Main.soundPlayer.setVolume(Main.soundVolume);
		
		if (Main.autoplay == true){
			Main.soundIsPlaying = true;
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
