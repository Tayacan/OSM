

import numpy as np
import matplotlib.pyplot as mpl

xs = [1,2,3,4,5,6,7,8,9,10,50,100]

class Statistics:
  def __init__(self, data):
    self.data = data
    self.amount = len(data) 
    self.mean = sum(self.data)/self.amount
    self.err = []
    for i in range(self.amount):
      self.err.append( (self.data[i] - self.mean)**2 )
    self.std = sum(self.err)/(self.amount-1)**0.5
    self.stdm = self.std/(self.amount-1)**0.5
    
  def makeXs(self,x):
    result = []
    for i in range(self.amount):
      result.append(x)
    return result

  def __str__(self):
    return str(self.data)

def loadData(fileName):
  data = []
  with open(fileName,'r') as f:
    for x in xs:
      subdata = []
      for i in range(10):
        f.readline()
        line = f.readline()
        subdata.append(float(line[12:]))
        
      #data.append(subdata)
      data.append(Statistics(subdata))
  return data




#print loadData('results.txt')

data1 = loadData('results.txt')
data2 = loadData('result1.txt')

#print data1


if True:
  mpl.hold(True)
  means = []
  stdErrUp = []
  stdErrDown = []
  stdErrMeanUp = []
  stdErrMeanDown = []
  means2 = []
  stdErrUp2 = []
  stdErrDown2 = []
  stdErrMeanUp2 = []
  stdErrMeanDown2 = []
  for i in range(len(xs)):
    txs = data1[i].makeXs(xs[i])
    mpl.plot(txs,data1[i].data,'r.')
    mpl.plot(txs,data2[i].data,'b.')
    
    means.append(data1[i].mean)
    stdErrUp.append(data1[i].mean + data1[i].std)
    stdErrDown.append(data1[i].mean - data1[i].std)
    stdErrMeanUp.append(data1[i].mean + data1[i].stdm)
    stdErrMeanDown.append(data1[i].mean - data1[i].stdm)
    
    means2.append(data2[i].mean)
    stdErrUp2.append(data2[i].mean + data2[i].std)
    stdErrDown2.append(data2[i].mean - data2[i].std)
    stdErrMeanUp2.append(data2[i].mean + data2[i].stdm)
    stdErrMeanDown2.append(data2[i].mean - data2[i].stdm)
    
    #print min(data1[i].data) , max(data1[i].data)
    #print min(data2[i].data) , max(data2[i].data)
    
  mpl.plot(xs, means,'r-', label='mean, 4c')
  mpl.plot(xs, stdErrUp,'r:', label='std, 4c')
  mpl.plot(xs, stdErrDown,'r:')
  mpl.plot(xs, stdErrMeanUp,'r-.', label='stdmean, 4c')
  mpl.plot(xs, stdErrMeanDown,'r-.')
  
  mpl.plot(xs, means2,'b-', label='mean, 4c ht')
  mpl.plot(xs, stdErrUp2,'b:', label='std, 4c ht')
  mpl.plot(xs, stdErrDown2,'b:')
  mpl.plot(xs, stdErrMeanUp2,'b-.', label='stdmean, 4c ht')
  mpl.plot(xs, stdErrMeanDown2,'b-.')
  
  mpl.hold(False)
  mpl.xlabel('threads')
  mpl.ylabel('computation time')
  a = mpl.gca()
  #mpl.set(mpl.gca(), 'XScale', 'log')
  a.set_xscale('log')
  mpl.legend()
  mpl.show()
