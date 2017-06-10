package gm_companion.addon_manager;

import javax.swing.*;

/**
 * Created by Phil on 10.06.2017.
 */
public class AddonManager {
    private JPanel panel;

    public static void main(String[] args) {
        JFrame frame = new JFrame("Addon Manager");
        frame.setContentPane(new AddonManager().panel);
        frame.setVisible(true);

        
    }
}
