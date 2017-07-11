
lessThan(QT_MAJOR_VERSION,5): error("Requires Qt 5")

TEMPLATE = subdirs

SUBDIRS += \
    core \
    tests #\
    #gui \
    #example

tests.depends = core
#gui.depends = core
#example.depends = core gui

# make unit testing project, do simple unit test of exception class
