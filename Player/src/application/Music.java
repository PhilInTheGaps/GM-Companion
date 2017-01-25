package application;

import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Random;
import java.util.stream.Stream;

import org.jsoup.Jsoup;
import org.jsoup.nodes.Document;

import javafx.collections.MapChangeListener;
import javafx.scene.image.Image;
import javafx.scene.media.Media;
import javafx.scene.media.MediaPlayer;

import java.net.MalformedURLException;  
import java.net.URL;  
import org.apache.commons.io.FileUtils;


public class Music {
	public static MediaPlayer mediaPlayer;
	//public static Duration duration;
	public static Media mMedia;
	public static String musicPath;
	public static String[] musicPathList = new String[500];
	public static String currentTrack;
	public static int maxTrackCount;
	public static Boolean musicIsPlaying = false;
	public static Boolean musicFolderSelected = false;
	public static String musicError;
	public static Random randomTrackID = new Random();
	public static int currentTrackID;
	public static double musicVolume = 0.5;
	public static Boolean initialPress = false;
	
	public static String defaultMusicPath = ("./Music/");
	public static String serverMusicURL = UI.serverURL + "music/";
	
	public static void play(){
		System.out.println("Converting File Path...");
		//Gets the path to the current music file and converts it, so the MediaPlayer can read it
		musicPath = musicPathList[currentTrackID];
		
		musicPath = musicPath.replace("\\", "/");
		
		if(UI.onlineMode){
			mMedia = new Media(musicPath);
		}
		else{
			mMedia = new Media(new File(musicPath).toURI().toString());
		}
		
		//Creates a MediaPlayer that plays the music file
		System.out.println("Creating MediaPlayer...");
		mediaPlayer = new MediaPlayer(mMedia);
		mediaPlayer.setAutoPlay(true);
		mediaPlayer.setVolume(musicVolume);
		musicIsPlaying = true;
	    
	    //Clear Metadata
	    System.out.println("Clearing old MetaData...");
	    UI.albumLabel.setText("Album: Unknown");
	    UI.artistLabel.setText("Artist: Unknown");
	    UI.titleLabel.setText("Title: Unknown");
	    UI.yearLabel.setText("Year: Unknown");
	    UI.coverImage.setImage(null);
	    
	    //If the music folder button was pressed, generate a new random playlist
	    if(initialPress){
	    	initialPress = false;
	    	currentTrackID = maxTrackCount-1;
	    	next();
	    }
	    
	    
	    //Get Metadata
	    System.out.println("Getting new MetaData...");
	    mMedia.getMetadata().addListener(new MapChangeListener<String, Object>(){
	    	@Override
	    	public void onChanged(Change<? extends String, ? extends Object> ch){
	    		if (ch.wasAdded()) {
	    			handleMetaData(ch.getKey(), ch.getValueAdded());
	    		}
	    	}
	    });
	    
	    //Plays the next music file if the current one ends
	    mediaPlayer.setOnEndOfMedia(new Runnable() {
	        @Override public void run() {
	        	if(UI.singleTrack == false){
	        		next();
	        	}
	        }
	    });
	    
	    //Updates the Progress Bar and sets Metadata
	    mediaPlayer.setOnReady(new Runnable() {
	        @Override
	        public void run() {
	            System.out.println("Duration: "+mMedia.getDuration().toSeconds());
	            UI.updatePB();
	        }
        });
    }
	
	public static void handleMetaData(String key, Object value){
		switch (key){
    	case ("album"):
    		UI.Album = value.toString();
    		UI.albumLabel.setText("Album: " + UI.Album);
    		System.out.println("Album: " + UI.Album);
			break;
    	case ("artist"):
    		UI.Artist = value.toString();
    		UI.artistLabel.setText("Artist: " + UI.Artist);
    		System.out.println("Artist: " + UI.Artist);
    		break;
    	case ("title"):
    		UI.Title = value.toString();
    		UI.titleLabel.setText("Title: " + UI.Title);
    		System.out.println("Title: " + UI.Title);
    		break;
    	case ("year"):
    		UI.Year = value.toString();
    		UI.yearLabel.setText("Year: " + UI.Year);
    		System.out.println("Year: " + UI.Year);
    		break;
    	}
		UI.coverImage.setImage((Image) mediaPlayer.getMedia().getMetadata().get("image"));
	}
	
	// Using common IO  
	public static void downloadFile (String fileName, String fileUrl) throws MalformedURLException, IOException {  
		FileUtils.copyURLToFile(new URL(fileUrl), new File(fileName));
	}
	
