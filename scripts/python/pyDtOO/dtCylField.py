import logging
from pyDtOO.dtField import dtField
from pyDtOO.dtCylTrf import dtCylTrf

class dtCylField(dtField):
  def __init__(self, fn):
    dtField.__init__(self, fn)
    
  def Read(self):
    data = dtField.Read(self)
    logging.info('Convert to cylindrical coordinates')
    trf = dtCylTrf(data[:,0], data[:,1], data[:,2])
    data[:,0] = trf.R()
    data[:,1] = trf.Phi()
    data[:,2] = trf.Z()
    return data