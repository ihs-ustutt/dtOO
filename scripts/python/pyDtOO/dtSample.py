import numpy
import logging
import os
import glob
import re

class dtSample:
  def __init__(self, folder, userDefined = ["none"]):
    self.folder_ = folder
    self.userDefined_ = userDefined

  def Read(self):
    data  = dict()

    # Get timesteps and sorted timesteps
    timesteps = glob.glob('./' + self.folder_ + '/*')
    timestepsForSorting = numpy.zeros(numpy.size(timesteps))
    for k in range(numpy.size(timesteps)):
      timesteps[k] = re.sub('^.*/', '', timesteps[k])
      timestepsForSorting[k] = float(timesteps[k])
    timestepsSort =  numpy.sort(timestepsForSorting)

    # Get the names of the samples
    sampleNames  = glob.glob('./'+ self.folder_  + '/' + timesteps[0]+'/*')
    for k in range(numpy.size(sampleNames )):
      sampleNames[k] = re.sub('^.*/', '',sampleNames[k])

    # Build the dict with the sampled variables
    variablesDict = dict()
    variables = ["_p", "_epsilon", "_k", "_U", "_omega"]
    if self.userDefined_[0] != "none":
      variables = numpy.concatenate((variables, self.userDefined_))
      for k in range(numpy.size(variables)):
        variablesDict[k] = variables[k]
    else:
      for k in range(numpy.size(variables)):
        variablesDict[k] = variables[k]

    # Loop over all samples
    for sample in range(numpy.size(sampleNames)):
      variablesName = dict()
      variablesPosition = numpy.zeros(numpy.size(variables))
      
      # Find number of variables in sample
      numberOfVariables = 0
      for k in range(numpy.size(variables)):
        if sampleNames[sample].find(variablesDict[k]) != -1:
          numberOfVariables = numberOfVariables + 1

      # Get the order of the variables equal to data---------------------------------------------
      for k in range(numpy.size(variables)):
        variableName = variablesDict[k]
        if sampleNames[sample].find(variableName) != -1:
          variablesName[sampleNames[sample][sampleNames[sample].find(variableName)+1:sampleNames \
          [sample].find(variableName)+len(variableName)]] = sampleNames[sample].find(variableName)
          variablesPosition[k] = sampleNames[sample].find(variableName)
      # Position of the variables in the name of the sample
      variablesPositionSort = numpy.sort(variablesPosition)[- numberOfVariables:] 
      # Dict to store variables in the same order as in the name of the sample
      variablesNameSort = dict() 

      for p in range(len(variablesName)):
        for k in variablesName:
          if variablesName[k] == variablesPositionSort[p]:
            variablesNameSort[p] = k
      # Get the order of the variables equal to data finished------------------------------------

      scalar = bool(1)
      pfad = os.path.join(self.folder_, timesteps[0], sampleNames[sample])

      # Check if sample is a line or sa point
      if numpy.genfromtxt(pfad, delimiter = '').ndim == 1: # True for points 
        # Check if sample is a scalar or a vector
        if (numpy.size(numpy.genfromtxt(pfad, delimiter = ''))>3+numberOfVariables): # false for vectors
          scalar = bool(0)
        counter = 0
        # Loop over the variable names in the order equal to data and save the data in the dict
        for k in variablesNameSort.itervalues():
            counter = counter + 1
            if scalar:
              data[sampleNames[sample],k] = numpy.zeros((numpy.size(timesteps), 5))
              for time in range(numpy.size(timesteps)):
                if timestepsSort[1]-timestepsSort[0] == 1.0:
                  pfad = os.path.join(self.folder_ , str(int(timestepsSort[time])), sampleNames[sample])
                else:
                  pfad = os.path.join(self.folder_ , str(timestepsSort[time]), sampleNames[sample])
                dataRead  = numpy.genfromtxt(pfad, delimiter = '')
                for d in range (0,5):
                   if d == 0:
                     data[sampleNames[sample],k][time,d] = timestepsSort[time]
                   elif d == 1 or d == 2 or d == 3:
                     data[sampleNames[sample],k][time,d] = dataRead[d-1]
                   else:
                     data[sampleNames[sample],k][time,d] = dataRead[2+counter]
            else:
              data[sampleNames[sample],k] = numpy.zeros((numpy.size(timesteps), 7))
              for time in range(numpy.size(timesteps)):
                if timestepsSort[1]-timestepsSort[0] == 1.0:
                  pfad = os.path.join(self.folder_ , str(int(timestepsSort[time])), sampleNames[sample])
                else:
                  pfad = os.path.join(self.folder_ , str(timestepsSort[time]), sampleNames[sample])
                dataRead = numpy.genfromtxt(pfad, delimiter = '')
                for d in range (0,7):
                  if d == 0:
                    data [sampleNames[sample],k][time,d] = timestepsSort[time]
                  elif d == 1 or d == 2 or d == 3:
	                  data[sampleNames[sample],k][time,d] = dataRead[d-1]
                  else:
                    data[sampleNames[sample],k][time,d] = dataRead[(d-1)+3*(counter-1)]

      else: # In the case of a line
        scalar = bool(1)
        if numpy.size(numpy.genfromtxt(pfad, delimiter = '')[0,:])>1+numberOfVariables:
          scalar = bool(0)
        numberOfLines = numpy.size(numpy.genfromtxt(pfad, delimiter = '')[:,0])
        counter = 0
        # Loop over the variable names in the order equal to data and save the data in the dict
        for k in variablesNameSort.itervalues():
          counter = counter + 1
          if scalar:
            data[sampleNames[sample],k] = numpy.zeros((numpy.size(timesteps), numberOfLines, 2))
            for time in range(numpy.size(timesteps)):
              if timestepsSort[1]-timestepsSort[0] == 1.0:
                pfad = os.path.join(self.folder_, str(int(timestepsSort[time])), sampleNames[sample])
              else:
                pfad = os.path.join(self.folder_, str(timestepsSort[time]), sampleNames[sample])

              dataRead = numpy.genfromtxt(pfad, delimiter = '')
              for d in range (0,2):
                if d == 0:
                  data[sampleNames[sample],k][time,:,d] = dataRead[:,d]
                else:
                  data[sampleNames[sample],k][time,:,d] = dataRead[:,counter]
          else:
            data[sampleNames[sample],k] = numpy.zeros((numpy.size(timesteps), numberOfLines, 4))
            for time in range(numpy.size(timesteps)):
              if timestepsSort[1]-timestepsSort[0] == 1.0:
                pfad = os.path.join(self.folder_ , str(int(timestepsSort[time])), sampleNames[sample])
              else:
                pfad = os.path.join(self.folder_ , str(timestepsSort[time]), sampleNames[sample])
              dataRead = numpy.genfromtxt(pfad, delimiter = '')
              for d in range (0,4):
                if d == 0:
                  data[sampleNames[sample],k][time,:,d] = dataRead[:,d]
                else:
                  data[sampleNames[sample],k][time,:,d] = dataRead[:,counter*3-3+d]
    return data 
