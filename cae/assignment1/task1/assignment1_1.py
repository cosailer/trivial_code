# -*- coding: utf-8 -*-
"""
-------------------------------------
 N A C S   P Y T H O N   S C R I P T 
-------------------------------------

NACS version: 2.0.2570 - pre2
NACS architecture: CENTOS 5.10 (X86_64)
File generated at Fri Nov  7 17:16:50 2014
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
simulation.setGrid('assignment1.nmf', 'axi')

simulation.addOutput(Output.Nacs())
text = Output.Text()
simulation.addOutput(text)
simulation.addOutput(Output.GiD())


# =====================
#  MATERIAL DEFINITION
# =====================
air = Material('air')
air.density(1.205)
air.permittivity.isotropic(8.85e-12)
air.permeability.isotropic(1.25664e-06)
air.conductivity.isotropic(0.0)
air.compressionModulus(141650)

srtio3 = Material('SrTiO3')
srtio3.permittivity.isotropic(2.655e-09)

aluminum = Material('Aluminum')
aluminum.density(2700.0)
aluminum.lossTangensDelta([1000],[0.0001])
aluminum.permittivity.isotropic(0.0885)
aluminum.stiffness.isotropic.byENu(70000000000.0, 0.34)


simulation.setMat('airRegion', air)
simulation.setMat('dielectricum_region', srtio3)
simulation.setMat('gndElectrode_region', aluminum)
simulation.setMat('midElectrode_region', aluminum)
simulation.setMat('topElectrode_region', aluminum)

# ===============
#  ANALYSIS STEP 
# ===============
static1 = Analysis.Static()

elec1 = Physic.Electrostatic()
elec1.addRegions(['airRegion', 'gndElectrode_region', 'dielectricum_region', 'topElectrode_region', 'midElectrode_region'])
elec1.addBc(elec1.BC.Potential.expr('topElectrode_region', "50"))
elec1.addBc(elec1.BC.Ground('gndElectrode_region'))
elec1.addBc(elec1.BC.EquiPotential('midElectrode_region'))
elec1.addResult(elec1.Result.FieldIntensity(['airRegion', 'gndElectrode_region', 'dielectricum_region', 'topElectrode_region', 'midElectrode_region'], 'amplPhase', 'mesh', [text]))
elec1.addResult(elec1.Result.Potential(['airRegion', 'gndElectrode_region', 'dielectricum_region', 'topElectrode_region', 'midElectrode_region'], 'amplPhase', 'mesh', [text]))
elec1.addResult(elec1.Result.Energy(['airRegion', 'gndElectrode_region', 'dielectricum_region', 'topElectrode_region', 'midElectrode_region'], 'amplPhase', 'history', [text]))
static1.addPhysic(elec1)

simulation.addAnalysis(static1)

