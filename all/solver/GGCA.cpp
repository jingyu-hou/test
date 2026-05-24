//*******************************************************//
//
//
//   2D Cellular Automaton modeling of Grain Growth
//              Coded by Namin Xiao
//				 nmxiao@imr.ac.cn
//       Institute of Metal Research, CAS
//
//
//*******************************************************//
#include <iostream>
#include <fstream>
#include <time.h>
#include <math.h>

using namespace std;

const double COEF0 = 9.0e-11;//0.5e-12;
const double COEF1 = 3.0e-10;//4.5e-10;
const double PI = 3.14159;
const double THETA0 = 70.0 / 180.0 * PI;

int main(int argc, char *argv[]) {
	int i, j, k, l, m;
	int ii, jj, kk;
	int tStep, Ls;
	int Cx, Cy;
	int C_I, C_Nei;
	int E_I, N_I, W_I, S_I, EN_I, NW_I, WS_I, SE_I;
	int Num;
	int Index;
	double Frac;
	double m_Radius;
	double dt, dx, dy;
	double Time;
	double Curvature;

	double Ve, Vn, Vw, Vs;
	double Coef;
	double MisOri;

	char * outputTXT, * outputBIN;
	int * pGrain;
	int * pGrainN;
	int * NeiOriCounter;
	int * NeiOri;
	double * pFraction;
	
	int GrainN;
	int m_Max, m_Maxi;
	double Raver;
	int Gi;

	Cx = 200;
  	Cy = 200;
	tStep = 10000;
	Ls = 500;
	dt = 0.01;
	dx = 0.000004;
	dy = dx;
	GrainN = 40000;

	outputTXT = new char[32];
	outputBIN = new char[32];
	pGrain = new int[Cy * Cx + 1];
	pFraction = new double[Cy * Cx + 1];
	NeiOriCounter = new int[9];
	NeiOri = new int[9];
	pGrainN = new int[GrainN+1];
	
	ifstream IFile1("Grain-0.txt", ios::in);
	ofstream OFile2("Grain-0.raw", ios::out|ios::trunc|ios::binary);
	ofstream OFile4("IndividualGrainRadius.txt", ios::out);
	ofstream OFile5("AverageGrainRadius.txt", ios::out);

	for (j = 1; j <= Cy; j++) {
		for (i = 1; i <= Cx; i++) {
			IFile1 >> pGrain[(j - 1) * Cx + i];
			OFile2.write((char *)(&(pGrain[(j - 1) * Cx + i])), sizeof(int)); 
			pFraction[(j - 1) * Cx + i] = 1.0;
		}
	}
	IFile1.close();
	OFile2.close();

	for (jj = 1; jj <= GrainN; jj++) {
		pGrainN[jj] = 0;
	}
	for (j = 1; j <= Cy; j++) {
		for (i = 1; i <= Cx; i++) {
			C_I = (j - 1) * Cx + i;
			for (jj = 1; jj <= GrainN; jj++) {
				if (pGrain[C_I] == jj) {
					pGrainN[jj]++;
					continue;
				}
			}
		}
	}
	
	for (ii = 1; ii <= tStep; ii++) {
    	Time=ii * dt;

		for (j = 1; j <= Cy; j++) {
			for (i = 1; i <= Cx; i++) {
        		C_I = (j - 1) * Cx + i;
				if ((i + 1) <= Cx) {
					E_I = C_I + 1;
				} else {
					E_I = 0;
				}
				if ((j - 1) > 0)
				{
					N_I = C_I - Cx;
				} else {
					N_I = 0;
				}
				if ((i - 1) > 0) {
					W_I = C_I - 1;
				} else {
					W_I = 0;
				}
				if ((j + 1) <= Cy) {
					S_I = C_I + Cx;
				} else {
					S_I = 0;
				}
//				if ((i + 1) <= Cx && (j - 1) > 0) {
//					EN_I = C_I - Cx + 1;
//				} else {
//					EN_I = 0;
//				}
//				if ((i - 1) > 0 && (j - 1) > 0) {
//					NW_I = C_I - Cx - 1;
//				} else {
//					NW_I = 0;
//				}
//				if ((i - 1) > 0 && (j + 1) <= Cy) {
//					WS_I = C_I + Cx - 1;
//				} else {
//					WS_I = 0;
//				}
//				if ((i + 1) <= Cx && (j + 1) <= Cy) {
//					SE_I = C_I + Cx + 1;
//				} else {
//					SE_I = 0;
//				}
				m = 0;
				if (E_I > 0) {
					if (pGrain[C_I] != pGrain[E_I]) m++;
				}
				if (N_I > 0) {
					if (pGrain[C_I] != pGrain[N_I]) m++;
				}
				if (W_I > 0) {
					if (pGrain[C_I] != pGrain[W_I]) m++;
				}
				if (S_I > 0) {
					if (pGrain[C_I] != pGrain[S_I]) m++;
				}
//				if (EN_I > 0)
//				{
//					if (pGrain[C_I] != pGrain[EN_I]) m++;
//				}
//				if (NW_I > 0)
//				{
//					if (pGrain[C_I] != pGrain[NW_I]) m++;
//				}
//				if (WS_I > 0)
//				{
//					if (pGrain[C_I] != pGrain[WS_I]) m++;
//				}
//				if (SE_I > 0)
//				{
//					if (pGrain[C_I] != pGrain[SE_I]) m++;
//				}

				if (m > 0) {
					Num = 0;
					Index = 0;
					
					for (l = j - 2; l <= j + 2; l++) {
						for (k = i - 2; k <= i + 2; k++) {
							if (k > 0 && l > 0 && k <= Cx && l<= Cy) {
								C_Nei = (l - 1) * Cx + k;
								Num++;
								
								if (pGrain[C_I] == pGrain[C_Nei]) {
									Index++;
								}
							}						
						}
					}
					
					/*pOriNei1=pOriNei2=pOriNei3=pOriNei[1];
					for(jj=1;jj<=Num;jj++)
					{
					    if(pOriNei[jj]!=pOriNei1)pOriNei2=pOriNei[jj];
					}
					for(jj=1;jj<=Num;jj++)
					{
					    if(pOriNei[jj]!=pOriNei1&&pOriNei[jj]!=pOriNei2)
					    {
					        pOriNei3=pOriNei[jj];
					    }
					}
					
					if(pOriNei1!=pOriNei2&&pOriNei2!=pOriNei3&&pOriNei1!=pOriNei3)
					{
					    m_Ix=i;
					    m_Iy=j;
				    }*/ //Position of triple junction
					
					if (Num == 9) {
						Curvature = 1.28 / dx * (5.0 - Index) / Num;
					} else if (Num == 15) {
						Curvature = 1.28 / dx * (8.0 - Index) / Num;
					} else {
						Curvature = 1.28 / dx * (14.0 - Index) / Num;
					}
					
					if (Curvature > 0.0) {
						Ve = 0.0;
						Vn = 0.0;
						Vw = 0.0;
						Vs = 0.0;
						if (E_I > 0) {
							if (pGrain[C_I] != pGrain[E_I]) {
								MisOri = 2.0 * PI * fabs(double(pGrain[E_I] -pGrain[C_I]))/ double(GrainN - 1);
								if (MisOri >= PI) MisOri = 2.0 * PI - MisOri;
								if (MisOri < THETA0) {
									Coef = COEF0 * MisOri / THETA0 * (1.0 - log(MisOri / THETA0));
								} else {
									Coef = COEF0;
								}
								Ve = Coef * Curvature;
							}
						}
						if (N_I > 0) {
							if (pGrain[C_I] != pGrain[N_I]) {
								MisOri = 2.0 * PI * fabs(double(pGrain[N_I] -pGrain[C_I]))/ double(GrainN - 1);
								if (MisOri >= PI) MisOri = 2.0 * PI - MisOri;
								if (MisOri < THETA0) {
									Coef = COEF0 * MisOri / THETA0 * (1.0 - log(MisOri / THETA0));
								} else {
									Coef = COEF0;
								}
								Vn = Coef * Curvature;
							}	
						}
						if (W_I > 0) {
							if (pGrain[C_I] != pGrain[W_I]) {
								MisOri = 2.0 * PI * fabs(double(pGrain[W_I] -pGrain[C_I]))/ double(GrainN - 1);
								if (MisOri >= PI) MisOri = 2.0 * PI - MisOri;
								if (MisOri < THETA0) {
									Coef = COEF0 * MisOri / THETA0 * (1.0 - log(MisOri / THETA0));
								} else {
									Coef = COEF0;
								}
								Vw = Coef * Curvature;
							}
						}
						if (S_I > 0) {
							if (pGrain[C_I] != pGrain[S_I]) {
								MisOri = 2.0 * PI * fabs(double(pGrain[S_I] -pGrain[C_I]))/ double(GrainN - 1);
								if (MisOri >= PI) MisOri = 2.0 * PI - MisOri;
								if (MisOri < THETA0) {
									Coef = COEF0 * MisOri / THETA0 * (1.0 - log(MisOri / THETA0));
								} else {
									Coef = COEF0;
								}
								Vs = Coef * Curvature;
							}
						}
						pFraction[C_I] = pFraction[C_I] - Ve * dt / dx - Vn * dt / dy - Vw * dt / dx - Vs * dt / dy
							             + Ve * Vn * pow(dt, 2.0) / (dx * dy)
							             + Vn * Vw * pow(dt, 2.0) / (dx * dy)
							             + Vw * Vs * pow(dt, 2.0) / (dx * dy)
							             + Vs * Ve * pow(dt, 2.0) / (dx * dy);
					}
				}
			}
		}

		for (j = 1; j <= Cy; j++) {
			for (i = 1; i<= Cx; i++) {
        		C_I = (j - 1) * Cx + i;
				if ((i + 1) <= Cx) {
					E_I = C_I + 1;
				} else {
					E_I = 0;
				}
				if ((j - 1) > 0) {
					N_I = C_I - Cx;
				} else {
					N_I = 0;
				}
				if ((i - 1) > 0) {
					W_I = C_I - 1;
				} else {
					W_I = 0;
				}
				if ((j + 1) <= Cy) {
					S_I = C_I + Cx;
				} else {
					S_I = 0;
				}
//				if ((i + 1) <= Cx && (j - 1) > 0) {
//					EN_I = C_I - Cx + 1;
//				} else {
//					EN_I = 0;
//				}
//				if ((i - 1) > 0 && (j - 1) > 0) {
//					NW_I = C_I - Cx - 1;
//				} else {
//					NW_I = 0;
//				}
//				if ((i - 1) > 0 && (j + 1) <= Cy) {
//					WS_I = C_I + Cx - 1;
//				} else {
//					WS_I = 0;
//				}
//				if((i + 1) <= Cx && (j + 1) <= Cy) {
//					SE_I = C_I + Cx + 1;
//				} else {
//					SE_I = 0;
//				}

				if (pFraction[C_I] < 0.01) {
					NeiOri[0] = 0;
					NeiOri[1] = 0;
					NeiOri[2] = 0;
					NeiOri[3] = 0;
//					NeiOri[4] = 0;
//					NeiOri[5] = 0;
//					NeiOri[6] = 0;
//					NeiOri[7] = 0;
					
					NeiOriCounter[0] = 0;
					NeiOriCounter[1] = 0;
					NeiOriCounter[2] = 0;
					NeiOriCounter[3] = 0;
//					NeiOriCounter[4] = 0;
//					NeiOriCounter[5] = 0;
//					NeiOriCounter[6] = 0;
//					NeiOriCounter[7] = 0;

					if (E_I > 0) NeiOri[0] = pGrain[E_I];
					if (N_I > 0) NeiOri[1] = pGrain[N_I];
					if (W_I > 0) NeiOri[2] = pGrain[W_I];
					if (S_I > 0) NeiOri[3] = pGrain[S_I];
//					if (EN_I > 0) NeiOri[4] = pGrain[EN_I];
//					if (NW_I > 0) NeiOri[5] = pGrain[NW_I];
//					if (WS_I > 0) NeiOri[6] = pGrain[WS_I];
//					if (SE_I > 0) NeiOri[7] = pGrain[SE_I];

					for (l = j - 2; l <= j + 2; l++) {
						for(k = i - 2; k <= i + 2; k++) {
							if (k > 0 && l > 0 && k <= Cx && l <= Cy) {
								C_Nei = (l - 1) * Cx + k;						
								if (E_I > 0) {
									if (pGrain[E_I] == pGrain[C_Nei]) NeiOriCounter[0]++;
								}
								if (N_I > 0) {
									if (pGrain[N_I] == pGrain[C_Nei]) NeiOriCounter[1]++;
								}
								if (W_I > 0) {
									if (pGrain[W_I] == pGrain[C_Nei]) NeiOriCounter[2]++;
								}
								if (S_I > 0) {
									if (pGrain[S_I] == pGrain[C_Nei]) NeiOriCounter[3]++;
								}
//								if (EN_I > 0) {
//									if (pGrain[EN_I] == pGrain[C_Nei]) NeiOriCounter[4]++;
//								}
//								if (NW_I > 0) {
//									if (pGrain[NW_I] == pGrain[C_Nei]) NeiOriCounter[5]++;
//								}
//								if (WS_I > 0) {
//									if (pGrain[WS_I] == pGrain[C_Nei]) NeiOriCounter[6]++;
//								}
//								if (SE_I > 0) {
//									if (pGrain[SE_I] == pGrain[C_Nei]) NeiOriCounter[7]++;
//								}
							}						
						}
					}
					if(NeiOri[0] != pGrain[C_I])
					{
						m_Max = NeiOriCounter[0];
					    m_Maxi = 0;
					}
					if(NeiOri[1] != pGrain[C_I])
					{
						m_Max = NeiOriCounter[1];
					    m_Maxi = 1;
					}
					if(NeiOri[2] != pGrain[C_I])
					{
						m_Max = NeiOriCounter[2];
					    m_Maxi = 2;
					}
					if(NeiOri[3] != pGrain[C_I])
					{
						m_Max = NeiOriCounter[3];
					    m_Maxi = 3;
					}
//					if(NeiOri[4] != pGrain[C_I])
//					{
//						m_Max = NeiOriCounter[4];
//					    m_Maxi = 4;
//					}
//					if(NeiOri[5] != pGrain[C_I])
//					{
//						m_Max = NeiOriCounter[5];
//					    m_Maxi = 5;
//					}
//					if(NeiOri[6] != pGrain[C_I])
//					{
//						m_Max = NeiOriCounter[6];
//					    m_Maxi = 6;
//					}
//					if(NeiOri[7] != pGrain[C_I])
//					{
//						m_Max = NeiOriCounter[7];
//					    m_Maxi = 7;
//					}
					for (kk = 0; kk < 4; kk++) {
						if (NeiOriCounter[kk] > m_Max && NeiOri[kk] != pGrain[C_I]) {
							m_Max = NeiOriCounter[kk];
							m_Maxi = kk;
						}
					}
					pGrain[C_I] = NeiOri[m_Maxi];
					pFraction[C_I] = 1.0;
				}
			}
		}
		if (ii % Ls == 0) {
			sprintf(outputTXT, "%s%d%s", "Grain-", ii, ".txt");
			sprintf(outputBIN, "%s%d%s", "Grain-", ii, ".raw");
			ofstream OFile3(outputTXT, ios::out|ios::trunc);
          	ofstream OFile10(outputBIN, ios::out|ios::trunc|ios::binary);
            
			for (jj = 1; jj <= GrainN; jj++) {
				pGrainN[jj] = 0;
			}
			Raver = 0.0;
			for (j = 1; j <= Cy; j++) {
				for (i = 1; i <= Cx; i++) {
					C_I = (j - 1) * Cx + i;
					OFile3 << pGrain[C_I] << "\n";
					OFile10.write((char *)(&(pGrain[C_I])), sizeof(int));  

					for (jj = 1; jj <= GrainN; jj++) {
						if (pGrain[C_I] == jj) {
							pGrainN[jj]++;
							continue;
						}
					}
				}
			}
			OFile3.close();
			OFile10.close();			

			OFile4 << ii << "\n";
			Gi=0;
			for (jj = 1; jj <= GrainN; jj++) {
			  OFile4 << pow((pGrainN[jj] * dx * dy / PI), 0.5) << "\n";
				if (pGrainN[jj] != 0) {
					Raver = Raver + pow((pGrainN[jj] * dx * dy / PI), 0.5);
					Gi++;
				}
			}
			OFile4 << "\n";
			OFile5 << Raver / Gi << "\n";
		}
	}
	OFile4.close();
	OFile5.close();
	
	delete outputTXT, outputBIN;
	delete pGrain;
	delete pFraction;
	delete NeiOri;
	delete NeiOriCounter;
	delete pGrainN;
}
