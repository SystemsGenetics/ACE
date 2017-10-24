
set -e

rsync -a ./build/libs/ /usr/local/lib/
rsync -a --include="core/" --include="/console" --include="/gui" --include="*.h" --exclude="*" ./src/ /usr/include/ace/

