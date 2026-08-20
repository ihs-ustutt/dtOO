import logging
import lzma
import os
import tempfile

import meshio
import numpy as np


class robustCompare:
    """Compare two meshes.

    This class provides methods for reading two meshes and comparing
    their mesh topology and node positions.

    The element types are selected using the Gmsh element class names:

        - ``MTetrahedron``
        - ``MPrism``
        - ``MHexahedron``
        - ``MPyramid``
        - ``MQuad``
        - ``MTriangle``
        - ``MLine``
        - ``MPoint``

    Internally, these names are mapped to the corresponding ``meshio``
    cell types.

    The comparison of node positions is independent of the node numbering
    used in the two meshes.

    Attributes
    ----------
    meshA_: str
        File name of the first mesh.
    meshB_: str
        File name of the second mesh.
    meshAData_: meshio.Mesh
        Data of the first mesh.
    meshBData_: meshio.Mesh
        Data of the second mesh.
    tolerance_: float
        Absolute tolerance used for comparing node positions.

    Examples
    --------
    >>> compare = robustCompare(
    ...     "meshA.msh",
    ...     "meshB.msh.xz"
    ... )
    >>> compare.readMeshes()
    >>> compare.compareElementNumber("MTetrahedron")
    >>> compare.compareNodeNumber()
    >>> compare.compareNodeNumberByElementType("MTriangle")
    >>> compare.compareNodePositionByElementType("MTriangle")

    The comparison methods return ``True`` if the corresponding quantities
    agree and ``False`` otherwise.
    """

    def __init__(
        self,
        meshA: str,
        meshB: str,
        tolerance: float = 1.0e-12
    ) -> None:
        """Constructor.

        Parameters
        ----------
        meshA: str
            File name of the first mesh.
        meshB: str
            File name of the second mesh.
        tolerance: float
            Absolute tolerance used for comparing node positions.

        Returns
        -------
        None
        """
        logging.info(
            "Initializing robust mesh comparison ..."
        )

        self.meshA_ = meshA
        self.meshB_ = meshB

        self.meshAData_ = None
        self.meshBData_ = None

        self.tolerance_ = tolerance

        self.meshesRead_ = False

        #
        # Public element type names follow the Gmsh terminology.
        #
        # The values are the corresponding meshio cell types.
        #
        self.elementTypes_ = {
            "MTetrahedron": "tetra",
            "MPrism": "prism",
            "MHexahedron": "hexahedron",
            "MPyramid": "pyramid",
            "MQuad": "quad",
            "MTriangle": "triangle",
            "MLine": "line",
            "MPoint": "vertex"
        }
        self.readMeshes()

    def readMeshes(self) -> None:
        """Read both meshes.

        The two mesh files are read using ``meshio``. Files ending in
        ``.xz`` are decompressed using the XZ/LZMA format before they
        are passed to ``meshio``.

        Parameters
        ----------
        None

        Returns
        -------
        None
        """
        logging.info(
            "Reading mesh A: %s"
            % self.meshA_
        )

        self.meshAData_ = self._readMesh(
            self.meshA_
        )

        logging.info(
            "Mesh A contains %d nodes"
            % len(self.meshAData_.points)
        )

        logging.info(
            "Reading mesh B: %s"
            % self.meshB_
        )

        self.meshBData_ = self._readMesh(
            self.meshB_
        )

        logging.info(
            "Mesh B contains %d nodes"
            % len(self.meshBData_.points)
        )

        self.meshesRead_ = True

        logging.info(
            "Meshes successfully read."
        )

    def compareElementNumber(
        self,
        elementType: str
    ) -> bool:
        """Compare the number of elements of a given type.

        Parameters
        ----------
        elementType: str
            Gmsh element class name.

            Supported values are:

                - ``MTetrahedron``
                - ``MPrism``
                - ``MHexahedron``
                - ``MPyramid``
                - ``MQuad``
                - ``MTriangle``
                - ``MLine``
                - ``MPoint``

        Returns
        -------
        bool
            ``True`` if both meshes contain the same number of elements
            of the requested type, otherwise ``False``.
        """
        self._checkMeshesRead()
        self._checkElementType(
            elementType
        )

        numberA = self._getElementNumber(
            self.meshAData_,
            elementType
        )

        numberB = self._getElementNumber(
            self.meshBData_,
            elementType
        )

        result = numberA == numberB

        logging.info(
            "Element number for %s: mesh A = %d, mesh B = %d"
            % (
                elementType,
                numberA,
                numberB
            )
        )

        if result:
            logging.info(
                "Element number comparison for %s: OK"
                % elementType
            )
        else:
            logging.info(
                "Element number comparison for %s: FAILED"
                % elementType
            )

        return result

    def compareNodeNumber(self) -> bool:
        """Compare the total number of mesh nodes.

        Parameters
        ----------
        None

        Returns
        -------
        bool
            ``True`` if both meshes contain the same number of nodes,
            otherwise ``False``.
        """
        self._checkMeshesRead()

        numberA = len(
            self.meshAData_.points
        )

        numberB = len(
            self.meshBData_.points
        )

        result = numberA == numberB

        logging.info(
            "Total node number: mesh A = %d, mesh B = %d"
            % (
                numberA,
                numberB
            )
        )

        if result:
            logging.info(
                "Total node number comparison: OK"
            )
        else:
            logging.info(
                "Total node number comparison: FAILED"
            )

        return result

    def compareNodeNumberByElementType(
        self,
        elementType: str
    ) -> bool:
        """Compare the number of nodes belonging to an element type.

        Each node is counted only once, even if it belongs to multiple
        elements of the selected type.

        Parameters
        ----------
        elementType: str
            Gmsh element class name.

        Returns
        -------
        bool
            ``True`` if both meshes contain the same number of unique
            nodes belonging to the requested element type, otherwise
            ``False``.
        """
        self._checkMeshesRead()
        self._checkElementType(
            elementType
        )

        nodesA = self._getNodesByElementType(
            self.meshAData_,
            elementType
        )

        nodesB = self._getNodesByElementType(
            self.meshBData_,
            elementType
        )

        numberA = len(nodesA)
        numberB = len(nodesB)

        result = numberA == numberB

        logging.info(
            "Node number for %s: mesh A = %d, mesh B = %d"
            % (
                elementType,
                numberA,
                numberB
            )
        )

        if result:
            logging.info(
                "Node number comparison for %s: OK"
                % elementType
            )
        else:
            logging.info(
                "Node number comparison for %s: FAILED"
                % elementType
            )

        return result

    def compareNodePositionByElementType(
        self,
        elementType: str
    ) -> bool:
        """Compare node positions belonging to an element type.

        The comparison does not depend on the node numbering of the two
        meshes. The coordinates of the unique nodes belonging to the
        requested element type are compared.

        Parameters
        ----------
        elementType: str
            Gmsh element class name.

        Returns
        -------
        bool
            ``True`` if the node positions agree within ``tolerance_``,
            otherwise ``False``.
        """
        self._checkMeshesRead()
        self._checkElementType(
            elementType
        )

        nodesA = self._getNodesByElementType(
            self.meshAData_,
            elementType
        )

        nodesB = self._getNodesByElementType(
            self.meshBData_,
            elementType
        )

        coordinatesA = self._getNodeCoordinates(
            self.meshAData_,
            nodesA
        )

        coordinatesB = self._getNodeCoordinates(
            self.meshBData_,
            nodesB
        )

        if len(coordinatesA) != len(coordinatesB):

            logging.info(
                "Node position comparison for %s: "
                "different number of nodes"
                % elementType
            )

            return False

        if len(coordinatesA) == 0:

            logging.info(
                "Node position comparison for %s: "
                "no nodes found"
                % elementType
            )

            return True

        #
        # The node numbering is irrelevant for the comparison.
        #
        # Sort the coordinates lexicographically before comparing them.
        #
        coordinatesA = self._sortCoordinates(
            coordinatesA
        )

        coordinatesB = self._sortCoordinates(
            coordinatesB
        )

        difference = np.abs(
            coordinatesA - coordinatesB
        )

        maximumDifference = np.max(
            difference
        )

        result = np.all(
            difference <= self.tolerance_
        )

        logging.info(
            "Node position comparison for %s: "
            "maximum difference = %.6e"
            % (
                elementType,
                maximumDifference
            )
        )

        if result:
            logging.info(
                "Node position comparison for %s: OK"
                % elementType
            )
        else:
            logging.info(
                "Node position comparison for %s: FAILED"
                % elementType
            )

        return result

    def _readMesh(
        self,
        meshFile: str
    ):
        """Read a single mesh.

        Files ending in ``.xz`` are decompressed using the XZ/LZMA
        compression format before being passed to ``meshio``.

        Parameters
        ----------
        meshFile: str
            File name of the mesh.

        Returns
        -------
        meshio.Mesh
            Mesh data.
        """
        if meshFile.endswith(
            ".xz"
        ):

            logging.info(
                "Decompressing XZ mesh: %s"
                % meshFile
            )

            temporaryFileName = None

            try:

                with lzma.open(
                    meshFile,
                    "rb"
                ) as compressedFile:

                    meshData = compressedFile.read()

                with tempfile.NamedTemporaryFile(
                    suffix=".msh",
                    delete=False
                ) as temporaryFile:

                    temporaryFile.write(
                        meshData
                    )

                    temporaryFileName = (
                        temporaryFile.name
                    )

                logging.info(
                    "Reading decompressed mesh"
                )

                return meshio.read(
                    temporaryFileName
                )

            finally:

                if (
                    temporaryFileName is not None
                    and os.path.exists(
                        temporaryFileName
                    )
                ):

                    os.remove(
                        temporaryFileName
                    )

        return meshio.read(
            meshFile
        )

    def _getElementNumber(
        self,
        mesh,
        elementType: str
    ) -> int:
        """Return the number of elements of a given type.

        Parameters
        ----------
        mesh: meshio.Mesh
            Mesh data.
        elementType: str
            Gmsh element class name.

        Returns
        -------
        int
            Number of elements.
        """
        meshioElementType = self.elementTypes_[
            elementType
        ]

        number = 0

        for cellBlock in mesh.cells:

            if cellBlock.type == meshioElementType:

                number += len(
                    cellBlock.data
                )

        return number

    def _getNodesByElementType(
        self,
        mesh,
        elementType: str
    ) -> set:
        """Return all unique node indices of an element type.

        Parameters
        ----------
        mesh: meshio.Mesh
            Mesh data.
        elementType: str
            Gmsh element class name.

        Returns
        -------
        set
            Set of unique zero-based node indices.
        """
        meshioElementType = self.elementTypes_[
            elementType
        ]

        nodeTags = set()

        for cellBlock in mesh.cells:

            if cellBlock.type != meshioElementType:
                continue

            for nodeIds in cellBlock.data:

                for nodeId in nodeIds:

                    nodeTags.add(
                        int(nodeId)
                    )

        return nodeTags

    def _getNodeCoordinates(
        self,
        mesh,
        nodeTags: set
    ) -> np.ndarray:
        """Return coordinates for a set of node indices.

        Parameters
        ----------
        mesh: meshio.Mesh
            Mesh data.
        nodeTags: set
            Zero-based node indices.

        Returns
        -------
        numpy.ndarray
            Array containing the node coordinates.
        """
        if len(nodeTags) == 0:

            return np.empty(
                (0, 3),
                dtype=float
            )

        coordinates = mesh.points[
            list(nodeTags)
        ]

        #
        # meshio may provide two-dimensional points.
        # The comparison is always performed in three dimensions.
        #
        if coordinates.shape[1] == 2:

            coordinates = np.column_stack(
                (
                    coordinates,
                    np.zeros(
                        len(coordinates)
                    )
                )
            )

        return np.asarray(
            coordinates,
            dtype=float
        )

    def _sortCoordinates(
        self,
        coordinates: np.ndarray
    ) -> np.ndarray:
        """Sort node coordinates lexicographically.

        Parameters
        ----------
        coordinates: numpy.ndarray
            Node coordinates.

        Returns
        -------
        numpy.ndarray
            Sorted node coordinates.
        """
        indices = np.lexsort(
            (
                coordinates[:, 2],
                coordinates[:, 1],
                coordinates[:, 0]
            )
        )

        return coordinates[
            indices
        ]

    def _checkMeshesRead(self) -> None:
        """Check whether both meshes have been read.

        Parameters
        ----------
        None

        Returns
        -------
        None

        Raises
        ------
        RuntimeError
            If the meshes have not been read.
        """
        if not self.meshesRead_:

            raise RuntimeError(
                "Meshes have not been read. "
                "Call readMeshes() first."
            )

    def _checkElementType(
        self,
        elementType: str
    ) -> None:
        """Check whether an element type is supported.

        Parameters
        ----------
        elementType: str
            Gmsh element class name.

        Returns
        -------
        None

        Raises
        ------
        ValueError
            If the element type is not supported.
        """
        if elementType not in self.elementTypes_:

            raise ValueError(
                "Unknown element type '%s'. "
                "Supported types are: %s"
                % (
                    elementType,
                    ", ".join(
                        self.elementTypes_.keys()
                    )
                )
            )
