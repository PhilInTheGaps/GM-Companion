package application;

import java.io.File;

import javafx.event.ActionEvent;
import javafx.geometry.Insets;
import javafx.scene.control.Button;
import javafx.scene.layout.TilePane;

public class MusicButtonsProcedually {
	public static TilePane addMusicTilePane(){
  		TilePane tile = new TilePane();
  		tile.setPadding(new Insets(20, 20, 20, 20));
  		tile.setVgap(3);
  		tile.setHgap(5);
  		tile.setPrefColumns(2);
  		tile.setStyle("-fx-background-color: LightGrey");
  		tile.setPrefWidth(Main.defaultMusicAndSoundWidth);
  		tile.setPrefHeight(Main.defaultHeight);
  		
  		File file = new File("/Music");
  		String[] names = file.list();

  		for(String name : names)
  		{
  		    if (new File("/Music" + name).isDirectory())
  		    {
  		        System.out.println(name);
  		    }
  		}
  		
  		for(int i  = 0; i <5; i++){
  			String bName = "Button " + i;
  			Button b = new Button(String.valueOf(i));
  			b.setText(bName);
  			b.setPrefSize(Main.defaultFolderButtonWidth, Main.defaultFolderButtonHeight);
  			
  			b.setOnAction((ActionEvent e) -> {
  	  			System.out.println(bName);
  	  		});
  			
  			tile.getChildren().add(b);
  		}
  		
  		/*
  		//Action
  		Button Action = new Button();
  		Action.setText("Action");
  		Action.setPrefWidth(Main.defaultFolderButtonWidth);
  		Action.setPrefHeight(Main.defaultFolderButtonHeight);
  		Action.setOnAction((ActionEvent e) -> {
  			if(Main.linux == true){
  				Main.defaultMusicPath = Main.defaultLinuxFolder + "Music/Action";
  			}
  			else{
  				Main.defaultMusicPath = "Music/Action"; 
  			}
  			
  			Main.musicFolder = "Action";
  			Main.musicFolderLabel.setText("Folder: " + Main.musicFolder);
  			Main.musicFolderSelected = true;
  			
  			if (Main.musicIsPlaying == true){
  				Main.mediaPlayer.stop();
  			}
  			getMusicFiles.get();
  			MPlayer.play();
  		});
  		*/

  		
  		//Add every Button
  		/*tile.getChildren().addAll(/*Action, SANDSTORM, 
  								  TheBearAndTheMaidenFair, TheRainsOfCastamere); */
  		
  		/*
  		Action 		Dramatisch 		Bedrueckend
  		Taverne		Entspannt		HE
  		Insel		Stadt			Reise
  		PIRATEN
  		SciFiAction	SciFiDramatisch	SciFiBedrueckend
  		SciFiReise	SciFiEntspannt	SciFiFroehlich
  		SciFiTraurigSciFiMarkt		SciFiLounge	
  		JazzLounge		
  		Credits		DnS				RandomOpener
  		SANDSTORM	TheBearAndTheMaidenFair		TheRainsOfCastamere
  		*/
  		
		return tile;
  	}
}
