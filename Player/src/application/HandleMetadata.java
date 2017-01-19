package application;

import javafx.scene.image.Image;

public class HandleMetadata {
	
	//Adds the found MetaData to display
	public static void handle(String key, Object value){
		switch (key){
    	case ("album"):
    		Main.Album = value.toString();
    		Main.albumLabel.setText("Album: " + Main.Album);
    		System.out.println("Set Album to '"+Main.Album+"'");
			break;
    	case ("artist"):
    		Main.Artist = value.toString();
    		Main.artistLabel.setText("Artist: " + Main.Artist);
    		System.out.println("Set Artist to '"+Main.Artist+"'");
    		break;
    	case ("title"):
    		Main.Title = value.toString();
    		Main.titleLabel.setText("Title: " + Main.Title);
    		System.out.println("Set Title to '"+Main.Title+"'");
    		break;
    	case ("year"):
    		Main.Year = value.toString();
    		Main.yearLabel.setText("Year: " + Main.Year);
    		System.out.println("Set Yeat to '"+Main.Year+"'");
    		break;
    	case ("image"):
    		Image cover = (Image) value;
    		Main.coverImage.setImage(cover);
    		System.out.println("Set Album Cover");
    		break;
    	}
	}
}
