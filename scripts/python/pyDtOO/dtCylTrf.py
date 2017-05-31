import logging
import numpy

class dtCylTrf:
  def __init__(self, x, y, z):
    self.r_ = numpy.sqrt(x**2 + y**2)
    self.phi_ = numpy.arctan2(y, x)
    self.z_ = z
  
  def R(self):
    return self.r_
  
  def Phi(self):
    return self.phi_
  
  def Z(self):
    return self.z_  

