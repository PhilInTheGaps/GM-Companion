package application;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import org.jsoup.Jsoup;
import org.jsoup.nodes.Document;

import javafx.event.ActionEvent;
import javafx.geometry.Insets;
import javafx.scene.control.Button;
import javafx.scene.layout.TilePane;

public class SoundButtons {
	public static TilePane addSoundTilePane() throws IOException{
		System.out.println("Generating sound buttons...");
		
  		Main.tile2.setPadding(new Insets(Main.defaultPadding, Main.defaultPadding, Main.defaultPadding, Main.defaultPadding/2));
  		Main.tile2.setVgap(Main.defaultPadding/4);
  		Main.tile2.setHgap(Main.defaultPadding/4);
  		Main.tile2.setPrefColumns(3);
  		Main.tile2.setStyle("-fx-background-color: LightGrey");
  		Main.tile2.setPrefWidth(Main.defaultMusicAndSoundWidth);
  		//tile.setPrefHeight(Main.defaultHeight);
  		
  		Main.tile2.getChildren().clear();
  		
  		String[] folderArray = new String[500];
  		
  		File file = new File("Sounds/");
  		
  		if(Main.onlineMode){
  		//Get all foldernames from server
  			Document doc = Jsoup.connect(Main.serverSoundsURL).get();
  	        //System.out.println(doc.toString());
  	        String str = doc.toString();
  	        String findStr = "<li><a href=";
  	        int lastIndex1 = 0;
  	        int lastIndex2 = 10;
  	        ArrayList<String> folderNames = new ArrayList<String>();
  	        String test = new String();
  	        int count = 0;
  	        
  	        System.out.println("Found the following sound folders:");
  	        while(lastIndex1 != -1){

  	            lastIndex1 = str.indexOf(findStr,lastIndex1);
  	            lastIndex2 = str.indexOf("/", lastIndex1);
  	            
  	            
  	            if(lastIndex1 != -1){
  	            	for(int i = lastIndex1+findStr.length()+1; i < lastIndex2; i++){
  	            		test += str.charAt(i);
  	            	}
  	            	System.out.println(test);
  	            	folderNames.add(test);
  	            	test = "";
  	            	count += 1;
  	                lastIndex1 += findStr.length();
  	            }
  	        }
  	        for(int i = 0; i< folderNames.size(); i++){
  	        	String temp = folderNames.get(i).toString();
  	        	folderArray[i] = temp;
  	        }
  	        
  	        String[] folderArrayTemp = new String[500];
  	        for(int i = 1; i < count; i++){
  	        	//System.out.println(i);
  	        	//System.out.println(folderArray[i]);
				folderArrayTemp[i-1] = folderArray[i].toString();
  	        }
  	        folderArray = folderArrayTemp;
  	        
  		}
  		else{
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
  	  		String[] folderArrayTemp = new String[500];
	        for(int i = 0; i < folders.size()-1; i++){
	        	//System.out.println(i);
	        	//System.out.println(folders.get(i));
	        	folderArrayTemp[i] = folders.get(i).toString();
	        }
	        
	        folderArray = folderArrayTemp;
  		}
  		
  		for(int i  = 0; i < folderArray.length; i++){
				if(folderArray[i] != null){
					String bName = folderArray[i].toString();
		  			Button b = new Button(String.valueOf(i));
		  			
		  			b.setText(bName);
		  			b.setPrefSize(Main.defaultFolderButtonWidth, Main.defaultFolderButtonHeight);
		  			
		  			b.setOnAction((ActionEvent e) -> {
		  				if(Main.onlineMode){
		  					Main.defaultSoundPath = bName;
		  				}
		  				else{
		  					if(Main.linux == true){
			  	  				Main.defaultSoundPath = Main.defaultLinuxFolder + "Music/"+bName;
			  	  			}
			  	  			else{
			  	  				Main.defaultSoundPath = "Sounds/"+bName;
			  	  			}
		  				}
		  				
		  				Main.soundFolder = bName;
		  	  			Main.soundFolderLabel.setText("Folder: " + Main.soundFolder);
		  	  			Main.soundFolderSelected = true;
		  	  			
		  	  			if (Main.soundIsPlaying == true){
		  	  				Main.soundPlayer.stop();
		  	  			}
		  	  			
		  	  			try {
							getSoundFiles.get();
						} catch (IOException e1) {
							// TODO Auto-generated catch block
							e1.printStackTrace();
						}
		  	  			SPlayer.play();
		  	  		});
		  			
		  			Main.tile2.getChildren().add(b);
				}

  			}
		
		System.out.println("Added sound buttons");
		System.out.println("");
		return Main.tile2;
	}
}
