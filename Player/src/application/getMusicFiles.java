package application;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.stream.Stream;

public class getMusicFiles {
public static void get(){
    	
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
		
		Main.maxTrackCount = Main.currentTrackID;
		if (Main.randomTrack == true){
			Main.currentTrackID = Main.randomTrackID.nextInt(Main.maxTrackCount);
		}
		else{
			Main.currentTrackID = 0;
		}
		
		for (int i = 0; i < Main.maxTrackCount + 1; i++){
			System.out.println(Main.musicPathList[i]);
		}
    }
}
