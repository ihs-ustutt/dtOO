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
