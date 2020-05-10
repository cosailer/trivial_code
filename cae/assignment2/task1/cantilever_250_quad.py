# -*- coding: utf-8 -*-
"""
-------------------------------------
 N A C S   P Y T H O N   S C R I P T 
-------------------------------------

NACS version: 2.0.2570 - pre2
NACS architecture: CENTOS 5.10 (X86_64)
File generated at Fri Nov 14 17:23:26 2014
On host 'lse87' by 'cae42'
"""

from __future__ import division  
try:
  from nacs.scripting import *
except:
  raise Exception("File is only executable in the NACS python interpreter!")

# =================
#  NACS SIMULATION 
# =================
simulation = NacsSimulation()
simulation.setGrid('cantilever_250_quad.nmf', 'plane')

simulation.addOutput(Output.Nacs())
text = Output.Text()
simulation.addOutput(text)


# =====================
#  MATERIAL DEFINITION
# =====================
steel = Material('Steel')
steel.density(7850.0)
steel.lossTangensDelta([1000],[0.0003])
steel.stiffness.isotropic.byENu(1.95e+11, 0.28)


simulation.setMat('cantilever', steel)

# ===============
#  ANALYSIS STEP 
# ===============
modal1 = Analysis.Modal()
modal1.set(3, True, 1)

mech14 = Physic.Mechanic('planeStrain')
mech14.addRegions(['cantilever'])
mech14.addBc(mech14.BC.Fix('left_edge', ['x', 'y']))
modal1.addPhysic(mech14)

simulation.addAnalysis(modal1)

