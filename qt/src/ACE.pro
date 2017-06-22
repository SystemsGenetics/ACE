
lessThan(QT_MAJOR_VERSION,5): error("Requires Qt 5")

TEMPLATE = subdirs

SUBDIRS += \
   core \
   gui \
   example

gui.depends = core
example.depends = core gui

# make OpenCLEvent NOTHROW exception class lolz(public EOpenCL)

# integrate metadata and its model into data object
