import qbs.base 1.0

Product {
    Depends { name: "cpp" }
    Depends { name: "core" }

    type: "application"
    consoleApplication: true
    destinationDirectory: project.install_binary_path
    files: [ "*.cpp", "*.h" ]
    excludeFiles: ["*_mac.cpp", "*_linux.cpp"]

    cpp.includePaths: project.includePaths
    cpp.cxxFlags: project.cxxFlags
    cpp.linkerFlags: project.linkFlags

    cpp.dynamicLibraries: [ "pthread" ]

    Properties {
        condition: qbs.targetOS.contains("osx")
        cpp.installNamePrefix: project.installNamePrefix
        cpp.minimumOsxVersion: "10.7"
    }

    Group {
        name: "mac"
        condition: qbs.targetOS.contains("osx")
        files: ["*_mac.cpp"]
    }

    Group {
        name: "linux"
        condition: qbs.targetOS.contains("linux")
        files: ["*_linux.cpp"]
    }

    Group {
        fileTagsFilter: product.type
        qbs.install: true
        qbs.installDir: project.install_app_path
    }
}
