from numpy import linspace as linspace
from matplotlib.pyplot import*
from pylab import *

def plotting(f,a,b):
    OX=linspace(a, b, 1000, endpoint=True)


    OY=[]
 
    for OX_ in OX:
        OY.append(f(OX_))
    plot(OX,OY)
    show()
    return

print "I am here"	
	
#OY=array(OY)	
#OX=array(OX)
#subplot(211)
plot(OX,OY,label="Pressure drop gradient separated flow")
#plot(OX,OY1,label="Pressure drop gradient homogeneous flow")
plt.legend(bbox_to_anchor=(0., 1.02, 1., .102), loc=3,
           ncol=1, mode="expand", borderaxespad=0.)
ylabel('Presseure drop gradient, kPa/m')
xlabel('Distance along the pipe, m')
#subplot(212)
#plot(OX,VoidFractionSep,label="Void fraction separated flow")
#plot(OX,VoidFractionHomo,label="Void fraction homogeneous flow")
#plot(OX,Quality_graph,label="Quality")
#plt.legend(bbox_to_anchor=(0., 1.02, 1., .102), loc=3,
#           ncol=1, mode="expand", borderaxespad=0.)
#ylabel('Fraction')
#xlabel('Distance along the pipe, m')
show()