import logging
import os

class dtPath:
  """Define a path.

  A path consists of a base and directory portion. All dir_* parths of the
  path
    /dir_0/dir_1/dir_2/base
  form the directory portion
    /dir_0/dir_1/dir_2
  and the last element
    base
  represents the base portion of the path name. A trailing `/` sign is
  automatically removed.

  Attributes
  ----------
  fn_ : str
    Path to object as string.

  """

  def __init__(self, fn: str) -> None:
    """Constructor.

    Parameters
    ----------
    fn : str
      Path to object as string.

    Returns
    -------
    None

    Warns
    -----
    Warning if last character is equal `=`.

    """
    if fn.find("/")>=0:
      logging.warning("Last character of > fn[-1]==/ < -> rstrip")
    self.fn_ = fn.rstrip("/")

  def BaseName(self) -> str:
    """Get base of path name.

    Returns
    -------
    str
      Base of path name.

    """
    return os.path.basename(self.fn_)

  def DirName(self) -> str:
    """Get directory portion of path name.

    Returns
    -------
    str
      Directory portion of path name.

    """

    return os.path.dirname(self.fn_)

  def FullName(self) -> str:
    """ Get full path name.

    Returns
    -------
    str
      Full path name.

    """
    return self.fn_
