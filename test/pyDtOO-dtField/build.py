#------------------------------------------------------------------------------
#  dtOO < design tool Object-Oriented >
#    
#    Copyright (C) 2024 A. Tismer.
#------------------------------------------------------------------------------
#License
#    This file is part of dtOO.
#
#    dtOO is distributed in the hope that it will be useful, but WITHOUT
#    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
#    FITNESS FOR A PARTICULAR PURPOSE.  See the LICENSE.txt file in the
#    dtOO root directory for more details.
#
#    You should have received a copy of the License along with dtOO.
#
#------------------------------------------------------------------------------

import sys
from pyDtOO import *
import numpy as np

tolDD = +1.0E-06

# volume flow
u_inlet = dtValueField( dtField("mesh_inlet_U_50.csv").Read() )
Q_inlet = dtScalarDeveloping( 
  dtDeveloping("postProcessing/swakExpression_Q_mesh_inlet").Read()
)
Q_inlet_csv = u_inlet.IntQ()

DD = (
  Q_inlet_csv - Q_inlet.scalar_[-1]
) / np.abs(Q_inlet_csv)
if  DD < tolDD:
  print("OK - VolumeFlow - %e" % DD)
else:
  print("FAIL")
  sys.exit(-1)

# total pressure
p_inlet = dtValueField( dtField("mesh_inlet_p_50.csv").Read() )
ptot_inlet_csv = (
  p_inlet.IntValueQ() + 0.5 * u_inlet.IntMagSquareQ()
) / p_inlet.IntQ()
#print( ptot_inlet_csv[0] )
#array([-9.67341397])
ptot_inlet_swak = dtScalarDeveloping( 
  dtDeveloping("postProcessing/swakExpression_PT_mesh_inlet").Read() 
)
#print( ptot_inlet_swak.scalar_[-1] )
#-9.67341

DD = (
  ptot_inlet_csv[0] - ptot_inlet_swak.scalar_[-1]
) / np.abs(ptot_inlet_csv[0])
if  DD < tolDD:
  print("OK - TotalPressure - %e" % DD)
else:
  print("FAIL")
  sys.exit(-1)

# force
F = dtForceDeveloping( 
  dtDeveloping("postProcessing/forces").Read(
    {'force.dat' : ':,4:10', 'moment.dat' : ':,4:10', '*.*' : ''}
  ) 
)

# force pressure
p_blade = dtValueField( dtField("mesh_blade_p_50.csv").Read() )
p_blade.SIntValueA()
#array([[  2.96050063,  22.17072824, -17.42009827]])
F.forcePressure_[-1,:]
#array([  2951.62,  22104.21, -17367.84])

DD = ( 
  (997.*p_blade.SIntValueA()[0,0] - F.forcePressure_[-1,0])**2.0
  +
  (997.*p_blade.SIntValueA()[0,1] - F.forcePressure_[-1,1])**2.0
  +
  (997.*p_blade.SIntValueA()[0,2] - F.forcePressure_[-1,2])**2.0
)**0.5 / np.sqrt( np.sum( F.forcePressure_[-1,:]**2.0 ) )
if  DD < tolDD:
  print("OK - ForcePressure - %e" % DD)
else:
  print("FAIL -> %e" % DD)
  sys.exit(-1)

# force viscous
tau_blade = dtValueField( dtField("mesh_blade_wallShearStress_50.csv").Read() )
tau_blade.IntValueA()
#array([0.00187665, 0.01433018, 0.04733842])
F.forceViscous_[-1,:]
#array([ -1.871019, -14.28719 , -47.1964  ])

DD = ( 
  (997.*tau_blade.IntValueA()[0] + F.forceViscous_[-1,0])**2.0
  +
  (997.*tau_blade.IntValueA()[1] + F.forceViscous_[-1,1])**2.0
  +
  (997.*tau_blade.IntValueA()[2] + F.forceViscous_[-1,2])**2.0
)**0.5 / np.sqrt( np.sum( F.forceViscous_[-1,:]**2.0 ) )
if  DD < tolDD:
  print("OK - ForceViscous - %e" % DD)
else:
  print("FAIL -> %e" % DD)
  sys.exit(-1)

# moment viscous
rXtau_blade = dtValueField( 
  dtCoordCrossField("mesh_blade_wallShearStress_50.csv").Read() 
)
rXtau_blade.IntValueA()
#array([-0.01264001, -0.06113892,  0.02134516])
F.momentViscous_[-1,:]
#array([ 12.60209,  60.9555 , -21.28113])

DD = ( 
  (997.*rXtau_blade.IntValueA()[0] + F.momentViscous_[-1,0])**2.0
  +
  (997.*rXtau_blade.IntValueA()[1] + F.momentViscous_[-1,1])**2.0
  +
  (997.*rXtau_blade.IntValueA()[2] + F.momentViscous_[-1,2])**2.0
)**0.5 / np.sqrt( np.sum( F.momentViscous_[-1,:]**2.0 ) )
if  DD < tolDD:
  print("OK - MomentViscous - %e" % DD)
else:
  print("FAIL -> %e" % DD)
  sys.exit(-1)

# moment pressure
rXp_blade = dtValueField( dtCoordCrossField("mesh_blade_p_50.csv").Read() )
rXp_blade.SIntValueA()
#array([[  4.09745839,  31.67941721, -24.55527675]])
F.momentPressure_[-1,:]
#array([-17402.56,  26446.19,  32451.49])

DD = ( 
  (997.*rXp_blade.SIntValueA()[0,0] + F.momentPressure_[-1,0])**2.0
  +
  (997.*rXp_blade.SIntValueA()[0,1] + F.momentPressure_[-1,1])**2.0
  +
  (997.*rXp_blade.SIntValueA()[0,2] + F.momentPressure_[-1,2])**2.0
)**0.5 / np.sqrt( np.sum( F.momentPressure_[-1,:]**2.0 ) )
if  DD < tolDD:
  print("OK - MomentPressure - %e" % DD)
else:
  print("FAIL -> %e" % DD)
  sys.exit(-1)
