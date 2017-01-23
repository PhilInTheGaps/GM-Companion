package application;

import javafx.application.Application;
import java.io.IOException;
import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.event.EventHandler;
import javafx.scene.Scene;
import javafx.scene.paint.Color;
import javafx.stage.Screen;
import javafx.stage.Stage;
import javafx.scene.input.KeyCode;
import javafx.scene.input.KeyEvent;
import javafx.scene.layout.BorderPane;
import javafx.scene.layout.HBox;
import javafx.scene.layout.Region;

public class Main extends Application {
	
	//Defining Variables
	
	public static Scene scene;
		
	//Main
	public static void main(String[] args) throws IOException {
		
		launch(args);
	}
	
	//Start
	public void start(Stage primaryStage) throws IOException {
		
		System.out.println("Initializing...");
		System.out.println("");
		
        scene = setScene(1280, 720);
        scene.setFill(Color.BLACK);

        primaryStage.setTitle("RPG Music and Sound Player | © 2017 Phil Hoffmann, Niklas Lüdtke | Version 0.2.0 Beta");
        primaryStage.setScene(scene);
        primaryStage.show();
        //primaryStage.setWidth(Screen.getPrimary().getVisualBounds().getMinX());
        //primaryStage.setHeight(Screen.getPrimary().getVisualBounds().getMinY());
        //primaryStage.setMaximized(true);
	}	
	
	//Defining Scene
	public static Scene setScene(double width, double height) throws IOException{
		
		BorderPane borderPane = new BorderPane();
		
		//Check OS
		System.out.println("Checking Operating System...");
		checkOS();
		
		//Add Components
		/*System.out.println("Adding components to view...");
        borderPane.setTop(UI.addToolBar());
        borderPane.setStyle("-fx-background-color: White");
		borderPane.setCenter(UI.addMusicTilePane());
		borderPane.setRight(UI.addSoundTilePane());
		borderPane.setLeft(UI.addVBox());
		//borderPane.setBottom(addBotBox());
        */
        scene.setFill(Color.WHITE);
        
        
        return scene;
	}
  	
  	//Adding HBox at the Bottom
  	public static HBox addBotBox() throws IOException{
  		UI.botBox.setMinHeight(20);
  		UI.botBox.setStyle("-fx-background-color: Grey");
  		
  		UI.pb.setPrefWidth(UI.defaultWidth);
  		UI.pb.setPrefHeight(5);
  		//pb.setMaxHeight(5);
  		UI.pb.setStyle("-fx-control-inner-background: Grey; -fx-text-box-border: Grey; -fx-accent: LightGrey; -fx-background-color: Grey;");
  		UI.pb.setProgress(0);
  		UI.botBox.getChildren().add(UI.pb);
  		
  		return UI.botBox;
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
