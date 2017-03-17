package application;

import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.io.LineNumberReader;
import java.net.URI;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import com.sun.javafx.application.LauncherImpl;

import javafx.application.Application;
import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.event.EventHandler;
import javafx.geometry.Rectangle2D;
import javafx.scene.Scene;
import javafx.scene.image.Image;
import javafx.scene.input.KeyCode;
import javafx.scene.input.KeyEvent;
import javafx.scene.layout.BorderPane;
import javafx.stage.Screen;
import javafx.stage.Stage;

@SuppressWarnings("restriction")
public class Main extends Application {

	// Declaring Variables
	BorderPane borderPane;
	private static Scene scene;
	protected static String uim = "";
	private Stage mainStage = new Stage();

	// Main
	public static void main(String[] args) {
		LauncherImpl.launchApplication(Main.class, PreloadScreen.class, args);
	}

	@Override
	public void init() throws Exception {
		// Builds UI
		scene = setScene();

		// Adds DarkMode and BrightMode CSS files
		scene.getStylesheets().addAll(getClass().getResource("DarkMode.css").toExternalForm(),
				getClass().getResource("BrightMode.css").toExternalForm());

		// Sets UI Mode according to settings
		if (uim.equals("dark")) {
			UIMODE("dark");
		} else {
			UIMODE("bright");
		}

		// Sets TitleBar text, starts the program maximized and sets "scene" as
		// default scene
		mainStage.setTitle("GM-Companion | © 2016-2017 Phil Hoffmann, Niklas Lüdtke | Version Beta 2.8 PRE2 (0.2.8)");
		
		// Adds the icon
		if (UI.resourceFolder != " " && UI.resourceFolder != null) {
			if (new File(UI.resourceFolder + "icon.png").exists()) {
				URI icon = new File(UI.resourceFolder + "icon.png").toURI();
				mainStage.getIcons().clear();
				mainStage.getIcons().add(new Image(icon.toString()));
			} else if (new File(UI.resourceFolder + "icon.jpg").exists()) {
				URI icon = new File(UI.resourceFolder + "icon.jpg").toURI();
				mainStage.getIcons().clear();
				mainStage.getIcons().add(new Image(icon.toString()));
			}
		}

		// Start on screen 1
		Rectangle2D bounds = Screen.getScreens().get(0).getVisualBounds();
		mainStage.setWidth(bounds.getWidth());
		mainStage.setHeight(bounds.getHeight());
		mainStage.centerOnScreen();
		mainStage.setMaximized(true);

		UI.defaultWidth = scene.getWidth();

	}

	// Start of Program
	@Override
	public void start(Stage primaryStage) {
		primaryStage = mainStage;
		primaryStage.setScene(scene);
		primaryStage.show();
	}

	public static String readSettings(String findString) {
		ArrayList<String> settings = new ArrayList<String>();
		String setting = "";
		Boolean exists = false;
		String sp = "settings.txt";
		
		try (LineNumberReader br = new LineNumberReader(new FileReader(sp))) {
			for (String line = null; (line = br.readLine()) != null;) {
				settings.add(line);
			}
		} catch (IOException e) {
			e.printStackTrace();
			System.out.println("Reading Settings Failed");
		}

		for (String s : settings) {
			if (s.contains(findString)) {
				exists = true;
				String temp = s.substring(findString.length());
				setting = temp;
				System.out.println(s);
			}
		}

		if (!exists) {
			settings.add(findString);
		}

		// Writes new settings to file
		Path p = Paths.get(sp);
		try {
			Files.write(p, settings);
		} catch (IOException e1) {
			System.out.println("Could not write settings");
			e1.printStackTrace();
		}

		System.out.println("");

		return setting;
	}

