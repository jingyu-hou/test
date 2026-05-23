!     
!     WeICME (Wedge Integrated Computational Materials Engineering)
!                 - A 3-dimensional finite element program.
!     
!     Developed and maintained by Shenzhen Wedge Central 
!     South Research Institute co., Ltd., Shenzhen, China
!     
!     Copy Right 2019-2023.
!
      subroutine dynamic_viscosity (kgas,T,dvi)
!
      implicit none
!
      integer kgas
!
      real*8 T,dvi
      kgas=kgas
!
      dvi=0.00001711d0*dsqrt(T/273.15d0)*(1d0+113d0/273.15d0)
     &     /(1.d0+113.d0/T)
!
      return
!
      end
!
