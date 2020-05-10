# -*- coding: utf-8 -*-
"""
-------------------------------------
 N A C S   P Y T H O N   S C R I P T 
-------------------------------------

NACS version: 2.0.2745 - pre3
NACS architecture: CENTOS 5.11 (X86_64)
File generated at Fri Dec  5 14:46:21 2014
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
simulation.setGrid(u'assignment3.2_55.nmf', 'axi')

simulation.addOutput(Output.Nacs())
text = Output.Text()
simulation.addOutput(text)
simulation.addOutput(Output.GiD())


# =====================
#  MATERIAL DEFINITION
# =====================
air = Material('air')
air.density(1.205)
air.permeability.isotropic(1.25664e-06)
air.conductivity.isotropic(0.0)
air.compressionModulus(141650)

iron = Material('Iron')
iron.density(7100.0)
iron.lossTangensDelta([1000],[0.0003])
iron.stiffness.isotropic.byENu(2.06e+11, 0.28)
iron.permeability.isotropic(0.0018333)
iron.conductivity.isotropic(100000.0)


simulation.setMat('air', air)
simulation.setMat('coil', air)
simulation.setMat('core', iron)

# ===============
#  ANALYSIS STEP 
# ===============
static1 = Analysis.Static()

mag1 = Physic.Magnetic('node')
mag1.addRegions(['coil', 'air', 'core'])
coil = mag1.BC.Coil('coil', "0.001")
coil.addPart(coil.Part('coil', 50))
mag1.addBc(coil)
mag1.addBc(mag1.BC.FluxParallel(['outerbounds_bot', 'outerbounds_left', 'outerbounds_right', 'outerbounds_top']))
mag1.addResult(mag1.Result.CoilInductance([coil], 'amplPhase', 'history', [text]))
mag1.addResult(mag1.Result.FluxDensity(['air', 'coil', 'core']))
static1.addPhysic(mag1)

simulation.addAnalysis(static1)

