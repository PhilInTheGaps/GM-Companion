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
	public Scene scene;
	public BorderPane borderPane = new BorderPane();
		
	//Main
	public static void main(String[] args) {
		
		launch(args);
		
	}
	
	//Start
	public void start(Stage primaryStage) throws IOException {
		//Application.launch();
		
		System.out.println("Initializing...");
		System.out.println("");
		
		//Main.defaultWidth = Screen.getPrimary().getVisualBounds().getMinX();
		//Main.defaultHeight = Screen.getPrimary().getVisualBounds().getMinY();
		
		
        scene = setScene(UI.defaultWidth, UI.defaultHeight);
        scene.setFill(Color.BLACK);

        primaryStage.setTitle("RPG Music and Sound Player | © 2017 Phil Hoffmann, Niklas Lüdtke | Version 0.2.0 Beta");
        primaryStage.setScene(scene);
        primaryStage.show();
        primaryStage.setWidth(Screen.getPrimary().getVisualBounds().getMinX());
        primaryStage.setHeight(Screen.getPrimary().getVisualBounds().getMinY());
        primaryStage.setMaximized(true);
	}	
	
	//Defining Scene
	public Scene setScene(double width, double height) throws IOException{
		
		//Check OS
		System.out.println("Checking Operating System...");
		checkOS();
		
		//Add Components
		System.out.println("Adding components to view...");
        borderPane.setTop(UI.addToolBar());
        borderPane.setStyle("-fx-background-color: White");
		borderPane.setCenter(UI.addMusicTilePane());
		borderPane.setRight(UI.addSoundTilePane());
		borderPane.setLeft(UI.addVBox());
		borderPane.setBottom(addBotBox());
        
        scene = new Scene(borderPane, UI.defaultWidth, UI.defaultHeight);
        scene.setFill(Color.WHITE);
        
        scene.widthProperty().addListener(new ChangeListener<Number>() {
            @Override public void changed(ObservableValue<? extends Number> observableValue, Number oldSceneWidth, Number newSceneWidth) {
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
        
        scene.heightProperty().addListener(new ChangeListener<Number>() {
            @Override public void changed(ObservableValue<? extends Number> observableValue, Number oldSceneHeight, Number newSceneHeight) {
                //System.out.println("Height: " + newSceneHeight);
            	UI.defaultHeight = (double) newSceneHeight;
            	UI.tile.setPrefHeight(UI.defaultHeight);
                
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
  	private void checkOS(){
  		
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