	// Defining Scene
	public Scene setScene() {

		// Reads the setting value and sets the variables accordingly
		
		// Server URL
		UI.serverURL = readSettings("SERVER_URL=");
		System.out.println("Set Server URL to " + UI.serverURL);
		
		// Auto Play
		if (readSettings("AUTO_PLAY=").toLowerCase().equals("true")) {
			UI.autoplay = true;
		} else {
			UI.autoplay = false;
		}
		System.out.println("Set Autoplay to " + UI.autoplay);
		
		// Fade Out
		if (readSettings("FADE_OUT=").toLowerCase().equals("true")) {
			UI.fadeOut = true;
		} else {
			UI.fadeOut = false;
		}
		System.out.println("Set Fade Out to " + UI.fadeOut);
		
		// Fade Duration
		try {
			UI.fadeDuration = Integer.parseInt(readSettings("FADE_DURATION="));
			System.out.println("Set Fade Duration to " + UI.fadeDuration);
		} catch (Exception e) {
			System.out.println("WARNING: FADE_DURATION is not set!");
			System.out.println("Set Fade Duration to " + UI.fadeDuration + "(Default)");
		}
		
		// Music Path
		String MUSIC_PATH = readSettings("MUSIC_PATH=");
		Music.defaultMusicPath = MUSIC_PATH;
		Music.musicDirectory = MUSIC_PATH;
		System.out.println("Set Music Path to " + MUSIC_PATH);	
		
		// Sound Path
		String SOUND_PATH = readSettings("SOUND_PATH=");
		Sound.defaultSoundPath = SOUND_PATH;
		Sound.soundDirectory = SOUND_PATH;
		System.out.println("Set Sound Path to " + SOUND_PATH);
		
		// Resource Path
		String RESOURCE_PATH = readSettings("RESOURCE_PATH=");
		UI.resourceFolder = RESOURCE_PATH;
		System.out.println("Set Resource Path to " + RESOURCE_PATH);	
		
		// Maps Path
		String MAPS_PATH = readSettings("MAPS_PATH=");
		UI.mapsFolder = MAPS_PATH;
		System.out.println("Set Maps Path to " + MAPS_PATH);
		
		// UI Mode
		String UI_MODE = readSettings("UI_MODE=");
		uim = UI_MODE;
		System.out.println("Set UI Mode to " + UI_MODE);
		
		// Online Mode
		String ONLINE_MODE = readSettings("ONLINE_MODE=");
		if (ONLINE_MODE.toLowerCase().equals("true")) {
			UI.localOnline = true;
			UI.onlineMode = true;
		} else {
			UI.localOnline = false;
			UI.onlineMode = false;
		}
		System.out.println("Set Online Mode to " + ONLINE_MODE);
		
		// Database Path
		String DATABASE_PATH = readSettings("DATABASE_PATH=");
		GM.databasePath = DATABASE_PATH;
		System.out.println("Set Database Path to " + DATABASE_PATH);
		
		// Adds Components to BorderPane
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

		// Defines scene size (Does not really matter because it starts
		// maximized anyways)
		scene = new Scene(borderPane);
		UI.defaultWidth = scene.getWidth();

		// Listens to window-size changes and updates the variables
		// (Necessary because somehow objects like the ProgressBar are not
		// updated when the window is resized by default)
		scene.widthProperty().addListener(new ChangeListener<Number>() {
			@Override
			public void changed(ObservableValue<? extends Number> observableValue, Number oldSceneWidth,
					Number newSceneWidth) {
				UI.defaultWidth = (double) newSceneWidth;
				UI.pb.setPrefWidth((double) newSceneWidth);
			}
		});

		scene.heightProperty().addListener(new ChangeListener<Number>() {
			@Override
			public void changed(ObservableValue<? extends Number> observableValue, Number oldSceneHeight,
					Number newSceneHeight) {
				UI.defaultHeight = (double) newSceneHeight;

			}
		});

		// Activates a debug mode when F1 is pressed
		// (Currently not really useful for anything, was needed in early
		// testing stages)
		scene.setOnKeyPressed(new EventHandler<KeyEvent>() {
			@Override
			public void handle(KeyEvent keyEvent) {
				if (keyEvent.getCode() == KeyCode.F1) {
					System.out.println("Dev Mode Activated!");
					if (UI.devV) {
						UI.devV = false;
						UI.debug = false;
						borderPane.setLeft(UI.addVBox());
					} else {
						UI.devV = true;
						borderPane.setLeft(UI.addVBox());
					}
				}
			}
		});

		return scene;
	}

	// Adjusting UI Mode
	public static void UIMODE(String mode) {
		if (mode.equals("dark")) {
			scene.getStylesheets().remove(1);
			System.out.println("DarkMode Activated");
		} else {
			scene.getStylesheets().remove(0);
			System.out.println("BrightMode Activated");
		}
	}

	// Sets UI.stopDownload to true when program is closed
	// (Only used by the currently disabled "Slow Server Mode")
	@Override
	public void stop() {
		System.out.println("Closing...");
		UI.stopDownload = true;
	}
}
