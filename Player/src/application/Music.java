package application;

import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Random;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.stream.Stream;

import org.jsoup.Jsoup;
import org.jsoup.nodes.Document;

import javafx.animation.KeyFrame;
import javafx.animation.KeyValue;
import javafx.animation.Timeline;
import javafx.collections.MapChangeListener;
import javafx.scene.image.Image;
import javafx.scene.media.Media;
import javafx.scene.media.MediaPlayer;
import javafx.util.Duration;

import java.net.MalformedURLException;
import java.net.URL;
import org.apache.commons.io.FileUtils;

public class Music {
	public static MediaPlayer mediaPlayer;
	public static Media mMedia;
	public static String musicPath;
	public static String[] musicPathList = new String[500];
	public static String currentTrack;
	public static int maxTrackCount;
	public static Boolean musicIsPlaying = false;
	public static Boolean musicFolderSelected = false;
	public static String musicError;
	public static Random randomTrackID = new Random();
	public static int currentTrackID;
	public static double musicVolume = 0.5;
	public static Boolean initialPress = false;
	static String slowFolder;
	static ArrayList<String> slowFileNames = new ArrayList<String>();
	public static ExecutorService executor = Executors.newCachedThreadPool();
	public static Boolean fading = false;

	static int folderCount = 0;
	static int folderID = 0;
	static int scenCount = 0;
	static int scenID = 0;
	static int musicID = 1;

	public static String musicDirectory = "";
	public static String defaultMusicPath = "";
	public static String serverMusicURL = "";

	public static void play() {
		System.out.println("Converting File Path...");
		// Gets the path to the current music file and converts it, so the
		// MediaPlayer can read it
		musicPath = musicPathList[currentTrackID];

		musicPath = musicPath.replace("\\", "/");

		// Select Song in ListView
		UI.lv.getSelectionModel().select(currentTrackID);

		if (UI.onlineMode) {
			if (UI.slowServer) {
				mMedia = new Media(new File(musicPath).toURI().toString());
			} else {
				mMedia = new Media(musicPath);
			}
		} else {
			mMedia = new Media(new File(musicPath).toURI().toString());
		}

		// Creates a MediaPlayer that plays the music file
		System.out.println("Creating MediaPlayer...");
		mediaPlayer = new MediaPlayer(mMedia);
		mediaPlayer.setAutoPlay(true);
		mediaPlayer.setVolume(musicVolume);
		musicIsPlaying = true;

		// Clear Metadata
		System.out.println("Clearing old MetaData...");
		UI.albumLabel.setText("Album: Unknown");
		UI.artistLabel.setText("Artist: Unknown");
		UI.titleLabel.setText("Title: Unknown");
		UI.yearLabel.setText("Year: Unknown");
		UI.coverImage.setImage(null);

		// If the music folder button was pressed, generate a new random
		// playlist
		if (initialPress) {
			initialPress = false;
			currentTrackID = maxTrackCount - 1;
			next();
		}

		// Get Metadata
		System.out.println("Getting new MetaData...");
		mMedia.getMetadata().addListener(new MapChangeListener<String, Object>() {
			@Override
			public void onChanged(Change<? extends String, ? extends Object> ch) {
				if (ch.wasAdded()) {
					handleMetaData(ch.getKey(), ch.getValueAdded());
				}
			}
		});

		// Plays the next music file if the current one ends
		mediaPlayer.setOnEndOfMedia(new Runnable() {
			@Override
			public void run() {
				if (UI.singleTrack == false) {
					next();
				}
			}
		});

		// Updates the Progress Bar and sets Metadata
		mediaPlayer.setOnReady(new Runnable() {
			@Override
			public void run() {
				System.out.println("Duration: " + mMedia.getDuration().toSeconds());
				UI.updatePB();
			}
		});
	}

	public static void handleMetaData(String key, Object value) {
		switch (key) {
		case ("album"):
			UI.Album = value.toString();
			UI.albumLabel.setText("Album: " + UI.Album);
			System.out.println("Album: " + UI.Album);
			break;
		case ("artist"):
			UI.Artist = value.toString();
			UI.artistLabel.setText("Artist: " + UI.Artist);
			System.out.println("Artist: " + UI.Artist);
			break;
		case ("title"):
			UI.Title = value.toString();
			UI.titleLabel.setText("Title: " + UI.Title);
			System.out.println("Title: " + UI.Title);
			break;
		case ("year"):
			UI.Year = value.toString();
			UI.yearLabel.setText("Year: " + UI.Year);
			System.out.println("Year: " + UI.Year);
			break;
		}
		UI.coverImage.setImage((Image) mediaPlayer.getMedia().getMetadata().get("image"));
	}

