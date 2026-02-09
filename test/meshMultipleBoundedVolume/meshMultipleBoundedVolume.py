import sys

from dtOOPythonSWIG import *
from dtOOPythonApp.tools.dtBundleTools import dtBundleBuilder
import numpy as np

class meshMultipleBoundedVolume (dtBundleBuilder):

    def __init__(self):
        super(meshMultipleBoundedVolume, self).__init__()

    def create(self) -> dtBundle:
    
        # Create an empty container
        container = dtBundle()

        # Create log file
        logMe.initLog('build.log')

        # Init staticProperties
        staticPropertiesHandler.getInstance().jInit(
          jsonPrimitive(
            '{'
              '"option" : ['
                '{"name" : "reparamOnFace_precision", "value" : "1.e-05"},'
                '{"name" : "reparamInVolume_precision","value" : "1.e-05"},'
                '{"name" : "reparam_internalRestarts", "value" : "10"},'
                '{"name" : "reparam_restarts", "value" : "10"},'
                '{"name" : "reparam_restartIncreasePrecision", "value" : "10."},'
                '{'
                  '"name" : "reparam_internalRestartDecreasePrecision",'
                  ' "value" : "0.9"'
                '},'
                '{'
                  '"name" : "reparamOnFace_minimizer", '
                  '"value" : ":Minuit2::kMigrad:"'
                '},'
                '{'
                  '"name" : "reparamInVolume_minimizer", '
                  '"value" : ":Minuit2::kMigrad:"'
                '},'
                '{"name" : "invY_precision", "value" : "1.e-04"},'
                '{"name" : "xyz_resolution", "value" : "1.e-05"},'
                '{"name" : "XYZ_resolution", "value" : "1.e-04"},'
                '{"name" : "uvw_resolution", "value" : "1.e-04"},'
                '{"name" : "point_render_diameter", "value" : "0.005"},'
                '{"name" : "vector_render_size", "value" : "0.05"},'
                '{"name" : "function_render_resolution_u", "value" : "21"},'
                '{"name" : "function_render_resolution_v", "value" : "21"},'
                '{"name" : "function_render_resolution_w", "value" : "21"},'
                '{"name" : "geometry_render_resolution_u", "value" : "200"},'
                '{"name" : "geometry_render_resolution_v", "value" : "200"},'
                '{"name" : "geometry_render_resolution_w", "value" : "200"},'
                '{"name" : "root_printLevel", "value" : "0"},'
                '{"name" : "root_maxIterations", "value" : "1000"},'
                '{"name" : "root_maxFunctionCalls", "value" : "1000000"},'
                '{"name" : "ompNumThreads", "value" : "2"},'
                '{"name" : "map1dTo3d_deltaPer", "value" : "0.01"},'
                '{"name" : "map2dTo3d_deltaPer", "value" : "0.01"},'
                '{"name" : "map3dTo3d_deltaPer", "value" : "0.01"},'
                '{"name" : "logLevel", "value" : "99"},'
                '{"name" : "isEqualExtendCheck", "value" : "false"}'
              ']'
            '}'
          )
        )

        # Init baseContainer and labeledVectorHandlings
        bC = container.cptr_bC()
        cV = container.cptr_cV()

        # clearing prevents segmentation fault
        lVHOstateHandler.clear()
        lVHOstateHandler( jsonPrimitive(), cV ).thisown = False

        aF = container.cptr_aF()
        aG = container.cptr_aG()
        bV = container.cptr_bV()
        dC = container.cptr_dC()
        dP = container.cptr_dP()
        
        print(type(
            bSplineCurve_pointConstructOCC(
                        vectorDtPoint3()
                          << dtPoint3(0.00, 0.00, 0.00)
                          << dtPoint3(1.00, 0.00, 0.00),
                          1
                    ).result()
            )
        )
        box = analyticSurface(
                bSplineSurface_bSplineCurveFillConstructOCC( 
                    bSplineCurve_pointConstructOCC(
                        vectorDtPoint3()
                          << dtPoint3(0.00, 0.00, 0.00)
                          << dtPoint3(1.00, 0.00, 0.00),
                          1
                    ).result(),
                    bSplineCurve_pointConstructOCC(
                        vectorDtPoint3()
                          << dtPoint3(1.00, 0.00, 0.00)
                          << dtPoint3(1.00, 0.00, 1.00),
                          1
                    ).result(),
                    bSplineCurve_pointConstructOCC(
                        vectorDtPoint3()
                          << dtPoint3(1.00, 0.00, 1.00)
                          << dtPoint3(0.00, 0.00, 1.00),
                          1
                    ).result(),
                    bSplineCurve_pointConstructOCC(
                        vectorDtPoint3()
                          << dtPoint3(0.00, 0.00, 1.00)
                          << dtPoint3(0.00, 0.00, 0.00),
                          1
                    ).result(),
                ).result()
            )
       
        box.setLabel("box")
        aG.push_back(box.clone())
        print(type(box))
        #self.appendAnalyticGeometry(
        #    box.clone(),
        #    "debug_box",
        #)
        
        curve0 = analyticCurve(
          bSplineCurve_pointConstructOCC(
            vectorDtPoint3()
              << dtPoint3(+0.20, +0.00, 0.20)
              << dtPoint3(+0.80, +0.00, 0.20),
            1
          ).result()
        )
        curve0.setLabel("curve0")
        aG.push_back(curve0.clone())

        curve1 = analyticCurve(
          bSplineCurve_pointConstructOCC(
            vectorDtPoint3()
              << dtPoint3(+0.80, +0.00, 0.20)
              << dtPoint3(+0.80, +0.00, 0.80),
            1
          ).result()
        )
        curve1.setLabel("curve1")
        aG.push_back(curve1.clone())

        curve2 = analyticCurve(
          bSplineCurve_pointConstructOCC(
            vectorDtPoint3()
              << dtPoint3(+0.80, +0.00, 0.80)
              << dtPoint3(+0.50, +0.00, 0.80),
            1
          ).result()
        )
        curve2.setLabel("curve2")
        aG.push_back(curve2.clone())

        curve3 = analyticCurve(
          bSplineCurve_pointConstructOCC(
            vectorDtPoint3()
              << dtPoint3(+0.50, +0.00, 0.80)
              << dtPoint3(+0.20, +0.00, 0.50),
            1
          ).result()
        )
        curve3.setLabel("curve3")
        aG.push_back(curve3.clone())

        curve4 = analyticCurve(
          bSplineCurve_pointConstructOCC(
            vectorDtPoint3()
              << dtPoint3(+0.20, +0.00, 0.50)
              << dtPoint3(+0.20, +0.00, 0.20),
            1
          ).result()
        )
        curve4.setLabel("curve4")
        aG.push_back(curve4.clone())
        



        print("finished")
        return container
