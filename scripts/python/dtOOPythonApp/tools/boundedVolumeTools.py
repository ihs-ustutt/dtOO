from dtOOPythonSWIG import baseContainer
from dtOOPythonSWIG import labeledVectorHandlingConstValue
from dtOOPythonSWIG import labeledVectorHandlingAnalyticFunction
from dtOOPythonSWIG import labeledVectorHandlingAnalyticGeometry
from dtOOPythonSWIG import labeledVectorHandlingBoundedVolume
from dtOOPythonSWIG import labeledVectorHandlingDtCase
from .generalTools import generalStrategy

from typing import Callable
from abc import ABC,abstractmethod

from .decoratorTools import checkDecorator

class boundedVolumeStrategy(generalStrategy):
  @abstractmethod
  def apply(
    self,
    bC: baseContainer,
    cV: labeledVectorHandlingConstValue,
    aF: labeledVectorHandlingAnalyticFunction,
    aG: labeledVectorHandlingAnalyticGeometry,
    bV: labeledVectorHandlingBoundedVolume,
    dC: labeledVectorHandlingDtCase
  ) -> labeledVectorHandlingBoundedVolume:
    pass


class boundedVolumeStrategyFactory:
  REGISTRY = {}

  @classmethod
  def create(self, name: str) -> boundedVolumeStrategy:
    strategy_class = self.REGISTRY[name]
    strategy = strategy_class()
    return strategy

  @classmethod
  def register(self, name: str) -> Callable:
    def inner_wrapper(wrapped_class: boundedVolumeStrategy) -> Callable:
      nameStr = name
      if nameStr=="":
        nameStr = wrapped_class.__name__
      self.REGISTRY[nameStr] = wrapped_class
      return wrapped_class

    return inner_wrapper


class boundedVolumeComponent(ABC):
  def __init__(self, strategy: boundedVolumeStrategy) -> None:
    self._strategy = strategy

  @property
  def strategy(self) -> boundedVolumeStrategy:
    return self._strategy

  @strategy.setter
  def strategy(self, strategy: boundedVolumeStrategy) -> None:
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
  ) -> labeledVectorHandlingBoundedVolume:
    return self._strategy.apply(bC, cV, aF, aG, bV, dC)
