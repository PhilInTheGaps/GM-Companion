package application;

import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.Random;
import java.util.stream.Stream;
import javafx.application.Application;
import javafx.beans.InvalidationListener;
import javafx.beans.Observable;
import javafx.collections.MapChangeListener;
import javafx.event.ActionEvent;
import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.Scene;
import javafx.scene.media.Media;
import javafx.scene.media.MediaPlayer;
import javafx.scene.media.MediaView;
import javafx.scene.paint.Color;
import javafx.stage.FileChooser;
import javafx.stage.FileChooser.ExtensionFilter;
import javafx.stage.Stage;
import javafx.scene.control.Slider;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.layout.BorderPane;
import javafx.scene.layout.GridPane;
import javafx.scene.layout.HBox;
import javafx.scene.layout.TilePane;
import javafx.scene.layout.VBox;
import javafx.util.Duration;
import javafx.scene.control.*;


public class Main extends Application {
	
	//Defining Variables
	public static MediaPlayer mediaPlayer;
	public static MediaPlayer soundPlayer;
	public static Duration duration;
	public static Scene scene;
	public static Media mMedia;
	public static Media sMedia;
	public static double width;
	public static double height;
	public static MediaView mediaView;
	public static String musicPath;
	public static String soundPath;
	public static BorderPane borderPane;
	public static Slider mVolumeSlider;
	public static Slider sVolumeSlider;
	public static String[] musicPathList = new String[500];
	public static String[] soundPathList = new String[500];
	public static String currentTrack;
	public static String currentSound;
	public static int maxTrackCount;
	public static int maxSoundCount;
	public static Boolean musicIsPlaying = false;
	public static Boolean soundIsPlaying = false;
	public static String osName;
	public static Boolean windows = false;
	public static Boolean linux = false;
	public static Boolean musicFolderSelected = false;
	public static Boolean soundFolderSelected = false;
	public static String musicError;
	public static String soundError;
	
	public static Label titleLabel = new Label();
	public static Label albumLabel = new Label();
	public static Label artistLabel = new Label();
	public static Label yearLabel = new Label();
	public static Label musicFolderLabel = new Label();
	public static Label soundFolderLabel = new Label();
	public static Random randomTrackID = new Random();
	public static Random randomSoundID = new Random();
	public static ImageView coverImage = new ImageView();
	
	//Setting Default Values
	public static double defaultWidth = 1366;
	public static double defaultHeight = 768;
	public static double defaultSpacing = 20;
	public static double defaultPadding = 20;
	public static double defaultButtonHeight = 70;
	public static double defaultSliderWidth = 320;
	public static double defaultSliderHeight = 50;
	public static double defaultFolderButtonWidth = 150;
	public static double defaultFolderButtonHeight = 65;
	public static double defaultMusicAndSoundWidth = defaultFolderButtonWidth*3 + 2*defaultPadding + 10;
	
	public static String defaultLinuxFolder = "/home/phil/RPG Music Player/";
	
	public static double musicVolume = 0.5;
	public static double soundVolume = 0.25;
	public static String defaultMusicPath = ("./Music/");
	public static String defaultSoundPath = ("./Sounds/");
	public static int currentTrackID = 0;
	public static int currentSoundID = 0;
	public static Boolean autoplay = true;
	public static Boolean randomTrack = true;
	public static Boolean singleTrack = false;
	public static Boolean debug = false;
	public static Boolean devV = false;
	
	public static String Album = "Unknown";
	public static String Title = "Unknown";
	public static String Artist = "Unknown";
	public static String Year = "";
	public static String mainPath;
	public static String musicFolder = "Not Chosen";
	public static String soundFolder = "Not Chosen";

	//Start
	@Override
	public void start(Stage primaryStage) {
		
        scene = setScene(Main.width, Main.height);
        scene.setFill(Color.BLACK);

        primaryStage.setTitle("RPG Music and Sound Player | © 2016 Phil Hoffmann");
        primaryStage.setScene(scene);
        primaryStage.show();
	}
	
	//Defining Scene
	public Scene setScene(double width, double height){
		
		//Check OS
		checkOS();
		
		//Add Components
        borderPane = new BorderPane();
        borderPane.setTop(ToolBars.addToolBar());
        borderPane.setStyle("-fx-background-color: White");
		borderPane.setRight(addGridPane());
		borderPane.setLeft(ToolBars.addVBox());
        
        scene = new Scene(borderPane, defaultWidth, defaultHeight);
        scene.setFill(Color.WHITE);
        
        return scene;
	}
	
	/*Defining MusicFileChooser
	public void MFChooser(){
		FileChooser fc = new FileChooser();
		fc.setInitialDirectory(new File(defaultMusicPath));
		fc.getExtensionFilters().addAll(new ExtensionFilter("Sound Files", "*.mp3"));
        File file = fc.showOpenDialog(null);
        
        musicPath = file.getAbsolutePath();
        musicPath = musicPath.replace("\\", "/");
        
        MPlayer.play();
	}
	*/
	
	/*Set MusicPlayer and Play
	public void MPlayer(){
	
		musicPath = musicPathList[currentTrackID];
		musicPath = musicPath.replace("\\", "/");
		
		mMedia = new Media(new File(musicPath).toURI().toString());

		mediaPlayer = new MediaPlayer(mMedia);
        mediaPlayer.setAutoPlay(autoplay);
        mediaPlayer.setVolume(musicVolume);
        if (autoplay == true){
        	musicIsPlaying = true;
        }
        
        //Clear Metadata
        albumLabel.setText("Album: Unknown");
        artistLabel.setText("Artist: Unknown");
        titleLabel.setText("Title: Unknown");
        yearLabel.setText("Year: Unknown");
        coverImage.setImage(null);
        
        //Get Metadata
        mMedia.getMetadata().addListener(new MapChangeListener<String, Object>(){
        	@Override
        	public void onChanged(Change<? extends String, ? extends Object> ch){
        		if (ch.wasAdded()) {
        			handleMetadata(ch.getKey(), ch.getValueAdded());
        		}
        	}
        });
        
        mediaPlayer.setOnEndOfMedia(new Runnable() {
            @Override public void run() {
            	if(singleTrack == false){
            		nextMusicFile();
            	}
            }
        });
	}
	*/
	
	/*Set SoundPlayer and Play
	public void SPlayer(){
	
		soundPath = soundPathList[currentSoundID];
		soundPath = soundPath.replace("\\", "/");
		
		sMedia = new Media(new File(soundPath).toURI().toString());

		soundPlayer = new MediaPlayer(sMedia);
		soundPlayer.setAutoPlay(autoplay);
		soundPlayer.setVolume(soundVolume);
		
		if (autoplay == true){
			soundIsPlaying = true;
		}
        
		soundPlayer.setOnEndOfMedia(new Runnable() {
            @Override public void run() {
            	if (singleTrack == false){
            		nextSoundFile();
            	}
            }
        });
	}
	*/
	
	/*Do something with Metadata
    public void handleMetadata(String key, Object value){
    	switch (key){
    	case ("album"):
    		Album = value.toString();
			albumLabel.setText("Album: " + Album);
			break;
    	case ("artist"):
    		Artist = value.toString();
			artistLabel.setText("Artist: " + Artist);
    		break;
    	case ("title"):
    		Title = value.toString();
			titleLabel.setText("Title: " + Title);
    		break;
    	case ("year"):
    		Year = value.toString();
			yearLabel.setText("Year: " + Year);
    		break;
    	case ("image"):
    		Image cover = (Image) value;
			coverImage.setImage(cover);
    		break;
    	}
    	/*
    	if (key.equals("album")){
    		System.out.println("Album: "+ value.toString());
    		Album = value.toString();
    		albumLabel.setText("Album: " + Album);
    	}
    	if (key.equals("artist")){
    		System.out.println("Artist: " + value.toString());
    		Artist = value.toString();
    		artistLabel.setText("Artist: " + Artist);
    	}
    	if (key.equals("title")){
    		System.out.println("Title: " + value.toString());
    		Title = value.toString();
    		titleLabel.setText("Title: " + Title);
    	}
    	if (key.equals("year")){
    		System.out.println("Year: " + value.toString());
    		Year = value.toString();
    		yearLabel.setText("Year: " + Year);
    	}
    	if (key.equals("image")){
    		System.out.println("Found Cover Image!");
    		Image cover = (Image) value;
    		coverImage.setImage(cover);
    	}
    
    }
    */
    
