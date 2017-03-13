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

public class Main extends Application {

	// Defining Variables
	BorderPane borderPane;
	static Scene scene;
	static String uim = "";
	public static ArrayList<String> settings = new ArrayList<String>();

	// Main
	public static void main(String[] args) {
		launch(args);
	}

	// Start of Program
	@Override
	public void start(Stage primaryStage) {

		System.out.println("Initializing...");
		System.out.println("");

		// Builds UI
		scene = setScene();

		// Adds DarkMode and BrightMode CSS files
		scene.getStylesheets().addAll(getClass().getResource("DarkMode.css").toExternalForm(),
				getClass().getResource("BrightMode.css").toExternalForm());

		// Sets TitleBar text, starts the program maximized and sets "scene" as
		// default scene
		primaryStage
				.setTitle("GM-Companion | © 2016-2017 Phil Hoffmann, Niklas Lüdtke | Version Beta 2.8 PRE1 (0.2.8)");

		// Adds the icon
		if (UI.resourceFolder != " " && UI.resourceFolder != null) {
			if (new File(UI.resourceFolder + "icon.png").exists()) {
				URI icon = new File(UI.resourceFolder + "icon.png").toURI();
				primaryStage.getIcons().clear();
				primaryStage.getIcons().add(new Image(icon.toString()));
			} else if (new File(UI.resourceFolder + "icon.jpg").exists()) {
				URI icon = new File(UI.resourceFolder + "icon.jpg").toURI();
				primaryStage.getIcons().clear();
				primaryStage.getIcons().add(new Image(icon.toString()));
			}
		}

		// Sets UI Mode according to settings
		if (uim.equals("dark")) {
			UIMODE("dark");
		} else {
			UIMODE("bright");
		}

		;
		primaryStage.hide();
		primaryStage.setScene(scene);

		// Start on screen 1
		Rectangle2D bounds = Screen.getScreens().get(0).getVisualBounds();
		primaryStage.setWidth(bounds.getWidth());
		primaryStage.setHeight(bounds.getHeight());
		primaryStage.centerOnScreen();
		primaryStage.setMaximized(true);
		primaryStage.show();
		UI.defaultWidth = scene.getWidth();
	}