	public static void downloadFile(String downloadPath, String downloadUrl) throws MalformedURLException, IOException {
		System.out.println(downloadPath);
		if (new File(downloadPath).isFile()) {
			System.out.println("Is already downloaded, switching to next one");
			System.out.println("");
			System.out.println("FolderID: " + folderID);
			System.out.println("folderCount: " + folderCount);
			System.out.println("scenID: " + scenID);
			System.out.println("scenCount: " + scenCount);
			if (folderID >= folderCount) {

				if (scenID < scenCount - 1) {
					System.out.println("Switching to next scenario");
					scenID++;
				} else {
					System.out.println("Setting Scene ID 0");
					scenID = 0;
					musicID++;
				}
				System.out.println("Folder = 0");
				folderID = 0;
			} else {
				System.out.println("Switching to next folder");
				folderID++;
			}

			if (UI.stopDownload == false) {
				setDownloadFile();
			}
		} else {
			System.out.println("Starting Downloading to " + downloadPath);
			try {
				FileUtils.copyURLToFile(new URL(downloadUrl), new File(downloadPath));
			} catch (IOException e) {
				System.out.println("Something went wrong");
			}
			System.out.println("Finished Downloading");
			System.out.println("");

			folderID++;
			if (UI.stopDownload == false) {
				setDownloadFile();
			}
		}
	}

