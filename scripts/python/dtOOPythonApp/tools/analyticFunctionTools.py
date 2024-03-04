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

from dtOOPythonSWIG import baseContainer
from dtOOPythonSWIG import labeledVectorHandlingConstValue
from dtOOPythonSWIG import labeledVectorHandlingAnalyticFunction
from dtOOPythonSWIG import labeledVectorHandlingAnalyticGeometry
from dtOOPythonSWIG import labeledVectorHandlingBoundedVolume
from dtOOPythonSWIG import labeledVectorHandlingDtCase
from dtOOPythonSWIG import scaOneD
from .generalTools import generalStrategy

from typing import Callable
from abc import ABC,abstractmethod

from .decoratorTools import checkDecorator

class analyticFunctionStrategy(generalStrategy):
  @abstractmethod
  def apply(
    self,
    bC: baseContainer,
    cV: labeledVectorHandlingConstValue,
    aF: labeledVectorHandlingAnalyticFunction,
    aG: labeledVectorHandlingAnalyticGeometry,
    bV: labeledVectorHandlingBoundedVolume,
    dC: labeledVectorHandlingDtCase
  ) -> labeledVectorHandlingAnalyticFunction:
    pass


class analyticFunctionStrategyFactory:
  REGISTRY = {}

  @classmethod
  def create(self, name: str) -> analyticFunctionStrategy:
    strategy_class = self.REGISTRY[name]
    strategy = strategy_class()
    return strategy

  @classmethod
  def register(self, name: str) -> Callable:
    def inner_wrapper(wrapped_class: analyticFunctionStrategy) -> Callable:
      nameStr = name
      if nameStr=="":
        nameStr = wrapped_class.__name__
      self.REGISTRY[nameStr] = wrapped_class
      return wrapped_class

    return inner_wrapper


class analyticFunctionComponent(ABC):
  def __init__(self, strategy: analyticFunctionStrategy) -> None:
    self._strategy = strategy

  @property
  def strategy(self) -> analyticFunctionStrategy:
    return self._strategy

  @strategy.setter
  def strategy(self, strategy: analyticFunctionStrategy) -> None:
    self._strategy = strategy

  @checkDecorator
  def apply(
    self,
    bC: baseContainer,
    cV: labeledVectorHandlingConstValue,
    aF: labeledVectorHandlingAnalyticFunction,
    aG: labeledVectorHandlingAnalyticGeometry,
    bV: labeledVectorHandlingBoundedVolume,
    dC: labeledVectorHandlingDtCase
  ) -> labeledVectorHandlingAnalyticFunction:
    return self._strategy.apply(bC, cV, aF, aG, bV, dC)

class scaOneDBuilder(ABC):
  @abstractmethod
  def build( self ) -> scaOneD:
    pass

  def __call__( self ) -> scaOneD:
    return self.build()
