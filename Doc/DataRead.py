import re
import matplotlib.pyplot as plt
import numpy as np

List = []
with open("SerialData.txt", 'r') as File:
    for i in range(800):
        List += [str(File.readline())]

NewList = []
for Elmt in List:
    NewList += re.findall("TemperValue:\d{2}\.\d{6}", Elmt)

TemperValueList = []
for List in NewList:
    TempList = re.split(':', List)
    TemperValueList += [float(TempList[1])]

x = np.arange(0, len(TemperValueList), 1)
y = np.array(TemperValueList)
plt.axis([0, 400, 20, 35])

# plt.ylim xlim

plt.plot(x, y)

plt.show()
