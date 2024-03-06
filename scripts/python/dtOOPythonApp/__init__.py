#------------------------------------------------------------------------------
#  dtOO < design tool Object-Oriented >
#    
#    Copyright (C) 2024 A. Tismer.
#------------------------------------------------------------------------------
#License
#    This file is part of dtOO.
#
#    dtOO is distributed in the hope that it will be useful, but WITHOUT
#    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
#    FITNESS FOR A PARTICULAR PURPOSE.  See the LICENSE.txt file in the
#    dtOO root directory for more details.
#
#    You should have received a copy of the License along with dtOO.
#
#------------------------------------------------------------------------------

"""Package for creating hydraulic machinery

# What is dtOOPythonApp?

The package contains classes for creating hydraulic machinery. It is split in
three subpackages:

  - builder: main builders for machine creation

  - tools: tools used by builders

  - vis: classes for visualization

# Quickstart

The whole package is imported by:

```python
import dtOOPythonApp as dpa
```
"""
from .builder import *
from .tools import *

import importlib.util
if importlib.util.find_spec("paraview.simple") is not None:
  from .vis import *
