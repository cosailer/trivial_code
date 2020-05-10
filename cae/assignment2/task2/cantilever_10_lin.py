# -*- coding: utf-8 -*-
"""
-------------------------------------
 N A C S   P Y T H O N   S C R I P T 
-------------------------------------

NACS version: 2.0.2570 - pre2
NACS architecture: CENTOS 5.10 (X86_64)
File generated at Fri Nov 14 18:22:09 2014
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
simulation.setGrid('cantilever_10_lin.nmf', 'plane')

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
harm1 = Analysis.Harmonic()
harm1.set(100, 30.0, 570.0, 'log')

mech4 = Physic.Mechanic('planeStrain')
mech4.addRegions(['cantilever'])
mech4.addBc(mech4.BC.Fix('left_edge', ['x', 'y']))
mech4.addBc(mech4.BC.Force.expr('bot_right', 'y', "-5"))
mech4.addResult(mech4.Result.Displacement('top_right', 'amplPhase', 'mesh', [text]))
harm1.addPhysic(mech4)

simulation.addAnalysis(harm1)

