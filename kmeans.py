import math
import sys
import random


class Kmeans:

    def __init__(self, i_numberOfK, i_inputData,i_iter =200):
        self.m_numberOfK = i_numberOfK
        self.e = 0.001
        self.m_maxIter = i_iter
        self.m_iterationCount = 0
        self.m_inputData = i_inputData
        self.m_dataPoint = []
        self.m_kCenters = []
        self.m_bigestDif = -1
        self.readDataInput()
        self.initializeSelectOfKCenter()

    def readDataInput(self):
        with open(self.m_inputData, 'r') as file:
            while True:
                line = file.readline()
                if not line:
                    break
                point = []
                for num in line.replace("\n","").split(","):
                    point.append(float(num))
                self.m_dataPoint.append(point)

    def initializeSelectOfKCenter(self):
        kCentersPoint = self.m_dataPoint[:self.m_numberOfK]
        for kCenter in kCentersPoint:
            self.m_kCenters.append(Center(kCenter))

    def iteration(self):
        self.m_bigestDif = -1
        self.m_iterationCount += 1
        self.assignAllPoint()
        self.updateAllKCenter()

    def calculateDistance(self,xArray,yArry):
        sum = 0
        for i in range(len(xArray)):
            dif = xArray[i] - yArry.m_kLocation[i]
            sum += pow(dif,2)
        return math.sqrt(sum)

    def assignXToClosestK(self,xArray):
        minDiff = sys.maxsize
        closestK = None
        for k in self.m_kCenters:
            tempDif =self.calculateDistance(xArray,k)
            if tempDif<minDiff:
                closestK = k
                minDiff = tempDif
        closestK.addDataPoint(xArray)
        return minDiff

    def assignAllPoint(self):
        for k in self.m_kCenters:
            k.clearDataPoint()
        for point in self.m_dataPoint:
            tempDif = self.assignXToClosestK(point)
            if tempDif>self.m_bigestDif:
                self.m_bigestDif = tempDif

    def updateAllKCenter(self):
        for k in self.m_kCenters:
            k.updateLocationOfK()

    def isAlgoEnd(self):
        return self.ifDoneLeftItertion() or self.ifWeCloseEnough()

    def ifDoneLeftItertion(self):
        return self.m_iterationCount >= self.m_maxIter

    def ifWeCloseEnough(self):
        if self.m_bigestDif != -1:
            return self.m_bigestDif<self.e
        return False

    def printCenters(self):
        for k in self.m_kCenters:
            for i in range(len(k.m_kLocation)):
                if i < len(k.m_kLocation)-1:
                    print(f"{k.m_kLocation[i]:.4f}",end=",")
                else:
                    print(f"{k.m_kLocation[i]:.4f}")

class Center:
    def __init__(self,i_kLocation):
        self.m_kLocation = i_kLocation
        self.m_kDataPoint = []
        self.m_pointlen = len(self.m_kLocation)

    def updateLocationOfK(self):
        if len(self.m_kDataPoint) > 0:
            for i in range(self.m_pointlen):
                sum = 0
                for point in self.m_kDataPoint:
                    sum += point[i]
                sum /= len(self.m_kDataPoint)
                self.m_kLocation[i] = sum

    def clearDataPoint(self):
        self.m_kDataPoint = []


    def addDataPoint(self,i_point):
        self.m_kDataPoint.append(i_point)

def main():
    if(len(sys.argv) == 4):
        kmeans = Kmeans(int(sys.argv[1]),sys.argv[3],int(sys.argv[2]))
    else:
        kmeans = Kmeans(int(sys.argv[1]),sys.argv[2])

    while not kmeans.isAlgoEnd():
        kmeans.iteration()
    kmeans.printCenters()
# Press the green button in the gutter to run the script.
if __name__ == '__main__':
    main()

# See PyCharm help at https://www.jetbrains.com/help/pycharm/
