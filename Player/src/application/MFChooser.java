package application;

import java.io.File;

import javafx.stage.FileChooser;
import javafx.stage.FileChooser.ExtensionFilter;

public class MFChooser {
	public static void choose(){
		FileChooser fc = new FileChooser();
		fc.setInitialDirectory(new File(Main.defaultMusicPath));
		fc.getExtensionFilters().addAll(new ExtensionFilter("Sound Files", "*.mp3"));
        File file = fc.showOpenDialog(null);
        
        Main.musicPath = file.getAbsolutePath();
        Main.musicPath = Main.musicPath.replace("\\", "/");
        
        MPlayer.play();
	}
}
