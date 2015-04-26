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

    cpp.dynamicLibraries: [ "pthread" ]

    Properties {
        condition: qbs.targetOS.contains("osx")
        cpp.installNamePrefix: project.installNamePrefix
        cpp.minimumOsxVersion: "10.7"
    }

    Properties {
        condition: qbs.targetOS.contains("unix") && !qbs.targetOS.contains("osx")
        cpp.rpaths: [ "$ORIGIN/../lib/" + project.app_target ]
    }

    Group {
        fileTagsFilter: product.type
        qbs.install: true
        qbs.installDir: project.install_binary_path
    }
}
