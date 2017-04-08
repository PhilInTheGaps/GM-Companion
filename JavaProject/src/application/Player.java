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

import org.jaudiotagger.audio.AudioFile;
import org.jaudiotagger.audio.AudioFileIO;
import org.jaudiotagger.audio.exceptions.CannotReadException;
import org.jaudiotagger.audio.exceptions.InvalidAudioFrameException;
import org.jaudiotagger.audio.exceptions.ReadOnlyFileException;
import org.jaudiotagger.tag.FieldKey;
import org.jaudiotagger.tag.Tag;
import org.jaudiotagger.tag.TagException;
import org.jsoup.Jsoup;
import org.jsoup.nodes.Document;

import javafx.animation.KeyFrame;
import javafx.animation.KeyValue;
import javafx.animation.Timeline;
import javafx.collections.MapChangeListener;
import javafx.scene.image.Image;
import javafx.scene.media.Media;
import javafx.scene.media.MediaPlayer;
import javafx.scene.media.MediaPlayer.Status;
import javafx.util.Duration;

public class Player {
	public static MediaPlayer soundPlayer;
	public static MediaPlayer mediaPlayer;

	public static Media sMedia;
	public static Media mMedia;

	public static String soundPath;
	public static String musicPath;

	public static Boolean soundIsPlaying = false;
	public static Boolean musicIsPlaying = false;

	public static Boolean soundFolderSelected = false;
	public static Boolean musicFolderSelected = false;

	public static Random randomSoundID = new Random();
	public static Random randomTrackID = new Random();

	public static int currentSoundID = 0;
	public static int currentTrackID = 0;

	public static Boolean soundsInitialPress = true;
	public static Boolean initialPress = false;

	public static ArrayList<String> soundFiles = new ArrayList<String>();
	public static ArrayList<String> musicFiles = new ArrayList<String>();

	public static double soundVolume = 0.25;
	public static double musicVolume = 0.5;

	public static String soundDirectory = "";
	public static String musicDirectory = "";

	public static String defaultSoundPath = "";
	public static String defaultMusicPath = "";

	public static String serverSoundsURL = UI.serverURL + "sounds/";
	public static String serverMusicURL = UI.serverURL + "music/";

	public static Boolean fading = false;

