
lessThan(QT_MAJOR_VERSION,5): error("Requires Qt 5")

TEMPLATE = subdirs

SUBDIRS += \
    core \
    tests \
    gui \
    console \
    example

tests.depends = core
gui.depends = core
example.depends = core gui

#TODO: Add exception thrown passthrough for GUI for analytic thread
