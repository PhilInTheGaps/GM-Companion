package application;

import java.io.File;
import java.io.IOException;
import java.net.URI;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.stream.Stream;

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
import javafx.scene.control.CheckMenuItem;
import javafx.scene.control.ContentDisplay;
import javafx.scene.control.Label;
import javafx.scene.control.ListView;
import javafx.scene.control.Menu;
import javafx.scene.control.MenuBar;
import javafx.scene.control.MenuItem;
import javafx.scene.control.ProgressBar;
import javafx.scene.control.ScrollPane;
import javafx.scene.control.SeparatorMenuItem;
import javafx.scene.control.Slider;
import javafx.scene.control.Tab;
import javafx.scene.control.TabPane;
import javafx.scene.control.TabPane.TabClosingPolicy;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.input.MouseButton;
import javafx.scene.input.MouseEvent;
import javafx.scene.layout.BorderPane;
import javafx.scene.layout.GridPane;
import javafx.scene.layout.HBox;
import javafx.scene.layout.TilePane;
import javafx.scene.layout.VBox;
import javafx.scene.media.MediaPlayer.Status;
import javafx.scene.text.TextAlignment;
import javafx.stage.DirectoryChooser;
import javafx.stage.FileChooser;
import javafx.util.Duration;

public class UI {

	// Declaring Variables
	// The volume sliders for music and sound
	public static Slider mVolumeSlider;
	public static Slider sVolumeSlider;

	// The labels on the left
	public static Label titleLabel = new Label();
	public static Label albumLabel = new Label();
	public static Label artistLabel = new Label();
	public static Label yearLabel = new Label();
	public static Label musicFolderLabel = new Label();
	public static Label soundFolderLabel = new Label();

	// The ImageView showing the cover image of the mp3
	public static ImageView coverImage = new ImageView();

	// Layouts
	static GridPane grid = new GridPane();
	static HBox botBox = new HBox();
	private static TabPane tabPane = new TabPane();
	static HBox toolBar1 = new HBox();
	static HBox toolBar2 = new HBox();
	static ListView<String> lv = new ListView<String>();
	static MenuBar menu = new MenuBar();
	static ProgressBar pb = new ProgressBar();

	static ObservableList<String> items = FXCollections.observableArrayList();
	static ExecutorService executor = Executors.newCachedThreadPool();

	// Different options
	public static Boolean autoplay = true;
	public static Boolean randomTrack = true;
	public static Boolean singleTrack = false;
	public static Boolean debug = false;
	public static Boolean devV = false;
	public static Boolean onlineMode = false;
	public static Boolean fadeOut = true;
	public static int fadeDuration = 3;
	public static String serverURL;
	public static String resourceFolder;
	public static String mapsFolder = "";

	public static String Album = "Unknown";
	public static String Title = "Unknown";
	public static String Artist = "Unknown";
	public static String Year = "";
	public static String mainPath;
	public static String musicFolder = "Not Chosen";
	public static String soundFolder = "Not Chosen";
	public static String musicFolderName = "";

	static Boolean updating = false;
	public static double defaultWidth = 1366;
	public static double defaultHeight = 768;

	// The default space between different elements like buttons
	static double defaultSpacing = 10;
	static double defaultPadding = 10;

	// The size of the volume sliders
	public static double defaultSliderHeight = 50;
	public static double defaultSliderWidth = 320;

	// Size of tabPane
	public static double defaultMusicAndSoundWidth = (defaultWidth - defaultSliderWidth - 2 * defaultPadding);
	public static double defaultFolderButtonWidth = 150;
	public static double defaultFolderButtonHeight = 150;
	public static double folderButtonWidth;

	// Menus
	private static Menu optionsMenu = new Menu("Options");
	private static Menu musicMenu = new Menu("Music");
	private static Menu soundsMenu = new Menu("Sounds");
	private static Menu gmhMenu = new Menu("GM Help");
	private static Menu mapsMenu = new Menu("Maps");

	// Menu Tabs
	private static Tab tmusic = new Tab();
	private static Tab tsound = new Tab();
	private static Tab tgm = new Tab();
	private static Tab tdb = new Tab();
	private static Tab tmaps = new Tab();

	public static int screenWidth = 1000;
	public static int screenHeight = 1000;

	private static BorderPane musicPane = new BorderPane();
	private static BorderPane soundsPane = new BorderPane();
	private static BorderPane mapsPane = new BorderPane();

