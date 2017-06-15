package gm_companion.addon_manager;

import java.io.*;
import java.util.zip.ZipEntry;
import java.util.zip.ZipInputStream;

// This code was taken from http://www.codejava.net/

public class Unzipper {
    private static final int BUFFER_SIZE = 4096;

    public static void unzip(String filePath, String directory) throws IOException {
        System.out.println("Extracting Contents...");

        File dir = new File(directory);
        if (!dir.exists()) {
            dir.mkdir();
        }

        ZipInputStream zipIn = new ZipInputStream(new FileInputStream(filePath));
        ZipEntry entry = zipIn.getNextEntry();

        // iterates over entries in the zip file
        while (entry != null) {
            String newFilePath = directory + File.separator + entry.getName();
            if (!entry.isDirectory()) {
                // if the entry is a file, extracts it
                extractFile(zipIn, newFilePath);
            } else {
                // if the entry is a directory, make the directory
                File newDir = new File(newFilePath);
                newDir.mkdir();
            }
            zipIn.closeEntry();
            entry = zipIn.getNextEntry();
        }
        zipIn.close();

        File zipFile = new File(filePath);
        zipFile.delete();

        System.out.println("Done.");
    }

    private static void extractFile(ZipInputStream zipIn, String filePath) throws IOException {
        BufferedOutputStream bos = new BufferedOutputStream(new FileOutputStream(filePath));
        byte[] bytesIn = new byte[BUFFER_SIZE];
        int read = 0;
        while ((read = zipIn.read(bytesIn)) != -1) {
            bos.write(bytesIn, 0, read);
        }
        bos.close();
    }
}
