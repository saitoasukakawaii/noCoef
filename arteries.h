/***************************************************************************/
/*                                                                         */
/*  Program: arteries.h                                                    */
/*  Version: 2.0                                                           */
/*  By: Mette Olufsen, Math-Tech                                           */
/*  Date: 14. Jan. 1997                                                    */
/*                                                                         */
/*  This header file includes the definition of the global constants, and  */
/*  the definition of the class representing one blood vessel.             */
/*                                                                         */
/***************************************************************************/

// $Id: arteries.h,v 1.10 2010-10-20 15:38:28 mette Exp $

#ifndef _ARTERIES_H
#define _ARTERIES_H
#include <cstdio>
#include <cmath>
// #define TOTAL_PRESSURE

// Global parameters imported from main.h

extern double   conv, rho, mu, mu_pl, nu, Lr, Lr2, Lr3, g, q, Fr2,
                Re, p0, pmean, tmst, Period, Fcst, alpha, CO, COm,
                Deltat, tau, m1, m2, m3, m4, Tper, 
	              *fjac[18], xr, f, df;

// The class structure.
class Tube {
public:
  double L;                    // The length of the vessel
  double rtop, rbot;           // The top and bottom radii of the vessel
  Tube *LD, *RD;               // The left and right daughter-vessels. If
                               // these are set the Peripheral resistance
                               // should be set to zero.
  Tube *LP, *RP;               // The left and right parent-vessels. Used
                               // only in the dissection.
  double pts;                  // The number of grid points per cm
  double K_loss_LD,
  	     K_loss_RD,
	       K_loss_LP,
	       K_loss_RP;
  double ff1, ff2, ff3;
  double termresist;

  int N;                       // The number of grid points along the vessel
  double h;                    // The interval length of delta x
  double RLrb;                 // The peripheral resistance of the vessel

  double *Qnew, *Qold, *Qh,    // The arrays needed to store data during
         *Anew, *Aold, *Ah,    // the numerical solution of the system.
         *R1, *R2, *R1h, *R2h,
         *S1, *S2, *S1h, *S2h,
         *Qprv, *Aprv,
         *pL, *y, *QL, *Z, 
	 *r0, *r0h,
	 *dr0dx, *dr0dxh,
	 *A0, *A0h, *wom, *Cu,
	 *fr, *frh,
	 *dfrdr0, *dfrdr0h,
	 *p1, *p1h,
	 *dp1dr0, *dp1dr0h;

// heart model
  enum { numSol = 5};
  double pRK4[numSol];
  double dydt[numSol];
  double kRK4[numSol];
  double yRK4[numSol];
  double Q0, P0;

  Tube (double Length,
        double topradius, double botradius,
        Tube *LeftParent, Tube *RightParent,
        Tube *LeftDaughter, Tube *RightDaughter,
        double rmin, double points, int init, double KLD, double KRD, double KLP, double KRP,
        double f1, double f2, double f3, double fa1, double fa2, double fa3, double trmrst);
                                                         // Constructor.
  ~Tube ();                                              // Destructor.


  // Prints P(x_fixed,t), A(x_fixed,t), F(x_fixed,t), or Q(x_fixed,t) for all
  // t's along the tube.
  // void printQ0  (FILE *fd);
  void printHeart (FILE *fd, double t);
  void printPt  (FILE *fd, double t, int i);
  void printQt  (FILE *fd, double t, int i);
  void printAt  (FILE *fd, double t, int i);
  void printFt  (FILE *fd, double t, int i);


  // Prints P(x,t), A(x,t), F(x,t), or Q(x,t) for all x's and t's
  // along the tube. The argument offset makes sure that the vessel
  // is located with the right offset from the inlet.
  void printPxt (FILE *fd, double t, int offset);
  void printAxt (FILE *fd, double t, int offset);
  void printFxt (FILE *fd, double t, int offset);
  void printQxt (FILE *fd, double t, int offset);
  void printCxt (FILE *fd, double t, int offset);
  void printUxt (FILE *fd, double t, int offset);

  // Prints P as a function of Q and A, respectively,  at a given location x.
  void printPQ (FILE *fd, int i);
  void printPA (FILE *fd, int i);

  // Prints dP/dx(x_fixed,t), dQ/dx(x_fixed,t), dA/dt(x_fixed,t),
  // dQ/dt(x_fixed,t), Fric(x_fixed, t), TotConRes(x_fixed,t),
  // TotMomRes(x_fixed,t) for any fixed x and for all t.
  // I.e. all terms including the sums of the momentum and
  // continuity equations.
  void printdAdt      (FILE *fd, double t, int i, double Aprev, double tmst);
  void printdQdx      (FILE *fd, double t, int i);
  void printTotConRes (FILE *fd, double t, int i, double Qprev, double tmst);

