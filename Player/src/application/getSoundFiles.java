package application;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.stream.Stream;

public class getSoundFiles {
public static void get(){
    	
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
		
		Main.maxSoundCount = Main.currentSoundID;
		if (Main.randomTrack == true){
			Main.currentSoundID = Main.randomSoundID.nextInt(Main.maxSoundCount);
		}
		else{
			Main.currentSoundID = 0;
		}
		for (int i = 0; i < Main.maxSoundCount + 1; i++){
			System.out.println(Main.soundPathList[i]);
		}
    }
}
