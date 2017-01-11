package application;

import javafx.event.ActionEvent;
import javafx.geometry.Insets;
import javafx.scene.control.Button;
import javafx.scene.layout.TilePane;

public class SoundButtons {
	public static TilePane addSoundTilePane(){
  		TilePane tile2 = new TilePane();
  		tile2.setPadding(new Insets(20, 20, 20, 20));
  		tile2.setVgap(3);
  		tile2.setHgap(5);
  		tile2.setPrefColumns(2);
  		tile2.setStyle("-fx-background-color: LightGrey");
  		tile2.setPrefWidth(Main.defaultMusicAndSoundWidth);
  		
  		//Fledermäuse
  		Button fmaus = new Button();
  		fmaus.setText("Fledermäuse");
  		fmaus.setPrefWidth(Main.defaultFolderButtonWidth);
  		fmaus.setPrefHeight(Main.defaultFolderButtonHeight);
  		fmaus.setOnAction((ActionEvent e) -> {
  			
  			if(Main.linux == true){
  				Main.defaultSoundPath = Main.defaultLinuxFolder + "Sounds/Fledermaeuse";
  			}
  			else{
  				Main.defaultSoundPath = "Sounds/Fledermaeuse"; 
  			}
  			
  			Main.soundFolder = "Fledermaeuse";
  			Main.soundFolderLabel.setText("Folder: " + Main.soundFolder);
  			Main.soundFolderSelected = true;
  			
  			if (Main.soundIsPlaying == true){
  				Main.soundPlayer.stop();
  			}
  			getSoundFiles.get();
  			SPlayer.play();
  		});
  		
  		//Grillenzirpen
  		Button zirp = new Button();
  		zirp.setText("Grillenzirpen");
  		zirp.setPrefWidth(Main.defaultFolderButtonWidth);
  		zirp.setPrefHeight(Main.defaultFolderButtonHeight);
  		zirp.setOnAction((ActionEvent e) -> {
  			
  			if(Main.linux == true){
  				Main.defaultSoundPath = Main.defaultLinuxFolder + "Sounds/GrillenZirpen";
  			}
  			else{
  				Main.defaultSoundPath = "Sounds/GrillenZirpen"; 
  			}
  			
  			Main.soundFolder = "GrillenZirpen";
  			Main.soundFolderLabel.setText("Folder: " + Main.soundFolder);
  			Main.soundFolderSelected = true;
  			
  			if (Main.soundIsPlaying == true){
  				Main.soundPlayer.stop();
  			}
  			getSoundFiles.get();
  			SPlayer.play();
  		});
  		
  		//Hafen
  		Button hafen = new Button();
  		hafen.setText("Hafen");
  		hafen.setPrefWidth(Main.defaultFolderButtonWidth);
  		hafen.setPrefHeight(Main.defaultFolderButtonHeight);
  		hafen.setOnAction((ActionEvent e) -> {
  			
  			if(Main.linux == true){
  				Main.defaultSoundPath = Main.defaultLinuxFolder + "Sounds/Hafen";
  			}
  			else{
  				Main.defaultSoundPath = "Sounds/Hafen"; 
  			}
  			
  			Main.soundFolder = "Hafen";
  			Main.soundFolderLabel.setText("Folder: " + Main.soundFolder);
  			Main.soundFolderSelected = true;
  			
  			if (Main.soundIsPlaying == true){
  				Main.soundPlayer.stop();
  			}
  			getSoundFiles.get();
  			SPlayer.play();
  		});
  		
  		//Höhle
  		Button cave = new Button();
  		cave.setText("Höhle");
  		cave.setPrefWidth(Main.defaultFolderButtonWidth);
  		cave.setPrefHeight(Main.defaultFolderButtonHeight);
  		cave.setOnAction((ActionEvent e) -> {
  			
  			if(Main.linux == true){
  				Main.defaultSoundPath = Main.defaultLinuxFolder + "Sounds/Cave";
  			}
  			else{
  				Main.defaultSoundPath = "Sounds/Cave"; 
  			}
  			
  			Main.soundFolder = "Cave";
  			Main.soundFolderLabel.setText("Folder: " + Main.soundFolder);
  			Main.soundFolderSelected = true;
  			
  			if (Main.soundIsPlaying == true){
  				Main.soundPlayer.stop();
  			}
  			getSoundFiles.get();
  			SPlayer.play();
  		});
  		
  		//Kanalisation
  		Button kanali = new Button();
  		kanali.setText("Kanalisation");
  		kanali.setPrefWidth(Main.defaultFolderButtonWidth);
  		kanali.setPrefHeight(Main.defaultFolderButtonHeight);
  		kanali.setOnAction((ActionEvent e) -> {
  			
  			if(Main.linux == true){
  				Main.defaultSoundPath = Main.defaultLinuxFolder + "Sounds/Kanalisation";
  			}
  			else{
  				Main.defaultSoundPath = "Sounds/Kanalisation"; 
  			}
  			
  			Main.soundFolder = "Kanalisation";
  			Main.soundFolderLabel.setText("Folder: " + Main.soundFolder);
  			Main.soundFolderSelected = true;
  			
  			if (Main.soundIsPlaying == true){
  				Main.soundPlayer.stop();
  			}
  			getSoundFiles.get();
  			SPlayer.play();
  		});
  		
  		//Katakomben
  		Button catac = new Button();
  		catac.setText("Katakomben");
  		catac.setPrefWidth(Main.defaultFolderButtonWidth);
  		catac.setPrefHeight(Main.defaultFolderButtonHeight);
  		catac.setOnAction((ActionEvent e) -> {
  			
  			if(Main.linux == true){
  				Main.defaultSoundPath = Main.defaultLinuxFolder + "Sounds/Katakomben";
  			}
  			else{
  				Main.defaultSoundPath = "Sounds/Katakomben"; 
  			}
  			
  			Main.soundFolder = "Katakomben";
  			Main.soundFolderLabel.setText("Folder: " + Main.soundFolder);
  			Main.soundFolderSelected = true;
  			
  			if (Main.soundIsPlaying == true){
  				Main.soundPlayer.stop();
  			}
  			getSoundFiles.get();
  			SPlayer.play();
  		});
  		
  		//Markt
  		Button markt = new Button();
  		markt.setText("Markt");
  		markt.setPrefWidth(Main.defaultFolderButtonWidth);
  		markt.setPrefHeight(Main.defaultFolderButtonHeight);
  		markt.setOnAction((ActionEvent e) -> {
  			
  			if(Main.linux == true){
  				Main.defaultSoundPath = Main.defaultLinuxFolder + "Sounds/Markt";
  			}
  			else{
  				Main.defaultSoundPath = "Sounds/Markt"; 
  			}
  			
  			Main.soundFolder = "Markt";
  			Main.soundFolderLabel.setText("Folder: " + Main.soundFolder);
  			Main.soundFolderSelected = true;
  			
  			if (Main.soundIsPlaying == true){
  				Main.soundPlayer.stop();
  			}
  			getSoundFiles.get();
  			SPlayer.play();
  		});
  		
  		//Nacht 1
  		Button nacht01 = new Button();
  		nacht01.setText("Nacht 1");
  		nacht01.setPrefWidth(Main.defaultFolderButtonWidth);
  		nacht01.setPrefHeight(Main.defaultFolderButtonHeight);
  		nacht01.setOnAction((ActionEvent e) -> {
  			
  			if(Main.linux == true){
  				Main.defaultSoundPath = Main.defaultLinuxFolder + "Sounds/Nacht";
  			}
  			else{
  				Main.defaultSoundPath = "Sounds/Nacht"; 
  			}
  			
  			Main.soundFolder = "Nacht";
  			Main.soundFolderLabel.setText("Folder: " + Main.soundFolder);
  			Main.soundFolderSelected = true;
  			
  			if (Main.soundIsPlaying == true){
  				Main.soundPlayer.stop();
  			}
  			getSoundFiles.get();
  			SPlayer.play();
  		});
  		
  		//Nacht 2
  		Button nacht02 = new Button();
  		nacht02.setText("Nacht 2");
  		nacht02.setPrefWidth(Main.defaultFolderButtonWidth);
  		nacht02.setPrefHeight(Main.defaultFolderButtonHeight);
  		nacht02.setOnAction((ActionEvent e) -> {
  			
  			if(Main.linux == true){
  				Main.defaultSoundPath = Main.defaultLinuxFolder + "Sounds/Nacht2";
  			}
  			else{
  				Main.defaultSoundPath = "Sounds/Nacht2"; 
  			}
  			
  			Main.soundFolder = "Nacht2";
  			Main.soundFolderLabel.setText("Folder: " + Main.soundFolder);
  			Main.soundFolderSelected = true;
  			
  			if (Main.soundIsPlaying == true){
  				Main.soundPlayer.stop();
  			}
  			getSoundFiles.get();
  			SPlayer.play();
  		});
  		
  		//Regen (Gewitter)
  		Button rain = new Button();
  		rain.setText("Regen (Gewitter)");
  		rain.setPrefWidth(Main.defaultFolderButtonWidth);
  		rain.setPrefHeight(Main.defaultFolderButtonHeight);
  		rain.setOnAction((ActionEvent e) -> {
  			
  			if(Main.linux == true){
  				Main.defaultSoundPath = Main.defaultLinuxFolder + "Sounds/RegenGewitter";
  			}
  			else{
  				Main.defaultSoundPath = "Sounds/RegenGewitter"; 
  			}
  			
  			Main.soundFolder = "RegenGewitter";
  			Main.soundFolderLabel.setText("Folder: " + Main.soundFolder);
  			Main.soundFolderSelected = true;
  			
  			if (Main.soundIsPlaying == true){
  				Main.soundPlayer.stop();
  			}
  			getSoundFiles.get();
  			SPlayer.play();
  		});
  		
  		//Schlacht
  		Button battle = new Button();
  		battle.setText("Schlacht");
  		battle.setPrefWidth(Main.defaultFolderButtonWidth);
  		battle.setPrefHeight(Main.defaultFolderButtonHeight);
  		battle.setOnAction((ActionEvent e) -> {
  			
  			if(Main.linux == true){
  				Main.defaultSoundPath = Main.defaultLinuxFolder + "Sounds/Schlacht";
  			}
  			else{
  				Main.defaultSoundPath = "Sounds/Schlacht"; 
  			}
  			
  			Main.soundFolder = "Schlacht";
  			Main.soundFolderLabel.setText("Folder: " + Main.soundFolder);
  			Main.soundFolderSelected = true;
  			
  			if (Main.soundIsPlaying == true){
  				Main.soundPlayer.stop();
  			}
  			getSoundFiles.get();
  			SPlayer.play();
  		});
  		
  		//Stadt
  		Button town = new Button();
  		town.setText("Stadt");
  		town.setPrefWidth(Main.defaultFolderButtonWidth);
  		town.setPrefHeight(Main.defaultFolderButtonHeight);
  		town.setOnAction((ActionEvent e) -> {
  			
  			if(Main.linux == true){
  				Main.defaultSoundPath = Main.defaultLinuxFolder + "Sounds/Stadt";
  			}
  			else{
  				Main.defaultSoundPath = "Sounds/Stadt"; 
  			}
  			
  			Main.soundFolder = "Stadt";
  			Main.soundFolderLabel.setText("Folder: " + Main.soundFolder);
  			Main.soundFolderSelected = true;
  			
  			if (Main.soundIsPlaying == true){
  				Main.soundPlayer.stop();
  			}
  			getSoundFiles.get();
  			SPlayer.play();
  		});
  		
  		//Tag (Reise)
  		Button day = new Button();
  		day.setText("Tag (Reise)");
  		day.setPrefWidth(Main.defaultFolderButtonWidth);
  		day.setPrefHeight(Main.defaultFolderButtonHeight);
  		day.setOnAction((ActionEvent e) -> {
  			
  			if(Main.linux == true){
  				Main.defaultSoundPath = Main.defaultLinuxFolder + "Sounds/TagReise";
  			}
  			else{
  				Main.defaultSoundPath = "Sounds/TagReise"; 
  			}
  			
  			Main.soundFolder = "TagReise";
  			Main.soundFolderLabel.setText("Folder: " + Main.soundFolder);
  			Main.soundFolderSelected = true;
  			
  			if (Main.soundIsPlaying == true){
  				Main.soundPlayer.stop();
  			}
  			getSoundFiles.get();
  			SPlayer.play();
  		});
  		
  		//Taverne
  		Button tavern = new Button();
  		tavern.setText("Taverne");
  		tavern.setPrefWidth(Main.defaultFolderButtonWidth);
  		tavern.setPrefHeight(Main.defaultFolderButtonHeight);
  		tavern.setOnAction((ActionEvent e) -> {
  			
  			if(Main.linux == true){
  				Main.defaultSoundPath = Main.defaultLinuxFolder + "Sounds/Taverne";
  			}
  			else{
  				Main.defaultSoundPath = "Sounds/Taverne"; 
  			}
  			
  			Main.soundFolder = "Taverne";
  			Main.soundFolderLabel.setText("Folder: " + Main.soundFolder);
  			Main.soundFolderSelected = true;
  			
  			if (Main.soundIsPlaying == true){
  				Main.soundPlayer.stop();
  			}
  			getSoundFiles.get();
  			SPlayer.play();
  		});
  		
  		//Taverne (Berg)
  		Button tavern2 = new Button();
  		tavern2.setText("Taverne (Berg)");
  		tavern2.setPrefWidth(Main.defaultFolderButtonWidth);
  		tavern2.setPrefHeight(Main.defaultFolderButtonHeight);
  		tavern2.setOnAction((ActionEvent e) -> {
  			
  			if(Main.linux == true){
  				Main.defaultSoundPath = Main.defaultLinuxFolder + "Sounds/TaverneBerg";
  			}
  			else{
  				Main.defaultSoundPath = "Sounds/TaverneBerg"; 
  			}
  			
  			Main.soundFolder = "TaverneBerg";
  			Main.soundFolderLabel.setText("Folder: " + Main.soundFolder);
  			Main.soundFolderSelected = true;
  			
  			if (Main.soundIsPlaying == true){
  				Main.soundPlayer.stop();
  			}
  			getSoundFiles.get();
  			SPlayer.play();
  		});
  		
  		//Vögel
  		Button bird = new Button();
  		bird.setText("Vögel");
  		bird.setPrefWidth(Main.defaultFolderButtonWidth);
  		bird.setPrefHeight(Main.defaultFolderButtonHeight);
  		bird.setOnAction((ActionEvent e) -> {
  			
  			if(Main.linux == true){
  				Main.defaultSoundPath = Main.defaultLinuxFolder + "Sounds/Voegel";
  			}
  			else{
  				Main.defaultSoundPath = "Sounds/Voegel"; 
  			}
  			
  			Main.soundFolder = "Voegel";
  			Main.soundFolderLabel.setText("Folder: " + Main.soundFolder);
  			Main.soundFolderSelected = true;
  			
  			if (Main.soundIsPlaying == true){
  				Main.soundPlayer.stop();
  			}
  			getSoundFiles.get();
  			SPlayer.play();
  		});
  		
  		//Warcraft III
  		Button warc = new Button();
  		warc.setText("NICHT DRÜCKEN!");
  		warc.setPrefWidth(Main.defaultFolderButtonWidth);
  		warc.setPrefHeight(Main.defaultFolderButtonHeight);
  		warc.setOnAction((ActionEvent e) -> {
  			
  			if(Main.linux == true){
  				Main.defaultSoundPath = Main.defaultLinuxFolder + "Sounds/Warcraft3";
  			}
  			else{
  				Main.defaultSoundPath = "Sounds/Warcraft3"; 
  			}
  			
  			Main.soundFolder = "Warcraft3";
  			Main.soundFolderLabel.setText("Folder: " + Main.soundFolder);
  			Main.soundFolderSelected = true;
  			
  			if (Main.soundIsPlaying == true){
  				Main.soundPlayer.stop();
  			}
  			getSoundFiles.get();
  			SPlayer.play();
  		});
  		
  		//Wasser (Bach)
  		Button water = new Button();
  		water.setText("Wasser (Bach)");
  		water.setPrefWidth(Main.defaultFolderButtonWidth);
  		water.setPrefHeight(Main.defaultFolderButtonHeight);
  		water.setOnAction((ActionEvent e) -> {
  			
  			if(Main.linux == true){
  				Main.defaultSoundPath = Main.defaultLinuxFolder + "Sounds/Wasser01";
  			}
  			else{
  				Main.defaultSoundPath = "Sounds/Wasser01"; 
  			}
  			
  			Main.soundFolder = "Wasser01";
  			Main.soundFolderLabel.setText("Folder: " + Main.soundFolder);
  			Main.soundFolderSelected = true;
  			
  			if (Main.soundIsPlaying == true){
  				Main.soundPlayer.stop();
  			}
  			getSoundFiles.get();
  			SPlayer.play();
  		});
  		
  		//Add everything
  		tile2.getChildren().addAll(day, nacht01, nacht02, bird, rain, water, zirp, fmaus, battle, 
  								   tavern, tavern2, markt, town, hafen, cave, kanali, catac, warc);
  		
		return tile2;
  	}
}
