from pyDtOO.dtPath import dtPath

class dtFile(dtPath):
  def __init__(self, fn):
    super().__init__(fn)
 
  def FileName(self):
    return super().BaseName()