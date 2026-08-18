from dtOOPythonApp.tools import dtBundleBuilder

from dtOOPythonSWIG import (
    analyticGeometry,
    vectorHandlingAnalyticGeometry,
    map1dTo3d,
    analyticCurve,
    geomCurve_curveReverseConstructOCC
)

import numpy as np

class vectorHandlingAnalyticGeometry_sortCurves(dtBundleBuilder):

    def __init__(
        self, 
        curvesUnsorted: vectorHandlingAnalyticGeometry
        
    ) -> None:
        super(vectorHandlingAnalyticGeometry_sortCurves, self).__init__()
        
        self.curvesUnsorted_ = []

        for curve in curvesUnsorted:
            
            print(type(curve))
            print(curve.virtualClassName())

             
            self.curvesUnsorted_.append(
                analyticCurve.MustConstDownCast(curve).ptrDtCurve()
            )
        
        self.curvesSorted_ = []


    def build(self) -> None:

        # Precompute endpoints
        curve_data = []
        for curve in self.curvesUnsorted_:

            p0 = curve.point(curve.getUMin())
            p1 = curve.point(curve.getUMax())
            curve_data.append((curve, p0, p1))

        # Start with the first curve
        first_curve, first_p0, first_p1 = curve_data.pop(0)
        
        self.curvesSorted_.append(
            analyticCurve(first_curve)
        )

        current_end = first_p1
        start_point = first_p0

        while curve_data:
            
            found = False

            for i, (curve, p0, p1) in enumerate(curve_data):
                
                # Current end -> curve start
                if analyticGeometry.inXYZTolerance(current_end, p0):
                    self.curvesSorted_.append(
                            analyticCurve(curve)
                    )

                    current_end = p1
                    curve_data.pop(i)

                    found = True
                    break

                # Current end -> curve end
                elif analyticGeometry.inXYZTolerance(current_end, p1):
                    
                    self.curvesSorted_.append(
                        analyticCurve(
                            geomCurve_curveReverseConstructOCC(curve, True).result()
                        )
                    )

                    current_end = p0
                    curve_data.pop(i)

                    found = True
                    break

            if not found:
                raise ValueError(
                    "Could not construct a continuous curve loop. "
                    f"No curve starts or ends at the current point."
                )
        
        # Check that the final endpoint connects back to the first
        if not analyticGeometry.inXYZTolerance(current_end, start_point):
            raise ValueError(
                "Curves form a continuous chain, but not a closed loop."
            )
        
        return 

    def getSortedCurves(self):
        return self.curvesSorted_
