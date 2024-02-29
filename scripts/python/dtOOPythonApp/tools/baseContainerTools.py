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

class baseContainerStrategy(generalStrategy):
  @abstractmethod
  def apply(
    self,
    bC: baseContainer,
    cV: labeledVectorHandlingConstValue,
    aF: labeledVectorHandlingAnalyticFunction,
    aG: labeledVectorHandlingAnalyticGeometry,
    bV: labeledVectorHandlingBoundedVolume,
    dC: labeledVectorHandlingDtCase
  ) -> baseContainer:
    pass


class baseContainerStrategyFactory:
  REGISTRY = {}

  @classmethod
  def create(self, name: str) -> baseContainerStrategy:
    strategy_class = self.REGISTRY[name]
    strategy = strategy_class()
    return strategy

  @classmethod
  def register(self, name: str) -> Callable:
    def inner_wrapper(wrapped_class: baseContainerStrategy) -> Callable:
      nameStr = name
      if nameStr=="":
        nameStr = wrapped_class.__name__
      self.REGISTRY[nameStr] = wrapped_class

      return wrapped_class

    return inner_wrapper

class baseContainerComponent(ABC):
  def __init__(self, strategy: baseContainerStrategy) -> None:
    self._strategy = strategy

  @property
  def strategy(self) -> baseContainerStrategy:
    return self._strategy

  @strategy.setter
  def strategy(self, strategy: baseContainerStrategy) -> None:
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
  ) -> baseContainer:
    return self._strategy.apply(bC, cV, aF, aG, bV, dC)
