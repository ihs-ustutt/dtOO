import logging
import os
from pyDtOO.dtDirectory import dtDirectory
import warnings
from typing import Union

class dtGod:
  """
  Singleton for constants, lock path, ...

  Attributes
  ----------

  instance : __dtGod | None

  """
  class __dtGod:
    def __init__(self, g: float, rho: float) -> None:
      logging.info('Create dtGod')
      self.g_ = g
      self.rho_ = rho

  instance: Union[__dtGod, None] = None

  def __init__(self, g: float=9.81, rho: float=1000.0) -> None:
    """


    Parameters
    ----------
    g : float, optional
      DESCRIPTION. The default is 9.81.
    rho : float, optional
      DESCRIPTION. The default is 1000.0.

    Returns
    -------
    None
      DESCRIPTION.

    """
    if not dtGod.instance:
      dtGod.instance = dtGod.__dtGod(g, rho)

  def Murder( self ) -> None:
    """
    Destroy self.instance


    Returns
    -------
    None.

    """
    logging.info('Murder dtGod')
    dtGod.instance = None

  def G( self ):
    return dtGod.instance.g_

  def Rho( self ):
    return dtGod.instance.rho_

  def LockPath( self ) -> dtDirectory:
    if "OSLO_LOCK_PATH" not in os.environ:
      warnings.warn("Environment variable OSLO_LOCK_PATH not defined.")
      return dtDirectory("")
    else:
      return dtDirectory( os.environ['OSLO_LOCK_PATH'] )