    /*Find all MusicFiles in Folder and add to Array
    public void getMusicFiles(){
    	
    	currentTrackID = 0;
    	
    	for (int i = 0; i<500; i++){
    		musicPathList[i] = "";
    	}
    	
		try(Stream<Path> paths = Files.walk(Paths.get(defaultMusicPath))) {
		    paths.forEach(filePath -> {
		        if (Files.isRegularFile(filePath)) {
		            //System.out.println(filePath);
		            String tempPath = filePath.toString();
		            musicPathList[currentTrackID] = tempPath;
		            currentTrackID ++;
		        }
		    });
		} catch (IOException e) {
			e.printStackTrace();
		} 
		
		maxTrackCount = currentTrackID;
		if (randomTrack == true){
			currentTrackID = randomTrackID.nextInt(maxTrackCount);
		}
		else{
			currentTrackID = 0;
		}
		
		for (int i = 0; i < maxTrackCount + 1; i++){
			System.out.println(musicPathList[i]);
		}
    }
    */
    
    /*Find all SoundFiles in Folder and add to Array
    public void getSoundFiles(){
    	
    	currentSoundID = 0;
    	
    	for (int i = 0; i<500; i++){
    		soundPathList[i] = "";
    	}
	
		try(Stream<Path> paths = Files.walk(Paths.get(defaultSoundPath))) {
		    paths.forEach(filePath -> {
		        if (Files.isRegularFile(filePath)) {
		            //System.out.println(filePath);
		            String tempPath = filePath.toString();
		            soundPathList[currentSoundID] = tempPath;
		            currentSoundID ++;
		        }
		    });
		} catch (IOException e) {
			e.printStackTrace();
		} 
		
		maxSoundCount = currentSoundID;
		if (randomTrack == true){
			currentSoundID = randomSoundID.nextInt(maxSoundCount);
		}
		else{
			currentSoundID = 0;
		}
		for (int i = 0; i < maxSoundCount + 1; i++){
			System.out.println(soundPathList[i]);
		}
    }
    */
    
    /*Skip to next Song or play from beginning
    public static void nextMusicFile(){
    	if (musicIsPlaying == true){
    		mediaPlayer.stop();
    	}
    	
    	if (randomTrack == true){
    		if(maxTrackCount != 0){
    			if(currentTrackID == maxTrackCount-1){
        			for(int i = 0; i<50; i++){
        				System.out.println(musicPathList);
        				System.out.println("Neue Liste wird generiert");
        				
        				String shuffle1 = "";
        				String shuffle2 = "";
        				String shuffle3 = "";
        				
        				System.out.println(shuffle1);
        				System.out.println(shuffle2);
        				System.out.println(shuffle3);
        				
        				System.out.println(maxTrackCount);
        				
        				int ID1 = randomTrackID.nextInt(maxTrackCount);
        				shuffle1 = musicPathList[ID1];
        				System.out.println(ID1);
        				int ID2 = randomTrackID.nextInt(maxTrackCount);
        				shuffle2 = musicPathList[ID2];
        				System.out.println(ID2);
        				shuffle3 = shuffle2;
        				shuffle2 = shuffle1;
        				shuffle1 = shuffle3;
        				currentTrackID = 0;
        			}
        			currentTrackID = 0;
        			//System.out.println("Next Random Track: " + currentTrackID);
        		}
        		else{
        			currentTrackID++;
        			//currentTrackID = 0;
        		}
    		}
    		
    	}
    	else{
    		if (currentTrackID < maxTrackCount - 1){
        		currentTrackID ++;
        	}
        	else{
        		currentTrackID = 0;
        	}
    	}
    	/*
    	if (randomTrack == true){
    		if(maxTrackCount != 0){
    			currentTrackID = randomTrackID.nextInt(maxTrackCount);
    			System.out.println("Next Random Track: " + currentTrackID);
    		}
    		else{
    			currentTrackID = 0;
    		}
    	}
    	else{
    		if (currentTrackID < maxTrackCount - 1){
        		currentTrackID ++;
        	}
        	else{
        		currentTrackID = 0;
        	}
    	}
    	
    	
    	MPlayer.play();
    }
    */
    
    //Skip to next Sound or play from beginning
    public static void nextSoundFile(){
    	
    	if (soundIsPlaying == true){
    		soundPlayer.stop();
    	}
    	
    	if (randomTrack == true){
    		if(maxSoundCount != 0){
    			//System.out.println(maxSoundCount);
    			currentSoundID = randomSoundID.nextInt(maxSoundCount);
    			System.out.println("Next Random Sound ID: " + currentSoundID);
    		}
    		else{
    			currentSoundID = 0;
    		}
    	}
    	else{
    		if (currentSoundID < maxSoundCount - 1){
    			currentSoundID ++;
        	}
        	else{
        		currentSoundID = 0;
        	}
    	}

    	SPlayer.play();
    }
    
