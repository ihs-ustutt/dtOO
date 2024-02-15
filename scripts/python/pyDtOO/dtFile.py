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
