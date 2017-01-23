package application;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import org.jsoup.Jsoup;
import org.jsoup.nodes.Document;

import javafx.beans.InvalidationListener;
import javafx.beans.Observable;
import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.ProgressBar;
import javafx.scene.control.Slider;
import javafx.scene.control.TextField;
import javafx.scene.image.ImageView;
import javafx.scene.input.KeyCode;
import javafx.scene.input.KeyEvent;
import javafx.scene.layout.GridPane;
import javafx.scene.layout.HBox;
import javafx.scene.layout.Region;
import javafx.scene.layout.TilePane;
import javafx.scene.layout.VBox;
import javafx.stage.FileChooser;
import javafx.stage.FileChooser.ExtensionFilter;
import javafx.util.Duration;

public class UI {
	
	public static Slider mVolumeSlider;
	public static Slider sVolumeSlider;
	public static String osName;
	public static Boolean windows = false;
	public static Boolean linux = false;
	public static Label titleLabel = new Label();
	public static Label albumLabel = new Label();
	public static Label artistLabel = new Label();
	public static Label yearLabel = new Label();
	public static Label musicFolderLabel = new Label();
	public static Label soundFolderLabel = new Label();
	public static ImageView coverImage = new ImageView();
	public static GridPane grid = new GridPane();
	public static HBox botBox = new HBox();
	public static ProgressBar pb = new ProgressBar();
	public static HBox toolBar1 = new HBox();
	public static HBox toolBar2 = new HBox();
	public static TilePane tile = new TilePane();
	public static TilePane tile2 = new TilePane();
	
	public static Boolean autoplay = true;
	public static Boolean randomTrack = true;
	public static Boolean singleTrack = false;
	public static Boolean debug = false;
	public static Boolean devV = false;
	public static Boolean onlineMode = false;
	public static Boolean localOnline = onlineMode;
	
	public static String Album = "Unknown";
	public static String Title = "Unknown";
	public static String Artist = "Unknown";
	public static String Year = "";
	public static String mainPath;
	public static String musicFolder = "Not Chosen";
	public static String soundFolder = "Not Chosen";
	
	public static String serverURL = "http://192.168.178.55/"; //http://192.168.178.55/
	
	public static String defaultLinuxFolder = "/home/phil/RPG Music Player/";
	
	public static double defaultWidth = 1366;
	public static double defaultHeight = 768;
	
	public static int buttonRowCount;
	public static double currentWidth;
	
	//The default space between different elements like buttons
	public static double defaultSpacing = 10;
	public static double defaultPadding = 10;
	
	//Default values of the buttons in the top toolbar
	public static double defaultButtonHeight = 50;
	public static double defaultButtonWidth = 175; //Currently not used, generates from window width
	
	//The size of the volume sliders
	public static double defaultSliderHeight = 50;
	public static double defaultSliderWidth = 320;
	