	public static void get() throws IOException{
		//This finds every music file in the folder and writes them into a list
		
		System.out.println("Finding music files in folder: "+defaultMusicPath);
		
		currentTrackID = 0;
    	
    	for (int i = 0; i<500; i++){
    		musicPathList[i] = "";
    	}
    	
  		if(UI.onlineMode){
  			//Get all music files from server
  			Document doc = Jsoup.connect(serverMusicURL + defaultMusicPath).get();
  	        System.out.println(doc.toString());
  	        String str = doc.toString();
  	        String findStr = "href=";
  	        int lastIndex1 = 0;
  	        int lastIndex2 = 10;
  	        ArrayList<String> fileNames = new ArrayList<String>();
  	        String test = new String();
  	        int count = 0;
  	        
  	        while(lastIndex1 != -1){

  	            lastIndex1 = str.indexOf(findStr,lastIndex1);
  	            lastIndex2 = str.indexOf(".mp3", lastIndex1);
  	            
  	            
  	            if(lastIndex1 != -1){
  	            	for(int i = lastIndex1+findStr.length()+1; i < lastIndex2+4; i++){
  	            		test += str.charAt(i);
  	            	}
  	            	fileNames.add(test);
  	            	test = "";
  	                lastIndex1 += findStr.length();
  	            }
  	        }
  	        int f = 0;
  	        for(int i = 1; i< fileNames.size(); i++){
  	        	String temp = fileNames.get(i).toString();
  	        	musicPathList[f] = serverMusicURL + UI.musicFolder +"/" + temp;
  	        	f++;
  	        	count += 1;
  	        }
  	        maxTrackCount = count;
  		}
  		else{
  			try(Stream<Path> paths = Files.walk(Paths.get(defaultMusicPath))) {
  			    paths.forEach(filePath -> {
  			        if (Files.isRegularFile(filePath)) {
  			            String tempPath = filePath.toString();
  			            musicPathList[currentTrackID] = tempPath;
  			            currentTrackID ++;
  			        }
  			    });
  			} catch (IOException e) {
  				e.printStackTrace();
  			}
  			maxTrackCount = currentTrackID;
  		}
		
		// This checks whether RandomTrack is enabled and if true, generates a random TrackID
		// at which the playlist starts.
		// It was implemented before the randomly generated playlists and might be completely unnecessary.	
		
		if (UI.randomTrack == true){
			currentTrackID = randomTrackID.nextInt(maxTrackCount);
		}
		else{
			currentTrackID = 0;
		}
		
		//Prints out the name of every music file in the folder
		System.out.println("Found the following files:");
		for (int i = 0; i < maxTrackCount; i++){
			System.out.println(musicPathList[i]);
		}
		System.out.println("");
		
    }
	
	public static void next(){
		//Switches to the next music file
		System.out.println("");
		System.out.println("Changing to next music file...");
		
		//Checks, whether a file is currently playing and stops it if true
    	if (musicIsPlaying == true){
    		mediaPlayer.stop();
    	}
    	
    	//Creates a randomly generated playlist if the current file is the last one in the playlist
    	if (UI.randomTrack == true){
    		if(maxTrackCount != 0){
    			if(currentTrackID == maxTrackCount-1){
    				System.out.println("Generating new random music playlist...");
        			for(int i = 0; i<500; i++){
        				String shuffle1 = "";
        				String shuffle2 = "";
        				String shuffle3 = "";
        				
        				int ID1 = randomTrackID.nextInt(maxTrackCount);
        				shuffle1 = musicPathList[ID1];
        				
        				int ID2 = randomTrackID.nextInt(maxTrackCount);
        				shuffle2 = musicPathList[ID2];
        				
        				shuffle3 = shuffle2;
        				shuffle2 = shuffle1;
        				shuffle1 = shuffle3;
        				
        				musicPathList[ID1] = shuffle1;
        				musicPathList[ID2] = shuffle2;
        				
        				currentTrackID = 0;
        			}
        			System.out.println("Random music playlist generated:");
        			if(UI.debug){
        				for(int i = 0; i < maxTrackCount; i++){
        					System.out.println(musicPathList[i]);
        				}
        			}
        			
        			System.out.println("");
        		}
        		else{
        			currentTrackID++;
        		}
    		}
    		
    	}
    	else{
    		if (currentTrackID < maxTrackCount - 1){
    			currentTrackID ++;
        	}
        	else{
        		currentTrackID = 0;
        	}
    	}
    	
    	play();
    }
	
}
