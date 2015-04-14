import qbs.base 1.0

Product {
    Depends { name: "cpp" }

    type: "application"
    consoleApplication: true
    destinationDirectory: project.install_binary_path
    files: [ "*.cpp", "*.h" ]

    Group {
        fileTagsFilter: product.type
        qbs.install: true
        qbs.installDir: project.install_app_path
    }
}
