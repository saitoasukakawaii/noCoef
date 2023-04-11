import pandas as pd


def name(i: int, string: str) -> str:
    return "  const char *name{1}{0} = \"./result/{1}{0}\";\n".format(
        i + 1, string)


def openfile(i: int, string: str) -> str:
    return "  FILE *f{1}{0} = fopen(name{1}{0}, \"w\");\n  if (f{1}{0}) fprintf(stdout, \"File {0}{1} OK " \
           "\\n\"); else error(\"main.C\", \"File {0}{1} NOT OK\");\n".format(
        i + 1, string)


def allocateArteryPoint(i:int,TL:int, TR:int,PL:int, PR:int) -> str:
    strings = ''
    if TL != TR:
        strings = strings+'  Arteries[{0}]->LD = Arteries[{1}]; Arteries[{0}]->RD = Arteries[{2}];\n'.format(i,TL-1,TR-1)
    if (PL!=0 and PR!=0) and (PL!=PR):
        strings = strings+'  Arteries[{0}]->LP = Arteries[{1}]; Arteries[{0}]->RP = Arteries[{2}];\n'.format(i,PL-1,PR-1)
        
    return strings

def initSmallTree(i:int,TL:int,TR:int) -> str:
    if TL == 0 and TR == 0:
        return '  Arteries[{0}]->initSmallTree();\n'.format(i)
    else:
        return ''

def constructure_artery(i:int, L:float, rt:float, rb:float, TL:int, TR:int) -> str:
    if TL == 0 and TR == 0:
        # block_l = "0"
        # block_r = "0"
        rm = "rm4"
    else:
        # block_l = "Arteries[{}]".format(TL-1)
        # block_r = "Arteries[{}]".format(TR-1)
        rm = "0"
    if i == 0:
        return '  Arteries[{0}] = new Tube({1:.3f},{2:.3f},{3:.3f},nullptr,nullptr,nullptr,nullptr,{4},point,1,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0);\n'.format(i,L,rt,rb,rm)
    else:
        return '  Arteries[{0}] = new Tube({1:.3f},{2:.3f},{3:.3f},nullptr,nullptr,nullptr,nullptr,{4},point,0,0,0,0,0,ff1,ff2,ff3,fa1,fa2,fa3, 0.0);\n'.format(i,L,rt,rb,rm)

def output(i: int, string: str) -> str:
    return "    Arteries[{2}]->print{3}xt(f{1}{0}, tend, 0);\n".format(
        i + 1, string, i, string.upper())


def closefile(i: int, string: str) -> str:
    return '  if (fclose(f{1}{0}) != EOF) fprintf(stdout, "Close {0}{1} OK\\n"); else error("main.C", "Close {0}{1} NOT ' \
           'OK");\n'.format(i + 1, string)


filename = "../Geometry/jcj71.xlsx"
# read the geometry data
df1 = pd.read_excel(filename, sheet_name="Sheet1")
# read the topology
df2 = pd.read_excel(filename, sheet_name="Sheet2")

