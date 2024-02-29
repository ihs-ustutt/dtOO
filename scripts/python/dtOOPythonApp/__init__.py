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
