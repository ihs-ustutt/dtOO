import foamlib
from pyDtOO import *

import matplotlib.pyplot as plt
import numpy as np
import re
import sys

n_cores = sys.argv[1]
case_name = sys.argv[2]

_case = './'+case_name+'/'
#_case = './of_bladeAngle05_tolP001/'
_safe_case = re.sub(r'[\\/:\*\?"<>|]', '_', _case)

fc = foamlib.FoamCase( _case )
if True:
  fc.decompose_par_dict['method'] = 'metis'
  fc.decompose_par_dict['numberOfSubdomains'] = int(n_cores)
   
  fc.turbulence_properties["RAS"]["turbulence"] = False
  fc.fv_schemes['gradSchemes']['none'] = ('cellLimited', 'Gauss', 'linear', 0.33)
  fc.fv_schemes['gradSchemes']['grad(p)'] = ('cellLimited', 'Gauss', 'linear', 0.33)
  fc.fv_schemes['gradSchemes']['grad(U)'] = ('cellLimited', 'Gauss', 'linear', 0.33)
  fc.fv_schemes['divSchemes']['div(phi,U)'] = \
    ('Gauss', 'linearUpwind', 'cellLimited', 'Gauss', 'linear', 0.33)
  fc.fv_schemes["mixingInterface"] = {
        "U": "consistentAveraging",
        "p": "consistentAveraging",
        "k": "fluxAveraging",
        "epsilon": "fluxAveraging",
        "omega": "fluxAveraging"
      }
  fc.fv_solution["relaxationFactors"] = {
        "p": 0.2,
        "U": 0.6,
        "k": 0.6,
        "epsilon": 0.6,
        "omega": 0.6
      }
  fc.fv_solution["SIMPLE"] = {
        "nNonOrthogonalCorrectors": 2
      }
 
  fc.control_dict['DebugSwitches'] = {
          'mixingInterfacePolyPatch': 2
        }
  fc.control_dict['writeInterval'] = 100
  fc.control_dict['endTime'] = 1000
  fc.run()
  
  fc.control_dict['endTime'] = 2500
  fc.control_dict['writeInterval'] = 10
  #fc.control_dict['purgeWrite'] = 10
  fc.turbulence_properties["RAS"]["turbulence"] = True
  
  fc.run()
  
#fc.reconstruct_par()
fc.run(["reconstructPar", '-time', '500,2490:'])

_omega = np.abs(
  foamlib.FoamFile(fc.path/'constant/MRFProperties')['MRF_ru_mesh']['omega']
)
_rho = 997.
_g = 9.81
for var in ["U", "U:Transformed", "p",]:
  for patch in [
          "gv_mesh_inlet", "gv_mesh_outlet", 
          "ru_mesh_inlet", "ru_mesh_outlet", 
          "meshLayers_inlet", "meshLayers_outlet", 
          "dt_mesh_inlet", "dt_mesh_outlet"
        ]:
    fc.run(["patchToCsv", var, patch])

_time = fc[-1].name

#patchToCsv U ru_mesh_outlet
UTrans_gv_in = dtAverageValueField(dtCylField(_case+"gv_mesh_inlet_U:Transformed_"+_time+".csv").Read(), 10, 10, 10)
UTrans_gv_out = dtAverageValueField(dtCylField(_case+"gv_mesh_outlet_U:Transformed_"+_time+".csv").Read(), 10, 10, 10)
UTrans_ru_in = dtAverageValueField(dtCylField(_case+"ru_mesh_inlet_U:Transformed_"+_time+".csv").Read(), 10, 10, 10)
UTrans_ru_out = dtAverageValueField(dtCylField(_case+"ru_mesh_outlet_U:Transformed_"+_time+".csv").Read(), 10, 10, 10)
UTrans_ml_in = dtAverageValueField(dtCylField(_case+"meshLayers_inlet_U:Transformed_"+_time+".csv").Read(), 10, 10, 10)
UTrans_ml_out = dtAverageValueField(dtCylField(_case+"meshLayers_outlet_U:Transformed_"+_time+".csv").Read(), 10, 10, 10)
UTrans_dt_in = dtAverageValueField(dtCylField(_case+"dt_mesh_inlet_U:Transformed_"+_time+".csv").Read(), 10, 10, 10)

#UTrans_dt_out = dtValueField(dtField(_case+"dt_mesh_outlet_U:Transformed_"+_time+".csv").Read())

