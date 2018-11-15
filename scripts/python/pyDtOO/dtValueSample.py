import numpy

class dtValueSample:
  def __init__(self, data):
    self.data_ = data

  def GetValueSample(self, sampleName, variable):
    for k in self.data_:
      if variable in k:
        if k[0].find(sampleName) != -1:
          return self.data_[k]
