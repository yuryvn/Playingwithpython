import numpy
import scipy.optimize as optimize
import gc
import matplotlib.pylab
from scipy.integrate import quad
from scipy.stats import gamma as gamma
from numpy import linspace as linspace
#import matplotlib.pyplot as pyplot
from scipy.optimize import curve_fit as curve_fit
#import plotly.plotly as py



def delete_module(modname, paranoid=None):
    from sys import modules
    try:
        thismod = modules[modname]
    except KeyError:
        raise ValueError(modname)
    these_symbols = dir(thismod)
    if paranoid:
        try:
            paranoid[:]  # sequence support
        except:
            raise ValueError('must supply a finite list for paranoid')
        else:
            these_symbols = paranoid[:]
    del modules[modname]
    for mod in modules.values():
        try:
            delattr(mod, modname)
        except AttributeError:
            pass
        if paranoid:
            for symbol in these_symbols:
                if symbol[:2] == '__':  # ignore special symbols
                    continue
                try:
                    delattr(mod, symbol)
                except AttributeError:
                    pass


#print "I am here1"
print "Generating with SIZE=",SIZE,"Shape=",alpha,"Bound=",loc,"Scale=",beta
#print "I am here2"
SIZE=SIZE+1
#alpha = 5.
#loc = 2.
#beta = 20
#print "I am here3"
data2=2
#print "I am here4"
data = gamma.rvs(alpha, loc=loc, scale=beta, size=SIZE)  
  
data.sort()

MaxMolarMass=data[SIZE-1]*1.5



#print "Molar Masses"
#print(data)

MassFractions=[]
MolarMasses=[]
MolFractions=[]
UpperBoundMolarMass=numpy.array(data)

pargs=(alpha,loc,beta)

def MolarMassIntegrand(x,p1,p2,p3):
	return gamma.pdf(x,p1,p2,p3)*x

MolarMassAve=quad(MolarMassIntegrand,0.,MaxMolarMass,args=pargs)[0]

TotalMass=0

#print "here?"
for i in range(SIZE-1):
	MolarMasses.append(UpperBoundMolarMass[i])
#	print MolarMasses[i],UpperBoundMolarMass[i+1]
	Int1=quad(gamma.pdf,MolarMasses[i],UpperBoundMolarMass[i+1],args=pargs)[0]
#	print "molesfraction=",Int1
	Int2=quad(MolarMassIntegrand,MolarMasses[i],UpperBoundMolarMass[i+1],args=pargs)[0]/Int1
#	print "Molarmasses",Int2
	print i
	MolFractions.append(Int1)
	MassFractions.append(Int1*Int2/MolarMassAve)
	TotalMass=TotalMass+Int1*Int2/MolarMassAve


#print "here?"

#print "sum of gamma", quad(gamma.pdf,0.,MaxMolarMass,args=pargs)[0]
#print "Total sum of MassFractions=",TotalMass
#print "MolarMassAve=",MolarMassAve

OX=linspace(0.,MaxMolarMass,1000)
OY=[gamma.pdf(x,alpha, loc=loc, scale=beta) for x in OX]



#OY=array(OY)	
#OX=array(OX)


fig=matplotlib.pylab.figure()

sub1=fig.add_subplot(211)
sub1.plot(OX,OY,label="Molar Mass Gamma Distribution")

sub1.legend(bbox_to_anchor=(0., 1.02, 1., .102), loc=3,
           ncol=1, mode="expand", borderaxespad=0.)

		   
sub1.set_ylabel('Probability Density')
sub1.set_xlabel('Molar Mass')



sub2=fig.add_subplot(212)
#plot(OX,OY,label="Molar Mass Gamma Distribution")
sub2.plot(MolarMasses,MassFractions,label="Weight Fractions")
sub2.legend(bbox_to_anchor=(0., 1.02, 1., .102), loc=3,
		   ncol=1, mode="expand", borderaxespad=0.)
sub2.set_ylabel('Weight Fractions')
sub2.set_xlabel('Molar Mass')
matplotlib.pylab.savefig('foo.png')

matplotlib.pylab.show(fig)

matplotlib.pylab.close(fig)
fig.clf
del fig
gc.collect()
delete_module("matplotlib.pylab")
'''
#plot_url = py.plot_mpl(fig, filename='Molarmasses')
#del fig

#wait(5)

#print "Mass Fractions Size=",len(MassFractions),"MassFractions"
#print MassFractions	
#print "Mol Fractions"
#print MolFractions
#print MolarMasses
print "data", data
'''