    /*Adding ToolBar
  	private HBox addToolBar() {
  		//Setting ToolBar Style
  		HBox toolBar = new HBox();
  		toolBar.setPadding(new Insets(defaultPadding));
  		toolBar.setAlignment(Pos.CENTER_LEFT);
  		toolBar.alignmentProperty().isBound();
  		toolBar.setSpacing(defaultSpacing);
  		toolBar.setStyle("-fx-background-color: Grey");
  		toolBar.setMinHeight(100);
  		
  		//Play Button
  		Button playButton = new Button();
  		playButton.setText("Play / Continue");
  		playButton.setPrefHeight(defaultButtonHeight);
  		playButton.setOnAction((ActionEvent e) ->{
  			if(musicFolderSelected == true){
  				mediaPlayer.play();
  			}
  			else{
  				musicError = "Please select music folder!";
  			}
  			if(soundFolderSelected == true){
  				soundPlayer.play();
  			}
  			else{
  				soundError = "Please select sound folder!";
  			}
  			
  		});
  		
  		//Pause Button
  		Button pauseButton = new Button();
  		pauseButton.setText("Pause Both");
  		pauseButton.setPrefHeight(defaultButtonHeight);
  		pauseButton.setOnAction((ActionEvent e) -> {
  			if(musicFolderSelected == true){
  				mediaPlayer.pause();
  			}
  			else{
  				musicError = "Please select music folder!";
  			}
  			if(soundFolderSelected == true){
  				soundPlayer.pause();
  			}
  			else{
  				soundError = "Please select sound folder!";
  			}
  		});
  		
  		//Pause Sound Button
  		Button pauseSButton = new Button();
  		pauseSButton.setText("Pause Sound");
  		pauseSButton.setPrefHeight(defaultButtonHeight);
  		pauseSButton.setOnAction((ActionEvent e) -> {
  			if(soundFolderSelected == true){
  				soundPlayer.pause();
  			}
  			else{
  				soundError = "Please select sound folder!";
  			}
  		});
  		
  		//Files Button
  		Button filesButton = new Button();
  		filesButton.setText("Files");
  		filesButton.setPrefHeight(defaultButtonHeight);
  		filesButton.setOnAction((ActionEvent e) -> {
  			mediaPlayer.stop();
  			MFChooser.choose();
  		});
  		
  		//Reload Music Button
  		Button reloadMButton = new Button();
  		reloadMButton.setText("Replay Track");
  		reloadMButton.setPrefHeight(defaultButtonHeight);
  		reloadMButton.setOnAction((ActionEvent e) -> {
  			if(musicFolderSelected == true){
  				mediaPlayer.seek(mediaPlayer.getStartTime());
  			}
  			else{
  				musicError = "Please select music folder!";
  			}
  			
  		});
  		
  		//Reload Sound Button
  		Button reloadSButton = new Button();
  		reloadSButton.setText("Replay Sound");
  		reloadSButton.setPrefHeight(defaultButtonHeight);
  		reloadSButton.setOnAction((ActionEvent e) -> {
  			if(soundFolderSelected == true){
  				soundPlayer.seek(soundPlayer.getStartTime());
  			}
  			else{
  				soundError = "Please select sound folder!";
  			}
  			
  		});
  		
  		//Next Music Button
  		Button nextMButton = new Button();
  		nextMButton.setText("Next Song");
  		nextMButton.setPrefHeight(defaultButtonHeight);
  		nextMButton.setOnAction((ActionEvent e) -> {
  			if(musicFolderSelected == true){
  				nextMusicFile.next();
  			}
  			else{
  				musicError = "Please select music folder!";
  			}
  			
  		});
  		
  		//Next Sound Button
  		Button nextSButton = new Button();
  		nextSButton.setText("Next Sound");
  		nextSButton.setPrefHeight(defaultButtonHeight);
  		nextSButton.setOnAction((ActionEvent e) -> {
  			if(soundFolderSelected == true){
  				nextSoundFile();
  			}
  			else{
  				soundError = "Please select sound folder!";
  			}
  			
  		});
  		
  		//Toggle Random
  		Button toggleRandomButton = new Button();
  		toggleRandomButton.setPrefHeight(defaultButtonHeight);
  		if(randomTrack == true){
  			toggleRandomButton.setText("Disable Random");
  		}
  		else{
  			toggleRandomButton.setText("Enable Random");
  		}
  		toggleRandomButton.setOnAction((ActionEvent e) -> {
  			if(randomTrack == true){
  				randomTrack = false;
  				toggleRandomButton.setText("Enable Random");
  			}
  			else{
  				randomTrack = true;
  				toggleRandomButton.setText("Disable Random");
  			}
  			
  		});
  		
  		//Toggle Single Track
  		Button toggleSingleButton = new Button();
  		toggleSingleButton.setPrefHeight(defaultButtonHeight);
  		if(singleTrack == true){
  			toggleSingleButton.setText("Disable Single M.");
  		}
  		else{
  			toggleSingleButton.setText("Enable Single M.");
  		}
  		toggleSingleButton.setOnAction((ActionEvent e) -> {
  			if(singleTrack == true){
  				singleTrack = false;
  				toggleSingleButton.setText("Enable Single M.");
  			}
  			else{
  				singleTrack = true;
  				toggleSingleButton.setText("Disable Single M.");
  			}
  			
  		});
  		
  		//Add everything to ToolBar
  		toolBar.getChildren().addAll(playButton, pauseButton, pauseSButton, reloadMButton, reloadSButton, 
  									 nextMButton, nextSButton, toggleRandomButton, toggleSingleButton);
  		
  		//Set Button Width
  		int buttonCount = toolBar.getChildren().toArray().length;
  		double defaultButtonWidth = (defaultWidth - ((buttonCount+1)*defaultPadding)) / buttonCount;
  		
  		toggleSingleButton.setPrefWidth(defaultButtonWidth);
  		toggleRandomButton.setPrefWidth(defaultButtonWidth);
  		nextSButton.setPrefWidth(defaultButtonWidth);
  		nextMButton.setPrefWidth(defaultButtonWidth);
  		reloadSButton.setPrefWidth(defaultButtonWidth);
  		reloadMButton.setPrefWidth(defaultButtonWidth);
  		filesButton.setPrefWidth(defaultButtonWidth);
  		pauseButton.setPrefWidth(defaultButtonWidth);
  		playButton.setPrefWidth(defaultButtonWidth);
  		pauseSButton.setPrefWidth(defaultButtonWidth);
  		
  		return toolBar;
  		}
    */
  	
  	/*Adding VBar
  	private VBox addVBox(){
  		VBox vBox = new VBox();
  		vBox.setPadding(new Insets(defaultPadding));
  		vBox.setStyle("-fx-background-color: White");
  		vBox.setMaxWidth(defaultSliderWidth+2*defaultPadding);
  		
  		Label mVolumeLabel = new Label();
  		mVolumeLabel.setText("Music Volume:");
  		vBox.getChildren().add(mVolumeLabel);
  		
  		//mVolume Slider
  		mVolumeSlider = new Slider();
  		mVolumeSlider.setPrefWidth(defaultSliderWidth);
  		mVolumeSlider.setPrefHeight(defaultSliderHeight);
  		mVolumeSlider.setMinWidth(30);
  		mVolumeSlider.setValue(50);
  		
  		mVolumeSlider.valueProperty().addListener(new InvalidationListener() {
  		    public void invalidated(Observable ov) {
  		       if (mVolumeSlider.isValueChanging()) {
  		           musicVolume = mVolumeSlider.getValue() / 100.0;
  		           if(musicIsPlaying == true){
  		        	   mediaPlayer.setVolume(mVolumeSlider.getValue() / 100.0);
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
  		       if (sVolumeSlider.isValueChanging()) {
  		           soundVolume = sVolumeSlider.getValue() / 100.0;
  		           if(soundIsPlaying == true){
  		        	   soundPlayer.setVolume(sVolumeSlider.getValue() / 100.0);
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
  		  		
  		  		pathLabel2.setText("Music Path: " + defaultMusicPath);
  		  		vBox.getChildren().add(pathLabel2);
  		  		
  		  		pathLabel3.setText("Sound Path: " + defaultSoundPath);
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
  		
  		pathLabel2.setText("Music Path: " + defaultMusicPath);
  		vBox.getChildren().add(pathLabel2);
  		
  		pathLabel3.setText("Sound Path: " + defaultSoundPath);
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
  	*/
  	
