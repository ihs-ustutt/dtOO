import logging
import os

class dtFile:
  def __init__(self, fn):
    self.fn_ = fn
  
  def FileName(self):
    return os.path.basename(self.fn_)

  def DirName(self):
    return os.path.dirname(self.fn_)
  
  def FullName(self):
    return self.fn_