	public static double defaultMusicAndSoundWidth = (defaultWidth - defaultSliderWidth - 2*defaultPadding)/2; //(defaultFolderButtonWidth*3 + 3*defaultPadding)
	public static double defaultFolderButtonWidth = (defaultMusicAndSoundWidth - 3*defaultPadding)/3;
	public static double defaultFolderButtonHeight = 60;
	
	
	//Adds the toolbar on the top
	public static VBox addToolBar() {
		System.out.println("Adding toolbar...");
		//Adding a VBox that contains 2 HBoxes
		VBox toolBox = new VBox();
		
  		//Setting ToolBar1 Style
  		toolBar1.setPadding(new Insets(defaultPadding/2, defaultPadding/2, defaultPadding/4, defaultPadding/2));
  		toolBar1.setAlignment(Pos.CENTER_LEFT);
  		toolBar1.alignmentProperty().isBound();
  		toolBar1.setSpacing(defaultSpacing/2);
  		toolBar1.setStyle("-fx-background-color: Grey");
  		toolBar1.setMinHeight(50);
  		toolBar1.setMaxHeight(50);
  		
  		//Setting ToolBar2 Style
  		toolBar2.setPadding(new Insets(defaultPadding/4, defaultPadding/2, defaultPadding/2, defaultPadding/2));
  		toolBar2.setAlignment(Pos.CENTER_LEFT);
  		toolBar2.alignmentProperty().isBound();
  		toolBar2.setSpacing(defaultSpacing/2);
  		toolBar2.setStyle("-fx-background-color: Grey");
  		toolBar2.setMinHeight(50);
  		toolBar2.setMaxHeight(50);
  		
  		//Play Button
  		Button playButton = new Button();
  		playButton.setText("Play / Continue");
  		playButton.setPrefHeight(defaultButtonHeight);
  		playButton.setOnAction((ActionEvent e) ->{
  			if(Music.musicFolderSelected == true){
  				Music.mediaPlayer.play();
  			}
  			else{
  				Music.musicError = "Please select music folder!";
  			}
  			if(Sound.soundFolderSelected == true){
  				Sound.soundPlayer.play();
  			}
  			else{
  				Sound.soundError = "Please select sound folder!";
  			}
  			
  		});
  		
  		//Pause Button
  		Button pauseButton = new Button();
  		pauseButton.setText("Pause Both");
  		pauseButton.setPrefHeight(defaultButtonHeight);
  		pauseButton.setOnAction((ActionEvent e) -> {
  			if(Music.musicFolderSelected == true){
  				Music.mediaPlayer.pause();
  			}
  			else{
  				Music.musicError = "Please select music folder!";
  			}
  			if(Sound.soundFolderSelected == true){
  				Sound.soundPlayer.pause();
  			}
  			else{
  				Sound.soundError = "Please select sound folder!";
  			}
  		});
  		
  		//Pause Music Button
  		Button pauseMButton = new Button();
  		pauseMButton.setText("Pause Music");
  		pauseMButton.setPrefHeight(defaultButtonHeight);
  		pauseMButton.setOnAction((ActionEvent e) -> {
  			if(Music.musicFolderSelected == true){
  				Music.mediaPlayer.pause();
  			}
  			else{
  				Music.musicError = "Please select sound folder!";
  			}
  		});
  		
  		//Pause Sound Button
  		Button pauseSButton = new Button();
  		pauseSButton.setText("Pause Sound");
  		pauseSButton.setPrefHeight(defaultButtonHeight);
  		pauseSButton.setOnAction((ActionEvent e) -> {
  			if(Sound.soundFolderSelected == true){
  				Sound.soundPlayer.pause();
  			}
  			else{
  				Sound.soundError = "Please select sound folder!";
  			}
  		});
  		
  		//Files Button
  		Button filesButton = new Button();
  		filesButton.setText("Files");
  		filesButton.setPrefHeight(defaultButtonHeight);
  		filesButton.setOnAction((ActionEvent e) -> {
  			Music.mediaPlayer.stop();
  			chooser();
  		});
  		
  		//Reload Music Button
  		Button reloadMButton = new Button();
  		reloadMButton.setText("Replay Track");
  		reloadMButton.setPrefHeight(defaultButtonHeight);
  		reloadMButton.setOnAction((ActionEvent e) -> {
  			if(Music.musicFolderSelected == true){
  				Music.mediaPlayer.seek(Music.mediaPlayer.getStartTime());
  			}
  			else{
  				Music.musicError = "Please select music folder!";
  			}
  			
  		});
  		
  		//Reload Sound Button
  		Button reloadSButton = new Button();
  		reloadSButton.setText("Replay Sound");
  		reloadSButton.setPrefHeight(defaultButtonHeight);
  		reloadSButton.setOnAction((ActionEvent e) -> {
  			if(Sound.soundFolderSelected == true){
  				Sound.soundPlayer.seek(Sound.soundPlayer.getStartTime());
  			}
  			else{
  				Sound.soundError = "Please select sound folder!";
  			}
  			
  		});
  		
  		//Next Music Button
  		Button nextMButton = new Button();
  		nextMButton.setText("Next Song");
  		nextMButton.setPrefHeight(defaultButtonHeight);
  		nextMButton.setOnAction((ActionEvent e) -> {
  			if(Music.musicFolderSelected == true){
  				Music.next();
  			}
  			else{
  				Music.musicError = "Please select music folder!";
  			}
  			
  		});
  		
  		//Next Sound Button
  		Button nextSButton = new Button();
  		nextSButton.setText("Next Sound");
  		nextSButton.setPrefHeight(defaultButtonHeight);
  		nextSButton.setOnAction((ActionEvent e) -> {
  			if(Sound.soundFolderSelected == true){
  				Sound.next();
  			}
  			else{
  				Sound.soundError = "Please select sound folder!";
  			}
  			
  		});
  		
  		//Toggle Random
  		Button toggleRandomButton = new Button();
  		toggleRandomButton.setPrefHeight(defaultButtonHeight);
  		if(randomTrack == true){
  			toggleRandomButton.setText("Disable Random Mode");
  		}
  		else{
  			toggleRandomButton.setText("Enable Random Mode");
  		}
  		toggleRandomButton.setOnAction((ActionEvent e) -> {
  			if(randomTrack == true){
  				randomTrack = false;
  				toggleRandomButton.setText("Enable Random Mode");
  			}
  			else{
  				randomTrack = true;
  				toggleRandomButton.setText("Disable Random Mode");
  			}
  			
  		});
  		
  		//Toggle Single Track
  		Button toggleSingleButton = new Button();
  		toggleSingleButton.setPrefHeight(defaultButtonHeight);
  		if(singleTrack == true){
  			toggleSingleButton.setText("Disable Single Mode");
  		}
  		else{
  			toggleSingleButton.setText("Enable Single Mode");
  		}
  		toggleSingleButton.setOnAction((ActionEvent e) -> {
  			if(singleTrack == true){
  				singleTrack = false;
  				toggleSingleButton.setText("Enable Single Mode");
  			}
  			else{
  				singleTrack = true;
  				toggleSingleButton.setText("Disable Single Mode");
  			}
  			
  		});
  		
  		
  		//Online Mode Button
  		Button toggleOnline = new Button();
  		toggleOnline.setPrefHeight(defaultButtonHeight);
  		if(onlineMode == true){
  			toggleOnline.setText("Use Local Files");
  		}
  		else{
  			toggleOnline.setText("Use Server Files");
  		}
  		toggleOnline.setOnAction((ActionEvent e) -> {
  			if(localOnline == true){
  				localOnline = false;
  				toggleOnline.setText("Use Server Files");
  			}
  			else{
  				localOnline = true;
  				toggleOnline.setText("Use Local Files");
  			}
  			
  		});
  		
  		//SpacerLabel1
  		Label spacerLabel1 = new Label();
  		spacerLabel1.setPrefWidth(150);
  		
  		//SpacerLabel1
  		Label spacerLabel2 = new Label();
  		spacerLabel2.setPrefWidth(150);
  		
  		//Server URL
  		TextField serverField = new TextField();
  		serverField.setPromptText("Server URL");
  		serverField.setPrefHeight(defaultButtonHeight);
  		
  		//Set Server URL
  		Button setServerURL = new Button();
  		setServerURL.setPrefHeight(defaultButtonHeight);
  		//setServerURL.setPrefWidth(75);
  		setServerURL.setText("Set URL");
  		setServerURL.setOnAction((ActionEvent e) -> {
  			serverURL = serverField.getText();
  			int i = serverURL.length();
  			if(serverURL.charAt(i-1)!= ("/").toCharArray()[0]){
  				serverURL = serverURL + "/";
  			}
  			if(serverURL.contains("http://") == false){
  				serverURL = "http://" + serverURL;
  			}
  			Music.serverMusicURL = serverURL + "music/";
  			Sound.serverSoundsURL = serverURL + "sounds/";
  		});
  		
  		//Update Folders
  		Button updateFolders = new Button();
  		updateFolders.setPrefHeight(defaultButtonHeight);
  		//updateFolders.setPrefWidth(75);
  		updateFolders.setText("Update");
  		updateFolders.setOnAction((ActionEvent e) -> {
  			System.out.println("Updating Folders...");
  			try {
  				onlineMode = localOnline;
  				if(Music.musicFolderSelected == true){
  					Music.mediaPlayer.pause();
  	  			}
  	  			if(Sound.soundFolderSelected == true){
  	  				Sound.soundPlayer.pause();
  	  			}
  	  			
  	  			/*
  	  			borderPane.setCenter(null);
	  			borderPane.setRight(null);
  	  			
  	  			borderPane.setCenter(MusicButtons.addMusicTilePane());
  	  			borderPane.setRight(SoundButtons.addSoundTilePane());
  	  			*/
  	  			
				UI.addMusicTilePane();
				UI.addSoundTilePane();
				
				
			} catch (IOException e1) {
				e1.printStackTrace();
			}
  		});
  		
  		//Add everything to ToolBar
  		toolBar1.getChildren().addAll(playButton, pauseMButton, reloadMButton, nextMButton, toggleRandomButton, toggleOnline, updateFolders);
  		toolBar2.getChildren().addAll(pauseButton, pauseSButton, reloadSButton, nextSButton, toggleSingleButton, serverField, setServerURL);
  		
  		//Set Button Width
  		//int buttonCount = toolBar1.getChildren().toArray().length;
  		//double defaultButtonWidth = defaultButtonWidth; //(defaultWidth - ((buttonCount+1)*defaultPadding)) / buttonCount
  		
  		Object[] bArray1 = toolBar1.getChildren().toArray();
        Object[] bArray2 = toolBar2.getChildren().toArray();
        int bCount = bArray1.length;
        defaultButtonWidth = defaultWidth/bCount;
        
  		for(int i = 0; i < bCount; i++){
        	((Region) bArray1[i]).setPrefWidth(defaultButtonWidth);
        	if (bArray2[i] != null){
        		((Region) bArray2[i]).setPrefWidth(defaultButtonWidth);
        	}
        }
  		
  		toolBox.getChildren().add(toolBar1);
  		toolBox.getChildren().add(toolBar2);
  		
  		return toolBox;
  		}
	
