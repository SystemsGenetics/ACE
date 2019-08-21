
lessThan(QT_MAJOR_VERSION,5): error("Requires Qt 5")
lessThan(QT_MINOR_VERSION,7): error("Requires Qt 5.7")

isEmpty(GUI) { GUI = "yes" }

TEMPLATE = subdirs

SUBDIRS += \
    core \
    #tests \
    cli \
    example \
    log

#tests.depends = core
cli.depends = core
example.depends = core cli

equals(GUI,"yes") {
    SUBDIRS += gui
    gui.depends = core
    example.depends += gui
}
