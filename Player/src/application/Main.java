package application;

import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.io.LineNumberReader;
import java.net.URI;
import java.util.ArrayList;
import javafx.application.Application;
import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.event.EventHandler;
import javafx.scene.Scene;
import javafx.scene.image.Image;
import javafx.scene.input.KeyCode;
import javafx.scene.input.KeyEvent;
import javafx.scene.layout.BorderPane;
import javafx.scene.paint.Color;
import javafx.stage.Stage;

public class Main extends Application {
	
	//Defining Variables
	static Scene scene;
	BorderPane borderPane;
	double height;
	double width;
	public static ArrayList<String> settings = new ArrayList<String>();
	static String uim = "";
		
	//Main
	public static void main(String[] args){
		launch(args);
	}
	
	//Start of Program
	@Override
	public void start(Stage primaryStage) {
        
		System.out.println("Initializing...");
		System.out.println("");
		
        scene = setScene(this.width, this.height);
        scene.setFill(Color.BLACK);
        scene.getStylesheets().addAll(
        		getClass().getResource("DarkMode.css").toExternalForm(), 
        		getClass().getResource("BrightMode.css").toExternalForm());
  		
        primaryStage.setTitle("GM-Companion | © 2016-2017 Phil Hoffmann, Niklas Lüdtke | Version 0.2.7 Beta");
        primaryStage.setMaximized(true);
        
        if(UI.resourceFolder != " " && UI.resourceFolder != null){
        	if(new File(UI.resourceFolder+"icon.png").exists()){
        		URI icon = new File(UI.resourceFolder+"icon.png").toURI();
                primaryStage.getIcons().clear();
                primaryStage.getIcons().add(new Image(icon.toString()));
                System.out.println(UI.resourceFolder);
        	}
        	else if(new File(UI.resourceFolder+"icon.jpg").exists()){
        		URI icon = new File(UI.resourceFolder+"icon.jpg").toURI();
                primaryStage.getIcons().clear();
                primaryStage.getIcons().add(new Image(icon.toString()));
                System.out.println(UI.resourceFolder);
        	}
        }
        primaryStage.setScene(scene);
        
        //Set UI Mode
    	if(uim.equals("dark")){
    		UIMODE("dark");
    	}
    	else{
    		UIMODE("bright");
    	}
        primaryStage.show();
        UI.defaultWidth = (double) scene.getWidth();
	}
	
