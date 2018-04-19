
lessThan(QT_MAJOR_VERSION,5): error("Requires Qt 5")
lessThan(QT_MINOR_VERSION,7): error("Requires Qt 5.7")

TEMPLATE = subdirs

SUBDIRS += \
    core \
    #tests \
    #gui \
    console \
    example

#tests.depends = core
#gui.depends = core
console.depends = core
example.depends = core console