	public static void play(String type, Boolean loop) {
		// Plays the file
		System.out.println("Converting File Path...");

		if (type.equals("Music")) {
			// Gets the path to the current music file and converts it, so the
			// MediaPlayer can read it

			musicPath = musicFiles.get(currentTrackID);
			musicPath = musicPath.replace("\\", "/");

			// Select Song in ListView
			UI.lv.getSelectionModel().select(currentTrackID);

			if (UI.onlineMode) {
				mMedia = new Media(musicPath);

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
				currentTrackID = musicFiles.size() - 1;
				next("Music", true);
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
						next("Music", true);
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

		} else {
			soundPath = soundFiles.get(currentSoundID);
			soundPath = soundPath.replace("\\", "/");

			if (UI.onlineMode) {
				sMedia = new Media(soundPath);
			} else {
				sMedia = new Media(new File(soundPath).toURI().toString());
			}

			System.out.println("Creating Sound MediaPlayer...");
			if (!initialPress) {
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
				currentSoundID = soundFiles.size() - 1;
				next("Sounds", loop);
			}
			
			// Switches to next sound if current one ends
			if (loop){
				soundPlayer.setOnEndOfMedia(new Runnable() {
					@Override
					public void run() {
						if (UI.singleTrack == false) {
							next("Sounds", loop);
						}
					}
				});
			}
		}
	}

	public static Boolean Initial() {
		return soundsInitialPress;
	}

	public static void get(String type, Boolean add) throws IOException {
		// This finds every sound file in the folder and writes them into a list
		System.out.println("Finding sound files in folder: /" + defaultSoundPath);

		if (type.equals("Music")) {
			// This finds every music file in the folder and writes them into a
			// list
			currentTrackID = 0;
			
			if (!add){
				musicFiles.clear();
			}

			if (UI.onlineMode) {

				// Get all music files from server
				Document doc = Jsoup.connect(serverMusicURL + defaultMusicPath).get();

				System.out.println(doc.toString());
				String str = doc.toString();
				String findStr = "href=";
				int lastIndex1 = 0;
				int lastIndex2 = 10;
				ArrayList<String> fileNames = new ArrayList<String>();
				String test = new String();

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
				for (String name : fileNames) {
					musicFiles.add(serverMusicURL + UI.musicFolder + "/" + name);
				}
				musicFiles.remove(0);

			} else {
				System.out.println(defaultMusicPath);
				try (Stream<Path> paths = Files.walk(Paths.get(defaultMusicPath))) {
					paths.forEach(filePath -> {
						if (Files.isRegularFile(filePath)) {
							if (filePath.toString().contains(".mp3") || filePath.toString().contains(".wav")) {
								String tempPath = filePath.toString();
								musicFiles.add(tempPath);
								currentTrackID++;
							}
						}
					});
				} catch (IOException e) {
					e.printStackTrace();
				}
			}

			// Display Song List in ListView
			UI.items.clear();
			for (int i = 0; i < musicFiles.size(); i++) {
				String temp = musicFiles.get(i);
				try {
					AudioFile f = AudioFileIO.read(new File(musicFiles.get(i)));
					Tag tag = f.getTag();
					String title = tag.getFirst(FieldKey.TITLE);
					UI.items.add(i, title);
				} catch (CannotReadException e) {
					e.printStackTrace();
					UI.items.add(i, temp.substring(temp.lastIndexOf("\\") + 1));
				} catch (IOException e) {
					e.printStackTrace();
					UI.items.add(i, temp.substring(temp.lastIndexOf("\\") + 1));
				} catch (TagException e) {
					e.printStackTrace();
					UI.items.add(i, temp.substring(temp.lastIndexOf("\\") + 1));
				} catch (ReadOnlyFileException e) {
					e.printStackTrace();
					UI.items.add(i, temp.substring(temp.lastIndexOf("\\") + 1));
				} catch (InvalidAudioFrameException e) {
					e.printStackTrace();
					UI.items.add(i, temp.substring(temp.lastIndexOf("\\") + 1));
				}
			}

			currentTrackID = 0;

			// Prints out the name of every music file in the folder
			System.out.println("Found the following files:");
			for (String file : musicFiles) {
				System.out.println(file);
			}
			System.out.println("");
		} else {
			soundFiles.clear();
			currentSoundID = 0;

			if (UI.onlineMode) {
				// Get all sound files from server
				Document doc = Jsoup.connect(UI.serverURL + "sounds/" + UI.soundFolder).get();
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
					soundFiles.add(UI.serverURL + "sounds/" + UI.soundFolder + "/" + name);
				}
				soundFiles.remove(0);

			} else {
				try (Stream<Path> paths = Files.walk(Paths.get(defaultSoundPath))) {
					paths.forEach(filePath -> {
						if (Files.isRegularFile(filePath)) {
							if (filePath.toString().contains(".mp3") || filePath.toString().contains(".wav")) {
								String tempPath = filePath.toString();
								soundFiles.add(tempPath);
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
			for (String file : soundFiles) {
				System.out.println(file);
			}
			System.out.println("");
		}
	}

	public static void next(String type, Boolean loop) {

		if (type.equals("Music")) {
			// Switches to the next music file
			System.out.println("");
			System.out.println("Changing to next music file...");

			fading = false;

			// Checks, whether a file is currently playing and stops it if true
			if (musicIsPlaying == true) {
				mediaPlayer.stop();
			}

			// Creates a randomly generated playlist if the current file is the
			// last
			// one in the playlist

			if (currentTrackID >= musicFiles.size() - 1) {
				if (UI.randomTrack == true) {
					if (musicFiles.size() != 0) {
						System.out.println("Generating new random music playlist...");
						Collections.shuffle(musicFiles);
						System.out.println("Random music playlist generated!");
						UI.items.clear();

						for (int i = 0; i < musicFiles.size(); i++) {
							String temp = musicFiles.get(i);
							try {
								AudioFile f = AudioFileIO.read(new File(musicFiles.get(i)));
								Tag tag = f.getTag();
								String title = tag.getFirst(FieldKey.TITLE);
								UI.items.add(i, title);
							} catch (CannotReadException e) {
								e.printStackTrace();
								UI.items.add(i, temp.substring(temp.lastIndexOf("\\") + 1));
							} catch (IOException e) {
								e.printStackTrace();
								UI.items.add(i, temp.substring(temp.lastIndexOf("\\") + 1));
							} catch (TagException e) {
								e.printStackTrace();
								UI.items.add(i, temp.substring(temp.lastIndexOf("\\") + 1));
							} catch (ReadOnlyFileException e) {
								e.printStackTrace();
								UI.items.add(i, temp.substring(temp.lastIndexOf("\\") + 1));
							} catch (InvalidAudioFrameException e) {
								e.printStackTrace();
								UI.items.add(i, temp.substring(temp.lastIndexOf("\\") + 1));
							}
						}
						System.out.println("");
					}
					currentTrackID = 0;
				} else {
					currentTrackID = 0;
				}
			} else {
				currentTrackID++;
			}

			play("Music", true);
		} else {
			// Switches to next sound
			System.out.println("");
			System.out.println("Changing to next sound file...");

			if (soundPlayer.getStatus() == Status.PLAYING) {
				soundPlayer.stop();
			}

			if (currentSoundID >= soundFiles.size() - 1) {
				if (UI.randomTrack == true) {
					if (soundFiles.size() != 0) {
						System.out.println("Generating new random sound playlist...");
						Collections.shuffle(soundFiles);
						System.out.println("Random sound playlist generated!");
					}
					currentSoundID = 0;
				} else {
					currentSoundID = 0;
				}

			} else {
				currentSoundID++;
			}

			play("Sounds", loop);
		}
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

	public static String addTitleToList(String key, Object value, String file) {
		String title = "";
		switch (key) {
		case ("album"):
			break;
		case ("artist"):
			break;
		case ("title"):
			System.out.println("Title: " + value.toString());
			title = value.toString();
			return null;
		case ("year"):
			break;
		}
		return title;
	}

	public static void fade() {
		Timeline timeline = new Timeline(
				new KeyFrame(Duration.seconds(UI.fadeDuration), new KeyValue(mediaPlayer.volumeProperty(), 0)));
		timeline.play();
	}
}
