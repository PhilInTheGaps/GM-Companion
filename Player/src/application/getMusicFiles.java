package application;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.stream.Stream;

import org.jsoup.Jsoup;
import org.jsoup.nodes.Document;

public class getMusicFiles {
	public static void get() throws IOException{
		//This finds every music file in the folder and writes them into a list
		
		System.out.println("Finding music files in folder: "+Main.defaultMusicPath);
		
		Main.currentTrackID = 0;
    	
    	for (int i = 0; i<500; i++){
    		Main.musicPathList[i] = "";
    	}
		
  		if(Main.onlineMode){
  			//Get all music files from server
  			Document doc = Jsoup.connect(Main.serverMusicURL + Main.defaultMusicPath).get();
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
  	        	Main.musicPathList[f] = Main.serverMusicURL + Main.musicFolder +"/" + temp;
  	        	f++;
  	        	count += 1;
  	        }
  	        Main.maxTrackCount = count;
  		}
  		else{
  			try(Stream<Path> paths = Files.walk(Paths.get(Main.defaultMusicPath))) {
  			    paths.forEach(filePath -> {
  			        if (Files.isRegularFile(filePath)) {
  			            String tempPath = filePath.toString();
  			            Main.musicPathList[Main.currentTrackID] = tempPath;
  			            Main.currentTrackID ++;
  			        }
  			    });
  			} catch (IOException e) {
  				e.printStackTrace();
  			}
  			Main.maxTrackCount = Main.currentTrackID;
  		}
		
		// This checks whether RandomTrack is enabled and if true, generates a random TrackID
		// at which the playlist starts.
		// It was implemented before the randomly generated playlists and might be completely unnecessary.	
		
		if (Main.randomTrack == true){
			Main.currentTrackID = Main.randomTrackID.nextInt(Main.maxTrackCount);
		}
		else{
			Main.currentTrackID = 0;
		}
		
		//Prints out the name of every music file in the folder
		System.out.println("Found the following files:");
		for (int i = 0; i < Main.maxTrackCount; i++){
			System.out.println(Main.musicPathList[i]);
		}
		System.out.println("");
		
    }
}
