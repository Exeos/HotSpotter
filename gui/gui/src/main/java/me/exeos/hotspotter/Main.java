package me.exeos.hotspotter;

import javafx.application.Application;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.layout.StackPane;
import javafx.scene.layout.VBox;
import javafx.scene.text.Text;
import javafx.stage.Stage;
import me.exeos.hotspotter.ipc.Talker;
import me.exeos.hotspotter.ipc.packet.impl.ConnectPacket;


public class Main extends Application {

    public final static Talker talker = new Talker();

    public static void main(String[] args) {
        launch(args);
    }

    @Override
    public void start(Stage stage) throws Exception {
        stage.setScene(new Scene(getContent(), 300, 300));
        stage.show();
    }

    private Parent getContent() {
        VBox boxContainer = new VBox();
        Button injectButton = new Button("Inject");

        injectButton.setOnAction((event) -> {
            talker.request(new ConnectPacket());
            System.out.println("Connected");
        });

        boxContainer.getChildren().add(injectButton);

        return boxContainer;
    }
}