	// Adds Menu
	public static MenuBar menu() {
		// Add Menus to the MenuBar
		// Adds Menus
		menu.getMenus().addAll(gmhMenu, musicMenu, soundsMenu, mapsMenu, optionsMenu);

		// Disables MnemonicParsing because it led to underscores not being
		// shown
		for (Menu m : menu.getMenus()) {
			m.setMnemonicParsing(false);
		}

		// Menu Items:
		// Random Mode
		CheckMenuItem random = new CheckMenuItem("Random Mode (Generate Random Playlists For Music Files)");
		if (randomTrack) {
			random.setSelected(true);
		} else {
			random.setSelected(false);
		}
		random.setOnAction((ActionEvent e) -> {
			if (randomTrack) {
				randomTrack = false;
				System.out.println("Disabled Random Track Mode");
			} else {
				randomTrack = true;
				System.out.println("Enabled Random Track Mode");
			}
		});

		// Single Track Mode
		CheckMenuItem single = new CheckMenuItem("Single Track Mode (Play Music Files One At A Time)");
		if (singleTrack == true) {
			single.setSelected(true);
		} else {
			single.setSelected(false);
		}
		single.setOnAction((ActionEvent e) -> {
			if (single.isSelected()) {
				singleTrack = true;
			} else {
				singleTrack = false;
			}
		});

		// Online Mode
		CheckMenuItem online = new CheckMenuItem("Online Mode (Use Server URL)");
		if (onlineMode) {
			online.setSelected(true);
		} else {
			online.setSelected(false);
		}
		online.setOnAction((ActionEvent e) -> {
			if (onlineMode) {
				onlineMode = false;
				Main.writeSettings("ONLINE_MODE=", "ONLINE_MODE=false");

			} else {
				onlineMode = true;
				Main.writeSettings("ONLINE_MODE=", "ONLINE_MODE=true");
			}
		});

		// Update Folders
		MenuItem fupdate = new MenuItem("Update Folders");
		fupdate.setOnAction((ActionEvent e) -> {
			System.out.println("Updating Folders...");
			updateMenu(musicMenu);
			updateMenu(soundsMenu);
			updateMenu(mapsMenu);

		});

		// Set AutoPlay
		CheckMenuItem checkAutoPlay = new CheckMenuItem("AutoPlay (Start Playing When Button Is Clicked)");
		if (autoplay) {
			checkAutoPlay.setSelected(true);
		} else {
			checkAutoPlay.setSelected(false);
		}
		checkAutoPlay.setOnAction((ActionEvent e) -> {
			if (autoplay) {
				autoplay = false;
				Main.writeSettings("AUTO_PLAY=", "AUTO_PLAY=false");

			} else {
				autoplay = true;
				Main.writeSettings("AUTO_PLAY=", "AUTO_PLAY=true");
			}
		});

		// Set FadeOut
		CheckMenuItem checkFadeOut = new CheckMenuItem("FadeOut (Fade Out Music At End Of Song)");
		if (fadeOut) {
			checkFadeOut.setSelected(true);
		} else {
			checkFadeOut.setSelected(false);
		}
		checkFadeOut.setOnAction((ActionEvent e) -> {
			if (fadeOut) {
				fadeOut = false;
				Main.writeSettings("FADE_OUT=", "FADE_OUT=false");

			} else {
				fadeOut = true;
				Main.writeSettings("FADE_OUT=", "FADE_OUT=true");
			}
		});

		// Set UI Mode
		CheckMenuItem checkUIMode = new CheckMenuItem("UI DarkMode (Requires Restart!)");
		if (Main.uim.equals("dark")) {
			checkUIMode.setSelected(true);
		} else {
			checkUIMode.setSelected(false);
		}
		checkUIMode.setOnAction((ActionEvent e) -> {
			if (Main.uim.equals("dark")) {
				Main.uim = "bright";
				Main.writeSettings("UI_MODE=", "UI_MODE=bright");

			} else {
				Main.uim = "dark";
				Main.writeSettings("UI_MODE=", "UI_MODE=dark");

			}
		});

		// Dice
		MenuItem dice = new MenuItem("Dice");
		dice.setOnAction((ActionEvent e) -> {
			tabPane.getSelectionModel().select(tgm);
		});

		// Database
		MenuItem db = new MenuItem("Database");
		db.setOnAction((ActionEvent e) -> {
			tabPane.getSelectionModel().select(tdb);
		});

		// Set Music Folder
		MenuItem setMusicFolder = new MenuItem("Set Music Folder");
		setMusicFolder.setOnAction((ActionEvent e) -> {
			String folder = chooser();

			Music.defaultMusicPath = folder;
			Music.musicDirectory = folder;

			updateMenu(musicMenu);

			Main.writeSettings("MUSIC_PATH=", "MUSIC_PATH=" + folder);
		});

		// Set Sound Folder
		MenuItem setSoundFolder = new MenuItem("Set Sound Folder");
		setSoundFolder.setOnAction((ActionEvent e) -> {
			String folder = chooser();

			Sound.defaultSoundPath = folder;
			Sound.soundDirectory = folder;

			updateMenu(soundsMenu);

			Main.writeSettings("SOUND_PATH=", "SOUND_PATH=" + folder);
		});

		// Set Resource Folder
		MenuItem setResourceFolder = new MenuItem("Set Resource Folder");
		setResourceFolder.setOnAction((ActionEvent e) -> {
			String folder = chooser();

			resourceFolder = folder;

			updateResources();

			Main.writeSettings("RESOURCE_PATH=", "RESOURCE_PATH=" + folder);
		});

		// Set Database Path
		MenuItem setDatabasePath = new MenuItem("Set Database Path (BETA)");
		setDatabasePath.setOnAction((ActionEvent e) -> {
			String path = fchooser();
			GM.databasePath = path;

			updateDatabase(dice, db);

			Main.writeSettings("DATABASE_PATH=", "DATABASE_PATH=" + path);
		});

		// Set Maps Folder
		MenuItem setMapsFolder = new MenuItem("Set Maps Folder");
		setMapsFolder.setOnAction((ActionEvent e) -> {
			String folder = chooser();
			mapsFolder = folder;

			updateMenu(mapsMenu);

			Main.writeSettings("MAPS_PATH=", "MAPS_PATH=" + folder);
		});

		// Seperator Items
		SeparatorMenuItem sep1 = new SeparatorMenuItem();
		SeparatorMenuItem sep2 = new SeparatorMenuItem();

		// Adding Items to Menus
		gmhMenu.getItems().addAll(dice);
		optionsMenu.getItems().addAll(random, single, online, checkAutoPlay, checkFadeOut, checkUIMode, sep2,
				setMusicFolder, setSoundFolder, setResourceFolder, setDatabasePath, setMapsFolder, sep1, fupdate);

		// If Database Path was set, database tab is shown in menu
		showDatabaseTab(dice, db);

		return menu;
	}

