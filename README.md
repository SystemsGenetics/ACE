# Accelerated Computational Engine (ACE)

ACE is a generic framework for creating GPU-accelerated applications.

## Installation

To build ACE:
```
cd build
qmake ../src
make
cd ..
```

To install ACE globally (requires root):
```
./install.sh
```

To install ACE locally:
```
./install.sh --local

# append these lines to ~/.bashrc
export INSTALL_PREFIX=$HOME/software
export CPLUS_INCLUDE_PATH=$CPLUS_INCLUDE_PATH:$INSTALL_PREFIX/include
export LIBRARY_PATH=$LIBRARY_PATH:$INSTALL_PREFIX/lib
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$INSTALL_PREFIX/lib
```

## Usage

For a example use of ACE, run the `example` application after building:
```
build/example/example
```
