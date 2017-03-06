package application;

import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Random;
import java.util.stream.Stream;

import org.jsoup.Jsoup;
import org.jsoup.nodes.Document;

import javafx.scene.media.Media;
import javafx.scene.media.MediaPlayer;
import javafx.scene.media.MediaPlayer.Status;

public class Sound {

	public static MediaPlayer soundPlayer;
	public static Media sMedia;
	public static String soundPath;
	public static String currentSound;
	public static Boolean soundIsPlaying = false;
	public static Boolean soundFolderSelected = false;
	public static String soundError;
	public static Random randomSoundID = new Random();
	public static int currentSoundID;
	public static Boolean initialPress = true;
	public static ArrayList<String> files = new ArrayList<String>();

	public static double soundVolume = 0.25;

	public static String soundDirectory = "";
	public static String defaultSoundPath = "";
	public static String serverSoundsURL = "";

	public static void play() {
		// Plays the sound file

		System.out.println("Converting File Path...");
		soundPath = files.get(currentSoundID);
		soundPath = soundPath.replace("\\", "/");

		if (UI.onlineMode) {
			sMedia = new Media(soundPath);
		} else {
			sMedia = new Media(new File(soundPath).toURI().toString());
		}

		System.out.println("Creating Sound MediaPlayer...");
		if(!initialPress){
			soundPlayer.stop();
		}
		soundPlayer = new MediaPlayer(sMedia);
		soundPlayer.setAutoPlay(true);
		soundPlayer.setVolume(soundVolume);
		soundIsPlaying = true;

		// If the sound folder button was pressed, generate a new random
		// playlist
		if (initialPress) {
			initialPress = false;
			currentSoundID = files.size()-1;
			next();
		}

		// Switches to next sound if current one ends
		soundPlayer.setOnEndOfMedia(new Runnable() {
			@Override
			public void run() {
				if (UI.singleTrack == false) {
					next();
				}
			}
		});
	}
	
	public static Boolean Initial(){
		return initialPress;
	}
	
	public static void get() throws IOException {
		// This finds every sound file in the folder and writes them into a list
		System.out.println("Finding sound files in folder: /" + defaultSoundPath);
		
		files.clear();
		currentSoundID = 0;

		if (UI.onlineMode) {
			// Get all sound files from server
			Document doc = Jsoup.connect(serverSoundsURL + defaultSoundPath).get();
			// System.out.println(doc.toString());
			String str = doc.toString();
			String findStr = "href=";
			int lastIndex1 = 0;
			int lastIndex2 = 10;
			ArrayList<String> fileNames = new ArrayList<String>();
			String temp = new String();

			while (lastIndex1 != -1) {

				lastIndex1 = str.indexOf(findStr, lastIndex1);
				lastIndex2 = str.indexOf(".mp3", lastIndex1);

				if (lastIndex1 != -1) {
					for (int i = lastIndex1 + findStr.length() + 1; i < lastIndex2 + 4; i++) {
						temp += str.charAt(i);
					}
					fileNames.add(temp);
					temp = "";
					lastIndex1 += findStr.length();
				}
			}
			for (String name : fileNames) {
				files.add(serverSoundsURL + UI.soundFolder + "/" + name);
			}

		} else {
			try (Stream<Path> paths = Files.walk(Paths.get(defaultSoundPath))) {
				paths.forEach(filePath -> {
					if (Files.isRegularFile(filePath)) {
						if (filePath.toString().contains(".mp3") || filePath.toString().contains(".wav")) {
							String tempPath = filePath.toString();
							files.add(tempPath);
							currentSoundID++;
						}
					}
				});
			} catch (IOException e) {
				e.printStackTrace();
			}
		}

		currentSoundID = 0;

		// Prints out the name of every sound file in the folder
		System.out.println("Found the following files:");
		for (String file : files) {
			System.out.println(file);
		}
		System.out.println("");
	}

	public static void next() {
		// Switches to next sound
		System.out.println("");
		System.out.println("Changing to next sound file...");

		if (soundPlayer.getStatus() == Status.PLAYING) {
			soundPlayer.stop();
		}
		
		
		
		if (UI.randomTrack == true) {
			if (files.size() != 0) {
				Collections.shuffle(files);
			}
		} else {
			if (currentSoundID < files.size() - 1) {
				currentSoundID++;
			} else {
				currentSoundID = 0;
			}
		}

		play();
	}

}
