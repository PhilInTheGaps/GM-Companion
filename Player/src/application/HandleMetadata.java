package application;

import javafx.scene.image.Image;

public class HandleMetadata {
	public static void handle(String key, Object value){
		switch (key){
    	case ("album"):
    		Main.Album = value.toString();
    		Main.albumLabel.setText("Album: " + Main.Album);
    		System.out.println("Album: " + Main.Album);
			break;
    	case ("artist"):
    		Main.Artist = value.toString();
    		Main.artistLabel.setText("Artist: " + Main.Artist);
    		System.out.println("Artist: " + Main.Artist);
    		break;
    	case ("title"):
    		Main.Title = value.toString();
    		Main.titleLabel.setText("Title: " + Main.Title);
    		System.out.println("Title: " + Main.Title);
    		break;
    	case ("year"):
    		Main.Year = value.toString();
    		Main.yearLabel.setText("Year: " + Main.Year);
    		System.out.println("Year: " + Main.Year);
    		break;
    	//case ("image"):
    		//Image cover = (Image) value;
    		//Main.coverImage.setImage(cover);
    		//break;
    	}
		Main.coverImage.setImage((Image) MPlayer.mediaPlayer.getMedia().getMetadata().get("image"));
	}
}
