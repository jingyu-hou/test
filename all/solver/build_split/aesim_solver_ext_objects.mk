# aesim_solver_ext_objects.mk
# Generated: 2026-06-05 (revised 2026-06-05 after WeICME_MT.a analysis)
# Part of: CalculiX open-source boundary split - Step 4
#
# ARCHITECTURE:
# - WeICME_MT.a (1117 objects) = TRUE CalculiX core library
# - Files compiled here = OVERRIDE layer (all B-class: CalculiX-derived,
#   modified by AESim_FM). These files replace same-named objects in
#   WeICME_MT.a at link time.
# - A few net-new C-class files (not in WeICME_MT.a) are also added.
#
# This group contains ALL compiled objects from solver/*.f and solver/*.c
# (except WeICME.o, the main entry). They form the AESim_FM solver
# extension/override layer.
#
# Link order: WeICME.o + libaesim_solver_ext.a + WeICME_MT.a + numerical libs

# All Fortran objects from SCCXF (overrides WeICME_MT.a)
AESIM_SOLVER_EXT_F_OBJS = \
addimd.o \
bmatrix.o \
bsort.o \
calccvel.o \
calccvfa.o \
calcgradtemp_cast.o \
calchcel.o \
calchcfa.o \
calcinitialflux.o \
calcmechstrain.o \
calcpel.o \
calcrespfluid.o \
calcrestfluid.o \
calcresvfluid1.o \
calcresvfluid2.o \
calcrhoel.o \
calcrhoelcomp.o \
calcrhofa.o \
calcumel.o \
calcumfa.o \
calcvel.o \
calcvol.o \
calcvol2.o \
cd_Mcgreehan_Schotsch.o \
cd_lichtarowicz.o \
cd_own_albers.o \
cd_pk_albers.o \
cident.o \
cident20.o \
cident80.o \
closefile.o \
closefilefluid.o \
convert2slapcol.o \
correctrhoelcomp.o \
correctvel.o \
createialnk.o \
dKdX.o \
damage.o \
damagecal.o \
damages.o \
dattime.o \
drx.o \
drx_hip_weicme.o \
drx_weicme.o \
dynamicrecrystallizations.o \
edg.o \
elemperorien.o \
eplane.o \
fcrit.o \
flowstress.o \
fsub.o \
fsuper.o \
gasmechbc.o \
getversion.o \
htchart.o \
init.o \
initialh.o \
inputerror.o \
inputinfo.o \
inputwarning.o \
insertsortd.o \
insertsorti.o \
integratelinesproduct.o \
integraterhocp.o \
intersectionpoint.o \
invariant.o \
lab_straight_ppkrit.o \
lump.o \
materialdata_dtdh.o \
materialdata_h2temp.o \
materialdata_temp2h.o \
materialdata_th_h.o \
materialdata_th_h2.o \
matvec.o \
meannode.o \
metal_powder.o \
metalpowders.o \
modf.o \
msolve.o \
msolve_struct.o \
multvec.o \
negativepressure.o \
nident.o \
nident2.o \
nidentk.o \
nidentll.o \
norm.o \
nystar2flcr.o \
nystarflcrchart.o \
op.o \
op_corio.o \
opas.o \
openfilefluid.o \
phaseother.o \
phasetransition.o \
phasetransition1.o \
phys2con_cast.o \
pk_cdc_cl3.o \
pk_cdi_r.o \
pk_cdi_rl.o \
pk_cdi_se.o \
pop.o \
postprocess_cast.o \
posttransition.o \
preconvert2slapcol.o \
predir.o \
preparenycalc.o \
push.o \
randomval.o \
ratedependentplastics.o \
rdplas.o \
readphaseinf.o \
reorderampl.o \
reorderlhs.o \
reorderrhs.o \
sensitivity_glob.o \
smoothshock.o \
stop.o \
templiquidussolidus.o \
thermmodel.o \
time2solidus.o \
transformation.o \
triloc.o \
uamplitude.o \
uexpan.o \
uhard.o \
uhardening.o \
velsolve.o \
writeboun.o \
writebv.o \
writedesi.o \
writeevcomplex.o \
writehe.o \
writeim.o \
writemac.o \
writemaccs.o \
writempc.o \
writerandomfield.o \
writere.o \
writesta.o \
writestadiv.o \
writevfa.o

# All C objects from SCCXC (overrides WeICME_MT.a)
AESIM_SOLVER_EXT_C_OBJS = \
drxc.o \
getSystemCPUs.o \
itinerate_print.o \
output_mtx.o \
plasticwork.o \
random_print.o \
readxstateini.o \
stof.o \
stoi.o \
strcmp2.o \
strcpy1.o \
v_betrag.o \
v_prod.o \
v_result.o

AESIM_SOLVER_EXT_OBJS = $(AESIM_SOLVER_EXT_F_OBJS) $(AESIM_SOLVER_EXT_C_OBJS)

# Main entry object (kept separate from the library)
# SOLVER_MAIN_OBJ = WeICME.o
