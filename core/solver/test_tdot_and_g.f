!     
!     WeICME (Wedge Integrated Computational Materials Engineering)
!                 - A 3-dimensional finite element program.
!     
!     Developed and maintained by Shenzhen Wedge Central 
!     South Research Institute co., Ltd., Shenzhen, China
!     
!     Copy Right 2019-2023.
!  
      subroutine test_tdot_and_g(nk,vold,tempold,mi,coolingrate,gtemp,
     &  gradtempn,dtime,fl_ny,flcon,nflcon,ntmat_,ithermal)
!
!     determine cooling rate and  temperature gradient
!
        implicit none
!
        integer nk,mi(*),nflcon(*),ithermal(*),ntmat_,imat,i
!
        real*8 vold(0:mi(2),*),tempold(*),coolingrate(*),gtemp(*),
     &       gradtempn(3,*),dtime,fl_ny,flcon(0:1,ntmat_,*),
     &       temp,fl
!
      do i=1,nk
c        print *,i,coolingrate(i),gtemp(i)
!
        coolingrate(i)=(tempold(i)-vold(0,i))/dtime
!
        gtemp(i)=(gradtempn(1,i)**2.d0
     &           +gradtempn(2,i)**2.d0
     &           +gradtempn(3,i)**2.d0)**0.5d0
c        print *,i,temp,fl,fl_ny,coolingrate(i),gtemp(i)
      enddo
!
      return
      end
!