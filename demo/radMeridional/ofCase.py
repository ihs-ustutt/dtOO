import logging
import shutil
import sys 
import os
import time
from pathlib import Path

if not logging.getLogger().handlers:
    logging.basicConfig(
      format='[ %(asctime)s - %(levelname)8s - %(filename)s:%(lineno)d ]'
             ' - %(message)s', 
      datefmt='%d-%b-%y %H:%M:%S', 
      level=logging.DEBUG
    )

    logging.debug("Debug")
    logging.info("Info")
    logging.warning("Warning")
    logging.error("Error")

import foamlib
from pyDtOO import *
import numpy as np
import dtOOPythonSWIG as dtOO

class ofCase:
    
    import sys

    def __init__(self, state):

        self.state_ = state
        ### change working directory to local ssd ###
        self.wd_ = os.environ["TMPDIR"]
        #os.chdir(self.wd_)
        
        #working_dir =f'{{\"name\": \"workingDirectory\", \"value\": \"{self.wd_}\" }}'
        
        self.cDir_ = os.path.join(self.wd_, "of_"+state)
        logging.info(f"case directory: {self.cDir_}")
        
        self.history_ = {}

    def createOFCase(self, container, h_inlet):
    
        bV = container.cptr_bV()
        dC = container.cptr_dC()
 
        dtOO.lVHOstateHandler().makeState(self.state_)

        #
        # draft tube
        #
        
        
        LEGACY_DIR = Path(__file__).resolve().parent
        SAUGROHR = LEGACY_DIR / "saugrohr.cgns"

        rmsh = dtOO.readMOABMesh()
        rmsh.jInit(
            dtOO.jsonPrimitive(
                '{"label" : "dt_mesh", "_fileName" : "'+str(SAUGROHR)+'"}'
            ), 
            None, None, None, None, None
        )
        rmsh.makeGrid()
        
        bV.push_back( rmsh )

        ob = dtOO.bVOFaceToPatchRule()
        ob.thisown = False
        ob.jInit(
          dtOO.jsonPrimitive(
            '{'
              '"_patchRule" : ['
                '":meshSet_2d_0::dt_mesh_wall:",'
                '":meshSet_2d_1::dt_mesh_central:",'
                '":meshSet_2d_2::dt_mesh_vent:",'
                '":meshSet_2d_3::dt_mesh_outlet:",'
                '":meshSet_2d_4::dt_mesh_inlet:"'
              '],'
              '"_regRule" : ['
                '":meshSet_3d_0::dt_mesh:"'
              ']'
            '}'
          ),
          rmsh
        )
        ob.postUpdate()
        
        ob = dtOO.bVOOrientCellVolumes()
        ob.thisown = False
        ob.jInit(
            dtOO.jsonPrimitive('{"_positive" : true}'), rmsh
        )
        ob.postUpdate()

        #ob = dtOO.bVOWriteMSH()
        #ob.thisown = False
        #ob.jInit(
        #        dtOO.jsonPrimitive('{"_filename" : "dt_mesh.msh", "_saveAll" : true}'),
        #  None, None, None, None, None, rmsh
        #)
        #ob.postUpdate()
        
         
        from dtOOPythonApp.builder import (
          ofOpenFOAMCase_turboMachine,
          ofOpenFOAMCase_setupWrapper
        )

        container = ofOpenFOAMCase_turboMachine(
          label = "of",
          bVs = [
            bV["gv_mesh"], 
            bV["ru_mesh"], 
            bV["dt_mesh"],
          ],
          dictRule = \
              ofOpenFOAMCase_setupWrapper.controlDict(
                application = "simpleFoam",
                endTime = 2000,
                # Patches where Q and PT is tracked
                QPatches = ['gv_mesh_inlet', 'gv_mesh_outlet', 
                            'ru_mesh_inlet', 'ru_mesh_outlet',
                            'dt_mesh_inlet', 'dt_mesh_outlet',
                            'gv_mesh_suction', 'gv_mesh_pressure',
                            'ru_mesh_suction', 'ru_mesh_pressure'],
                PTPatches = ['gv_mesh_inlet', 'gv_mesh_outlet',
                            'ru_mesh_inlet', 'ru_mesh_outlet',
                             'dt_mesh_inlet', 'dt_mesh_outlet',
                            'gv_mesh_suction', 'gv_mesh_pressure',
                            'ru_mesh_suction', 'ru_mesh_pressure'],
                FPatches = ['gv_mesh_blade', 'ru_mesh_blade'],
                libs = [
                  "libsimpleFunctionObjects.so",
                  #"libsimpleSwakFunctionObjects.so",
                  #"libmappedFieldFixedValue.so",
                  "libmixingPlane.so",
                ]
              )
            + ofOpenFOAMCase_setupWrapper.fvSchemes()
            + ofOpenFOAMCase_setupWrapper.fvSolution()
            + ofOpenFOAMCase_setupWrapper.transportModel()
            + ofOpenFOAMCase_setupWrapper.turbulenceProperties()
            + ofOpenFOAMCase_setupWrapper.MRFProperties(
                cellZones = ["ru_mesh",],
                omegas    = [375*2*np.pi/60,],
                nonRotatingPatches = [
                  [
                    "ru_mesh_suction", "ru_mesh_pressure",
                    "ru_mesh_inlet", "ru_mesh_outlet",
                  ],
                ],
                patches = [],
                axes = [dtOO.dtVector3(0,0,-1)],
                origins = [dtOO.dtPoint3(0,0,0)]
              ),
            fieldRules = [
              ofOpenFOAMCase_setupWrapper.fieldRuleString("U", [0.0,0.0,-1.0,]),
              ofOpenFOAMCase_setupWrapper.fieldRuleString("p", [0.0,]),
              ofOpenFOAMCase_setupWrapper.fieldRuleString("k", [0.1,]),
              ofOpenFOAMCase_setupWrapper.fieldRuleString("omega", [0.1,]),
              ofOpenFOAMCase_setupWrapper.fieldRuleString("nut", [0.1,]),
            ],
            setupRules = [
              ofOpenFOAMCase_setupWrapper.emptyRuleString(),
              ## meshInlet
              ofOpenFOAMCase_setupWrapper.cylindricalInletRuleString(
                "gv_mesh_inlet",
                ["U"],
                [ [-2.92*2.7,-7.23*2.7,0], ]
              ),
              ofOpenFOAMCase_setupWrapper.inletRuleString(
                "gv_mesh_inlet",
                ["p", "k", "omega",],
                [ [0], [0.1, 0.10], [0.032*h_inlet, 0.1] ]
              ),
              ofOpenFOAMCase_setupWrapper.wallRuleString(
                "gv_mesh_shroud",
                ["omega", "U", "p", "k", "nut"]
              ),
               ofOpenFOAMCase_setupWrapper.wallRuleString(
                "gv_mesh_hub",
                ["omega", "U", "p", "k", "nut"]
              ),
              ofOpenFOAMCase_setupWrapper.wallRuleString(
                "gv_mesh_blade",
                ["omega", "U", "p", "k", "nut"]
              ),
              ofOpenFOAMCase_setupWrapper.cyclicAmiRuleString(
                "gv_mesh_suction", "gv_mesh_pressure"
              ),
              # mixing plane?
              #ofOpenFOAMCase_setupWrapper.cyclicAmiRuleString(
              #  "meshInlet_outlet", "meshChannel_inlet",
              #  rotAxis = None,
              #  rotCentre = None
              #),
              ofOpenFOAMCase_setupWrapper.mixingPlaneRuleString(
                "gv_mesh_outlet", "ru_mesh_inlet",
                ["U", "p", "k", "omega",],
                axis = dtOO.dtVector3(0,0,1),
                origin = dtOO.dtPoint3(0,0,0),
                stackAxis = "Z",
                discretization = "userDefined",
                planes = 40,
                planesBl = 15,
                gradingIf = "false"
              ),

              # runner
              ofOpenFOAMCase_setupWrapper.wallRuleString(
                "ru_mesh_hub",
                ["omega", "U", "p", "k", "nut"]
              ),
              ofOpenFOAMCase_setupWrapper.wallRuleString(
                "ru_mesh_shroud",
                ["omega", "U", "p", "k", "nut"]
              ),
              ofOpenFOAMCase_setupWrapper.wallRuleString(
                "ru_mesh_blade",
                ["omega", "U", "p", "k", "nut"]
              ),
              ofOpenFOAMCase_setupWrapper.cyclicAmiRuleString(
                "ru_mesh_suction", "ru_mesh_pressure"
              ),
              ofOpenFOAMCase_setupWrapper.mixingPlaneRuleString(
                "ru_mesh_outlet", "dt_mesh_inlet",
                ["U", "p", "k", "omega",],
                axis = dtOO.dtVector3(0,0,1),
                origin = dtOO.dtPoint3(0,0,0),
                stackAxis = "R",
                discretization = "userDefined",
                planes = 40,
                planesBl = 15,
                gradingIf = "false"
              ),
              
              # draft tube
              ofOpenFOAMCase_setupWrapper.wallRuleString(
                "dt_mesh_vent",
                ["omega", "U", "p", "k", "nut"]
              ),
              ofOpenFOAMCase_setupWrapper.wallRuleString(
                "dt_mesh_central",
                ["omega", "U", "p", "k", "nut"]
              ),
              ofOpenFOAMCase_setupWrapper.wallRuleString(
                "dt_mesh_wall",
                ["omega", "U", "p", "k", "nut"]
              ),
              ofOpenFOAMCase_setupWrapper.outletRuleString(
                "dt_mesh_outlet",  
                ["U", "p", "k", "omega",]
              ),
            ]
        ).buildExtract( container )
        
        #
        # Output to log of the current state label
        #
        logging.info( 
          "Current state is > %s <." % (dtOO.lVHOstateHandler().commonState()) 
        )

        #self.cDir_ = dC["of"].getDirectory(dtOO.lVHOstateHandler().commonState())
        dC["of"].runCurrentState()
    
    def simulate(self, solver_launcher=None):
        """Perform the simulation.

        Perform the simulation using foamlib. The simulation runs for 500 
        iterations as a laminar simulation. Afterwards, it is switched to turbulent
        mode.
        """
        # get the cpus per task, the first set value is taken
        #  "FLOW_OPT_MPI_RANKS" is defined by the hydroflow config as mpi_ranks
        #  "SLURM_CPUS_PER_TASK" is set in the batch script
        #  "1" is used as fallback value
        cpus_per_task = int(
          os.environ.get(
            "FLOW_OPT_MPI_RANKS",
            os.environ.get("SLURM_CPUS_PER_TASK", "1")
            #os.environ.get("SLURM_TRES_PER_TASK", "cpu=1").split("=")[-1]
          )
        )
        
        if solver_launcher is not None:
            self.solver_launcher_ = list(solver_launcher)
        else:
            self.solver_launcher_ = [
                "mpiexec",
                "--oversubscribe",
                "-n",
                str(cpus_per_task),
            ]

        logging.info(f"Start CFD for state {self.state_} on {cpus_per_task} cores.")
        self.history_['Start Time'] = time.time()
        try:
            #
            # Create an FoamCase object of foamlib to control the simulation; the
            # "getDirectory" function returns the case directory that was created
            #
            fc = foamlib.FoamCase(self.cDir_)

            if cpus_per_task > 1:
                fc.decompose_par_dict['numberOfSubdomains'] = cpus_per_task 
                fc.decompose_par_dict['method'] = 'metis' 
                #fc.decompose_par_dict['method'] = 'scotch' 
                fc.decompose_par()

                fc.turbulence_properties["RAS"]["turbulence"] = False
                fc.fv_schemes['gradSchemes']['none'] = ('cellLimited', 'Gauss', 'linear', 0.33)
                fc.fv_schemes['gradSchemes']['grad(p)'] = ('cellLimited', 'Gauss', 'linear', 0.33)
                fc.fv_schemes['gradSchemes']['grad(U)'] = ('cellLimited', 'Gauss', 'linear', 0.33)
                fc.fv_schemes['divSchemes']['div(phi,U)'] = \
                    ('Gauss', 'linearUpwind', 'cellLimited', 'Gauss', 'linear', 0.33)
                fc.fv_schemes["mixingInterface"] = {
                    "U": "consistentAveraging",
                    "p": "consistentAveraging",
                    "k": "fluxAveraging",
                    "epsilon": "fluxAveraging",
                    "omega": "fluxAveraging"
                  }
                fc.fv_solution["relaxationFactors"] = {
                    "p": 0.2,
                    "U": 0.4,
                    "k": 0.6,
                    "epsilon": 0.6,
                    "omega": 0.6
                  }
                fc.fv_solution["SIMPLE"] = {
                    "nNonOrthogonalCorrectors": 20,
                    #"consistent": "yes"
                  }

                fc.control_dict['DebugSwitches'] = {
                      'mixingInterfacePolyPatch': 2
                    }
                fc.control_dict['writeInterval'] = 100
                fc.control_dict['endTime'] = 500
                solver_cmd = [*self.solver_launcher_, "simpleFoam"]
                if cpus_per_task > 1:
                    solver_cmd.append("-parallel")
                fc.run(cmd=solver_cmd)
                #if cpus_per_task > 1:
                #    fc.run(cmd=["mpiexec", "--oversubscribe", "-n", f"{cpus_per_task}","simpleFoam", "-parallel"])
                #else:
                #    fc.run()

                fc.control_dict['endTime'] = 2000
                fc.control_dict['writeInterval'] = 100
                #fc.control_dict['purgeWrite'] = 10
                fc.turbulence_properties["RAS"]["turbulence"] = True
                fc.run(cmd=solver_cmd)

                #if cpus_per_task > 1:
                #    fc.run(cmd=["mpiexec", "--oversubscribe", "-n", f"{cpus_per_task}","simpleFoam", "-parallel"])
                #else:
                #    fc.run()

            if cpus_per_task > 1:
                fc.run(["reconstructPar", '-time', '500,2000'])
                #fc.run(["reconstructPar", '-time', '100'])

        except:
            logging.exception(f"Failed: {self.state_}.")

    @staticmethod
    def failedFitness():
        """Failed fitness.

        Returns the value that represents a failed design.

        Returns
        -------
        float
          Fitness for a failed design.
        """
        return ofCase.sys.float_info.max    

    def evaluate(self):

        _omega = np.abs(
          foamlib.FoamFile(self.cDir_+'/constant/MRFProperties')['MRF_ru_mesh']['omega']
        )
        fc = foamlib.FoamCase(self.cDir_)
        
        _rho = 997.
        _g = 9.81
        for var in ["U", "p",]:
          for patch in [
                  "gv_mesh_inlet", "gv_mesh_outlet", 
                  "ru_mesh_inlet", "ru_mesh_outlet", 
                  "dt_mesh_inlet", "dt_mesh_outlet"
                ]:
            fc.run(["patchToCsv", var, patch])
        
        _time = fc[-1].name
        #_time = 2000

        pT_gv_in = dtScalarDeveloping( 
            dtDeveloping(self.cDir_+"/postProcessing/PT_gv_mesh_inlet").Read(pattern={"*.dat": ":,1:"}) 
        )
        pT_gv_out = dtScalarDeveloping( 
            dtDeveloping(self.cDir_+"/postProcessing/PT_gv_mesh_outlet").Read(pattern={"*.dat": ":,1:"}) 
        )
        pT_ru_in = dtScalarDeveloping( 
            dtDeveloping(self.cDir_+"/postProcessing/PT_ru_mesh_inlet").Read(pattern={"*.dat": ":,1:"}) 
        )
        pT_ru_out = dtScalarDeveloping(
            dtDeveloping(self.cDir_+"/postProcessing/PT_ru_mesh_outlet").Read(pattern={"*.dat": ":,1:"}) 
        )
        pT_dt_in = dtScalarDeveloping(
            dtDeveloping(self.cDir_+"/postProcessing/PT_dt_mesh_inlet").Read(pattern={"*.dat": ":,1:"}) 
        )
        pT_dt_out = dtScalarDeveloping(
            dtDeveloping(self.cDir_+"/postProcessing/PT_dt_mesh_outlet").Read(pattern={"*.dat": ":,1:"}) 
        )

        dh_gv0 = (pT_gv_out.MeanLast(10) - pT_gv_in.MeanLast(10))/_g
        dh_ru0 = (pT_ru_out.MeanLast(10) - pT_ru_in.MeanLast(10))/_g
        dh_dt0 = (pT_dt_out.MeanLast(10) - pT_dt_in.MeanLast(10))/_g

        dh = dh_gv0 + dh_ru0 + dh_dt0
        logging.info("dh = %f" % dh)
        
        F = dtForceDeveloping( 
            dtDeveloping(
                self.cDir_+"/postProcessing/F_ru_mesh_blade"
            ).Read(pattern={'force.dat' : ':,4:10', 'moment.dat' : ':,4:10', '*.*' : ''}) 
        )

        pt_globIn = pT_gv_in.MeanLast(10)
        pt_globOut = pT_dt_out.MeanLast(10)
        F_z = F.MomentMeanLast(10)[2]

        U_gv_in = dtValueField(dtField(self.cDir_+"/gv_mesh_inlet_U_"+_time+".csv").Read())
        U_gv_out = dtValueField(dtField(self.cDir_+"/gv_mesh_outlet_U_"+_time+".csv").Read())
        p_gv_in = dtValueField(dtField(self.cDir_+"/gv_mesh_inlet_p_"+_time+".csv").Read())
        p_gv_out = dtValueField(dtField(self.cDir_+"/gv_mesh_outlet_p_"+_time+".csv").Read())
        Q_gv = np.abs(p_gv_in.IntQ())

        h_tot_gv_in = (p_gv_in.IntValueQ() / _g + U_gv_in.IntMagSquareQ() / (2.*_g))/Q_gv

        U_ru_in = dtValueField(dtField(self.cDir_+"/ru_mesh_inlet_U_"+_time+".csv").Read())
        U_ru_out = dtValueField(dtField(self.cDir_+"/ru_mesh_outlet_U_"+_time+".csv").Read())
        p_ru_in = dtValueField(dtField(self.cDir_+"/ru_mesh_inlet_p_"+_time+".csv").Read())
        p_ru_out = dtValueField(dtField(self.cDir_+"/ru_mesh_outlet_p_"+_time+".csv").Read())

        Q_ru = np.abs(p_ru_in.IntQ())
        h_tot_ru_in = -(p_ru_in.IntValueQ() / _g + U_ru_in.IntMagSquareQ() / (2.*_g))/Q_ru
        h_tot_ru_out = (p_ru_out.IntValueQ() / _g + U_ru_out.IntMagSquareQ() / (2.*_g))/Q_ru

        dh_ru = (h_tot_ru_out - h_tot_ru_in)
        M_z = F.MomentMeanLast(100)[2]

        eta = M_z * _omega / (_rho*_g*dh_ru*Q_ru)

        logging.info("eta = %f" % eta)
        logging.info("dh_ru = %f" % dh_ru)

        #fit = np.abs(dh_ru + 0.8)/0.8 + (1.0 - eta)
        fit = (1.0 - eta)

        #
        # Check if the simulated geometry is a pump or a turbine; if it is a pump,
        # return an artificial value that is greater than all other fitness values
        #
        failed_fit = min(10,max(10. * np.abs(fit),30))
        
        failed_turbine = False
        if (dh_ru > 0.0) or (M_z > 0.0 ) or (eta > 1.0):
            fit = failed_fit
        #
        # Return fitness value
        #
        return fit, dh_ru, M_z, eta

    def delete_case(self):
      label = "of_"
      dir_path = self.cDir_
      try: 
          #shutil.rmtree(dir_path)

          for file in [
            self.wd_ +"/"+ self.state_ + "_gv_mesh_debug.msh",
            self.wd_ +"/"+ self.state_ + "_gv_mesh.msh",
            self.wd_ +"/"+ self.state_ + "_ru_mesh_debug.msh",
            self.wd_ +"/"+ self.state_ + "_ru_mesh.msh",
                ]:
            os.remove(file)

          logging.debug(f"Deleted OpenFoam Case of state {self.state_} successfully.")
      except Exception as e:
          logging.warning(f"Failed deleting OpenFoam Case of state {self.state_}.")
          logging.warning(e)


    def get_history(self):
        return self.history_
