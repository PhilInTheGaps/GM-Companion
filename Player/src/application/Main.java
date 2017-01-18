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

  	
  	//Adding Grid on the Right
  	private GridPane addGridPane(){
  		GridPane grid = new GridPane();
  		grid.setHgap(5);
  		grid.setVgap(5);
  		grid.setMinWidth(500);
  		
  		grid.add(MusicButtonsProcedurally.addMusicTilePane(), 0, 0);
  		grid.add(SoundButtonsProcedurally.addSoundTilePane(), 1, 0);
  		
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
			System.out.println("Could you please use a normal OS? Just kidding.");
		}
  	}
  	
	//Main
	public static void main(String[] args) {
		
		//System.out.println(MEDIA_URL);
		launch(args);
		
	}
}
