plugins {
    id("application")
    id("org.openjfx.javafxplugin") version "0.1.0"
}

group = "me.exeos.hotspotter"
version = "1.0-SNAPSHOT"

repositories {
    mavenCentral()
}

dependencies {
    implementation(project(":event-bus"))
    implementation(project(":ipc"))
}

java {
    toolchain {
        languageVersion.set(JavaLanguageVersion.of(26))
    }
}

javafx {
    version = "26.0.1"
    modules = listOf("javafx.controls", "javafx.fxml")
}

application {
    mainClass.set("me.exeos.hotspotter.Main")
}