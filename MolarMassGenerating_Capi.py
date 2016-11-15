import numpy
import scipy.optimize as optimize
import matplotlib.pylab as plt
from scipy.integrate import quad
from scipy.stats import gamma as gamma
from numpy import linspace as linspace
from matplotlib.pyplot import*
from scipy.optimize import curve_fit as curve_fit

print "I am here1"
print "Generating with SIZE=",SIZE,"Shape=",alpha,"Bound=",loc,"Scale=",beta
print "I am here2"
SIZE=SIZE+1
#alpha = 5.
#loc = 2.
#beta = 20
print "I am here3"
data2=2
print "I am here4"
data = gamma.rvs(alpha, loc=loc, scale=beta, size=SIZE)  
  
data.sort()

MaxMolarMass=data[SIZE-1]*1.5



print "Molar Masses"
print(data)

MassFractions=[]
MolarMasses=[]
MolFractions=[]
UpperBoundMolarMass=numpy.array(data)

pargs=(alpha,loc,beta)

def MolarMassIntegrand(x,p1,p2,p3):
	return gamma.pdf(x,p1,p2,p3)*x

MolarMassAve=quad(MolarMassIntegrand,0.,MaxMolarMass,args=pargs)[0]

TotalMass=0

for i in range(SIZE-1):
	MolarMasses.append(UpperBoundMolarMass[i])
#	print MolarMasses[i],UpperBoundMolarMass[i+1]
	Int1=quad(gamma.pdf,MolarMasses[i],UpperBoundMolarMass[i+1],args=pargs)[0]
#	print "molesfraction=",Int1
	Int2=quad(MolarMassIntegrand,MolarMasses[i],UpperBoundMolarMass[i+1],args=pargs)[0]/Int1
#	print "Molarmasses",Int2
	MolFractions.append(Int1)
	MassFractions.append(Int1*Int2/MolarMassAve)
	TotalMass=TotalMass+Int1*Int2/MolarMassAve




print "sum of gamma", quad(gamma.pdf,0.,MaxMolarMass,args=pargs)[0]
print "Total sum of MassFractions=",TotalMass
print "MolarMassAve=",MolarMassAve

OX=linspace(0.,MaxMolarMass,1000)
OY=[gamma.pdf(x,alpha, loc=loc, scale=beta) for x in OX]



#OY=array(OY)	
#OX=array(OX)



subplot(211)
plot(OX,OY,label="Molar Mass Gamma Distribution")

plt.legend(bbox_to_anchor=(0., 1.02, 1., .102), loc=3,
           ncol=1, mode="expand", borderaxespad=0.)
ylabel('Probability Density')
xlabel('Molar Mass')

subplot(212)
#plot(OX,OY,label="Molar Mass Gamma Distribution")
plot(MolarMasses,MassFractions,label="Weight Fractions")
plt.legend(bbox_to_anchor=(0., 1.02, 1., .102), loc=3,
           ncol=1, mode="expand", borderaxespad=0.)
ylabel('Weight Fractions')
xlabel('Molar Mass')
show()


print "Mass Fractions Size=",len(MassFractions),"MassFractions"
print MassFractions	
print "Mol Fractions"
print MolFractions
print MolarMasses
print "data", data