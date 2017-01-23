package application;

public class nextMusicFile {
	public static void next(){
		//Switches to the next music file
		System.out.println("");
		System.out.println("Changing to next music file...");
		
		//Checks, whether a file is currently playing and stops it if true
    	if (Main.musicIsPlaying == true){
    		MPlayer.mediaPlayer.stop();
    	}
    	
    	//Creates a randomly generated playlist if the current file is the last one in the playlist
    	if (Main.randomTrack == true){
    		if(Main.maxTrackCount != 0){
    			if(Main.currentTrackID == Main.maxTrackCount-1){
    				System.out.println("Generating new random music playlist...");
        			for(int i = 0; i<500; i++){
        				String shuffle1 = "";
        				String shuffle2 = "";
        				String shuffle3 = "";
        				
        				int ID1 = Main.randomTrackID.nextInt(Main.maxTrackCount);
        				shuffle1 = Main.musicPathList[ID1];
        				
        				int ID2 = Main.randomTrackID.nextInt(Main.maxTrackCount);
        				shuffle2 = Main.musicPathList[ID2];
        				
        				shuffle3 = shuffle2;
        				shuffle2 = shuffle1;
        				shuffle1 = shuffle3;
        				
        				Main.musicPathList[ID1] = shuffle1;
        				Main.musicPathList[ID2] = shuffle2;
        				
        				Main.currentTrackID = 0;
        			}
        			System.out.println("Random music playlist generated:");
        			if(Main.debug){
        				for(int i = 0; i < Main.maxTrackCount; i++){
        					System.out.println(Main.musicPathList[i]);
        				}
        			}
        			
        			System.out.println("");
        		}
        		else{
        			Main.currentTrackID++;
        		}
    		}
    		
    	}
    	else{
    		if (Main.currentTrackID < Main.maxTrackCount - 1){
    			Main.currentTrackID ++;
        	}
        	else{
        		Main.currentTrackID = 0;
        	}
    	}
    	
    	MPlayer.play();
    }
}