fig, axes = plt.subplots(3, 1, figsize=(6.7, 8))

ax = axes[0]
ax.plot(UTrans_ru_in.CoordThree()[:,2], UTrans_ru_in.ValueAvAThree()[:,0], 'ks', label = r'RU Inlet $U_r$')
#ax.plot(U_ru_in.Coord()[:,2], U_ru_in.Value()[:,0], 'k.')
ax.plot(UTrans_gv_out.CoordThree()[:,2], UTrans_gv_out.ValueAvAThree()[:,0], 'k-', label = r'GV Outlet $U_r$')
#ax.plot(U_gv_out.Coord()[:,2], U_gv_out.Value()[:,0], 'kx')
ax.plot(UTrans_ru_in.CoordThree()[:,2], UTrans_ru_in.ValueAvAThree()[:,1], 'rs', label = r'RU Inlet $U_φ$')
ax.plot(UTrans_gv_out.CoordThree()[:,2], UTrans_gv_out.ValueAvAThree()[:,1], 'r-', label = r'GV Outlet $U_φ$')
# Labels and title
ax.set_xlabel('z position')
ax.set_ylabel('Velocity Transformed')
ax.set_title("Runner (RU) Inlet and Guide Vane (GV) Outlet")

# Legend
ax.legend(loc='center left', bbox_to_anchor=(1, 0.5))
ax.grid(True)
#plt.savefig("fig_ru_in.png")
#plt.clf()

ax = axes[1]
ax.plot(UTrans_ru_out.CoordOne()[:,0], UTrans_ru_out.ValueAvAOne()[:,2], 'ks', label = r'RU Outlet $U_z$')
ax.plot(UTrans_ml_in.CoordOne()[:,0], UTrans_ml_in.ValueAvAOne()[:,2], 'k-', label = r'ML Inlet $U_z$')
ax.plot(UTrans_ru_out.CoordOne()[:,0], UTrans_ru_out.ValueAvAOne()[:,1], 'rs', label = r'RU Outlet $U_φ$')
ax.plot(UTrans_ml_in.CoordOne()[:,0], UTrans_ml_in.ValueAvAOne()[:,1], 'r-', label = r'ML Inlet $U_φ$')
#ax.savefig("fig_ru_out.png")
# Labels and title
ax.set_xlabel('x position')
ax.set_ylabel('Velocity Transformed')
ax.set_title("Runner (RU) Outlet and Mesh Layer (ML) Inlet")
ax.legend(loc='center left', bbox_to_anchor=(1, 0.5))
ax.grid(True)

ax = axes[2]
ax.plot(UTrans_ml_out.CoordOne()[:,0], UTrans_ml_out.ValueAvAOne()[:,2], 'ks', label = r'ML Outlet $U_z$')
ax.plot(UTrans_dt_in.CoordOne()[:,0], UTrans_dt_in.ValueAvAOne()[:,2], 'k-', label = r'DT Inlet $U_z$')
ax.plot(UTrans_ml_out.CoordOne()[:,0], UTrans_ml_out.ValueAvAOne()[:,1], 'rs', label = r'ML Outlet $U_φ$')
ax.plot(UTrans_dt_in.CoordOne()[:,0], UTrans_dt_in.ValueAvAOne()[:,1], 'r-', label = r'DT Inlet $U_φ$')
#ax.savefig("fig_ru_out.png")
# Labels and title
ax.set_xlabel('x position')
ax.set_ylabel('Velocity Transformed')
ax.set_title("Mesh Layer (ML) Outlet and Draft Tube (DT) Inlet")
ax.legend(loc='center left', bbox_to_anchor=(1, 0.5))
ax.grid(True)

#ax = axes[3]
#ax.plot(UTrans_dt_out.CoordOne()[:,2], UTrans_dt_out.ValueAvAOne()[:,0], 'ks', label = r'DT Outlet $U_z$')
##ax.savefig("fig_ru_out.png")
## Labels and title
#ax.set_xlabel('x position')
#ax.set_ylabel('Velocity Transformed')
#ax.set_title("Draft Tube (DT) Outlet")
#ax.legend(loc='center left', bbox_to_anchor=(1, 0.5))
#ax.grid(True)