	//Adds the VerticalBox on the left
	public static VBox addVBox(){
		System.out.println("Adding MediaInfo Box...");
  		VBox vBox = new VBox();
  		vBox.setPadding(new Insets(defaultPadding));
  		vBox.setStyle("-fx-background-color: White");
  		vBox.setMaxWidth(defaultSliderWidth+2*defaultPadding);
  		
  		Label mVolumeLabel = new Label();
  		mVolumeLabel.setText("Music Volume:");
  		vBox.getChildren().add(mVolumeLabel);
  		
  		vBox.getChildren().clear();
  		
  		//mVolume Slider
  		mVolumeSlider = new Slider();
  		mVolumeSlider.setPrefWidth(defaultSliderWidth);
  		mVolumeSlider.setPrefHeight(defaultSliderHeight);
  		mVolumeSlider.setMinWidth(30);
  		mVolumeSlider.setValue(50);
  		
  		mVolumeSlider.valueProperty().addListener(new InvalidationListener() {
  		    public void invalidated(Observable ov) {
  		       if (mVolumeSlider.isPressed()) {
  		    	   Music.musicVolume = mVolumeSlider.getValue() / 100.0;
  		           if(Music.musicIsPlaying == true){
  		        	 Music.mediaPlayer.setVolume(mVolumeSlider.getValue() / 100.0);
  		           }
  		       }
  		    }
  		});
  		vBox.getChildren().add(mVolumeSlider);
  		
  		Label sVolumeLabel = new Label();
  		sVolumeLabel.setText("Sound Volume:");
  		vBox.getChildren().add(sVolumeLabel);
  		
  		//sVolume Slider
  		sVolumeSlider = new Slider();
  		sVolumeSlider.setPrefWidth(defaultSliderWidth);
  		sVolumeSlider.setPrefHeight(defaultSliderHeight);
  		sVolumeSlider.setMinWidth(30);
  		sVolumeSlider.setValue(25);
  		
  		sVolumeSlider.valueProperty().addListener(new InvalidationListener() {
  		    public void invalidated(Observable ov) {
  		    	if (sVolumeSlider.isPressed()){
  		    		Sound.soundVolume = sVolumeSlider.getValue() / 100.0;
  		    		if(Sound.soundIsPlaying == true){
  		    			Sound.soundPlayer.setVolume(sVolumeSlider.getValue() / 100.0);
  		    		}
  		    	}
	    	}
  		});
  		vBox.getChildren().add(sVolumeSlider);
  		
  		//Music Information
  		Label spacerLabel2 = new Label();
  		spacerLabel2.setText("");
  		vBox.getChildren().add(spacerLabel2);
  		
  		Label trackLabel = new Label();
  		trackLabel.setText("Music Track Information:");
  		vBox.getChildren().add(trackLabel);
  		
  		musicFolderLabel.setText("Folder: " + musicFolder);
  		vBox.getChildren().add(musicFolderLabel);
  		
  		titleLabel.setText("Title: " + Title);
  		vBox.getChildren().add(titleLabel);
  		
  		albumLabel.setText("Album: " + Title);
  		vBox.getChildren().add(albumLabel);
  		
  		artistLabel.setText("Artist: " + Title);
  		vBox.getChildren().add(artistLabel);
  		
  		yearLabel.setText("Year: " + Title);
  		vBox.getChildren().add(yearLabel);
  		
  		//Sound Information
  		Label spacerLabelS = new Label();
  		spacerLabelS.setText("");
  		vBox.getChildren().add(spacerLabelS);
  		
  		Label soundInfoLabel = new Label();
  		soundInfoLabel.setText("Sound Information:");
  		vBox.getChildren().add(soundInfoLabel);
  		
  		soundFolderLabel.setText("Folder: " + soundFolder);
  		vBox.getChildren().add(soundFolderLabel);
  		
  		//Debug Information
  		Label spacerLabel = new Label();
  		Label spacerLabel3 = new Label();
  		Label debugLabel = new Label();
  		Label pathLabel = new Label();
  		Label pathLabel2 = new Label();
  		Label pathLabel3 = new Label();
  		Label osLabel = new Label();
  		
  		Button debugButton = new Button();
  		debugButton.setText("Debug Mode");
  		debugButton.setOnAction((ActionEvent e) ->{
  			if(debug){
  				debug = false;
  				vBox.getChildren().remove(spacerLabel);
  				vBox.getChildren().remove(debugLabel);
  				vBox.getChildren().remove(pathLabel);
  				vBox.getChildren().remove(pathLabel2);
  				vBox.getChildren().remove(pathLabel3);
  				vBox.getChildren().remove(osLabel);
  			}
  			else{
  				debug = true;
  		  		spacerLabel.setText("");
  		  		vBox.getChildren().add(spacerLabel);
  		  		
  		  		debugLabel.setText("Debug Information:");
  		  		vBox.getChildren().add(debugLabel);
  		  		
  		  		pathLabel.setText(".jar Path: " + mainPath);
  		  		vBox.getChildren().add(pathLabel);
  		  		
  		  		pathLabel2.setText("Music Path: " + Music.defaultMusicPath);
  		  		vBox.getChildren().add(pathLabel2);
  		  		
  		  		pathLabel3.setText("Sound Path: " + Sound.defaultSoundPath);
  		  		vBox.getChildren().add(pathLabel3);
  		  		
  		  		osLabel.setText("OS: " + osName);
  		  		vBox.getChildren().add(osLabel);
  			}
  		});
  		if(devV){
  			vBox.getChildren().add(spacerLabel3);
  			vBox.getChildren().add(debugButton);
  		}
  		
  		if(debug){
  		
  		spacerLabel.setText("");
  		vBox.getChildren().add(spacerLabel);
  		
  		debugLabel.setText("Debug Information:");
  		vBox.getChildren().add(debugLabel);
  		
  		pathLabel.setText(".jar Path: " + mainPath);
  		vBox.getChildren().add(pathLabel);
  		
  		pathLabel2.setText("Music Path: " + Music.defaultMusicPath);
  		vBox.getChildren().add(pathLabel2);
  		
  		pathLabel3.setText("Sound Path: " + Sound.defaultSoundPath);
  		vBox.getChildren().add(pathLabel3);
  		
  		osLabel.setText("OS: " + osName);
  		vBox.getChildren().add(osLabel);
  		}
  		 
  		//Set Cover Image
  		//coverImage.setFitHeight(defaultSliderWidth-30);
  		Label coverSpacerLabel = new Label();
  		vBox.getChildren().add(coverSpacerLabel);
  		coverImage.setFitWidth(defaultSliderWidth);
  		coverImage.setPreserveRatio(true);
  		vBox.getChildren().add(coverImage);
  		
  		return vBox;
  	}
	
