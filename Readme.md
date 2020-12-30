# (d)esign (t)ool (O)bject-(O)riented

This is dtOO, a highly customizeable framework to design hydraulic machines.

## Compile external libraries
Before building `dtOO` the prerequisites have to be installed or built. It is recomended to put all shared objects and binaries in one directory (`<dir>` in the following).

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

###gmsh

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

Prerequisites of `dtOO` are
- gmsh
- OpenCascade
- Root
- CGAL
- GSL
- Boost
- Qt5
- MeshKit
- MOAB
- muParser
- cgm
- foam-ext-3.1

The environment variable `DTOO_EXTERNLIBS` defines an additional search path for the required prerequisites. Additionally, initialize the `foam-ext-3.1` environment to find the libraries.

Clone repository, create a build directory and run cmake:
```bash
git clone git@github.tik.uni-stuttgart.de:atismer/dtOO.git
cd dtOO
git submodule init
git submodule update
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=../install -DDTOO_EXTERNLIBS=<dir> ..
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
