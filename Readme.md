# (d)esign (t)ool (O)bject-(O)riented

This is dtOO, a highly customizeable framework to design hydraulic machines.

Prerequisites in openSUSE Leap 15.2 of the dependencies and `dtOO` can be installed by
```
zypper addrepo https://download.opensuse.org/repositories/science/openSUSE_Leap_15.1/science.repo
zypper refresh
zypper install git cmake gcc gcc-c++ make automake cmake-gui \
  oce oce-devel autogen autoconf libtool cgns cgns-devel \
  muparser-devel libCGAL13 cgal-devel gcc-fortran \
  libQt5Core-devel libQt5Xml-devel gsl-devel \
  libboost_system1_66_0-devel libboost_timer1_66_0-devel \
  libboost_filesystem1_66_0-devel \
  libboost_program_options1_66_0-devel \
  libboost_regex1_66_0-devel libboost_thread1_66_0-devel \
  libboost_python-py3-1_66_0-devel python3-devel \
  root6 root6-devel
```

## Compile external libraries
Before building `dtOO` the prerequisites have to be built. 
It is recommended to put all shared objects and binaries in one directory (`<dir>` in the following).
One way is to execute
```
sh buildDep -i <path> -n 2 -o all
```
in the cloned repository.
This will create a `ThirdParty` directory and provide dependencies for a minimal installation of `dtOO`.
If it is working continue [here](#Build).
If this is not working, you have to install all dependencies manually as described in the following.

### cgm

```bash
git clone https://bitbucket.org/fathomteam/cgm.git
autoreconf -i
./configure --prefix=<dir> --enable-debug \
  --with-occ=/usr/ --enable-shared=yes \
  --enable-static=no
make install
```

### moab

Create a symbolic link `lib -> lib64` in the installation directory of [cgm](#cgm), otherwise moab will not find the installation.

```bash
git clone https://bitbucket.org/fathomteam/moab.git
autoreconf -i
./configure --exec-prefix=<dir> --prefix=<dir> \
  --with-cgns=/usr/lib --enable-shared=yes \
  --enable-static=no --enable-debug --with-cgm=<dir>
make install
```

### OpenMesh

```bash
git clone https://github.com/esummers/OpenMesh.git
cd OpenMesh
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=<dir> ..
make install
```

### OpenVolumeMesh

```bash
git clone https://github.com/nTopology/OpenVolumeMesh.git
cd OpenVolumeMesh
mkdir build_15.2
cd build_15.2
cmake -DCMAKE_INSTALL_PREFIX=<pwd> \
  -DCMAKE_BUILD_TYPE=Release ..
make install
```

### gmsh

It is necessary to modify the source code of gmsh. First check out the source code
```bash
git clone https://gitlab.onelab.info/gmsh/gmsh.git
```
and change directory
```
cd gmsh
```

Modify `CMakeLists.txt` to include headers `Parser/*.h` in `API`:
```diff
message(WARNING "The private API is unsupported and undocumented. It is meant "
       "for expert Gmsh developers, not for regular Gmsh users, who should rely "
       "on the stable public API (gmsh/api) instead.")
-  file(GLOB_RECURSE HEADERS Common/*.h Numeric/*.h Geo/*.h Mesh/*.h Solver/*.h
+  file(GLOB_RECURSE HEADERS Parser/*.h Common/*.h Numeric/*.h Geo/*.h Mesh/*.h Solver/*.h
     Post/*.h Plugin/*.h Graphics/*.h contrib/kbipack/*.h
```

Add in `Geo/GEntity.h` the function `setModel`:
```diff
   // the model owning this entity
   GModel *model() const { return _model; }
+  void setModel( GModel * model ) { _model = model; }
```

Add in `Geo/MVertex.h` the function `setNum`
```diff
   // get the immutab vertex number
   std::size_t getNum() const { return _num; }
+  inline void setNum(int const num) { _num = num; }
```

Remove the `static` keyword from the function `copyMesh` in `Mesh/meshGEdge.cpp`
```diff
-static void copyMesh(GEdge *from, GEdge *to, int direction)
+void copyMesh(GEdge *from, GEdge *to, int direction)
```

and also in `Mesh/meshGFace.cpp`
```diff
-static void copyMesh(GFace *source, GFace *target)
+void copyMesh(GFace *source, GFace *target)
```

Compile gmsh after changing the source:
```bash
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=<dir> \
  -DENABLE_BUILD_SHARED=ON \
  -DENABLE_PRIVATE_API=ON ..
```

### foam-ext-3.1

A detailed description of compiling foam-ext can be found [here](https://openfoamwiki.net/index.php/Installation/Linux/foam-extend-3.1). Besides foam-ext also the `openfoam-extend-Breeder1.6-OSIG-TurboMachinery` library is necessary.

```bash
git clone https://github.com/Unofficial-Extend-Project-Mirror/openfoam-extend-foam-extend-3.1.git
cd foam-extend-3.1
mkdir site
cd site
git clone https://github.com/Unofficial-Extend-Project-Mirror/openfoam-extend-Breeder1.6-OSIG-TurboMachinery
```

If there are problems with `flex` it is maybe necessary to modify the source code. Please execute
```bash
find src applications -name "*.L" -type f | xargs sed -i -e \
  's=\(YY\_FLEX\_SUBMINOR\_VERSION\)=YY_FLEX_MINOR_VERSION < 6 \&\& \1='
```
in your `foam-ext` directory.

## Build

The environment variable `DTOO_EXTERNLIBS` defines an additional search path, e.g.

```bash
export DTOO_EXTERNLIBS=<dir>
```

for the required prerequisites. If available, initialize the `foam-ext-3.1` environment to find the libraries.

Initialize and update the submodules with

```bash
git submodule init
git submodule update
```

and create a build directory before configuring with `cmake`:

```bash
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=../install ..
```

After successful compiling the binaries and libraries of `dtOO` should be added:

```
export PATH=<path>:${PATH}
export LD_LIBRARY_PATH=<path>/lib:<path>/lib64:${LD_LIBRARY_PATH}
```

### GUI

For the GUI an existing `COVISE` installation is necessary. The source code and build instructions are available [here](https://github.com/hlrs-vis/covise)

Create a build directory and run `cmake`

```bash
cd dtOO
mkdir buildCoviseGui
cd buildCoviseGui
cmake -DCMAKE_INSTALL_PREFIX=~/covise ../coviseGUIHeaven/ -DDTOO_ROOTDIR=<dir>
```

After successful compiling and adding the path `~/covise` to `COVISE` the `dtOODesignTool` is accessable within `COVISE`.