	//A FileChooser only used for debugging purposes, normally not implemented
	public static void chooser(){
		System.out.println("Opening FileChooser...");
		System.out.println("");
		
		FileChooser fc = new FileChooser();
		fc.setInitialDirectory(new File(Music.defaultMusicPath));
		fc.getExtensionFilters().addAll(new ExtensionFilter("Sound Files", "*.mp3"));
        File file = fc.showOpenDialog(null);
        
        Music.musicPath = file.getAbsolutePath();
        Music.musicPath = Music.musicPath.replace("\\", "/");
        
        Music.play();
	}
	
	public static TilePane addMusicTilePane() throws IOException{
		System.out.println("Generating music buttons...");
		
  		tile.setPadding(new Insets(defaultPadding, defaultPadding/2, defaultPadding, defaultPadding));
  		tile.setVgap(defaultPadding/4);
  		tile.setHgap(defaultPadding/4);
  		tile.setPrefColumns(3);
  		tile.setStyle("-fx-background-color: LightGrey");
  		tile.setPrefWidth(defaultMusicAndSoundWidth);
  		//tile.setPrefHeight(defaultHeight);
  		
  		tile.getChildren().clear();
  		
  		String[] folderArray = new String[500];
  		String[] folderArrayTemp = new String[500];
  		List<String> folders = new ArrayList<String>();
  		
  		File file = new File("Music/");
  		
  		if(onlineMode){
  			//Get all foldernames from server
  			Document doc = Jsoup.connect(Music.serverMusicURL).get();
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
  		  			b.setPrefSize(defaultFolderButtonWidth, defaultFolderButtonHeight);
  		  			
  		  			b.setOnAction((ActionEvent e) -> {
  		  				if(onlineMode){
  		  					Music.defaultMusicPath = bName;
  		  				}
  		  				else{
  		  					if(linux == true){
  		  						Music.defaultMusicPath = defaultLinuxFolder + "Music/"+bName;
  			  	  			}
  			  	  			else{
  			  	  				Music.defaultMusicPath = "Music/"+bName; 
  			  	  			}
  		  				}
  		  				
  		  				musicFolder = bName;
  		  	  			musicFolderLabel.setText("Folder: " + musicFolder);
  		  	  			Music.musicFolderSelected = true;
  		  	  			
  		  	  			Music.initialPress = true;
  		  	  			
  		  	  			if (Music.musicIsPlaying == true){
  		  	  				Music.mediaPlayer.stop();
  		  	  			}
  		  	  			
  		  	  			try {
  		  	  				Music.get();
						} catch (IOException e1) {
							e1.printStackTrace();
						}
  		  	  			Music.play();
  		  	  		});
  		  			
  		  			tile.getChildren().add(b);
  				}

	  		}
  		
