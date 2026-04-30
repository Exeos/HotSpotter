plugins {
    id("java")
}

group = "me.exeos.hotspotter.ipc"
version = "1.0-SNAPSHOT"

repositories {
    mavenCentral()
}

dependencies {
    implementation("org.zeromq:jeromq:0.6.0")
    implementation(project(":event-bus"))
}

java {
    toolchain {
        languageVersion.set(JavaLanguageVersion.of(26))
    }
}