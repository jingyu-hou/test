COMP_DIR = ../components/open_source
CALCULIX_DIR = $(COMP_DIR)/calculix

SPOOLES_DIR = $(COMP_DIR)/spooles
ARPACK_DIR = $(COMP_DIR)/arpack
LAPACK_DIR = $(COMP_DIR)/lapack_blas

EXTERNAL_HEADERS = ../../../../externalized_components
CFLAGS = -Wall -O3 -fopenmp -DARCH="Linux" -DSPOOLES -DARPACK -DMATRIXSTORAGE -DUSE_MT=1 -I$(EXTERNAL_HEADERS)/calculix_headers
FFLAGS = -Wall -O3 -fopenmp

CC=cc
FC=gfortran

%.o: %.c
	$(CC) $(CFLAGS) -c $<
%.o: %.f
	$(FC) $(FFLAGS) -c $<

include Makefile.inc
include build_split/calculix_base_objects.mk
include build_split/aesim_solver_ext_objects.mk

SCCXMAIN = WeICME.c

OCCXF = $(SCCXF:.f=.o)
OCCXC = $(SCCXC:.c=.o)
OCCXMAIN = $(SCCXMAIN:.c=.o)

SO_LIBS = \
       -L$(SPOOLES_DIR) -lspoolesMT \
       -L$(SPOOLES_DIR) -lspooles \
       -L$(ARPACK_DIR) -larpack \
       -L$(LAPACK_DIR) -l:liblapack.so \
       -L$(LAPACK_DIR) -l:libblas.so

RPATH = -Wl,-rpath,\$$ORIGIN/../components/open_source/spooles \
        -Wl,-rpath,\$$ORIGIN/../components/open_source/arpack \
        -Wl,-rpath,\$$ORIGIN/../components/open_source/lapack_blas \
        -Wl,-rpath,\$$ORIGIN/../../components/open_source/spooles \
        -Wl,-rpath,\$$ORIGIN/../../components/open_source/arpack \
        -Wl,-rpath,\$$ORIGIN/../../components/open_source/lapack_blas

# CalculiX core library (built from D:\ccx\src, CalculiX 2.15 baseline)
# Previously referenced as WeICME_MT.a
LIBS = $(CALCULIX_DIR)/libcalculix_base.a $(SO_LIBS) -lpthread -lm -lc

SOLVER_DEPS = \
       $(SPOOLES_DIR)/libspooles.so \
       $(SPOOLES_DIR)/libspoolesMT.so \
       $(ARPACK_DIR)/libarpack.so \
       $(LAPACK_DIR)/liblapack.so \
       $(LAPACK_DIR)/libblas.so

SOLVER_NAME ?= AESim-FM

$(SOLVER_NAME): $(OCCXMAIN) $(OCCXF) $(OCCXC) $(SOLVER_DEPS)
	./date.pl; $(CC) $(CFLAGS) -c WeICME.c; $(FC) -fopenmp -Wall -O3 -no-pie -o $@ $(OCCXMAIN) $(OCCXF) $(OCCXC) $(RPATH) $(LIBS)

# --- Open-source boundary split: static library targets ---
# Architecture:
#   libcalculix_base.a (1117 objects) = CalculiX 2.15 core (built from D:\ccx\src)
#   solver/*.f + solver/*.c          = OVERRIDE layer (all B-class: CalculiX-derived,
#                                      modified by AESim_FM; replace same-named
#                                      objects in libcalculix_base.a at link time)
#   WeICME.o                         = Main entry (kept separate)
#
# Split link order: WeICME.o + libaesim_solver_ext.a + libcalculix_base.a + numerical libs

LIB_AESIM_SOLVER_EXT = libaesim_solver_ext.a
SOLVER_MAIN_OBJ = WeICME.o

# Build AESim_FM solver extension library (override layer: all compiled solver/ objects)
$(LIB_AESIM_SOLVER_EXT): $(AESIM_SOLVER_EXT_OBJS)
	ar rcs $@ $(AESIM_SOLVER_EXT_OBJS)

# Split-build solver: links override library + CalculiX core library
$(SOLVER_NAME)_split: $(SOLVER_MAIN_OBJ) $(LIB_AESIM_SOLVER_EXT) $(SOLVER_DEPS)
	$(FC) -fopenmp -Wall -O3 -no-pie -o $@ $(SOLVER_MAIN_OBJ) $(LIB_AESIM_SOLVER_EXT) $(CALCULIX_DIR)/libcalculix_base.a $(RPATH) $(SO_LIBS) -lpthread -lm -lc

# Convenience targets
libs: $(LIB_AESIM_SOLVER_EXT)
all_split: $(SOLVER_NAME)_split

clean:
	rm -f *.o *.a $(SOLVER_NAME) $(SOLVER_NAME)_split
