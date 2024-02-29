from dtOOPythonSWIG import dtVector3
from dtOOPythonSWIG import dtPoint3

from typing import List#, Tuple, Union, Dict
import logging

class ofOpenFOAMCase_setupWrapper:
  """Wrapper to create dicts and rules.

  Class creates the inputs for dtOOs' setupRules.

  Attributes
  ----------
  None

  """
  def __init__() -> 'ofOpenFOAMCase_setupWrapper' : 
    """Constructor.

    Parameters
    ----------
    None

    Returns
    -------
    None

    """
    pass

  @staticmethod
  def controlDict(
    application: str  = "simpleFoam",
    endTime: int = 1000,
    QPatches: List[ str ] = [],
    PTPatches: List[ str ] = [],
    FPatches: List[ str ] = [],
    libs: List[str] = []
  ) -> str:
    """Create controlDict.

    Parameters
    ----------
    application: str
      Application name.
    endTime: int
      endTime for simulation.
    QPatches: List[ str ]
      Patches to track volume flow.
    PTPatches: List[ str ]
      Patches to track total pressure.
    FPatches: List[ str ]
      Patches to track forces.
    libs: List[ str ]
      Libraries to be loaded within controlDict.

    Returns
    -------
    str
      controlDictRule.
    """

    retStr = str(
      'controlDict {'
      '  dtOOFileName      \"system/controlDict\";\n' 
      '  application       '+application+';\n'
      '  startFrom         latestTime;\n'
      '  startTime         0;\n'
      '  stopAt            endTime;\n'
      '  endTime           '+str(endTime)+';\n'
      '  deltaT            1;\n'
      '  writeInterval     '+str(endTime)+';\n'
      '  runTimeModifiable yes;\n'
      '  writeFormat       binary;\n'
      '  functions {\n'
    )
    for QPatch in QPatches:
      retStr += str(
        '    Q_'+QPatch+' {\n'
        '      type swakExpression ;\n'
        '      valueType patch ;\n'
        '      verbose true ;\n'
        '      patchName '+QPatch+' ;\n'
        '      expression \" phi \" ;\n'
        '      accumulations ( sum ) ;\n'
        '      outputInterval 1 ;\n'
        '    }\n'
      )
    for PTPatch in PTPatches:
      retStr += str(
        '    PT_'+PTPatch+' {\n'
        '      type swakExpression ;\n'
        '      valueType patch ;\n'
        '      verbose true ;\n'
        '      patchName '+PTPatch+' ;\n'
        '      expression \" (p+.5*magSqr(U)) * phi / sum( phi ) \" ;\n'
        '      accumulations ( sum ) ;\n'
        '      outputInterval 1 ;\n'
        '    }\n'
      )
    for FPatch in FPatches:
      retStr += str(
        '    forces {\n'
        '      type forces ;\n'
        '      libs ( \"libforces.so\" );\n'
        '      outputControl timeStep ;\n'
        '      outputInterval 1 ;\n'
        '      patches ( '+FPatch+' ) ;\n'
        '      rho rhoInf ;\n'
        '      log true ;\n'
        '      rhoInf 997 ;\n'
        '      CofR ( 0 0 0 ) ;\n'
        '    }\n'
      )
    retStr += str(
      '    convertToCylindrical {\n'
      '      type            fieldCoordinateSystemTransform;\n'
      '      libs            (fieldFunctionObjects);\n'
      '      fields          (U);\n'
      '      coordinateSystem {\n'
      '        type            cylindrical;\n'
      '        origin          (0 0 0);\n'
      '        rotation {\n'
      '          type            cylindrical;\n'
      '          axis            (0 0 1);\n'
      '        }\n'
      '      }\n'
      '      writeControl    writeTime;\n'
      '    }\n'
      '    yPlus1 {\n'
      '      type            yPlus;\n'
      '      libs            (fieldFunctionObjects);\n'
      '      writeControl    writeTime;\n'
      '    }\n'
      '    wallShearStress {\n'
      '      type            wallShearStress;\n'
      '      libs            (fieldFunctionObjects);\n'
      '      patches         ();\n'
      '      writeControl    writeTime;\n'
      '    }\n'
      '  };\n'
    )
    retStr += str(
      #'  mappedFieldFixedValue {'
      #'    pathToData      \"../boundaryData_RU_INLET/'+ii+'/boundaryData\";'
      #'  };'
    )
    retStr += str(
      '  libs ('
      )
    for lib in libs:
      retStr += str(
        '    \"'+lib+'\"'
      )
    retStr += str(
      '  );'
      '}'
    )
    return retStr

  @staticmethod
  def fvSolution():
    """Create fvSolution.

    Parameters
    ----------
    None

    Returns
    -------
    str
      fvSolutionRule.

    """
    return str(
      'fvSolution {'
      '  dtOOFileName        \"system/fvSolution\";'
      '  solvers'
      '  {'
      '    p'
      '    {'
      '      solver           PCG;'
      '      preconditioner   FDIC;'
      '      tolerance        1e-06;'
      '      relTol           0.1;'
      '    };'
      '    U'
      '    {'
      '      solver           PBiCGStab;'
      '      preconditioner   DILU;'
      '      tolerance        1e-08;'
      '      relTol           0.0;'
      '    };'
      '    k'
      '    {'
      '      solver           PBiCGStab;'
      '      preconditioner   DILU;'
      '      tolerance        1e-08;'
      '      relTol           0.0;'
      '    };'
      '    omega'
      '    {'
      '      solver           PBiCGStab;'
      '      preconditioner   DILU;'
      '      tolerance        1e-08;'
      '      relTol           0.0;'
      '    };'
      '  }'
      '  relaxationFactors'
      '  {'
      '    p               0.3;'
      '    U               0.7;'
      '    k               0.7;'
      '    epsilon         0.7;'
      '    omega           0.7;'
      '  }'
      '  SIMPLE'
      '  {'
      '    nNonOrthogonalCorrectors 2;'
      '  }'
      '}'
    )

  @staticmethod
  def fvSchemes():
    """Create fvSchemes.

    Parameters
    ----------
    None

    Returns
    -------
    str
      fvSchemesRule.
    
    """   

    return str(
      'fvSchemes {'
      '  dtOOFileName        \"system/fvSchemes\";'
      '  ddtSchemes'
      '  {'
      '    default steadyState;'
      '  }'
      '  gradSchemes'
      '  {'
      '    default         cellLimited Gauss linear 0.33;'
      '    grad(p)         cellLimited Gauss linear 0.33;'
      '    grad(U)         cellLimited Gauss linear 0.33;'
      '  }'
      '  divSchemes'
      '  {'
      '    default         none;'
      '    div(phi,U)      Gauss linearUpwindV cellLimited Gauss linear 0.33;'
      '    div(phi,k)      Gauss upwind;'
      '    div(phi,omega)  Gauss upwind;'
      '    div((nuEff*dev(grad(U).T()))) Gauss linear;'
      '    div((nuEff*dev2(T(grad(U))))) Gauss linear;'
      '  }'
      '  laplacianSchemes'
      '  {'
      '    default Gauss linear limited 0.333;'
      '  }'
      '  snGradSchemes'
      '  {'
      '    default limited 0.333;'
      '  }'
      '  interpolationSchemes'
      '  {'
      '    default         linear;'
      '    interpolate(U)  linear;'
      '  }'
      '  fluxRequired'
      '  {'
      '    default         no;'
      '    p;'
      '  }'
      '  wallDist'
      '  {'
      '    method         meshWave;'
      '  }'
      '  mixingInterface'
      '  {'
      '    U         consistentAveraging;'
      '    p         consistentAveraging;'
      '    k         fluxAveraging;'
      '    epsilon   fluxAveraging;'
      '    omega     fluxAveraging;'
      '  }'
      '}'
    )
    
  @staticmethod
  def decomposeParDict(nSubD: int, method: str) -> str:
    """Create decomposeParDict.

    Parameters
    ----------
    nSubD: int
      Number of subdomains.
    method: str
      Method.

    Returns
    -------
    str
      decomposeParDictRule.
    
    """   
   
    return str(
      'decomposeParDict {'
      '  dtOOFileName        \"system/decomposeParDict\";'
      '  method              '+method+';'
      '  numberOfSubdomains  '+str(nSubD)+';'
      '}'
    )
  
  @staticmethod
  def transportModel() -> str:
    """Create transportModelDict.

    Parameters
    ----------
    None

    Returns
    -------
    str
      transportModelDict.
    
    """

    return str(
      'transportModel {'
      '  dtOOFileName    \"constant/transportProperties\";\n'
      '  transportModel  Newtonian;\n'
      '  nu              nu [0 2 -1 0 0 0 0] 1.0e-06;\n'
      '}'
    )

  @staticmethod
  def turbulenceProperties(RASModel:str = 'kOmegaSST') -> str:
    """Create turbulencePropertiesDict.

    Parameters
    ----------
    RASModel: str
      Label of turbulenceModel.

    Returns
    -------
    str
      turbulencePropertiesDict.
    
    """
   
    return str(
      'turbulenceProperties {'
      '  dtOOFileName            \"constant/turbulenceProperties\";\n'
      '  simulationType          RAS;\n'
      '  RAS {\n'
      '    RASModel            '+RASModel+';\n'
      '    turbulence          on;\n'
      '    printCoeffs         on;\n'
      '  }\n'
      '}'
    )

  @staticmethod
  def MRFProperties(
    cellZones: List[str] = [], 
    omegas: List[float] = [], 
    nonRotatingPatches: List[ List[str] ] = [], 
    patches: List[ List[str] ] = [], 
    axes: List[dtVector3] = [],
    origins: List[dtPoint3] = []
  ) -> str:
    """Create MRFPropertisDict.

    Parameters
    ----------
    cellZones: List[str]
      Rotating cell zones.
    omegas: List[float]
      Omega for each rotating cell zone.
    nonRotatingPatches: List[ List[str] ]
      Non rotating patches.
    patches: List[ List[str] ]
      Rotating patches.
    axes: List[dtVector3]
      Axis of rotation for each cell zone.
    origins: List[dtPoint3]
      Origin for each cell zone.

    Returns
    -------
    str
      MRFPropertiesDict.
    
    """
   
    for cellZone in cellZones:
      if len(omegas)<len(cellZones):
        omegas.append(0.0)
      if len(nonRotatingPatches)<len(cellZones):
        nonRotatingPatches.append(List())
      if len(patches)<len(cellZones):
        patches.append(list())
      if len(axes)<len(cellZones):
        axes.append(dtVector3(0,0,1))
      if len(origins)<len(cellZones):
        origins.append(dtPoint3(0,0,0))
       
    retStr = str(
      'MRFProperties {'
      '  dtOOFileName        \"constant/MRFProperties\";'
    )
    for cellZone, omega, nonRotatingPatch, patch, axis, origin in zip(
      cellZones, omegas, nonRotatingPatches, patches, axes, origins
    ):
      retStr += str(
      '  MRF_'+cellZone+''
      '  {'
      '    cellZone           '+cellZone+';'
      '    active             yes;'
      '    patches            ( '+' '.join(patch)+' );'
      '    nonRotatingPatches ( '+' '.join(nonRotatingPatch)+' );'
      '    axis               ( '
             +str(axis[0])+' '+str(axis[1])+' '+str(axis[2])+' '
           ');'
      '    origin             ( '
             +str(origin[0])+' '+str(origin[1])+' '+str(origin[2])+' '
           ');'
      '    omega              '+str(omega)+';'
      '  }'
      )
    retStr += str(
      '}'
    )
    return retStr

  @staticmethod
  def fieldRuleString(
    varName: str, 
    value: List[float]
  ) -> str:
    """Create string to initialize flow field.

    Initialize flow field `varName` with `value`. If `varName` is a vector
    field, `value` is also a vector.

    Parameters
    ----------
    varName: str
      Variable name.
    value: List[float]
      Initialization value.

    Returns
    -------
    str
      FieldRuleString.

    Raises
    ------
    ValueError
      If varName is not known.

    """
    if varName=="U":
      return str(
        ":volVectorField::U::[0 1 -1 0 0 0 0]::( "
        +" ".join(map(str,value))+" ):"
      )
    elif varName=="p":
      return ":volScalarField::p::[0 2 -2 0 0 0 0]::"+str(value[0])+":"
    elif varName=="k":
      return ":volScalarField::k::[0 2 -2 0 0 0 0]::"+str(value[0])+":"
    elif varName=="omega":
      return ":volScalarField::omega::[0 0 -1 0 0 0 0]::"+str(value[0])+":"
    elif varName=="nut":
      return ":volScalarField::nut::[0 2 -1 0 0 0 0]::"+str(value[0])+":"
    else:
      raise ValueError("Unkown varName = %s" % varName)
 
  @staticmethod
  def emptyRuleString() -> str:
    """Create emptyRuleString.

    Parameters
    ----------
    None

    Returns
    -------
    str
      EmptyRuleString.
    
    """

    return str(':ofOpenFOAMEmptyRule:::::')

  @staticmethod
  def inletRuleString( 
    patchName: str, 
    variables: List[str], 
    values: List[List[float]] = [] 
  ) -> str:
    """Create an inlet rule.

    U, default [0.0, 0.0, 0.0]:
      - If `value` is a scalar, `value` is used as the volumetric flow rate.

      - If `value` is a vector, `value` are the components if the fixed value.

    p, default 0.0:
      - `value` is the value of zeroGradient.

    k, default [0.10, 0.0]:
      - `value` is the intensity of turbulent kinetic energy.
    
    omega, default 0.0:
      - `value` is the value of fixedValue.

    Parameters
    ----------
    patchName: str
      Patch name.
    variables: List[str]
      List of variables.
    values: List[ List[float] ]
      List of values.

    Returns
    -------
    str
      Inlet rule string.

    Raises
    ------
    ValueError
      If variable is not known.

    """
    retStr = ":ofOpenFOAMSetupRule::"+patchName+":::"
    ii = 0
    for variable in variables:
      if ii>=len(values):
        value = []
      else:
        value = values[ii]
      ii = ii + 1

      if variable=="U":
        if len(value)==0:
          value = [0.0,0.0,0.0]
        if len(value)==3:
          retStr += str(
            ':U::fixedValue( value uniform ('+' '.join(map(str,value))+');):'
          )
        elif len(value)==1:
          retStr += str(
            ':U::flowRateInletVelocity('
            '  value               uniform (0 0 0 );'
            '  volumetricFlowRate  '+str(value[0])+';'
            '  extrapolateProfile  yes;'
            '):'
          )
      elif variable=="p":
        if len(value)==0:
          value = [0.0,]
        retStr += str(
          ':p::zeroGradient(value uniform '+str(value[0])+';):'
        )
      elif variable=="k":
        if len(value)==0:
          value = [0.10,0.0]
        retStr += str(
          ':k::turbulentIntensityKineticEnergyInlet('
          '  intensity     '+str(value[0])+';'
          '  value uniform '+str(value[1])+';'
          '  U              U;'
          '  phi            phi;'
          '):'
        )
      elif variable=="omega":
        if len(value)==0:
          value = [0.0,]
        retStr += str(
          ':omega::fixedValue(value uniform '+str(value[0])+';):'
        )
      else:
        raise ValueError("Unknown variable %s" % variable)
    return retStr

  @staticmethod
  def cylindricalInletRuleString( 
    patchName: str, 
    variables: List[str], 
    values: List[List[float]] = [] ,
    rotAxis: dtVector3 = dtVector3(0,0,1), 
    rotCentre: dtPoint3 = dtPoint3(0,0,0)
  ) -> str:
    """Create an cylindrical inlet rule.

    U, default [0.0, 0.0, 0.0]:
      - components of `value` represent (c_r, c_phi, c_z).

    Parameters
    ----------
    patchName: str
      Patch name.
    variables: List[str]
      List of variables.
    values: List[ List[float] ]
      List of values.
    rotAxis: dtVector3
      Axis of rotation.
    rotCentre: dtPoint3
      Centre of rotation.

    Returns
    -------
    str
      Cylindrical inlet rule string.

    Raises
    ------
    ValueError
      If variable is not known.

    """
   
    retStr = ":ofOpenFOAMCylindricalInletRule::"+patchName+":::"
    ii = 0
    for variable in variables:
      if ii>=len(values):
        value = []
      else:
        value = values[ii]
      ii = ii + 1

      if variable=="U":
        if len(value)==0:
          value = [0.0,0.0,0.0]
        retStr += str(
          ':U:'
          ':'
            'rotationAxis('
              +str(rotAxis[0])+','+str(rotAxis[1])+','+str(rotAxis[2])+
            '),'
            'origin('
              +str(rotCentre[0])+','+str(rotCentre[1])+','+str(rotCentre[2])+
            '),'
            'value('+','.join(map(str,value))+')'
          ':'
        )
      else:
        raise ValueError("Unknown variable %s" % variable)
    return retStr

  @staticmethod
  def outletRuleString( 
    patchName: str, 
    variables: List[str], 
    values: List[List[float]] = [] 
  ) -> str:
    """Create an outlet rule.

    U, default [0.0, 0.0, 0.0]:
      - `value` is the value of zeroGradient.

    p, default 0.0:
      - `value` is the value of fixedValue.

    k, default [0.0]:
      - `value` is the value of zeroGradient.
    
    omega, default 0.0:
      - `value` is the value of zeroGradient.

    Parameters
    ----------
    patchName: str
      Patch name.
    variables: List[str]
      List of variables.
    values: List[ List[float] ]
      List of values.

    Returns
    -------
    str
      Outlet rule string.

    Raises
    ------
    ValueError
      If variable is not known.
    """
   
    retStr = ":ofOpenFOAMSetupRule::"+patchName+":::"
    ii = 0
    for variable in variables:
      if ii>=len(values):
        value = []
      else:
        value = values[ii]
      ii = ii + 1

      if variable=="U":
        if len(value)==0:
          value = [0.0,0.0,0.0]
        retStr += str(
          ':U::zeroGradient( value uniform ('+' '.join(map(str,value))+');):'
        )
      elif variable=="p":
        if len(value)==0:
          value = [0.0,]
        retStr += str(
          ':p::fixedValue(value uniform '+str(value[0])+';):'
        )
      elif variable=="k":
        if len(value)==0:
          value = [0.0,]
        retStr += str(
          ':k::zeroGradient(value uniform '+str(value[0])+';):'
        )
      elif variable=="omega":
        if len(value)==0:
          value = [0.0,]
        retStr += str(
          ':omega::zeroGradient(value uniform '+str(value[0])+';):'
        )
      else:
        raise ValueError("Unknown variable %s" % variable)
    return retStr

  @staticmethod
  def wallRuleString( 
    patchName: str, 
    variables: List[str], 
    values: List[List[float]] = [] 
  ) -> str:
    """Create a wall rule.

    U, default [0.0, 0.0, 0.0]:
      - `value` is the value of fixedValue.

    p, default 0.0:
      - `value` is the value of zeroGradient.

    k, default [0.0]:
      - `value` is the value of kqRWallFunction.
    
    omega, default 0.0:
      - `value` is the value of omegaWallFunction.

    Parameters
    ----------
    patchName: str
      Patch name.
    variables: List[str]
      List of variables.
    values: List[ List[float] ]
      List of values.

    Returns
    -------
    str
      Wall rule string.

    Raises
    ------
    ValueError
      If variable is not known.
    """
   
    retStr = ":ofOpenFOAMWallRule::"+patchName+":::"
    ii = 0
    for variable in variables:
      if ii>=len(values):
        value = []
      else:
        value = values[ii]
      ii = ii + 1
      if variable=="U":
        if len(value)==0:
          value = [0.0,0.0,0.0]
        retStr += str(
          ':U::fixedValue( value uniform ('+' '.join(map(str,value))+');):'
        )
      elif variable=="p":
        if len(value)==0:
          value = [0.0,]
        retStr += str(
          ':p::zeroGradient(value uniform '+str(value[0])+';):'
        )
      elif variable=="k":
        if len(value)==0:
          value = [0.0,]
        retStr += str(
          ':k::kqRWallFunction(value uniform '+str(value[0])+';):'
        )
      elif variable=="omega":
        if len(value)==0:
          value = [0.0,]
        retStr += str(
          ':omega::omegaWallFunction('
          '  U        U;'
          '  blending stepwise;'
          '  value    uniform '+str(value[0])+';'
          '):'
        )
      elif variable=="nut":
        if len(value)==0:
          value = [0.0,]
        retStr += str(
          ':nut::nutkWallFunction('
          '  value    uniform '+str(value[0])+';'
          '):'
        )
      else:
        raise ValueError("Unknown variable %s" % variable)
    return retStr

  @staticmethod
  def cyclicAmiRuleString( 
    patchOne: str, patchTwo: str, 
    rotAxis: dtVector3 = dtVector3(0,0,1), 
    rotCentre: dtPoint3 = dtPoint3(0,0,0)
  ) -> str:
    """Create a cyclic AMI rule.

    AMI is created between `patchOne` and `patchTwo` with axis of rotation
    `rotAxis` and centre of rotation `rotCentre`.

    Parameters
    ----------
    patchOne: str
      Patch one name.
    patchTwo: str
      Patch two name.
    rotAxis: dtVector3
      Axis of rotation.
    rotCentre: dtPoint3
      Centre of rotation.

    Returns
    -------
    str
      Cyclic AMI rule string.
    """
   
    retStr = ":ofOpenFOAMCyclicAmiRule::"+patchOne+","+patchTwo+":"
    retStr += str(
      ":"
      "  "+patchOne+"("
      "    transform       rotational;"
      "    rotationAxis    ("
             +str(rotAxis[0])+" "+str(rotAxis[1])+" "+str(rotAxis[2])
           +");"
      "    rotationCentre  ("
             +str(rotCentre[0])+" "+str(rotCentre[1])+" "+str(rotCentre[2])
           +");"
      "  ),"
      "  "+patchTwo+"("
      "    transform       rotational;"
      "    rotationAxis    ("
             +str(rotAxis[0])+" "+str(rotAxis[1])+" "+str(rotAxis[2])
           +");"
      "    rotationCentre  ("
             +str(rotCentre[0])+" "+str(rotCentre[1])+" "+str(rotCentre[2])
           +");"
      "  )"
      ":"
    )
    return retStr

  @staticmethod
  def mixingPlaneRuleString(
    patchOne: str, patchTwo: str, 
    variables: List[str],
    axis: dtVector3 = dtVector3(0,0,1), 
    origin: dtPoint3 = dtPoint3(0,0,0),
    stackAxis: str = "R"
  ) -> str:
    """Create a cyclic AMI rule.

    AMI is created between `patchOne` and `patchTwo` with axis of rotation
    `rotAxis` and centre of rotation `rotCentre`.

    Parameters
    ----------
    patchOne: str
      Patch one name.
    patchTwo: str
      Patch two name.
    variables: List[str]
      List of variables.
    rotAxis: dtVector3
      Axis of rotation.
    rotCentre: dtPoint3
      Centre of rotation.
    stackAxis: str
      Stack axis for averaging.

    Returns
    -------
    str
      Cyclic AMI rule string.
    """
   
    retStr = ":ofOpenFOAMMixingPlaneRule::"+patchOne+","+patchTwo+":"
    retStr += str(
      ":"
      "  "+patchOne+"("
      "    axis    ("+str(axis[0])+" "+str(axis[1])+" "+str(axis[2])+");"
      "    origin  ("+str(origin[0])+" "+str(origin[1])+" "+str(origin[2])+");"
      "    ribbonPatch {"
      "      division        meshDependent;"
      "      stackAxis       "+stackAxis+";"
      "      discretisation  bothPatches;"
      "    }"                                                                                                     
      "  )" 
      "  "+patchTwo+"("
      "    axis    ("+str(axis[0])+" "+str(axis[1])+" "+str(axis[2])+");"
      "    origin  ("+str(origin[0])+" "+str(origin[1])+" "+str(origin[2])+");"
      "  )"
      ":"
    )
    for variable in variables:
      if variable=="U":
        retStr += str(
          ":"+variable+":"
          ":"
          "  mixingInterface("
          "    value  uniform (0 0 0);"
          "  ),"
          "  mixingInterface("
          "    value  uniform (0 0 0);"
          "  )"
          ":"
        )
      elif variable=="omega":
        retStr += str(
          ":"+variable+":"
          ":"
          "  mixingInterface("
          "    value  uniform 1;"
          "  ),"
          "  mixingInterface("
          "    value  uniform 1;"
          "  )"
          ":"
        )
      else:
        retStr += str(
          ":"+variable+":"
          ":"
          "  mixingInterface("
          "    value  uniform 0;"
          "  ),"
          "  mixingInterface("
          "    value  uniform 0;"
          "  )"
          ":"
        )
    return retStr
