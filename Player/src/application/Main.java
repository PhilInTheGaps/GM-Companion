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
import javafx.scene.layout.Background;
import javafx.scene.layout.BackgroundImage;
import javafx.scene.layout.BackgroundPosition;
import javafx.scene.layout.BackgroundRepeat;
import javafx.scene.layout.BackgroundSize;
import javafx.scene.layout.BorderPane;
import javafx.scene.layout.Region;
import javafx.scene.paint.Color;
import javafx.stage.Stage;

public class Main extends Application {
	
	//Defining Variables
	Scene scene;
	BorderPane borderPane;
	double height;
	double width;
	public static ArrayList<String> settings = new ArrayList<String>();
		
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

        primaryStage.setTitle("RPG Music and Sound Player | © 2016-2017 Phil Hoffmann, Niklas Lüdtke | Version 0.2.5 Beta");
        primaryStage.setScene(scene);
        primaryStage.show();
        //primaryStage.setMaximized(true);
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
        UI.defaultWidth = (double) scene.getWidth();
        adjustUI();
        scene.getStylesheets().add(getClass().getResource("application.css").toExternalForm());
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
        	
        	//Set Server URL
        	UI.serverURL = settings.get(6);
        	System.out.println("Set Server URL to "+UI.serverURL);
        	
        	//Set Autoplay
        	if(settings.get(10).equals("True")){
        		UI.autoplay = true;
        	}
        	else{
        		UI.autoplay = false;
        	}
        	System.out.println("Set Autoplay to "+UI.autoplay);
        	
        	//Set FadeOut
        	if(settings.get(8).equals("True")){
        		UI.fadeOut = true;
        	}
        	else{
        		UI.fadeOut = false;
        	}
        	System.out.println("Set Fade Out to "+UI.fadeOut);
        	
        	//Set Fade Duration
        	UI.fadeDuration = Integer.parseInt(settings.get(12));
        	System.out.println("Set Fade Duration to "+UI.fadeDuration);
        	
        	//Set Resource Folder
        	if(settings.get(14).equals(" ") || settings.get(14).equals(null)){
        		System.out.println("Resource folder not set");
        	}
        	else{
        		UI.resourceFolder = settings.get(14);
        	}
        	
		} catch (IOException e) {
			e.printStackTrace();
			System.out.println("Reading Settings Failed");
		}
        System.out.println("Finished Reading Settings");
        System.out.println("");
		
		//Add Components
        borderPane = new BorderPane();
        borderPane.setTop(UI.addToolBar());
        //borderPane.setStyle("-fx-background-color: White");
        borderPane.setCenter(UI.tabPane);	
		borderPane.setLeft(UI.addVBox());
		try {
			borderPane.setBottom(UI.addBotBox());
		} catch (IOException e) {
			e.printStackTrace();
		}
		UI.addTabPane();
		
		//Setting Background Image
		if(new File(UI.resourceFolder+"Backgrounds/"+"bg.png").exists()){
			URI bip = new File(UI.resourceFolder+"Backgrounds/"+"bg.png").toURI();
			BackgroundImage bi= new BackgroundImage(
					new Image(bip.toString(), 0, 0, true, true),
			        BackgroundRepeat.REPEAT, BackgroundRepeat.REPEAT, BackgroundPosition.DEFAULT,
			        BackgroundSize.DEFAULT);
			borderPane.setBackground(new Background(bi));
		}
		else if(new File(UI.resourceFolder+"Backgrounds/"+"bg.jpg").exists()){
			URI bip = new File(UI.resourceFolder+"Backgrounds/"+"bg.jpg").toURI();
			BackgroundImage bi= new BackgroundImage(
					new Image(bip.toString(), 0, 0, true, true),
			        BackgroundRepeat.REPEAT, BackgroundRepeat.REPEAT, BackgroundPosition.DEFAULT,
			        BackgroundSize.DEFAULT);
			borderPane.setBackground(new Background(bi));
		}
		else{
			borderPane.setStyle("-fx-background-color: LightGrey");
		}
		
        scene = new Scene(borderPane, 1280, 720);
        scene.setFill(Color.WHITE);
        UI.defaultWidth = (double) scene.getWidth();
        adjustUI();
        
        scene.widthProperty().addListener(new ChangeListener<Number>() {
            @Override 
            public void changed(ObservableValue<? extends Number> observableValue, Number oldSceneWidth, Number newSceneWidth) {
            	//System.out.println("Width: " + newSceneWidth);
            	UI.defaultWidth = (double) newSceneWidth;
            	adjustUI();
            }
        });
        
        scene.heightProperty().addListener(new ChangeListener<Number>() {
            @Override public void changed(ObservableValue<? extends Number> observableValue, Number oldSceneHeight, Number newSceneHeight) {
                //System.out.println("Height: " + newSceneHeight);
            	UI.defaultHeight = (double) newSceneHeight;
            	//UI.tile.setPrefHeight(UI.defaultHeight);
                
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
  	
	//Adjusting UI
	public static void adjustUI(){
        
        UI.toolBar1.setPrefWidth(UI.defaultWidth);
        Object[] bArray1 = UI.toolBar1.getChildren().toArray();
        Object[] bArray2 = UI.toolBar2.getChildren().toArray();
        int bCount = bArray1.length;
        int bCount2 = bArray2.length;
        UI.defaultButtonWidth = UI.defaultWidth/bCount;
		
    	//Adjusting ToolBar button width
        for(int i = 0; i < bCount; i++){
        	((Region) bArray1[i]).setPrefWidth(UI.defaultButtonWidth);
        }
        for(int i = 0; i < bCount2; i++){
        	((Region) bArray2[i]).setPrefWidth(UI.defaultButtonWidth);
        }
        
        //Adjusting ProgressBar Width
        
        UI.pb.setPrefWidth(UI.defaultWidth);
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
			System.out.println("OS currently not supportet, maybe it will work, maybe not.");
			UI.windows = true;
		}
		System.out.println("");
  	}
}
