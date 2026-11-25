import logging

import numpy as np
from fluidfoam.readof import OpenFoamFile
import os

class dtMeanplaneSeparator():

    def __init__(self, case):
        
        self.case_ = case

        mesh_path = os.path.join(case, "constant/polyMesh") + "/"

        # Read OpenFOAM mesh files
        self.point_file = OpenFoamFile(path=mesh_path, name="points")
        self.face_file = OpenFoamFile(path=mesh_path, name="faces")
        self.boundary_file = OpenFoamFile(path=mesh_path, name="boundary")


    def getSeparator(
            self,
            regionName = "ru_mesh",
            planes = None,
            planesBl = None,
            stackAxis = "Z",
            debug = False
        ):
        
        if stackAxis == "Z":
            ax = 1
        elif stackAxis == "R":
            ax = 0
        else:
            logging.error("Invalid value for stackAxis, Valid are 'R' or 'Z'")

        outletVertices = self.getPatchVertices(regionName + "_outlet")
        pressureVertices = self.getPatchVertices(regionName +"_pressure")

        edgeVertices = outletVertices.intersection(pressureVertices)
        edgeVertices = sorted(edgeVertices)

        logging.info("Edge vertices on patch %s_outlet" % regionName)
        logging.info("Number of vertices: %d" % len(edgeVertices))

        verticeCoordsRZ = np.column_stack((
            np.sqrt( 
                self.point_file.values_x[edgeVertices]**2 + self.point_file.values_y[edgeVertices]**2 
            ),
            self.point_file.values_z[edgeVertices]
        ))
        verticeCoordsRZ = verticeCoordsRZ[
            np.argsort(verticeCoordsRZ[:, ax])
        ]

        if planes != None and planesBl != None:
            
            hubBl = verticeCoordsRZ[:planesBl+1]
            shrBl = verticeCoordsRZ[-(planesBl+1):]
            
            lastHub = hubBl[-1]
            firstShr = shrBl[0]

            planesUnstr = planes - planesBl*2
            
            rUnstr = np.linspace(lastHub[0], firstShr[0], planesUnstr + 1)[1:-1]
            zUnstr = np.linspace(lastHub[1], firstShr[1], planesUnstr + 1)[1:-1]

            verticeCoordsUnstr = np.column_stack((rUnstr, zUnstr))

            verticeCoordsRZ = np.concatenate((
                hubBl,
                verticeCoordsUnstr,
                shrBl
            ), axis=0)
        
        if debug == True:

            # Number of points
            n = len(verticeCoordsRZ)

            logging.info("Meanplane averaging coordinates in RZ on patch %s_outlet" % regionName)
            logging.info("Number of vertices: %d" % n)
            logging.info("Coordinates (r,z):")
            logging.info(verticeCoordsRZ)
            logging.info("Writing .vtk file ...")


            with open(self.case_+"meanplaneSep_"+regionName+"_outlet.vtk", "w") as f:

                # VTK header
                f.write("# vtk DataFile Version 3.0\n")
                f.write("Hub to shroud verticeCoordsRZ\n")
                f.write("ASCII\n")
                f.write("DATASET POLYDATA\n")

                # Write points (r, 0, z)
                f.write(f"POINTS {n} float\n")

                for r, z in verticeCoordsRZ:
                    f.write(f"{r} 0.0 {z}\n")
                
                # Vertices
                f.write(f"VERTICES {n} {2*n}\n")

                for i in range(n):
                    f.write(f"1 {i}\n")

            logging.info("Saved in %smeanplaneSep_%s_outlet.vtk" % (self.case_,regionName))
        
        sep = verticeCoordsRZ[:,ax]
        #sep = sep[1:-1]

        return sep

    def getPatchVertices(self, patchName):

        patch = self.boundary_file.boundaryface[patchName.encode()]

        startFace = int(patch[b"startFace"])
        nFaces = int(patch[b"nFaces"])

        vertexIDs = set()

        for faceID in range(startFace, startFace + nFaces):

            faceVertices = self.face_file.faces[faceID]["id_pts"]

            vertexIDs.update(faceVertices)

        return vertexIDs

