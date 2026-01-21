.. Example documentation master file, created by
   sphinx-quickstart on Sat Sep 23 20:35:12 2023.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.


Welcome to dtOO's documentation!
================================

I want this sentence here!

dtOO is a scientific C++ library with a Python interface for designing
hydraulic machines. It is built based on the idea of providing a 
toolbox for geometry generation, mesh generation and simulation setup 
generation within one tool specifically dedicated to hydraulic machinery. 

A parameterized machine in dtOO can be automatically optimized within any
optimization toolbox. The framework was successfully tested for optimizing
a kinetic turbine using OpenFOAM and pygmo.

If you are using dtOO as part of your research, teaching, or other 
activities, we would be grateful if you could star the repository and/or 
cite our work. For citation purposes, you can use the following BibTex 
entry, which refers to the correspoding 
`PhD thesis <https://d-nb.info/1220692778/34>`_

.. code-block:: bibtex

 @PhdThesis {Tismer_2020,
    author = {Tismer, Alexander},
    title = {Entwicklung einer Softwareumgebung zur automatischen Auslegung von hydraulischen Maschinen mit dem Inselmodell},
    school = {Universität Stuttgart},
    year = {2020},
    month = {05},
    isbn = {978-3-948328-01-6},
    publisher = {Universität Stuttgart, Institut für Strömungsmechanik und Hydraulische Strömungsmaschinen},
    series = {Institutsmitteilung},
    url = {https://d-nb.info/1220692778/34},
 }


.. toctree::
   :maxdepth: 2
   :caption: Contents:
  
   quickstart.rst
   demonstration.rst
   test.rst
   api.rst
   Back to GitHub <https://github.com/ihs-ustutt/dtOO>
