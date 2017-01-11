package application;

import javafx.event.ActionEvent;
import javafx.geometry.Insets;
import javafx.scene.control.Button;
import javafx.scene.layout.TilePane;

public class MusicButtons {
	public static TilePane addMusicTilePane(){
  		TilePane tile = new TilePane();
  		tile.setPadding(new Insets(20, 20, 20, 20));
  		tile.setVgap(3);
  		tile.setHgap(5);
  		tile.setPrefColumns(2);
  		tile.setStyle("-fx-background-color: LightGrey");
  		tile.setPrefWidth(Main.defaultMusicAndSoundWidth);
  		tile.setPrefHeight(Main.defaultHeight);
  		
  		//Action
  		Button Action = new Button();
  		Action.setText("Action");
  		Action.setPrefWidth(Main.defaultFolderButtonWidth);
  		Action.setPrefHeight(Main.defaultFolderButtonHeight);
  		Action.setOnAction((ActionEvent e) -> {
  			if(Main.linux == true){
  				Main.defaultMusicPath = Main.defaultLinuxFolder + "Music/Action";
  			}
  			else{
  				Main.defaultMusicPath = "Music/Action"; 
  			}
  			
  			Main.musicFolder = "Action";
  			Main.musicFolderLabel.setText("Folder: " + Main.musicFolder);
  			Main.musicFolderSelected = true;
  			
  			if (Main.musicIsPlaying == true){
  				Main.mediaPlayer.stop();
  			}
  			getMusicFiles.get();
  			MPlayer.play();
  		});
  		
  		//Bedrückend
  		Button Bedrueckend = new Button();
  		Bedrueckend.setText("Bedrückend");
  		Bedrueckend.setPrefWidth(Main.defaultFolderButtonWidth);
  		Bedrueckend.setPrefHeight(Main.defaultFolderButtonHeight);
  		Bedrueckend.setOnAction((ActionEvent e) -> {
  			if(Main.linux == true){
  				Main.defaultMusicPath = Main.defaultLinuxFolder + "Music/Bedrueckend";
  			}
  			else{
  				Main.defaultMusicPath = "Music/Bedrueckend"; 
  			}
  			
  			Main.musicFolder = "Bedrueckend";
  			Main.musicFolderLabel.setText("Folder: " + Main.musicFolder);
  			Main.musicFolderSelected = true;
  			
  			if (Main.musicIsPlaying == true){
  				Main.mediaPlayer.stop();
  			}
  			getMusicFiles.get();
  			MPlayer.play();
  		});
  		
  		//Credits
  		Button Credits = new Button();
  		Credits.setText("Credits");
  		Credits.setPrefWidth(Main.defaultFolderButtonWidth);
  		Credits.setPrefHeight(Main.defaultFolderButtonHeight);
  		Credits.setOnAction((ActionEvent e) -> {
  			if(Main.linux == true){
  				Main.defaultMusicPath = Main.defaultLinuxFolder + "Music/Credits";
  			}
  			else{
  				Main.defaultMusicPath = "Music/Credits"; 
  			}
  			
  			Main.musicFolder = "Credits";
  			Main.musicFolderLabel.setText("Folder: " + Main.musicFolder);
  			Main.musicFolderSelected = true;
  			
  			if (Main.musicIsPlaying == true){
  				Main.mediaPlayer.stop();
  			}
  			getMusicFiles.get();
  			MPlayer.play();
  		});
  		
  		//Dramatisch
  		Button Dramatisch = new Button();
  		Dramatisch.setText("Dramatisch");
  		Dramatisch.setPrefWidth(Main.defaultFolderButtonWidth);
  		Dramatisch.setPrefHeight(Main.defaultFolderButtonHeight);
  		Dramatisch.setOnAction((ActionEvent e) -> {
  			if(Main.linux == true){
  				Main.defaultMusicPath = Main.defaultLinuxFolder + "Music/Dramatisch";
  			}
  			else{
  				Main.defaultMusicPath = "Music/Dramatisch"; 
  			}
  			
  			Main.musicFolder = "Dramatisch";
  			Main.musicFolderLabel.setText("Folder: " + Main.musicFolder);
  			Main.musicFolderSelected = true;
  			
  			if (Main.musicIsPlaying == true){
  				Main.mediaPlayer.stop();
  			}
  			getMusicFiles.get();
  			MPlayer.play();
  		});
  		
  		//DUNGEONS AND SWAGONS
  		Button DnS = new Button();
  		DnS.setText("Dungeons and Swagons");
  		DnS.setPrefWidth(Main.defaultFolderButtonWidth);
  		DnS.setPrefHeight(Main.defaultFolderButtonHeight);
  		DnS.setOnAction((ActionEvent e) -> {
  			if(Main.linux == true){
  				Main.defaultMusicPath = Main.defaultLinuxFolder + "Music/DnS";
  			}
  			else{
  				Main.defaultMusicPath = "Music/DnS"; 
  			}
  			
  			Main.musicFolder = "DnS";
  			Main.musicFolderLabel.setText("Folder: " + Main.musicFolder);
  			Main.musicFolderSelected = true;
  			
  			if (Main.musicIsPlaying == true){
  				Main.mediaPlayer.stop();
  			}
  			getMusicFiles.get();
  			MPlayer.play();
  		});
  		
  		//Entspannt
  		Button Entspannt = new Button();
  		Entspannt.setText("Entspannt");
  		Entspannt.setPrefWidth(Main.defaultFolderButtonWidth);
  		Entspannt.setPrefHeight(Main.defaultFolderButtonHeight);
  		Entspannt.setOnAction((ActionEvent e) -> {
  			if(Main.linux == true){
  				Main.defaultMusicPath = Main.defaultLinuxFolder + "Music/Entspannt";
  			}
  			else{
  				Main.defaultMusicPath = "Music/Entspannt"; 
  			}
  			
  			Main.musicFolder = "Entspannt";
  			Main.musicFolderLabel.setText("Folder: " + Main.musicFolder);
  			Main.musicFolderSelected = true;
  			
  			if (Main.musicIsPlaying == true){
  				Main.mediaPlayer.stop();
  			}
  			getMusicFiles.get();
  			MPlayer.play();
  		});
  		
  		//Heroisch / Episch
  		Button HE = new Button();
  		HE.setText("Heroisch / Episch");
  		HE.setPrefWidth(Main.defaultFolderButtonWidth);
  		HE.setPrefHeight(Main.defaultFolderButtonHeight);
  		HE.setOnAction((ActionEvent e) -> {
  			if(Main.linux == true){
  				Main.defaultMusicPath = Main.defaultLinuxFolder + "Music/HeroischEpisch";
  			}
  			else{
  				Main.defaultMusicPath = "Music/HeroischEpisch"; 
  			}
  			
  			Main.musicFolder = "HeroischEpisch";
  			Main.musicFolderLabel.setText("Folder: " + Main.musicFolder);
  			Main.musicFolderSelected = true;
  			
  			if (Main.musicIsPlaying == true){
  				Main.mediaPlayer.stop();
  			}
  			getMusicFiles.get();
  			MPlayer.play();
  		});
  		
  		//Insel
  		Button Insel = new Button();
  		Insel.setText("Insel");
  		Insel.setPrefWidth(Main.defaultFolderButtonWidth);
  		Insel.setPrefHeight(Main.defaultFolderButtonHeight);
  		Insel.setOnAction((ActionEvent e) -> {
  			if(Main.linux == true){
  				Main.defaultMusicPath = Main.defaultLinuxFolder + "Music/Insel";
  			}
  			else{
  				Main.defaultMusicPath = "Music/Insel"; 
  			}
  			
  			Main.musicFolder = "Insel";
  			Main.musicFolderLabel.setText("Folder: " + Main.musicFolder);
  			Main.musicFolderSelected = true;
  			
  			if (Main.musicIsPlaying == true){
  				Main.mediaPlayer.stop();
  			}
  			getMusicFiles.get();
  			MPlayer.play();
  		});
  		
  		//JazzLounge
  		Button JazzLounge = new Button();
  		JazzLounge.setText("Jazz Lounge");
  		JazzLounge.setPrefWidth(Main.defaultFolderButtonWidth);
  		JazzLounge.setPrefHeight(Main.defaultFolderButtonHeight);
  		JazzLounge.setOnAction((ActionEvent e) -> {
  			if(Main.linux == true){
  				Main.defaultMusicPath = Main.defaultLinuxFolder + "Music/JazzLounge";
  			}
  			else{
  				Main.defaultMusicPath = "Music/JazzLounge"; 
  			}
  			
  			Main.musicFolder = "JazzLounge";
  			Main.musicFolderLabel.setText("Folder: " + Main.musicFolder);
  			Main.musicFolderSelected = true;
  			
  			if (Main.musicIsPlaying == true){
  				Main.mediaPlayer.stop();
  			}
  			getMusicFiles.get();
  			MPlayer.play();
  		});
  		
  		//Stadt
  		Button Stadt = new Button();
  		Stadt.setText("Stadt");
  		Stadt.setPrefWidth(Main.defaultFolderButtonWidth);
  		Stadt.setPrefHeight(Main.defaultFolderButtonHeight);
  		Stadt.setOnAction((ActionEvent e) -> {
  			if(Main.linux == true){
  				Main.defaultMusicPath = Main.defaultLinuxFolder + "Music/Stadt";
  			}
  			else{
  				Main.defaultMusicPath = "Music/Stadt"; 
  			}
  			
  			Main.musicFolder = "Stadt";
  			Main.musicFolderLabel.setText("Folder: " + Main.musicFolder);
  			Main.musicFolderSelected = true;
  			
  			if (Main.musicIsPlaying == true){
  				Main.mediaPlayer.stop();
  			}
  			getMusicFiles.get();
  			MPlayer.play();
  		});
  		
  		//PIRATEN
  		Button PIRATEN = new Button();
  		PIRATEN.setText("PIRATEN");
  		PIRATEN.setPrefWidth(Main.defaultFolderButtonWidth);
  		PIRATEN.setPrefHeight(Main.defaultFolderButtonHeight);
  		PIRATEN.setOnAction((ActionEvent e) -> {
  			if(Main.linux == true){
  				Main.defaultMusicPath = Main.defaultLinuxFolder + "Music/PIRATEN";
  			}
  			else{
  				Main.defaultMusicPath = "Music/PIRATEN"; 
  			}
  			
  			Main.musicFolder = "PIRATEN";
  			Main.musicFolderLabel.setText("Folder: " + Main.musicFolder);
  			Main.musicFolderSelected = true;
  			
  			if (Main.musicIsPlaying == true){
  				Main.mediaPlayer.stop();
  			}
  			getMusicFiles.get();
  			MPlayer.play();
  		});
  		
  		//HE IS A PIRATE!
  		Button pirate = new Button();
  		pirate.setText("He's a Pirate!");
  		pirate.setPrefWidth(Main.defaultFolderButtonWidth);
  		pirate.setPrefHeight(Main.defaultFolderButtonHeight);
  		pirate.setOnAction((ActionEvent e) -> {
  			if(Main.linux == true){
  				Main.defaultMusicPath = Main.defaultLinuxFolder + "Music/Pirate";
  			}
  			else{
  				Main.defaultMusicPath = "Music/Pirate"; 
  			}
  			
  			Main.musicFolder = "Pirate";
  			Main.musicFolderLabel.setText("Folder: " + Main.musicFolder);
  			Main.musicFolderSelected = true;
  			
  			if (Main.musicIsPlaying == true){
  				Main.mediaPlayer.stop();
  			}
  			getMusicFiles.get();
  			MPlayer.play();
  		});
  		
  		//Random Opener
  		Button RandomOpener = new Button();
  		RandomOpener.setText("Opener");
  		RandomOpener.setPrefWidth(Main.defaultFolderButtonWidth);
  		RandomOpener.setPrefHeight(Main.defaultFolderButtonHeight);
  		RandomOpener.setOnAction((ActionEvent e) -> {
  			if(Main.linux == true){
  				Main.defaultMusicPath = Main.defaultLinuxFolder + "Music/RandomOpener";
  			}
  			else{
  				Main.defaultMusicPath = "Music/RandomOpener"; 
  			}
  			
  			Main.musicFolder = "RandomOpener";
  			Main.musicFolderLabel.setText("Folder: " + Main.musicFolder);
  			Main.musicFolderSelected = true;
  			
  			if (Main.musicIsPlaying == true){
  				Main.mediaPlayer.stop();
  			}
  			getMusicFiles.get();
  			MPlayer.play();
  		});
  		
  		//Reise
  		Button Reise = new Button();
  		Reise.setText("Reise");
  		Reise.setPrefWidth(Main.defaultFolderButtonWidth);
  		Reise.setPrefHeight(Main.defaultFolderButtonHeight);
  		Reise.setOnAction((ActionEvent e) -> {
  			if(Main.linux == true){
  				Main.defaultMusicPath = Main.defaultLinuxFolder + "Music/Reise";
  			}
  			else{
  				Main.defaultMusicPath = "Music/Reise"; 
  			}
  			
  			Main.musicFolder = "Reise";
  			Main.musicFolderLabel.setText("Folder: " + Main.musicFolder);
  			Main.musicFolderSelected = true;
  			
  			if (Main.musicIsPlaying == true){
  				Main.mediaPlayer.stop();
  			}
  			getMusicFiles.get();
  			MPlayer.play();
  		});
  		
  		//SANDSTORM
  		Button SANDSTORM = new Button();
  		SANDSTORM.setText("SANDSTORM");
  		SANDSTORM.setPrefWidth(Main.defaultFolderButtonWidth);
  		SANDSTORM.setPrefHeight(Main.defaultFolderButtonHeight);
  		SANDSTORM.setOnAction((ActionEvent e) -> {
  			if(Main.linux == true){
  				Main.defaultMusicPath = Main.defaultLinuxFolder + "Music/SANDSTORM";
  			}
  			else{
  				Main.defaultMusicPath = "Music/SANDSTORM"; 
  			}
  			
  			Main.musicFolder = "SANDSTORM";
  			Main.musicFolderLabel.setText("Folder: " + Main.musicFolder);
  			Main.musicFolderSelected = true;
  			
  			if (Main.musicIsPlaying == true){
  				Main.mediaPlayer.stop();
  			}
  			getMusicFiles.get();
  			MPlayer.play();
  		});
  		
  		//SciFi Action
  		Button SciFiAction = new Button();
  		SciFiAction.setText("SciFi Action");
  		SciFiAction.setPrefWidth(Main.defaultFolderButtonWidth);
  		SciFiAction.setPrefHeight(Main.defaultFolderButtonHeight);
  		SciFiAction.setOnAction((ActionEvent e) -> {
  			if(Main.linux == true){
  				Main.defaultMusicPath = Main.defaultLinuxFolder + "Music/SciFiAction";
  			}
  			else{
  				Main.defaultMusicPath = "Music/SciFiAction"; 
  			}
  			
  			Main.musicFolder = "SciFiAction";
  			Main.musicFolderLabel.setText("Folder: " + Main.musicFolder);
  			Main.musicFolderSelected = true;
  			
  			if (Main.musicIsPlaying == true){
  				Main.mediaPlayer.stop();
  			}
  			getMusicFiles.get();
  			MPlayer.play();
  		});
  		
  		//SciFi Bedrückend
  		Button SciFiBedrueckend = new Button();
  		SciFiBedrueckend.setText("SciFi Bedrückend");
  		SciFiBedrueckend.setPrefWidth(Main.defaultFolderButtonWidth);
  		SciFiBedrueckend.setPrefHeight(Main.defaultFolderButtonHeight);
  		SciFiBedrueckend.setOnAction((ActionEvent e) -> {
  			if(Main.linux == true){
  				Main.defaultMusicPath = Main.defaultLinuxFolder + "Music/SciFiBedrueckend";
  			}
  			else{
  				Main.defaultMusicPath = "Music/SciFiBedrueckend"; 
  			}
  			
  			Main.musicFolder = "SciFiBedrueckend";
  			Main.musicFolderLabel.setText("Folder: " + Main.musicFolder);
  			Main.musicFolderSelected = true;
  			
  			if (Main.musicIsPlaying == true){
  				Main.mediaPlayer.stop();
  			}
  			getMusicFiles.get();
  			MPlayer.play();
  		});
  		
  		//SciFiDramatisch
  		Button SciFiDramatisch = new Button();
  		SciFiDramatisch.setText("SciFi Dramatisch");
  		SciFiDramatisch.setPrefWidth(Main.defaultFolderButtonWidth);
  		SciFiDramatisch.setPrefHeight(Main.defaultFolderButtonHeight);
  		SciFiDramatisch.setOnAction((ActionEvent e) -> {
  			if(Main.linux == true){
  				Main.defaultMusicPath = Main.defaultLinuxFolder + "Music/SciFiDramatisch";
  			}
  			else{
  				Main.defaultMusicPath = "Music/SciFiDramatisch"; 
  			}
  			
  			Main.musicFolder = "SciFiDramatisch";
  			Main.musicFolderLabel.setText("Folder: " + Main.musicFolder);
  			Main.musicFolderSelected = true;
  			
  			if (Main.musicIsPlaying == true){
  				Main.mediaPlayer.stop();
  			}
  			getMusicFiles.get();
  			MPlayer.play();
  		});
  		
  		//SciFiEntspannt
  		Button SciFiEntspannt = new Button();
  		SciFiEntspannt.setText("SciFi Entspannt");
  		SciFiEntspannt.setPrefWidth(Main.defaultFolderButtonWidth);
  		SciFiEntspannt.setPrefHeight(Main.defaultFolderButtonHeight);
  		SciFiEntspannt.setOnAction((ActionEvent e) -> {
  			if(Main.linux == true){
  				Main.defaultMusicPath = Main.defaultLinuxFolder + "Music/SciFiEntspannt";
  			}
  			else{
  				Main.defaultMusicPath = "Music/SciFiEntspannt"; 
  			}
  			
  			Main.musicFolder = "SciFiEntspannt";
  			Main.musicFolderLabel.setText("Folder: " + Main.musicFolder);
  			Main.musicFolderSelected = true;
  			
  			if (Main.musicIsPlaying == true){
  				Main.mediaPlayer.stop();
  			}
  			getMusicFiles.get();
  			MPlayer.play();
  		});
  		
  		//SciFiFroehlich
  		Button SciFiFroehlich = new Button();
  		SciFiFroehlich.setText("SciFi Fröhlich");
  		SciFiFroehlich.setPrefWidth(Main.defaultFolderButtonWidth);
  		SciFiFroehlich.setPrefHeight(Main.defaultFolderButtonHeight);
  		SciFiFroehlich.setOnAction((ActionEvent e) -> {
  			if(Main.linux == true){
  				Main.defaultMusicPath = Main.defaultLinuxFolder + "Music/SciFiFroehlich";
  			}
  			else{
  				Main.defaultMusicPath = "Music/SciFiFroehlich"; 
  			}
  			
  			Main.musicFolder = "SciFiFroehlich";
  			Main.musicFolderLabel.setText("Folder: " + Main.musicFolder);
  			Main.musicFolderSelected = true;
  			
  			if (Main.musicIsPlaying == true){
  				Main.mediaPlayer.stop();
  			}
  			getMusicFiles.get();
  			MPlayer.play();
  		});
  		
  		//SciFiLounge
  		Button SciFiLounge = new Button();
  		SciFiLounge.setText("SciFi Lounge");
  		SciFiLounge.setPrefWidth(Main.defaultFolderButtonWidth);
  		SciFiLounge.setPrefHeight(Main.defaultFolderButtonHeight);
  		SciFiLounge.setOnAction((ActionEvent e) -> {
  			if(Main.linux == true){
  				Main.defaultMusicPath = Main.defaultLinuxFolder + "Music/SciFiLounge";
  			}
  			else{
  				Main.defaultMusicPath = "Music/SciFiLounge"; 
  			}
  			
  			Main.musicFolder = "SciFiLounge";
  			Main.musicFolderLabel.setText("Folder: " + Main.musicFolder);
  			Main.musicFolderSelected = true;
  			
  			if (Main.musicIsPlaying == true){
  				Main.mediaPlayer.stop();
  			}
  			getMusicFiles.get();
  			MPlayer.play();
  		});
  		
  		//SciFiMarkt
  		Button SciFiMarkt = new Button();
  		SciFiMarkt.setText("SciFi Markt");
  		SciFiMarkt.setPrefWidth(Main.defaultFolderButtonWidth);
  		SciFiMarkt.setPrefHeight(Main.defaultFolderButtonHeight);
  		SciFiMarkt.setOnAction((ActionEvent e) -> {
  			if(Main.linux == true){
  				Main.defaultMusicPath = Main.defaultLinuxFolder + "Music/SciFiMarkt";
  			}
  			else{
  				Main.defaultMusicPath = "Music/SciFiMarkt"; 
  			}
  			
  			Main.musicFolder = "SciFiMarkt";
  			Main.musicFolderLabel.setText("Folder: " + Main.musicFolder);
  			Main.musicFolderSelected = true;
  			
  			if (Main.musicIsPlaying == true){
  				Main.mediaPlayer.stop();
  			}
  			getMusicFiles.get();
  			MPlayer.play();
  		});
  		
  		//SciFiReise
  		Button SciFiReise = new Button();
  		SciFiReise.setText("SciFi Reise");
  		SciFiReise.setPrefWidth(Main.defaultFolderButtonWidth);
  		SciFiReise.setPrefHeight(Main.defaultFolderButtonHeight);
  		SciFiReise.setOnAction((ActionEvent e) -> {
  			if(Main.linux == true){
  				Main.defaultMusicPath = Main.defaultLinuxFolder + "Music/SciFiReise";
  			}
  			else{
  				Main.defaultMusicPath = "Music/SciFiReise"; 
  			}
  			
  			Main.musicFolder = "SciFiReise";
  			Main.musicFolderLabel.setText("Folder: " + Main.musicFolder);
  			Main.musicFolderSelected = true;
  			
  			if (Main.musicIsPlaying == true){
  				Main.mediaPlayer.stop();
  			}
  			getMusicFiles.get();
  			MPlayer.play();
  		});
  		
  		//SciFiTraurig
  		Button SciFiTraurig = new Button();
  		SciFiTraurig.setText("SciFi Traurig");
  		SciFiTraurig.setPrefWidth(Main.defaultFolderButtonWidth);
  		SciFiTraurig.setPrefHeight(Main.defaultFolderButtonHeight);
  		SciFiTraurig.setOnAction((ActionEvent e) -> {
  			if(Main.linux == true){
  				Main.defaultMusicPath = Main.defaultLinuxFolder + "Music/SciFiTraurig";
  			}
  			else{
  				Main.defaultMusicPath = "Music/SciFiTraurig"; 
  			}
  			
  			Main.musicFolder = "SciFiTraurig";
  			Main.musicFolderLabel.setText("Folder: " + Main.musicFolder);
  			Main.musicFolderSelected = true;
  			
  			if (Main.musicIsPlaying == true){
  				Main.mediaPlayer.stop();
  			}
  			getMusicFiles.get();
  			MPlayer.play();
  		});
  		
  		//Taverne
  		Button Taverne = new Button();
  		Taverne.setText("Taverne");
  		Taverne.setPrefWidth(Main.defaultFolderButtonWidth);
  		Taverne.setPrefHeight(Main.defaultFolderButtonHeight);
  		Taverne.setOnAction((ActionEvent e) -> {
  			if(Main.linux == true){
  				Main.defaultMusicPath = Main.defaultLinuxFolder + "Music/Taverne";
  			}
  			else{
  				Main.defaultMusicPath = "Music/Taverne"; 
  			}
  			
  			Main.musicFolder = "Taverne";
  			Main.musicFolderLabel.setText("Folder: " + Main.musicFolder);
  			Main.musicFolderSelected = true;
  			
  			if (Main.musicIsPlaying == true){
  				Main.mediaPlayer.stop();
  			}
  			getMusicFiles.get();
  			MPlayer.play();
  		});
  		
  		//TheBearAndTheMaidenFair
  		Button TheBearAndTheMaidenFair = new Button();
  		TheBearAndTheMaidenFair.setText("The Bear And The Maiden Fair");
  		TheBearAndTheMaidenFair.setPrefWidth(Main.defaultFolderButtonWidth);
  		TheBearAndTheMaidenFair.setPrefHeight(Main.defaultFolderButtonHeight);
  		TheBearAndTheMaidenFair.setOnAction((ActionEvent e) -> {
  			if(Main.linux == true){
  				Main.defaultMusicPath = Main.defaultLinuxFolder + "Music/TheBearAndTheMaidenFair";
  			}
  			else{
  				Main.defaultMusicPath = "Music/TheBearAndTheMaidenFair"; 
  			}
  			
  			Main.musicFolder = "TheBearAndTheMaidenFair";
  			Main.musicFolderLabel.setText("Folder: " + Main.musicFolder);
  			Main.musicFolderSelected = true;
  			
  			if (Main.musicIsPlaying == true){
  				Main.mediaPlayer.stop();
  			}
  			getMusicFiles.get();
  			MPlayer.play();
  		});
  		
  		//TheRainsOfCastamere
  		Button TheRainsOfCastamere = new Button();
  		TheRainsOfCastamere.setText("The Rains Of Castamere");
  		TheRainsOfCastamere.setPrefWidth(Main.defaultFolderButtonWidth);
  		TheRainsOfCastamere.setPrefHeight(Main.defaultFolderButtonHeight);
  		TheRainsOfCastamere.setOnAction((ActionEvent e) -> {
  			if(Main.linux == true){
  				Main.defaultMusicPath = Main.defaultLinuxFolder + "Music/TheRainsOfCastamere";
  			}
  			else{
  				Main.defaultMusicPath = "Music/TheRainsOfCastamere"; 
  			}
  			
  			Main.musicFolder = "TheRainsOfCastamere";
  			Main.musicFolderLabel.setText("Folder: " + Main.musicFolder);
  			Main.musicFolderSelected = true;
  			
  			if (Main.musicIsPlaying == true){
  				Main.mediaPlayer.stop();
  			}
  			getMusicFiles.get();
  			MPlayer.play();
  		});
  		
  		//Piraten Songs
  		Button piratenSongs = new Button();
  		piratenSongs.setText("Piraten Songs");
  		piratenSongs.setPrefWidth(Main.defaultFolderButtonWidth);
  		piratenSongs.setPrefHeight(Main.defaultFolderButtonHeight);
  		piratenSongs.setOnAction((ActionEvent e) -> {
  			if(Main.linux == true){
  				Main.defaultMusicPath = Main.defaultLinuxFolder + "Music/Piratensongs";
  			}
  			else{
  				Main.defaultMusicPath = "Music/Piratensongs"; 
  			}
  			
  			Main.musicFolder = "Piratensongs";
  			Main.musicFolderLabel.setText("Folder: " + Main.musicFolder);
  			Main.musicFolderSelected = true;
  			
  			if (Main.musicIsPlaying == true){
  				Main.mediaPlayer.stop();
  			}
  			getMusicFiles.get();
  			MPlayer.play();
  		});
  		
  		//Add every Button
  		tile.getChildren().addAll(Action, Dramatisch, Bedrueckend, Taverne, Entspannt, HE, Insel, Stadt, Reise, 
  								  SciFiAction, SciFiDramatisch, SciFiBedrueckend, SciFiReise, SciFiEntspannt, SciFiFroehlich, 
  								  SciFiTraurig, SciFiMarkt, SciFiLounge, JazzLounge, Credits, RandomOpener, DnS, 
  								  PIRATEN, pirate, piratenSongs /*, SANDSTORM, 
  								  TheBearAndTheMaidenFair, TheRainsOfCastamere*/);
  		
  		/*
  		Action 		Dramatisch 		Bedrueckend
  		Taverne		Entspannt		HE
  		Insel		Stadt			Reise
  		PIRATEN
  		SciFiAction	SciFiDramatisch	SciFiBedrueckend
  		SciFiReise	SciFiEntspannt	SciFiFroehlich
  		SciFiTraurigSciFiMarkt		SciFiLounge	
  		JazzLounge		
  		Credits		DnS				RandomOpener
  		SANDSTORM	TheBearAndTheMaidenFair		TheRainsOfCastamere
  		*/
  		
		return tile;
  	}
}
