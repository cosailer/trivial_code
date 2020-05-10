# -*- coding: utf-8 -*-
"""
-------------------------------------
 N A C S   P Y T H O N   S C R I P T 
-------------------------------------

NACS version: 2.0.2745 - pre3
NACS architecture: CENTOS 5.11 (X86_64)
File generated at Tue Jan 20 16:55:05 2015
On host 'lse86' by 'cae42'
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
simulation.setGrid(u'project3.nmf', 'plane')

simulation.addOutput(Output.Nacs())
text = Output.Text()
simulation.addOutput(text)
simulation.addOutput(Output.GiD())


# =====================
#  MATERIAL DEFINITION
# =====================
copper = Material('Copper')
copper.density(8940.0)
copper.lossTangensDelta([1000],[0.002])
copper.stiffness.isotropic.byENu(1.15e+11, 0.35)

steel = Material('Steel')
steel.density(6280)
steel.lossTangensDelta([1000],[0.0003])
steel.stiffness.isotropic.byENu(1.95e+11, 0.28)

silicon = Material('Silicon')
silicon.density(2300.0)
silicon.stiffness.isotropic.byENu(67500000000.0, 0.1)


simulation.setMat('exc_f_r', copper)
simulation.setMat('rec_f_r', copper)
simulation.setMat('sen_coat_r', steel)
simulation.setMat('silicon_r', silicon)

# ===============
#  ANALYSIS STEP 
# ===============
trans1 = Analysis.Transient()
trans1.set(3.6417e-11, None, 300, False)

mech1 = Physic.Mechanic('planeStrain')
mech1.addRegions(['exc_f_r', 'sen_coat_r', 'silicon_r', 'rec_f_r'])
mech1.addBc(mech1.BC.Force.expr('exc_f_r', 'y', "-1000*sinBurst(1.3730e+09, 5, 1, 1 ,t)"))
mech1.addBc(mech1.BC.Fix('outerbounds_bot', ['x', 'y']))
mech1.addResult(mech1.Result.Displacement(['exc_f_r', 'rec_f_r', 'sen_coat_r', 'silicon_r']))
mech1.addResult(mech1.Result.Displacement(['observer_point_1', 'observer_point_2', 'observer_point_3', 'observer_point_4', 'observer_point_e4'], 'amplPhase', 'mesh', [text]))
trans1.addPhysic(mech1)

simulation.addAnalysis(trans1)

