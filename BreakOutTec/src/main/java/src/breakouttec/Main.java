package src.breakouttec;

import javafx.application.Application;
import javafx.scene.Group;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.paint.Color;
import javafx.stage.Stage;
import org.json.simple.parser.ParseException;


public class Main extends Application {

    public static void main(String[] args) {
        launch(args);
    }

    /**
     * @param win Ventana principal
     */
    @Override
    public void start(Stage win) {

        win.setTitle("BreakOutTec!");
        Group group = new Group();
        Scene scene = new Scene(group, 800, 600, Color.AZURE);
        win.setScene(scene);

        Button btn = new Button("Jugador");
        btn.setLayoutX(325);
        btn.setLayoutY(190);
        btn.setMinWidth(150);
        btn.setMinHeight(100);
        group.getChildren().add(btn);
        btn.setOnAction(e -> {
            win.close();
            try {
                Jugador.getInstance().start(new Stage());
            } catch (ParseException ex) {
                throw new RuntimeException(ex);
            }
        });

        Button btn2 = new Button("Espectador");
        btn2.setLayoutX(325);
        btn2.setLayoutY(310);
        btn2.setMinWidth(150);
        btn2.setMinHeight(100);
        group.getChildren().add(btn2);
        btn2.setOnAction(e -> {
            win.close();
            Espectador.getInstance().start(new Stage());
        });

        win.show();
    }
}
