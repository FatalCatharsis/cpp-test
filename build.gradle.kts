plugins {
    `cpp-library`
    `maven-publish`
}

group = "github.fatalcatharsis"
version = "0.1-SNAPSHOT"

repositories {
    mavenLocal()
    mavenCentral()
    maven {
        name = "game-engine-jfrog"
        credentials {
            val gameEngineRepoUsername : String by project
            val gameEngineRepoPassword : String by project
            username = gameEngineRepoUsername
            password = gameEngineRepoPassword
        }
        url = uri("https://artifactory.fatalcatharsis.com/artifactory/game-engine-repo")
    }
}

library {
    linkage.set(listOf(Linkage.STATIC))

    targetMachines.set(listOf(
        machines.linux.x86,
        machines.linux.x86_64,
        machines.windows.x86,
        machines.windows.x86_64
    ))

    binaries.configureEach {
        when (toolChain) {
            is VisualCpp -> {
                val linkerArgs = mutableListOf("/SUBSYSTEM:windows", "/ENTRY:mainCRTStartup")
                val compileArgs = mutableListOf("/std:c++17", "/D_HAS_STD_BYTE=0", "/EHsc")
                if (!isOptimized) {
                    compileArgs.addAll(listOf("/MDd", "/Zi"))
                    linkerArgs.add("/NODEFAULTLIB:MSVCRT")
                } else {
                    compileArgs.add("/MD")
                }
                compileTask.get().compilerArgs.addAll(compileArgs)
                if (this is CppExecutable) {
                    linkTask.get().linkerArgs.addAll(linkerArgs)
                }
            }
            is Gcc, is Clang -> compileTask.get().compilerArgs.addAll("-std=c++17")
        }
    }
}

publishing {
    repositories {
        mavenLocal()
        maven {
            name = "game-engine-jfrog"
            credentials {
                val gameEngineRepoUsername : String by project
                val gameEngineRepoPassword : String by project
                username = gameEngineRepoUsername
                password = gameEngineRepoPassword
            }
            url = uri("https://artifactory.fatalcatharsis.com/artifactory/game-engine-repo/")
        }
    }
}