pT_gv_in = dtScalarDeveloping( dtDeveloping(_case+"postProcessing/PT_gv_mesh_inlet").Read(pattern={"*.dat": ":,1:"}) )
pT_gv_out = dtScalarDeveloping( dtDeveloping(_case+"postProcessing/PT_gv_mesh_outlet").Read(pattern={"*.dat": ":,1:"}) )
pT_ru_in = dtScalarDeveloping( dtDeveloping(_case+"postProcessing/PT_ru_mesh_inlet").Read(pattern={"*.dat": ":,1:"}) )
pT_ru_out = dtScalarDeveloping( dtDeveloping(_case+"postProcessing/PT_ru_mesh_outlet").Read(pattern={"*.dat": ":,1:"}) )
pT_ml_in = dtScalarDeveloping( dtDeveloping(_case+"postProcessing/PT_meshLayers_inlet").Read(pattern={"*.dat": ":,1:"}) )
pT_ml_out = dtScalarDeveloping( dtDeveloping(_case+"postProcessing/PT_meshLayers_outlet").Read(pattern={"*.dat": ":,1:"}) )
pT_dt_in = dtScalarDeveloping( dtDeveloping(_case+"postProcessing/PT_dt_mesh_inlet").Read(pattern={"*.dat": ":,1:"}) )
pT_dt_out = dtScalarDeveloping( dtDeveloping(_case+"postProcessing/PT_dt_mesh_outlet").Read(pattern={"*.dat": ":,1:"}) )
dh_gv0 = (pT_gv_out.MeanLast(10) - pT_gv_in.MeanLast(10))/_g
dh_ru0 = (pT_ru_out.MeanLast(10) - pT_ru_in.MeanLast(10))/_g
dh_ml0 = (pT_ml_out.MeanLast(10) - pT_ml_in.MeanLast(10))/_g
dh_dt0 = (pT_dt_out.MeanLast(10) - pT_dt_in.MeanLast(10))/_g
dh = dh_gv0 + dh_ru0 + dh_ml0 + dh_dt0
F = dtForceDeveloping( dtDeveloping(_case+"postProcessing/F_ru_mesh_blade").Read(pattern={'force.dat' : ':,4:10', 'moment.dat' : ':,4:10', '*.*' : ''}) )

print("pT_ru_in = ", pT_ru_in.MeanLast(1))
print("pT_ru_out = ", pT_ru_out.MeanLast(1))
pt_globIn = pT_gv_in.MeanLast(10)
pt_globOut = pT_ml_out.MeanLast(10)
F_z = F.MomentMeanLast(10)[2]
print("p_gv_in = %f" % pT_gv_in.MeanLast(10))
print("p_ml_out = %f" % pT_ml_out.MeanLast(10))
print("dh_gv = %f" % dh_gv0)
print("dh_ru = %f" % dh_ru0)
print("dh_ml = %f" % dh_ml0)
print("dh_dt = %f" % dh_dt0)
print("Resulting height:")
print("dh = %f" % dh)
print("F_z = %f" % F.MomentMeanLast(10)[2])


U_gv_in = dtValueField(dtField(_case+"gv_mesh_inlet_U_"+_time+".csv").Read())
U_gv_out = dtValueField(dtField(_case+"gv_mesh_outlet_U_"+_time+".csv").Read())
p_gv_in = dtValueField(dtField(_case+"gv_mesh_inlet_p_"+_time+".csv").Read())
p_gv_out = dtValueField(dtField(_case+"gv_mesh_outlet_p_"+_time+".csv").Read())
Q_gv = np.abs(p_gv_in.IntQ())

h_tot_gv_in = (p_gv_in.IntValueQ() / _g + U_gv_in.IntMagSquareQ() / (2.*_g))/Q_gv
print("h_tot_gv_in = %f" % h_tot_gv_in)
#h_tot_ru_out = (p_ru_out.IntValueQ() / _g + U_ru_out.IntMagSquareQ() / (2.*_g))/Q

U_ru_in = dtValueField(dtField(_case+"ru_mesh_inlet_U_"+_time+".csv").Read())
U_ru_out = dtValueField(dtField(_case+"ru_mesh_outlet_U_"+_time+".csv").Read())
p_ru_in = dtValueField(dtField(_case+"ru_mesh_inlet_p_"+_time+".csv").Read())
p_ru_out = dtValueField(dtField(_case+"ru_mesh_outlet_p_"+_time+".csv").Read())

print("p_ru_in = ", p_ru_in.IntValueQ())
print("p_ru_out = ", p_ru_out.IntValueQ())

