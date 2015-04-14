import qbs.base 1.0

Project {
    property string app_target: "test-cpp1"

    property string install_app_path: {
        if (qbs.targetOS.contains("windows"))
            return ".";
        else
            return "bin";
    }
    property string lib_suffix: ""

    property string install_binary_path: install_app_path

    property string install_library_path: {
        if (qbs.targetOS.contains("windows"))
            return install_app_path
        else
            return "lib" + lib_suffix + "/" + app_target
    }

    property stringList includePaths: [ path + "/include" ]

    property stringList cxxFlags: {
        if (qbs.targetOS.contains("osx"))
            return [ "-std=c++11", "-stdlib=libc++" ]
        if (qbs.targetOS.contains("linux"))
            return [ "-std=c++11", "-Werror" ]
        else
            return []
    }
    property stringList linkFlags: {
        if (qbs.targetOS.contains("osx"))
            return [ "-stdlib=libc++" ]
        else
            return []
    }

    name: "Test Cpp 2"

    SubProject {
        filePath: "src/src.qbs"
    }
}

