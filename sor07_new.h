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

int    tmstps = 8192,                 // The number of timesteps per period.
       plts   = 1024;                 // Number of plots per period.

const char *CO_filename = "FlowProfile/masri_8192.dat";     // Input flow file at the heart.

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
       
       alpha  = 1.1,                  // flow profile
       //Fcst = 10.0,                 // Determines the damping coeff.
       Fcst   = 2*alpha/(alpha-1), //17.7778,        // Determines the damping coeff.
                                      // for the friction.
       
       Lr     = 1.0,                  // Characteristic radius of the
                                      // vessels in the tree [cm].
       Lr2    = sq(Lr),               // The squared radius [cm2].
       Lr3    = cu(Lr),               // The radius to the third power [cm^3].
       g      = 981.0,                // The gravitational force [cm/s^2].
       q      = 10.0*Lr2,             // The characteristic flow [cm^3/s].
       Fr2    = sq(q)/g/pow(Lr,5),    // The squared Froudes number.
       Re     = q*rho/mu/Lr,          // Reynolds number.
       Period = Tper*q/Lr3,           // The dimension-less period.
       C_Period = 0.2*q/Lr3,
       tau    = 0.08*q/Lr3,           // End of pulse, dimension-less.
       k      = Period/tmstps,        // Length of a timestep.
       Deltat = Period/plts,          // Interval between each point plottet.
       // p0     = 55.0/rho/g/Lr*conv,   // Ensures a certain diastolic pressure.
        p0     = 55./rho/g/Lr*conv,   // Ensures a certain diastolic pressure.

       D_d    = 0.02,                 // cm^2/s
       invD   = sq(alpha-1)*q/Lr/D_d/2/alpha/(3*alpha-2),
       Cn     = 1.,

       *fjac[18],                     // Work space used by bound_bif.
       xr, f, df;                     // Work space used by bound_right.
// double K_diss = 0.;
// double ahpha_a=0.9,                   // the ratio of area at stenosis
//        ku = 1.2,                      // kv is a function, ku,kt are constant.
//        kt = 1.5;

#endif
