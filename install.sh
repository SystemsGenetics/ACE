
set -e

INSTALL_PREFIX=/usr/local

if [ $1 == "--local" ]; then
	INSTALL_PREFIX=$HOME/software
fi

rsync -a ./build/libs/ $INSTALL_PREFIX/lib/
rsync -a --include="core/" --include="/console" --include="/gui" --include="*.h" --exclude="*" ./src/ $INSTALL_PREFIX/include/ace/

