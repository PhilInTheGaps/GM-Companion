package application;

import java.util.concurrent.ThreadLocalRandom;

import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.TextField;
import javafx.scene.layout.BorderPane;
import javafx.scene.layout.GridPane;

public class GM {
	public static BorderPane GMHelp(){
		BorderPane gm = new BorderPane();
		gm.setStyle("-fx-background-color: transparent");
		
		GridPane grid = new GridPane();
		grid.setStyle("-fx-background-color: white");
		grid.setAlignment(Pos.TOP_LEFT);
		grid.setHgap(20);
		grid.setVgap(10);
		grid.setMinHeight(360);
		grid.setPadding(new Insets(20));
		//grid.setMaxHeight(400);
		
		Label die = new Label("Die");
		Label number = new Label("Number");
		Label mod = new Label("Modifier");
		Label result = new Label("Result");
		
		Label d3 = new Label("D3");
		Label d4 = new Label("D4");
		Label d6 = new Label("D6");
		Label d8 = new Label("D8");
		Label d10 = new Label("D10");
		Label d12 = new Label("D12");
		Label d20 = new Label("D20");
		TextField dc = new TextField();
		dc.setMaxWidth(50);
		
		TextField r3 = new TextField();
		r3.setEditable(false);
		TextField r4 = new TextField();
		r4.setEditable(false);
		TextField r6 = new TextField();
		r6.setEditable(false);
		TextField r8 = new TextField();
		r8.setEditable(false);
		TextField r10 = new TextField();
		r10.setEditable(false);
		TextField r12 = new TextField();
		r12.setEditable(false);
		TextField r20 = new TextField();
		r20.setEditable(false);
		TextField rc = new TextField();
		rc.setEditable(false);
		
		TextField n3 = new TextField();
		n3.setText("1");
		n3.setMaxWidth(60);
		TextField n4 = new TextField();
		n4.setText("1");
		n4.setMaxWidth(60);
		TextField n6 = new TextField();
		n6.setText("1");
		n6.setMaxWidth(60);
		TextField n8 = new TextField();
		n8.setText("1");
		n8.setMaxWidth(60);
		TextField n10 = new TextField();
		n10.setText("1");
		n10.setMaxWidth(60);
		TextField n12 = new TextField();
		n12.setText("1");
		n12.setMaxWidth(60);
		TextField n20 = new TextField();
		n20.setText("1");
		n20.setMaxWidth(60);
		TextField nc = new TextField();
		nc.setText("1");
		nc.setMaxWidth(60);
		
		TextField m3 = new TextField();
		TextField m4 = new TextField();
		TextField m6 = new TextField();
		TextField m8 = new TextField();
		TextField m10 = new TextField();
		TextField m12 = new TextField();
		TextField m20 = new TextField();
		TextField mc = new TextField();
		
		grid.add(number, 0, 0);
		grid.add(die, 1, 0);
		grid.add(mod, 2, 0);
		grid.add(result, 4, 0);
		
		grid.add(d3, 1, 1);
		grid.add(d4, 1, 2);
		grid.add(d6, 1, 3);
		grid.add(d8, 1, 4);
		grid.add(d10, 1, 5);
		grid.add(d12, 1, 6);
		grid.add(d20, 1, 7);
		grid.add(dc, 1, 8);
		
		grid.add(r3, 4, 1);
		grid.add(r4, 4, 2);
		grid.add(r6, 4, 3);
		grid.add(r8, 4, 4);
		grid.add(r10, 4, 5);
		grid.add(r12, 4, 6);
		grid.add(r20, 4, 7);
		grid.add(rc, 4, 8);
		
		grid.add(n3, 0, 1);
		grid.add(n4, 0, 2);
		grid.add(n6, 0, 3);
		grid.add(n8, 0, 4);
		grid.add(n10, 0, 5);
		grid.add(n12, 0, 6);
		grid.add(n20, 0, 7);
		grid.add(nc, 0, 8);
		
		grid.add(m3, 2, 1);
		grid.add(m4, 2, 2);
		grid.add(m6, 2, 3);
		grid.add(m8, 2, 4);
		grid.add(m10, 2, 5);
		grid.add(m12, 2, 6);
		grid.add(m20, 2, 7);
		grid.add(mc, 2, 8);
		
		for(int i = 1; i<9; i++){
			Button roll = new Button();
			roll.setText("Roll");
			
			switch(i){
				case 1: 
					roll.setOnAction(ActionEvent ->{
						int r = 0;
						for(int f = 0; f<Integer.valueOf(n3.getText()); f++){
							r += ThreadLocalRandom.current().nextInt(1, 4);
						}
						if(!m3.getText().isEmpty()){
							r += Integer.valueOf(m3.getText());
						}
						r3.setText(r+"");
					});
					break;
				case 2: 
					roll.setOnAction(ActionEvent ->{
						int r = 0;
						for(int f = 0; f<Integer.valueOf(n4.getText()); f++){
							r += ThreadLocalRandom.current().nextInt(1, 5);
						}
						if(!m4.getText().isEmpty()){
							r += Integer.valueOf(m4.getText());
						}
						r4.setText(r+"");
					});
					break;
				case 3:
					roll.setOnAction(ActionEvent ->{
						int r = 0;
						for(int f = 0; f<Integer.valueOf(n6.getText()); f++){
							r += ThreadLocalRandom.current().nextInt(1, 7);
						}
						if(!m6.getText().isEmpty()){
							r += Integer.valueOf(m6.getText());
						}
						r6.setText(r+"");
					});
					break;
				case 4:
					roll.setOnAction(ActionEvent ->{
						int r = 0;
						for(int f = 0; f<Integer.valueOf(n8.getText()); f++){
							r += ThreadLocalRandom.current().nextInt(1, 9);
						}
						if(!m8.getText().isEmpty()){
							r += Integer.valueOf(m8.getText());
						}
						r8.setText(r+"");
					});
					break;
				case 5:
					roll.setOnAction(ActionEvent ->{
						int r = 0;
						for(int f = 0; f<Integer.valueOf(n10.getText()); f++){
							r += ThreadLocalRandom.current().nextInt(1, 11);
						}
						if(!m10.getText().isEmpty()){
							r += Integer.valueOf(m10.getText());
						}
						r10.setText(r+"");
					});
					break;
				case 6:
					roll.setOnAction(ActionEvent ->{
						int r = 0;
						for(int f = 0; f<Integer.valueOf(n12.getText()); f++){
							r += ThreadLocalRandom.current().nextInt(1, 13);
						}
						if(!m12.getText().isEmpty()){
							r += Integer.valueOf(m12.getText());
						}
						r12.setText(r+"");
					});
					break;
				case 7:
					roll.setOnAction(ActionEvent ->{
						int r = 0;
						for(int f = 0; f<Integer.valueOf(n20.getText()); f++){
							r += ThreadLocalRandom.current().nextInt(1, 21);
						}
						if(!m20.getText().isEmpty()){
							r += Integer.valueOf(m20.getText());
						}
						r20.setText(r+"");
					});
					break;
				case 8:
					roll.setOnAction(ActionEvent ->{
						int r = 0;
						for(int f = 0; f<Integer.valueOf(nc.getText()); f++){
							r += ThreadLocalRandom.current().nextInt(1, Integer.valueOf(dc.getText())+1);
						}
						if(!mc.getText().isEmpty()){
							r += Integer.valueOf(mc.getText());
						}
						rc.setText(r+"");
					});
					break;
			}
			
			grid.add(roll, 3, i);
		}
		
		gm.setTop(grid);
		return gm;
	}
}