  		//Adjusting Music Button width
        double buttonsFittingIn = (defaultMusicAndSoundWidth-2*defaultPadding-(buttonRowCount-1)*defaultPadding/4)/defaultFolderButtonWidth+0.1;
        double availableSpace = (defaultMusicAndSoundWidth-2*defaultPadding-(buttonRowCount-1)*defaultPadding/4);
        buttonRowCount = (int) Math.floor(buttonsFittingIn+0.1);
        System.out.println("Buttons Fitting in: "+buttonsFittingIn);
        System.out.println("Available Space: "+availableSpace);
        Object[] bArrayMusic = tile.getChildren().toArray();
        int bCountMusic = bArrayMusic.length;
    	
    	double currentWidth = defaultFolderButtonWidth;
        //currentWidth = currentWidth;//(buttonsFittingIn/(buttonRowCount))*defaultFolderButtonWidth;
        System.out.println("CurrentWidth: "+currentWidth);
        System.out.println("Space/Width: "+availableSpace/currentWidth);
    	for(int i = 0; i < bCountMusic; i++){
    		((Region) bArrayMusic[i]).setPrefWidth(currentWidth-1);
    	}
  		
  		System.out.println("Added music buttons");
  		System.out.println("");
		return tile;
  	}
	
	public static TilePane addSoundTilePane() throws IOException{
		System.out.println("Generating sound buttons...");
		
  		tile2.setPadding(new Insets(defaultPadding, defaultPadding, defaultPadding, defaultPadding/2));
  		tile2.setVgap(defaultPadding/4);
  		tile2.setHgap(defaultPadding/4);
  		tile2.setPrefColumns(3);
  		tile2.setStyle("-fx-background-color: LightGrey");
  		tile2.setPrefWidth(defaultMusicAndSoundWidth);
  		//tile.setPrefHeight(defaultHeight);
  		
  		tile2.getChildren().clear();
  		
  		String[] folderArray = new String[500];
  		
  		File file = new File("Sounds/");
  		
  		if(onlineMode){
  		//Get all foldernames from server
  			Document doc = Jsoup.connect(Sound.serverSoundsURL).get();
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
		  			b.setPrefSize(currentWidth-1, defaultFolderButtonHeight);
		  			
		  			b.setOnAction((ActionEvent e) -> {
		  				if(onlineMode){
		  					Sound.defaultSoundPath = bName;
		  				}
		  				else{
		  					if(linux == true){
		  						Sound.defaultSoundPath = defaultLinuxFolder + "Music/"+bName;
			  	  			}
			  	  			else{
			  	  			Sound.defaultSoundPath = "Sounds/"+bName;
			  	  			}
		  				}
		  				
		  				soundFolder = bName;
		  	  			soundFolderLabel.setText("Folder: " + soundFolder);
		  	  			Sound.soundFolderSelected = true;
		  	  			
		  	  			Sound.initialPress = true;
		  	  			
		  	  			if (Sound.soundIsPlaying == true){
		  	  				Sound.soundPlayer.stop();
		  	  			}
		  	  			
		  	  			try {
		  	  				Sound.get();
						} catch (IOException e1) {
							// TODO Auto-generated catch block
							e1.printStackTrace();
						}
		  	  			Sound.play();
		  	  		});
		  			
		  			tile2.getChildren().add(b);
				}

  			}
		
		System.out.println("Added sound buttons");
		System.out.println("");
		return tile2;
	}
	
	public static void updatePB(){
		ChangeListener<Duration> progressChangeListener;
		progressChangeListener = new ChangeListener<Duration>(){
			@Override public void changed(ObservableValue<? extends Duration> observableValue, Duration oldValue, Duration newValue) {
		        pb.setProgress(1.0 * Music.mediaPlayer.getCurrentTime().toMillis() / Music.mediaPlayer.getTotalDuration().toMillis());
		      }
			
		};
		Music.mediaPlayer.currentTimeProperty().addListener(progressChangeListener);

	}
	
	public static void test(){
		Main.scene.widthProperty().addListener(new ChangeListener<Number>() {
            @Override 
            public void changed(ObservableValue<? extends Number> observableValue, Number oldSceneWidth, Number newSceneWidth) {
                //System.out.println("Width: " + newSceneWidth);
            	UI.defaultWidth = (double) newSceneWidth;
            	UI.defaultMusicAndSoundWidth = (UI.defaultWidth - UI.defaultSliderWidth - 2*UI.defaultPadding)/2;
                UI.tile.setPrefWidth(UI.defaultMusicAndSoundWidth);
                UI.tile.setMinWidth(UI.defaultMusicAndSoundWidth);
                UI.tile2.setPrefWidth(UI.defaultMusicAndSoundWidth);
                UI.tile2.setMinWidth(UI.defaultMusicAndSoundWidth);
                UI.toolBar1.setPrefWidth(UI.defaultWidth);
                Object[] bArray1 = UI.toolBar1.getChildren().toArray();
                Object[] bArray2 = UI.toolBar2.getChildren().toArray();
                int bCount = bArray1.length;
                Object[] bArrayMusic = UI.tile.getChildren().toArray();
                int bCountMusic = bArrayMusic.length;
                Object[] bArraySounds = UI.tile2.getChildren().toArray();
                int bCountSounds = bArraySounds.length;
                UI.defaultButtonWidth = UI.defaultWidth/bCount;
                
                //Adjusting Music and Sound Button width
                double buttonsFittingIn = (UI.defaultMusicAndSoundWidth-2*UI.defaultPadding-(UI.buttonRowCount-1)*UI.defaultPadding/4)/UI.defaultFolderButtonWidth;
                double availableSpace = (UI.defaultMusicAndSoundWidth-2*UI.defaultPadding-(UI.buttonRowCount-1)*UI.defaultPadding/4);
                UI.buttonRowCount = (int) Math.floor(buttonsFittingIn+0.1);
                System.out.println("Buttons Fitting in: "+buttonsFittingIn);
                System.out.println("Available Space: "+availableSpace);
            	
            	
                UI.currentWidth = (buttonsFittingIn/(UI.buttonRowCount))*UI.defaultFolderButtonWidth;
                System.out.println("CurrentWidth: "+UI.currentWidth);
                System.out.println("Space/Width: "+availableSpace/UI.currentWidth);
            	for(int i = 0; i < bCountMusic; i++){
            		((Region) bArrayMusic[i]).setPrefWidth(UI.currentWidth-1);
            	}
            	for(int i = 0; i < bCountSounds; i++){
            		((Region) bArraySounds[i]).setPrefWidth(UI.currentWidth-1);
            	}
            	
            	System.out.println("");
            	
            	//Adjusting ToolBar button width
                for(int i = 0; i < bCount; i++){
                	((Region) bArray1[i]).setPrefWidth(UI.defaultButtonWidth);
                	if (bArray2[i] != null){
                		((Region) bArray2[i]).setPrefWidth(UI.defaultButtonWidth);
                	}
                }
                
                //Adjusting ProgressBar Width
                UI.pb.setPrefWidth(UI.defaultWidth);
            }
        });
        
        Main.scene.heightProperty().addListener(new ChangeListener<Number>() {
            @Override public void changed(ObservableValue<? extends Number> observableValue, Number oldSceneHeight, Number newSceneHeight) {
                //System.out.println("Height: " + newSceneHeight);
            	UI.defaultHeight = (double) newSceneHeight;
            	UI.tile.setPrefHeight(UI.defaultHeight);
                
            }
        });
        
        Main.scene.setOnKeyPressed(
    		new EventHandler<KeyEvent>(){
		         @Override
		         public void handle(KeyEvent keyEvent){
		        	 if (keyEvent.getCode() == KeyCode.F1) {
	                    System.out.println("Dev Mode Activated!");
	                    if(UI.devV){
	                    	UI.devV = false;
	                    	UI.debug = false;
	                    	Main.borderPane.setLeft(UI.addVBox());
	                    }
	                    else{
	                    	UI.devV = true;
	                    	Main.borderPane.setLeft(UI.addVBox());
	                    }
    	 			}
	         	}	
			}
		);
	}
}
