
lessThan(QT_MAJOR_VERSION,5): error("Requires Qt 5")
lessThan(QT_MINOR_VERSION,7): error("Requires Qt 5.7")

isEmpty(GUI) { GUI = "yes" }

TEMPLATE = subdirs

SUBDIRS += \
    core \
    #tests \
    console \
    example

#tests.depends = core
console.depends = core
example.depends = core console

equals(GUI,"yes") {
    SUBDIRS += gui
    gui.depends = core
}
