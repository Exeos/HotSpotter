plugins {
    id("java")
}

group = "me.exeos.hotspotter.event"
version = "1.0-SNAPSHOT"

repositories {
    mavenCentral()
}

dependencies {
}

java {
    toolchain {
        languageVersion.set(JavaLanguageVersion.of(26))
    }
}