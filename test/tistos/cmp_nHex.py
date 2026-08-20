#!/usr/bin/env python3
import logging
logging.basicConfig(
  format='[ %(asctime)s - %(levelname)8s - %(filename)s:%(lineno)d ]'
         ' - %(message)s', 
  datefmt='%d-%b-%y %H:%M:%S', 
  level=logging.INFO
)
import sys
import re
from dtOOPythonApp.mesh import robustCompare

new_mesh = sys.argv[1]
org_mesh = re.sub(r"~.*\.msh.*$", ".msh", new_mesh)

if robustCompare(org_mesh, new_mesh).compareElementNumber("MHexahedron"):
    print("OK")
    sys.exit(0)

sys.exit(1)
