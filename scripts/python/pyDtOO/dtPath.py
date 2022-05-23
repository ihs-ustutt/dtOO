import logging
import os

class dtPath:
  def __init__(self, fn):
    if fn.find("/")>=0:
      logging.warning("Last character of > fn[-1]==/ < -> rstrip")
    self.fn_ = fn.rstrip("/")

  def BaseName(self):
    return os.path.basename(self.fn_)

  def DirName(self):
    return os.path.dirname(self.fn_)

  def FullName(self):
    return self.fn_
