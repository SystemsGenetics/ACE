
lessThan(QT_MAJOR_VERSION,5): error("Requires Qt 5")

TEMPLATE = subdirs

SUBDIRS += \
   core \
   gui \
   example

gui.depends = core
example.depends = core gui
