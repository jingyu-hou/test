# CalculiX Component Info
# Generated: 2026-06-05

## Version Baseline
- **Project**: CalculiX / CrunchiX (CCX)
- **Baseline Version**: **2.15** (confirmed by copyright 1998-2018 in WeICME.h)
- **Author**: Guido Dhondt (guido.dhondt@me.com)
- **License**: GPL v2 or later
- **Official Site**: http://www.calculix.de

## Source Location
```
D:\ccx\src\
  960  .f  files (Fortran source)
  165  .c  files (C source)
  1    WeICME.h (main header)
  =1127 total source files
```
This is a complete CalculiX 2.15 source tree with the original
Guido Dhondt copyright headers preserved. It forms the upstream
open-source baseline for AESim_FM.

## Build Artifact
```
components/open_source/calculix/libcalculix_base.a
  - 1117 objects (some conditionally compiled)
  - Built from: D:\ccx\src via "make WeICME_MT.a"
  - Compiler: gfortran 11.x + cc, -Wall -O3 -fopenmp
  - Size: ~11 MB
```

## Header File
```
WeICME.h  —  CalculiX main header, Copyright (C) 1998-2018 Guido Dhondt
```
Contains all public CalculiX function declarations, Fortran name
mangling macros (FORTRAN/CEE), memory management macros (NNEW/RENEW/SFREE),
and the ITG integer type definition.

## AESim_FM Override Layer
The solver/ directory contains 157 files (144 .f + 14 .c + WeICME.c)
that override same-named objects in libcalculix_base.a at link time.

Files ONLY in solver (net new to AESim_FM, 13 .f):
- 7 business model files (C): plastic_disloKM, plastic_isotropic,
  plastic_kmjmak, plastic_phenopowerlaw_classic, constitutive, crystallite
  (not yet compiled into the solver)
- 3 Abaqus legacy files (D): CCT, TTT, K90DRX
- 3 independent programs (E): DRX_meso, GGCA2D, GGPF
- 2 CalculiX utility files (A): formatfile, gauss
- 1 independent program (E): SRX_3D

## License Compliance
For all/core delivery:
- libcalculix_base.a → GPL v2+, distributed as binary with source
  reference to D:\ccx (or packaged CalculiX 2.15 source tarball)
- libaesim_solver_ext.a → Proprietary license (AESim_FM override layer)
- WeICME.o → Proprietary license (custom main entry)
- WeICME.h → GPL v2+ (CalculiX API header, minimal interface)
