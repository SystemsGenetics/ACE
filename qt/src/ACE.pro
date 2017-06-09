
lessThan(QT_MAJOR_VERSION,5): error("Requires Qt 5")

TEMPLATE = subdirs

SUBDIRS += \
   core \
   gui \
   example

gui.depends = core
example.depends = core gui

# TODO: remove MetadataArray and Object(can just be QList or QMap), rename MetadataValue to Metadata
#       and refactor code for objects and arrays... also make it so it can copy itself
#       (but no moving)
