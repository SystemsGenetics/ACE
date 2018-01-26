
set -e

INSTALL_PREFIX=/usr/local

if [ $1 == "--local" ]; then
	INSTALL_PREFIX=$HOME/software
	if [ ! -e $INSTALL_PREFIX/include ]; then
		mkdir -p $INSTALL_PREFIX/include
	fi
fi

rsync -a ./build/libs/ $INSTALL_PREFIX/lib/
rsync -a --include="core/" --include="/console" --include="/gui" --include="*.h" --exclude="*" ./src/ $INSTALL_PREFIX/include/ace/

