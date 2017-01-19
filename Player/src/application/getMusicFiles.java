package application;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.stream.Stream;

public class getMusicFiles {
	public static void get(){
    	
		//This finds every music file in the folder and writes them into a list
    	Main.currentTrackID = 0;
    	
    	for (int i = 0; i<500; i++){
    		Main.musicPathList[i] = "";
    	}
    	
		try(Stream<Path> paths = Files.walk(Paths.get(Main.defaultMusicPath))) {
		    paths.forEach(filePath -> {
		        if (Files.isRegularFile(filePath)) {
		            //System.out.println(filePath);
		            String tempPath = filePath.toString();
		            Main.musicPathList[Main.currentTrackID] = tempPath;
		            Main.currentTrackID ++;
		        }
		    });
		} catch (IOException e) {
			e.printStackTrace();
		} 
		
		// This checks whether RandomTrack is enabled and if true, generates a random TrackID
		// at which the playlist starts.
		// It was implemented before the randomly generated playlists and might be completely unnecessary.	
		Main.maxTrackCount = Main.currentTrackID;
		if (Main.randomTrack == true){
			Main.currentTrackID = Main.randomTrackID.nextInt(Main.maxTrackCount);
		}
		else{
			Main.currentTrackID = 0;
		}
		
		//Prints out the name of every music file in the folder
		for (int i = 0; i < Main.maxTrackCount + 1; i++){
			System.out.println(Main.musicPathList[i]);
		}
    }
}