def CreateAndShow( *args, **kwargs ):
  """Create and show machine.

  Parameters
  ----------
  None

  Returns
  -------
  List[ dtOO.dtBundle, dtOOInParaVIEW ]
    List containing the dtBundle and the render object for ParaVIEW.

  See Also:
  ---------
  simpleAxialRunner.create : Create machine.

  """
  from dtOOPythonApp.vis import dtOOInParaVIEW
  cc = meshMultipleBoundedVolume().create(*args, **kwargs)

  rr = dtOOInParaVIEW( cc )
##  rr.Show( rr.Find( "xyz_channel.*", True), "xyz_channel" )
##  rr.Show( rr.Find( ".*debug.*", True), "xyz_debug" )
#  rr.Show( rr.Find( "xyz_diffuser.*", True), "xyz_diffuser" )
##  rr.Show( rr.Find( "xyz_meanplane.*", True), "xyz_meanplane" )
#  rr.Show( rr.Find( "xyz_blade.*", True), "xyz_blade" )
##  rr.Show( rr.Find( "xyz_meshBlock.*", True), "xyz_meshBlock" )
#  rr.Show( rr.Find( "meanplane.*", True), "meanplane" )
#  rr.Show( rr.Find( "blade.*", True), "blade" )
#  rr.Show( rr.Find( "meshBlock.*", True), "meshBlock" )
#  rr.Show( rr.Find( "xyz_fe_meanplane.*", True), "xyz_fe_meanplane" )
#  rr.Show( rr.Find( "xyz_gridChannel.*", True), "xyz_gridChannel" )
#  rr.Show( rr.Find( "xyz_gridDiffuser.*", True), "xyz_gridDiffuser" )

  return cc, rr

if __name__ == "__main__":
  meshMultipleBoundedVolume().create()
meshMultipleBoundedVolume

