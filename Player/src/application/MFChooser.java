package application;

import java.io.File;

import javafx.stage.FileChooser;
import javafx.stage.FileChooser.ExtensionFilter;

public class MFChooser {
	//A FileChooser only used for debugging purposes, not in the normally implemented
	public static void choose(){
		System.out.println("Opening FileChooser...");
		System.out.println("");
		
		FileChooser fc = new FileChooser();
		fc.setInitialDirectory(new File(Main.defaultMusicPath));
		fc.getExtensionFilters().addAll(new ExtensionFilter("Sound Files", "*.mp3"));
        File file = fc.showOpenDialog(null);
        
        Main.musicPath = file.getAbsolutePath();
        Main.musicPath = Main.musicPath.replace("\\", "/");
        
        MPlayer.play();
	}
}
