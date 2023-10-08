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
  double NOtstart, NOfinaltime, NOtend;
                               // is reached.
  // normal range of rmin of small vessel is 0.01~0.04
  double rm4 = 0.03;
  int point = 8;
  nbrves     = 55;             // The number of vessels in the network.

  string FileName = "topology55.txt";
  set<int> ID_Out, ID_Bif, ID_Merge;
  get_ID(FileName, ID_Out, ID_Bif, ID_Merge);

  // pressure and flow file point name:
  vector<string> nameP(nbrves, "./result/p");
  vector<string> nameQ(nbrves, "./result/q");
  vector<string> nameA(nbrves, "./result/A");
  vector<string> nameNO(nbrves, "./result/NO");
  vector<string> nameCA(nbrves, "./result/CA");
  vector<string> namecGMP(nbrves, "./result/cGMP");
  FILE *fp[nbrves];
  FILE *fq[nbrves];
  FILE *fa[nbrves];
  FILE *fNO[nbrves];
  FILE *fCA[nbrves];
  FILE *fcGMP[nbrves];
  for(int i=0;i<nbrves;i++)
  {
    auto j=i+1;
    nameP[i] += to_string(j);
    nameQ[i] += to_string(j);
    nameA[i] += to_string(j);
    nameNO[i] += to_string(j);
    nameCA[i] += to_string(j);
    namecGMP[i] += to_string(j);
  }
  // const char *nameA1 = "./result/A1";
  // const char *nameC1 = "./result/C1";


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
    fa[i] = fopen((nameA[i]).c_str(), "w");
    string SuccessInfo3 = "File "+to_string(j)+"a OK \n";
    string FailInfo3 = "File "+to_string(j)+"a NOT OK \n";
    if (fa[i]) fprintf(stdout, "%s", (SuccessInfo3).c_str()); else error("main.C", (FailInfo3).c_str());
    fNO[i] = fopen((nameNO[i]).c_str(), "w");
    string SuccessInfo4 = "File "+to_string(j)+"NO OK \n";
    string FailInfo4 = "File "+to_string(j)+"NO NOT OK \n";
    if (fNO[i]) fprintf(stdout, "%s", (SuccessInfo4).c_str()); else error("main.C", (FailInfo4).c_str());
    fCA[i] = fopen((nameCA[i]).c_str(), "w");
    string SuccessInfo5 = "File "+to_string(j)+"CA OK \n";
    string FailInfo5 = "File "+to_string(j)+"CA NOT OK \n";
    if (fCA[i]) fprintf(stdout, "%s", (SuccessInfo5).c_str()); else error("main.C", (FailInfo5).c_str());
    fcGMP[i] = fopen((namecGMP[i]).c_str(), "w");
    string SuccessInfo6 = "File "+to_string(j)+"cGMP OK \n";
    string FailInfo6 = "File "+to_string(j)+"cGMP NOT OK \n";
    if (fcGMP[i]) fprintf(stdout, "%s", (SuccessInfo6).c_str()); else error("main.C", (FailInfo6).c_str());
  }
  // FILE *fA1 = fopen(nameA1, "w");
  // if (fA1) fprintf(stdout, "File 1A OK \n"); else error("main.C", "File 1A NOT OK");
  // FILE *fC1 = fopen(nameC1, "w");
  // if (fC1) fprintf(stdout, "File 1C OK \n"); else error("main.C", "File 1C NOT OK");

  // Workspace used by bound_bif
  for(int i=0; i<18; i++) fjac[i] = new double[18];

  clock_t c1 = clock();        // Only used when timing the program.
  tstart     = 0.0;            // Starting time.
  finaltime  = 8*Period;       // Final end-time during a simulation.
  tend       = 0*Period;       // Timestep before the first plot-point
                               // is reached.
  NOtstart     = 0.0;            // Starting time.
  NOfinaltime  = 20*Period;       // Final end-time during a simulation.
  NOtend       = 0*Period;       // Timestep before the first plot-point
                               // is reached.

  // The number of vessels in the network is given when the governing array of
  // vessels is declared.

  impedance_init_driver_(&tmstps);

  Tube   *Arteries[nbrves];                    // Array of blood vessels.

  // Geomety of arteries
  // Left external carotid
  Arteries[54] = new Tube(55, 15.750,0.275,0.275,nullptr,nullptr,nullptr,nullptr,rm4,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0, alpha_value_ST, beta_value_ST);
  // Left internal carotid
  Arteries[53] = new Tube(54, 15.750,0.275,0.275,nullptr,nullptr,nullptr,nullptr,rm4,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0, alpha_value_ST, beta_value_ST);
  // Right external carotid
  Arteries[52] = new Tube(53, 15.750,0.275,0.275,nullptr,nullptr,nullptr,nullptr,rm4,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0, alpha_value_ST, beta_value_ST);
  // Right internal carotid
  Arteries[51] = new Tube(52, 15.750,0.275,0.275,nullptr,nullptr,nullptr,nullptr,rm4,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0, alpha_value_ST, beta_value_ST);
  // Left Posterior tibal
  Arteries[50] = new Tube(51, 32.000,0.125,0.125,nullptr,nullptr,nullptr,nullptr,rm4,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0, alpha_value_ST, beta_value_ST);
  // Left Anterior Tibal
  Arteries[49] = new Tube(50, 32.000,0.125,0.125,nullptr,nullptr,nullptr,nullptr,rm4,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0, alpha_value_ST, beta_value_ST);
  // Left Femoral B
  Arteries[48] = new Tube(49, 44.250,0.314,0.200,nullptr,nullptr,nullptr,nullptr,0,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0, alpha_value_ST, beta_value_ST);
  // Left Deep femoral
  Arteries[47] = new Tube(48, 11.250,0.200,0.200,nullptr,nullptr,nullptr,nullptr,rm4,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0, alpha_value_ST, beta_value_ST);
  // Left Internal iliac
  Arteries[46] = new Tube(47, 4.500,0.200,0.200,nullptr,nullptr,nullptr,nullptr,rm4,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0, alpha_value_ST, beta_value_ST);
  // Left Femoral A
  Arteries[45] = new Tube(46, 14.500,0.370,0.314,nullptr,nullptr,nullptr,nullptr,0,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0, alpha_value_ST, beta_value_ST);
  // Left External iliac
  Arteries[44] = new Tube(45, 5.750,0.400,0.370,nullptr,nullptr,nullptr,nullptr,0,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0, alpha_value_ST, beta_value_ST);
  // Right Posterior tibal
  Arteries[43] = new Tube(44, 32.000,0.125,0.125,nullptr,nullptr,nullptr,nullptr,rm4,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0, alpha_value_ST, beta_value_ST);
  // Right Anterior Tibal
  Arteries[42] = new Tube(43, 32.000,0.125,0.125,nullptr,nullptr,nullptr,nullptr,rm4,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0, alpha_value_ST, beta_value_ST);
  // Right Femoral B
  Arteries[41] = new Tube(42, 44.250,0.314,0.200,nullptr,nullptr,nullptr,nullptr,0,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0, alpha_value_ST, beta_value_ST);
  // Right Deep femoral
  Arteries[40] = new Tube(41, 11.250,0.200,0.200,nullptr,nullptr,nullptr,nullptr,rm4,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0, alpha_value_ST, beta_value_ST);
  // Right Internal iliac
  Arteries[39] = new Tube(40, 4.500,0.200,0.200,nullptr,nullptr,nullptr,nullptr,rm4,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0, alpha_value_ST, beta_value_ST);
  // Right Femoral A
  Arteries[38] = new Tube(39, 14.500,0.370,0.314,nullptr,nullptr,nullptr,nullptr,0,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0, alpha_value_ST, beta_value_ST);
  // Right External iliac
  Arteries[37] = new Tube(38, 5.750,0.400,0.370,nullptr,nullptr,nullptr,nullptr,0,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0, alpha_value_ST, beta_value_ST);
  // Abdominal aorta E
  Arteries[36] = new Tube(37, 8.000,0.627,0.550,nullptr,nullptr,nullptr,nullptr,0,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0, alpha_value_ST, beta_value_ST);
  // Inferior mesenteric
  Arteries[35] = new Tube(36, 3.750,0.200,0.175,nullptr,nullptr,nullptr,nullptr,rm4,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0, alpha_value_ST, beta_value_ST);
  // Abdominal aorta D
  Arteries[34] = new Tube(35, 12.500,0.792,0.627,nullptr,nullptr,nullptr,nullptr,0,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0, alpha_value_ST, beta_value_ST);
  // Left Renal
  Arteries[33] = new Tube(34, 3.000,0.275,0.275,nullptr,nullptr,nullptr,nullptr,rm4,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0, alpha_value_ST, beta_value_ST);
  // Abdominal aorta C
  Arteries[32] = new Tube(33, 1.500,0.814,0.792,nullptr,nullptr,nullptr,nullptr,0,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0, alpha_value_ST, beta_value_ST);
  // Right Renal
  Arteries[31] = new Tube(32, 3.000,0.275,0.275,nullptr,nullptr,nullptr,nullptr,rm4,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0, alpha_value_ST, beta_value_ST);
  // Abdominal aorta B
  Arteries[30] = new Tube(31, 1.500,0.838,0.814,nullptr,nullptr,nullptr,nullptr,0,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0, alpha_value_ST, beta_value_ST);
  // Superior mesenteric
  Arteries[29] = new Tube(30, 5.000,0.400,0.350,nullptr,nullptr,nullptr,nullptr,rm4,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0, alpha_value_ST, beta_value_ST);
  // Abdominal aorta A
  Arteries[28] = new Tube(29, 5.250,0.924,0.838,nullptr,nullptr,nullptr,nullptr,0,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0, alpha_value_ST, beta_value_ST);
  // Splenic
  Arteries[27] = new Tube(28, 5.500,0.200,0.200,nullptr,nullptr,nullptr,nullptr,rm4,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0, alpha_value_ST, beta_value_ST);
  // Gastric
  Arteries[26] = new Tube(27, 5.750,0.175,0.150,nullptr,nullptr,nullptr,nullptr,rm4,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0, alpha_value_ST, beta_value_ST);
  // Hepatic B
  Arteries[25] = new Tube(26, 6.500,0.275,0.250,nullptr,nullptr,nullptr,nullptr,rm4,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0, alpha_value_ST, beta_value_ST);
  // Hepatic A
  Arteries[24] = new Tube(25, 2.000,0.300,0.250,nullptr,nullptr,nullptr,nullptr,0,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0, alpha_value_ST, beta_value_ST);
  // Celiac axis
  Arteries[23] = new Tube(24, 2.000,0.350,0.300,nullptr,nullptr,nullptr,nullptr,0,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0, alpha_value_ST, beta_value_ST);
  // Left Interosseus
  Arteries[22] = new Tube(23, 7.000,0.100,0.100,nullptr,nullptr,nullptr,nullptr,rm4,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0, alpha_value_ST, beta_value_ST);
  // Left Ulner B
  Arteries[21] = new Tube(22, 22.250,0.175,0.175,nullptr,nullptr,nullptr,nullptr,rm4,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0, alpha_value_ST, beta_value_ST);
  // Left Ulner A
  Arteries[20] = new Tube(21, 17.000,0.200,0.200,nullptr,nullptr,nullptr,nullptr,0,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0, alpha_value_ST, beta_value_ST);
  // Right & Left Radial
  Arteries[19] = new Tube(20, 22.000,0.175,0.175,nullptr,nullptr,nullptr,nullptr,rm4,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0, alpha_value_ST, beta_value_ST);
  // Left Brachial
  Arteries[18] = new Tube(19, 39.750,0.407,0.250,nullptr,nullptr,nullptr,nullptr,0,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0, alpha_value_ST, beta_value_ST);
  // Left Vertebral
  Arteries[17] = new Tube(18, 13.500,0.200,0.200,nullptr,nullptr,nullptr,nullptr,rm4,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0, alpha_value_ST, beta_value_ST);
  // Left Subclavian
  Arteries[16] = new Tube(17, 3.500,0.425,0.407,nullptr,nullptr,nullptr,nullptr,0,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0, alpha_value_ST, beta_value_ST);
  // Intercostals
  Arteries[15] = new Tube(16, 7.250,0.630,0.500,nullptr,nullptr,nullptr,nullptr,rm4,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0, alpha_value_ST, beta_value_ST);
  // Thoracic aorta B
  Arteries[14] = new Tube(15, 10.500,1.124,0.924,nullptr,nullptr,nullptr,nullptr,0,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0, alpha_value_ST, beta_value_ST);
  // Thoracic aorta A
  Arteries[13] = new Tube(14, 5.500,1.246,1.124,nullptr,nullptr,nullptr,nullptr,0,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0, alpha_value_ST, beta_value_ST);
  // Left common Carotid
  Arteries[12] = new Tube(13, 19.250,0.525,0.400,nullptr,nullptr,nullptr,nullptr,0,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0, alpha_value_ST, beta_value_ST);
  // Aortic arch B
  Arteries[11] = new Tube(12, 4.000,1.342,1.246,nullptr,nullptr,nullptr,nullptr,0,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0, alpha_value_ST, beta_value_ST);
  // Right Interosseus
  Arteries[10] = new Tube(11, 7.000,0.100,0.100,nullptr,nullptr,nullptr,nullptr,rm4,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0, alpha_value_ST, beta_value_ST);
  // Right Ulner B
  Arteries[9] = new Tube(10, 22.250,0.175,0.175,nullptr,nullptr,nullptr,nullptr,rm4,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0, alpha_value_ST, beta_value_ST);
  // Right Ulner A
  Arteries[8] = new Tube(9, 17.000,0.200,0.200,nullptr,nullptr,nullptr,nullptr,0,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0, alpha_value_ST, beta_value_ST);
  // Right Radial
  Arteries[7] = new Tube(8, 22.000,0.175,0.175,nullptr,nullptr,nullptr,nullptr,rm4,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0, alpha_value_ST, beta_value_ST);
  // Right Brachial
  Arteries[6] = new Tube(7, 39.750,0.407,0.250,nullptr,nullptr,nullptr,nullptr,0,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0, alpha_value_ST, beta_value_ST);
  // Right Vertebral
  Arteries[5] = new Tube(6, 13.500,0.200,0.200,nullptr,nullptr,nullptr,nullptr,rm4,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0, alpha_value_ST, beta_value_ST);
  // Right common Carotid
  Arteries[4] = new Tube(5, 16.750,0.525,0.400,nullptr,nullptr,nullptr,nullptr,0,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0, alpha_value_ST, beta_value_ST);
  // Right Subclavian
  Arteries[3] = new Tube(4, 3.500,0.425,0.407,nullptr,nullptr,nullptr,nullptr,0,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0, alpha_value_ST, beta_value_ST);
  // Brachiocephalic
  Arteries[2] = new Tube(3, 3.500,0.950,0.700,nullptr,nullptr,nullptr,nullptr,0,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0, alpha_value_ST, beta_value_ST);
  // Aortic arch A
  Arteries[1] = new Tube(2, 3.000,1.420,1.342,nullptr,nullptr,nullptr,nullptr,0,point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0, alpha_value_ST, beta_value_ST);
  // Ascending aorta
  Arteries[0] = new Tube(1, 4.000,1.525,1.420,nullptr,nullptr,nullptr,nullptr,0,point,1,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0, alpha_value_ST, beta_value_ST);

  fprintf (stdout,"finish construction\n");


  // Then point to the bif and merge arteries.

  Arteries[0]->LD = Arteries[1];	Arteries[0]->RD = Arteries[2];	// 1->2,3
  Arteries[1]->LD = Arteries[11];	Arteries[1]->RD = Arteries[12];	// 2->12,13
  Arteries[2]->LD = Arteries[3];	Arteries[2]->RD = Arteries[4];	// 3->4,5
  Arteries[3]->LD = Arteries[5];	Arteries[3]->RD = Arteries[6];	// 4->6,7
  Arteries[4]->LD = Arteries[51];	Arteries[4]->RD = Arteries[52];	// 5->52,53
  Arteries[6]->LD = Arteries[7];	Arteries[6]->RD = Arteries[8];	// 7->8,9
  Arteries[8]->LD = Arteries[9];	Arteries[8]->RD = Arteries[10];	// 9->10,11
  Arteries[11]->LD = Arteries[13];	Arteries[11]->RD = Arteries[16];	// 12->14,17
  Arteries[12]->LD = Arteries[53];	Arteries[12]->RD = Arteries[54];	// 13->54,55
  Arteries[13]->LD = Arteries[14];	Arteries[13]->RD = Arteries[15];	// 14->15,16
  Arteries[14]->LD = Arteries[23];	Arteries[14]->RD = Arteries[28];	// 15->24,29
  Arteries[16]->LD = Arteries[17];	Arteries[16]->RD = Arteries[18];	// 17->18,19
  Arteries[18]->LD = Arteries[19];	Arteries[18]->RD = Arteries[20];	// 19->20,21
  Arteries[20]->LD = Arteries[21];	Arteries[20]->RD = Arteries[22];	// 21->22,23
  Arteries[23]->LD = Arteries[24];	Arteries[23]->RD = Arteries[25];	// 24->25,26
  Arteries[24]->LD = Arteries[26];	Arteries[24]->RD = Arteries[27];	// 25->27,28
  Arteries[28]->LD = Arteries[29];	Arteries[28]->RD = Arteries[30];	// 29->30,31
  Arteries[30]->LD = Arteries[31];	Arteries[30]->RD = Arteries[32];	// 31->32,33
  Arteries[32]->LD = Arteries[33];	Arteries[32]->RD = Arteries[34];	// 33->34,35
  Arteries[34]->LD = Arteries[35];	Arteries[34]->RD = Arteries[36];	// 35->36,37
  Arteries[36]->LD = Arteries[37];	Arteries[36]->RD = Arteries[44];	// 37->38,45
  Arteries[37]->LD = Arteries[38];	Arteries[37]->RD = Arteries[39];	// 38->39,40
  Arteries[38]->LD = Arteries[40];	Arteries[38]->RD = Arteries[41];	// 39->41,42
  Arteries[41]->LD = Arteries[42];	Arteries[41]->RD = Arteries[43];	// 42->43,44
  Arteries[44]->LD = Arteries[45];	Arteries[44]->RD = Arteries[46];	// 45->46,47
  Arteries[45]->LD = Arteries[47];	Arteries[45]->RD = Arteries[48];	// 46->48,49
  Arteries[48]->LD = Arteries[49];	Arteries[48]->RD = Arteries[50];	// 49->50,51


  // Finally initiate the small structure tree.

  Arteries[5]->initSmallTree();
  Arteries[7]->initSmallTree();
  Arteries[9]->initSmallTree();
  Arteries[10]->initSmallTree();
  Arteries[15]->initSmallTree();
  Arteries[17]->initSmallTree();
  Arteries[19]->initSmallTree();
  Arteries[21]->initSmallTree();
  Arteries[22]->initSmallTree();
  Arteries[25]->initSmallTree();
  Arteries[26]->initSmallTree();
  Arteries[27]->initSmallTree();
  Arteries[29]->initSmallTree();
  Arteries[31]->initSmallTree();
  Arteries[33]->initSmallTree();
  Arteries[35]->initSmallTree();
  Arteries[39]->initSmallTree();
  Arteries[40]->initSmallTree();
  Arteries[42]->initSmallTree();
  Arteries[43]->initSmallTree();
  Arteries[46]->initSmallTree();
  Arteries[47]->initSmallTree();
  Arteries[49]->initSmallTree();
  Arteries[50]->initSmallTree();
  Arteries[51]->initSmallTree();
  Arteries[52]->initSmallTree();
  Arteries[53]->initSmallTree();
  Arteries[54]->initSmallTree();


  // In the next three statements the simulations are performed until
  // tstart = tend. That is this is without making any output during this
  // first period of time. If one needs output during this period, these three
  // lines should be commented out, and the entire simulation done within the
  // forthcomming while-loop.

  // Solves the equations until time equals tend.
  solver (Arteries, tstart, tend, k, ID_Out, ID_Bif, ID_Merge);
  tstart = tend;
  tend = tend + Deltat;


  fprintf (stdout,"plots start\n");


  // The loop is continued until the final time
  // is reached. If one wants to make a plot of
  // the solution versus x, tend is set to final-
  // time in the above declaration.
  while (tend <= finaltime)
  {
    // Solves the equations until time equals tend.
    solver (Arteries, tstart, tend, k, ID_Out, ID_Bif, ID_Merge);
    fprintf (stdout,".");




    // call output member function:
    for(int i=0;i<nbrves;++i)
    {
      Arteries[i]->printPxt(fp[i], tend, 0);
      Arteries[i]->printQxt(fq[i], tend, 0);
      Arteries[i]->printAxt(fa[i], tend, 0);
      // Arteries[i]->printNOxt(fNO[i], tend, 0);
      // Arteries[i]->printCAxt(fCA[i], tend, 0);
      // Arteries[i]->printcGMPxt(fcGMP[i], tend, 0);
      // Arteries[i]->printWSSxt(fWSS[i], tend, 0);
    }
    // Arteries[0]->printAxt(fA1, tend, 0);
    // Arteries[0]->printCxt(fC1, tend, 0);


    // The time within each print is increased.
    tstart = tend;
    tend   = tend + Deltat; // The current ending time is increased by Deltat.
  }
  fprintf(stdout,"\n");

  // Solves the NO equations until time equals tend.
  solver_NO (Arteries, NOtstart, NOtend, k);
  NOtstart = NOtend;
  NOtend = NOtend + Deltat;


  fprintf (stdout,"plots NO start\n");
  // The loop is continued until the final time
  // is reached. If one wants to make a plot of
  // the solution versus x, tend is set to final-
  // time in the above declaration.
  while (NOtend <= NOfinaltime)
  {
    // Solves the equations until time equals tend.
    solver_NO (Arteries, NOtstart, NOtend, k);
    fprintf (stdout,".");

    // call output member function:
    for(int i=0;i<nbrves;++i)
    {
      Arteries[i]->printNOxt(fNO[i], NOtend, 0);
      Arteries[i]->printCAxt(fCA[i], NOtend, 0);
      Arteries[i]->printcGMPxt(fcGMP[i], NOtend, 0);
    }


    // The time within each print is increased.
    NOtstart = NOtend;
    NOtend   = NOtend + Deltat; // The current ending time is increased by Deltat.
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
    string SuccessInfo3 = "Close "+to_string(j)+"a OK \n";
    string FailInfo3 = "Close "+to_string(j)+"a NOT OK \n";
    if (fclose(fa[i]) != EOF) fprintf(stdout, "%s", (SuccessInfo3).c_str()); else error("main.C", (FailInfo3).c_str());
        string SuccessInfo4 = "Close "+to_string(j)+"NO OK \n";
    string FailInfo4 = "Close "+to_string(j)+"NO NOT OK \n";
    if (fclose(fNO[i]) != EOF) fprintf(stdout, "%s", (SuccessInfo4).c_str()); else error("main.C", (FailInfo4).c_str());
        string SuccessInfo5 = "Close "+to_string(j)+"CA OK \n";
    string FailInfo5 = "Close "+to_string(j)+"CA NOT OK \n";
    if (fclose(fCA[i]) != EOF) fprintf(stdout, "%s", (SuccessInfo5).c_str()); else error("main.C", (FailInfo5).c_str());
        string SuccessInfo6 = "Close "+to_string(j)+"cGMP OK \n";
    string FailInfo6 = "Close "+to_string(j)+"cGMP NOT OK \n";
    if (fclose(fcGMP[i]) != EOF) fprintf(stdout, "%s", (SuccessInfo6).c_str()); else error("main.C", (FailInfo6).c_str());
  }
  // if (fclose(fA1) != EOF) fprintf(stdout, "Close 1A OK\n"); else error("main.C", "Close 1A NOT OK");
  // if (fclose(fC1) != EOF) fprintf(stdout, "Close 1C OK\n"); else error("main.C", "Close 1C NOT OK");
  return 0;
}
