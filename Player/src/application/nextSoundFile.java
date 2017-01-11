package application;

public class nextSoundFile {
	public static void next(){
    	
    	if (Main.soundIsPlaying == true){
    		Main.soundPlayer.stop();
    	}
    	
    	if (Main.randomTrack == true){
    		if(Main.maxSoundCount != 0){
    			//System.out.println(maxSoundCount);
    			Main.currentSoundID = Main.randomSoundID.nextInt(Main.maxSoundCount);
    			System.out.println("Next Random Sound ID: " + Main.currentSoundID);
    		}
    		else{
    			Main.currentSoundID = 0;
    		}
    	}
    	else{
    		if (Main.currentSoundID < Main.maxSoundCount - 1){
    			Main.currentSoundID ++;
        	}
        	else{
        		Main.currentSoundID = 0;
        	}
    	}

    	SPlayer.play();
    }
}
