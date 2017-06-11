package gm_companion.addon_manager;

import org.apache.commons.io.FileUtils;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.*;
import java.net.URL;
import java.net.HttpURLConnection;

public class AddonManager {
    private JPanel panel;
    private JPanel addons;
    private JButton cancelButton;

    public AddonManager() {
        cancelButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                System.exit(0);
            }
        });
    }

    public static void download(String urlString, String name){
        // Downloading
        try{
            URL newUrl = new URL(urlString);
            String bytesString = newUrl.openConnection().getHeaderField("Content-Length");
            int bytesInt = Integer.parseInt(bytesString);

            File f = new File("C:/Users/Phil/.gm-companion/addons/"+name+".zip");

            FileUtils.copyURLToFile(newUrl, f);

            if (f.length() < bytesInt)
                System.out.println("Something went wrong, the download is incomplete.");

        }catch (Exception e){
            System.out.println("Something went wrong downloading the addon.");
        }

        // Unzipping
        try{
            String zipFile = "C:/Users/Phil/.gm-companion/addons/"+name+".zip";
            String outputFolder = "C:/Users/Phil/.gm-companion/addons";

            Unzipper u = new Unzipper();
            u.unzip(zipFile, outputFolder);
        }catch (Exception e2){
            System.out.println("Could not unzip addon.");
        }
    }

    public static void main(String[] args) throws Exception {
        AddonManager addonManager = new AddonManager();

        JFrame frame = new JFrame("Addon Manager");
        frame.setContentPane(addonManager.panel);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setMinimumSize(new Dimension(350, 300));
        frame.pack();

        addonManager.addons.setLayout(new GridLayout(0, 4, 10, 10));
        addonManager.addons.add(new JLabel("Install"));
        addonManager.addons.add(new JLabel("Name"));
        addonManager.addons.add(new JLabel("Version"));
        addonManager.addons.add(new JLabel("Status"));

        URL url = new URL("https://philinthegaps.github.io/GM-Companion/addons/versions.txt");

        // Get Home Directory
        String homeDirectory = System.getProperty("user.home");
        homeDirectory = homeDirectory.replace("\\", "/");

        // Set .gm-companion/addons path
        String addonsPath = homeDirectory+"/.gm-companion/addons";
        File dir = new File(addonsPath);
        if (!dir.exists()) {
            System.out.println("GM-Companion directory does not exist. Creating...");
            dir.mkdir();
        }

        // Read available Addons from URL
        System.out.println("Getting available addons...");
        BufferedReader in = new BufferedReader(
                new InputStreamReader(url.openStream()));
        String inputLine;
        while ((inputLine = in.readLine()) != null) {

            int nameEnd = inputLine.indexOf(":");

            String name = inputLine.substring(0, nameEnd);
            String version = inputLine.substring(nameEnd + 1);
            String addonURLString = "https://philinthegaps.github.io/GM-Companion/addons/" + name + "_" + version + ".zip";

            JButton installButton = new JButton("Install");
            installButton.setMinimumSize(new Dimension(0, 20));
            installButton.setMaximumSize(new Dimension(200, 50));
            addonManager.addons.add(installButton);

            JLabel nameLabel = new JLabel(name);
            nameLabel.setMinimumSize(new Dimension(0, 20));
            nameLabel.setMaximumSize(new Dimension(200, 50));
            addonManager.addons.add(nameLabel);

            JLabel versionLabel = new JLabel(version);
            versionLabel.setMinimumSize(new Dimension(0, 20));
            versionLabel.setMaximumSize(new Dimension(200, 50));
            addonManager.addons.add(versionLabel);

            JLabel statusLabel = new JLabel("Available");
            statusLabel.setMinimumSize(new Dimension(0, 20));
            statusLabel.setMaximumSize(new Dimension(200, 50));
            addonManager.addons.add(statusLabel);

            URL addonURL = new URL(addonURLString);
            HttpURLConnection huc = (HttpURLConnection)addonURL.openConnection();
            huc.setRequestMethod("HEAD");
            int responseCode = huc.getResponseCode();
            if (responseCode != 404) {
                // ToDo
            } else {
                statusLabel.setText("Not available");
                installButton.setEnabled(false);
            }

            // Check if local version exists and if it is outdated
            File local = new File(addonsPath+"/"+name);
            if (local.exists()) {
                File versionFile = new File(addonsPath+"/"+name+"/version.txt");

                if (versionFile.exists()){
                    int versionInt = 0;
                    BufferedReader br = new BufferedReader(new FileReader(versionFile));
                    try {
                        String versionString = "";
                        String line = br.readLine();

                        while (line != null) {
                            versionString += line;
                            line = br.readLine();
                        }

                        versionInt = Integer.parseInt(versionString.replace(".", ""));

                    } finally {
                        br.close();
                    }

                    if (versionInt < Integer.parseInt(version.replace(".", ""))){
                        statusLabel.setText("Update Available");
                        installButton.setText("Update");
                    }else {
                        statusLabel.setText("Already Installed");
                        installButton.setEnabled(false);
                    }

                }else {
                    statusLabel.setText("Already Installed");
                    installButton.setEnabled(false);
                }
            }

            // Connect install button with functionality
            installButton.addActionListener(new ActionListener() {
                @Override
                public void actionPerformed(ActionEvent e) {
                    System.out.println("Downloading "+name+"...");
                    download(addonURLString, name);
                }
            });

            frame.validate();
        }
        in.close();

        frame.setVisible(true);
    }
}
