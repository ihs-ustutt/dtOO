import logging
import os
from pyDtOO.dtDirectory import dtDirectory

class dtGod:
  class __dtGod:
    def __init__(self, g, rho):
        logging.info('Create dtGod')
        self.g_ = g
        self.rho_ = rho
  instance = None

  def __init__(self, g=9.81, rho=1000.):
    if not dtGod.instance:
      dtGod.instance = dtGod.__dtGod(g, rho)

  def Murder( self ):
    logging.info('Murder dtGod')
    dtGod.instance = None

  def G( self ):
    return dtGod.instance.g_

  def Rho( self ):
    return dtGod.instance.rho_

  def LockPath( self ):
    if "OSLO_LOCK_PATH" not in os.environ:
      raise RuntimeError("Environment variable OSLO_LOCK_PATH not defined.")
    return dtDirectory( os.environ['OSLO_LOCK_PATH'] )