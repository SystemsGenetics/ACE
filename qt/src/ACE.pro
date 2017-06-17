
lessThan(QT_MAJOR_VERSION,5): error("Requires Qt 5")

TEMPLATE = subdirs

SUBDIRS += \
   core \
   gui \
   example

gui.depends = core
example.depends = core gui

# make OpenCLEvent NOTHROW exception class lolz(public EOpenCL)

# change qpixmap in edatastream to bytearray so it only needs qtcore, add load/save for metadata
# using edatastream
