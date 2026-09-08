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

rr = robustCompare(org_mesh, new_mesh)

nErrors = 0
for elType in ['MHexahedron', 'MTetrahedron', 'MPyramid',]:
  if not rr.compareElementNumber(elType):
      print("FAIL")
      nErrors = nErrors + 1
  
  if not rr.compareNodePositionByElementType(elType):
      print("FAIL")
      nErrors = nErrors + 1

if nErrors==0:
    sys.exit(0)
else:
    sys.exit(1)
