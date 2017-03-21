package application;

import java.net.URL;

import javafx.application.Preloader;
import javafx.scene.Scene;
import javafx.scene.control.Label;
import javafx.scene.control.ProgressBar;
import javafx.scene.image.Image;
import javafx.scene.layout.BorderPane;
import javafx.scene.layout.VBox;
import javafx.stage.Stage;
import javafx.stage.StageStyle;
import javafx.application.Preloader.StateChangeNotification.Type;
import javafx.geometry.Pos;

public class PreloadScreen extends Preloader {

	private Stage preloaderStage;

	@Override
	public void start(Stage primaryStage) throws Exception {

		System.out.println("Initializing...");
		System.out.println("");
		
		this.preloaderStage = primaryStage;
		
		VBox loading = new VBox(20);
		loading.setPrefWidth(400);
		loading.setAlignment(Pos.CENTER);
		
		ProgressBar pb = new ProgressBar();
		pb.setPrefWidth(350);
		
		loading.getChildren().addAll(new Label(" "), pb, new Label("Please wait..."));
		
		BorderPane root = new BorderPane(loading);
		root.getStylesheets().addAll(getClass().getResource("DarkMode.css").toExternalForm());
		URL bip = getClass().getResource("splash.jpg");
		root.setStyle("-fx-background-image: url('" + bip
				+ "'); -fx-background-size: auto; -fx-background-position: center;");
		
		Scene scene = new Scene(root);
		
		// Adds the icon
		String[] icons = {"icon32.png", "icon64.png", "icon128.png", "icon256.png"};
		primaryStage.getIcons().clear();
		for (String icon : icons){
			URL iconURL = getClass().getResource(icon);
			primaryStage.getIcons().add(new Image(iconURL.toExternalForm()));
		}
		
		primaryStage.initStyle(StageStyle.UNDECORATED);
		primaryStage.setWidth(400);
		primaryStage.setHeight(200);
		primaryStage.setScene(scene);
		primaryStage.show();
	}

	@Override
	public void handleStateChangeNotification(StateChangeNotification stateChangeNotification) {
		if (stateChangeNotification.getType() == Type.BEFORE_START) {
			preloaderStage.hide();
		}
	}
}
