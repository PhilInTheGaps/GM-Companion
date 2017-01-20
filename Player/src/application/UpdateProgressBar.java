package application;

import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.util.Duration;

public class UpdateProgressBar {
	public static void update(){
		ChangeListener<Duration> progressChangeListener;
		progressChangeListener = new ChangeListener<Duration>(){
			@Override public void changed(ObservableValue<? extends Duration> observableValue, Duration oldValue, Duration newValue) {
		        Main.pb.setProgress(1.0 * Main.mediaPlayer.getCurrentTime().toMillis() / Main.mediaPlayer.getTotalDuration().toMillis());
		      }
			
		};
		Main.mediaPlayer.currentTimeProperty().addListener(progressChangeListener);

	}
}