	//Defining Scene
	public Scene setScene(double width, double height){
		
		//Check OS
		checkOS();
		
		//Read Settings
        String sp = "settings.txt";
        System.out.println("Reading Settings...");
        try (LineNumberReader br = new LineNumberReader(new FileReader(sp))) {
        	for(String line = null; (line = br.readLine()) != null;){
        		settings.add(line);
        	}
		}
        catch (IOException e) {
			e.printStackTrace();
			System.out.println("Reading Settings Failed");
		}
        
        for(String s : settings){
        	if(s.contains("SERVER_URL=")){
        		String surl = s.substring("SERVER_URL=".length());
        		UI.serverURL = surl;
        		Music.serverMusicURL = surl+"music/";
        		Sound.serverSoundsURL = surl+"sounds/";
        		System.out.println("Set Server URL to "+ surl);
        	}
        	if(s.contains("AUTO_PLAY=")){
        		String ap = s.substring("AUTO_PLAY=".length());
        		if(ap.toLowerCase().equals("true")){
            		UI.autoplay = true;
            	}
            	else{
            		UI.autoplay = false;
            	}
            	System.out.println("Set Autoplay to "+UI.autoplay);
        	}
        	if(s.contains("FADE_OUT=")){
        		String fo = s.substring("FADE_OUT=".length());
            	if(fo.toLowerCase().equals("true")){
            		UI.fadeOut = true;
            	}
            	else{
            		UI.fadeOut = false;
            	}
            	System.out.println("Set Fade Out to "+UI.fadeOut);
        	}
        	if(s.contains("FADE_DURATION=")){
        		String fd = s.substring("FADE_DURATION=".length());
            	UI.fadeDuration = Integer.parseInt(fd);
            	System.out.println("Set Fade Duration to "+UI.fadeDuration);
        	}
        	if(s.contains("MUSIC_PATH=")){
        		String MUSIC_PATH = s.substring("MUSIC_PATH=".length());
        		Music.defaultMusicPath = MUSIC_PATH;
        		Music.musicDirectory = MUSIC_PATH;
        		System.out.println("Set Music Path to "+MUSIC_PATH);
        	}
        	if(s.contains("SOUND_PATH=")){
        		String SOUND_PATH = s.substring("SOUND_PATH=".length());
        		Sound.defaultSoundPath = SOUND_PATH;
        		Sound.soundDirectory = SOUND_PATH;
        		System.out.println("Set Sound Path to "+SOUND_PATH);
        	}
        	if(s.contains("RESOURCE_PATH=")){
        		String RESOURCE_PATH = s.substring("RESOURCE_PATH=".length());
        		UI.resourceFolder = RESOURCE_PATH;
        		System.out.println("Set Resource Path to "+RESOURCE_PATH);
        	}
        	if(s.contains("UI_MODE=")){
        		String UI_MODE = s.substring("UI_MODE=".length());
        		uim = UI_MODE;
        		System.out.println("Set UI Mode to "+UI_MODE);
        	}
        	if(s.contains("ONLINE_MODE=")){
        		String ONLINE_MODE = s.substring("ONLINE_MODE=".length());
        		if(ONLINE_MODE.toLowerCase().equals("true")){
        			UI.localOnline = true;
        			UI.onlineMode = true;
        		}
        		else{
        			UI.localOnline = false;
        			UI.onlineMode = false;
        		}
        		System.out.println("Set Online Mode to "+ONLINE_MODE);
        	}
        }
        
        System.out.println("Finished Reading Settings");
        System.out.println("");
		
		//Add Components
        borderPane = new BorderPane();
        borderPane.setTop(UI.menu());
        borderPane.setCenter(UI.tabPane);
		borderPane.setLeft(UI.addVBox());
		borderPane.getStyleClass().add("border-pane");
		try {
			borderPane.setBottom(UI.addBotBox());
		} catch (IOException e) {
			e.printStackTrace();
		}
		UI.addTabPane();
		
        scene = new Scene(borderPane, 1280, 720);
        UI.defaultWidth = (double) scene.getWidth();
        
        scene.widthProperty().addListener(new ChangeListener<Number>() {
            @Override 
            public void changed(ObservableValue<? extends Number> observableValue, Number oldSceneWidth, Number newSceneWidth) {
            	UI.defaultWidth = (double) newSceneWidth;
            	UI.pb.setPrefWidth((double) newSceneWidth);
            }
        });
        
        scene.heightProperty().addListener(new ChangeListener<Number>() {
            @Override public void changed(ObservableValue<? extends Number> observableValue, Number oldSceneHeight, Number newSceneHeight) {
            	UI.defaultHeight = (double) newSceneHeight;
                
            }
        });
        
        scene.setOnKeyPressed(
    		new EventHandler<KeyEvent>(){
		         @Override
		         public void handle(KeyEvent keyEvent){
		        	 if (keyEvent.getCode() == KeyCode.F1) {
	                    System.out.println("Dev Mode Activated!");
	                    if(UI.devV){
	                    	UI.devV = false;
	                    	UI.debug = false;
	                    	borderPane.setLeft(UI.addVBox());
	                    }
	                    else{
	                    	UI.devV = true;
	                    	borderPane.setLeft(UI.addVBox());
	                    }
    	 			}
	         	}	
			}
		);
        
        return scene;
        
	}
  	
	
	//Adjusting UI Mode
	public static void UIMODE(String mode){
		if(mode.equals("dark")){
			scene.getStylesheets().remove(1);
			System.out.println("DarkMode Activated");
		}
		else{
			scene.getStylesheets().remove(0);
			System.out.println("BrightMode Activated");
		}
	}
	
	@Override
	public void stop(){
	    System.out.println("Closing...");
	    UI.stopDownload = true;
	}
	
  	//Checking the Operating System
  	public static void checkOS(){
  		
  		UI.mainPath = Main.class.getProtectionDomain().getCodeSource().getLocation().getPath();
		
  		UI.osName = System.getProperty("os.name");
		System.out.println("Operating System: "+UI.osName);
		
		if(UI.osName.toLowerCase().contains("windows")){
			UI.windows = true;
			System.out.println("OS detected as Windows");
		}
		else if(UI.osName.toLowerCase().contains("linux")){
			UI.linux = true;
			System.out.println("OS detected as Linux");
		}
		else{
			System.out.println("OS currently not natively supportet, maybe it will work, maybe not.");
			UI.windows = true;
		}
		System.out.println("");
  	}
}
