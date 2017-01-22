package application;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import org.jsoup.Jsoup;
import org.jsoup.nodes.Document;
import org.jsoup.nodes.Element;

import javafx.event.ActionEvent;
import javafx.geometry.Insets;
import javafx.scene.control.Button;
import javafx.scene.layout.Region;
import javafx.scene.layout.TilePane;

public class MusicButtons {
	public static TilePane addMusicTilePane() throws IOException{
		System.out.println("Generating music buttons...");
		
  		Main.tile.setPadding(new Insets(Main.defaultPadding, Main.defaultPadding/2, Main.defaultPadding, Main.defaultPadding));
  		Main.tile.setVgap(Main.defaultPadding/4);
  		Main.tile.setHgap(Main.defaultPadding/4);
  		Main.tile.setPrefColumns(3);
  		Main.tile.setStyle("-fx-background-color: LightGrey");
  		Main.tile.setPrefWidth(Main.defaultMusicAndSoundWidth);
  		//tile.setPrefHeight(Main.defaultHeight);
  		
  		Main.tile.getChildren().clear();
  		
  		String[] folderArray = new String[500];
  		String[] folderArrayTemp = new String[500];
  		List<String> folders = new ArrayList<String>();
  		
  		File file = new File("Music/");
  		
  		if(Main.onlineMode){
  			//Get all foldernames from server
  			Document doc = Jsoup.connect(Main.serverMusicURL).get();
  	        //System.out.println(doc.toString());
  	        String str = doc.toString();
  	        String findStr = "<li><a href=";
  	        int lastIndex1 = 0;
  	        int lastIndex2 = 10;
  	        ArrayList<String> folderNames = new ArrayList<String>();
  	        String test = new String();
  	        int count = 0;
  	        
  	        System.out.println("Found the following music folders:");
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
  	        
  	        for(int i = 1; i < count; i++){
  	        	//System.out.println(i);
  	        	//System.out.println(folderArray[i]);
				folderArrayTemp[i-1] = folderArray[i].toString();
  	        }
  	        folderArray = folderArrayTemp;
  	        
  		}
  		else{
  			String[] names = file.list();
  	  		
  	  		
  	  		System.out.println("Found the following music folders:");
  	  		
  	  		for(String name : names)
  	  		{
  	  		    if (new File("Music/" + name).isDirectory())
  	  		    {
  	  		        System.out.println(name);
  	  		        folders.add(name);
  	  		    }
  	  		}
  	  		
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
  		  					Main.defaultMusicPath = bName;
  		  				}
  		  				else{
  		  					if(Main.linux == true){
  			  	  				Main.defaultMusicPath = Main.defaultLinuxFolder + "Music/"+bName;
  			  	  			}
  			  	  			else{
  			  	  				Main.defaultMusicPath = "Music/"+bName; 
  			  	  			}
  		  				}
  		  				
  		  				Main.musicFolder = bName;
  		  	  			Main.musicFolderLabel.setText("Folder: " + Main.musicFolder);
  		  	  			Main.musicFolderSelected = true;
  		  	  			
  		  	  			Main.initialPress = true;
  		  	  			
  		  	  			if (Main.musicIsPlaying == true){
  		  	  				Main.mediaPlayer.stop();
  		  	  			}
  		  	  			
  		  	  			try {
							getMusicFiles.get();
						} catch (IOException e1) {
							e1.printStackTrace();
						}
  		  	  			MPlayer.play();
  		  	  		});
  		  			
  		  			Main.tile.getChildren().add(b);
  				}

	  		}
  		
  		//Adjusting Music Button width
        double buttonsFittingIn = (Main.defaultMusicAndSoundWidth-2*Main.defaultPadding-(Main.buttonRowCount-1)*Main.defaultPadding/4)/Main.defaultFolderButtonWidth+0.1;
        double availableSpace = (Main.defaultMusicAndSoundWidth-2*Main.defaultPadding-(Main.buttonRowCount-1)*Main.defaultPadding/4);
        Main.buttonRowCount = (int) Math.floor(buttonsFittingIn+0.1);
        System.out.println("Buttons Fitting in: "+buttonsFittingIn);
        System.out.println("Available Space: "+availableSpace);
        Object[] bArrayMusic = Main.tile.getChildren().toArray();
        int bCountMusic = bArrayMusic.length;
    	
    	double currentWidth = Main.defaultFolderButtonWidth;
        currentWidth = Main.currentWidth;//(buttonsFittingIn/(Main.buttonRowCount))*Main.defaultFolderButtonWidth;
        System.out.println("CurrentWidth: "+currentWidth);
        System.out.println("Space/Width: "+availableSpace/currentWidth);
    	for(int i = 0; i < bCountMusic; i++){
    		((Region) bArrayMusic[i]).setPrefWidth(currentWidth-1);
    	}
  		
  		System.out.println("Added music buttons");
  		System.out.println("");
		return Main.tile;
  	}
}