  void printdQdt      (FILE *fd, double t, int i, double Qprev, double tmst);
  void printddxQ2divA (FILE *fd, double t, int i);
  void printdPdx      (FILE *fd, double t, int i);
  void printFric      (FILE *fd, double t, int i);
  void printTotMomRes (FILE *fd, double t, int i, double Qprev, double tmst);


  // Defines P(x,A(x,t)).
  double P (int i, double A);

  // Defines dPdA(x,A(x,t)).
  double dPdA (int i, double A);

  // Defines dPdx1(x,A(x,t)).
  double dPdx1 (int i, double A);

  // Defines B(x,A(x,t)).
  double B (int i, double A);

  // Defines Bh(x,A(x,t)).
  double Bh (int i, double A);

  // Defines dBdx1(x,A(x,t)).
  double dBdx1 (int i, double A);

  // Defines dBdx1h(x,A(x,t)).
  double dBdx1h (int i, double A);

  // Defines dBdAh (x,A(x,t)).
  double dBdAh (int i, double A);

  // Defines d2BdAdxh (x, A(x,t));
  double d2BdAdxh (int i, double A);


  // Tests that the CFL-condition is valid throughout the tube.
  double CFL ();


  // Finds the flux acc. to sys. eq.
  double Rvec (int k, int i, int j, double Q, double A);

  // Finds the rhs. of system eq.
  double Svec (int k, int i, int j, double Q, double A);


  // Steps through interior points.
  void step (double k);


  // Updates left bndry. This should only be done for the inlet tube.
  void bound_left (double t, double k, double Period);
  double elastance_lv(const double &t);
  void dydt_heart(const double &t);

  // Updates right bndry. This should only be done for terminal vessels.
  double c  (int i, double A); // The wave speed through aorta.
  double Hp (int i, double Q, double A);
  double Hn (int i, double Q, double A);
  void poschar (double theta, double &qR, double &aR, double &cR, double &HpR);
  void negchar (double theta, double &qS, double &aS, double &cS, double &HnS);
  void bound_right (int qLnb, double k, double theta, double t);


  // Updates bifurcation conditions. Uses daughter vessels, and should
  // only be called when such exist.
  void bound_bif_right (double theta, double gamma);
  void bound_bif_left (double theta, double gamma);

  // In order to ensure a more efficient execution of the program the following
  // functions is made as in-line functions.

// A function returning the Friction of the system. The definition of this
// function is given according to the derivation in the mathematical model.
// The constant cst, determines the amount of damping in the system.
inline double F (double Q, double A)
{
  double tmp1 = Fcst*M_PI*Q;
  double tmp2 = A*Re;
  double tmp3 = -tmp1/tmp2;
  return(tmp3);
  // return(-Fcst*M_PI*Q/(A*Re));
}

inline double dFdQ (double A)
{
  return(-Fcst*M_PI/(A*Re));
}

inline double dFdA (double Q, double A)
{
  return(Fcst*M_PI*Q/(sq(A)*Re));
}

private:
  // The private function Q0 may only be accessed from the left boundary
  // function. It ensures a certain and given CO (defined in main.h).
  void Q0_init (double t, double k);
};

void solver (Tube *Arteries[], double tstart, double tend, double k, set<int>& ID_Out, set<int>& ID_Bif);

//heart parameters
const double p_la = 12*1333.2;
const double v_lv0= 10.0;

const double Aeff_av_max = 5.1;              // cm^2
const double Aeff_va_max = 5.0;              // cm^2
const double l_av = 2.0;                     // cm
const double l_va = 1.0;                     // cm
const double K_vac = 26.7/1333.2;            // mmHg^{-1} s^{-1} to g^{-1} cm s
const double K_vao = 26.7/1333.2;            // mmHg^{-1} s^{-1} to g^{-1} cm s
const double K_avc = 53.3/1333.2;            // mmHg^{-1} s^{-1} to g^{-1} cm s
const double K_avo = 26.7/1333.2;            // mmHg^{-1} s^{-1} to g^{-1} cm s
const double EA = 2.75*1333.2;  				// g cm^{-4} s^{-2}
const double EB = 0.10*1333.2;  				// g cm^{-4} s^{-2}
const int rk4N = 5;
const double rk4a[rk4N] = { 0.0,
        -567301805773.0/1357537059087.0,
        -2404267990393.0/2016746695238.0,
        -3550918686646.0/2091501179385.0,
        -1275806237668.0/842570457699.0};
const double rk4b[rk4N] = { 1432997174477.0/9575080441755.0,
         5161836677717.0/13612068292357.0,
         1720146321549.0/2090206949498.0,
         3134564353537.0/4481467310338.0,
         2277821191437.0/14882151754819.0};
const double rk4c[rk4N] = { 0.0,
         1432997174477.0/9575080441755.0,
         2526269341429.0/6820363962896.0,
         2006345519317.0/3224310063776.0,
         2802321613138.0/2924317926251.0};

#endif
