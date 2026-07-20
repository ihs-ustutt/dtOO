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

import numpy
import logging

class dtValueField:
  def __init__(self, data):
    """Constructor.

    Parameters
    ----------
    data : numpy.array
      Field to evaluate.

    Returns
    -------
    None

    Examples
    --------
    >>> from pyDtOO import dtField
    >>> from pyDtOO.dtField import (
    ...   _test_vecField_as_file, _test_scaField_as_file
    ... )
    >>> dtValueField( 
    ...   dtField( _test_vecField_as_file() ).Read() 
    ... ).vDim_
    3
    >>> dtValueField( 
    ...   dtField( _test_scaField_as_file() ).Read() 
    ... ).vDim_
    1
    """

    self.data_ = data
    data_shape = numpy.shape(data)

    self.coord_ = 0
    self.value_ = 0
    self.q_ = 0
    self.sf_ = 0
    self.vDim_ = 0
    
    if data_shape[0] == 0:
      return
    
    self.coord_ = data[:,0:3]
    self.nValues_ = data_shape[0]
    
    if data_shape[1] == 8:
      logging.info( 'Create dtValueField of scalar')    
      self.value_ = data[:,3:4]
      self.sf_ = data[:,4:7]
      self.q_ = data[:,7:8]
      self.vDim_ = 1
    elif data_shape[1] == 10:
      logging.info( 'Create dtValueField of vector')
      self.value_ = data[:,3:6]
      self.sf_ = data[:,6:9]
      self.q_ = data[:,9:10]      
      self.vDim_ = 3
    else:
      raise ValueError('Bad shape of data array.')
  
    self.a_ = numpy.zeros( numpy.shape(self.sf_)[0], float )
    self.n_ = numpy.zeros( numpy.shape(self.sf_), float )
    for i in range( numpy.shape(self.sf_)[0] ):
      self.a_[i] = numpy.linalg.norm( self.sf_[i,:] )
      self.n_[i,:] = self.sf_[i,:] / self.a_[i]

  def Coord(self):
    """Constructor.

    Parameters
    ----------
    None

    Returns
    -------
    numpy.array:
      Coordinates.  

    Examples
    --------
    >>> from pyDtOO import dtField
    >>> from pyDtOO.dtField import _test_vecField_as_file
    >>> f = dtValueField( dtField( _test_vecField_as_file() ).Read() )
    >>> f.Coord()
    array([[0.5, 1. , 1.5],
           [2.5, 2. , 2.5],
           [3.5, 3. , 3.5]])
    """
     
    return self.coord_

  def Value(self):
    """Get values.

    Parameters
    ----------
    None

    Returns
    -------
    numpy.array:
      Values.  

    Examples
    --------
    >>> from pyDtOO import dtField
    >>> from pyDtOO.dtField import _test_vecField_as_file
    >>> f = dtValueField( dtField( _test_vecField_as_file() ).Read() )
    >>> f.Value()
    array([[10.5, 11.5, 12. ],
           [20.5, 21.5, 22. ],
           [30.5, 31.5, 32. ]])
    >>> from pyDtOO.dtField import _test_scaField_as_file
    >>> f = dtValueField( dtField( _test_scaField_as_file() ).Read() )
    >>> f.Value()
    array([[11.5],
           [21.5],
           [31.5]])
    """
     
    return self.value_

  def Q(self):
    """Get discharge.

    Parameters
    ----------
    None

    Returns
    -------
    numpy.array:
      Discharges.  

    Examples
    --------
    >>> from pyDtOO import dtField
    >>> from pyDtOO.dtField import _test_vecField_as_file
    >>> f = dtValueField( dtField( _test_vecField_as_file() ).Read() )
    >>> f.Q()
    array([[100.],
           [200.],
           [300.]])
    """
     
    return self.q_

  def A(self):
    """Get areas.

    Parameters
    ----------
    None

    Returns
    -------
    numpy.array:
      Areas.  

    Examples
    --------
    >>> from pyDtOO import dtField
    >>> from pyDtOO.dtField import _test_vecField_as_file
    >>> f = dtValueField( dtField( _test_vecField_as_file() ).Read() )
    >>> f.A()
    array([0.18708287, 1.9065676 , 3.63799395])
    """
     
    return self.a_
  
  def IntQ(self):
    """Get integrated discharge.

    Parameters
    ----------
    None

    Returns
    -------
    float:
      Integral of discharges.  

    Examples
    --------
    >>> from pyDtOO import dtField
    >>> from pyDtOO.dtField import _test_vecField_as_file
    >>> f = dtValueField( dtField( _test_vecField_as_file() ).Read() )
    >>> f.IntQ()
    np.float64(600.0)
    """
     
    return numpy.sum( self.q_ )

  def IntA(self):
    """Get integrated area.

    Parameters
    ----------
    None

    Returns
    -------
    float:
      Integral of Area.  

    Examples
    --------
    >>> import math
    >>> from pyDtOO import dtField
    >>> from pyDtOO.dtField import _test_vecField_as_file
    >>> f = dtValueField( dtField( _test_vecField_as_file() ).Read() )
    >>> math.isclose( f.IntA(), 5.73164, rel_tol=1.e-5)
    True
    """
     
    return numpy.sum( self.a_ )
  
  def IntValueQ(self):
    """Get integrated value times discharge.

    Parameters
    ----------
    None

    Returns
    -------
    numpy.array:
      Integral of value times discharge.  

    Examples
    --------
    >>> from pyDtOO import dtField
    >>> from pyDtOO.dtField import _test_vecField_as_file
    >>> f = dtValueField( dtField( _test_vecField_as_file() ).Read() )
    >>> f.IntValueQ()
    array([14300., 14900., 15200.])
    >>> from pyDtOO.dtField import _test_scaField_as_file
    >>> f = dtValueField( dtField( _test_scaField_as_file() ).Read() )
    >>> f.IntValueQ()
    array([14900.])
    """
     
    ret = numpy.zeros( self.vDim_ )
    logging.info('vDim = %d', self.vDim_)
    for i in range(0, self.vDim_):
      ret[i] = numpy.dot( self.value_[:,i], self.q_[:,0] )
    return ret

  def IntMagQ(self):
    """Get integrated absolute discharge.

    Parameters
    ----------
    None

    Returns
    -------
    numpy.array:
      Integral of absolute discharge.  

    Examples
    --------
    >>> from pyDtOO import dtField
    >>> from pyDtOO.dtField import _test_vecField_as_file
    >>> f = dtValueField( dtField( _test_vecField_as_file() ).Read() )
    >>> f.IntMagQ()
    np.float64(25643.702315410068)
    """
     
    return numpy.dot( 
      numpy.sqrt(numpy.sum( self.value_[:,:]**2., axis=1 )), self.q_[:,0]
    )

  def IntMagSquareQ(self):
    """Get integrated squared discharge.

    Parameters
    ----------
    None

    Returns
    -------
    numpy.array:
      Integral of squared discharge.  

    Examples
    --------
    >>> from pyDtOO import dtField
    >>> from pyDtOO.dtField import _test_vecField_as_file
    >>> f = dtValueField( dtField( _test_vecField_as_file() ).Read() )
    >>> f.IntMagSquareQ()
    np.float64(1195900.0)
    """
     
    return numpy.dot( numpy.sum( self.value_[:,:]**2., axis=1 ), self.q_[:,0] )
  
  def IntValueA(self):
    """Get integrated value times area.

    Parameters
    ----------
    None

    Returns
    -------
    numpy.array:
      Integral of value times area.  

    Examples
    --------
    >>> from pyDtOO import dtField
    >>> from pyDtOO.dtField import _test_vecField_as_file
    >>> f = dtValueField( dtField( _test_vecField_as_file() ).Read() )
    >>> f.IntValueA()
    array([152.00782141, 157.73946583, 160.60528804])
    >>> from pyDtOO.dtField import _test_scaField_as_file
    >>> f = dtValueField( dtField( _test_scaField_as_file() ).Read() )
    >>> f.IntValueA()
    array([157.73946583])
    """
     
    ret = numpy.zeros( self.vDim_ )
    logging.info('vDim = %d', self.vDim_)
    for i in range(0, self.vDim_):
      for j in range( len(self.q_) ):
        ret[i] = ret[i] + self.value_[j,i] * numpy.linalg.norm( self.sf_[j,:] )      
    return ret
    
  def SIntValueA(self):
    """Get integrated value times surface vector.

    Parameters
    ----------
    None

    Returns
    -------
    numpy.array:
      Integral of value times surface vector.  

    Examples
    --------
    >>> from pyDtOO import dtField
    >>> from pyDtOO.dtField import _test_vecField_as_file
    >>> f = dtValueField( dtField( _test_vecField_as_file() ).Read() )
    >>> f.SIntValueA()
    array([[84.575, 87.65 , 90.725],
           [87.725, 90.95 , 94.175],
           [89.3  , 92.6  , 95.9  ]])
    >>> from pyDtOO.dtField import _test_scaField_as_file
    >>> f = dtValueField( dtField( _test_scaField_as_file() ).Read() )
    >>> f.SIntValueA()
    array([[87.725, 90.95 , 94.175]])
    """
     
    ret = numpy.zeros( (self.vDim_, 3) )
    logging.info('vDim = %d', self.vDim_)
    for i in range(0, self.vDim_):
      ret[i,:] = numpy.sum( self.value_[:,i:i+1] * self.sf_, axis=0 )
    return ret
  
  def VDim(self):
    """Get field dimension.

    Parameters
    ----------
    None

    Returns
    -------
    int:
      Dimension of field (values).  

    Examples
    --------
    >>> from pyDtOO import dtField
    >>> from pyDtOO.dtField import _test_vecField_as_file
    >>> f = dtValueField( dtField( _test_vecField_as_file() ).Read() )
    >>> f.VDim()
    3
    >>> from pyDtOO.dtField import _test_scaField_as_file
    >>> f = dtValueField( dtField( _test_scaField_as_file() ).Read() )
    >>> f.VDim()
    1
    """

    return self.vDim_
  
  def NumValues(self):
    """Get number of values in the field.

    Parameters
    ----------
    None

    Returns
    -------
    int:
      Number of values in the field.  

    Examples
    --------
    >>> from pyDtOO import dtField
    >>> from pyDtOO.dtField import _test_vecField_as_file
    >>> f = dtValueField( dtField( _test_vecField_as_file() ).Read() )
    >>> f.NumValues()
    3
    """
     
    return self.nValues_
