package application;

public class nextSoundFile {
	public static void next(){
    	//Same as with nextMusicFile, only for sounds
    	if (Main.soundIsPlaying == true){
    		Main.soundPlayer.stop();
    	}
    	
    	if (Main.randomTrack == true){
    		if(Main.maxSoundCount != 0){
    			if(Main.currentSoundID == Main.maxSoundCount-1){
        			for(int i = 0; i<50; i++){
        				System.out.println(Main.soundPathList);
        				System.out.println("Neue Liste wird generiert");
        				
        				String shuffle1 = "";
        				String shuffle2 = "";
        				String shuffle3 = "";
        				
        				System.out.println(shuffle1);
        				System.out.println(shuffle2);
        				System.out.println(shuffle3);
        				
        				System.out.println(Main.maxSoundCount);
        				
        				int ID1 = Main.randomSoundID.nextInt(Main.maxSoundCount);
        				shuffle1 = Main.soundPathList[ID1];
        				System.out.println(ID1);
        				int ID2 = Main.randomSoundID.nextInt(Main.maxSoundCount);
        				shuffle2 = Main.soundPathList[ID2];
        				System.out.println(ID2);
        				shuffle3 = shuffle2;
        				shuffle2 = shuffle1;
        				shuffle1 = shuffle3;
        				Main.currentSoundID = 0;
        			}
        			Main.currentSoundID = 0;
        			//System.out.println("Next Random Sound: " + currentSoundID);
        		}
        		else{
        			Main.currentSoundID++;
        			//currentSoundID = 0;
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
