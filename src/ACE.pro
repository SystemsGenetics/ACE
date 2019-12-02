
lessThan(QT_MAJOR_VERSION,5): error("Requires Qt 5 or Greater")
equals(QT_MAJOR_VERSION,5) {
    lessThan(QT_MINOR_VERSION,7): error("Requires Qt 5.7 or Greater")
}

isEmpty(GUI) { GUI = "yes" }

TEMPLATE = subdirs

SUBDIRS += \
    core \
    cli \
    example \
    log

cli.depends = core
example.depends = core cli

equals(GUI,"yes") {
    SUBDIRS += gui
    gui.depends = core
    example.depends += gui
}