	// Updates the Menu Items
	public static void updateMenu(Menu menu) {
		System.out.println("Updating Menu " + menu.toString());
		menu.getItems().clear();
		updating = true;

		if (menu == musicMenu) {
			if (Music.musicIsPlaying) {
				Music.mediaPlayer.pause();

			}
			musicPane.setCenter(addMusicTabPane());
		}

		if (menu == soundsMenu) {
			if (Sound.soundIsPlaying) {
				Sound.soundPlayer.pause();

			}
			soundsPane.setCenter(addSoundsTabPane());
		}

		if (menu == mapsMenu) {
			addMapsTabPane();
		}

		System.gc();
	}

	// Updates the Menus that use resources
	private static void updateResources() {
		System.out.println("Updating Resources");

		updateMenu(musicMenu);
		updateMenu(soundsMenu);

	}

	// Updates Database
	private static void updateDatabase(MenuItem dice, MenuItem db) {
		System.out.println("Updating Database");

		if (gmhMenu.getItems().contains(db)) {
			gmhMenu.getItems().remove(db);
		}
		tdb.setContent(GM.Database());

		showDatabaseTab(dice, db);
	}

	// Shows Database Tab when Database exists
	private static void showDatabaseTab(MenuItem dice, MenuItem db) {
		if (GM.databasePath != null && GM.databasePath.toCharArray().length > 3) {
			for (int i = 0; i < gmhMenu.getItems().toArray().length; i++) {
				if (gmhMenu.getItems().get(i).equals(dice)) {
					gmhMenu.getItems().add(i + 1, db);
					System.out.println("Added Database Tab");
				}
			}
		}
	}