n: int = len(df1)        	# 血管数量
topology_name = "../topology"+str(n)+".txt"
n_period: int = 9  		# 从第几个周期开始画图
point: int = 10     		# 每1cm划分点个数，初始为8192划分4，16384试试8
wave_flag: bool = True 	# Whether implement wave analysis
df2.to_csv(topology_name, sep='\t',header=False,index=False)
with open("generateCode.txt", "w") as f:
    f.write('#include "sor07_new.h"\n')
    f.write('#include "tools.h"\n')
    f.write('#include "arteries.h"\n\n')
    f.write('extern "C"  void impedance_init_driver_(int *tmstps);\n\n')
    f.write("#include <cstdio>\n#include <cstdlib>\n#include <cmath>\n#include <ctime>\n#include <cstring>\n#include "
            "<string>\n#include <vector>\n#include <set>\n\nusing namespace std;\n\n")
    f.write("int main(int argc, char *argv[])\n{\n  double tstart, tend, finaltime;\n\n  // normal range of rmin of "
            "small vessel is 0.01~0.04\n  double rm4 = 0.03;\n")
    f.write('  int point = {};\n'.format(point))
    f.write("  nbrves     = {};             // The number of vessels in the network.\n\n".format(n))

    f.write('  string FileName = "{}";\n'.format(topology_name))
    f.write("  set<int> ID_Out, ID_Bif, ID_Merge;\n")
    f.write("  get_ID(FileName, ID_Out, ID_Bif, ID_Merge);\n\n")

    f.write("  // pressure and flow file point name:\n")
    f.write('  vector<string> nameP(nbrves, "./result/p");\n')
    f.write('  vector<string> nameQ(nbrves, "./result/q");\n')
    f.write('  FILE *fp[nbrves];\n  FILE *fq[nbrves];\n')
  
    f.write("  for(int i=0;i<nbrves;i++)\n  {\n    auto j=i+1;\n    nameP[i] += to_string(j);\n    nameQ[i] += to_string(j);\n  }\n")

    if wave_flag:
        f.write(name(0, "A"))
        f.write(name(0, "C"))


    f.write("\n\n  // open pressure and flow file and declare the point:\n")
    f.write('  for(int i=0;i<nbrves;i++)\n  {\n    auto j=i+1;\n    fp[i] = fopen((nameP[i]).c_str(), "w");\n    string SuccessInfo1 = "File "+to_string(j)+"p OK \\n";\n    string FailInfo1 = "File "+to_string(j)+"p NOT OK \\n";\n    if (fp[i]) fprintf(stdout, "%s", (SuccessInfo1).c_str()); else error("main.C", (FailInfo1).c_str());\n    fq[i] = fopen((nameQ[i]).c_str(), "w");\n    string SuccessInfo2 = "File "+to_string(j)+"q OK \\n";\n    string FailInfo2 = "File "+to_string(j)+"q NOT OK \\n";\n    if (fq[i]) fprintf(stdout, "%s", (SuccessInfo2).c_str()); else error("main.C", (FailInfo2).c_str());\n  }\n')
    if wave_flag:
        f.write(openfile(0, "A"))
        f.write(openfile(0, "C"))

    f.write('\n  // Workspace used by bound_bif\n  for(int i=0; i<18; i++) fjac[i] = new double[18];\n\n')
    f.write('  clock_t c1 = clock();        // Only used when timing the program.\n  tstart     = 0.0;            // Starting time.\n  finaltime  = {0}*Period;       // Final end-time during a simulation.\n  tend       = {1}*Period;       // Timestep before the first plot-point\n                               // is reached.\n\n'.format(n_period + 1, n_period))
    f.write('  // The number of vessels in the network is given when the governing array of\n  // vessels is '
            'declared.\n\n  impedance_init_driver_(&tmstps);\n\n  Tube   *Arteries[nbrves];                    // '
            'Array of blood vessels.\n\n  // Geomety of arteries\n')

    for i in range(n):
        f.write('  // '+df1['Artery'][i]+'\n')
        f.write(constructure_artery(i,df1['L(cm)'][i],df1['r0(cm)'][i],df1['r1(cm)'][i],df2['l'][i],df2['r'][i]))

    f.write('\n  fprintf (stdout,"finish construction\\n");\n\n\n')
    f.write('  // Then point to the bif and merge arteries.\n\n')
    for i in range(n):
        f.write(allocateArteryPoint(i,df2['l'][i],df2['r'][i],df2['pl'][i],df2['pr'][i]))
    f.write('\n\n')
    f.write('  // Finally initiate the small structure tree.\n\n')
    for i in range(n):
        f.write(initSmallTree(i,df2['l'][i],df2['r'][i]))
    f.write('\n\n')
    f.write('  // In the next three statements the simulations are performed until\n  // tstart = tend. That is this is without making any output during this\n  // first period of time. If one needs output during this period, these three\n  // lines should be commented out, and the entire simulation done within the\n  // forthcomming while-loop.')
    f.write('\n\n  // Solves the equations until time equals tend.\n  solver (Arteries, tstart, tend, k, ID_Out, ID_Bif, ID_Merge);\n  tstart = tend;\n  tend = tend + Deltat;\n\n')
    f.write('\n  fprintf (stdout,"plots start\\n");\n\n')
    f.write('\n  // The loop is continued until the final time\n  // is reached. If one wants to make a plot of\n  // the solution versus x, tend is set to final-\n  // time in the above declaration.\n')
    f.write('  while (tend <= finaltime)\n  {\n    for (int j=0; j<nbrves; j++)\n    {\n      int ArtjN = Arteries[j]->N;     // The number of grid points along the vessel\n      for (int i=0; i<ArtjN; i++)\n      {\n        Arteries[j]->Qprv[i+1] = Arteries[j]->Qnew[i+1];\n        Arteries[j]->Aprv[i+1] = Arteries[j]->Anew[i+1];\n      }\n    }\n\n\n')
    f.write('    // Solves the equations until time equals tend.\n    solver (Arteries, tstart, tend, k, ID_Out, ID_Bif, ID_Merge);\n    fprintf (stdout,".");\n\n\n')

    f.write("\n\n    // call output member function:\n")
    f.write("    for(int i=0;i<nbrves;++i)\n    {\n      Arteries[i]->printPxt(fp[i], tend, 0);\n      Arteries[i]->printQxt(fq[i], tend, 0);\n    }\n")
    if wave_flag:
    	f.write(output(0, "A"))
    	f.write(output(0, "C"))


    f.write('\n\n    // The time within each print is increased.\n    tstart = tend;\n    tend   = tend + Deltat; // The current ending time is increased by Deltat.\n  }\n  fprintf(stdout,"\\n");\n\n\n')
    f.write('  // The following statements is used when timing the simulation.\n  fprintf(stdout,"nbrves = %d, Lax, ", nbrves);\n  clock_t c2 = clock(); // FIXME clock() may wrap after about 72 min.\n  int tsec = (int) ((double) (c2-c1)/CLOCKS_PER_SEC + 0.5);\n  fprintf(stdout,"cpu-time %d:%02d\\n", tsec / 60, tsec % 60);\n  fprintf(stdout,"\\n");\n\n  // In order to termate the program correctly the vessel network and hence\n  // all the vessels and their workspace are deleted.\n  for (int i=0; i<nbrves; i++) delete Arteries[i];\n\n  // Matrices and arrays are deleted\n  for (int i=0; i<18; i++) delete[] fjac[i];\n\n')

    f.write("\n\n  // close the files:\n")
    f.write('  for(int i=0;i<nbrves;++i)\n  {\n    auto j=i+1;\n    string SuccessInfo1 = "Close "+to_string(j)+"p OK \\n";\n    string FailInfo1 = "Close "+to_string(j)+"p NOT OK \\n";\n    if (fclose(fp[i]) != EOF) fprintf(stdout, "%s", (SuccessInfo1).c_str()); else error("main.C", (FailInfo1).c_str());\n    string SuccessInfo2 = "Close "+to_string(j)+"q OK \\n";\n    string FailInfo2 = "Close "+to_string(j)+"q NOT OK \\n";\n    if (fclose(fq[i]) != EOF) fprintf(stdout, "%s", (SuccessInfo2).c_str()); else error("main.C", (FailInfo2).c_str());\n  }\n')
    if wave_flag:
    	f.write(closefile(0, "A"))
    	f.write(closefile(0, "C"))

    f.write("  return 0;\n}\n")
