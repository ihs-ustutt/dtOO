from pyDtOO.dtPath import dtPath

class dtFile(dtPath):
  """Define a file.

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

  def FileName(self) -> str:
    """Get File name.

    Returns
    -------
    str
      File name as string.

    """
    return super().BaseName()