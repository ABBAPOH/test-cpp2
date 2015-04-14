import qbs.base 1.0

Product {
    Depends { name: "cpp" }

    type: "application"
    consoleApplication: true
    destinationDirectory: project.install_binary_path
    files: [ "*.cpp", "*.h" ]

    cpp.includePaths: project.includePaths
    cpp.cxxFlags: project.cxxFlags
    cpp.linkerFlags: project.linkFlags

    Properties {
        condition: qbs.targetOS.contains("osx")
        cpp.installNamePrefix: project.installNamePrefix
        cpp.minimumOsxVersion: "10.7"
    }

    Group {
        fileTagsFilter: product.type
        qbs.install: true
        qbs.installDir: project.install_app_path
    }
}
