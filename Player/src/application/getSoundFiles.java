package application;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.stream.Stream;

public class getSoundFiles {
	public static void get(){
    	
		//This finds every sound file in the folder and writes them into a list
		
		System.out.println("Finding sound files in folder: /"+Main.defaultSoundPath);
		
    	Main.currentSoundID = 0;
    	
    	for (int i = 0; i<500; i++){
    		Main.soundPathList[i] = "";
    	}
	
		try(Stream<Path> paths = Files.walk(Paths.get(Main.defaultSoundPath))) {
		    paths.forEach(filePath -> {
		        if (Files.isRegularFile(filePath)) {
		            //System.out.println(filePath);
		            String tempPath = filePath.toString();
		            Main.soundPathList[Main.currentSoundID] = tempPath;
		            Main.currentSoundID ++;
		        }
		    });
		} catch (IOException e) {
			e.printStackTrace();
		} 
		
		
		// This checks whether RandomTrack is enabled and if true, generates a random SoundID
		// at which the playlist starts.
		// It was implemented before the randomly generated playlists and might be completely unnecessary.	
		Main.maxSoundCount = Main.currentSoundID;
		if (Main.randomTrack == true){
			Main.currentSoundID = Main.randomSoundID.nextInt(Main.maxSoundCount);
		}
		else{
			Main.currentSoundID = 0;
		}
		
		//Prints out the name of every music file in the folder
		System.out.println("Found the following files:");
		for (int i = 0; i < Main.maxSoundCount + 1; i++){
			System.out.println(Main.soundPathList[i]);
		}
		System.out.println("");
    }
}