	// Adds the VerticalBox on the left
	public static VBox addVBox() {
		System.out.println("Adding MediaInfo Box...");
		VBox vBox = new VBox();
		vBox.setPadding(new Insets(defaultPadding));
		vBox.setMaxWidth(defaultSliderWidth + 2 * defaultPadding);
		vBox.getStyleClass().add("vbox");
		vBox.getChildren().clear();

		Label mVolumeLabel = new Label();
		mVolumeLabel.setText("Music:");
		mVolumeLabel.setStyle("-fx-font-weight: normal;");
		vBox.getChildren().add(mVolumeLabel);

		// Setting ToolBar1 Style
		toolBar1.setPadding(new Insets(defaultPadding / 2, defaultPadding / 2, defaultPadding / 4, defaultPadding / 2));
		toolBar1.setAlignment(Pos.CENTER_LEFT);
		toolBar1.alignmentProperty().isBound();
		toolBar1.setSpacing(defaultSpacing / 2);
		toolBar1.setMinHeight(50);
		toolBar1.setMaxHeight(50);
		toolBar1.getChildren().clear();
		vBox.getChildren().add(toolBar1);

		// Play Button
		Button playButton = new Button();
		playButton.setText("Play");
		playButton.setMinSize(50, 25);
		playButton.setMaxSize(100, 50);
		playButton.setPrefSize(100, 50);
		playButton.setOnAction((ActionEvent e) -> {
			if (Music.mediaPlayer.getStatus() == Status.PAUSED) {
				Music.mediaPlayer.play();
			} else if (Music.mediaPlayer.getStatus() != Status.PLAYING) {
				if (Music.musicFolderSelected == true) {
					Music.play();
				} else {
					Music.musicError = "Please select music folder!";
				}
			}
		});
		toolBar1.getChildren().add(playButton);

		// Pause Music Button
		Button pauseMButton = new Button();
		pauseMButton.setText("Pause");
		pauseMButton.setMinSize(50, 25);
		pauseMButton.setMaxSize(100, 50);
		pauseMButton.setPrefSize(100, 50);
		pauseMButton.setOnAction((ActionEvent e) -> {
			if (Music.musicFolderSelected == true) {
				Music.mediaPlayer.pause();
			} else {
				Music.musicError = "Please select sound folder!";
			}
		});
		toolBar1.getChildren().add(pauseMButton);

		// Reload Music Button
		Button reloadMButton = new Button();
		reloadMButton.setText("Replay");
		reloadMButton.setMinSize(50, 25);
		reloadMButton.setMaxSize(100, 50);
		reloadMButton.setPrefSize(100, 50);
		reloadMButton.setOnAction((ActionEvent e) -> {
			if (Music.musicFolderSelected == true) {
				Music.mediaPlayer.seek(Music.mediaPlayer.getStartTime());
			} else {
				Music.musicError = "Please select music folder!";
			}

		});
		toolBar1.getChildren().add(reloadMButton);

		// Next Music Button
		Button nextMButton = new Button();
		nextMButton.setText("Next");
		nextMButton.setMinSize(50, 25);
		nextMButton.setMaxSize(100, 50);
		nextMButton.setPrefSize(100, 50);
		nextMButton.setOnAction((ActionEvent e) -> {
			if (Music.musicFolderSelected == true) {
				Music.next();
			} else {
				Music.musicError = "Please select music folder!";
			}

		});
		toolBar1.getChildren().add(nextMButton);

		// mVolume Slider
		mVolumeSlider = new Slider();
		mVolumeSlider.setPrefWidth(defaultSliderWidth);
		mVolumeSlider.setPrefHeight(defaultSliderHeight);
		mVolumeSlider.setMinWidth(30);
		mVolumeSlider.setValue(50);

		mVolumeSlider.valueProperty().addListener(new InvalidationListener() {
			public void invalidated(Observable ov) {
				if (mVolumeSlider.isPressed()) {
					Music.musicVolume = mVolumeSlider.getValue() / 100.0;
					if (Music.musicIsPlaying == true) {
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

		// Setting ToolBar2 Style
		toolBar2.setPadding(new Insets(defaultPadding / 4, defaultPadding / 2, defaultPadding / 2, defaultPadding / 2));
		toolBar2.setAlignment(Pos.CENTER_LEFT);
		toolBar2.alignmentProperty().isBound();
		toolBar2.setSpacing(defaultSpacing / 2);
		toolBar2.setMinHeight(50);
		toolBar2.setMaxHeight(50);
		toolBar2.getChildren().clear();
		vBox.getChildren().add(toolBar2);

		// Play Button
		Button playSButton = new Button();
		playSButton.setText("Play");
		playSButton.setMinSize(50, 25);
		playSButton.setMaxSize(100, 50);
		playSButton.setPrefSize(100, 50);
		playSButton.setOnAction((ActionEvent e) -> {
			if (Sound.soundPlayer.getStatus() != Status.PAUSED) {
				Sound.soundPlayer.play();
			}

			else if (Sound.soundPlayer.getStatus() != Status.PLAYING) {
				if (Sound.soundFolderSelected == true) {
					Sound.play();
				} else {
					Sound.soundError = "Please select sound folder!";
				}
			}
		});
		toolBar2.getChildren().add(playSButton);

		// Pause Sound Button
		Button pauseSButton = new Button();
		pauseSButton.setText("Pause");
		pauseSButton.setMinSize(50, 25);
		pauseSButton.setMaxSize(100, 50);
		pauseSButton.setPrefSize(100, 50);
		pauseSButton.setOnAction((ActionEvent e) -> {
			if (Sound.soundFolderSelected == true) {
				Sound.soundPlayer.pause();
			} else {
				Sound.soundError = "Please select sound folder!";
			}
		});
		toolBar2.getChildren().add(pauseSButton);

		// Reload Sound Button
		Button reloadSButton = new Button();
		reloadSButton.setText("Replay");
		reloadSButton.setMinSize(50, 25);
		reloadSButton.setMaxSize(100, 50);
		reloadSButton.setPrefSize(100, 50);
		reloadSButton.setOnAction((ActionEvent e) -> {
			if (Sound.soundFolderSelected == true) {
				Sound.soundPlayer.seek(Sound.soundPlayer.getStartTime());
			} else {
				Sound.soundError = "Please select sound folder!";
			}

		});
		toolBar2.getChildren().add(reloadSButton);

		// Next Sound Button
		Button nextSButton = new Button();
		nextSButton.setText("Next");
		nextSButton.setMinSize(50, 25);
		nextSButton.setMaxSize(100, 50);
		nextSButton.setPrefSize(100, 50);
		nextSButton.setOnAction((ActionEvent e) -> {
			if (Sound.soundFolderSelected == true) {
				Sound.next();
			} else {
				Sound.soundError = "Please select sound folder!";
			}

		});
		toolBar2.getChildren().add(nextSButton);

		// sVolume Slider
		sVolumeSlider = new Slider();
		sVolumeSlider.setPrefWidth(defaultSliderWidth);
		sVolumeSlider.setPrefHeight(defaultSliderHeight);
		sVolumeSlider.setMinWidth(30);
		sVolumeSlider.setValue(25);

		sVolumeSlider.valueProperty().addListener(new InvalidationListener() {
			public void invalidated(Observable ov) {
				if (sVolumeSlider.isPressed()) {
					Sound.soundVolume = sVolumeSlider.getValue() / 100.0;
					if (Sound.soundIsPlaying == true) {
						Sound.soundPlayer.setVolume(sVolumeSlider.getValue() / 100.0);
					}
				}
			}
		});
		vBox.getChildren().add(sVolumeSlider);

		// Music Information
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

		// Sound Information
		Label soundInfoLabel = new Label();
		soundInfoLabel.setText("Sound Information:");
		vBox.getChildren().add(soundInfoLabel);

		soundFolderLabel.setText("Folder: " + soundFolder);
		soundFolderLabel.setStyle("-fx-font-weight: normal;");
		vBox.getChildren().add(soundFolderLabel);

		// Debug Information
		Label spacerLabel = new Label();
		Label spacerLabel3 = new Label();
		Label debugLabel = new Label();
		Label pathLabel = new Label();
		Label pathLabel2 = new Label();
		Label pathLabel3 = new Label();
		Label osLabel = new Label();

		Button debugButton = new Button();
		debugButton.setText("Debug Mode");
		debugButton.setOnAction((ActionEvent e) -> {
			if (debug) {
				debug = false;
				vBox.getChildren().remove(spacerLabel);
				vBox.getChildren().remove(debugLabel);
				vBox.getChildren().remove(pathLabel);
				vBox.getChildren().remove(pathLabel2);
				vBox.getChildren().remove(pathLabel3);
				vBox.getChildren().remove(osLabel);
			} else {
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

			}
		});
		if (devV) {
			vBox.getChildren().add(spacerLabel3);
			vBox.getChildren().add(debugButton);
		}

		if (debug) {
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

		}

		// Set Cover Image
		// coverImage.setFitHeight(defaultSliderWidth-30);
		coverImage.setFitWidth(defaultSliderWidth);
		coverImage.setPreserveRatio(true);
		vBox.getChildren().add(coverImage);

		return vBox;
	}

	// The Directory Chooser used to select folders
	private static String chooser() {
		System.out.println("Opening Directory Chooser...");
		System.out.println("");

		DirectoryChooser dc = new DirectoryChooser();
		File file = dc.showDialog(null);

		String folder = file.getAbsolutePath();
		folder = folder.replace("\\", "/");
		folder += "/";
		System.out.println(folder);
		return folder;
	}

	// The File Chooser used to select files
	private static String fchooser() {
		System.out.println("Opening File Chooser...");
		System.out.println("");

		FileChooser fc = new FileChooser();
		File file = fc.showOpenDialog(null);

		String path = file.getAbsolutePath();
		path = path.replace("\\", "/");
		path += "/";
		System.out.println(path);
		return path;
	}

	// Sets Background Image of TabPane
	private static void setBackgroundImage(String name, TabPane tp) {
		// Setting Background Image
		Runnable r = new Runnable() {
			@Override
			public void run() {
				if (new File(resourceFolder + "Backgrounds/" + name + ".png").exists()) {
					URI bip = new File(resourceFolder + "Backgrounds/" + name + ".png").toURI();
					tp.setStyle("-fx-background-image: url('" + bip
							+ "'); -fx-background-size: auto; -fx-background-position: center;");

				} else if (new File(resourceFolder + "Backgrounds/" + name + ".jpg").exists()) {
					URI bip = new File(resourceFolder + "Backgrounds/" + name + ".jpg").toURI();
					tp.setStyle("-fx-background-image: url('" + bip
							+ "'); -fx-background-size: auto; -fx-background-position: center;");
				} else {
					tp.setBackground(null);
				}
			}
		};
		executor.submit(r);
	}

	// Generates Categories for Music and Sounds
	private static ArrayList<String> generateCategories(ArrayList<String> list, String directory, String url) {
		System.out.println("Generating categories...");

		File file = new File(directory);
		System.out.println(file);

		if (onlineMode) {
			// Get all foldernames from server
			Document doc = null;
			try {
				doc = Jsoup.connect(url).get();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			System.out.println(doc.toString());
			String str = doc.toString();
			String findStr = "<li><a href=";
			int lastIndex1 = 0;
			int lastIndex2 = 10;
			ArrayList<String> folderNames = new ArrayList<String>();
			String test = new String();

			System.out.println("Found the following category folders:");
			while (lastIndex1 != -1) {

				lastIndex1 = str.indexOf(findStr, lastIndex1);
				lastIndex2 = str.indexOf("/", lastIndex1);

				if (lastIndex1 != -1) {
					for (int i = lastIndex1 + findStr.length() + 1; i < lastIndex2; i++) {
						test += str.charAt(i);
					}
					System.out.println(test);
					folderNames.add(test);
					test = "";
					lastIndex1 += findStr.length();
				}
			}
			for (String folder : folderNames) {
				list.add(folder);
			}
			list.remove(0);
		} else {
			String[] names = file.list();

			System.out.println("Found the following category folders:");

			if (names != null) {
				for (String name : names) {
					if (new File(Music.musicDirectory + name).isDirectory()) {
						System.out.println(name);
						list.add(name);
					}
				}
			}
		}

		Collections.sort(list);

		updating = false;

		System.out.println("Added music category tabs");
		System.out.println("");

		return list;
	}

	// Generates Tabs from Categories
	private static void generateTabs(ArrayList<String> list, TabPane tp, Tab tab, Menu menu, Boolean generateTiles,
			String type, String defaultPath, String serverURL) {
		for (String folder : list) {
			Tab t = new Tab();
			MenuItem mi = new MenuItem();
			String name = folder;
			String nName = name.replace("_", " ");
			t.setClosable(false);
			t.setId(name);
			t.setText(nName);
			mi.setMnemonicParsing(false);
			mi.setText(nName);

			t.setOnSelectionChanged((Event e) -> {
				if (t.isSelected()) {
					setBackgroundImage(name, tp);
				}
			});

			ScrollPane s = new ScrollPane();
			s.setBackground(null);
			s.setFitToWidth(true);

			if (generateTiles) {
				s.setContent(addButtons(folder, defaultPath, serverURL, type, folder.toString()));
			}

			t.setContent(s);

			mi.setOnAction((ActionEvent e) -> {
				tabPane.getSelectionModel().select(tab);
				tp.getSelectionModel().select(t);
			});

			tp.getTabs().add(t);
			menu.getItems().add(mi);
		}
	}

	// Adds Music TabPane
	private static TabPane addMusicTabPane() {
		TabPane tp = new TabPane();
		tp.setId("TEST ID");
		ArrayList<String> list = new ArrayList<String>();
		list.clear();
		tp.setTabClosingPolicy(TabClosingPolicy.UNAVAILABLE);
		tp.setTabMinWidth(200);
		tp.setTabMinHeight(40);

		// Generation Tabs
		ArrayList<String> categories = generateCategories(list, Music.musicDirectory, Music.serverMusicURL);
		generateTabs(categories, tp, tmusic, musicMenu, true, "Music", Music.defaultMusicPath, Music.serverMusicURL);

		tp.getSelectionModel().select(0);

		return tp;
	}

	// Adds Sounds TabPane
	private static TabPane addSoundsTabPane() {
		TabPane tp = new TabPane();
		ArrayList<String> list = new ArrayList<String>();
		tp.setTabClosingPolicy(TabClosingPolicy.UNAVAILABLE);
		tp.setTabMinWidth(200);
		tp.setTabMinHeight(40);

		generateTabs(generateCategories(list, Sound.soundDirectory, Sound.serverSoundsURL), tp, tsound, soundsMenu,
				true, "Sounds", Sound.defaultSoundPath, Sound.serverSoundsURL);

		return tp;
	}

	// Adds Maps TabPane
	private static TabPane addMapsTabPane() {
		TabPane tp = new TabPane();
		ArrayList<File> list = new ArrayList<File>();
		tp.setTabClosingPolicy(TabClosingPolicy.UNAVAILABLE);
		tp.setTabMinWidth(200);
		tp.setTabMinHeight(40);

		System.out.println("Getting Maps...");
		File file = new File(mapsFolder);
		if (mapsFolder != "" && mapsFolder != " ") {
			System.out.println(file);
			try (Stream<Path> paths = Files.walk(Paths.get(mapsFolder))) {
				paths.forEach(filePath -> {
					if (Files.isRegularFile(filePath)) {
						if (filePath.toString().contains(".jpg") || filePath.toString().contains(".jpeg")
								|| filePath.toString().contains(".png")) {
							System.out.println("Path: " + filePath);
							File f = new File(filePath.toString());
							list.add(f);
						}
					}
				});
				paths.close();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
		Collections.sort(list);

		// Generating Tabs

		for (File map : list) {
			Tab t = new Tab();
			MenuItem mi = new MenuItem();
			String name = map.getName().toString().substring(0, map.getName().indexOf("."));
			String nName = name.replace("_", " ");
			t.setClosable(false);
			t.setId(name);
			t.setText(nName);
			mi.setMnemonicParsing(false);
			mi.setText(nName);

			Image img = new Image(map.toURI().toString(), 2 * screenWidth, 2 * screenHeight, true, true);
			ImageView iv = new ImageView(img);
			iv.setPreserveRatio(true);
			iv.autosize();

			ScrollPane s = new ScrollPane();
			s.setBackground(null);
			s.setFitToWidth(true);
			s.setContent(iv);
			t.setContent(s);
			tp.getTabs().add(t);

			mi.setOnAction((ActionEvent e) -> {
				tabPane.getSelectionModel().select(tmaps);
				tp.getSelectionModel().select(t);
			});
			mapsMenu.getItems().add(mi);
		}

		return tp;
	}

	// Adds Buttons for folders in categories
	private static TilePane addButtons(String directory, String mainDirectory, String serverURL, String type,
			String iconFolder) {
		System.out.println("Generating buttons for directory: " + directory);

		TilePane tile = new TilePane();
		tile.setPadding(new Insets(defaultPadding, defaultPadding / 2, defaultPadding, defaultPadding));
		tile.setVgap(defaultPadding / 4);
		tile.setHgap(defaultPadding / 4);
		tile.setPrefColumns(3);
		tile.setPrefWidth(defaultMusicAndSoundWidth);

		tile.getChildren().clear();

		ScrollPane sp = new ScrollPane();
		sp.setContent(tile);

		List<String> folders = new ArrayList<String>();

		File file = new File(mainDirectory + directory + "/");

		if (onlineMode) {
			// Get all foldernames from server
			Document doc = null;
			try {
				doc = Jsoup.connect(serverURL + directory + "/").get();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			String str = doc.toString();
			String findStr = "<li><a href=";
			int lastIndex1 = 0;
			int lastIndex2 = 10;
			String temp = new String();

			System.out.println("Found the following folders:");
			while (lastIndex1 != -1) {

				lastIndex1 = str.indexOf(findStr, lastIndex1);
				lastIndex2 = str.indexOf("/", lastIndex1);

				if (lastIndex1 != -1) {
					for (int i = lastIndex1 + findStr.length() + 1; i < lastIndex2; i++) {
						temp += str.charAt(i);
					}
					System.out.println(temp);
					folders.add(temp);
					temp = "";
					lastIndex1 += findStr.length();
				}
			}
			folders.remove(0);
		} else {
			String[] names = file.list();
			System.out.println("Found the following folders:");

			if (names != null) {
				for (String name : names) {
					if (new File(mainDirectory + directory + "/" + name).isDirectory()) {
						System.out.println(name);
						folders.add(name);
					}
				}
			}
		}

		Collections.sort(folders);

		for (String folder : folders) {
			if (folder != null) {
				String bName = new String();
				bName = folder;
				Button b = new Button();
				b.setMnemonicParsing(false);
				b.setText("");
				b.setWrapText(true);
				b.setAlignment(Pos.CENTER);

				VBox v = new VBox();
				v.setMaxWidth(150);
				v.setAlignment(Pos.TOP_CENTER);
				VBox v2 = new VBox();
				v2.setMaxWidth(150);
				v2.setAlignment(Pos.CENTER);
				Label l = new Label();
				l.setWrapText(true);
				l.setMaxWidth(Double.MAX_VALUE);

				l.setTextAlignment(TextAlignment.CENTER);
				l.setContentDisplay(ContentDisplay.CENTER);

				String nbName = bName.replace("%2520", "%20");
				l.setText(nbName.replace("%20", " ").replace("_", " "));
				b.setPrefSize(defaultFolderButtonWidth, defaultFolderButtonHeight);
				b.setMinSize(defaultFolderButtonWidth, defaultFolderButtonHeight);
				b.setMaxSize(defaultFolderButtonWidth, defaultFolderButtonHeight);
				b.getStyleClass().add("button1");
				
				if (new File(resourceFolder + "Icons/" + type + "/" + iconFolder + "/" + bName + ".png").exists()) {
					URI pic = new File(resourceFolder + "Icons/" + type + "/" + iconFolder + "/" + bName + ".png")
							.toURI();
					b.setStyle("-fx-background-image: url('" + pic + "'); -fx-opacity: 0.9; -fx-text-fill: white");
				} else if (new File(resourceFolder + "Icons/" + type + "/" + iconFolder + "/" + bName + ".jpg")
						.exists()) {
					URI pic = new File(resourceFolder + "Icons/" + type + "/" + iconFolder + "/" + bName + ".jpg")
							.toURI();
					b.setStyle("-fx-background-image: url('" + pic + "'); -fx-opacity: 0.9;");
				} else {
					b.setText(nbName.replace("_", " "));
				}

				b.setOnAction((ActionEvent e) -> {
					if (type.equals("Sounds")) {
						Boolean initial = Sound.Initial();
						System.out.println(initial);
						if (initial == false) {
							Sound.soundPlayer.pause();
							Sound.soundPlayer.stop();
						}
						if (Sound.soundFolderSelected) {
							Sound.soundPlayer.stop();
						}
						if (onlineMode) {
							Sound.defaultSoundPath = directory + "/" + nbName;
							System.out.println();
						} else {
							Sound.defaultSoundPath = Sound.soundDirectory + directory + "/" + nbName;
						}

						soundFolder = directory + "/" + nbName;
						soundFolderLabel.setText("Folder: " + soundFolder);
						Sound.soundFolderSelected = true;

						Sound.initialPress = true;

						if (Sound.soundIsPlaying == true) {
							Sound.soundPlayer.stop();
						}

						try {
							Sound.get();
						} catch (IOException e1) {
							e1.printStackTrace();
						}
						if (autoplay) {
							Sound.play();
						}
					}
					if (type.equals("Music")) {
						if (onlineMode) {
							Music.defaultMusicPath = directory + "/" + nbName;
							System.out.println();
						} else {
							Music.defaultMusicPath = Music.musicDirectory + directory + "/" + nbName;
						}

						musicFolder = directory + "/" + nbName;
						musicFolderName = nbName;
						musicFolderLabel.setText("Folder: " + musicFolder);
						Music.musicFolderSelected = true;

						Music.initialPress = true;

						if (Music.musicIsPlaying == true) {
							Music.mediaPlayer.stop();
						}

						try {
							Music.get();
						} catch (IOException e1) {
							e1.printStackTrace();
						}
						if (autoplay) {
							Music.play();
						}
					}

				});

				v2.getChildren().add(l);
				v.getChildren().addAll(b, v2);
				tile.getChildren().add(v);
			}
		}

		updating = false;

		System.out.println("Added buttons");
		System.out.println("");

		return tile;
	}

	// Adds ListView with Music files
	private static ListView<String> addListView() {

		lv.setMaxHeight(150);
		lv.setFocusTraversable(false);
		lv.setItems(items);
		lv.setOnMouseClicked(new EventHandler<MouseEvent>() {
			@Override
			public void handle(MouseEvent mouseEvent) {
				if (mouseEvent.getButton().equals(MouseButton.PRIMARY)) {
					if (mouseEvent.getClickCount() == 2) {
						Music.currentTrackID = lv.getSelectionModel().getSelectedIndex() - 1;
						Music.next();
					}
				}
			}
		});
		return lv;
	}

	// Add TabPane
	public static TabPane addTabPane() {
		// Settings size and properties
		tabPane.setTabMinWidth(200);
		tabPane.setTabMinHeight(45);
		tabPane.getTabs().clear();
		tabPane.setTabClosingPolicy(TabClosingPolicy.UNAVAILABLE);
		tabPane.getStylesheets().clear();
		tabPane.getStyleClass().add(".tab-pane");

		// Music TabPane
		musicPane.setCenter(addMusicTabPane());
		musicPane.setBottom(addListView());

		// Sound TabPane
		soundsPane.setCenter(addSoundsTabPane());

		// Maps TabPane
		mapsPane.setCenter(addMapsTabPane());

		tmusic.setClosable(false);
		tmusic.setText("Music");
		tmusic.setContent(musicPane);
		tabPane.getTabs().add(tmusic);

		tsound.setClosable(false);
		tsound.setText("Sounds");
		tsound.setContent(soundsPane);
		tabPane.getTabs().add(tsound);

		tgm.setClosable(false);
		tgm.setText("GM Help");
		tgm.setContent(GM.Dice());
		tabPane.getTabs().add(tgm);

		tdb.setClosable(false);
		tdb.setText("Database");
		tdb.setContent(GM.Database());
		tabPane.getTabs().add(tdb);

		tmaps.setClosable(false);
		tmaps.setText("Maps");
		tmaps.setContent(mapsPane);
		tabPane.getTabs().add(tmaps);
		return tabPane;
	}

	// Updates the ProgressBar
	public static void updatePB() {
		ChangeListener<Duration> progressChangeListener;
		progressChangeListener = new ChangeListener<Duration>() {
			@Override
			public void changed(ObservableValue<? extends Duration> observableValue, Duration oldValue,
					Duration newValue) {
				pb.setProgress(1.0 * Music.mediaPlayer.getCurrentTime().toMillis()
						/ Music.mediaPlayer.getTotalDuration().toMillis());

				// Fade Out
				if (fadeOut) {
					if (Music.mediaPlayer.getCurrentTime()
							.toSeconds() > Music.mediaPlayer.getTotalDuration().toSeconds() - fadeDuration) {
						if (Music.fading == false) {
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

	// Adds ProgressBar
	public static HBox addBotBox() throws IOException {
		botBox.setMinHeight(20);

		pb.setPrefWidth(defaultWidth);
		pb.setPrefHeight(5);
		pb.setProgress(0);
		botBox.getChildren().add(pb);

		return UI.botBox;
	}
}
