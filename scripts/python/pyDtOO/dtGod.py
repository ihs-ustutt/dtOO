import logging
import os
from pyDtOO.dtDirectory import dtDirectory
import warnings
from typing import Union

class dtGod:
  """Singleton for constants, lock path, ...

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
    """Constructor.

    Parameters
    ----------
    g : float, optional
      Gravitational constant. The default is 9.81.
    rho : float, optional
      Density. The default is 1000.0.

    Returns
    -------
    None

    """
    if not dtGod.instance:
      dtGod.instance = dtGod.__dtGod(g, rho)

  def Murder( self ) -> None:
    """Destroy instance.

    Returns
    -------
    None

    """
    logging.info('Murder dtGod')
    dtGod.instance = None

  def G( self ) -> float:
    """
    Get gravitational constant.

    Returns
    -------
    float
      Gravitational constant.

    """
    return dtGod.instance.g_

  def Rho( self ) -> float:
    """Get density.

    Returns
    -------
    float
      Density.

    """
    return dtGod.instance.rho_

  def LockPath( self ) -> dtDirectory:
    """Get lock path.

    Function checks if environment variable `OSLO_LOCK_PATH`
    is defined.

    Returns
    -------
    dtDirectory
      Lock path.

    Warns
    -----
    Warning if environment variable `OSLO_LOCK_PATH` not defined.

    """
    if "OSLO_LOCK_PATH" not in os.environ:
      warnings.warn("Environment variable OSLO_LOCK_PATH not defined.")
      return dtDirectory("")
    else:
      return dtDirectory( os.environ['OSLO_LOCK_PATH'] )