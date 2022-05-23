from pyDtOO.dtPath import dtPath

class dtDirectory(dtPath):
  """Define a directory.

  Attributes
  ----------
  None

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

    """
    super().__init__(fn)