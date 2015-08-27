**************************************************
*     .___  __    ________   ________
*   __| _/_/  |_  \_____  \  \_____  \
*  / __ | \   __\  /   |   \  /   |   \
* / /_/ |  |  |   /    |    \/    |    \
* \____ |  |__|   \_______  /\_______  /
*      \/                 \/         \/
* (d)esign (t)ool (O)bject-(O)riented
*
**************************************************

This is dtOO, a highly customizeable framework to design hydraulic machines.

Build dtOO
----------

Prerequisites of this library are:
- gmsh
- OpenCascade
- Root
- CGAL
- GSL
- Boost
- Qt5
- MeshKit
- MOAB
- openFOAM

The environment variables 

  EXTERNLIBS
  COVISEDIR

have to be set.

Additionally you need  openFOAM variables to find the libraries. Please 
take the correct openFOAM version for your OS.

Clone repository, create a build directory and run cmake:

  git clone git@bitbucket.org:atismer/dtoo.git
  cd dtOO
  mkdir buildDtOO
  cd buildDtOO
  cmake -DCMAKE_INSTALL_PREFIX=../install ..

The GUI needs an existing COVISE installation.

For building the GUI please set the environment variable DTOO_ROOTDIR:

  export DTOO_ROOTDIR={install directory of dtOO lib}

Create a build directory and run cmake

  cd dtOO
  mkdir buildCoviseGuiHeaven
  cd buildCoviseGuiHeaven
  cmake -DCMAKE_INSTALL_PREFIX=~/covise ../coviseGUIHeaven/

After successful compiling you can run COVISE with the GUI of the dtOO library
with the script.