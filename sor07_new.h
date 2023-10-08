/***************************************************************************/
/*                                                                         */
/*  Program: sor06.h                                                       */
/*  Version: 2.0                                                           */
/*  By: Mette Olufsen, Math-Tech                                           */
/*  Date: 14. Jan. 1997                                                    */
/*                                                                         */
/*  This header file defines the global parameters                         */
/*                                                                         */
/***************************************************************************/

// $Id: sor06.h,v 1.8 2010-10-20 15:38:28 mette Exp $

#ifndef _SOR07_H
#define _SOR07_H

#include <cmath>
#include "tools.h"

int    nbrves, N_aorta;               // Number of vessels in the tree.

int    tmstps = 16384,                 // The number of timesteps per period. 16384
       plts   = 128;                 // Number of plots per period.

const char *CO_filename = "input.dat";     // Input flow file at the heart.

double conv   = 1332.20,              // Conversion from mmHg to SI-units.
       rho    = 1.055,                // Density of blood [g/cm^3].
       mu     = 0.049,                // Viscosity of blood [g/cm/s].
       mu_pl  = mu,                   // Viscosity of blood [g/cm/s].
       nu     = mu/rho,

       Tper   = 1.00,                 // The period of one heart beat [s].

    //    ff1    = 0.999639e07,
    //    ff2    = -11.2635,
    //    ff3    = 432631,
       ff1  = 1.99925e+07,
       ff2  = -22.5267,
       ff3  =  865251,
       fa1  = 1.89e+07,
       fa2  = -18.34,
       fa3  = 3.53e+06,
       alpha_value_ST = 0.91, 
       beta_value_ST = 0.58, 
       //ff1  = 1.99925e07,
       //ff2  = -25.5267,
       //ff3  = 465251,
       // GUESS FROM REVIEW PAPER
       //ff1 = 0.999639e07,
       //ff2 = -5.2635,
       //ff3 = 6432631,

       m1     = 7.3693,
       m2     = 1.2122,
       m3     = 5.6517,
       m4     = 0.21763,

       //Fcst = 10.0,                 // Determines the damping coeff.
       Fcst   = 22, //17.7778,        // Determines the damping coeff. Fcst = (2+\xi)*2
                                      // for the friction.             \xi = 9
       // alpha  = 4./3,
       Lr     = 1.0,                  // Characteristic radius of the
                                      // vessels in the tree [cm].
       Lr2    = sq(Lr),               // The squared radius [cm2].
       Lr3    = cu(Lr),               // The radius to the third power [cm^3].
       g      = 981.0,                // The gravitational force [cm/s^2].
       q      = 10.0*Lr2,             // The characteristic flow [cm^3/s].
       Fr2    = sq(q)/g/pow(Lr,5),    // The squared Froudes number.
       Re     = q*rho/mu/Lr,          // Reynolds number.
       Period = Tper*q/Lr3,           // The dimension-less period.
       tau    = 0.08*q/Lr3,           // End of pulse, dimension-less.
       k      = Period/tmstps,        // Length of a timestep.
       Deltat = Period/plts,          // Interval between each point plottet.
       // p0     = 55.0/rho/g/Lr*conv,   // Ensures a certain diastolic pressure.
        p0     = 75./rho/g/Lr*conv,   // Ensures a certain diastolic pressure.

       *fjac[18],                     // Work space used by bound_bif.
       xr, f, df;                     // Work space used by bound_right.
// double K_diss = 0.;
// double ahpha_a=0.9,                   // the ratio of area at stenosis
//        ku = 1.2,                      // kv is a function, ku,kt are constant.
//        kt = 1.5;

                                                        // unit:
double ke_no = 1.0*(Lr3/q),                             // 1/s to dimensionless
       nno_0 = 0.1,                                     // mu mol/L       
       delta_no_inner = 0.1*(Lr3/q),                          // 1/s to dimensionless
       delta_no_middle = 0.01*(Lr3/q),                          // 1/s to dimensionless
       alpha1_ca = 0.05*(Lr3/q),                        // 1/s to dimensionless
       D_inner = 3300e-8*(Lr3/q)/Lr2,                   // cm^2/s to dimensionless
       D_middle = 3300e-8*(Lr3/q)/Lr2,                  // cm^2/s to dimensionless
       k2_ca = 0.9*0.015*(Lr3/q)*nno_0,                 // L/s/(mu mol) to dimensionless
                                                        // 0.9 is the damp from endothelium to media layer from tang results
       Cca_th = 0.1,                                    // mu mol/L  
       phica_0 = 5.81e-3*(Lr3/q)/Cca_th,                // mu mol/L/s to dimensionless
       cGMP0 = 0.2616,                                  // mu mol
       alpha2_cGMP = 0.5*(Lr3/q),                       // 1/s to dimensionless
       gamma_cGMP = 10*(Lr3/q)*Cca_th/cGMP0,            // 1/s to dimensionless
       epsilon_cGMP = 0.1,    
       k3 = 153320/(rho*g*Lr)*(2000/450);               // tang k3 = P0*R0/F0/h0    F0 omite here due to contain in F
                                                        // F0 is same as cGMP0    


double dr = 1e-3;


#endif
