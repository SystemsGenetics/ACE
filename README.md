# Accelerated Computational Engine (ACE)

ACE is a generic framework for creating GPU-accelerated applications.

## Installation

To build ACE:
```
cd build
qmake ../src/ACE.pro
make qmake_all
make
make qmake_all
```

To install ACE globally (requires root):
```
make install
```

To build & install ACE locally:
```
cd build
qmake ../src/ACE.pro PREFIX=$HOME/software
make qmake_all
make
make qmake_all
make install

# append these lines to ~/.bashrc
export PATH=$PATH:$HOME/software/bin
export CPLUS_INCLUDE_PATH=$CPLUS_INCLUDE_PATH:$HOME/software/include
export LIBRARY_PATH=$LIBRARY_PATH:$HOME/software/lib
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$HOME/software/lib
```

## Usage

For a example use of ACE, run the `aceex` or `qaceex` application after building.
