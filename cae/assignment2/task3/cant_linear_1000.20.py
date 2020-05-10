# -*- coding: utf-8 -*-
"""
-------------------------------------
 N A C S   P Y T H O N   S C R I P T 
-------------------------------------

NACS version: 2.0.2745 - pre3
NACS architecture: CENTOS 5.11 (X86_64)
File generated at Fri Nov 21 15:37:07 2014
On host 'lse82' by 'cae03'
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
simulation.setGrid(u'../tesk2/cantilever_1000_lin.nmf', 'plane')

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
trans1 = Analysis.Transient()
trans1.set(0.00024775, None, 200, False)

mech1 = Physic.Mechanic('planeStrain')
mech1.addRegions(['cantilever'])
mech1.addBc(mech1.BC.Fix('left_edge', ['x', 'y']))
mech1.addBc(mech1.BC.Force.expr('top_right', 'y', "-5*sinBurst(201.82, 3, 1, 1 ,t)"))
mech1.addResult(mech1.Result.Displacement('top_right', 'amplPhase', 'mesh', [text]))
trans1.addPhysic(mech1)

simulation.addAnalysis(trans1)

