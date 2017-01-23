package application;

import java.io.File;
import java.util.Random;

import javafx.scene.media.Media;
import javafx.scene.media.MediaPlayer;

public class SPlayer {
	
	public static MediaPlayer soundPlayer;
	public static Media sMedia;
	public static String soundPath;
	public static String[] soundPathList = new String[500];
	public static String currentSound;
	public static int maxSoundCount;
	public static Boolean soundIsPlaying = false;
	public static Boolean soundFolderSelected = false;
	public static String soundError;
	public static Random randomSoundID = new Random();
	public static int currentSoundID;
	
	
	
	
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
		soundPlayer = new MediaPlayer(Main.sMedia);
		soundPlayer.setAutoPlay(Main.autoplay);
		soundPlayer.setVolume(Main.soundVolume);
		
		if (Main.autoplay == true){
			Main.soundIsPlaying = true;
		}
        
		//If the sound folder button was pressed, generate a new random playlist
	    if(Main.initialPress){
	    	Main.initialPress = false;
	    	Main.currentSoundID = Main.maxSoundCount-1;
	    	nextSoundFile.next();
	    }
		
		soundPlayer.setOnEndOfMedia(new Runnable() {
            @Override public void run() {
            	if (Main.singleTrack == false){
            		nextSoundFile.next();
            	}
            }
        });
	}
}
