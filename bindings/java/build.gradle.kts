plugins {
    `java-library`
    id("com.vanniktech.maven.publish") version "0.37.0"
}

group = "io.github.boyninja1555"
version = "1.0.2"

mavenPublishing {
    publishToMavenCentral()
    signAllPublications()

    pom {
        name.set("WBTP Java")
        description.set("(Java implementation) Drop-in binary-framed replacement for HTTP")
        url.set("https://github.com/boyninja1555/wbtp#readme")

        licenses {
            license {
                name.set("OFRTISYDCAL")
                url.set("https://github.com/boyninja1555/wbtp/blob/main/LICENSE")
            }
        }

        developers {
            developer {
                id.set("boyninja1555")
                name.set("Floor Mann")
                email.set("flappyfloorg@gmail.com")
            }
        }

        scm {
            connection.set("scm:git:https://github.com/boyninja1555/wbtp.git")
            developerConnection.set("scm:git:git@github.com:boyninja1555/wbtp.git")
            url.set("https://github.com/boyninja1555/wbtp#readme")
        }
    }
}

repositories {
    mavenCentral()
}

dependencies {
    compileOnlyApi("org.jetbrains:annotations:26.1.0")
}