"""
Create, simulate, and evaluate an axial runner. This machine is the test case
of `[Eyselein_2025] <https://doi.org/10.3390/en18030677>`_, 
`[Rentschler_2024] <https://doi.org/10.1002/pamm.202400126>`_, 
`[Raj_2024] <https://doi.org/10.1002/pamm.202400104>`_, and
`[Ebel_2024] <https://doi.org/10.48550/arXiv.2410.18358>`_. For the 
publication `[Ebel_2024] <https://doi.org/10.48550/arXiv.2410.18358>`_, the
data set is provided in 
`[axial_turbine_database] <https://doi.org/10.5281/zenodo.14014525>`_.
This GitHub repository serves as the database for this demonstration case.

Run this tutorial by executing:

.. code-block:: bash

  export OSLO_LOCK_PATH=/tmp && export FOAM_SIGFPE=0 \\
    && python3.12 -m doctest build.py

Import ``logging`` package and create a configuration:

>>> import logging
>>> logging.basicConfig(
...     format='%(asctime)s %(levelname)s : %(message)s',
...     datefmt='%Y-%m-%d %H:%M:%S',
...     level=logging.INFO
... )

Import necessary classes from ``dtOO``:

>>> from dtOOPythonSWIG import (
...   logMe,
...   dtXmlParser,
...   baseContainer,
...   labeledVectorHandlingConstValue,
...   labeledVectorHandlingAnalyticFunction,
...   labeledVectorHandlingAnalyticGeometry,
...   labeledVectorHandlingBoundedVolume,
...   labeledVectorHandlingDtCase,
...   labeledVectorHandlingDtPlugin,
...   lVHOstateHandler,
... )

Import packages from ``pyDtOO``:

>>> from pyDtOO import (
...   dtScalarDeveloping, 
...   dtForceDeveloping, 
...   dtDeveloping
... )
>>> from pyDtOO import dtClusteredSingletonState as stateCounter

Import `foamlib <https://github.com/gerlero/foamlib>`_ to control OpenFoam
within Python:

>>> import foamlib

Import other default packages, necessary for evaluation and system calls:

>>> import numpy as np
>>> import sys
>>> import os

Clone repository
`[axial_turbine_database] <https://doi.org/10.5281/zenodo.14014525>`_, if
necessary:

>>> clone = "git clone https://github.com/ihs-ustutt/axial_turbine_database.git" 
>>> if not os.path.isdir("./axial_turbine_database"):
...   logging.info("Clone repository.")
...   ret = os.system(clone)

Set up ``dtClusteredSingletonState`` configuration:

>>> stateCounter.PREFIX = 'T1'
>>> stateCounter.CASE = 'tistos_ru_of'
>>> stateCounter.DATADIR = './axial_turbine_database/runData'

Define the variables stored in the database:

>>> stateCounter.ADDDATA = [
...   'P', 
...   'dH', 
...   'eta', 
...   'VCav', 
...   'history', 
...   'islandID',
... ]

Define default constructors for variables:

>>> stateCounter.ADDDATADEF = [
...   {"tl": 0, "n": 0, "vl": 0}, # P
...   {"tl": 0, "n": 0, "vl": 0}, # dH
...   {"tl": 0, "n": 0, "vl": 0}, # eta
...   {"tl": 0, "n": 0, "vl": 0}, # VCav
...   {}, # history
...   -1, # islandID
... ]

Define mapping between database's parameters and :ref:`constValue` label. The
parameters in ``runData`` are in the same sequence as the mapping:

>>> cVArr = [
...   {'label': 'cV_ru_alpha_1_ex_0.0', 'min': -0.155, 'max': 0.025}, 
...   {'label': 'cV_ru_alpha_1_ex_0.5', 'min': -0.19, 'max': -0.01}, 
...   {'label': 'cV_ru_alpha_1_ex_1.0', 'min': -0.19, 'max': -0.01}, 
...   {'label': 'cV_ru_alpha_2_ex_0.0', 'min': -0.08, 'max': 0.1}, 
...   {'label': 'cV_ru_alpha_2_ex_0.5', 'min': -0.08, 'max': 0.1}, 
...   {'label': 'cV_ru_alpha_2_ex_1.0', 'min': -0.08, 'max': 0.07}, 
...   {'label': 'cV_ru_offsetM_ex_0.0', 'min': 1.0, 'max': 1.5},
...   {'label': 'cV_ru_offsetM_ex_0.5', 'min': 1.0, 'max': 1.5}, 
...   {'label': 'cV_ru_offsetM_ex_1.0', 'min': 1.0, 'max': 1.5}, 
...   {'label': 'cV_ru_ratio_0.0', 'min': 0.4, 'max': 0.6}, 
...   {'label': 'cV_ru_ratio_0.5', 'min': 0.4, 'max': 0.6}, 
...   {'label': 'cV_ru_ratio_1.0', 'min': 0.4, 'max': 0.6}, 
...   {'label': 'cV_ru_offsetPhiR_ex_0.0', 'min': -0.15, 'max': 0.15}, 
...   {'label': 'cV_ru_offsetPhiR_ex_0.5', 'min': -0.15, 'max': 0.15}, 
...   {'label': 'cV_ru_offsetPhiR_ex_1.0', 'min': -0.15, 'max': 0.15}, 
...   {'label': 'cV_ru_bladeLength_0.0', 'min': 0.4, 'max': 0.8}, 
...   {'label': 'cV_ru_bladeLength_0.5', 'min': 0.6, 'max': 1.0}, 
...   {'label': 'cV_ru_bladeLength_1.0', 'min': 0.8, 'max': 1.3}, 
...   {'label': 'cV_ru_t_le_a_0', 'min': 0.005, 'max': 0.06}, 
...   {'label': 'cV_ru_t_le_a_0.5', 'min': 0.005, 'max': 0.06},
...   {'label': 'cV_ru_t_le_a_1', 'min': 0.005, 'max': 0.06}, 
...   {'label': 'cV_ru_t_mid_a_0', 'min': 0.005, 'max': 0.06}, 
...   {'label': 'cV_ru_t_mid_a_0.5', 'min': 0.005, 'max': 0.06},
...   {'label': 'cV_ru_t_mid_a_1', 'min': 0.005, 'max': 0.06}, 
...   {'label': 'cV_ru_t_te_a_0', 'min': 0.005, 'max': 0.06}, 
...   {'label': 'cV_ru_t_te_a_0.5', 'min': 0.005, 'max': 0.06},
...   {'label': 'cV_ru_t_te_a_1', 'min': 0.005, 'max': 0.06}, 
...   {'label': 'cV_ru_u_mid_a_0', 'min': 0.4, 'max': 0.6}, 
...   {'label': 'cV_ru_u_mid_a_0.5', 'min': 0.4, 'max': 0.6},
...   {'label': 'cV_ru_u_mid_a_1', 'min': 0.4, 'max': 0.6},
... ]

Initialize a ``dtClusteredSingletonState`` object for state ``21260``:

>>> sc = stateCounter(21260)

Create log file:

>>> logMe.initLog('build-'+sc.state()+'.log')
'build-T1_21260.log'

Initialize ``XML`` parser with construction and save file:

>>> parser = dtXmlParser.init("machine.xml", "machineSave.xml").reference()

Parse ``XML`` files:

>>> parser.parse()

Create basic container:

>>> bC = baseContainer()
>>> cV = labeledVectorHandlingConstValue()
>>> aF = labeledVectorHandlingAnalyticFunction()
>>> aG = labeledVectorHandlingAnalyticGeometry()
>>> bV = labeledVectorHandlingBoundedVolume()
>>> dC = labeledVectorHandlingDtCase()
>>> dP = labeledVectorHandlingDtPlugin()

Create :ref:`constValue` instances and load the ``templateState``:

>>> parser.createConstValue(cV)
>>> parser.loadStateToConst("templateState", cV)

Iterate over all parameters from database and set the value in the 
correspoding :ref:`constValue` object. The array ``cVArr`` reuturns the
correct label:

>>> cc = 0
>>> for anObj in sc.objective():
...   cV[ cVArr[cc]['label'] ].setValue( anObj )
...   cc = cc + 1

Create geometry, apply the ``ru_adjustDomain`` plugin, and create geometry
again. The plugin adjusts the runner domain by changing the DOFs; therefore,
it necessary to create the geometry twice:

>>> parser.destroyAndCreate(bC, cV, aF, aG, bV, dC, dP)
>>> dP.get('ru_adjustDomain').apply()
>>> parser.destroyAndCreate(bC, cV, aF, aG, bV, dC, dP)

Make a state in the container of :ref:`constValue`:

>>> lVHOstateHandler().makeState(sc.state())

Define empty dicts for power, head, efficiency, and cavitation volume:

>>> P = {}
>>> dH = {}
>>> eta = {}
>>> Vcav = {}

Create geometry, perform simulation, and perform evaluation for each
operating point:

>>> for i in ["tl" , "n", "vl"]:
...   #
...   # Create OpenFoam case
...   #
...   dC.get('tistos_ru_of_'+i).runCurrentState()
...   #
...   # Initialize ``foamlib`` object; define parallel solution on 4 
...   # processors; simulate 100 iterations laminar with a ``faceLimited``
...   # scheme
...   #
...   fc = foamlib.FoamCase( 'tistos_ru_of_'+i+'_'+sc.state() )
...   fc.decompose_par_dict['method'] = 'scotch'
...   fc.decompose_par_dict['numberOfSubdomains'] = 4
...   fc.control_dict['writeInterval'] = 100
...   fc.control_dict['endTime'] = 100
...   fc.turbulence_properties["RAS"]["turbulence"] = False
...   fc.fv_schemes['gradSchemes']['none'] = 'faceLimited Gauss linear 0.33'
...   fc.fv_schemes['gradSchemes']['grad(p)'] = 'faceLimited Gauss linear 0.33'
...   fc.fv_schemes['gradSchemes']['grad(U)'] = 'faceLimited Gauss linear 0.33'
...   fc.fv_schemes['divSchemes']['div(phi,U)'] = \\
...     'Gauss linearUpwindV faceLimited Gauss linear 0.33'
...   fc.run()
...   #
...   # Simulate until 1000 iterations reached as a turbulent simulation with
...   # a ``cellLimited`` scheme
...   #
...   fc.control_dict['writeInterval'] = 1000
...   fc.control_dict['endTime'] = 1000
...   fc.turbulence_properties["RAS"]["turbulence"] = True
...   fc.fv_schemes['gradSchemes']['none'] = 'cellLimited Gauss linear 0.33'
...   fc.fv_schemes['gradSchemes']['grad(p)'] = 'cellLimited Gauss linear 0.33'
...   fc.fv_schemes['gradSchemes']['grad(U)'] = 'cellLimited Gauss linear 0.33'
...   fc.fv_schemes['divSchemes']['div(phi,U)'] = \\
...     'Gauss linearUpwindV cellLimited Gauss linear 0.33'
...   fc.run()
...   #
...   # Reconstruct the case
...   #
...   fc.reconstruct_par()
...   #
...   # Extract the rotation speed from the case
...   #
...   omega = np.abs(
...     foamlib.FoamFile(fc.path/'constant/MRFProperties')['MRF_RU']['omega']
...   )
...   try:
...     #
...     # Read data from ``postProcessing`` folder
...     #
...     Q_ru_dev = dtScalarDeveloping( 
...       dtDeveloping(str(fc.path/'postProcessing/Q_ru_in/100')).Read() 
...     )
...     pIn_ru_dev = dtScalarDeveloping( 
...       dtDeveloping(str(fc.path/'postProcessing/ptot_ru_in/100')).Read() 
...     )
...     pOut_ru_dev = dtScalarDeveloping( 
...       dtDeveloping(str(fc.path/'postProcessing/ptot_ru_out/100')).Read() 
...     )
...     Vcav_dev = dtScalarDeveloping( 
...       dtDeveloping(str(fc.path/'postProcessing/V_CAV/100')).Read() 
...     )
...     F_dev = dtForceDeveloping( 
...       dtDeveloping(str(fc.path/'postProcessing/forces')).Read(
...         {'force.dat' : ':,4:10', 'moment.dat' : ':,4:10', '*.*' : ''}
...       ) 
...     )
...     #
...     # Calculate power, head, efficiency, and cavitation volume; average
...     # simulation results over 100 iterations
...     #
...     P[i] = F_dev.MomentMeanLast(100)[2] * omega
...     dH[i] = (pOut_ru_dev.MeanLast(100) - pIn_ru_dev.MeanLast(100)) / 9.81
...     eta[i] = P[i] / (1000. * 9.81 * dH[i] * Q_ru_dev.MeanLast(100) )
...     Vcav[i] = Vcav_dev.MeanLast(100)
...     #
...     # Check if number of iterations reached and if it is a turbine not a
...     # pump
...     #
...     if fc[-1].name != '1000':
...       raise ValueError('Max number of iterations not reached.')
...     if np.abs(eta[i]) > 1:
...       raise ValueError('Pump detected.')
...   #
...   # Handle exceptions
...   #
...   except Exception as e:
...     logging.error('Catch exception : %s', e)

Compare simulation results with the results in the database. Accept an error of
1 %, otherwise report error:

>>> for valLab, valVar in zip(['P', 'dH', 'eta',],[P, dH, eta,]):
...   ref = sc.readDict(valLab)
...   for i in ['n', 'vl', 'tl',]:
...     diff = np.abs( (ref[i] - valVar[i]) / ref[i] )
...     logging.info("Difference for %s[%s] : %f %%" % (valLab, i, 100.*diff))
...     if diff > 0.01:
...       print("Difference for %s[%s] : %f %% > 1%%!" % (valLab, i, 100.*diff))
"""

if __name__ == "__main__":
    import doctest
    doctest.testmod()
