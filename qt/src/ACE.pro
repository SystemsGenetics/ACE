
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

# TODO(after release): change dataobject to NOT read file in constructor, but have open and openNew
# functions (where openNew replaces clear and open is for an existing data object)
