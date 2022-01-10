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
                Deltat, tau, m1, m2, m3, m4, Cn, invD, C_Period,
	              *fjac[18], *dff[9], xr, f, df;

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
         *Cnew, *Cold, *Ch,
         *dCh, *dCold, 
         *R1, *R2, *R1h, *R2h,
         *S1, *S2, *S1h, *S2h,
         *R3, *S3, *R3h, *S3h,
         *Qprv, *Aprv,
         *pL, *y, *QL, *Q0, *Z,
	 *r0, *r0h,
	 *dr0dx, *dr0dxh,
	 *A0, *A0h, *wom, *Cu,
	 *fr, *frh,
	 *dfrdr0, *dfrdr0h,
	 *p1, *p1h,
	 *dp1dr0, *dp1dr0h;

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
  void printQ0  (FILE *fd);
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
  void printDxt (FILE *fd, double t, int offset);

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
  void step1 (double k);
  void step2 (double k);

  // Updates left bndry. This should only be done for the inlet tube.
  void bound_left (double t, double k, double Period);


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
inline void Update(){
  for (int i=0; i<=N; i++)  // Remember the values at this time level.
  {
    Qold[i] = Qnew[i];
    Aold[i] = Anew[i];
    Cold[i] = Cnew[i];
  }
}

inline void Get_dC(){
  dCold[0] = (Cold[1]-Cold[0])/h;
  for (int i=1; i<N; i++)  // Remember the values at this time level.
  {
    dCold[i] = (Cold[i+1]-Cold[i-1])/h/2;
  }
  dCold[N] = (Cold[N]-Cold[N-1])/h;
}

inline void Get_dCh(){
  dCh[0] = (0.5*(Ch[2]+Ch[1])-Ch[0])/h;
  for (int i=1; i<N-1; i++)  // Remember the values at this time level.
  {
    dCh[i] = (Ch[i+2]-Ch[i])/h/2;
  }
  dCh[N-1] = (Ch[N+1]-0.5*(Ch[N]+Ch[N-1]))/h;
}

double C0(double t);

  // The private function Q0 may only be accessed from the left boundary
  // function. It ensures a certain and given CO (defined in main.h).
  double Q0_init (double t, double k, double Period);
  
};

void solver (Tube *Arteries[], double tstart, double tend, double k, set<int>& ID_Out, set<int>& ID_Bif);

#endif