	// Defining Scene
	public Scene setScene() {

		// Reads Settings.txt (Located next to the .jar when exported)
		String sp = "settings.txt";
		System.out.println("Reading Settings...");
		try (LineNumberReader br = new LineNumberReader(new FileReader(sp))) {
			for (String line = null; (line = br.readLine()) != null;) {
				settings.add(line);
			}
		} catch (IOException e) {
			e.printStackTrace();
			System.out.println("Reading Settings Failed");
		}

		// Variables indicate, whether specific setting was found in
		// "settings.txt"
		Boolean server_url = false;
		Boolean auto_play = false;
		Boolean fade_out = false;
		Boolean fade_duration = false;
		Boolean music_path = false;
		Boolean sound_path = false;
		Boolean resource_path = false;
		Boolean ui_mode = false;
		Boolean online_mode = false;
		Boolean database_path = false;
		Boolean maps_path = false;

		// Reads the setting value and sets the variables accordingly
		for (String s : settings) {
			if (s.contains("SERVER_URL=")) {
				server_url = true;
				String surl = s.substring("SERVER_URL=".length());
				UI.serverURL = surl;
				Music.serverMusicURL = surl + "music/";
				Sound.serverSoundsURL = surl + "sounds/";
				System.out.println("Set Server URL to " + surl);
			}
			if (s.contains("AUTO_PLAY=")) {
				auto_play = true;
				String ap = s.substring("AUTO_PLAY=".length());
				if (ap.toLowerCase().equals("true")) {
					UI.autoplay = true;
				} else {
					UI.autoplay = false;
				}
				System.out.println("Set Autoplay to " + UI.autoplay);
			}
			if (s.contains("FADE_OUT=")) {
				fade_out = true;
				String fo = s.substring("FADE_OUT=".length());
				if (fo.toLowerCase().equals("true")) {
					UI.fadeOut = true;
				} else {
					UI.fadeOut = false;
				}
				System.out.println("Set Fade Out to " + UI.fadeOut);
			}
			if (s.contains("FADE_DURATION=")) {
				fade_duration = true;
				String fd = s.substring("FADE_DURATION=".length());
				try {
					UI.fadeDuration = Integer.parseInt(fd);
					System.out.println("Set Fade Duration to " + UI.fadeDuration);
				} catch (Exception e) {
					System.out.println("WARNING: FADE_DURATION is not set!");
					System.out.println("Set Fade Duration to " + UI.fadeDuration + "(Default)");
				}
			}
			if (s.contains("MUSIC_PATH=")) {
				music_path = true;
				String MUSIC_PATH = s.substring("MUSIC_PATH=".length());
				Music.defaultMusicPath = MUSIC_PATH;
				Music.musicDirectory = MUSIC_PATH;
				System.out.println("Set Music Path to " + MUSIC_PATH);
			}
			if (s.contains("SOUND_PATH=")) {
				sound_path = true;
				String SOUND_PATH = s.substring("SOUND_PATH=".length());
				Sound.defaultSoundPath = SOUND_PATH;
				Sound.soundDirectory = SOUND_PATH;
				System.out.println("Set Sound Path to " + SOUND_PATH);
			}
			if (s.contains("RESOURCE_PATH=")) {
				resource_path = true;
				String RESOURCE_PATH = s.substring("RESOURCE_PATH=".length());
				UI.resourceFolder = RESOURCE_PATH;
				System.out.println("Set Resource Path to " + RESOURCE_PATH);
			}
			if (s.contains("MAPS_PATH=")) {
				maps_path = true;
				String MAPS_PATH = s.substring("MAPS_PATH=".length());
				UI.mapsFolder = MAPS_PATH;
				System.out.println("Set Maps Path to " + MAPS_PATH);
			}
			if (s.contains("UI_MODE=")) {
				ui_mode = true;
				String UI_MODE = s.substring("UI_MODE=".length());
				uim = UI_MODE;
				System.out.println("Set UI Mode to " + UI_MODE);
			}
			if (s.contains("ONLINE_MODE=")) {
				online_mode = true;
				String ONLINE_MODE = s.substring("ONLINE_MODE=".length());
				if (ONLINE_MODE.toLowerCase().equals("true")) {
					UI.localOnline = true;
					UI.onlineMode = true;
				} else {
					UI.localOnline = false;
					UI.onlineMode = false;
				}
				System.out.println("Set Online Mode to " + ONLINE_MODE);
			}
			if (s.contains("DATABASE_PATH=")) {
				database_path = true;
				String DATABASE_PATH = s.substring("DATABASE_PATH=".length());
				GM.databasePath = DATABASE_PATH;
				System.out.println("Set Database Path to " + DATABASE_PATH);
			}
		}

		// If a setting was not found, it is added to the list
		if (!server_url) {
			settings.add("SERVER_URL=");
		}
		if (!auto_play) {
			settings.add("AUTO_PLAY=");
		}
		if (!fade_out) {
			settings.add("FADE_OUT=");
		}
		if (!fade_duration) {
			settings.add("FADE_DURATION=");
		}
		if (!music_path) {
			settings.add("MUSIC_PATH=");
		}
		if (!sound_path) {
			settings.add("SOUND_PATH=");
		}
		if (!resource_path) {
			settings.add("RESOURCE_PATH=");
		}
		if (!maps_path) {
			settings.add("MAPS_PATH=");
		}
		if (!ui_mode) {
			settings.add("UI_MODE=");
		}
		if (!online_mode) {
			settings.add("ONLINE_MODE=");
		}
		if (!database_path) {
			settings.add("DATABASE_PATH=");
		}

		// Writes new settings to file
		Path p = Paths.get(sp);
		try {
			Files.write(p, settings);
		} catch (IOException e1) {
			System.out.println("Could not write settings");
			e1.printStackTrace();
		}

		System.out.println("Finished Reading Settings");
		System.out.println("");

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
