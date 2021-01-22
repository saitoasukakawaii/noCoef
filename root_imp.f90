!  MODULE ROOT_IMP

!***************************************************************************
!*                                                                         *
!* Module: root_imp.f90                                                    *
!* Version: 1.0                                                            *
!* By: Mette Olufsen, Math-Tech                                            *
!* Last modified: 14. Jan. 1997                                            * 
!*                                                                         *
!* Contains subroutines and functions needed to compute the impedance at   *
!* the root of a structured tree (with constant asymmetry-ratio and a      *
!* geometry where the size of all branches depends of the radius of the    *
!* branch at the root of the tree.                                         *
!*                                                                         *
!***************************************************************************
module root_imp
use f90_tools     ! Contains several tools used for when computing FFT.
implicit none

private
public impedance  ! The subroutine called from the c-program arteries.cxx
                  ! (the driver routine)
public impedance_init, impedance_close


! The number of generations in the structured tree
integer, parameter   :: Maxgen = 80

! The asymmetry ratio of the structured tree
real(lng), parameter :: asym   = 0.4048

! Exponent in radius relation.
real(lng), parameter :: expo   = 2.7_lng

! A temporary matrix for storing root impedances in parts of the
! structured tree, those which are repeated because of the constant
! asymmetry ratio.
complex(lng)         :: Computed(0:Maxgen,0:Maxgen)

complex(lng), allocatable   :: Z_om(:)

integer localmax


contains

!***************************************************************************
!*                                                                         *
!* Function: Z0func                                                        *
!* Version: 1.0                                                            *
!* By: Mette Olufsen, Math-Tech                                            *
!* Last modified: 14. Jan. 1997                                             * 
!*                                                                         *
!* Computes the impedance as a function of Omega using a recursive         *
!* formula for a asymmetric tree with constant asymmetry-ratio.            *
!*                                                                         *
!* This subroutine is called from function comp_imp by:                    *
!*                                                                         *
!* Z0func (omega_k,alpha_pow,beta_pow,ff*,rho,mu,r_root,r_min,Lr,Fr2,q,g)  *
!*                                                                         *
!* where:                                                                  *
!*                                                                         *
!* Z0func    Returns the frequency dep. impedance (Z0) from an asym. tree. *
!* omega_k   Frequency for which to compute Z0.                            *
!* alpha_pow The power of alpha in the structured tree.                    *
!* beta_pow  The power of beta in the structured tree.                     *
!* ff*       Constants to model Eh/r = ff1*exp(ff2*r)+ff3, g/cm/s^2.       *
!* ff*       Constants to model Eh/r=-ff1*tanh(ff2*(r-ff3))+ff4, g/cm/s^2. *
!* rho       The density of the blood, g/cm^3.                             *
!* mu        Bloodplasma viscosity, g/cm/s.                                *
!* r_root    The input radius at the root of the structured tree, cm.      *
!* r_min     The minimum bottom radius, gives the order of the tree, cm.   *
!* Lr        Characteristic length scale (radius), cm.                     *
!* Fr2       The squared Froude number.                                    *
!* q         Characteristic flow-rate, cm^3/s.                              * 
!* g         The gravitational force, cm/s^2.                              *
!*                                                                         *
!***************************************************************************
recursive function Z0func (omega_k, alpha_pow,beta_pow,ff1,ff2,ff3,rho,mu,r_root,r_min,trm_rst,Lr,Fr2,q,g) result (Z0)
implicit none

  integer, intent(in)   :: alpha_pow, beta_pow
  real(lng), intent(in) :: omega_k,ff1,ff2,ff3,rho,mu,r_root,r_min,trm_rst,Lr,Fr2,q,g
  
  integer      :: generations
  real(lng)    :: nu, r, r_d, l, lrr, A, A_d, D, wom
  !real(lng)   :: w, w_d, wrr
  real(lng)    :: beta, alpha
  complex(lng) :: i, g_omega, c_omega, kappa, Z0, ZL, Zl_0, Zr_0, t1, t2, wom0, cbj0, cdj0, cbj1, cdj1, cby0, cdy0, cby1, cdy1 , Fj

  ! Physical constants.
  i     = cmplx(0.0_lng,1.0_lng,lng) ! The complex unit.

  lrr   = 50.0_lng                   ! Length to radius ratio.
  !wrr  = 0.154_lng                  ! Wall-thickness to radius ratio.

  beta  = ((asym**(expo/2)+1.0)**(-1/expo))**beta_pow  ! Scaling parameter.
  alpha = (sqrt(asym)*(asym**(expo/2)+1.0)**(-1/expo))**alpha_pow   ! do.
  !write(*,*) 'Beta', beta**(1.0d0/beta_pow)
  !write(*,*) 'Alpha',alpha**(1.0d0/alpha_pow)

  generations = alpha_pow + beta_pow  ! The current generation.

  r_d  = alpha*beta*r_root     ! Radius at root, cm.
  A_d  = pi*r_d**2             ! Cross-sectional area, cm^2.
  !w_d = wrr*r_d               ! Wall-thickness, cm.
  r    = r_d                   ! Radius at root, dimension-less.
  A    = A_d                   ! Cross-sectional area, dimension-less.
  l    = lrr*r                 ! Length of vessel segment.
  !w   = w_d                   ! Wall-thickness.
  !mu  = mu_pl*(v1/(v2 + exp(-v3*r_d)) + v4) ! If viscous dependend mu
  nu   = mu/rho                ! Kinematic blood viscosity, m^2/s.
  D    = 1/(ff1*exp(ff2*r_d)+ff3)*3*A_d/2 ! Distensibility.
  wom  = r_d*sqrt(omega_k/nu)  ! Womersleys parameter.

  ! Temporary functions of r.
  ! if (wom > 3.0) then 
  !   g_omega = sqrt(D*A/rho)* &
  !             sqrt(1.0_lng-2.0/i**(0.5)/wom*(1.0_lng+1.0/2.0/wom)) 
  !   c_omega = sqrt(A/D/rho)* &
  !             sqrt(1.0_lng-2.0/i**(0.5)/wom*(1.0_lng+1.0/2.0/wom))
  ! else
  !   if (wom > 2.0) then
  !     g_omega = sqrt(D*A/rho)*((3.0_lng-wom)* &
  !               sqrt(i*wom**2.0/8.0+wom**4.0/48.0) + &
  !               (wom-2.0_lng)*&
  !               sqrt(1.0_lng-2.0/i**(0.5)/wom*(1.0_lng+1.0/2.0/wom)))
  !     c_omega = sqrt(A/D/rho)*((3.0_lng-wom)* &
  !               sqrt(i*wom**2.0/8.0+wom**4.0/48.0) + &
  !               (wom-2.0_lng)*&
  !               sqrt(1.0_lng-2.0/i**(0.5)/wom*(1.0_lng+1.0/2.0/wom)))
  !   else
  !     if (wom == 0) then
  !       g_omega = 0.0
  !       c_omega = 0.0
  !     else
  !       g_omega = sqrt(D*A/rho)*sqrt(i*wom**2/8+wom**4/48)
  !       c_omega = sqrt(A/D/rho)*sqrt(i*wom**2/8+wom**4/48)
  !     end if
  !   end if
  ! end if

  !Change approximate bessel function to exact bessel function
  wom0 = i**1.5*wom
  call cjy01 ( wom0, CBJ0, CDJ0, CBJ1, CDJ1, CBY0, CDY0, CBY1, CDY1 )
  Fj = sqrt(1.0_lng-2.0_lng*CBJ1/CBJ0/wom0)
  g_omega = sqrt(D*A/rho)*Fj
  c_omega = sqrt(A/D/rho)*Fj
  if (wom == 0) then
          g_omega = 0.0
          c_omega = 0.0
  end if

  ! Temporary function of omega_k. 
   if (omega_k /= 0) then
    kappa  = omega_k*l/c_omega
  else
    kappa   = 0.0
  end if

  ! Determine the resistance of the root of the terminal branches.
  ! if (generations >= Maxgen) 
  if (r <= r_min) then
    if (generations >= localmax) then
      localmax = generations
    end if
    if (generations >= Maxgen) then
      write (*,*) 'Generations level exceeded'
    end if
    ZL = trm_rst
  else
    ! Get Z0 recursively at reduced frequencies.
    if (abs(Computed(alpha_pow+1, beta_pow)) /= 0.0) then
      Zl_0 = Computed(alpha_pow+1, beta_pow)
    else  
      Zl_0 = Z0func (omega_k,alpha_pow+1,beta_pow,ff1,ff2,ff3,rho,mu,r_root,r_min,trm_rst,Lr,Fr2,q,g)
    end if

    if (abs(Computed(alpha_pow, beta_pow+1)) /= 0.0) then
      Zr_0 = Computed(alpha_pow, beta_pow+1)
    else
      Zr_0 = Z0func (omega_k,alpha_pow,beta_pow+1,ff1,ff2,ff3,rho,mu,r_root,r_min,trm_rst,Lr,Fr2,q,g)
    end if

    ! Prediction of the resulting impedance from the recursion formula.
    ZL = rho*g*Lr/(q/Zl_0 + q/Zr_0)
  end if
 
  ! Finally get Z0(omega) as theoretically derived.
  if (g_omega /= 0.0) then 
    t1   = i*sin(kappa)/g_omega + cos(kappa)*ZL
    t2   = cos(kappa) + i*g_omega*sin(kappa)*ZL
    Z0   = q/(rho*g*Lr)*(t1/t2)
  else 
    Z0   = q/(rho*g*Lr)*(8*mu*l/(A*r**2) + ZL)
  end if

  Computed(alpha_pow,beta_pow) = Z0
end function Z0func



!***************************************************************************
!*                                                                         *
!* Function: comp_imp                                                      *
!* Version: 1.0                                                            *
!* By: Mette Olufsen, Math-Tech                                            *
!* Last modified: 14. Jan. 1997                                            * 
!*                                                                         *
!* Compute the impedance using a recursive formula for an asymmetric       *
!* tree.                                                                   *
!*                                                                         *
!* This subroutine is called from subroutine impedance by:                 *
!*                                                                         *
!* Z_om = comp_imp (N,Omega,ff*,rho,mu,r_root,Lr,Fr2,q,g)                  *
!*                                                                         *
!* comp_imp  Returns frequency dependent impedances (Z_om) at the root of  *
!*           the structured asymmetric tree.                               *
!* N         Number of frequencies (in Omega).                             *  
!* Omega     A vector containing N frequencies.                            *
!* ff*       Constants to model Eh/r=ff1*exp(ff2*r)+ff3, g/cm/s^2.         *
!* ff*       Constants to model Eh/r=-ff1*tanh(ff2*(r-ff3))+ff4, g/cm/s^2. *
!* rho       The density of the blood, g/cm^3.                             *
!* mu        Bloodplasma viscosity, g/cm/s.                                *
!* r_root    The input radius at the root of the structured tree, cm.      *
!* Lr        Characteristic length scale (radius), cm.                     *
!* Fr2       The squared Froude number.                                    *
!* q         Characteristic flow-rate, cm^3/s.                             *
!* g         The gravitational force, cm/s^2.                              *
!*                                                                         *
!* This function routine is a driver routine that uses Z0func to compute   *
!* Z_om at each generation.                                                *
!*                                                                         *
!***************************************************************************
function comp_imp (N,Omega,trm_rst,ff1,ff2,ff3,rho,mu,r_root,r_min,Lr,Fr2,q,g) result (Z_om)
implicit none

  integer, intent(in)       :: N
  real(lng), intent(in)     :: Omega(:),trm_rst,ff1,ff2,ff3,rho,mu,r_root,Lr,Fr2,q,g,r_min
  complex(lng)              :: temp, Z_om(N)
  integer                   :: k

  ! Omega contains tmstps+1 frequencies because when computing the
  ! impedance it is easier to invert it when it is computed for all positive
  ! frequencies and since the interval goes from [-N/2/Tper:N/2/Tper-df],
  ! we include the frequency N/2/Tper in Omega and from this we get
  ! Z(-N/2/Tper) we then end up throwing pushing Z(N/2/Tper) out.
 
  Z_om = cmplx (0.0_lng,0.0_lng,lng) ! Initialize Z_om with zeros

  ! For all the positive frequencies compute the impedance.
  ! Since we know that the system is self-adjoint we don't
  ! have to compute the negative frequencies as well. 
  do k = N/2+1, N+1
    Computed  = 0.0_lng ! For each frequency make sure we don't carry
                        ! any values with us from the computations for the
                        ! previous frequency, so make sure Computed is zero.
    ! Since Z_om only has N places leave result for the frequency k at
    ! Z_om(k-1) we will later make up for this.

    Z_om(k-1) = Z0func (Omega(k),0,0,ff1,ff2,ff3,rho,mu,r_root,r_min,trm_rst,Lr,Fr2,q,g)
  end do 
  
  temp = Z_om (N/2)
  
  ! Apply self-adjointness 
  Z_om(1:N/2)   = conjg(flipud(Z_om(N/2+1:N)))

  ! Shift the results for the positive frequencies one to the right
  ! to make up for the above.
  Z_om(N/2+1:N) = eoshift(Z_om(N/2+1:N),-1)    

  ! Insert Z(0,0) as described in the theoretical derivation
  Z_om(N/2+1) = temp

end function comp_imp


!***************************************************************************
!*                                                                         *
!* Subroutine: impedance                                                   *
!* Version: 1.0                                                            *
!* By: Mette Olufsen, Math-Tech                                            *
!* Last modified: 14. Jan. 1997                                            * 
!*                                                                         *
!*                                                                         *
!* Compute the Impedance as a function of space and time at the root of a  *
!* structured tree.                                                        *
!*                                                                         *
!* This subroutine is called from the c++ subroutine arteries.cxx whenever *
!* a new end-tube (of the tree of larger arteries) is constructed. We then *
!* use the resulting impedances to get a time-dependent boundary-condition.*
!* It is called by:                                                        *
!*                                                                         *
!* Impedance (tmstps,Tper,ff*,rho,mu,r_root,r_min,z_xt,Lr,Fr2,q,g)         *
!*                                                                         *
!* where:                                                                  *
!*                                                                         *
!* tmstps    Number of sample points in time (Must be a power of 2)!       *
!* Tper      The period of the simulation, s.                              *
!* ff*       Constants to model Eh/r = ff1*exp(ff2*r)+ff3, g/cm/s^2.       *
!* ff*       Constants to model Eh/r=-ff1*tanh(ff2*(r-ff3))+ff4, g/cm/s^2. *
!* rho       Blood density, g/cm^3.                                        *
!* mu        Bloodplasma viscosity, g/cm/s.                                *
!* r_root    The radius of the root of the structured tree, cm.            *
!* r_min     The terminal radius of the structured tree, cm.               *
!* z_xt      Will return the impedances at the root of the structured tree *
!*           in the time domain.                                           *
!* Lr        Characteristic length scale (radius), cm.                     *
!* Fr2       The squared Froude number.                                    *
!* q         Characteristic flow-rate, cm^3/s.                             *
!* g         The gravitational force, cm/s^2.                              *
!*                                                                         *
!***************************************************************************
subroutine impedance (tmstps,Period,ff1,ff2,ff3,rho,mu,r_root,r_min,y_xt,Lr,Fr2,q,g,trm_rst)
implicit none

  integer,   intent(in)      :: tmstps
  real(lng), intent(in)      :: Period,ff1,ff2,ff3,rho,mu,Lr,Fr2,q,g,r_root,r_min,trm_rst
! real(lng)                  :: z_xt(tmstps)
  real(lng)                  :: y_xt(tmstps)

  integer                    :: j
! integer                    :: nb_terms
! real(lng)                  :: beta, alpha
  real(lng)                  :: df, Freq(tmstps+1), Omega(tmstps+1)!, trm_rst
  complex(lng)               :: Z_hat(tmstps), Y_hat(tmstps)

  integer, parameter                  :: nbuf = 2, f1 = 10
  character (len=30)                  :: fn
  character (len=40), dimension(nbuf) :: buffer  ! Temporary strings
  integer k
  
  ! Physical parameters
  df     = 1/Period                            ! Frequency interval. 
  Freq   = (/ (j*df, j=-tmstps/2, tmstps/2) /) ! Frequency-vector (abscissae). 8192
  Omega  = 2*pi*Freq                           ! Freq.-vector scaled by a factor 2pi.

  !beta  = ((asym**(expo/2)+1.0)**(-1/expo))    ! Scaling parameter.
  !alpha = (sqrt(asym)*beta)                    ! do.
  !nb_terms = 0
  !call counting (0,0,alpha,beta,r_root,r_min,nb_terms)

  localmax = 0
  !trm_rst = 0     ! Terminal resistance could be (nb_terms*resist)

  ! Compute the impedance at the root of the structured tree.
  Z_om =comp_imp (tmstps,Omega,trm_rst,ff1,ff2,ff3,rho,mu,r_root,r_min,Lr,Fr2,q,g)
  ! Z_om(1) = real(Z_om(1),lng)   ! Dirty hack, that makes Z_om real
                                ! first at the lowest frequency.

  ! Transform P back to the time domain. 
  ! Divide by tmstps to approximate continuous inv. Fourier transform.
  ! In particular, amplitude must be independent of resolution.
! z_xt   = real(IFFT(bitreverse(FFTshift(Z_om)/Period)),lng)
  Z_hat = Z_om
  Y_hat = 1/Z_om
  y_xt   = real(IFFT(bitreverse(FFTshift(Y_hat)/Period)),lng)

  write (buffer(1),'(I4)') floor(1000*r_root)
  write (buffer(2),'(I4)') floor(100*r_min)
  do k = 1, nbuf
    buffer(k) = adjustl(buffer(k))
  end do
  fn = 'Zhat' // trim(buffer(1)) // '_' // trim(buffer(2))

  open (f1, file=fn, action='write') 
  do k=1,tmstps
    write (f1,'(3F26.16)') Omega(k)/Lr**3*q, Z_hat(k)*rho*g*Lr/q
  end do 
  close(f1)
  
  return
  
end subroutine impedance


subroutine impedance_init (tmstps)

  integer,   intent(in)  :: tmstps
 
  allocate(Z_om(tmstps))
  
end subroutine impedance_init


subroutine impedance_close 

  deallocate(Z_om)
    
end subroutine impedance_close

end module root_imp



