package application;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.stream.Stream;

import org.jsoup.Jsoup;
import org.jsoup.nodes.Document;

public class getSoundFiles {
	public static void get() throws IOException{
		//This finds every sound file in the folder and writes them into a list
		
		System.out.println("Finding sound files in folder: /"+Main.defaultSoundPath);
		
    	Main.currentSoundID = 0;
    	
    	for (int i = 0; i<500; i++){
    		Main.soundPathList[i] = "";
    	}
	
    	if(Main.onlineMode){
  			//Get all sound files from server
  			Document doc = Jsoup.connect(Main.serverSoundsURL + Main.defaultSoundPath).get();
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
  	        	Main.soundPathList[f] = Main.serverSoundsURL + Main.soundFolder +"/" + temp;
  	        	f++;
  	        	count += 1;
  	        }
  	        Main.maxSoundCount = count;
  		}
  		else{
  			try(Stream<Path> paths = Files.walk(Paths.get(Main.defaultSoundPath))) {
  			    paths.forEach(filePath -> {
  			        if (Files.isRegularFile(filePath)) {
  			            String tempPath = filePath.toString();
  			            Main.soundPathList[Main.currentSoundID] = tempPath;
  			            Main.currentSoundID ++;
  			        }
  			    });
  			} catch (IOException e) {
  				e.printStackTrace();
  			}
  			Main.maxSoundCount = Main.currentSoundID;
  		}
		
		// This checks whether RandomTrack is enabled and if true, generates a random SoundID
		// at which the playlist starts.
		// It was implemented before the randomly generated playlists and might be completely unnecessary.	
		
		if (Main.randomTrack == true){
			Main.currentSoundID = Main.randomSoundID.nextInt(Main.maxSoundCount);
		}
		else{
			Main.currentSoundID = 0;
		}
		
		//Prints out the name of every sound file in the folder
		System.out.println("Found the following files:");
		for (int i = 0; i < Main.maxSoundCount; i++){
			System.out.println(Main.soundPathList[i]);
		}
		System.out.println("");
    }
}
