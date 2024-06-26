Quickstart
==========

The fastest way of trying dtOO is by using the provided image:

.. code-block:: bash

  docker pull atismer/dtoo:stable

You can simply run the container by executing:

.. code-block:: bash

  docker run -it atismer/dtoo:stable

Within the container navigate to the demo case:

.. code-block:: bash

  cd /dtOO/demo/simpleAxialRunner

Simulate the machine as a ``doctest`` in the container by:

.. code-block:: python

  python3.11 -m doctest simpleAxialRunner.py

The geometry, mesh and simulation case is automatically created. Additionally,
the simulation is performed. After a few minutes, you will see a folder named
``of_DD-MM-YYYY_HH:MM:SS`` that is the correspoding ``OpenFOAM`` case. ``D``, 
``M``, ``Y``, ``H``, ``M`` and ``S`` stands for, respectively, day, month, 
year, hours, minutes and seconds.

Visualization
-------------

.. note::
  Since 
  `[dtOO-ThirdParty@7ec5a5a] <https://github.com/ihs-ustutt/dtOO-ThirdParty/commit/7ec5a5af8caa8e26ab2f63c76d8482eeb8c0c3ff>`_
  and `[dtOO@07fc88] <https://github.com/ihs-ustutt/dtOO/commit/07fcd88e86e42123a17c692071eafaf4c8c96742>`_
  the framework in the container is compiled with Python 3.11. Currently, 
  the openSUSE Leap respositories provide ``paraview`` only compiled with 
  Python 3.6. Then, it is not possible to use the framework in ``paraview``'s 
  ``Python Shell``. Therefore, it is necessary to compile ``paraview`` in the 
  container, too. This has to be done locally, because the compile time is very 
  time consuming. A prepared input file for ``docker`` is provided in the 
  repository. The container can be build by:

  .. code-block:: bash

    docker \
      build \
      --build-arg NCPU=8 \
      -t dtoo-gui \
      . \
      -f Dockerfile.quickstart

If you would like to see the machine, simply open ``paraview`` in the 
container. Therefore, you have to execute on the host

.. code-block:: bash

  xhost +local:*

before running the container by:

.. code-block:: bash

  docker run \
    -it \
    -e DISPLAY=$DISPLAY \
    -v /tmp/.X11-unix/:/tmp/.X11-unix/ \
    dtoo-gui

Within the container navigate to the demo case:

.. code-block:: bash

  cd /dtOO/demo/simpleAxialRunner

You can open ``paraview`` and investigate the geometry. Adjust ``PYTHONPATH`` 
and open ``paraview``  by:

.. code-block:: bash

  export PYTHONPATH=/dtOO/demo/simpleAxialRunner:$PYTHONPATH
  paraview

Then, just open the ``Python Shell`` and type:

.. code-block:: python

  from simpleAxialRunner import *
  CreateAndShow()

You will see this:

.. image:: img/quickstart_paraview.png

As described above, you can also simulate the machine and postprocess the
result with ``paraview``.
