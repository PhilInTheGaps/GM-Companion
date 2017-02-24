package application;

import java.io.File;
import java.io.IOException;
import java.net.URI;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

import org.jsoup.Jsoup;
import org.jsoup.nodes.Document;

import javafx.beans.InvalidationListener;
import javafx.beans.Observable;
import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.event.ActionEvent;
import javafx.event.Event;
import javafx.event.EventHandler;
import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.control.Button;
import javafx.scene.control.CheckBox;
import javafx.scene.control.CheckMenuItem;
import javafx.scene.control.Label;
import javafx.scene.control.ListView;
import javafx.scene.control.Menu;
import javafx.scene.control.MenuBar;
import javafx.scene.control.MenuItem;
import javafx.scene.control.ProgressBar;
import javafx.scene.control.ScrollPane;
import javafx.scene.control.Slider;
import javafx.scene.control.Tab;
import javafx.scene.control.TabPane;
import javafx.scene.control.TabPane.TabClosingPolicy;
import javafx.scene.control.TextField;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.input.MouseButton;
import javafx.scene.input.MouseEvent;
import javafx.scene.layout.Background;
import javafx.scene.layout.BackgroundImage;
import javafx.scene.layout.BackgroundPosition;
import javafx.scene.layout.BackgroundRepeat;
import javafx.scene.layout.BackgroundSize;
import javafx.scene.layout.BorderPane;
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
	public static TilePane tile2 = new TilePane();
	public static TabPane tabPane = new TabPane();
	public static ListView<String> lv = new ListView<String>();
	public static ObservableList<String> items =FXCollections.observableArrayList();
	public static ExecutorService executor = Executors.newCachedThreadPool();
	public static MenuBar menu = new MenuBar();
	
	public static Boolean autoplay = true;
	public static Boolean randomTrack = true;
	public static Boolean singleTrack = false;
	public static Boolean debug = false;
	public static Boolean devV = false;
	public static Boolean onlineMode = false;
	public static Boolean localOnline = onlineMode;
	public static Boolean slowServer = false;
	public static Boolean stopDownload = false;
	public static Boolean fadeOut = true;
	public static int fadeDuration = 10;
	
	public static String Album = "Unknown";
	public static String Title = "Unknown";
	public static String Artist = "Unknown";
	public static String Year = "";
	public static String mainPath;
	public static String musicFolder = "Not Chosen";
	public static String soundFolder = "Not Chosen";
	
	public static String musicFolderName = "";
	
	public static String serverURL; //http://192.168.178.55/ http://rpgmsp.ddns.net/
	public static String resourceFolder;
	
	public static String defaultLinuxFolder = "/home/phil/RPGMusicPlayer/";
	
	public static double defaultWidth = 1366;
	public static double defaultHeight = 768;
	
	public static int buttonRowCount;
	public static double currentWidth;
	
	public static Boolean updating = false;
	public static int catCount = 0;
	public static String[] catArray = new String[500];
	
	//The default space between different elements like buttons
	public static double defaultSpacing = 10;
	public static double defaultPadding = 10;
	
	//Default values of the buttons in the top toolbar
	public static double defaultButtonHeight = 50;
	public static double defaultButtonWidth = 175; //Currently not used, generates from window width
	
	//The size of the volume sliders
	public static double defaultSliderHeight = 50;
	public static double defaultSliderWidth = 320;
	
	public static double defaultMusicAndSoundWidth = (defaultWidth - defaultSliderWidth - 2*defaultPadding);
	public static double defaultFolderButtonWidth = 150;
	public static double defaultFolderButtonHeight = 150;
	public static double folderButtonWidth;
	
	public static Tab tmusic = new Tab();
	public static Tab sound = new Tab();
	public static Tab tgm = new Tab();
	
	//Menus
	public static Menu options = new Menu("Options");
	public static Menu music = new Menu("Music");
	public static Menu sounds = new Menu("Sounds");
	public static Menu gmh = new Menu("GM Help");
	
	//Adds Menu
	public static MenuBar menu(){
		//Add Menus to the MenuBar
        menu.getMenus().addAll(gmh, music, sounds, options);
        
		//Menu Items
        //Random Mode
        CheckMenuItem random = new CheckMenuItem("Random Mode");
        if(randomTrack == true){
  			random.setSelected(true);
  		}
  		else{
  			random.setSelected(false);
  		}
        random.setOnAction((ActionEvent e) ->{
        	if(random.isSelected()){
				randomTrack = true;
			}
			else{
				randomTrack = false;
			}
        });
        
        //Single Track Mode
        CheckMenuItem single = new CheckMenuItem("Single Track Mode");
        if(singleTrack == true){
  			single.setSelected(true);
  		}
  		else{
  			single.setSelected(false);
  		}
        random.setOnAction((ActionEvent e) ->{
        	if(single.isSelected()){
				singleTrack = true;
			}
			else{
				singleTrack = false;
			}
        });
        
        //Online Mode
        CheckMenuItem online = new CheckMenuItem("Online Mode");
  		if(onlineMode == true){
  			online.setSelected(true);
  		}
  		else{
  			online.setSelected(false);
  		}
  		online.setOnAction((ActionEvent e) -> {
  			if(localOnline == true){
  				localOnline = false;
  			}
  			else{
  				localOnline = true;
  			}
  		});
  		
  		
  		
  		//Update Folders
  		MenuItem fupdate = new MenuItem("Update Folders");
  		fupdate.setOnAction((ActionEvent e) -> {
  			System.out.println("Updating Folders...");
  			try {
  				onlineMode = localOnline;
  				if(Music.musicFolderSelected == true){
  					Music.mediaPlayer.pause();
  	  			}
  	  			if(Sound.soundFolderSelected == true){
  	  				Sound.soundPlayer.pause();
  	  			}
  	  			
  	  			updating = true;
  	  			
  	  			music.getItems().clear();
  	  			
  	  			addTabPane();
				addSoundTilePane();
				tabPane.getStylesheets().clear();
				tabPane.getStyleClass().add(".tab-pane");
				
			} catch (IOException e1) {
				e1.printStackTrace();
			}
  		});
  		
  		//All Sounds
  		MenuItem allS = new MenuItem("All");
  		allS.setOnAction((ActionEvent e) ->{
  			tabPane.getSelectionModel().select(sound);
  		});
  		
  		//Dice
  		MenuItem dice = new MenuItem("Dice");
  		dice.setOnAction((ActionEvent e) ->{
  			tabPane.getSelectionModel().select(tgm);
  		});
		
        //Adding Items to Menus
  		gmh.getItems().addAll(dice);
  		sounds.getItems().addAll(allS);
  		options.getItems().addAll(random, single, online, fupdate);
        
		return menu;
	}
	
	//Adds the toolbar at the top
	public static VBox addToolBar() {
		System.out.println("Adding toolbar...");
		//Adding a VBox that contains 2 HBoxes
		VBox toolBox = new VBox();
  		
  		//Pause Button
  		Button pauseButton = new Button();
  		pauseButton.setText("Pause Both");
  		pauseButton.setPrefHeight(defaultButtonHeight);
  		pauseButton.setOnAction((ActionEvent e) -> {
  			if(Music.musicFolderSelected == true){
  				Music.mediaPlayer.pause();
  				Music.isPaused = true;
  			}
  			else{
  				Music.musicError = "Please select music folder!";
  			}
  			if(Sound.soundFolderSelected == true){
  				Sound.soundPlayer.pause();
  				Sound.isPaused = true;
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
  		
  		//Server URL
  		TextField serverField = new TextField();
  		serverField.setPromptText("Server URL");
  		serverField.setPrefHeight(defaultButtonHeight);
  		if(serverURL != ""){
  			serverField.setText(serverURL);
  		}
  		
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
  		
  		//Set Slow Server Mode CheckBox
  		CheckBox slow = new CheckBox();
  		slow.setPrefHeight(defaultButtonHeight);
  		slow.setText("Slow Server Mode");
  		slow.setOnAction((ActionEvent e) -> {
			if(slow.isSelected()){
				slowServer = true;
				System.out.println("Activating Slow Server Mode...");
				Music.setDownloadFile();
			}
			else{
				System.out.println("Disabling Slow Server Mode...");
				slowServer = false;
				stopDownload = true;
			}
  			
  		});
  		
  		//Settings Button
  		Button settings = new Button();
  		settings.setPrefHeight(defaultButtonHeight);
  		settings.setText("Settings");
  		
  		Object[] bArray1 = toolBar1.getChildren().toArray();
        Object[] bArray2 = toolBar2.getChildren().toArray();
        int bCount = bArray1.length;
        int bCount2 = bArray2.length;
        defaultButtonWidth = defaultWidth/bCount;
        
  		for(int i = 0; i < bCount; i++){
        	((Region) bArray1[i]).setPrefWidth(defaultButtonWidth);
        }
  		for(int i = 0; i < bCount2; i++){
        	((Region) bArray2[i]).setPrefWidth(defaultButtonWidth);
        }
  		
  		toolBox.getChildren().add(toolBar1);
  		toolBox.getChildren().add(toolBar2);
  		toolBox.getStyleClass().add("hbox");
  		return toolBox;
  		}
	
	//Adds the VerticalBox on the left
	public static VBox addVBox(){
		System.out.println("Adding MediaInfo Box...");
  		VBox vBox = new VBox();
  		vBox.setPadding(new Insets(defaultPadding));
  		vBox.setMaxWidth(defaultSliderWidth+2*defaultPadding);
  		vBox.getStyleClass().add("vbox");
  		vBox.getChildren().clear();
  		
  		Label mVolumeLabel = new Label();
  		mVolumeLabel.setText("Music:");
  		mVolumeLabel.setStyle("-fx-font-weight: normal;");
  		vBox.getChildren().add(mVolumeLabel);
  		
  		//Setting ToolBar1 Style
  		toolBar1.setPadding(new Insets(defaultPadding/2, defaultPadding/2, defaultPadding/4, defaultPadding/2));
  		toolBar1.setAlignment(Pos.CENTER_LEFT);
  		toolBar1.alignmentProperty().isBound();
  		toolBar1.setSpacing(defaultSpacing/2);
  		toolBar1.setMinHeight(50);
  		toolBar1.setMaxHeight(50);
  		toolBar1.getChildren().clear();
  		vBox.getChildren().add(toolBar1);
  		
  		//Play Button
  		Button playButton = new Button();
  		playButton.setText("Play");
  		playButton.setMinSize(50, 25);
  		playButton.setMaxSize(100, 50);
  		playButton.setPrefSize(100, 50);
  		playButton.setOnAction((ActionEvent e) ->{
  			if(Music.isPaused){
  				Music.mediaPlayer.play();
  			}
  			
  			if(Music.isPaused == false){
  				if(Music.musicFolderSelected == true){
  	  				Music.play();
  	  			}
  	  			else{
  	  				Music.musicError = "Please select music folder!";
  	  			}
  			}
  		});
  		toolBar1.getChildren().add(playButton);
  		
  		//Pause Music Button
  		Button pauseMButton = new Button();
  		pauseMButton.setText("Pause");
  		pauseMButton.setMinSize(50, 25);
  		pauseMButton.setMaxSize(100, 50);
  		pauseMButton.setPrefSize(100, 50);
  		pauseMButton.setOnAction((ActionEvent e) -> {
  			if(Music.musicFolderSelected == true){
  				Music.mediaPlayer.pause();
  			}
  			else{
  				Music.musicError = "Please select sound folder!";
  			}
  		});
  		toolBar1.getChildren().add(pauseMButton);
  		
  		//Reload Music Button
  		Button reloadMButton = new Button();
  		reloadMButton.setText("Replay");
  		reloadMButton.setMinSize(50, 25);
  		reloadMButton.setMaxSize(100, 50);
  		reloadMButton.setPrefSize(100, 50);
  		reloadMButton.setOnAction((ActionEvent e) -> {
  			if(Music.musicFolderSelected == true){
  				Music.mediaPlayer.seek(Music.mediaPlayer.getStartTime());
  			}
  			else{
  				Music.musicError = "Please select music folder!";
  			}
  			
  		});
  		toolBar1.getChildren().add(reloadMButton);
  		
  		//Next Music Button
  		Button nextMButton = new Button();
  		nextMButton.setText("Next");
  		nextMButton.setMinSize(50, 25);
  		nextMButton.setMaxSize(100, 50);
  		nextMButton.setPrefSize(100, 50);
  		nextMButton.setOnAction((ActionEvent e) -> {
  			if(Music.musicFolderSelected == true){
  				Music.next();
  			}
  			else{
  				Music.musicError = "Please select music folder!";
  			}
  			
  		});
  		toolBar1.getChildren().add(nextMButton);
  		
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
  		sVolumeLabel.setText("Sound:");
  		sVolumeLabel.setStyle("-fx-font-weight: normal;");
  		vBox.getChildren().add(sVolumeLabel);
  		
  		//Setting ToolBar2 Style
  		toolBar2.setPadding(new Insets(defaultPadding/4, defaultPadding/2, defaultPadding/2, defaultPadding/2));
  		toolBar2.setAlignment(Pos.CENTER_LEFT);
  		toolBar2.alignmentProperty().isBound();
  		toolBar2.setSpacing(defaultSpacing/2);
  		toolBar2.setMinHeight(50);
  		toolBar2.setMaxHeight(50);
  		toolBar2.getChildren().clear();
  		vBox.getChildren().add(toolBar2);
  		
  		//Play Button
  		Button playSButton = new Button();
  		playSButton.setText("Play");
  		playSButton.setMinSize(50, 25);
  		playSButton.setMaxSize(100, 50);
  		playSButton.setPrefSize(100, 50);
  		playSButton.setOnAction((ActionEvent e) ->{
  			if(Sound.isPaused){
  				Sound.soundPlayer.play();
  			}
  			
  			if(Sound.isPaused == false){
  				if(Sound.soundFolderSelected == true){
  	  				Sound.play();
  	  			}
  	  			else{
  	  				Sound.soundError = "Please select sound folder!";
  	  			}
  			}
  		});
  		toolBar2.getChildren().add(playSButton);
  		
  		//Pause Sound Button
  		Button pauseSButton = new Button();
  		pauseSButton.setText("Pause");
  		pauseSButton.setMinSize(50, 25);
  		pauseSButton.setMaxSize(100, 50);
  		pauseSButton.setPrefSize(100, 50);
  		pauseSButton.setOnAction((ActionEvent e) -> {
  			if(Sound.soundFolderSelected == true){
  				Sound.soundPlayer.pause();
  			}
  			else{
  				Sound.soundError = "Please select sound folder!";
  			}
  		});
  		toolBar2.getChildren().add(pauseSButton);
  		
  		//Reload Sound Button
  		Button reloadSButton = new Button();
  		reloadSButton.setText("Replay");
  		reloadSButton.setMinSize(50, 25);
  		reloadSButton.setMaxSize(100, 50);
  		reloadSButton.setPrefSize(100, 50);
  		reloadSButton.setOnAction((ActionEvent e) -> {
  			if(Sound.soundFolderSelected == true){
  				Sound.soundPlayer.seek(Sound.soundPlayer.getStartTime());
  			}
  			else{
  				Sound.soundError = "Please select sound folder!";
  			}
  			
  		});
  		toolBar2.getChildren().add(reloadSButton);
  		
  		//Next Sound Button
  		Button nextSButton = new Button();
  		nextSButton.setText("Next");
  		nextSButton.setMinSize(50, 25);
  		nextSButton.setMaxSize(100, 50);
  		nextSButton.setPrefSize(100, 50);
  		nextSButton.setOnAction((ActionEvent e) -> {
  			if(Sound.soundFolderSelected == true){
  				Sound.next();
  			}
  			else{
  				Sound.soundError = "Please select sound folder!";
  			}
  			
  		});
  		toolBar2.getChildren().add(nextSButton);
  		
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
  		
  		Label trackLabel = new Label();
  		trackLabel.setText("Music Track Information:");
  		vBox.getChildren().add(trackLabel);
  		
  		musicFolderLabel.setText("Folder: " + musicFolder);
  		musicFolderLabel.setStyle("-fx-font-weight: normal;");
  		vBox.getChildren().add(musicFolderLabel);
  		
  		titleLabel.setText("Title: " + Title);
  		titleLabel.setStyle("-fx-font-weight: normal;");
  		vBox.getChildren().add(titleLabel);
  		
  		albumLabel.setText("Album: " + Title);
  		albumLabel.setStyle("-fx-font-weight: normal;");
  		vBox.getChildren().add(albumLabel);
  		
  		artistLabel.setText("Artist: " + Title);
  		artistLabel.setStyle("-fx-font-weight: normal;");
  		vBox.getChildren().add(artistLabel);
  		
  		yearLabel.setText("Year: " + Title);
  		yearLabel.setStyle("-fx-font-weight: normal;");
  		vBox.getChildren().add(yearLabel);
  		
  		//Sound Information
  		Label soundInfoLabel = new Label();
  		soundInfoLabel.setText("Sound Information:");
  		vBox.getChildren().add(soundInfoLabel);
  		
  		soundFolderLabel.setText("Folder: " + soundFolder);
  		soundFolderLabel.setStyle("-fx-font-weight: normal;");
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
	
	//Add TabPane
	public static void addTabPane(){
		tabPane.setTabMinWidth(200);
		tabPane.setTabMinHeight(45);
		tabPane.getTabs().clear();
		tabPane.setTabClosingPolicy(TabClosingPolicy.UNAVAILABLE);
		
		defaultMusicAndSoundWidth = UI.defaultWidth-2*UI.defaultPadding-UI.defaultSliderWidth;
		
		TabPane tabPaneCategories = new TabPane();
		tabPaneCategories.setTabClosingPolicy(TabClosingPolicy.UNAVAILABLE);
		tabPaneCategories.setTabMinWidth(200);
		tabPaneCategories.setTabMinHeight(40);
		
		lv.setMaxHeight(150);
		lv.setFocusTraversable(false);
		//lv.setMouseTransparent(true);
		lv.setItems(items);
		lv.setOnMouseClicked(new EventHandler<MouseEvent>() {
		    @Override
		    public void handle(MouseEvent mouseEvent) {
		        if(mouseEvent.getButton().equals(MouseButton.PRIMARY)){
		            if(mouseEvent.getClickCount() == 2){
		                System.out.println("Double clicked");
		                Music.currentTrackID = lv.getSelectionModel().getSelectedIndex()-1;
		                Music.next();
		            }
		        }
		    }
		});
		
		BorderPane bp = new BorderPane();
		bp.setCenter(tabPaneCategories);
		bp.setBottom(lv);
		
		Tab general = new Tab();
		general.setClosable(false);
		general.setText("All");

		catCount = 0;
		try {
			addMusicCategories();
		} catch (IOException e2) {
			e2.printStackTrace();
		}
		//Adding Category Tabs
		for(int i = 0; i<catCount;i++){
			Tab t = new Tab();
			MenuItem mi = new MenuItem();
			String name = catArray[i];
			t.setClosable(false);
			t.setId(name);
			t.setText(name);
			mi.setText(name);
			//Setting Background Image
			Runnable r = new Runnable(){
				@Override
				public void run() {
					if(new File(resourceFolder+"Backgrounds/"+t.getText()+".png").exists()){
						URI bip = new File(resourceFolder+"Backgrounds/"+t.getText()+".png").toURI();
						BackgroundImage bi= new BackgroundImage(
								new Image(bip.toString(), 0, 0, true, true),
						        BackgroundRepeat.REPEAT, BackgroundRepeat.REPEAT, BackgroundPosition.CENTER,
						        BackgroundSize.DEFAULT);
						bp.setBackground(new Background(bi));
					}
					else if(new File(resourceFolder+"Backgrounds/"+t.getText()+".jpg").exists()){
						URI bip = new File(resourceFolder+"Backgrounds/"+t.getText()+".jpg").toURI();
						BackgroundImage bi= new BackgroundImage(
								new Image(bip.toString(), 0, 0, true, true),
						        BackgroundRepeat.REPEAT, BackgroundRepeat.REPEAT, BackgroundPosition.CENTER,
						        BackgroundSize.DEFAULT);
						bp.setBackground(new Background(bi));
					}
					else{
						bp.setBackground(null);
					}
				}
			};
			t.setOnSelectionChanged((Event e) -> {
				if(t.isSelected()){
					executor.submit(r);
				}
	  		});
			
			try {
				ScrollPane s = new ScrollPane();
				s.setBackground(null);
				s.setFitToWidth(true);
				s.setContent(addMusicTilePane(catArray[i]));
				t.setContent(s);
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			
			mi.setOnAction((ActionEvent e) ->{
				tabPane.getSelectionModel().select(tmusic);
				tabPaneCategories.getSelectionModel().select(t);
			});
			
			tabPaneCategories.getTabs().add(t);
			music.getItems().add(mi);
		}
		
		tmusic.setClosable(false);
		tmusic.setText("Music");
		tmusic.setContent(bp);
		tabPane.getTabs().add(tmusic);
		
		sound.setClosable(false);
		sound.setText("Sounds");
		ScrollPane s = new ScrollPane();
		try {
			s.setBackground(null);
			s.setFitToWidth(true);
			s.setContent(UI.addSoundTilePane());
			sound.setContent(s);
		} catch (IOException e1) {
			System.out.println("ERROR: Could not create Sound Buttons");
			e1.printStackTrace();
		}
		Runnable r2 = new Runnable(){
			@Override
			public void run() {
				if(new File(resourceFolder+"Backgrounds/"+"Sounds.png").exists()){
					URI bip = new File(resourceFolder+"Backgrounds/"+"Sounds.png").toURI();
					BackgroundImage bi= new BackgroundImage(
							new Image(bip.toString(), 0, 0, true, true),
					        BackgroundRepeat.REPEAT, BackgroundRepeat.REPEAT, BackgroundPosition.CENTER,
					        BackgroundSize.DEFAULT);
					s.setBackground(new Background(bi));
				}
				else if(new File(resourceFolder+"Backgrounds/"+"Sounds.jpg").exists()){
					URI bip = new File(resourceFolder+"Backgrounds/"+"Sounds.jpg").toURI();
					BackgroundImage bi= new BackgroundImage(
							new Image(bip.toString(), 0, 0, true, true),
					        BackgroundRepeat.REPEAT, BackgroundRepeat.REPEAT, BackgroundPosition.CENTER,
					        BackgroundSize.DEFAULT);
					s.setBackground(new Background(bi));
				}
				else{
					s.setBackground(null);
				}
			}
		};
	
		executor.submit(r2);
		
		tabPane.getTabs().add(sound);
		
		tgm.setClosable(false);
		tgm.setText("GM Help");
		tgm.setContent(GM.GMHelp());
		tabPane.getTabs().add(tgm);
		
	}
	
	//Add Music Category Tabs
	public static void addMusicCategories() throws IOException{
		System.out.println("Generating music categories...");
  		
  		String[] catArrayTemp = new String[500];
  		List<String> cats = new ArrayList<String>();
  		
  		File file = new File(Main.settings.get(2));
  		System.out.println(file);
  		
  		if(onlineMode){
  			//Get all foldernames from server
  			Document doc = Jsoup.connect(Music.serverMusicURL).get();
  	        System.out.println(doc.toString());
  	        String str = doc.toString();
  	        String findStr = "<li><a href=";
  	        int lastIndex1 = 0;
  	        int lastIndex2 = 10;
  	        ArrayList<String> folderNames = new ArrayList<String>();
  	        String test = new String();
  	        int count = 0;
  	        
  	        System.out.println("Found the following music category folders:");
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
  	        	catArray[i] = temp;
  	        }
  	        
  	        for(int i = 1; i < count; i++){
  	        	//System.out.println(i);
  	        	//System.out.println(folderArray[i]);
  	        	catArrayTemp[i-1] = catArray[i].toString();
  	        }
  	        catArray = catArrayTemp;
  	        
  		}
  		else{
  			String[] names = file.list();
  			
  	  		System.out.println("Found the following music category folders:");
  	  		
  	  		if(names != null){
	  	  		for(String name : names){
	  	  		    if (new File(Main.settings.get(2)+ name).isDirectory()){
	  	  		        System.out.println(name);
	  	  		        cats.add(name);
	  	  		    }
	  	  		}
	  	  		
		        for(int i = 0; i < cats.size(); i++){
		        	//System.out.println(i);
		        	//System.out.println(folders.get(i));
		        	catArrayTemp[i] = cats.get(i).toString();
		        }
  	  		}
  	  		
	        catArray = catArrayTemp; 
  		}
  		
  		for(int i  = 0; i < catArray.length; i++){
  				if(catArray[i] != null){
  					catCount++;
  					//System.out.println("Category Count: "+catCount);
  		  			
  				}

	  		}
  		
  		//Main.adjustUI();
  		
  		updating = false;
  		
  		System.out.println("Added music category buttons");
  		System.out.println("");
  	}
	
	//Add Music Buttons
	public static TilePane addMusicTilePane(String directory) throws IOException{
		System.out.println("Generating music buttons for directory: "+directory);
		
		TilePane tile = new TilePane();
  		tile.setPadding(new Insets(defaultPadding, defaultPadding/2, defaultPadding, defaultPadding));
  		tile.setVgap(defaultPadding/4);
  		tile.setHgap(defaultPadding/4);
  		tile.setPrefColumns(3);
  		tile.setPrefWidth(defaultMusicAndSoundWidth);
  		//tile.setPrefHeight(defaultHeight);
  		
  		tile.getChildren().clear();
  		
  		ScrollPane sp = new ScrollPane();
		sp.setContent(tile);
  		
  		String[] folderArray = new String[500];
  		String[] folderArrayTemp = new String[500];
  		List<String> folders = new ArrayList<String>();
  		
  		File file = new File(Main.settings.get(2)+directory+"/");
  		//System.out.println(file);
  		
  		if(onlineMode){
  			//Get all foldernames from server
  			Document doc = Jsoup.connect(Music.serverMusicURL+directory+"/").get();
  	        //System.out.println(doc.toString());
  			//System.out.println(Music.serverMusicURL+directory+"/");
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
  	  		
  	  		if(names !=null){
	  	  		for(String name : names){
					if (new File(Main.settings.get(2)+directory+"/" + name).isDirectory()){
						System.out.println(name);
						folders.add(name);
					}
	  	  		}
	  	  		
		        for(int i = 0; i < folders.size(); i++){
		        	//System.out.println(i);
		        	//System.out.println(folders.get(i));
		        	folderArrayTemp[i] = folders.get(i).toString();
		        }
  	  		}
  	  		
	        
	        folderArray = folderArrayTemp; 
  		}
  		
  		for(int i  = 0; i < folderArray.length; i++){
  				if(folderArray[i] != null){
  					String bName = new String();
  					bName = folderArray[i].toString();
  		  			Button b = new Button(String.valueOf(i));
  		  			
  		  			String nbName = bName.replace("%2520", "%20");
  		  			System.out.println(nbName);
  		  			b.setText(nbName.replace("%20", " "));
  		  			b.setPrefSize(defaultFolderButtonWidth, defaultFolderButtonHeight);
  		  			b.setMinSize(defaultFolderButtonWidth, defaultFolderButtonHeight);
  		  			b.setMaxSize(defaultFolderButtonWidth, defaultFolderButtonHeight);
  		  			b.getStyleClass().add("button1");
  		  			if(new File(resourceFolder+"Icons/"+directory+"/"+bName+".png").exists()){
  		  				URI pic = new File(resourceFolder+"Icons/"+directory+"/"+bName+".png").toURI();
		  				b.setStyle("-fx-background-image: url('"+ pic +"'); -fx-opacity: 0.9; -fx-text-fill: white");
  		  			}
  		  			else if(new File(resourceFolder+"Icons/"+directory+"/"+bName+".jpg").exists()){
  		  				URI pic = new File(resourceFolder+"Icons/"+directory+"/"+bName+".jpg").toURI();
		  				b.setStyle("-fx-background-image: url('"+ pic +"'); -fx-opacity: 0.9;");
  		  			}
  		  			
  		  			b.setOnAction((ActionEvent e) -> {
  		  				if(onlineMode){
  		  					Music.defaultMusicPath = directory+"/"+nbName;
  		  					System.out.println();
  		  					//Music.slowFolder = directory;
  		  				}
  		  				else{
		  	  				Music.defaultMusicPath = Main.settings.get(2)+directory+"/"+nbName;
  		  				}
  		  				
  		  				musicFolder = directory+"/"+nbName;
  		  				musicFolderName = nbName;
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
  		  	  			if(autoplay){
  		  	  				Music.play();
  		  	  			}
  		  	  		});
  		  			
  		  			tile.getChildren().add(b);
  				}

	  		}
  		
  		//Main.adjustUI();
  		
  		updating = false;
  		
  		System.out.println("Added music buttons");
  		System.out.println("");
		return tile;
  	}
	
	//Add Sound Buttons
	public static TilePane addSoundTilePane() throws IOException{
		System.out.println("Generating sound buttons...");
		
  		tile2.setPadding(new Insets(defaultPadding, defaultPadding, defaultPadding, defaultPadding/2));
  		tile2.setVgap(defaultPadding/4);
  		tile2.setHgap(defaultPadding/4);
  		tile2.setPrefColumns(3);
  		tile2.setPrefWidth(defaultMusicAndSoundWidth);
  		//tile.setPrefHeight(defaultHeight);
  		
  		tile2.getChildren().clear();
  		
  		String[] folderArray = new String[500];
  		
  		File file = new File(Main.settings.get(4));
  		
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
  	  		
  	  		if(names != null){
	  	  		for(String name : names)
	  	  		{
	  	  		    if (new File(Main.settings.get(4) + name).isDirectory())
	  	  		    {
	  	  		        System.out.println(name);
	  	  		        folders.add(name);
	  	  		    }
	  	  		}
	  	  		String[] folderArrayTemp = new String[500];
		        for(int i = 0; i < folders.size(); i++){
		        	//System.out.println(i);
		        	//System.out.println(folders.get(i));
		        	folderArrayTemp[i] = folders.get(i).toString();
		        }
		        folderArray = folderArrayTemp;
  	  		}
	        
  		}
  		
  		for(int i  = 0; i < folderArray.length; i++){
				if(folderArray[i] != null){
					String bName = folderArray[i].toString();
		  			Button b = new Button(String.valueOf(i));
		  			
		  			b.setText(bName);
		  			b.setPrefSize(defaultFolderButtonWidth, defaultFolderButtonHeight);
		  			b.getStyleClass().add("button1");
		  			
		  			b.setOnAction((ActionEvent e) -> {
		  				if(onlineMode){
		  					Sound.defaultSoundPath = bName;
		  				}
		  				else{
			  	  			Sound.defaultSoundPath = Main.settings.get(4)+bName;
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
	
	//Updates the ProgressBar
	public static void updatePB(){
		ChangeListener<Duration> progressChangeListener;
		progressChangeListener = new ChangeListener<Duration>(){
			@Override public void changed(ObservableValue<? extends Duration> observableValue, Duration oldValue, Duration newValue) {
		        pb.setProgress(1.0 * Music.mediaPlayer.getCurrentTime().toMillis() / Music.mediaPlayer.getTotalDuration().toMillis());
		        //System.out.println(Music.mediaPlayer.getCurrentTime().toSeconds());
		        //Fade Out
		        if(fadeOut){
		        	if(Music.mediaPlayer.getCurrentTime().toSeconds()>Music.mediaPlayer.getTotalDuration().toSeconds()-fadeDuration){
		        		if(Music.fading == false){
		        			Music.fading = true;
		        			System.out.println("Fading Music...");
		        			Music.fade();
		        		}
		        	}
		        }
			}
		};
		Music.mediaPlayer.currentTimeProperty().addListener(progressChangeListener);
	}
	
	//Adds ProgressBar
	public static HBox addBotBox() throws IOException{
  		botBox.setMinHeight(20);
  		
  		pb.setPrefWidth(defaultWidth);
  		pb.setPrefHeight(5);
  		//pb.setMaxHeight(5);
  		pb.setStyle("-fx-control-inner-background: Grey; -fx-text-box-border: Grey; -fx-accent: LightGrey; -fx-background-color: Grey;");
  		pb.setProgress(0);
  		botBox.getChildren().add(pb);
  		
  		return UI.botBox;
  	}
}
