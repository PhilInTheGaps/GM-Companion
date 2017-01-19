package application;

import java.io.File;

import javafx.collections.MapChangeListener;
//import javafx.collections.MapChangeListener.Change;
import javafx.scene.media.Media;
import javafx.scene.media.MediaPlayer;

public class MPlayer {
	
	public static void play(){
		
		//Gets the path to the current music file and converts it, so the MediaPlayer can read it
		Main.musicPath = Main.musicPathList[Main.currentTrackID];
		
		Main.musicPath = Main.musicPath.replace("\\", "/");
		
		Main.mMedia = new Media(new File(Main.musicPath).toURI().toString());
		
		//Creates a MediaPlayer that plays the music file
		Main.mediaPlayer = new MediaPlayer(Main.mMedia);
		Main.mediaPlayer.setAutoPlay(Main.autoplay);
		Main.mediaPlayer.setVolume(Main.musicVolume);
	    if (Main.autoplay == true){
	    	Main.musicIsPlaying = true;
	    }
	    
	    //Clear Metadata
	    Main.albumLabel.setText("Album: Unknown");
	    Main.artistLabel.setText("Artist: Unknown");
	    Main.titleLabel.setText("Title: Unknown");
	    Main.yearLabel.setText("Year: Unknown");
	    Main.coverImage.setImage(null);
	    
	    //Get Metadata
	    Main.mMedia.getMetadata().addListener(new MapChangeListener<String, Object>(){
	    	@Override
	    	public void onChanged(Change<? extends String, ? extends Object> ch){
	    		if (ch.wasAdded()) {
	    			HandleMetadata.handle(ch.getKey(), ch.getValueAdded());
	    		}
	    	}
	    });
	    
	    
	    //Plays the next music file if the current one ends
	    Main.mediaPlayer.setOnEndOfMedia(new Runnable() {
	        @Override public void run() {
	        	if(Main.singleTrack == false){
	        		nextMusicFile.next();
	        	}
	        }
	    });
	}
}
