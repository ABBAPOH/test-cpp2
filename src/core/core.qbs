import qbs.base 1.0

DynamicLibrary {
    Depends { name: "cpp" }

    destinationDirectory: project.install_library_path
    files: [ "*.cpp", "*.h" ]

    cpp.includePaths: project.includePaths
    cpp.cxxFlags: project.cxxFlags
    cpp.linkerFlags: project.linkFlags

    Properties {
        condition: qbs.targetOS.contains("osx")
        cpp.installNamePrefix: project.installNamePrefix
        cpp.minimumOsxVersion: "10.7"
    }

    Properties {
         condition: qbs.targetOS.contains("unix") && !qbs.targetOS.contains("osx")
         cpp.rpaths: "$ORIGIN"
    }

    Group {
        fileTagsFilter: product.type
        qbs.install: true
        qbs.installDir: project.install_library_path
    }
}
