package application;

import java.io.File;
import java.util.ArrayList;
import java.util.List;

import javafx.event.ActionEvent;
import javafx.geometry.Insets;
import javafx.scene.control.Button;
import javafx.scene.layout.TilePane;

public class SoundButtons {
	public static TilePane addSoundTilePane(){
		System.out.println("Generating sound buttons...");
		
  		TilePane tile = new TilePane();
  		tile.setPadding(new Insets(20, 20, 20, 20));
  		tile.setVgap(3);
  		tile.setHgap(5);
  		tile.setPrefColumns(2);
  		tile.setStyle("-fx-background-color: LightGrey");
  		tile.setPrefWidth(Main.defaultMusicAndSoundWidth);
  		tile.setPrefHeight(Main.defaultHeight);
  		
  		File file = new File("Sounds/");
  		String[] names = file.list();
  		List<String> folders = new ArrayList<String>();
  		
  		System.out.println("Found the following sound folders:");
  		
  		for(String name : names)
  		{
  		    if (new File("Sounds/" + name).isDirectory())
  		    {
  		        System.out.println(name);
  		        folders.add(name);
  		    }
  		}
  		Object[] folderArray = folders.toArray();
  		
  		for(int i  = 0; i < folderArray.length; i++){
  			String bName = folderArray[i].toString();
  			Button b = new Button(String.valueOf(i));
  			
  			b.setText(bName);
  			b.setPrefSize(Main.defaultFolderButtonWidth, Main.defaultFolderButtonHeight);
  			
  			b.setOnAction((ActionEvent e) -> {
  				if(Main.linux == true){
  	  				Main.defaultSoundPath = Main.defaultLinuxFolder + "Sounds/"+bName;
  	  			}
  	  			else{
  	  				Main.defaultSoundPath = "Sounds/"+bName; 
  	  			}
  				
  				Main.soundFolder = bName;
  	  			Main.soundFolderLabel.setText("Folder: " + Main.soundFolder);
  	  			Main.soundFolderSelected = true;
  	  			
  	  			if (Main.soundIsPlaying == true){
  	  				Main.soundPlayer.stop();
  	  			}
  	  			getSoundFiles.get();
  	  			SPlayer.play();
  	  		});
  			
  			tile.getChildren().add(b);
  		}
  		System.out.println("Added sound buttons");
  		System.out.println("");
		return tile;
  	}
}
