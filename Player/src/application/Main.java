package application;

import java.util.Random;
import javafx.application.Application;
import javafx.scene.Scene;
import javafx.scene.media.Media;
import javafx.scene.media.MediaPlayer;
import javafx.scene.media.MediaView;
import javafx.scene.paint.Color;
import javafx.stage.Stage;
import javafx.scene.control.Slider;
import javafx.scene.image.ImageView;
import javafx.scene.layout.BorderPane;
import javafx.scene.layout.GridPane;
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
	public static int currentTrackID;
	public static int currentSoundID;
	
	//Setting Default Values
	//The default window size of the program
	public static double defaultWidth = 1366;
	public static double defaultHeight = 768;
	
	//The default space between different elements like buttons
	public static double defaultSpacing = 20;
	public static double defaultPadding = 20;
	
	//Default values of the buttons in the top toolbar
	public static double defaultButtonHeight = 70;
	public static double defaultSliderWidth = 320;
	
	//The height of the volume sliders
	public static double defaultSliderHeight = 50;
	
	//Default values of the buttons used to select a music or sound category
	public static double defaultFolderButtonWidth = 150;
	public static double defaultFolderButtonHeight = 65;
	public static double defaultMusicAndSoundWidth = defaultFolderButtonWidth*3 + 2*defaultPadding + 10;
	
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
	
	//Default Strings displayed when MetaData is not found
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

        primaryStage.setTitle("RPG Music and Sound Player | © 2017 Phil Hoffmann, Niklas Lüdtke");
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

  	
  	//Adding Grid on the Right
  	private GridPane addGridPane(){
  		GridPane grid = new GridPane();
  		grid.setHgap(5);
  		grid.setVgap(5);
  		grid.setMinWidth(500);
  		
  		grid.add(MusicButtons.addMusicTilePane(), 0, 0);
  		grid.add(SoundButtons.addSoundTilePane(), 1, 0);
  		
  		return grid;
  	}
  	
  	//Checking the Operating System
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
			System.out.println("OS currently not supportet, maybe it will work, maybe not.");
			windows = true;
		}
  	}
  	
	//Main
	public static void main(String[] args) {
		
		//System.out.println(MEDIA_URL);
		launch(args);
		
	}
}
