package application;

import java.io.IOException;

import javafx.beans.InvalidationListener;
import javafx.beans.Observable;
import javafx.event.ActionEvent;
import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.control.Button;
import javafx.scene.control.CheckBox;
import javafx.scene.control.Label;
import javafx.scene.control.Slider;
import javafx.scene.control.TextField;
import javafx.scene.layout.HBox;
import javafx.scene.layout.VBox;

public class ToolBars {
	
	public static Boolean localOnline = Main.onlineMode;
	
	//Adds the toolbar on the top
	public static VBox addToolBar() {
		System.out.println("Adding toolbar...");
		//Adding a VBox that contains 2 HBoxes
		VBox toolBox = new VBox();
		
  		//Setting ToolBar1 Style
  		HBox toolBar1 = new HBox();
  		toolBar1.setPadding(new Insets(Main.defaultPadding/2, Main.defaultPadding/2, Main.defaultPadding/2, Main.defaultPadding/2));
  		toolBar1.setAlignment(Pos.CENTER_LEFT);
  		toolBar1.alignmentProperty().isBound();
  		toolBar1.setSpacing(Main.defaultSpacing/2);
  		toolBar1.setStyle("-fx-background-color: Grey");
  		toolBar1.setMinHeight(50);
  		toolBar1.setMaxHeight(50);
  		
  		//Setting ToolBar2 Style
  		HBox toolBar2 = new HBox();
  		toolBar2.setPadding(new Insets(0, Main.defaultPadding/2, Main.defaultPadding/2, Main.defaultPadding/2));
  		toolBar2.setAlignment(Pos.CENTER_LEFT);
  		toolBar2.alignmentProperty().isBound();
  		toolBar2.setSpacing(Main.defaultSpacing/2);
  		toolBar2.setStyle("-fx-background-color: Grey");
  		toolBar2.setMinHeight(50);
  		toolBar2.setMaxHeight(50);
  		
  		//Play Button
  		Button playButton = new Button();
  		playButton.setText("Play / Continue");
  		playButton.setPrefHeight(Main.defaultButtonHeight);
  		playButton.setOnAction((ActionEvent e) ->{
  			if(Main.musicFolderSelected == true){
  				Main.mediaPlayer.play();
  			}
  			else{
  				Main.musicError = "Please select music folder!";
  			}
  			if(Main.soundFolderSelected == true){
  				Main.soundPlayer.play();
  			}
  			else{
  				Main.soundError = "Please select sound folder!";
  			}
  			
  		});
  		
  		//Pause Button
  		Button pauseButton = new Button();
  		pauseButton.setText("Pause Both");
  		pauseButton.setPrefHeight(Main.defaultButtonHeight);
  		pauseButton.setOnAction((ActionEvent e) -> {
  			if(Main.musicFolderSelected == true){
  				Main.mediaPlayer.pause();
  			}
  			else{
  				Main.musicError = "Please select music folder!";
  			}
  			if(Main.soundFolderSelected == true){
  				Main.soundPlayer.pause();
  			}
  			else{
  				Main.soundError = "Please select sound folder!";
  			}
  		});
  		
  		//Pause Music Button
  		Button pauseMButton = new Button();
  		pauseMButton.setText("Pause Music");
  		pauseMButton.setPrefHeight(Main.defaultButtonHeight);
  		pauseMButton.setOnAction((ActionEvent e) -> {
  			if(Main.musicFolderSelected == true){
  				Main.mediaPlayer.pause();
  			}
  			else{
  				Main.musicError = "Please select sound folder!";
  			}
  		});
  		
  		//Pause Sound Button
  		Button pauseSButton = new Button();
  		pauseSButton.setText("Pause Sound");
  		pauseSButton.setPrefHeight(Main.defaultButtonHeight);
  		pauseSButton.setOnAction((ActionEvent e) -> {
  			if(Main.soundFolderSelected == true){
  				Main.soundPlayer.pause();
  			}
  			else{
  				Main.soundError = "Please select sound folder!";
  			}
  		});
  		
  		//Files Button
  		Button filesButton = new Button();
  		filesButton.setText("Files");
  		filesButton.setPrefHeight(Main.defaultButtonHeight);
  		filesButton.setOnAction((ActionEvent e) -> {
  			Main.mediaPlayer.stop();
  			MFChooser.choose();
  		});
  		
  		//Reload Music Button
  		Button reloadMButton = new Button();
  		reloadMButton.setText("Replay Track");
  		reloadMButton.setPrefHeight(Main.defaultButtonHeight);
  		reloadMButton.setOnAction((ActionEvent e) -> {
  			if(Main.musicFolderSelected == true){
  				Main.mediaPlayer.seek(Main.mediaPlayer.getStartTime());
  			}
  			else{
  				Main.musicError = "Please select music folder!";
  			}
  			
  		});
  		
  		//Reload Sound Button
  		Button reloadSButton = new Button();
  		reloadSButton.setText("Replay Sound");
  		reloadSButton.setPrefHeight(Main.defaultButtonHeight);
  		reloadSButton.setOnAction((ActionEvent e) -> {
  			if(Main.soundFolderSelected == true){
  				Main.soundPlayer.seek(Main.soundPlayer.getStartTime());
  			}
  			else{
  				Main.soundError = "Please select sound folder!";
  			}
  			
  		});
  		
  		//Next Music Button
  		Button nextMButton = new Button();
  		nextMButton.setText("Next Song");
  		nextMButton.setPrefHeight(Main.defaultButtonHeight);
  		nextMButton.setOnAction((ActionEvent e) -> {
  			if(Main.musicFolderSelected == true){
  				nextMusicFile.next();
  			}
  			else{
  				Main.musicError = "Please select music folder!";
  			}
  			
  		});
  		
  		//Next Sound Button
  		Button nextSButton = new Button();
  		nextSButton.setText("Next Sound");
  		nextSButton.setPrefHeight(Main.defaultButtonHeight);
  		nextSButton.setOnAction((ActionEvent e) -> {
  			if(Main.soundFolderSelected == true){
  				nextSoundFile.next();
  			}
  			else{
  				Main.soundError = "Please select sound folder!";
  			}
  			
  		});
  		
  		//Toggle Random
  		Button toggleRandomButton = new Button();
  		toggleRandomButton.setPrefHeight(Main.defaultButtonHeight);
  		if(Main.randomTrack == true){
  			toggleRandomButton.setText("Disable Random Mode");
  		}
  		else{
  			toggleRandomButton.setText("Enable Random Mode");
  		}
  		toggleRandomButton.setOnAction((ActionEvent e) -> {
  			if(Main.randomTrack == true){
  				Main.randomTrack = false;
  				toggleRandomButton.setText("Enable Random Mode");
  			}
  			else{
  				Main.randomTrack = true;
  				toggleRandomButton.setText("Disable Random Mode");
  			}
  			
  		});
  		
  		//Toggle Single Track
  		Button toggleSingleButton = new Button();
  		toggleSingleButton.setPrefHeight(Main.defaultButtonHeight);
  		if(Main.singleTrack == true){
  			toggleSingleButton.setText("Disable Single Mode");
  		}
  		else{
  			toggleSingleButton.setText("Enable Single Mode");
  		}
  		toggleSingleButton.setOnAction((ActionEvent e) -> {
  			if(Main.singleTrack == true){
  				Main.singleTrack = false;
  				toggleSingleButton.setText("Enable Single Mode");
  			}
  			else{
  				Main.singleTrack = true;
  				toggleSingleButton.setText("Disable Single Mode");
  			}
  			
  		});
  		
  		
  		//Online Mode Button
  		Button toggleOnline = new Button();
  		toggleOnline.setPrefHeight(Main.defaultButtonHeight);
  		if(Main.onlineMode == true){
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
  		serverField.setPrefHeight(Main.defaultButtonHeight);
  		
  		//Set Server URL
  		Button setServerURL = new Button();
  		setServerURL.setPrefHeight(Main.defaultButtonHeight);
  		setServerURL.setPrefWidth(75);
  		setServerURL.setText("Set URL");
  		setServerURL.setOnAction((ActionEvent e) -> {
  			Main.serverURL = serverField.getText();
  			int i = Main.serverURL.length();
  			if(Main.serverURL.charAt(i-1)!= ("/").toCharArray()[0]){
  				Main.serverURL = Main.serverURL + "/";
  			}
  			if(Main.serverURL.contains("http://") == false){
  				Main.serverURL = "http://" + Main.serverURL;
  			}
  			Main.serverMusicURL = Main.serverURL + "music/";
  			Main.serverSoundsURL = Main.serverURL + "sounds/";
  		});
  		
  		//Update Folders
  		Button updateFolders = new Button();
  		updateFolders.setPrefHeight(Main.defaultButtonHeight);
  		updateFolders.setPrefWidth(75);
  		updateFolders.setText("Update");
  		updateFolders.setOnAction((ActionEvent e) -> {
  			System.out.println("Updating Folders...");
  			try {
  				Main.onlineMode = localOnline;
  				if(Main.musicFolderSelected == true){
  	  				Main.mediaPlayer.pause();
  	  			}
  	  			if(Main.soundFolderSelected == true){
  	  				Main.soundPlayer.pause();
  	  			}
				Main.grid.add(MusicButtons.addMusicTilePane(), 0, 0);
				Main.grid.add(SoundButtons.addSoundTilePane(), 1, 0);
			} catch (IOException e1) {
				// TODO Auto-generated catch block
				e1.printStackTrace();
			}
  		});
  		
  		//Add everything to ToolBar
  		toolBar1.getChildren().addAll(playButton, pauseMButton, reloadMButton, nextMButton, toggleRandomButton, toggleOnline, updateFolders);
  		toolBar2.getChildren().addAll(pauseButton, pauseSButton, reloadSButton, nextSButton, toggleSingleButton, serverField, setServerURL);
  		
  		//Set Button Width
  		int buttonCount = toolBar1.getChildren().toArray().length;
  		double defaultButtonWidth = Main.defaultButtonWidth; //(Main.defaultWidth - ((buttonCount+1)*Main.defaultPadding)) / buttonCount
  		
  		toggleSingleButton.setPrefWidth(defaultButtonWidth);
  		toggleRandomButton.setPrefWidth(defaultButtonWidth);
  		nextSButton.setPrefWidth(defaultButtonWidth);
  		nextMButton.setPrefWidth(defaultButtonWidth);
  		reloadSButton.setPrefWidth(defaultButtonWidth);
  		reloadMButton.setPrefWidth(defaultButtonWidth);
  		filesButton.setPrefWidth(defaultButtonWidth);
  		pauseMButton.setPrefWidth(defaultButtonWidth);
  		pauseButton.setPrefWidth(defaultButtonWidth);
  		playButton.setPrefWidth(defaultButtonWidth);
  		pauseSButton.setPrefWidth(defaultButtonWidth);
  		toggleOnline.setPrefWidth(defaultButtonWidth);
  		serverField.setPrefWidth(defaultButtonWidth);
  		
  		toolBox.getChildren().add(toolBar1);
  		toolBox.getChildren().add(toolBar2);
  		
  		return toolBox;
  		}
	
	//Adds the VerticalBox on the left
	public static VBox addVBox(){
		System.out.println("Adding MediaInfo Box...");
  		VBox vBox = new VBox();
  		vBox.setPadding(new Insets(Main.defaultPadding));
  		vBox.setStyle("-fx-background-color: White");
  		vBox.setMaxWidth(Main.defaultSliderWidth+2*Main.defaultPadding);
  		
  		Label mVolumeLabel = new Label();
  		mVolumeLabel.setText("Music Volume:");
  		vBox.getChildren().add(mVolumeLabel);
  		
  		//mVolume Slider
  		Main.mVolumeSlider = new Slider();
  		Main.mVolumeSlider.setPrefWidth(Main.defaultSliderWidth);
  		Main.mVolumeSlider.setPrefHeight(Main.defaultSliderHeight);
  		Main.mVolumeSlider.setMinWidth(30);
  		Main.mVolumeSlider.setValue(50);
  		
  		Main.mVolumeSlider.valueProperty().addListener(new InvalidationListener() {
  		    public void invalidated(Observable ov) {
  		       if (Main.mVolumeSlider.isPressed()) {
  		    	 Main.musicVolume = Main.mVolumeSlider.getValue() / 100.0;
  		           if(Main.musicIsPlaying == true){
  		        	 Main.mediaPlayer.setVolume(Main.mVolumeSlider.getValue() / 100.0);
  		           }
  		       }
  		    }
  		});
  		vBox.getChildren().add(Main.mVolumeSlider);
  		
  		Label sVolumeLabel = new Label();
  		sVolumeLabel.setText("Sound Volume:");
  		vBox.getChildren().add(sVolumeLabel);
  		
  		//sVolume Slider
  		Main.sVolumeSlider = new Slider();
  		Main.sVolumeSlider.setPrefWidth(Main.defaultSliderWidth);
  		Main.sVolumeSlider.setPrefHeight(Main.defaultSliderHeight);
  		Main.sVolumeSlider.setMinWidth(30);
  		Main.sVolumeSlider.setValue(25);
  		
  		Main.sVolumeSlider.valueProperty().addListener(new InvalidationListener() {
  		    public void invalidated(Observable ov) {
  		    	if (Main.sVolumeSlider.isPressed()){
  		    		Main.soundVolume = Main.sVolumeSlider.getValue() / 100.0;
  		    		if(Main.soundIsPlaying == true){
  		    			Main.soundPlayer.setVolume(Main.sVolumeSlider.getValue() / 100.0);
  		    		}
  		    	}
	    	}
  		});
  		vBox.getChildren().add(Main.sVolumeSlider);
  		
  		//Music Information
  		Label spacerLabel2 = new Label();
  		spacerLabel2.setText("");
  		vBox.getChildren().add(spacerLabel2);
  		
  		Label trackLabel = new Label();
  		trackLabel.setText("Music Track Information:");
  		vBox.getChildren().add(trackLabel);
  		
  		Main.musicFolderLabel.setText("Folder: " + Main.musicFolder);
  		vBox.getChildren().add(Main.musicFolderLabel);
  		
  		Main.titleLabel.setText("Title: " + Main.Title);
  		vBox.getChildren().add(Main.titleLabel);
  		
  		Main.albumLabel.setText("Album: " + Main.Title);
  		vBox.getChildren().add(Main.albumLabel);
  		
  		Main.artistLabel.setText("Artist: " + Main.Title);
  		vBox.getChildren().add(Main.artistLabel);
  		
  		Main.yearLabel.setText("Year: " + Main.Title);
  		vBox.getChildren().add(Main.yearLabel);
  		
  		//Sound Information
  		Label spacerLabelS = new Label();
  		spacerLabelS.setText("");
  		vBox.getChildren().add(spacerLabelS);
  		
  		Label soundInfoLabel = new Label();
  		soundInfoLabel.setText("Sound Information:");
  		vBox.getChildren().add(soundInfoLabel);
  		
  		Main.soundFolderLabel.setText("Folder: " + Main.soundFolder);
  		vBox.getChildren().add(Main.soundFolderLabel);
  		
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
  			if(Main.debug){
  				Main.debug = false;
  				vBox.getChildren().remove(spacerLabel);
  				vBox.getChildren().remove(debugLabel);
  				vBox.getChildren().remove(pathLabel);
  				vBox.getChildren().remove(pathLabel2);
  				vBox.getChildren().remove(pathLabel3);
  				vBox.getChildren().remove(osLabel);
  			}
  			else{
  				Main.debug = true;
  		  		spacerLabel.setText("");
  		  		vBox.getChildren().add(spacerLabel);
  		  		
  		  		debugLabel.setText("Debug Information:");
  		  		vBox.getChildren().add(debugLabel);
  		  		
  		  		pathLabel.setText(".jar Path: " + Main.mainPath);
  		  		vBox.getChildren().add(pathLabel);
  		  		
  		  		pathLabel2.setText("Music Path: " + Main.defaultMusicPath);
  		  		vBox.getChildren().add(pathLabel2);
  		  		
  		  		pathLabel3.setText("Sound Path: " + Main.defaultSoundPath);
  		  		vBox.getChildren().add(pathLabel3);
  		  		
  		  		osLabel.setText("OS: " + Main.osName);
  		  		vBox.getChildren().add(osLabel);
  			}
  		});
  		if(Main.devV){
  			vBox.getChildren().add(spacerLabel3);
  			vBox.getChildren().add(debugButton);
  		}
  		
  		if(Main.debug){
  		
  		spacerLabel.setText("");
  		vBox.getChildren().add(spacerLabel);
  		
  		debugLabel.setText("Debug Information:");
  		vBox.getChildren().add(debugLabel);
  		
  		pathLabel.setText(".jar Path: " + Main.mainPath);
  		vBox.getChildren().add(pathLabel);
  		
  		pathLabel2.setText("Music Path: " + Main.defaultMusicPath);
  		vBox.getChildren().add(pathLabel2);
  		
  		pathLabel3.setText("Sound Path: " + Main.defaultSoundPath);
  		vBox.getChildren().add(pathLabel3);
  		
  		osLabel.setText("OS: " + Main.osName);
  		vBox.getChildren().add(osLabel);
  		}
  		 
  		//Set Cover Image
  		//coverImage.setFitHeight(defaultSliderWidth-30);
  		Label coverSpacerLabel = new Label();
  		vBox.getChildren().add(coverSpacerLabel);
  		Main.coverImage.setFitWidth(Main.defaultSliderWidth);
  		Main.coverImage.setPreserveRatio(true);
  		vBox.getChildren().add(Main.coverImage);
  		
  		return vBox;
  	}
}
