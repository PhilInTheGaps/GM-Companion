package application;

import java.io.IOException;
import java.util.Random;
import javafx.application.Application;
import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.event.EventHandler;
import javafx.geometry.Rectangle2D;
import javafx.scene.Scene;
import javafx.scene.media.Media;
import javafx.scene.media.MediaPlayer;
import javafx.scene.media.MediaView;
import javafx.scene.paint.Color;
import javafx.stage.Screen;
import javafx.stage.Stage;
import javafx.scene.image.ImageView;
import javafx.scene.input.KeyCode;
import javafx.scene.input.KeyEvent;
import javafx.scene.layout.BorderPane;
import javafx.scene.layout.GridPane;
import javafx.scene.layout.HBox;
import javafx.scene.layout.Region;
import javafx.scene.layout.TilePane;
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
	public static MediaView mediaView;
	public static String musicPath;
	public static String soundPath;
	public static BorderPane borderPane = new BorderPane();
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
	public static int currentTrackID;
	public static int currentSoundID;
	public static GridPane grid = new GridPane();
	public static HBox botBox = new HBox();
	public static ProgressBar pb = new ProgressBar();
	public static HBox toolBar1 = new HBox();
	public static HBox toolBar2 = new HBox();
	public static TilePane tile = new TilePane();
	public static TilePane tile2 = new TilePane();
	
	//Setting Default Values
	//The default window size of the program
	public static double defaultWidth = 1366;
	public static double defaultHeight = 768;
	
	//The default space between different elements like buttons
	public static double defaultSpacing = 10;
	public static double defaultPadding = 10;
	
	//Default values of the buttons in the top toolbar
	public static double defaultButtonHeight = 50;
	public static double defaultButtonWidth = 175; //Currently not used, generates from window width
	
	//The size of the volume sliders
	public static double defaultSliderHeight = 50;
	public static double defaultSliderWidth = 320;
	
	//Default values of the buttons used to select a music or sound category
	public static double defaultMusicAndSoundWidth = (defaultWidth - defaultSliderWidth - 2*defaultPadding)/2; //(defaultFolderButtonWidth*3 + 3*defaultPadding)
	public static double defaultFolderButtonWidth = (defaultMusicAndSoundWidth - 3*defaultPadding)/3;
	public static double defaultFolderButtonHeight = 60;
	
	//The default path to the folders used to store music and sounds on linux systems, 
	//because relative file paths don't seem to work
	public static String defaultLinuxFolder = "/home/phil/RPG Music Player/";
	
	//Default volume values
	public static double musicVolume = 0.5;
	public static double soundVolume = 0.25;
	
	//Default subfolders for music and sounds
	public static String defaultMusicPath = ("./Music/");
	public static String defaultSoundPath = ("./Sounds/");

	//Default values of Autoplay, RandomTrack, Debug mode and more
	public static Boolean autoplay = true;
	public static Boolean randomTrack = true;
	public static Boolean singleTrack = false;
	public static Boolean debug = false;
	public static Boolean devV = false;
	public static Boolean onlineMode = false;
	
	//Default Strings displayed when MetaData is not found
	public static String Album = "Unknown";
	public static String Title = "Unknown";
	public static String Artist = "Unknown";
	public static String Year = "";
	public static String mainPath;
	public static String musicFolder = "Not Chosen";
	public static String soundFolder = "Not Chosen";
	
	//Default Server URL
	public static String serverURL = "http://192.168.178.55/"; //http://192.168.178.55/
	public static String serverMusicURL = serverURL + "music/";
	public static String serverSoundsURL = serverURL + "sounds/";
	
	//Start
	public void start(Stage primaryStage) throws IOException {
		
		System.out.println("Initializing...");
		System.out.println("");
		
		//Main.defaultWidth = Screen.getPrimary().getVisualBounds().getMinX();
		//Main.defaultHeight = Screen.getPrimary().getVisualBounds().getMinY();
		
		
        scene = setScene(Main.defaultWidth, Main.defaultHeight);
        scene.setFill(Color.BLACK);

        primaryStage.setTitle("RPG Music and Sound Player | © 2017 Phil Hoffmann, Niklas Lüdtke | Version 0.2.0 Beta");
        primaryStage.setScene(scene);
        primaryStage.show();
        primaryStage.setMaximized(true);
	}
	
	//Defining Scene
	public Scene setScene(double width, double height) throws IOException{
		
		//Check OS
		System.out.println("Checking Operating System...");
		checkOS();
		
		//Add Components
		System.out.println("Adding components to view...");
        borderPane.setTop(ToolBars.addToolBar());
        borderPane.setStyle("-fx-background-color: White");
		borderPane.setCenter(MusicButtons.addMusicTilePane());
		borderPane.setRight(SoundButtons.addSoundTilePane());
		borderPane.setLeft(ToolBars.addVBox());
		borderPane.setBottom(addBotBox());
        
        scene = new Scene(borderPane, defaultWidth, defaultHeight);
        scene.setFill(Color.WHITE);
        
        scene.widthProperty().addListener(new ChangeListener<Number>() {
            @Override public void changed(ObservableValue<? extends Number> observableValue, Number oldSceneWidth, Number newSceneWidth) {
                //System.out.println("Width: " + newSceneWidth);
                defaultWidth = (double) newSceneWidth;
                defaultMusicAndSoundWidth = (defaultWidth - defaultSliderWidth - 2*defaultPadding)/2;
                tile.setPrefWidth(defaultMusicAndSoundWidth);
                tile.setMinWidth(defaultMusicAndSoundWidth);
                tile2.setPrefWidth(defaultMusicAndSoundWidth);
                tile2.setMinWidth(defaultMusicAndSoundWidth);
                toolBar1.setPrefWidth(defaultWidth);
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
            }
        });
        
        scene.heightProperty().addListener(new ChangeListener<Number>() {
            @Override public void changed(ObservableValue<? extends Number> observableValue, Number oldSceneHeight, Number newSceneHeight) {
                //System.out.println("Height: " + newSceneHeight);
                defaultHeight = (double) newSceneHeight;
                tile.setPrefHeight(defaultHeight);
                
            }
        });
        
        scene.setOnKeyPressed(
        		new EventHandler<KeyEvent>(){
			         @Override
			         public void handle(KeyEvent keyEvent){
			        	 if (keyEvent.getCode() == KeyCode.F1) {
			                    System.out.println("Dev Mode Activated!");
			                    if(devV){
			                    	devV = false;
			                    	debug = false;
			                    	borderPane.setLeft(ToolBars.addVBox());
			                    }
			                    else{
			                    	devV = true;
			                    	borderPane.setLeft(ToolBars.addVBox());
			                    }
			                    
			        	 }
			         }
        		}
		);
        return scene;
	}
  	
  	//Adding HBox at the Bottom
  	public static HBox addBotBox() throws IOException{
  		botBox.setMinHeight(20);
  		botBox.setStyle("-fx-background-color: Grey");
  		
  		pb.setPrefWidth(defaultWidth);
  		pb.setPrefHeight(5);
  		pb.setMaxHeight(5);
  		pb.setStyle("-fx-control-inner-background: Grey; -fx-text-box-border: Grey; -fx-accent: LightGrey; -fx-background-color: Grey;");
  		pb.setProgress(0);
  		botBox.getChildren().add(pb);
  		
  		return botBox;
  	}
  	
  	//Checking the Operating System
  	private void checkOS(){
  		
  		mainPath = Main.class.getProtectionDomain().getCodeSource().getLocation().getPath();
		
		osName = System.getProperty("os.name");
		System.out.println("Operating System: "+osName);
		
		if(osName.toLowerCase().contains("windows")){
			windows = true;
			System.out.println("OS detected as Windows");
		}
		else if(osName.toLowerCase().contains("linux")){
			linux = true;
			System.out.println("OS detected as Linux");
		}
		else{
			System.out.println("OS currently not supportet, maybe it will work, maybe not.");
			windows = true;
		}
		System.out.println("");
  	}
  	
	//Main
	public static void main(String[] args) {
		
		//System.out.println(MEDIA_URL);
		launch(args);
		
	}
}
