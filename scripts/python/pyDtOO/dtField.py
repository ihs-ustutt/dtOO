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

def _test_vecField_as_file():
    import tempfile; import csv
    with tempfile.NamedTemporaryFile(mode='w', newline='', delete=False) as tf:
      csv.writer(tf).writerows([
        [0.5, 1.0, 1.5, 10.5, 11.5, 12.0, 0.05, 0.1, 0.15, 100.],
        [2.5, 2.0, 2.5, 20.5, 21.5, 22.0, 1.05, 1.1, 1.15, 200.],
        [3.5, 3.0, 3.5, 30.5, 31.5, 32.0, 2.05, 2.1, 2.15, 300.],
      ])
    return tf.name

def _test_scaField_as_file():
    import tempfile; import csv
    with tempfile.NamedTemporaryFile(mode='w', newline='', delete=False) as tf:
      csv.writer(tf).writerows([
        [0.5, 1.0, 1.5, 11.5, 0.05, 0.1, 0.15, 100.],
        [2.5, 2.0, 2.5, 21.5, 1.05, 1.1, 1.15, 200.],
        [3.5, 3.0, 3.5, 31.5, 2.05, 2.1, 2.15, 300.],
      ])
    return tf.name

import numpy
import logging
from pyDtOO.dtFile import dtFile

class dtField:
  def __init__(self, fn):
    """Constructor.

    Parameters
    ----------
    fn : str
      Filename of the field data.

    Returns
    -------
    None

    Examples
    --------
    >>> dtField( _test_vecField_as_file() ) # doctest: +ELLIPSIS
    <dtField.dtField object at ...
    >>> dtField( _test_scaField_as_file() ) # doctest: +ELLIPSIS
    <dtField.dtField object at ...
    """
     
    self.f_ = dtFile(fn)
    
  def Log(self):
    logging.info( 'Create dtField > %s > %s', self.f_.DirName(), self.f_.FileName() )

  def Read(self):
    """Read data.

    Parameters
    ----------
    None

    Returns
    -------
    numpy.ndarray
      Field's data.

    Examples
    --------
    >>> dtField( _test_vecField_as_file() ).Read() # doctest: +ELLIPSIS
    array([[5.00e-01, 1.00e+00, 1.50e+00, 1.05e+01, 1.15e+01, 1.20e+01,
            5.00e-02, 1.00e-01, 1.50e-01, 1.00e+02],
           [2.50e+00, 2.00e+00, 2.50e+00, 2.05e+01, 2.15e+01, 2.20e+01,
            1.05e+00, 1.10e+00, 1.15e+00, 2.00e+02],
           [3.50e+00, 3.00e+00, 3.50e+00, 3.05e+01, 3.15e+01, 3.20e+01,
            2.05e+00, 2.10e+00, 2.15e+00, 3.00e+02]])
    >>> dtField( _test_scaField_as_file() ).Read() # doctest: +ELLIPSIS
    array([[5.00e-01, 1.00e+00, 1.50e+00, 1.15e+01, 5.00e-02, 1.00e-01,
            1.50e-01, 1.00e+02],
           [2.50e+00, 2.00e+00, 2.50e+00, 2.15e+01, 1.05e+00, 1.10e+00,
            1.15e+00, 2.00e+02],
           [3.50e+00, 3.00e+00, 3.50e+00, 3.15e+01, 2.05e+00, 2.10e+00,
            2.15e+00, 3.00e+02]])
    """
     
    data = numpy.genfromtxt( self.f_.FullName(), delimiter=',', comments='#')
    if numpy.size( numpy.shape( data ) )==1 and numpy.size( data )>0 :
      data = data.reshape( (1,numpy.size(data)) ) 
    logging.info( 'Read data with %s', str(numpy.shape(data)) )      
    return data
