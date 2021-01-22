#include "sor07_new.h"
#include "tools.h"
#include "arteries.h"

extern "C"  void impedance_init_driver_(int *tmstps);

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <cstring>
#include <string>
#include <vector>
#include <set>

using namespace std;

int main(int argc, char *argv[])
{
  double tstart, tend, finaltime;

  // normal range of rmin of small vessel is 0.01~0.04
  double rm4 = 0.03;
  int point = 8;
  nbrves     = 55;             // The number of vessels in the network.
  string FileName = "topology55.txt";
  set<int> ID_Out, ID_Bif;
  get_ID(FileName, ID_Out, ID_Bif);
  // pressure and flow file point name:
  vector<string> nameP(nbrves, "./result/p");
  vector<string> nameQ(nbrves, "./result/q");
  FILE *fp[nbrves];
  FILE *fq[nbrves];
  for(int i=0;i<nbrves;i++)
  {
    auto j=i+1;
    nameP[i] += to_string(j);
    nameQ[i] += to_string(j);
  }
  const char *nameA1 = "./result/A1";
  const char *nameC1 = "./result/C1";


  // open pressure and flow file and declare the point:
  for(int i=0;i<nbrves;i++)
  {
    auto j=i+1;
    fp[i] = fopen((nameP[i]).c_str(), "w");
    string SuccessInfo1 = "File "+to_string(j)+"p OK \n";
    string FailInfo1 = "File "+to_string(j)+"p NOT OK \n";
    if (fp[i]) fprintf(stdout, "%s", (SuccessInfo1).c_str()); else error("main.C", (FailInfo1).c_str());
    fq[i] = fopen((nameQ[i]).c_str(), "w");
    string SuccessInfo2 = "File "+to_string(j)+"q OK \n";
    string FailInfo2 = "File "+to_string(j)+"q NOT OK \n";
    if (fq[i]) fprintf(stdout, "%s", (SuccessInfo2).c_str()); else error("main.C", (FailInfo2).c_str());
  }
  FILE *fA1 = fopen(nameA1, "w");
  if (fA1) fprintf(stdout, "File 1A OK \n"); else error("main.C", "File 1A NOT OK");
  FILE *fC1 = fopen(nameC1, "w");
  if (fC1) fprintf(stdout, "File 1C OK \n"); else error("main.C", "File 1C NOT OK");

  // Workspace used by bound_bif
  for(int i=0; i<18; i++) fjac[i] = new double[18];

  clock_t c1 = clock();        // Only used when timing the program.
  tstart     = 0.0;            // Starting time.
  finaltime  = 8*Period;       // Final end-time during a simulation.
  tend       = 7*Period;       // Timestep before the first plot-point
                               // is reached.

  // The number of vessels in the network is given when the governing array of
  // vessels is declared.

  impedance_init_driver_(&tmstps);

  Tube   *Arteries[nbrves];                    // Array of blood vessels.

  // Geomety of arteries
  // Left external carotid
  Arteries[54] = new Tube(15.750,0.275,0.275,0,0,0,0,rm4,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0);
  // Left internal carotid
  Arteries[53] = new Tube(15.750,0.275,0.275,0,0,0,0,rm4,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0);
  // Right external carotid
  Arteries[52] = new Tube(15.750,0.275,0.275,0,0,0,0,rm4,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0);
  // Right internal carotid
  Arteries[51] = new Tube(15.750,0.275,0.275,0,0,0,0,rm4,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0);
  // Left Posterior tibal
  Arteries[50] = new Tube(32.000,0.125,0.125,0,0,0,0,rm4,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0);
  // Left Anterior Tibal
  Arteries[49] = new Tube(32.000,0.125,0.125,0,0,0,0,rm4,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0);
  // Left Femoral B
  Arteries[48] = new Tube(44.250,0.314,0.200,0,0,Arteries[49],Arteries[50],0,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0);
  // Left Deep femoral
  Arteries[47] = new Tube(11.250,0.200,0.200,0,0,0,0,rm4,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0);
  // Left Internal iliac
  Arteries[46] = new Tube(4.500,0.200,0.200,0,0,0,0,rm4,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0);
  // Left Femoral A
  Arteries[45] = new Tube(14.500,0.370,0.314,0,0,Arteries[47],Arteries[48],0,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0);
  // Left External iliac
  Arteries[44] = new Tube(5.750,0.400,0.370,0,0,Arteries[45],Arteries[46],0,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0);
  // Right Posterior tibal
  Arteries[43] = new Tube(32.000,0.125,0.125,0,0,0,0,rm4,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0);
  // Right Anterior Tibal
  Arteries[42] = new Tube(32.000,0.125,0.125,0,0,0,0,rm4,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0);
  // Right Femoral B
  Arteries[41] = new Tube(44.250,0.314,0.200,0,0,Arteries[42],Arteries[43],0,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0);
  // Right Deep femoral
  Arteries[40] = new Tube(11.250,0.200,0.200,0,0,0,0,rm4,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0);
  // Right Internal iliac
  Arteries[39] = new Tube(4.500,0.200,0.200,0,0,0,0,rm4,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0);
  // Right Femoral A
  Arteries[38] = new Tube(14.500,0.370,0.314,0,0,Arteries[40],Arteries[41],0,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0);
  // Right External iliac
  Arteries[37] = new Tube(5.750,0.400,0.370,0,0,Arteries[38],Arteries[39],0,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0);
  // Abdominal aorta E
  Arteries[36] = new Tube(8.000,0.627,0.550,0,0,Arteries[37],Arteries[44],0,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0);
  // Inferior mesenteric
  Arteries[35] = new Tube(3.750,0.200,0.175,0,0,0,0,rm4,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0);
  // Abdominal aorta D
  Arteries[34] = new Tube(12.500,0.792,0.627,0,0,Arteries[35],Arteries[36],0,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0);
  // Left Renal
  Arteries[33] = new Tube(3.000,0.275,0.275,0,0,0,0,rm4,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0);
  // Abdominal aorta C
  Arteries[32] = new Tube(1.500,0.814,0.792,0,0,Arteries[33],Arteries[34],0,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0);
  // Right Renal
  Arteries[31] = new Tube(3.000,0.275,0.275,0,0,0,0,rm4,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0);
  // Abdominal aorta B
  Arteries[30] = new Tube(1.500,0.838,0.814,0,0,Arteries[31],Arteries[32],0,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0);
  // Superior mesenteric
  Arteries[29] = new Tube(5.000,0.400,0.350,0,0,0,0,rm4,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0);
  // Abdominal aorta A
  Arteries[28] = new Tube(5.250,0.924,0.838,0,0,Arteries[29],Arteries[30],0,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0);
  // Splenic
  Arteries[27] = new Tube(5.500,0.200,0.200,0,0,0,0,rm4,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0);
  // Gastric
  Arteries[26] = new Tube(5.750,0.175,0.150,0,0,0,0,rm4,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0);
  // Hepatic B
  Arteries[25] = new Tube(6.500,0.275,0.250,0,0,0,0,rm4,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0);
  // Hepatic A
  Arteries[24] = new Tube(2.000,0.300,0.250,0,0,Arteries[26],Arteries[27],0,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0);
  // Celiac axis
  Arteries[23] = new Tube(2.000,0.350,0.300,0,0,Arteries[24],Arteries[25],0,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0);
  // Left Interosseus
  Arteries[22] = new Tube(7.000,0.100,0.100,0,0,0,0,rm4,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0);
  // Left Ulner B
  Arteries[21] = new Tube(22.250,0.175,0.175,0,0,0,0,rm4,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0);
  // Left Ulner A
  Arteries[20] = new Tube(17.000,0.200,0.200,0,0,Arteries[21],Arteries[22],0,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0);
  // Right & Left Radial
  Arteries[19] = new Tube(22.000,0.175,0.175,0,0,0,0,rm4,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0);
  // Left Brachial
  Arteries[18] = new Tube(39.750,0.407,0.250,0,0,Arteries[19],Arteries[20],0,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0);
  // Left Vertebral
  Arteries[17] = new Tube(13.500,0.200,0.200,0,0,0,0,rm4,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0);
  // Left Subclavian
  Arteries[16] = new Tube(3.500,0.425,0.407,0,0,Arteries[17],Arteries[18],0,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0);
  // Intercostals
  Arteries[15] = new Tube(7.250,0.630,0.500,0,0,0,0,rm4,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0);
  // Thoracic aorta B
  Arteries[14] = new Tube(10.500,1.124,0.924,0,0,Arteries[23],Arteries[28],0,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0);
  // Thoracic aorta A
  Arteries[13] = new Tube(5.500,1.246,1.124,0,0,Arteries[14],Arteries[15],0,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0);
  // Left common Carotid
  Arteries[12] = new Tube(19.250,0.525,0.400,0,0,Arteries[53],Arteries[54],0,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0);
  // Aortic arch B
  Arteries[11] = new Tube(4.000,1.342,1.246,0,0,Arteries[13],Arteries[16],0,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0);
  // Right Interosseus
  Arteries[10] = new Tube(7.000,0.100,0.100,0,0,0,0,rm4,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0);
  // Right Ulner B
  Arteries[9] = new Tube(22.250,0.175,0.175,0,0,0,0,rm4,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0);
  // Right Ulner A
  Arteries[8] = new Tube(17.000,0.200,0.200,0,0,Arteries[9],Arteries[10],0,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0);
  // Right Radial
  Arteries[7] = new Tube(22.000,0.175,0.175,0,0,0,0,rm4,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0);
  // Right Brachial
  Arteries[6] = new Tube(39.750,0.407,0.250,0,0,Arteries[7],Arteries[8],0,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0);
  // Right Vertebral
  Arteries[5] = new Tube(13.500,0.200,0.200,0,0,0,0,rm4,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0);
  // Right common Carotid
  Arteries[4] = new Tube(16.750,0.525,0.400,0,0,Arteries[51],Arteries[52],0,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0);
  // Right Subclavian
  Arteries[3] = new Tube(3.500,0.425,0.407,0,0,Arteries[5],Arteries[6],0,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0);
  // Brachiocephalic
  Arteries[2] = new Tube(3.500,0.950,0.700,0,0,Arteries[3],Arteries[4],0,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0);
  // Aortic arch A
  Arteries[1] = new Tube(3.000,1.420,1.342,0,0,Arteries[11],Arteries[12],0,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0);
  // Ascending aorta
  Arteries[0] = new Tube(4.000,1.525,1.420,0,0,Arteries[1],Arteries[2],0,point,1,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0);

  fprintf (stdout,"finish construction\n");


  // In the next three statements the simulations are performed until
  // tstart = tend. That is this is without making any output during this
  // first period of time. If one needs output during this period, these three
  // lines should be commented out, and the entire simulation done within the
  // forthcomming while-loop.

  // Solves the equations until time equals tend.
  solver (Arteries, tstart, tend, k, ID_Out, ID_Bif);
  tstart = tend;
  tend = tend + Deltat;


  fprintf (stdout,"plots start\n");


  // The loop is continued until the final time
  // is reached. If one wants to make a plot of
  // the solution versus x, tend is set to final-
  // time in the above declaration.
  while (tend <= finaltime)
  {
    for (int j=0; j<nbrves; j++)
    {
      int ArtjN = Arteries[j]->N;     // The number of grid points along the vessel
      for (int i=0; i<ArtjN; i++)
      {
        Arteries[j]->Qprv[i+1] = Arteries[j]->Qnew[i+1];
        Arteries[j]->Aprv[i+1] = Arteries[j]->Anew[i+1];
      }
    }


    // Solves the equations until time equals tend.
    solver (Arteries, tstart, tend, k, ID_Out, ID_Bif);
    fprintf (stdout,".");




    // call output member function:
    for(int i=0;i<nbrves;++i)
    {
      Arteries[i]->printPxt(fp[i], tend, 0);
      Arteries[i]->printQxt(fq[i], tend, 0);
    }
    Arteries[0]->printAxt(fA1, tend, 0);
    Arteries[0]->printCxt(fC1, tend, 0);


    // The time within each print is increased.
    tstart = tend;
    tend   = tend + Deltat; // The current ending time is increased by Deltat.
  }
  fprintf(stdout,"\n");


  // The following statements is used when timing the simulation.
  fprintf(stdout,"nbrves = %d, Lax, ", nbrves);
  clock_t c2 = clock(); // FIXME clock() may wrap after about 72 min.
  int tsec = (int) ((double) (c2-c1)/CLOCKS_PER_SEC + 0.5);
  fprintf(stdout,"cpu-time %d:%02d\n", tsec / 60, tsec % 60);
  fprintf(stdout,"\n");

  // In order to termate the program correctly the vessel network and hence
  // all the vessels and their workspace are deleted.
  for (int i=0; i<nbrves; i++) delete Arteries[i];

  // Matrices and arrays are deleted
  for (int i=0; i<18; i++) delete[] fjac[i];



  // close the files:
  for(int i=0;i<nbrves;++i)
  {
    auto j=i+1;
    string SuccessInfo1 = "Close "+to_string(j)+"p OK \n";
    string FailInfo1 = "Close "+to_string(j)+"p NOT OK \n";
    if (fclose(fp[i]) != EOF) fprintf(stdout, "%s", (SuccessInfo1).c_str()); else error("main.C", (FailInfo1).c_str());
    string SuccessInfo2 = "Close "+to_string(j)+"q OK \n";
    string FailInfo2 = "Close "+to_string(j)+"q NOT OK \n";
    if (fclose(fq[i]) != EOF) fprintf(stdout, "%s", (SuccessInfo2).c_str()); else error("main.C", (FailInfo2).c_str());
  }
  if (fclose(fA1) != EOF) fprintf(stdout, "Close 1A OK\n"); else error("main.C", "Close 1A NOT OK");
  if (fclose(fC1) != EOF) fprintf(stdout, "Close 1C OK\n"); else error("main.C", "Close 1C NOT OK");
  return 0;
}