	public static void setDownloadFile() {
		Runnable r = new Runnable() {
			public void run() {

				// System.out.println("Getting Scenarios...");

				String[] scenArrayTemp = new String[500];
				String[] scenArray = new String[500];

				Document doc = null;
				try {
					doc = Jsoup.connect(Music.serverMusicURL).get();
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				// System.out.println(doc.toString());

				String str = doc.toString();
				String findStr = "<li><a href=";
				int lastIndex1 = 0;
				int lastIndex2 = 10;

				ArrayList<String> folderNames = new ArrayList<String>();

				String test = new String();
				int count = 0;

				// Finding Scenario Folders
				// System.out.println("Found the following scenario folders:");
				while (lastIndex1 != -1) {

					lastIndex1 = str.indexOf(findStr, lastIndex1);
					lastIndex2 = str.indexOf("/", lastIndex1);

					if (lastIndex1 != -1) {
						for (int i = lastIndex1 + findStr.length() + 1; i < lastIndex2; i++) {
							test += str.charAt(i);
						}
						// System.out.println(test);
						folderNames.add(test);
						test = "";
						count += 1;
						lastIndex1 += findStr.length();
					}
				}

				// System.out.println("");

				// Adding Scenarios to array
				for (int i = 0; i < folderNames.size(); i++) {
					String temp = folderNames.get(i).toString();
					scenArray[i] = temp;
				}

				// Shifting array +1 because of the "Parent Directory" link
				for (int i = 1; i < count; i++) {
					scenArrayTemp[i - 1] = scenArray[i].toString();
				}
				scenArray = scenArrayTemp;

				// Setting amount of scenarios
				scenCount = 0;
				for (int i = 0; i < scenArray.length; i++) {
					if (scenArray[i] != null) {
						scenCount++;
					}
				}

				// Getting Music Folders at scenario
				String[] folderArray = new String[500];
				String[] folderArrayTemp = new String[500];

				// Get all foldernames from server
				try {
					doc = Jsoup.connect(Music.serverMusicURL + scenArray[scenID] + "/").get();
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				// System.out.println(doc.toString());
				str = doc.toString();

				lastIndex1 = 0;
				lastIndex2 = 10;
				folderNames = new ArrayList<String>();
				count = 0;

				// System.out.println("Found the following music folders:");
				while (lastIndex1 != -1) {

					lastIndex1 = str.indexOf(findStr, lastIndex1);
					lastIndex2 = str.indexOf("/", lastIndex1);

					if (lastIndex1 != -1) {
						for (int i = lastIndex1 + findStr.length() + 1; i < lastIndex2; i++) {
							test += str.charAt(i);
						}
						// System.out.println(test);
						folderNames.add(test);
						test = "";
						count += 1;
						lastIndex1 += findStr.length();
					}
				}
				// System.out.println("");

				for (int i = 0; i < folderNames.size(); i++) {
					String temp = folderNames.get(i).toString();
					folderArray[i] = temp;
				}

				for (int i = 1; i < count; i++) {
					folderArrayTemp[i - 1] = folderArray[i].toString();
				}
				folderCount = count - 2;
				folderArray = folderArrayTemp;
				slowFolder = folderArray[folderID];

				// This finds every music file in the folder and writes them
				// into a list
				// System.out.println("Finding music files in folder...");

				currentTrackID = 0;

				for (int i = 0; i < 500; i++) {
					musicPathList[i] = "";
				}

				// Get all music files from server
				try {
					doc = Jsoup.connect(serverMusicURL + scenArray[scenID] + "/" + folderArray[folderID] + "/").get();
				} catch (IOException e1) {
					// TODO Auto-generated catch block
					e1.printStackTrace();
				}

				// System.out.println(doc.toString());
				str = doc.toString();
				findStr = "href=";
				lastIndex1 = 0;
				lastIndex2 = 10;
				ArrayList<String> fileNames = new ArrayList<String>();
				test = new String();
				count = 0;

				while (lastIndex1 != -1) {

					lastIndex1 = str.indexOf(findStr, lastIndex1);
					lastIndex2 = str.indexOf(".mp3", lastIndex1);

					if (lastIndex1 != -1) {
						for (int i = lastIndex1 + findStr.length() + 1; i < lastIndex2 + 4; i++) {
							test += str.charAt(i);
						}
						fileNames.add(test);
						// System.out.println(test);
						test = "";
						lastIndex1 += findStr.length();
					}
				}
				maxTrackCount = count;

				currentTrackID = 0;

				// Prints out the name of every music file in the folder
				// System.out.println("Found the following files:");
				for (int i = 1; i < maxTrackCount; i++) {
					// System.out.println(fileNames.get(i));
				}
				// System.out.println("");

				// Check if download directory exists
				if (new File("Download/" + UI.serverURL.substring(7, UI.serverURL.length() - 2) + "Music/"
						+ scenArray[scenID] + "/" + folderArray[folderID] + "/").isFile()) {
					System.out.println("Directory exists, downloading...");
				} else {
					System.out.println("Directory does not exist, creating...");
					new File("Download/" + UI.serverURL.substring(7, UI.serverURL.length() - 2) + "Music/"
							+ scenArray[scenID] + "/" + folderArray[folderID] + "/").mkdirs();
				}

				// Download File
				try {
					downloadFile(
							"Download/" + UI.serverURL.substring(7, UI.serverURL.length()) + "Music/"
									+ scenArray[scenID] + "/" + folderArray[folderID] + "/" + fileNames.get(musicID),
							serverMusicURL + scenArray[scenID] + "/" + folderArray[folderID] + "/"
									+ fileNames.get(musicID));
				} catch (MalformedURLException e) {
					System.out.println("ERROR: Malformed URL");
					e.printStackTrace();
				} catch (IOException e) {
					System.out.println("ERROR: Something went wrong");
					e.printStackTrace();
				}
			}
		};
		executor.submit(r);
	}

	public static void get() throws IOException {
		// This finds every music file in the folder and writes them into a list

		// System.out.println("Finding music files in folder:
		// "+"Download/"+UI.serverURL.substring(7,
		// UI.serverURL.length())+"Music/"+defaultMusicPath+"/");

		currentTrackID = 0;

		for (int i = 0; i < 500; i++) {
			musicPathList[i] = "";
		}

		if (UI.onlineMode) {
			if (UI.slowServer) {
				try (Stream<Path> paths = Files.walk(Paths.get("Download/"
						+ UI.serverURL.substring(7, UI.serverURL.length()) + "Music/" + defaultMusicPath + "/"))) {
					paths.forEach(filePath -> {
						if (Files.isRegularFile(filePath)) {
							System.out.println("Path: " + filePath);
							String tempPath = filePath.toString();
							musicPathList[currentTrackID] = tempPath;
							currentTrackID++;
						}
					});
					defaultMusicPath = "Download/" + UI.serverURL.substring(7, UI.serverURL.length()) + "Music/"
							+ defaultMusicPath + "/";
				} catch (IOException e) {
					System.out.println("Directory not existing, creating it...");
					new File(defaultMusicPath).mkdirs();
					// e.printStackTrace();
				}

				maxTrackCount = currentTrackID;
				System.out.println(maxTrackCount);
			} else {
				// Get all music files from server
				Document doc = Jsoup.connect(serverMusicURL + defaultMusicPath).get();

				System.out.println(doc.toString());
				String str = doc.toString();
				String findStr = "href=";
				int lastIndex1 = 0;
				int lastIndex2 = 10;
				ArrayList<String> fileNames = new ArrayList<String>();
				String test = new String();
				int count = 0;

				while (lastIndex1 != -1) {

					lastIndex1 = str.indexOf(findStr, lastIndex1);
					lastIndex2 = str.indexOf(".mp3", lastIndex1);

					if (lastIndex1 != -1) {
						for (int i = lastIndex1 + findStr.length() + 1; i < lastIndex2 + 4; i++) {
							test += str.charAt(i);
						}
						fileNames.add(test);
						test = "";
						lastIndex1 += findStr.length();
					}
				}
				int f = 0;
				for (int i = 1; i < fileNames.size(); i++) {
					String temp = fileNames.get(i).toString();
					musicPathList[f] = serverMusicURL + UI.musicFolder + "/" + temp;
					f++;
					count += 1;
				}
				maxTrackCount = count;
			}
		} else {
			System.out.println(defaultMusicPath);
			try (Stream<Path> paths = Files.walk(Paths.get(defaultMusicPath))) {
				paths.forEach(filePath -> {
					if (Files.isRegularFile(filePath)) {
						if (filePath.toString().contains(".mp3")) {
							System.out.println("Path: " + filePath);
							String tempPath = filePath.toString();
							musicPathList[currentTrackID] = tempPath;
							currentTrackID++;
						}
					}
				});
			} catch (IOException e) {
				e.printStackTrace();
			}
			maxTrackCount = currentTrackID;
		}

		// Display Song List in ListView
		UI.items.clear();
		for (int i = 0; i < musicPathList.length; i++) {
			if (musicPathList[i] != "") {
				UI.items.add(i, musicPathList[i]);
			}
		}

		currentTrackID = 0;

		// Prints out the name of every music file in the folder
		System.out.println("Found the following files:");
		for (int i = 0; i < maxTrackCount; i++) {
			System.out.println(musicPathList[i]);
		}
		System.out.println("");

	}

	public static void next() {
		// Switches to the next music file
		System.out.println("");
		System.out.println("Changing to next music file...");

		fading = false;

		// Checks, whether a file is currently playing and stops it if true
		if (musicIsPlaying == true) {
			mediaPlayer.stop();
		}

		// Creates a randomly generated playlist if the current file is the last
		// one in the playlist
		if (UI.randomTrack == true) {
			if (maxTrackCount != 0) {
				if (currentTrackID == maxTrackCount - 1) {
					System.out.println("Generating new random music playlist...");
					for (int i = 0; i < 500; i++) {
						String shuffle1 = "";
						String shuffle2 = "";
						String shuffle3 = "";

						int ID1 = randomTrackID.nextInt(maxTrackCount);
						shuffle1 = musicPathList[ID1];

						int ID2 = randomTrackID.nextInt(maxTrackCount);
						shuffle2 = musicPathList[ID2];

						shuffle3 = shuffle2;
						shuffle2 = shuffle1;
						shuffle1 = shuffle3;

						musicPathList[ID1] = shuffle1;
						musicPathList[ID2] = shuffle2;

						currentTrackID = 0;
					}
					System.out.println("Random music playlist generated:");
					if (UI.debug) {
						for (int i = 0; i < maxTrackCount; i++) {
							System.out.println(musicPathList[i]);
						}
					}

					UI.items.clear();
					for (int i = 0; i < musicPathList.length; i++) {
						if (musicPathList[i] != "") {
							UI.items.add(i, musicPathList[i]);
						}
					}

					System.out.println("");
				} else {
					currentTrackID++;
				}
			}

		} else {
			if (currentTrackID < maxTrackCount - 1) {
				currentTrackID++;
			} else {
				currentTrackID = 0;
			}
		}

		play();
	}

	public static void fade() {
		Timeline timeline = new Timeline(
				new KeyFrame(Duration.seconds(UI.fadeDuration), new KeyValue(Music.mediaPlayer.volumeProperty(), 0)));
		timeline.play();
	}

}
