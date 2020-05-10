# -*- coding: utf-8 -*-
"""
-------------------------------------
 N A C S   P Y T H O N   S C R I P T 
-------------------------------------

NACS version: 2.0.2745 - pre3
NACS architecture: CENTOS 5.11 (X86_64)
File generated at Fri Dec  5 12:59:12 2014
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
simulation.setGrid(u'assignment3_90.nmf', 'axi')

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


simulation.setMat('air', air)
simulation.setMat('coil', air)
simulation.setMat('core', air)

# ===============
#  ANALYSIS STEP 
# ===============
static1 = Analysis.Static()

mag1 = Physic.Magnetic('node')
mag1.addRegions(['coil', 'air', 'core'])
excoil = mag1.BC.Coil('excoil', "0.001")
excoil.addPart(excoil.Part('coil', 50))
mag1.addBc(excoil)
mag1.addBc(mag1.BC.FluxParallel(['outerbounds_bot', 'outerbounds_left', 'outerbounds_right', 'outerbounds_top']))
mag1.addResult(mag1.Result.VecPotential(['air', 'coil', 'core']))
mag1.addResult(mag1.Result.CoilInductance([excoil], 'amplPhase', 'history', [text]))
static1.addPhysic(mag1)

simulation.addAnalysis(static1)

