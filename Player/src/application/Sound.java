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

import javafx.scene.media.Media;
import javafx.scene.media.MediaPlayer;

public class Sound {

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
	public static Boolean initialPress = false;
	
	public static double soundVolume = 0.25;
	
	public static String defaultSoundPath = ("./Sounds/");
	public static String serverSoundsURL = UI.serverURL + "sounds/";
	
	public static void play(){
		
		//Like MPlayer.java, only for sounds
		
		System.out.println("Converting File Path...");
		soundPath = soundPathList[currentSoundID];
		soundPath = soundPath.replace("\\", "/");
		
		if(UI.onlineMode){
			sMedia = new Media(soundPath);
		}
		else{
			sMedia = new Media(new File(soundPath).toURI().toString());
		}
		
		System.out.println("Creating Sound MediaPlayer...");
		soundPlayer = new MediaPlayer(sMedia);
		soundPlayer.setAutoPlay(true);
		soundPlayer.setVolume(soundVolume);
		soundIsPlaying = true;
        
		//If the sound folder button was pressed, generate a new random playlist
	    if(initialPress){
	    	initialPress = false;
	    	currentSoundID = maxSoundCount-1;
	    	next();
	    }
		
		soundPlayer.setOnEndOfMedia(new Runnable() {
            @Override public void run() {
            	if (UI.singleTrack == false){
            		next();
            	}
            }
        });
	}
	
	public static void get() throws IOException{
		//This finds every sound file in the folder and writes them into a list
		
		System.out.println("Finding sound files in folder: /"+defaultSoundPath);
		
    	currentSoundID = 0;
    	
    	for (int i = 0; i<500; i++){
    		soundPathList[i] = "";
    	}
	
    	if(UI.onlineMode){
  			//Get all sound files from server
  			Document doc = Jsoup.connect(serverSoundsURL + defaultSoundPath).get();
  	        //System.out.println(doc.toString());
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
  	        	soundPathList[f] = serverSoundsURL + UI.soundFolder +"/" + temp;
  	        	f++;
  	        	count += 1;
  	        }
  	        maxSoundCount = count;
  		}
  		else{
  			try(Stream<Path> paths = Files.walk(Paths.get(defaultSoundPath))) {
  			    paths.forEach(filePath -> {
  			        if (Files.isRegularFile(filePath)) {
  			            String tempPath = filePath.toString();
  			            soundPathList[currentSoundID] = tempPath;
  			            currentSoundID ++;
  			        }
  			    });
  			} catch (IOException e) {
  				e.printStackTrace();
  			}
  			maxSoundCount = currentSoundID;
  		}
		
		// This checks whether RandomTrack is enabled and if true, generates a random SoundID
		// at which the playlist starts.
		// It was implemented before the randomly generated playlists and might be completely unnecessary.	
		
		if (UI.randomTrack == true){
			currentSoundID = randomSoundID.nextInt(maxSoundCount);
		}
		else{
			currentSoundID = 0;
		}
		
		//Prints out the name of every sound file in the folder
		System.out.println("Found the following files:");
		for (int i = 0; i < maxSoundCount; i++){
			System.out.println(soundPathList[i]);
		}
		System.out.println("");
    }
	
	public static void next(){
    	//Same as with nextMusicFile, only for sounds
		System.out.println("");
		System.out.println("Changing to next sound file...");
		
    	if (soundIsPlaying == true){
    		soundPlayer.stop();
    	}
    	
    	if (UI.randomTrack == true){
    		if(maxSoundCount != 0){
    			if(currentSoundID == maxSoundCount-1){
    				System.out.println("Generating new random sound playlist...");
        			for(int i = 0; i<50; i++){
        				
        				String shuffle1 = "";
        				String shuffle2 = "";
        				String shuffle3 = "";
        				
        				int ID1 = randomSoundID.nextInt(maxSoundCount);
        				shuffle1 = soundPathList[ID1];
        				
        				int ID2 = randomSoundID.nextInt(maxSoundCount);
        				shuffle2 = soundPathList[ID2];
        				
        				shuffle3 = shuffle2;
        				shuffle2 = shuffle1;
        				shuffle1 = shuffle3;
        				
        				soundPathList[ID1] = shuffle1;
        				soundPathList[ID2] = shuffle2;
        				
        				currentSoundID = 0;
        			}
        			System.out.println("Random sound playlist generated");
        			System.out.println("");
        		}
        		else{
        			currentSoundID++;
        		}
    		}
    	}
    	else{
    		if (currentSoundID < maxSoundCount - 1){
    			currentSoundID ++;
        	}
        	else{
        		currentSoundID = 0;
        	}
    	}

    	play();
    }
	
}
