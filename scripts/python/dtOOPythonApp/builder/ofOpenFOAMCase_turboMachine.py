from dtOOPythonApp.tools.dtBundleTools import dtBundleBuilder

from dtOOPythonSWIG import ofOpenFOAMCase
from dtOOPythonSWIG import jsonPrimitive
from dtOOPythonSWIG import boundedVolume
from dtOOPythonSWIG import vectorStr
from dtOOPythonSWIG import dtVector3
from dtOOPythonSWIG import dtPoint3
from dtOOPythonSWIG import labeledVectorHandlingBoundedVolume
from dtOOPythonSWIG import labeledVectorHandlingConstValue

from typing import List
import logging

class ofOpenFOAMCase_turboMachine(dtBundleBuilder):
  """Create simulation case for a turbomachine.

  The setup for a simulation case heavily depends on the class
  ofOpenFOAMCase_setupWrapper. Therefore, importing the class is recomended:

  ```python
  from ofOpenFOAMCase_setupWrapper import ofOpenFOAMCase_setupWrapper
  ```

  Basically, a new OpenFOAM case is created by calling
  
  ```python
  container = ofOpenFOAMCase_turboMachine(
    <arguments>
  ).buildExtract( container )
  ```
  with appropriate arguments that are described in the following. The 
  `container` argument is an instance of dtOOPythonSWIG.dtBundle.

  The `label` parameter defines the name of the case, e.g.:

  ```python
  label = "of_gvru"
  ```
  
  The `bVs` argument defines the instances of dtOOPythonSWIG.boundedVolume
  that are necessary for the creation of the dtOOPythonSWIG.dtCase object, 
  e.g.:

  ```python
  bVs = [ bV["gv"], bV["ru"], ]
  ```
  
  The `dictRule` argument handles all OpenFOAM dicts that are necessary for
  the simulation case. The dicts are created by the 
  ofOpenFOAMCase_setupWrapper class and simply added. Each dict can be
  adjusted, see documentation of ofOpenFOAMCase_setupWrapper. 

  ```python
  dictRule = \
      ofOpenFOAMCase_setupWrapper.controlDict(
        application = "simpleFoam",
        endTime = 1000,
        QPatches = ['ru_inlet','ru_outlet',],
        PTPatches = ['ru_inlet', 'ru_outlet',],
        FPatches = ['ru_blade',],
        libs = [
          "libsimpleFunctionObjects.so",
          "libsimpleSwakFunctionObjects.so",
          "libmappedFieldFixedValue.so",
          "libmixingPlane.so",
        ]
      )
    + ofOpenFOAMCase_setupWrapper.decomposeParDict(8, 'metis')
    + ofOpenFOAMCase_setupWrapper.fvSchemes()
    + ofOpenFOAMCase_setupWrapper.fvSolution()
    + ofOpenFOAMCase_setupWrapper.transportModel()
    + ofOpenFOAMCase_setupWrapper.turbulenceProperties()
    + ofOpenFOAMCase_setupWrapper.MRFProperties(
      ['ru',], [-9.81,], [['ru_inlet', 'ru_outlet'],]
    )
  ```
  
  The fields are iniitialized according to the `fieldRules` argument. Each
  initialization can be customized, see documentation in 
  ofOpenFOAMCase_setupWrapper. For a turbulent simulation the initialization
  can be done by:

  ```python
  fieldRules = [
    ofOpenFOAMCase_setupWrapper.fieldRuleString( "U", [0.0,0.0,0.0,] ),
    ofOpenFOAMCase_setupWrapper.fieldRuleString( "p", [0.0,] ),
    ofOpenFOAMCase_setupWrapper.fieldRuleString( "k", [0.1,] ),
    ofOpenFOAMCase_setupWrapper.fieldRuleString( "omega", [0.1,] ),
    ofOpenFOAMCase_setupWrapper.fieldRuleString("nut", [0.1,]),
  ]
  ```

  Boundary conditions for the simulation case can be applied with the
  `setupRules` argument. Each rule is basically a boundary condition on a
  single patch or between coupled patches. The rules can be customized
  depending on their inputs, see documentation in ofOpenFOAMCase_setupWrapper.
  In the following example typical boundary conditions are applied for a
  turbomachine simulation:

  ```python
  setupRules = [
    ofOpenFOAMCase_setupWrapper.emptyRuleString(),
    ofOpenFOAMCase_setupWrapper.inletRuleString(
      "gv_inlet", 
      ["U", "p", "k", "omega",], [[10.],[0], [0.0, 0.10], [1]]
    ),
    ofOpenFOAMCase_setupWrapper.outletRuleString(
      "ru_outlet", 
      ["U", "p", "k", "omega",]
    ),
    ofOpenFOAMCase_setupWrapper.wallRuleString(
      "gv_shroud", 
      ["omega", "U", "p", "k", "nut"]
    ),
    ofOpenFOAMCase_setupWrapper.wallRuleString(
      "ru_shroud", 
      ["omega", "U", "p", "k", "nut"]
    ),
    ofOpenFOAMCase_setupWrapper.wallRuleString(
      "gv_hub", 
      ["omega", "U", "p", "k", "nut"]
    ),
    ofOpenFOAMCase_setupWrapper.wallRuleString(
      "ru_hub", 
      ["omega", "U", "p", "k", "nut"]
    ),
    ofOpenFOAMCase_setupWrapper.wallRuleString(
      "gv_blade", 
      ["omega", "U", "p", "k", "nut"]
    ),
    ofOpenFOAMCase_setupWrapper.wallRuleString(
      "ru_blade", 
      ["omega", "U", "p", "k", "nut"]
    ),
    ofOpenFOAMCase_setupWrapper.cyclicAmiRuleString(
      "gv_pressure", "gv_suction"
    ),
    ofOpenFOAMCase_setupWrapper.cyclicAmiRuleString(
      "ru_pressure", "ru_suction"
    ),
    ofOpenFOAMCase_setupWrapper.mixingPlaneRuleString(
      patchOne = "gv_outlet", 
      patchTwo = "ru_inlet",
      variables = ["U", "p", "k", "omega",]
    )
  ]
  ```
  
  Attributes
  ----------
  label_: str
    Label.
  bVs_: List[ boundedVolume ]
    BoundedVolumes belonging to this dtCase.
  dictRule_: str
    Dicts for OpenFOAM.
  fieldRules_: List[ str ]
    Fields' initialization rules.
  setupRules_: List[ str ]
    Boundary condition given as setup rules.

  See Also
  --------

  ofOpenFOAMCase_setupWrapper : Setup wrapper.
  """
  def __init__( 
    self, 
    label: str,
    bVs: List[boundedVolume],
    dictRule: str,
    fieldRules: List[str],
    setupRules: List[str]
  ) -> 'ofOpenFOAMCase_turboMachine':
    """Constructor.
  
    Parameters
    ----------
  
    label: str
      Label.
    bVs: List[ boundedVolume ]
      BoundedVolumes belonging to this dtCase.
    dictRule: str
      Dicts for OpenFOAM.
    fieldRules: List[ str ]
      Fields' initialization rules.
    setupRules: List[ str ]
      Boundary condition given as setup rules.
  
    Returns
    -------
    None
  
    """
    logging.info( "Initializing %s ..." % (label) )
    super(ofOpenFOAMCase_turboMachine, self).__init__()
    self.label_ = label
    self.bVs_ = bVs
    self.dictRule_ = dictRule
    self.fieldRules_ = fieldRules
    self.setupRules_ = setupRules

  def build(self) -> None:
    """Build part.

    Parameters
    ----------
    None

    Returns
    -------
    None

    """
    logging.info( "Building %s ..." % (self.label_) )


    lVH_bV = labeledVectorHandlingBoundedVolume()
    for bV in self.bVs_:
      lVH_bV.append( bV )

    ref = ofOpenFOAMCase()
    ref.thisown = False
    ref.setLabel( self.label_ )  
    jE = jsonPrimitive(
      '{'
        '"option" : ['
          '{ "name" : "keepOrientation", "value" : "true"}'
        ']'
      '}'
    )
    jE.appendVectorBoundedVolume("", lVH_bV)
    jE.appendStr(
      "_dictRule",
      self.dictRule_
    )
    jE.appendVectorStr(
      "_fieldRule",
      vectorStr(
        self.fieldRules_
      )
    )
    jE.appendVectorStr(
      "_setupRule",
      vectorStr(
        self.setupRules_
      )
    )
 
    ref.jInit(
      jE, None, None, None, None, lVH_bV, None
    )
    self.appendDtCase( ref )

    return
