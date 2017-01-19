package application;

public class nextSoundFile {
	public static void next(){
    	//Same as with nextMusicFile, only for sounds
		System.out.println("");
		System.out.println("Changing to next sound file...");
		
    	if (Main.soundIsPlaying == true){
    		Main.soundPlayer.stop();
    	}
    	
    	if (Main.randomTrack == true){
    		if(Main.maxSoundCount != 0){
    			if(Main.currentSoundID == Main.maxSoundCount-1){
    				System.out.println("Generating new random sound playlist...");
        			for(int i = 0; i<50; i++){
        				
        				String shuffle1 = "";
        				String shuffle2 = "";
        				String shuffle3 = "";
        				
        				int ID1 = Main.randomSoundID.nextInt(Main.maxSoundCount);
        				shuffle1 = Main.soundPathList[ID1];
        				
        				int ID2 = Main.randomSoundID.nextInt(Main.maxSoundCount);
        				shuffle2 = Main.soundPathList[ID2];
        				
        				shuffle3 = shuffle2;
        				shuffle2 = shuffle1;
        				shuffle1 = shuffle3;
        				
        				Main.soundPathList[ID1] = shuffle1;
        				Main.soundPathList[ID2] = shuffle2;
        				
        				Main.currentSoundID = 0;
        			}
        			System.out.println("Random sound playlist generated");
        			System.out.println("");
        		}
        		else{
        			Main.currentSoundID++;
        		}
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