Q_ru = np.abs(p_ru_in.IntQ())
h_tot_ru_in = -(p_ru_in.IntValueQ() / _g + U_ru_in.IntMagSquareQ() / (2.*_g))/Q_ru
h_tot_ru_out = (p_ru_out.IntValueQ() / _g + U_ru_out.IntMagSquareQ() / (2.*_g))/Q_ru
print("")
print("p_ru_in.IntQ() = ", p_ru_in.IntQ())
print("p_ru_out.IntQ() = ", p_ru_out.IntQ())
print("p_ru_in.IntValueQ() = ", p_ru_in.IntValueQ())
print("U_ru_in.IntMagSquareQ() = ", U_ru_in.IntMagSquareQ())
print("p_ru_out.IntValueQ() = ", p_ru_out.IntValueQ())
print("U_ru_out.IntMagSquareQ() = ", U_ru_out.IntMagSquareQ())
print("h_tot_ru_in = %f" % h_tot_ru_in)
print("h_tot_ru_out = %f" % h_tot_ru_out)

dh_ru = (h_tot_ru_out - h_tot_ru_in)
M_z = F.MomentMeanLast(100)[2]

print("M_z = %f" % M_z)
print("_omega = %f" % _omega)
print("dh_ru = %f" % dh_ru)
print("Q_ru = %f" % Q_ru)

eta = M_z * _omega / (_rho*_g*dh_ru*Q_ru)
print("eta = %f" % eta)

fig.text(0.5, 0.02, 
         f"""Pressure: $p_{{T,GV,in}}$ = {pt_globIn:.2f}, $p_{{T,DT,out}}$ = {pt_globOut:.2f}, $dh$ = {dh:.2f} m
RU Blade Forces: $F_z$ = {F_z:.2f} N, $M_z$ = {M_z:.2f} N*m
RU Values: $\\omega$ = {_omega:.2f} 1/s, $dh_{{ru}}$ = {dh_ru[0]:.2f} m, $Q_{{ru}}$ = {Q_ru:.2f} m^3/s
Efficiency: $\\eta$ = {eta[0]:.4f}""",
         ha='center', fontsize=12, bbox=dict(facecolor='lightgray', alpha=0.5))


# ---- Final layout ----
fig.suptitle("Case: "+_case)
plt.tight_layout(rect=[0, 0.15, 1, 1])
plt.savefig(f"./ofCase_eval/eval_{_safe_case}.png")
plt.close()


#try:
#  #
#  # Read data from ``postProcessing`` folder
#  #
#  Q_ru_dev = dtScalarDeveloping(
#    dtDeveloping(str(fc.path/'postProcessing/Q_ru_in/100')).Read()
#  )
#  pIn_ru_dev = dtScalarDeveloping(
#    dtDeveloping(str(fc.path/'postProcessing/ptot_ru_in/100')).Read()
#  )
#  pOut_ru_dev = dtScalarDeveloping(
#    dtDeveloping(str(fc.path/'postProcessing/ptot_ru_out/100')).Read()
#  )
#  Vcav_dev = dtScalarDeveloping(
#    dtDeveloping(str(fc.path/'postProcessing/V_CAV/100')).Read()
#  )
#  F_dev = dtForceDeveloping(
#    dtDeveloping(str(fc.path/'postProcessing/forces')).Read(
#      {'force.dat' : ':,4:10', 'moment.dat' : ':,4:10', '*.*' : ''}
#    )
#  )
#  #
#  # Calculate power, head, efficiency, and cavitation volume; average
#  # simulation results over 100 iterations
#  #
#  P[i] = F_dev.MomentMeanLast(100)[2] * omega
#  dH[i] = (pOut_ru_dev.MeanLast(100) - pIn_ru_dev.MeanLast(100)) / 9.81
#  eta[i] = P[i] / (1000. * 9.81 * dH[i] * Q_ru_dev.MeanLast(100) )
#  Vcav[i] = Vcav_dev.MeanLast(100)
#  #
#  # Check if number of iterations reached and if it is a turbine not a
#  # pump
#  #
#  if fc[-1].name != '1000':
#    raise ValueError('Max number of iterations not reached.')
#  if np.abs(eta[i]) > 1:
#    raise ValueError('Pump detected.')
##
## Handle exceptions
##
#except Exception as e:
#  logging.error('Catch exception : %s', e)
