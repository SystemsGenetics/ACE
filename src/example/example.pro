
TEMPLATE = subdirs

SUBDIRS += \
    core \
    cli \
    gui

cli.depends = core
gui.depends = core
