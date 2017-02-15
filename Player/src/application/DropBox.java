package application;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

import com.dropbox.core.DbxException;
import com.dropbox.core.DbxRequestConfig;
import com.dropbox.core.v2.DbxClientV2;
import com.dropbox.core.v2.files.FileMetadata;
import com.dropbox.core.v2.files.ListFolderResult;
import com.dropbox.core.v2.files.Metadata;
import com.dropbox.core.v2.users.FullAccount;

public class DropBox {
	//APP KEY: u8tb7c5le0qr2ip
	//ACCES TOKEN: onk3FayfIUAAAAAAAAAAGUKyYY72tHuPBcWOAQVzBB7NyyE-KesExTXbL5_TgeaV
	
	
	private static final String ACCESS_TOKEN = "onk3FayfIUAAAAAAAAAAGUKyYY72tHuPBcWOAQVzBB7NyyE-KesExTXbL5_TgeaV";

	public static void main() throws DbxException {
        // Create Dropbox client
        DbxRequestConfig config = new DbxRequestConfig("dropbox/java-tutorial", "en_US");
        DbxClientV2 client = new DbxClientV2(config, ACCESS_TOKEN);
        
        // Get current account info
        
        FullAccount account = client.users().getCurrentAccount();
        System.out.println(account.getName().getDisplayName());
        
        // Get files and folder metadata from Dropbox root directory
        
        ListFolderResult result = client.files().listFolder("");
        
        for (Metadata metadata : result.getEntries()) {
            System.out.println(metadata.getPathLower());
        }
        
        // Upload "test.txt" to Dropbox
        try (InputStream in = new FileInputStream("resources/icon.png")) {
            try {
				FileMetadata metadata = client.files().uploadBuilder("/icon.png")
				    .uploadAndFinish(in);
				System.out.println("Uploaded File");
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
        } catch (FileNotFoundException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		} catch (IOException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}

        //result = client.files().listFolderContinue(result.getCursor());
        
    }
}
