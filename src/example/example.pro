
isEmpty(GUI) { GUI = "yes" }

TEMPLATE = subdirs

SUBDIRS += \
    core \
    cli

cli.depends = core

equals(GUI,"yes") {
    SUBDIRS += gui
    gui.depends = core
}
