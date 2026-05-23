COMP_DIR = ../components/open_source

SPOOLES_DIR = $(COMP_DIR)/spooles
ARPACK_DIR = $(COMP_DIR)/arpack
LAPACK_DIR = $(COMP_DIR)/lapack_blas

CFLAGS = -Wall -O3 -fopenmp -DARCH="Linux" -DSPOOLES -DARPACK -DMATRIXSTORAGE -DUSE_MT=1
FFLAGS = -Wall -O3 -fopenmp

CC=cc
FC=gfortran

%.o: %.c
	$(CC) $(CFLAGS) -c $<
%.o: %.f
	$(FC) $(FFLAGS) -c $<

include Makefile.inc

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

LIBS = $(COMP_DIR)/WeICME_MT.a $(SO_LIBS) -lpthread -lm -lc

SOLVER_DEPS = \
       $(SPOOLES_DIR)/libspooles.so \
       $(SPOOLES_DIR)/libspoolesMT.so \
       $(ARPACK_DIR)/libarpack.so \
       $(LAPACK_DIR)/liblapack.so \
       $(LAPACK_DIR)/libblas.so

SOLVER_NAME ?= AESim-FM

$(SOLVER_NAME): $(OCCXMAIN) $(OCCXF) $(OCCXC) $(SOLVER_DEPS)
	./date.pl; $(CC) $(CFLAGS) -c WeICME.c; $(FC) -fopenmp -Wall -O3 -no-pie -o $@ $(OCCXMAIN) $(OCCXF) $(OCCXC) $(RPATH) $(LIBS)

clean:
	rm -f *.o *.a $(SOLVER_NAME)
