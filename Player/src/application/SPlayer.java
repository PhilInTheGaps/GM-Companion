package application;

import java.io.File;

import javafx.scene.media.Media;
import javafx.scene.media.MediaPlayer;

public class SPlayer {
	public static void play(){
		
		Main.soundPath = Main.soundPathList[Main.currentSoundID];
		Main.soundPath = Main.soundPath.replace("\\", "/");
		
		Main.sMedia = new Media(new File(Main.soundPath).toURI().toString());

		Main.soundPlayer = new MediaPlayer(Main.sMedia);
		Main.soundPlayer.setAutoPlay(Main.autoplay);
		Main.soundPlayer.setVolume(Main.soundVolume);
		
		if (Main.autoplay == true){
			Main.soundIsPlaying = true;
		}
        
		Main.soundPlayer.setOnEndOfMedia(new Runnable() {
            @Override public void run() {
            	if (Main.singleTrack == false){
            		Main.nextSoundFile();
            	}
            }
        });
	}
}
