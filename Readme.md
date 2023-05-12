# (d)esign (t)ool (O)bject-(O)riented

This is dtOO, a highly customizeable framework to design hydraulic machines.

Prerequisites in openSUSE Leap of the dependencies and `dtOO` can be installed 
by
```
zypper install git cmake gcc gcc-c++ make automake cmake-gui \
  autogen autoconf libtool gcc-fortran \
  libQt5Core-devel libQt5Xml-devel gsl-devel \
  libboost_system1_66_0-devel libboost_timer1_66_0-devel \
  libboost_filesystem1_66_0-devel \
  libboost_program_options1_66_0-devel \
  libboost_regex1_66_0-devel libboost_thread1_66_0-devel \
  libboost_python-py3-1_66_0-devel python3-devel
```

## Compile external libraries
Before building `dtOO` the prerequisites have to be built. 
It is recommended to put all shared objects and binaries in one 
directory (`<dir>` in the following).
One way is to execute
```
sh buildDep -i <path> -n 2 -o all
```
in the cloned repository.
This will create a `ThirdParty` directory and provide dependencies for a 
minimal installation of `dtOO`.
If it is working continue [here](#Build).
If this is not working, you have to install all dependencies manually as 
described in the following.

## OpenFOAM

Some parts of the system need an installation of OpenFOAM (OpenCFD Ltd). If you
have an installation, please initialize before building.

## foamXdogBall

Some part of the system and also of OpenFOAM require libraries from 
[foamXdogBall](https://github.tik.uni-stuttgart.de/atismer/foamXdogBall). If you
have an installation, then set the environment variable `FOAMXDOGBALL_DIR` to
the source files' location.

## Build
The environment variable `DTOO_EXTERNLIBS` defines an additional search path, 
e.g.

```bash
export DTOO_EXTERNLIBS=<dir>
```

for the required prerequisites.

Initialize and update the submodules with

```bash
git submodule init
git submodule update
```

and create a build directory before configuring with `cmake`:

```bash
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=<path> -DCMAKE_BUILD_TYPE=RelWithDebInfo ..
```

After successful compiling the binaries and libraries of `dtOO` should be added:

```
export PATH=<path>:${PATH}
export LD_LIBRARY_PATH=<path>/lib:<path>/lib64:${LD_LIBRARY_PATH}
```

The design system `dtOO` has additional options for building, e.g.:

```bash
DTOO_BUILD_DOC
  - Build documentation with Doxygen

DTOO_USE_PYTHONOCC
  - Use PythonOCC (Python interface for OCC), necessary for handling of OCC
    types in Python
```

### GUI

For the GUI an existing `COVISE` installation is necessary. The source code and
build instructions are available [here](https://github.com/hlrs-vis/covise)

Create a build directory and run `cmake`

```bash
cd dtOO
mkdir buildCoviseGui
cd buildCoviseGui
cmake -DCMAKE_INSTALL_PREFIX=~/covise ../coviseGUIHeaven/ -DDTOO_ROOTDIR=<dir>
```

After successful compiling and adding the path `~/covise` to `COVISE` the 
`dtOODesignTool` is accessable within `COVISE`.
