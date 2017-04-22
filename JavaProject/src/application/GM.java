package application;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.concurrent.ThreadLocalRandom;

import javafx.event.ActionEvent;
import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.Node;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.ScrollPane;
import javafx.scene.control.TextField;
import javafx.scene.layout.BorderPane;
import javafx.scene.layout.GridPane;
import javafx.scene.layout.VBox;

public class GM {
	public static String databasePath;

	public static BorderPane Dice() {
		BorderPane gm = new BorderPane();
		int[] dice = { 3, 4, 6, 8, 10, 12, 20 };

		GridPane grid = new GridPane();
		grid.setAlignment(Pos.TOP_LEFT);
		grid.setHgap(20);
		grid.setVgap(10);
		grid.setMinHeight(360);
		grid.setPadding(new Insets(20));

		Label die = new Label("Die");
		Label number = new Label("Number");
		Label mod = new Label("Modifier");
		Label result = new Label("Result");

		grid.add(number, 0, 0);
		grid.add(die, 1, 0);
		grid.add(mod, 2, 0);
		grid.add(result, 4, 0);

		// Regular Dice
		int row = 1;
		for (int i : dice) {
			Label l = new Label("D" + i);

			TextField r = new TextField();
			r.setEditable(false);

			TextField n = new TextField();
			n.setText("1");
			n.setMaxWidth(60);

			TextField m = new TextField();

			Button roll = new Button();
			roll.setText("Roll");
			roll.setOnAction(ActionEvent -> {
				int res = 0;
				for (int f = 0; f < Integer.valueOf(n.getText()); f++) {
					res += ThreadLocalRandom.current().nextInt(1, i + 1);
				}
				if (!m.getText().isEmpty()) {
					res += Integer.valueOf(m.getText());
				}
				r.setText(res + "");
			});

			grid.add(n, 0, row);
			grid.add(l, 1, row);
			grid.add(m, 2, row);
			grid.add(roll, 3, row);
			grid.add(r, 4, row);

			row++;
		}

		// Custom Die
		TextField dc = new TextField();
		dc.setMaxWidth(50);
		TextField rc = new TextField();
		rc.setEditable(false);
		TextField nc = new TextField();
		nc.setText("1");
		nc.setMaxWidth(60);
		TextField mc = new TextField();
		Button roll = new Button();
		roll.setText("Roll");
		roll.setOnAction(ActionEvent -> {
			int res = 0;
			for (int f = 0; f < Integer.valueOf(nc.getText()); f++) {
				res += ThreadLocalRandom.current().nextInt(1, Integer.valueOf(dc.getText()) + 1);
			}
			if (!mc.getText().isEmpty()) {
				res += Integer.valueOf(mc.getText());
			}
			rc.setText(res + "");
		});
		grid.add(nc, 0, row);
		grid.add(dc, 1, row);
		grid.add(mc, 2, row);
		grid.add(roll, 3, row);
		grid.add(rc, 4, row);

		// Add grid to BorderPane and set style
		grid.getStyleClass().add("grid-pane");
		gm.setTop(grid);
		return gm;
	}

	@SuppressWarnings("static-access")
	public static BorderPane Database() {
		BorderPane db = new BorderPane();
		if (databasePath != null) {
			
			//String s = new String();
			ArrayList<String> database = new ArrayList<String>();
			ArrayList<String> a = new ArrayList<String>();
			ArrayList<GridPane> tables = new ArrayList<GridPane>();
			VBox v = new VBox();
			Boolean openColumns = false;
			Boolean openRows = false;
			String tableName;
			int tableIndex = -1;
			int tableRow = 0;
			int tableColumn = 0;
			
			FileReader fr;
			
			try {
				fr = new FileReader(new File(databasePath));
				BufferedReader br = new BufferedReader(fr);
				int index = 0;
				String temp;
				while ((temp = br.readLine()) != null) {
					database.add(index, temp);
					index++;
				}
				fr.close();
				br.close();

				for (String s : database) {
					if (s.contains(") ENGINE=")) {
						openColumns = false;
					}

					if (openColumns) {
						tableColumn++;
						int i1 = s.indexOf("`");
						int i2 = s.indexOf("`", i1 + 1);
						String text = s.substring(i1 + 1, i2);
						Label l = new Label(text);
						l.setId(text);
						tables.get(tableIndex).add(l, tableColumn, tableRow);
					}

					if (openRows) {
						tableColumn = 0;
						tableRow++;
						int i1 = 0;
						while (s.indexOf(",", i1 + 1) > 0) {
							int i2 = s.indexOf(",", i1 + 1);
							String text = s.substring(i1 + 1, i2);
							text = text.replace("`", "").replace("'", "").replace("\"", "").replace(")", "")
									.replace("(", "");
							Label l = new Label(text);
							l.setId(text);
							l.setStyle("-fx-font-weight: normal;");
							tables.get(tableIndex).add(l, tableColumn, tableRow);
							i1 = i2;
							tableColumn++;
						}
					}

					if (s.contains("CREATE TABLE")) {
						openColumns = true;
						int i1 = ("CREATE TABLE").length() + 2;
						int i2 = s.indexOf("`", i1);
						tableName = s.substring(i1, i2);
						tables.add(new GridPane());
						tableIndex++;
						tableRow = 1;
						tableColumn = -1;

						TextField tCharacter = new TextField();
						tCharacter.setPromptText("Character");
						TextField tItem = new TextField();
						tItem.setPromptText("Column");
						TextField tResult = new TextField();
						tResult.setPromptText("Search Result");
						tResult.setEditable(false);

						Button search = new Button("Search");
						int ti = tableIndex;
						search.setOnAction((ActionEvent e) -> {
							String result = " ";
							String character = tCharacter.getText();
							;
							String item = tItem.getText();
							int row = 0;
							int column = 0;

							for (Node node : tables.get(ti).getChildren()) {
								if (node.getId() != null && node.getId().equals(character)) {
									row = tables.get(ti).getRowIndex(node);
								}
								if (node.getId() != null && node.getId().equals(item)) {
									column = tables.get(ti).getColumnIndex(node);
								}
							}
							for (Node node : tables.get(ti).getChildren()) {
								if (tables.get(ti).getRowIndex(node) == row
										&& tables.get(ti).getColumnIndex(node) == column) {
									result = node.getId();
									break;
								}
							}
							tResult.setText(result);
						});

						tables.get(tableIndex).add(new Label(tableName), 0, 0);
						tables.get(tableIndex).add(tCharacter, 1, 0);
						tables.get(tableIndex).add(tItem, 2, 0);
						tables.get(tableIndex).add(search, 3, 0);
						tables.get(tableIndex).add(tResult, 4, 0);
						tables.get(tableIndex).setHgap(20);
						tables.get(tableIndex).setVgap(5);
						tables.get(tableIndex).getStyleClass().add("grid-pane");
						v.getChildren().add(tables.get(tableIndex));
						v.getChildren().add(new Label(""));
					}

					if (s.contains("INSERT INTO")) {
						openRows = true;
					}

					if (s.contains(";")) {
						openRows = false;
					}
					a.add(s);
				}

			} catch (FileNotFoundException e) {
				System.out.println("Database file not found!");
				// e.printStackTrace();
			} catch (IOException e) {
				e.printStackTrace();
			}

			ScrollPane sp = new ScrollPane();
			sp.setStyle("-fx-background-color: transparent");
			sp.setContent(v);

			db.setCenter(sp);
		}

		return db;
	}
}