  	//Adding MusicTilePane
  	private TilePane addMusicTilePane(){
  		TilePane tile = new TilePane();
  		tile.setPadding(new Insets(20, 20, 20, 20));
  		tile.setVgap(3);
  		tile.setHgap(5);
  		tile.setPrefColumns(2);
  		tile.setStyle("-fx-background-color: LightGrey");
  		tile.setPrefWidth(defaultMusicAndSoundWidth);
  		tile.setPrefHeight(defaultHeight);
  		
  		//Action
  		Button Action = new Button();
  		Action.setText("Action");
  		Action.setPrefWidth(defaultFolderButtonWidth);
  		Action.setPrefHeight(defaultFolderButtonHeight);
  		Action.setOnAction((ActionEvent e) -> {
  			if(linux == true){
  				defaultMusicPath = defaultLinuxFolder + "Music/Action";
  			}
  			else{
  				defaultMusicPath = "Music/Action"; 
  			}
  			
  			musicFolder = "Action";
  			musicFolderLabel.setText("Folder: " + musicFolder);
  			musicFolderSelected = true;
  			
  			if (musicIsPlaying == true){
  				mediaPlayer.stop();
  			}
  			getMusicFiles.get();
  			MPlayer.play();
  		});
  		
  		//Bedrückend
  		Button Bedrueckend = new Button();
  		Bedrueckend.setText("Bedrückend");
  		Bedrueckend.setPrefWidth(defaultFolderButtonWidth);
  		Bedrueckend.setPrefHeight(defaultFolderButtonHeight);
  		Bedrueckend.setOnAction((ActionEvent e) -> {
  			if(linux == true){
  				defaultMusicPath = defaultLinuxFolder + "Music/Bedrueckend";
  			}
  			else{
  				defaultMusicPath = "Music/Bedrueckend"; 
  			}
  			
  			musicFolder = "Bedrueckend";
  			musicFolderLabel.setText("Folder: " + musicFolder);
  			musicFolderSelected = true;
  			
  			if (musicIsPlaying == true){
  				mediaPlayer.stop();
  			}
  			getMusicFiles.get();
  			MPlayer.play();
  		});
  		
  		//Credits
  		Button Credits = new Button();
  		Credits.setText("Credits");
  		Credits.setPrefWidth(defaultFolderButtonWidth);
  		Credits.setPrefHeight(defaultFolderButtonHeight);
  		Credits.setOnAction((ActionEvent e) -> {
  			if(linux == true){
  				defaultMusicPath = defaultLinuxFolder + "Music/Credits";
  			}
  			else{
  				defaultMusicPath = "Music/Credits"; 
  			}
  			
  			musicFolder = "Credits";
  			musicFolderLabel.setText("Folder: " + musicFolder);
  			musicFolderSelected = true;
  			
  			if (musicIsPlaying == true){
  				mediaPlayer.stop();
  			}
  			getMusicFiles.get();
  			MPlayer.play();
  		});
  		
  		//Dramatisch
  		Button Dramatisch = new Button();
  		Dramatisch.setText("Dramatisch");
  		Dramatisch.setPrefWidth(defaultFolderButtonWidth);
  		Dramatisch.setPrefHeight(defaultFolderButtonHeight);
  		Dramatisch.setOnAction((ActionEvent e) -> {
  			if(linux == true){
  				defaultMusicPath = defaultLinuxFolder + "Music/Dramatisch";
  			}
  			else{
  				defaultMusicPath = "Music/Dramatisch"; 
  			}
  			
  			musicFolder = "Dramatisch";
  			musicFolderLabel.setText("Folder: " + musicFolder);
  			musicFolderSelected = true;
  			
  			if (musicIsPlaying == true){
  				mediaPlayer.stop();
  			}
  			getMusicFiles.get();
  			MPlayer.play();
  		});
  		
  		//DUNGEONS AND SWAGONS
  		Button DnS = new Button();
  		DnS.setText("Dungeons and Swagons");
  		DnS.setPrefWidth(defaultFolderButtonWidth);
  		DnS.setPrefHeight(defaultFolderButtonHeight);
  		DnS.setOnAction((ActionEvent e) -> {
  			if(linux == true){
  				defaultMusicPath = defaultLinuxFolder + "Music/DnS";
  			}
  			else{
  				defaultMusicPath = "Music/DnS"; 
  			}
  			
  			musicFolder = "DnS";
  			musicFolderLabel.setText("Folder: " + musicFolder);
  			musicFolderSelected = true;
  			
  			if (musicIsPlaying == true){
  				mediaPlayer.stop();
  			}
  			getMusicFiles.get();
  			MPlayer.play();
  		});
  		
  		//Entspannt
  		Button Entspannt = new Button();
  		Entspannt.setText("Entspannt");
  		Entspannt.setPrefWidth(defaultFolderButtonWidth);
  		Entspannt.setPrefHeight(defaultFolderButtonHeight);
  		Entspannt.setOnAction((ActionEvent e) -> {
  			if(linux == true){
  				defaultMusicPath = defaultLinuxFolder + "Music/Entspannt";
  			}
  			else{
  				defaultMusicPath = "Music/Entspannt"; 
  			}
  			
  			musicFolder = "Entspannt";
  			musicFolderLabel.setText("Folder: " + musicFolder);
  			musicFolderSelected = true;
  			
  			if (musicIsPlaying == true){
  				mediaPlayer.stop();
  			}
  			getMusicFiles.get();
  			MPlayer.play();
  		});
  		
  		//Heroisch / Episch
  		Button HE = new Button();
  		HE.setText("Heroisch / Episch");
  		HE.setPrefWidth(defaultFolderButtonWidth);
  		HE.setPrefHeight(defaultFolderButtonHeight);
  		HE.setOnAction((ActionEvent e) -> {
  			if(linux == true){
  				defaultMusicPath = defaultLinuxFolder + "Music/HeroischEpisch";
  			}
  			else{
  				defaultMusicPath = "Music/HeroischEpisch"; 
  			}
  			
  			musicFolder = "HeroischEpisch";
  			musicFolderLabel.setText("Folder: " + musicFolder);
  			musicFolderSelected = true;
  			
  			if (musicIsPlaying == true){
  				mediaPlayer.stop();
  			}
  			getMusicFiles.get();
  			MPlayer.play();
  		});
  		
  		//Insel
  		Button Insel = new Button();
  		Insel.setText("Insel");
  		Insel.setPrefWidth(defaultFolderButtonWidth);
  		Insel.setPrefHeight(defaultFolderButtonHeight);
  		Insel.setOnAction((ActionEvent e) -> {
  			if(linux == true){
  				defaultMusicPath = defaultLinuxFolder + "Music/Insel";
  			}
  			else{
  				defaultMusicPath = "Music/Insel"; 
  			}
  			
  			musicFolder = "Insel";
  			musicFolderLabel.setText("Folder: " + musicFolder);
  			musicFolderSelected = true;
  			
  			if (musicIsPlaying == true){
  				mediaPlayer.stop();
  			}
  			getMusicFiles.get();
  			MPlayer.play();
  		});
  		
  		//JazzLounge
  		Button JazzLounge = new Button();
  		JazzLounge.setText("Jazz Lounge");
  		JazzLounge.setPrefWidth(defaultFolderButtonWidth);
  		JazzLounge.setPrefHeight(defaultFolderButtonHeight);
  		JazzLounge.setOnAction((ActionEvent e) -> {
  			if(linux == true){
  				defaultMusicPath = defaultLinuxFolder + "Music/JazzLounge";
  			}
  			else{
  				defaultMusicPath = "Music/JazzLounge"; 
  			}
  			
  			musicFolder = "JazzLounge";
  			musicFolderLabel.setText("Folder: " + musicFolder);
  			musicFolderSelected = true;
  			
  			if (musicIsPlaying == true){
  				mediaPlayer.stop();
  			}
  			getMusicFiles.get();
  			MPlayer.play();
  		});
  		
  		//Stadt
  		Button Stadt = new Button();
  		Stadt.setText("Stadt");
  		Stadt.setPrefWidth(defaultFolderButtonWidth);
  		Stadt.setPrefHeight(defaultFolderButtonHeight);
  		Stadt.setOnAction((ActionEvent e) -> {
  			if(linux == true){
  				defaultMusicPath = defaultLinuxFolder + "Music/Stadt";
  			}
  			else{
  				defaultMusicPath = "Music/Stadt"; 
  			}
  			
  			musicFolder = "Stadt";
  			musicFolderLabel.setText("Folder: " + musicFolder);
  			musicFolderSelected = true;
  			
  			if (musicIsPlaying == true){
  				mediaPlayer.stop();
  			}
  			getMusicFiles.get();
  			MPlayer.play();
  		});
  		
  		//PIRATEN
  		Button PIRATEN = new Button();
  		PIRATEN.setText("PIRATEN");
  		PIRATEN.setPrefWidth(defaultFolderButtonWidth);
  		PIRATEN.setPrefHeight(defaultFolderButtonHeight);
  		PIRATEN.setOnAction((ActionEvent e) -> {
  			if(linux == true){
  				defaultMusicPath = defaultLinuxFolder + "Music/PIRATEN";
  			}
  			else{
  				defaultMusicPath = "Music/PIRATEN"; 
  			}
  			
  			musicFolder = "PIRATEN";
  			musicFolderLabel.setText("Folder: " + musicFolder);
  			musicFolderSelected = true;
  			
  			if (musicIsPlaying == true){
  				mediaPlayer.stop();
  			}
  			getMusicFiles.get();
  			MPlayer.play();
  		});
  		
  		//HE IS A PIRATE!
  		Button pirate = new Button();
  		pirate.setText("He's a Pirate!");
  		pirate.setPrefWidth(defaultFolderButtonWidth);
  		pirate.setPrefHeight(defaultFolderButtonHeight);
  		pirate.setOnAction((ActionEvent e) -> {
  			if(linux == true){
  				defaultMusicPath = defaultLinuxFolder + "Music/Pirate";
  			}
  			else{
  				defaultMusicPath = "Music/Pirate"; 
  			}
  			
  			musicFolder = "Pirate";
  			musicFolderLabel.setText("Folder: " + musicFolder);
  			musicFolderSelected = true;
  			
  			if (musicIsPlaying == true){
  				mediaPlayer.stop();
  			}
  			getMusicFiles.get();
  			MPlayer.play();
  		});
  		
  		//Random Opener
  		Button RandomOpener = new Button();
  		RandomOpener.setText("Opener");
  		RandomOpener.setPrefWidth(defaultFolderButtonWidth);
  		RandomOpener.setPrefHeight(defaultFolderButtonHeight);
  		RandomOpener.setOnAction((ActionEvent e) -> {
  			if(linux == true){
  				defaultMusicPath = defaultLinuxFolder + "Music/RandomOpener";
  			}
  			else{
  				defaultMusicPath = "Music/RandomOpener"; 
  			}
  			
  			musicFolder = "RandomOpener";
  			musicFolderLabel.setText("Folder: " + musicFolder);
  			musicFolderSelected = true;
  			
  			if (musicIsPlaying == true){
  				mediaPlayer.stop();
  			}
  			getMusicFiles.get();
  			MPlayer.play();
  		});
  		
  		//Reise
  		Button Reise = new Button();
  		Reise.setText("Reise");
  		Reise.setPrefWidth(defaultFolderButtonWidth);
  		Reise.setPrefHeight(defaultFolderButtonHeight);
  		Reise.setOnAction((ActionEvent e) -> {
  			if(linux == true){
  				defaultMusicPath = defaultLinuxFolder + "Music/Reise";
  			}
  			else{
  				defaultMusicPath = "Music/Reise"; 
  			}
  			
  			musicFolder = "Reise";
  			musicFolderLabel.setText("Folder: " + musicFolder);
  			musicFolderSelected = true;
  			
  			if (musicIsPlaying == true){
  				mediaPlayer.stop();
  			}
  			getMusicFiles.get();
  			MPlayer.play();
  		});
  		
  		//SANDSTORM
  		Button SANDSTORM = new Button();
  		SANDSTORM.setText("SANDSTORM");
  		SANDSTORM.setPrefWidth(defaultFolderButtonWidth);
  		SANDSTORM.setPrefHeight(defaultFolderButtonHeight);
  		SANDSTORM.setOnAction((ActionEvent e) -> {
  			if(linux == true){
  				defaultMusicPath = defaultLinuxFolder + "Music/SANDSTORM";
  			}
  			else{
  				defaultMusicPath = "Music/SANDSTORM"; 
  			}
  			
  			musicFolder = "SANDSTORM";
  			musicFolderLabel.setText("Folder: " + musicFolder);
  			musicFolderSelected = true;
  			
  			if (musicIsPlaying == true){
  				mediaPlayer.stop();
  			}
  			getMusicFiles.get();
  			MPlayer.play();
  		});
  		
  		//SciFi Action
  		Button SciFiAction = new Button();
  		SciFiAction.setText("SciFi Action");
  		SciFiAction.setPrefWidth(defaultFolderButtonWidth);
  		SciFiAction.setPrefHeight(defaultFolderButtonHeight);
  		SciFiAction.setOnAction((ActionEvent e) -> {
  			if(linux == true){
  				defaultMusicPath = defaultLinuxFolder + "Music/SciFiAction";
  			}
  			else{
  				defaultMusicPath = "Music/SciFiAction"; 
  			}
  			
  			musicFolder = "SciFiAction";
  			musicFolderLabel.setText("Folder: " + musicFolder);
  			musicFolderSelected = true;
  			
  			if (musicIsPlaying == true){
  				mediaPlayer.stop();
  			}
  			getMusicFiles.get();
  			MPlayer.play();
  		});
  		
  		//SciFi Bedrückend
  		Button SciFiBedrueckend = new Button();
  		SciFiBedrueckend.setText("SciFi Bedrückend");
  		SciFiBedrueckend.setPrefWidth(defaultFolderButtonWidth);
  		SciFiBedrueckend.setPrefHeight(defaultFolderButtonHeight);
  		SciFiBedrueckend.setOnAction((ActionEvent e) -> {
  			if(linux == true){
  				defaultMusicPath = defaultLinuxFolder + "Music/SciFiBedrueckend";
  			}
  			else{
  				defaultMusicPath = "Music/SciFiBedrueckend"; 
  			}
  			
  			musicFolder = "SciFiBedrueckend";
  			musicFolderLabel.setText("Folder: " + musicFolder);
  			musicFolderSelected = true;
  			
  			if (musicIsPlaying == true){
  				mediaPlayer.stop();
  			}
  			getMusicFiles.get();
  			MPlayer.play();
  		});
  		
  		//SciFiDramatisch
  		Button SciFiDramatisch = new Button();
  		SciFiDramatisch.setText("SciFi Dramatisch");
  		SciFiDramatisch.setPrefWidth(defaultFolderButtonWidth);
  		SciFiDramatisch.setPrefHeight(defaultFolderButtonHeight);
  		SciFiDramatisch.setOnAction((ActionEvent e) -> {
  			if(linux == true){
  				defaultMusicPath = defaultLinuxFolder + "Music/SciFiDramatisch";
  			}
  			else{
  				defaultMusicPath = "Music/SciFiDramatisch"; 
  			}
  			
  			musicFolder = "SciFiDramatisch";
  			musicFolderLabel.setText("Folder: " + musicFolder);
  			musicFolderSelected = true;
  			
  			if (musicIsPlaying == true){
  				mediaPlayer.stop();
  			}
  			getMusicFiles.get();
  			MPlayer.play();
  		});
  		
  		//SciFiEntspannt
  		Button SciFiEntspannt = new Button();
  		SciFiEntspannt.setText("SciFi Entspannt");
  		SciFiEntspannt.setPrefWidth(defaultFolderButtonWidth);
  		SciFiEntspannt.setPrefHeight(defaultFolderButtonHeight);
  		SciFiEntspannt.setOnAction((ActionEvent e) -> {
  			if(linux == true){
  				defaultMusicPath = defaultLinuxFolder + "Music/SciFiEntspannt";
  			}
  			else{
  				defaultMusicPath = "Music/SciFiEntspannt"; 
  			}
  			
  			musicFolder = "SciFiEntspannt";
  			musicFolderLabel.setText("Folder: " + musicFolder);
  			musicFolderSelected = true;
  			
  			if (musicIsPlaying == true){
  				mediaPlayer.stop();
  			}
  			getMusicFiles.get();
  			MPlayer.play();
  		});
  		
  		//SciFiFroehlich
  		Button SciFiFroehlich = new Button();
  		SciFiFroehlich.setText("SciFi Fröhlich");
  		SciFiFroehlich.setPrefWidth(defaultFolderButtonWidth);
  		SciFiFroehlich.setPrefHeight(defaultFolderButtonHeight);
  		SciFiFroehlich.setOnAction((ActionEvent e) -> {
  			if(linux == true){
  				defaultMusicPath = defaultLinuxFolder + "Music/SciFiFroehlich";
  			}
  			else{
  				defaultMusicPath = "Music/SciFiFroehlich"; 
  			}
  			
  			musicFolder = "SciFiFroehlich";
  			musicFolderLabel.setText("Folder: " + musicFolder);
  			musicFolderSelected = true;
  			
  			if (musicIsPlaying == true){
  				mediaPlayer.stop();
  			}
  			getMusicFiles.get();
  			MPlayer.play();
  		});
  		
  		//SciFiLounge
  		Button SciFiLounge = new Button();
  		SciFiLounge.setText("SciFi Lounge");
  		SciFiLounge.setPrefWidth(defaultFolderButtonWidth);
  		SciFiLounge.setPrefHeight(defaultFolderButtonHeight);
  		SciFiLounge.setOnAction((ActionEvent e) -> {
  			if(linux == true){
  				defaultMusicPath = defaultLinuxFolder + "Music/SciFiLounge";
  			}
  			else{
  				defaultMusicPath = "Music/SciFiLounge"; 
  			}
  			
  			musicFolder = "SciFiLounge";
  			musicFolderLabel.setText("Folder: " + musicFolder);
  			musicFolderSelected = true;
  			
  			if (musicIsPlaying == true){
  				mediaPlayer.stop();
  			}
  			getMusicFiles.get();
  			MPlayer.play();
  		});
  		
  		//SciFiMarkt
  		Button SciFiMarkt = new Button();
  		SciFiMarkt.setText("SciFi Markt");
  		SciFiMarkt.setPrefWidth(defaultFolderButtonWidth);
  		SciFiMarkt.setPrefHeight(defaultFolderButtonHeight);
  		SciFiMarkt.setOnAction((ActionEvent e) -> {
  			if(linux == true){
  				defaultMusicPath = defaultLinuxFolder + "Music/SciFiMarkt";
  			}
  			else{
  				defaultMusicPath = "Music/SciFiMarkt"; 
  			}
  			
  			musicFolder = "SciFiMarkt";
  			musicFolderLabel.setText("Folder: " + musicFolder);
  			musicFolderSelected = true;
  			
  			if (musicIsPlaying == true){
  				mediaPlayer.stop();
  			}
  			getMusicFiles.get();
  			MPlayer.play();
  		});
  		
  		//SciFiReise
  		Button SciFiReise = new Button();
  		SciFiReise.setText("SciFi Reise");
  		SciFiReise.setPrefWidth(defaultFolderButtonWidth);
  		SciFiReise.setPrefHeight(defaultFolderButtonHeight);
  		SciFiReise.setOnAction((ActionEvent e) -> {
  			if(linux == true){
  				defaultMusicPath = defaultLinuxFolder + "Music/SciFiReise";
  			}
  			else{
  				defaultMusicPath = "Music/SciFiReise"; 
  			}
  			
  			musicFolder = "SciFiReise";
  			musicFolderLabel.setText("Folder: " + musicFolder);
  			musicFolderSelected = true;
  			
  			if (musicIsPlaying == true){
  				mediaPlayer.stop();
  			}
  			getMusicFiles.get();
  			MPlayer.play();
  		});
  		
  		//SciFiTraurig
  		Button SciFiTraurig = new Button();
  		SciFiTraurig.setText("SciFi Traurig");
  		SciFiTraurig.setPrefWidth(defaultFolderButtonWidth);
  		SciFiTraurig.setPrefHeight(defaultFolderButtonHeight);
  		SciFiTraurig.setOnAction((ActionEvent e) -> {
  			if(linux == true){
  				defaultMusicPath = defaultLinuxFolder + "Music/SciFiTraurig";
  			}
  			else{
  				defaultMusicPath = "Music/SciFiTraurig"; 
  			}
  			
  			musicFolder = "SciFiTraurig";
  			musicFolderLabel.setText("Folder: " + musicFolder);
  			musicFolderSelected = true;
  			
  			if (musicIsPlaying == true){
  				mediaPlayer.stop();
  			}
  			getMusicFiles.get();
  			MPlayer.play();
  		});
  		
  		//Taverne
  		Button Taverne = new Button();
  		Taverne.setText("Taverne");
  		Taverne.setPrefWidth(defaultFolderButtonWidth);
  		Taverne.setPrefHeight(defaultFolderButtonHeight);
  		Taverne.setOnAction((ActionEvent e) -> {
  			if(linux == true){
  				defaultMusicPath = defaultLinuxFolder + "Music/Taverne";
  			}
  			else{
  				defaultMusicPath = "Music/Taverne"; 
  			}
  			
  			musicFolder = "Taverne";
  			musicFolderLabel.setText("Folder: " + musicFolder);
  			musicFolderSelected = true;
  			
  			if (musicIsPlaying == true){
  				mediaPlayer.stop();
  			}
  			getMusicFiles.get();
  			MPlayer.play();
  		});
  		
  		//TheBearAndTheMaidenFair
  		Button TheBearAndTheMaidenFair = new Button();
  		TheBearAndTheMaidenFair.setText("The Bear And The Maiden Fair");
  		TheBearAndTheMaidenFair.setPrefWidth(defaultFolderButtonWidth);
  		TheBearAndTheMaidenFair.setPrefHeight(defaultFolderButtonHeight);
  		TheBearAndTheMaidenFair.setOnAction((ActionEvent e) -> {
  			if(linux == true){
  				defaultMusicPath = defaultLinuxFolder + "Music/TheBearAndTheMaidenFair";
  			}
  			else{
  				defaultMusicPath = "Music/TheBearAndTheMaidenFair"; 
  			}
  			
  			musicFolder = "TheBearAndTheMaidenFair";
  			musicFolderLabel.setText("Folder: " + musicFolder);
  			musicFolderSelected = true;
  			
  			if (musicIsPlaying == true){
  				mediaPlayer.stop();
  			}
  			getMusicFiles.get();
  			MPlayer.play();
  		});
  		
  		//TheRainsOfCastamere
  		Button TheRainsOfCastamere = new Button();
  		TheRainsOfCastamere.setText("The Rains Of Castamere");
  		TheRainsOfCastamere.setPrefWidth(defaultFolderButtonWidth);
  		TheRainsOfCastamere.setPrefHeight(defaultFolderButtonHeight);
  		TheRainsOfCastamere.setOnAction((ActionEvent e) -> {
  			if(linux == true){
  				defaultMusicPath = defaultLinuxFolder + "Music/TheRainsOfCastamere";
  			}
  			else{
  				defaultMusicPath = "Music/TheRainsOfCastamere"; 
  			}
  			
  			musicFolder = "TheRainsOfCastamere";
  			musicFolderLabel.setText("Folder: " + musicFolder);
  			musicFolderSelected = true;
  			
  			if (musicIsPlaying == true){
  				mediaPlayer.stop();
  			}
  			getMusicFiles.get();
  			MPlayer.play();
  		});
  		
  		//Piraten Songs
  		Button piratenSongs = new Button();
  		piratenSongs.setText("Piraten Songs");
  		piratenSongs.setPrefWidth(defaultFolderButtonWidth);
  		piratenSongs.setPrefHeight(defaultFolderButtonHeight);
  		piratenSongs.setOnAction((ActionEvent e) -> {
  			if(linux == true){
  				defaultMusicPath = defaultLinuxFolder + "Music/Piratensongs";
  			}
  			else{
  				defaultMusicPath = "Music/Piratensongs"; 
  			}
  			
  			musicFolder = "Piratensongs";
  			musicFolderLabel.setText("Folder: " + musicFolder);
  			musicFolderSelected = true;
  			
  			if (musicIsPlaying == true){
  				mediaPlayer.stop();
  			}
  			getMusicFiles.get();
  			MPlayer.play();
  		});
  		
  		//Add every Button
  		tile.getChildren().addAll(Action, Dramatisch, Bedrueckend, Taverne, Entspannt, HE, Insel, Stadt, Reise, 
  								  SciFiAction, SciFiDramatisch, SciFiBedrueckend, SciFiReise, SciFiEntspannt, SciFiFroehlich, 
  								  SciFiTraurig, SciFiMarkt, SciFiLounge, JazzLounge, Credits, RandomOpener, DnS, 
  								  PIRATEN, pirate, piratenSongs /*, SANDSTORM, 
  								  TheBearAndTheMaidenFair, TheRainsOfCastamere*/);
  		
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
  	
  	//Adding SoundTilePane
  	private TilePane addSoundTilePane(){
  		TilePane tile2 = new TilePane();
  		tile2.setPadding(new Insets(20, 20, 20, 20));
  		tile2.setVgap(3);
  		tile2.setHgap(5);
  		tile2.setPrefColumns(2);
  		tile2.setStyle("-fx-background-color: LightGrey");
  		tile2.setPrefWidth(defaultMusicAndSoundWidth);
  		
  		//Fledermäuse
  		Button fmaus = new Button();
  		fmaus.setText("Fledermäuse");
  		fmaus.setPrefWidth(defaultFolderButtonWidth);
  		fmaus.setPrefHeight(defaultFolderButtonHeight);
  		fmaus.setOnAction((ActionEvent e) -> {
  			
  			if(linux == true){
  				defaultSoundPath = defaultLinuxFolder + "Sounds/Fledermaeuse";
  			}
  			else{
  				defaultSoundPath = "Sounds/Fledermaeuse"; 
  			}
  			
  			soundFolder = "Fledermaeuse";
  			soundFolderLabel.setText("Folder: " + soundFolder);
  			soundFolderSelected = true;
  			
  			if (soundIsPlaying == true){
  				soundPlayer.stop();
  			}
  			getSoundFiles.get();
  			SPlayer.play();
  		});
  		
  		//Grillenzirpen
  		Button zirp = new Button();
  		zirp.setText("Grillenzirpen");
  		zirp.setPrefWidth(defaultFolderButtonWidth);
  		zirp.setPrefHeight(defaultFolderButtonHeight);
  		zirp.setOnAction((ActionEvent e) -> {
  			
  			if(linux == true){
  				defaultSoundPath = defaultLinuxFolder + "Sounds/GrillenZirpen";
  			}
  			else{
  				defaultSoundPath = "Sounds/GrillenZirpen"; 
  			}
  			
  			soundFolder = "GrillenZirpen";
  			soundFolderLabel.setText("Folder: " + soundFolder);
  			soundFolderSelected = true;
  			
  			if (soundIsPlaying == true){
  				soundPlayer.stop();
  			}
  			getSoundFiles.get();
  			SPlayer.play();
  		});
  		
  		//Hafen
  		Button hafen = new Button();
  		hafen.setText("Hafen");
  		hafen.setPrefWidth(defaultFolderButtonWidth);
  		hafen.setPrefHeight(defaultFolderButtonHeight);
  		hafen.setOnAction((ActionEvent e) -> {
  			
  			if(linux == true){
  				defaultSoundPath = defaultLinuxFolder + "Sounds/Hafen";
  			}
  			else{
  				defaultSoundPath = "Sounds/Hafen"; 
  			}
  			
  			soundFolder = "Hafen";
  			soundFolderLabel.setText("Folder: " + soundFolder);
  			soundFolderSelected = true;
  			
  			if (soundIsPlaying == true){
  				soundPlayer.stop();
  			}
  			getSoundFiles.get();
  			SPlayer.play();
  		});
  		
  		//Höhle
  		Button cave = new Button();
  		cave.setText("Höhle");
  		cave.setPrefWidth(defaultFolderButtonWidth);
  		cave.setPrefHeight(defaultFolderButtonHeight);
  		cave.setOnAction((ActionEvent e) -> {
  			
  			if(linux == true){
  				defaultSoundPath = defaultLinuxFolder + "Sounds/Cave";
  			}
  			else{
  				defaultSoundPath = "Sounds/Cave"; 
  			}
  			
  			soundFolder = "Cave";
  			soundFolderLabel.setText("Folder: " + soundFolder);
  			soundFolderSelected = true;
  			
  			if (soundIsPlaying == true){
  				soundPlayer.stop();
  			}
  			getSoundFiles.get();
  			SPlayer.play();
  		});
  		
  		//Kanalisation
  		Button kanali = new Button();
  		kanali.setText("Kanalisation");
  		kanali.setPrefWidth(defaultFolderButtonWidth);
  		kanali.setPrefHeight(defaultFolderButtonHeight);
  		kanali.setOnAction((ActionEvent e) -> {
  			
  			if(linux == true){
  				defaultSoundPath = defaultLinuxFolder + "Sounds/Kanalisation";
  			}
  			else{
  				defaultSoundPath = "Sounds/Kanalisation"; 
  			}
  			
  			soundFolder = "Kanalisation";
  			soundFolderLabel.setText("Folder: " + soundFolder);
  			soundFolderSelected = true;
  			
  			if (soundIsPlaying == true){
  				soundPlayer.stop();
  			}
  			getSoundFiles.get();
  			SPlayer.play();
  		});
  		
  		//Katakomben
  		Button catac = new Button();
  		catac.setText("Katakomben");
  		catac.setPrefWidth(defaultFolderButtonWidth);
  		catac.setPrefHeight(defaultFolderButtonHeight);
  		catac.setOnAction((ActionEvent e) -> {
  			
  			if(linux == true){
  				defaultSoundPath = defaultLinuxFolder + "Sounds/Katakomben";
  			}
  			else{
  				defaultSoundPath = "Sounds/Katakomben"; 
  			}
  			
  			soundFolder = "Katakomben";
  			soundFolderLabel.setText("Folder: " + soundFolder);
  			soundFolderSelected = true;
  			
  			if (soundIsPlaying == true){
  				soundPlayer.stop();
  			}
  			getSoundFiles.get();
  			SPlayer.play();
  		});
  		
  		//Markt
  		Button markt = new Button();
  		markt.setText("Markt");
  		markt.setPrefWidth(defaultFolderButtonWidth);
  		markt.setPrefHeight(defaultFolderButtonHeight);
  		markt.setOnAction((ActionEvent e) -> {
  			
  			if(linux == true){
  				defaultSoundPath = defaultLinuxFolder + "Sounds/Markt";
  			}
  			else{
  				defaultSoundPath = "Sounds/Markt"; 
  			}
  			
  			soundFolder = "Markt";
  			soundFolderLabel.setText("Folder: " + soundFolder);
  			soundFolderSelected = true;
  			
  			if (soundIsPlaying == true){
  				soundPlayer.stop();
  			}
  			getSoundFiles.get();
  			SPlayer.play();
  		});
  		
  		//Nacht 1
  		Button nacht01 = new Button();
  		nacht01.setText("Nacht 1");
  		nacht01.setPrefWidth(defaultFolderButtonWidth);
  		nacht01.setPrefHeight(defaultFolderButtonHeight);
  		nacht01.setOnAction((ActionEvent e) -> {
  			
  			if(linux == true){
  				defaultSoundPath = defaultLinuxFolder + "Sounds/Nacht";
  			}
  			else{
  				defaultSoundPath = "Sounds/Nacht"; 
  			}
  			
  			soundFolder = "Nacht";
  			soundFolderLabel.setText("Folder: " + soundFolder);
  			soundFolderSelected = true;
  			
  			if (soundIsPlaying == true){
  				soundPlayer.stop();
  			}
  			getSoundFiles.get();
  			SPlayer.play();
  		});
  		
  		//Nacht 2
  		Button nacht02 = new Button();
  		nacht02.setText("Nacht 2");
  		nacht02.setPrefWidth(defaultFolderButtonWidth);
  		nacht02.setPrefHeight(defaultFolderButtonHeight);
  		nacht02.setOnAction((ActionEvent e) -> {
  			
  			if(linux == true){
  				defaultSoundPath = defaultLinuxFolder + "Sounds/Nacht2";
  			}
  			else{
  				defaultSoundPath = "Sounds/Nacht2"; 
  			}
  			
  			soundFolder = "Nacht2";
  			soundFolderLabel.setText("Folder: " + soundFolder);
  			soundFolderSelected = true;
  			
  			if (soundIsPlaying == true){
  				soundPlayer.stop();
  			}
  			getSoundFiles.get();
  			SPlayer.play();
  		});
  		
  		//Regen (Gewitter)
  		Button rain = new Button();
  		rain.setText("Regen (Gewitter)");
  		rain.setPrefWidth(defaultFolderButtonWidth);
  		rain.setPrefHeight(defaultFolderButtonHeight);
  		rain.setOnAction((ActionEvent e) -> {
  			
  			if(linux == true){
  				defaultSoundPath = defaultLinuxFolder + "Sounds/RegenGewitter";
  			}
  			else{
  				defaultSoundPath = "Sounds/RegenGewitter"; 
  			}
  			
  			soundFolder = "RegenGewitter";
  			soundFolderLabel.setText("Folder: " + soundFolder);
  			soundFolderSelected = true;
  			
  			if (soundIsPlaying == true){
  				soundPlayer.stop();
  			}
  			getSoundFiles.get();
  			SPlayer.play();
  		});
  		
  		//Schlacht
  		Button battle = new Button();
  		battle.setText("Schlacht");
  		battle.setPrefWidth(defaultFolderButtonWidth);
  		battle.setPrefHeight(defaultFolderButtonHeight);
  		battle.setOnAction((ActionEvent e) -> {
  			
  			if(linux == true){
  				defaultSoundPath = defaultLinuxFolder + "Sounds/Schlacht";
  			}
  			else{
  				defaultSoundPath = "Sounds/Schlacht"; 
  			}
  			
  			soundFolder = "Schlacht";
  			soundFolderLabel.setText("Folder: " + soundFolder);
  			soundFolderSelected = true;
  			
  			if (soundIsPlaying == true){
  				soundPlayer.stop();
  			}
  			getSoundFiles.get();
  			SPlayer.play();
  		});
  		
  		//Stadt
  		Button town = new Button();
  		town.setText("Stadt");
  		town.setPrefWidth(defaultFolderButtonWidth);
  		town.setPrefHeight(defaultFolderButtonHeight);
  		town.setOnAction((ActionEvent e) -> {
  			
  			if(linux == true){
  				defaultSoundPath = defaultLinuxFolder + "Sounds/Stadt";
  			}
  			else{
  				defaultSoundPath = "Sounds/Stadt"; 
  			}
  			
  			soundFolder = "Stadt";
  			soundFolderLabel.setText("Folder: " + soundFolder);
  			soundFolderSelected = true;
  			
  			if (soundIsPlaying == true){
  				soundPlayer.stop();
  			}
  			getSoundFiles.get();
  			SPlayer.play();
  		});
  		
  		//Tag (Reise)
  		Button day = new Button();
  		day.setText("Tag (Reise)");
  		day.setPrefWidth(defaultFolderButtonWidth);
  		day.setPrefHeight(defaultFolderButtonHeight);
  		day.setOnAction((ActionEvent e) -> {
  			
  			if(linux == true){
  				defaultSoundPath = defaultLinuxFolder + "Sounds/TagReise";
  			}
  			else{
  				defaultSoundPath = "Sounds/TagReise"; 
  			}
  			
  			soundFolder = "TagReise";
  			soundFolderLabel.setText("Folder: " + soundFolder);
  			soundFolderSelected = true;
  			
  			if (soundIsPlaying == true){
  				soundPlayer.stop();
  			}
  			getSoundFiles.get();
  			SPlayer.play();
  		});
  		
  		//Taverne
  		Button tavern = new Button();
  		tavern.setText("Taverne");
  		tavern.setPrefWidth(defaultFolderButtonWidth);
  		tavern.setPrefHeight(defaultFolderButtonHeight);
  		tavern.setOnAction((ActionEvent e) -> {
  			
  			if(linux == true){
  				defaultSoundPath = defaultLinuxFolder + "Sounds/Taverne";
  			}
  			else{
  				defaultSoundPath = "Sounds/Taverne"; 
  			}
  			
  			soundFolder = "Taverne";
  			soundFolderLabel.setText("Folder: " + soundFolder);
  			soundFolderSelected = true;
  			
  			if (soundIsPlaying == true){
  				soundPlayer.stop();
  			}
  			getSoundFiles.get();
  			SPlayer.play();
  		});
  		
  		//Taverne (Berg)
  		Button tavern2 = new Button();
  		tavern2.setText("Taverne (Berg)");
  		tavern2.setPrefWidth(defaultFolderButtonWidth);
  		tavern2.setPrefHeight(defaultFolderButtonHeight);
  		tavern2.setOnAction((ActionEvent e) -> {
  			
  			if(linux == true){
  				defaultSoundPath = defaultLinuxFolder + "Sounds/TaverneBerg";
  			}
  			else{
  				defaultSoundPath = "Sounds/TaverneBerg"; 
  			}
  			
  			soundFolder = "TaverneBerg";
  			soundFolderLabel.setText("Folder: " + soundFolder);
  			soundFolderSelected = true;
  			
  			if (soundIsPlaying == true){
  				soundPlayer.stop();
  			}
  			getSoundFiles.get();
  			SPlayer.play();
  		});
  		
  		//Vögel
  		Button bird = new Button();
  		bird.setText("Vögel");
  		bird.setPrefWidth(defaultFolderButtonWidth);
  		bird.setPrefHeight(defaultFolderButtonHeight);
  		bird.setOnAction((ActionEvent e) -> {
  			
  			if(linux == true){
  				defaultSoundPath = defaultLinuxFolder + "Sounds/Voegel";
  			}
  			else{
  				defaultSoundPath = "Sounds/Voegel"; 
  			}
  			
  			soundFolder = "Voegel";
  			soundFolderLabel.setText("Folder: " + soundFolder);
  			soundFolderSelected = true;
  			
  			if (soundIsPlaying == true){
  				soundPlayer.stop();
  			}
  			getSoundFiles.get();
  			SPlayer.play();
  		});
  		
  		//Warcraft III
  		Button warc = new Button();
  		warc.setText("NICHT DRÜCKEN!");
  		warc.setPrefWidth(defaultFolderButtonWidth);
  		warc.setPrefHeight(defaultFolderButtonHeight);
  		warc.setOnAction((ActionEvent e) -> {
  			
  			if(linux == true){
  				defaultSoundPath = defaultLinuxFolder + "Sounds/Warcraft3";
  			}
  			else{
  				defaultSoundPath = "Sounds/Warcraft3"; 
  			}
  			
  			soundFolder = "Warcraft3";
  			soundFolderLabel.setText("Folder: " + soundFolder);
  			soundFolderSelected = true;
  			
  			if (soundIsPlaying == true){
  				soundPlayer.stop();
  			}
  			getSoundFiles.get();
  			SPlayer.play();
  		});
  		
  		//Wasser (Bach)
  		Button water = new Button();
  		water.setText("Wasser (Bach)");
  		water.setPrefWidth(defaultFolderButtonWidth);
  		water.setPrefHeight(defaultFolderButtonHeight);
  		water.setOnAction((ActionEvent e) -> {
  			
  			if(linux == true){
  				defaultSoundPath = defaultLinuxFolder + "Sounds/Wasser01";
  			}
  			else{
  				defaultSoundPath = "Sounds/Wasser01"; 
  			}
  			
  			soundFolder = "Wasser01";
  			soundFolderLabel.setText("Folder: " + soundFolder);
  			soundFolderSelected = true;
  			
  			if (soundIsPlaying == true){
  				soundPlayer.stop();
  			}
  			getSoundFiles.get();
  			SPlayer.play();
  		});
  		
  		//Add everything
  		tile2.getChildren().addAll(day, nacht01, nacht02, bird, rain, water, zirp, fmaus, battle, 
  								   tavern, tavern2, markt, town, hafen, cave, kanali, catac, warc);
  		
		return tile2;
  	}
  	
  	//Adding Grid on the Right
  	private GridPane addGridPane(){
  		GridPane grid = new GridPane();
  		grid.setHgap(5);
  		grid.setVgap(5);
  		grid.setMinWidth(500);
  		
  		grid.add(addMusicTilePane(), 0, 0);
  		grid.add(addSoundTilePane(), 1, 0);
  		
  		return grid;
  	}
  	
  	private void checkOS(){
  		
  		mainPath = Main.class.getProtectionDomain().getCodeSource().getLocation().getPath();
		System.out.println(mainPath);
		
		osName = System.getProperty("os.name");
		System.out.println(osName);
		
		if(osName.toLowerCase().contains("windows")){
			windows = true;
		}
		else if(osName.toLowerCase().contains("linux")){
			linux = true;
		}
		else{
			System.out.println("Could you please use a normal OS? Just kidding.");
		}
  	}
  	
	//Main
	public static void main(String[] args) {
		
		//System.out.println(MEDIA_URL);
		launch(args);
		
	}
}
