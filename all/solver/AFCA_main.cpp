//*******************************************************//
//
//
//   Cellular Automaton modeling of Austenite-Ferrite Transformation
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

const double KB = 0.0; //玻耳兹曼常数
const double R = 8.314; //理想气体常数
const double K1 = 0.0; //形核模型常数
const double K2 = 0.0; //形核模型常数
const double LC = 0.0; //unit cell size, m
const double SC = 0.0; //unit cell area, m^2
const double PURE_FE_A3 = 1185.0; //(912 + 273), 纯铁的同素异构转变温度, K
const double EUTECTOID_TEMP = 1000.0; //(727 + 273), 共析线温度, K
const double EUTECTOID_SOLUTE_C = 0.77; //共析点碳浓度, wt%
const double MAX_BCC_SOLUTE_C = 0.0218; //铁素体中最大碳溶解度

int main(int argc, char *argv[]) {
  time_t StartTime, EndTime;

  int seed[1] = 8;
  int Cx = 100, Cy = 100;			//Cell Numbers at X and Y direction
  int GrainNa = 5;				//Number of initial austenite grains
  int GrainNf = 0;        //Number of total ferrite grains
  double Lc = 1.0e-6;					//Unit cell size, m
  int Q = 1000;						//orientation number
  int tStep = 10000;					//total steps
  int iStep = 0;					//current step
  double dt = 0.001;					//calculation time step, s
  double CRate = 0.0;					//cooling rate, K/s
  double Time = 0.0;				//current time, s
  double iTemp = 780.0;			//isothermal or initial temperature, K
  int i = 0, j = 0;
  double wtSi = 0.0;				//initial composition, wt%
  double wtMn = 0.0;
  double wtNi = 0.0;
  double wtCu = 0.0;
  double wtCr = 0.0;
  double wtC = 0.0;

  int *pGrain;				//variable of grain orientation
  int *pPhase;				//variable of order parameter: austenite (gamma) 0; A/F interface 1; ferrite (alpha) 2;
  double *pCarbon;			//variable of carbon composition, wt%
  double *pTemp;				//variable of temperature, K
  double *pFraction;			//variable of new phase (ferrite) fraction

  /* cout<<"This is the 2D Cellular Automaton modeling of austenitization"<<endl;
  cout<<"Please input the modeling parameters:"<<endl;
  cout<<"Number of lines (X direction):";
  cin>>Cx;
  cout<<"Number of rows (Y direction):";
  cin>>Cy;
  cout<<"Number of initial grains:";
  cin>>GrainN;
  cout<<"The size of unit cell (unit: m):";
  cin>>Lc;
  cout<<"The total simulation steps:";
  cin>>Nstep;
  cout<<"The simulation time interval (unit: sec):";
  cin>>dt; */

  StartTime = time (NULL);
  cout << "The job started at " << ctime(&StartTime) << endl;

  pGrain = new int[Cx * Cy + 1];
  pPhase = new int[Cx * Cy + 1];
  pCarbon = new double[Cx * Cy + 1];
  pTemp = new double[Cx * Cy + 1];
  pFraction = new double[Cx * Cy + 1];

# pragma omp parallel for private (i, j)
  for (j = 1; j <= Cy; j++) {
    for (i = 1; i <= Cx; i++) {
      pGrain[(j - 1) * Cx + i] = 0;
      pPhase[(j - 1) * Cx + i] = 0;
      pCarbon[(j - 1) * Cx + i] = wtC;
      pFraction[(j - 1) * Cx + i] = 0.0;
      pTemp[(j - 1) * Cx + i] = iTemp;
    }
  }

  ICondition(Cx, Cy, GrainNa, pGrain);

  ofstream IPFile1("Phase0.txt");
  ofstream IPFile2("Grain0.txt");

  for (j = 1; j <= Cy; j++) {
# pragma omp parallel for private (i) ordered
    for (i = 1; i <= Cx; i++) {
# pragma omp ordered
      {
        IPFile1 << pPhase[(j - 1) * Cx + i] << "\n";
        IPFile2 << pGrain[(j - 1) * Cx + i] << "\n";
      }
    }
  }
  IPFile1.close();
  IPFile2.close();
  cout << "The initialization of modeling finished." << endl;

  for (iStep = 1; iStep <= tStep; iStep++) {
    Time = Time + dt;
    Ae3 = PURE_FE_A3 - wtC * (PURE_FE_A3 - EUTECTOID_TEMP) / EUTECTOID_SOLUTE_C;
    Ar3 = Ae3 - 22.56 * pow(CRate, 0.27);
    Ar3 = 820.0;

    Nucleation(Cx, Cy, seed, Q, pGrain, pPhase, pCarbon, pTemp, Ar3, CRate, dt,
               wtSi, wtMn, wtNi, wtCu, wtCr, Time, GrainNf);

    Growth(Cx, Cy, seed, pGrain, pPhase, pCarbon, pTemp, dt, Time,
           pFraction, wtSi, wtMn, wtNi, wtCu, wtCr);

//    Coarsening(Cell_X, Cell_Y, DeltT, LC, LC,
//               pGrain, pFrac, pPhase, m_Nuclei);

    for (j = 1; j <= Cy; j++) {
      for (i = 1; i <= Cx; i++) {
        C_I = (j - 1) * Cx + i;
        pTemp[C_I] = pTemp[C_I] - CR * dt;
      }
    }

    if((iStep % Ls) == 0.0) {
      Save(iStep, Cx, Cy, pGrain, pPhase, pTemp, pCarbon, pFraction);
      Result(iStep, Cx, Cy, pGrain, pPhase, pTemp, pCarbon, pFraction, Time, GrainNf);
    }
  }

  delete  pGrain;
  delete  pPhase;
  delete  pTemp;
  delete  pCarbon;
  delete  pFraction;

  EndTime = time(NULL);

  cout << endl;
  cout << "The job finished completely at " << ctime(&EndTime) << endl;
  cout << "The total computing time: " << difftime(EndTime, StartTime) << endl;

  return (0);
}

void ICondition(int Cx, int Cy, int GrainNa, int *pGrain) {
  int i, j, k;
  int x, y;
  int I_Nuclei;
  int seed[1] = 16;
  int LoopIndex = 200;    //随机循环次数
  int C_I, E_I, W_I, ES_I, WS_I, WN_I, EN_I;
  int Nei[6] = {0, 0, 0, 0, 0, 0};
  int N;
  int temp;

  //随机选取GrainNa个元胞作为初始奥氏体晶粒取向的种子
  for (k = 1; k <= GrainNa; k++) {
    x = int(Cx * RandomDouble(seed)) + 1;
    y = int(Cy * RandomDouble(seed)) + 1;
    if (pGrain[(y - 1) * Cx + x] == 0) {
      temp = RandomInt(1000);
      pGrain[(y - 1) * Cx + x] = temp;
    }
  }

  //以就近原则给每个元胞赋予对应的晶粒取向
  for (k = 0; k <= LoopIndex; k++) {
    for (j = 1; j <= Cy; j++) {
      for (i = 1; i <= Cx; i++) {
        x = int(Cx * RandomDouble(seed)) + 1;
        y = int(Cy * RandomDouble(seed)) + 1;
        C_I = (y - 1) * Cx + x;

        Neighbor(C_I, Cx, Cy, Nei);
        E_I = Nei[0];
        EN_I = Nei[1];
        WN_I = Nei[2];
        W_I = Nei[3];
        WS_I = Nei[4];
        ES_I = Nei[5];

        if (pGrain[C_I] != 0) {
          if (E_I > 0) {
            if (pGrain[E_I] == 0) {
              pGrain[E_I] = pGrain[C_I];
            }
          }
          if (EN_I > 0) {
            if (pGrain[EN_I] == 0) {
              pGrain[EN_I] = pGrain[C_I];
            }
          }
          if (WN_I > 0) {
            if (pGrain[WN_I] == 0) {
              pGrain[WN_I] = pGrain[C_I];
            }
          }
          if (W_I > 0) {
            if (pGrain[W_I] == 0) {
              pGrain[W_I] = pGrain[C_I];
            }
          }
          if (WS_I > 0) {
            if (pGrain[WS_I] == 0) {
              pGrain[WS_I] = pGrain[C_I];
            }
          }
          if (ES_I > 0) {
            if (pGrain[ES_I] == 0) {
              pGrain[ES_I] = pGrain[C_I];
            }
          }
        }
      }
    }
  }
}

void Nucleation(int Cx, int Cy, int seed[1], int Q, int *pGrain, int *pPhase,
                double *pCarbon, double *pTemp, double Ar3, double CRate,
                double dt, double wtSi, double wtMn, double wtNi, double wtCu,
                double wtCr, double Time, int GrainNf) {
  int i, j, k;
  int C_I, E_I, W_I, ES_I, EN_I, WS_I, WN_I;
  double Ae3 = 0.0;
  double Calpha = 0.0;
  double Cgamma = 0.0;
  double Pnuc = 0.0;
  int Nei[6] = {0, 0, 0, 0, 0, 0};
  double DeltaG = 0.0;

  for (j = 1; j <= Cy; j++) {
    for (i = 1; i <= Cx; i++) {
      C_I = (j - 1) * Cx + i;
      Neighbor(C_I, Cx, Cy, Nei);
      E_I = Nei[1];
      EN_I = Nei[2];
      WN_I = Nei[3];
      W_I = Nei[4];
      WS_I = Nei[5];
      ES_I = Nei[6];

      Ae3 = PURE_FE_A3
            - pCarbon[C_I] * (PURE_FE_A3 - EUTECTOID_TEMP) / EUTECTOID_SOLUTE_C;
      k = 0;
      if ((pPhase[C_I] == 0) && (pTemp[C_I] < Ar3)) {
        if (E_I > 0) {
          if (pPhase[E_I] == 0) {
            if (pGrain[E_I] != pGrain[C_I]) {
              k = k + 1;
            }
          }
        }
        if (EN_I > 0) {
          if (pPhase[EN_I] == 0) {
            if (pGrain[EN_I] != pGrain[C_I]) {
              k = k + 1;
            }
          }
        }
        if (WN_I > 0) {
          if (pPhase[WN_I] == 0) {
            if (pGrain[WN_I] != pGrain[C_I]) {
              k = k + 1;
            }
          }
        }
        if (W_I > 0) {
          if (pPhase[W_I] == 0) {
            if (pGrain[W_I] != pGrain[C_I]) {
              k = k + 1;
            }
          }
        }
        if (WS_I > 0) {
          if (pPhase[WS_I] == 0) {
            if (pGrain[WS_I] != pGrain[C_I]) {
              k = k + 1;
            }
          }
        }
        if (ES_I > 0) {
          if (pPhase[ES_I] == 0) {
            if (pGrain[ES_I] != pGrain[C_I]) {
              k = k + 1;
            }
          }
        }

        if (k > 0) {
          /* steady state nucleation rate */
          Calpha = -MAX_BCC_SOLUTE_C
                   * (pTemp[C_I] - PURE_FE_A3) / (PURE_FE_A3 - EUTECTOID_TEMP);
          DeltaG = NucleationDeltaG(pCarbon[C_I], Calpha, wtSi, wtMn, wtNi,
                                        wtCu, wtCr, pTemp[C_I]);
          DeltaG = DeltaG / 7.18E-6;

          Cgamma = pCarbon[C_I] / 100.0;
          Dgamma = 1.0e-4 * 0.5 * exp(-30.0 * Cgamma) * exp((-3.83e4 - 1.9e5 *
                   Cgamma + 5.5e5 * Cgamma * Cgamma) / R / pTemp[C_I] / 4.12);

          NucRate = K1 * Dgamma * pow((KB * pTemp[C_I]), -0.5)
                    * exp (-1.0 * K2 / pow(DeltaG, 2.0) / KB / pTemp[C_I]);
          /* steady state nucleation rate */

          /* time dependent nucleation rate */
          x_Cgamma = (pCarbon[C_I] / 12.0)
                     / (pCarbon[C_I] / 12.0 + (100.0 - pCarbon[C_I]) / 56.0);
          a_alpha = 2.8863 * (1.0 + 17.5e-6 * 237.15) * 1.0E-10;
          a_gamma = (0.36306 + 7.83e-4 * x_Cgamma)
                    * (1.0 + (24.9 - 0.5 * x_Cgamma) * 37.15 * 1.0e-6) * 1.0E-9;
          a = (a_alpha + a_gamma) / 2.0;
          Valpha = pow(a_alpha, 3.0) / 2.0;
          NucRate = NucRate * exp(-12.0 * KB * pTemp[C_I] * pow(a, 4) * 0.09
                                  / Dgamma / X_Cgamma / pow(Valpha, 2) / pow(DeltaG, 2) / Time);
          /* time dependent nucleation rate */

          Pnuc = NucRate * SC * dt;

          if (RandomDouble(seed) <= Pnuc) {
            GrainNf = GrainNf + 1;
            pPhase[C_I] = 1;
            pGrain[C_I] = GrainNf;
          }
        }
      }
    }
  }
}

double NucleationDeltaG(double Cgamma, double Calpha, double wtSi,
                 double wtMn, double wtNi, double wtCu,
                 double wtCr, double T) {
  double Xa = 0.0;
  double Xf = 0.0;
  double X_Si = 0.0;
  double X_Mn = 0.0;
  double X_Ni = 0.0;
  double X_Cu = 0.0;
  double X_Cr = 0.0;
  double Dufe = 0.0;
  double G = 0.0;
  double Zalpha = 0.0;
  double Zgamma = 0.0;
  double T10 = 0.0;
  double T20 = 0.0;

  Xa = Cgamma / 12.0 / (Cgamma / 12.0 + wtSi / 28.0 + wtMn / 55.0 +
                        wtNi / 59.0 + wtCu / 64.0 + wtCr / 52.0 +
                        (100.0 - Cgamma - wtSi - wtMn - wtNi - wtCu - wtCr) / 56.0);
  Xf = Calpha / 12.0 / (Calpha / 12.0 + wtSi / 28.0 + wtMn / 55.0 +
                        wtNi / 59.0 + wtCu / 64.0 + wtCr / 52.0 +
                        (100.0 - Calpha - wtSi - wtMn - wtNi - wtCu - wtCr) / 56.0);
  X_Si = wtSi / 28.0 / (Cgamma / 12.0 + wtSi / 28.0 + wtMn / 55.0 +
                        wtNi / 59.0 + wtCu / 64.0 + wtCr / 52.0 +
                        (100.0 - Cgamma - wtSi - wtMn - wtNi - wtCu - wtCr) / 56.0);
  X_Mn = wtMn / 55.0 / (Cgamma / 12.0 + wtSi / 28.0 + wtMn / 55.0 +
                        wtNi / 59.0 + wtCu / 64.0 + wtCr / 52.0 +
                        (100.0 - Cgamma - wtSi - wtMn - wtNi - wtCu - wtCr) / 56.0);
  X_Ni = wtNi / 59.0 / (Cgamma / 12.0 + wtSi / 28.0 + wtMn / 55.0 +
                        wtNi / 59.0 + wtCu / 64.0 + wtCr / 52.0 +
                        (100.0 - Cgamma - wtSi - wtMn - wtNi - wtCu - wtCr) / 56.0);
  X_Cu = wtCu / 64.0 / (Cgamma / 12.0 + wtSi / 28.0 + wtMn / 55.0 +
                        wtNi / 59.0 + wtCu / 64.0 + wtCr / 52.0 +
                        (100.0 - Cgamma - wtSi - wtMn - wtNi - wtCu - wtCr) / 56.0);
  X_Cr = wtCr / 52.0 / (Cgamma / 12.0 + wtSi / 28.0 + wtMn / 55.0 +
                        wtNi / 59.0 + wtCu / 64.0 + wtCr / 52.0 +
                        (100.0 - Cgamma - wtSi - wtMn - wtNi - wtCu - wtCr) / 56.0);
  T10 = 141.0 * (X_Si * (-3.0) + X_Mn * (+2.0) + X_Ni * (+12.0) +
                 X_Cu * (+16.0) + X_Cr * (-1.0));
  T20 = 100.0 * (X_Si * (-3.0) + X_Mn * (-37.5) + X_Ni * (-6.0) +
                 X_Cu * (-4.5) + X_Cr * (-19.0));

  if (T < 3.56e+02) {
    G = 0.13132885e+04 + 1.3141025e-02 * T - 2.580128186e-03 * T * T;
  }
  if ((T >= 3.56e+02) && (T < 4.28e+02))
    G = 1.1224052e+03 + 3.542835682 * T - 2.2496774e-02 * T * T
        + 4.326701963e-05 * T * T * T - 3.072096339e-08 * T * T * T * T;
  if ((T >= 4.28e+02) && (T < 6.34e+02))
    G = -1.7031057e+04 + 0.1448353906e+03 * T - 0.428166357 * T * T
        + 5.50871582e-04 * T * T * T - 2.633397102e-07 * T * T * T * T;
  if ((T >= 6.34e+02) && (T < 7.40e+02))
    G = 0.13022482e+04 + 0.522110231 * T - 5.980360202e-03 * T * T
        + 6.659625695e-06 * T * T * T - 2.473169175e-09 * T * T * T * T;
  if ((T >= 7.40e+02) && (T < 8.60e+02))
    G = -0.64489093e+05 + 3.095104796e+02 * T - 0.547164618 * T * T
        + 4.255071973e-04 * T * T * T - 1.233061536e-07 * T * T * T * T;
  if ((T >= 8.60e+02) && (T < 9.27e+02))
    G = 0.13015055e+07 - 5.763825e+03 * T + 9.571875 * T * T
        - 7.0625e-03 * T * T * T + 1.953125e-06 * T * T * T * T;
  if ((T >= 9.27e+02) && (T < 1.08e+03))
    G = 0.58257237e+05 - 2.229275965e+02 * T + 0.322821329 * T * T
        - 2.089186484e-04 * T * T * T + 5.084325376e-08 * T * T * T * T;
  if ((T >= 1.08e+03) && (T < 1.24e+03))
    G = 0.14043448e+06 - 4.704834516e+02 * T + 0.591517384 * T * T
        - 3.306547594e-04 * T * T * T + 6.93139101e-08 * T * T * T * T;
  if ((T >= 1.24e+03) && (T < 1.502e+03))
    G = 0.15744376e+05 - 4.475440789e+01 * T + 4.7873355e-02 * T * T
        - 2.286184211e-05 * T * T * T + 4.111842105e-09 * T * T * T * T;
  if ((T >= 1.502e+03) && (T < 1.616e+03))
    G = - 0.58637400e+04 + 1.147433313e+01 * T
        - 7.5500e-03 * T * T + 1.6666666e-06 * T * T * T;
  if ((T >= 1.616e+03) && (T < 1.68e+03))
    G = 0.32972410e+06 - 8.211649405e+02 * T + 0.766426516 * T * T
        - 3.177724674e-04 * T * T * T + 4.938936812e-08 * T * T * T * T;
  if (T >= 1.68e+03)
    G = 0.33271186e+07 - 7.657409945e+03 * T
        + 6.607976863 * T * T - 2.534098538e-03 * T * T * T
        + 3.643939352e-07 * T * T * T * T;
  G = T10 - G * 4.187;
  //G=20853.06-466.35*T-0.046304*T*T+71.147*T*log(T); //*****Only for T<912+273 K.

  Zalpha = 12.0 - 8.0 * exp(-48570.0 / R / T);
  Zgamma = 14.0 - 12.0 * exp(-8054.0 / R / T);
  Dufe = (1.0 - Xf) * (G + R * T * (3.0 / (Zalpha - 3.0) *
                                    log((3.0 - Zalpha * Xf) / 3.0 / (1.0 - Xf)) -
                                    1.0 / (Zgamma - 1.0) * log((1.0 - Zgamma * Xa) / (1.0 - Xa)))) +
         Xf * (R * T * log(Xf * (1.0 - Zgama * Xa) / (Xa * (3.0 - Zalpha * Xf)))
               + (112206.0 - 38573.0) - (51.46 - 13.48) * T);

  return Dufe;
}

void Growth(int Cx, int Cy, int seed[1],
            int *pGrain, int *pPhase, double *pCarbon,
            double *pTemp, double dt, double Time,
            double *pFraction, double wtSi, double wtMn,
            double wtNi, double wtCu, double wtCr) {
  int i, j;
  int Nei[6] = {0, 0, 0, 0, 0, 0};
  int C_I, E_I, W_I;
  int EN_I, WN_I, WS_I, ES_I;
  double Dgamma = 2.5e-12;
  double Dalpha = 9.0e-11;
  double Calpha = 0.0;
  double Pcap = 0.0;
  double v = 0.0;
  double Du = 0.0;
  int k = 0;
  int l = 0;
  int m = 0;
  double iOri = 0.0;
  int iSite = 0;
  double *mCarbon;
  int *mPhase;
  double *mReject;

  mCarbon = new double[Cy * Cx + 1];
  mPhase = new int[Cy * Cx + 1];
  mReject = new double[Cy * Cx + 1];

  for (j = 1; j <= Cy; j++) {
    for (i = 1; i <= Cx; i++) {
      C_I = (j - 1) * Cx + i;
      mPhase[C_I] = pPhase[C_I];
      mCarbon[C_I] = pCarbon[C_I];
      mReject[C_I] = 0.0;
    }
  }

  Diffusion(Cx, Cy, dt, pCarbon, pTemp, pPhase, pGrain);

  for (j = 1; j <= Cy; j++) {
    for (i = 1; i <= Cx; i++) {
      C_I = (j - 1) * Cx + i;
      mCarbon[C_I] = pCarbon[C_I];
    }
  }

  for (j = 1; j <= Cy; j++) {
    for (i = 1; i <= Cx; i++) {
      C_I = (j - 1) * Cx + i;
      Neighbor(C_I, Cx, Cy, Nei);
      E_I = Nei[1];
      EN_I = Nei[2];
      WN_I = Nei[3];
      W_I = Nei[4];
      WS_I = Nei[5];
      ES_I = Nei[6];

      mReject[C_I] = 0.0;
      if ((mPhase[C_I] == 1) && (pFraction[C_I] < 0.99)) {
        Calpha = -MAX_BCC_SOLUTE_C *
               (pTemp[C_I] - PURE_FE_A3) / (PURE_FE_A3 - EUTECTOID_TEMP);

        k = 0;
        m = 0;
        if (E_I > 0) {
          if (m_Phase[E_I] == 2) {
            k = k + 1;
          }
          if (m_Phase[E_I] < 2) {
            if (pGrain[C_I] != pGrain[E_I]) {
              m = m + 1;
            }
          }
        }
        if (EN_I > 0) {
          if (m_Phase[EN_I] == 2) {
            k = k + 1;
          }
          if (m_Phase[EN_I] < 2) {
            if (pGrain[C_I] != pGrain[EN_I]) {
              m = m + 1;
            }
          }
        }
        if (WN_I > 0) {
          if (m_Phase[WN_I] == 2) {
            k = k + 1;
          }
          if (m_Phase[WN_I] < 2) {
            if (pGrain[C_I] != pGrain[WN_I]) {
              m = m + 1;
            }
          }
        }
        if (W_I > 0) {
          if (m_Phase[W_I] == 2) {
            k = k + 1;
          }
          if (m_Phase[W_I] < 2) {
            if (pGrain[C_I] != pGrain[W_I]) {
              m = m + 1;
            }
          }
        }
        if (WS_I > 0) {
          if (m_Phase[WS_I] == 2) {
            k = k + 1;
          }
          if (m_Phase[WS_I] < 2) {
            if (pGrain[C_I] != pGrain[WS_I]) {
              m = m + 1;
            }
          }
        }
        if (ES_I > 0) {
          if (m_Phase[ES_I] == 2) {
            k = k + 1;
          }
          if (m_Phase[ES_I] < 2) {
            if (pGrain[C_I] != pGrain[ES_I]) {
              m = m + 1;
            }
          }
        }

        if (k > 0) {
          Du = MobiDrivingForce(mCarbon[C_I], Calpha, wtSi,
                                    wtMn, wtNi, wtCu, wtCr, pTemp[C_I]);
          if (m == 0) {
            v = 0.08 * exp(-140000.0 / R / pTemp[C_I]) * Du;
          } else {
            v = 0.24 * exp(-140000.0 / R / pTemp[C_I]) * Du;
          }
          if (v < 0.0) {
            v = 0.0;
          }

          pFraction[C_I] = pFraction[C_I] + k * v * dt / LC;

          if (pFraction[C_I] < 0.99) {
            mReject[C_I] = (mCarbon[C_I] - Calpha) * k * v * dt / LC;
          } else {
            mReject[C_I] = (mCarbon[C_I] - Calpha) * (1.0 + k * v * dt / LC -
                            pFraction[C_I]);
          }
        } else {
          Du = MobiDrivingForce(mCarbon[C_I], Calpha, wtSi,
                                    wtMn, wtNi, wtCu, wtCr, pTemp[C_I]);
          if (m == 0) {
            v = 0.08 * exp(-140000.0 / R / pTemp[C_I]) * Du;
          } else {
            v = 0.24 * exp(-140000.0 / R / pTemp[C_I]) * Du;
          }
          if (v < 0.0) {
            v = 0.0;
          }

          pFraction[C_I] = pFraction[C_I] + 6.0 * v * dt / LC;

          if (pFraction[C_I] < 0.99) {
            mReject[C_I] = (m_Carbon[C_I] - Calpha) * 6.0 * v * dt / LC;
          } else {
            mReject[C_I] = (m_Carbon[C_I] - Calpha) * (1.0 + 6.0 * v * dt / LC -
                           pFraction[C_I]);
          }
          if ((pFraction[C_I] > 0.99) || (pFraction[C_I] == 0.99)) {
            pPhase[C_I] = 2;
            pCarbon[C_I] = Calpha;

            continue;
          }
        }

        iSite = 0;
        iOri = 0.0;
        Pcap = 0.0;
        if (E_I > 0) {
          if (mPhase[E_I] == 2) {
            if (pFraction[C_I] > 0.99 || pFraction[C_I] == 0.99) {
              Pcap = RandomDouble(seed);
              if (iOri < Pcap) {
                iOri = Pcap;
                iSite = 1;
              }
            }
          }
        }

        if (EN_I > 0) {
          if (mPhase[EN_I] == 2) {
            if (pFraction[C_I] > 0.99 || pFraction[C_I] == 0.99) {
              Pcap = Random(seed);
              if (iOri < Pcap) {
                iOri = Pcap;
                iSite = 2;
              }
            }
          }
        }

        if (WN_I > 0) {
          if (mPhase[WN_I] == 2) {
            if (pFraction[C_I] > 0.99 || pFraction[C_I] == 0.99) {
              Pcap = Random(seed);
              if (iOri < Pcap) {
                iOri = Pcap;
                iSite = 3;
              }
            }
          }
        }

        if (W_I > 0) {
          if (mPhase[W_I] == 2) {
            if (pFraction[C_I] > 0.99 || pFraction[C_I] == 0.99) {
              Pcap = Random(seed);
              if (iOri < Pcap) {
                iOri = Pcap;
                iSite = 4;
              }
            }
          }
        }

        if (WS_I > 0) {
          if (mPhase[WS_I] == 2) {
            if (pFraction[C_I] > 0.99 || pFraction[C_I] == 0.99) {
              Pcap = Random(seed);
              if (iOri < Pcap) {
                iOri = Pcap;
                iSite = 5;
              }
            }
          }
        }
        if (ES_I > 0) {
          if (mPhase[ES_I] == 2) {
            if (pFraction[C_I] > 0.99 || pFraction[C_I] == 0.99) {
              Pcap = Random(seed);
              if (iOri < Pcap) {
                iOri = Pcap;
                iSite = 6;
              }
            }
          }
        }

        if (iSite > 0) {
          pPhase[C_I] = 2;
          pCarbon[C_I] = Calpha;
          pGrain[C_I] = pGrain[Nei[iSite]];
        }
      }
    }
  }

  for (j = 1; j <= Cy; j++) {
    for (i = 1; i <= Cx; i++) {
      C_I = (j - 1) * Cx + i;
      Neighbor(C_I, Cx, Cy, Nei);
      E_I = Nei[1];
      EN_I = Nei[2];
      WN_I = Nei[3];
      W_I = Nei[4];
      WS_I = Nei[5];
      ES_I = Nei[6];

      if (mReject[C_I] != 0.0) {
        k = 0;
        if (E_I > 0) {
          if (pPhase[E_I] < 2) {
            k = k + 1;
          }
        }
        if (EN_I > 0) {
          if (pPhase[EN_I] < 2) {
            k = k + 1;
          }
        }
        if (WN_I > 0) {
          if (pPhase[WN_I] < 2) {
            k = k + 1;
          }
        }
        if (W_I > 0) {
          if (pPhase[W_I] < 2) {
            k = k + 1;
          }
        }
        if (WS_I > 0) {
          if (pPhase[WS_I] < 2) {
            k = k + 1;
          }
        }
        if (ES_I > 0) {
          if (pPhase[ES_I] < 2) {
            k = k + 1;
          }
        }

        if (k > 0) {
          if (E_I > 0) {
            if (pPhase[E_I] < 2) {
              pCarbon[E_I] = pCarbon[E_I] +
                             mReject[C_I] / double(k);
            }
          }
          if (EN_I > 0) {
            if (pPhase[EN_I] < 2) {
              pCarbon[EN_I] = pCarbon[EN_I] +
                              mReject[C_I] / double(k);
            }
          }
          if (WN_I > 0) {
            if (pPhase[WN_I] < 2) {
              pCarbon[WN_I] = pCarbon[WN_I] +
                              mReject[C_I] / double(k);
            }
          }
          if (W_I > 0) {
            if (pPhase[W_I] < 2) {
              pCarbon[W_I] = pCarbon[W_I] +
                             mReject[C_I] / double(k);
            }
          }
          if (WS_I > 0) {
            if (pPhase[WS_I] < 2) {
              pCarbon[WS_I] = pCarbon[WS_I] +
                              mReject[C_I] / double(k);
            }
          }
          if (ES_I > 0) {
            if (pPhase[ES_I] < 2) {
              pCarbon[ES_I] = pCarbon[ES_I] +
                              mReject[C_I] / double(k);
            }
          }
        }
      }
    }
  }

  for (j = 1; j <= Cy; j++) {
    for (i = 1; i <= Cx; i++) {
      C_I = (j - 1) * Cx + i;
      Neighbor(C_I, Cx, Cy, Nei);
      E_I = Nei[1];
      EN_I = Nei[2];
      WN_I = Nei[3];
      W_I = Nei[4];
      WS_I = Nei[5];
      ES_I = Nei[6];

      if (pPhase[C_I] == 2) {
        if (E_I > 0) {
          if (pPhase[E_I] == 0) {
            pPhase[E_I] = 1;
          }
        }
        if (EN_I > 0) {
          if (pPhase[EN_I] == 0) {
            pPhase[EN_I] = 1;
          }
        }
        if (WN_I > 0) {
          if (pPhase[WN_I] == 0) {
            pPhase[WN_I] = 1;
          }
        }
        if (W_I > 0) {
          if (pPhase[W_I] == 0) {
            pPhase[W_I] = 1;
          }
        }
        if (WS_I > 0) {
          if (pPhase[WS_I] == 0) {
            pPhase[WS_I] = 1;
          }
        }
        if (ES_I > 0) {
          if (pPhase[ES_I] == 0) {
            pPhase[ES_I] = 1;
          }
        }
      }
    }
  }

  delete Nei;
  delete mCarbon;
  delete mPhase;
  delete mReject;
}

double MobiDrivingForce(double CGamma, double Calpha, double wtSi,
							double wtMn, double wtNi, double wtCu,
							double wtCr, double T)
{
  double Xa;
	double Xf;
	double X_Si, X_Mn, X_Ni, X_Cu, X_Cr;
	double Dufe;
	double G;
	double Zalpha;
	double Zgamma;
	double Galpha;
	double Ggamma;
	double T10, T20;

	Xa=m_Gama/12.0/(m_Gama/12.0+m_Si/28.0+m_Mn/55.0+
		m_Ni/59.0+m_Cu/64.0+m_Cr/52.0+
		(100.0-m_Gama-m_Si-m_Mn-m_Ni-m_Cu-m_Cr)/56.0);
	Xf=m_Alfa/12.0/(m_Alfa/12.0+m_Si/28.0+m_Mn/55.0+
		m_Ni/59.0+m_Cu/64.0+m_Cr/52.0+
		(100.0-m_Alfa-m_Si-m_Mn-m_Ni-m_Cu-m_Cr)/56.0);
	X_Si=m_Si/28.0/(m_Gama/12.0+m_Si/28.0+m_Mn/55.0+
		m_Ni/59.0+m_Cu/64.0+m_Cr/52.0+
		(100.0-m_Gama-m_Si-m_Mn-m_Ni-m_Cu-m_Cr)/56.0);
	X_Mn=m_Mn/55.0/(m_Gama/12.0+m_Si/28.0+m_Mn/55.0+
		m_Ni/59.0+m_Cu/64.0+m_Cr/52.0+
		(100.0-m_Gama-m_Si-m_Mn-m_Ni-m_Cu-m_Cr)/56.0);
	X_Ni=m_Ni/59.0/(m_Gama/12.0+m_Si/28.0+m_Mn/55.0+
		m_Ni/59.0+m_Cu/64.0+m_Cr/52.0+
		(100.0-m_Gama-m_Si-m_Mn-m_Ni-m_Cu-m_Cr)/56.0);
	X_Cu=m_Cu/64.0/(m_Gama/12.0+m_Si/28.0+m_Mn/55.0+
		m_Ni/59.0+m_Cu/64.0+m_Cr/52.0+
		(100.0-m_Gama-m_Si-m_Mn-m_Ni-m_Cu-m_Cr)/56.0);
	X_Cr=m_Cr/52.0/(m_Gama/12.0+m_Si/28.0+m_Mn/55.0+
		m_Ni/59.0+m_Cu/64.0+m_Cr/52.0+
		(100.0-m_Gama-m_Si-m_Mn-m_Ni-m_Cu-m_Cr)/56.0);
	T10=141.0*(X_Si*(+3.0)+X_Mn*(-2.0)+X_Ni*(-12.0)+
		X_Cu*(-16.0)+X_Cr*(+1.0));
	T20=100.0*(X_Si*(-0.0)+X_Mn*(-39.5)+X_Ni*(-18.0)+X_Cu*(-11.5)+X_Cr*(-18.0));
	//T=Temp+273-T20;;
	Dufe=0.0;
	G=0.0;
	Zalfa=0.0;
	Zgama=0.0;
	Galfa=0.0;
	Ggama=0.0;

	if(T<3.56e+02)
		G =  0.13132885e+04 + 1.3141025e-02*T - 2.580128186e-03*T*T;
    if(T>=3.56e+02&&T<4.28e+02)
	    G =  1.1224052e+03 + 3.542835682*T - 2.2496774e-02*T*T
        + 4.326701963e-05*T*T*T - 3.072096339e-08*T*T*T*T;
    if(T>=4.28e+02&&T<6.34e+02)
	    G = - 1.7031057e+04 + 0.1448353906e+03*T - 0.428166357*T*T
        + 5.50871582e-04*T*T*T- 2.633397102e-07*T*T*T*T;
    if (T>=6.34e+02 && T<7.40e+02)
	    G = 0.13022482e+04 + 0.522110231*T - 5.980360202e-03*T*T
        + 6.659625695e-06*T*T*T - 2.473169175e-09*T*T*T*T;
    if (T>=7.40e+02 && T<8.60e+02)
	    G = - 0.64489093e+05 + 3.095104796e+02*T - 0.547164618*T*T
        + 4.255071973e-04*T*T*T - 1.233061536e-07*T*T*T*T;
    if (T>=8.60e+02 && T<9.27e+02)
	    G = + 0.13015055e+07 - 5.763825e+03*T + 9.571875*T*T
        - 7.0625e-03*T*T*T + 1.953125e-06*T*T*T*T;
    if (T>=9.27e+02 && T<1.08e+03)
	    G = + 0.58257237e+05 - 2.229275965e+02*T + 0.322821329*T*T
        - 2.089186484e-04*T*T*T + 5.084325376e-08*T*T*T*T;
    if (T>=1.08e+03 && T<1.24e+03)
	    G = 0.14043448e+06 - 4.704834516e+02*T + 0.591517384*T*T
        - 3.306547594e-04*T*T*T + 6.93139101e-08*T*T*T*T;
    if (T>=1.24e+03 && T<1.502e+03)
	    G = 0.15744376e+05 - 4.475440789e+01*T + 4.7873355e-02*T*T
        - 2.286184211e-05*T*T*T + 4.111842105e-09*T*T*T*T;
    if (T>=1.502e+03 && T<1.616e+03)
	    G = - 0.58637400e+04 + 1.147433313e+01*T
        - 7.5500e-03*T*T + 1.6666666e-06*T*T*T;
    if (T>=1.616e+03&&T<1.68e+03)
	    G = 0.32972410e+06 - 8.211649405e+02*T + 0.766426516*T*T
        - 3.177724674e-04*T*T*T + 4.938936812e-08*T*T*T*T;
    if (T>=1.68e+03)
	    G = 0.33271186e+07 - 7.657409945e+03*T
        + 6.607976863*T*T - 2.534098538e-03*T*T*T
        + 3.643939352e-07*T*T*T*T;
	G=T10*4.187-G*4.187;
	//G=20853.06-466.35*T-0.046304*T*T+71.147*T*log(T); //*****Only for T<912+273 K.
	G=-G;
	G=G+0.0;

	Zalfa=12.0-8.0*exp(-48570.0/R/T);
	Zgama=14.0-12.0*exp(-8054.0/R/T);
	Dufe=G+R*T*(1.0/(Zgama-1.0)*log((1.0-Zgama*Xa)/(1.0-Xa))-
		3.0/(Zalfa-3.0)*log((3.0-Zalfa*Xf)/3.0/(1.0-Xf)));

	return Dufe;
}


void Diffusion(int Cx, int Cy, double dt, double *pCarbon, int *pPhase,
              double *pTemp, int *pGrain) {
  int i, j;
  double Dgamma = 2.5e-12;
  double Dalpha = 9.0e-11;
  double *mCarbon;
  int Nei[6] = {0, 0, 0, 0, 0, 0};
  int C_I, E_I, W_I;
  int EN_I, WN_I, WS_I, ES_I;
  double Calpha = 0.0;
  double Ae3 = 0.0;

  mCarbon = new double[Cy * Cx + 1];

  for (j = 1; j <= Cy; j++) {
    for (i = 1; i <= Cx; i++) {
      C_I = (j - 1) * Cx + i;
      mCarbon[C_I] = pCarbon[C_I];
    }
  }

  for (j = 1; j <= Cy; j++) {
    for (i = 1; i <= Cx; i++) {
      C_I = (j - 1) * Cx + i;
      Neighbor(C_I, Cx, Cy, Nei);
      E_I = Nei[0];
      EN_I = Nei[1];
      WN_I = Nei[2];
      W_I = Nei[3];
      WS_I = Nei[4];
      ES_I = Nei[5];

      if (pPhase[C_I] == 0) {
        if (E_I > 0) {
          if (pPhase[E_I] < 2) {
            if (pGrain[C_I] == pGrain[E_I]) {
              mCarbon[C_I] = mCarbon[C_I] +
                             2.0 / 3.0 * dt * Dgamma / pow(LC, 2.0) *
                             (pCarbon[E_I] - pCarbon[C_I]);
            } else {
              mCarbon[C_I] = mCarbon[C_I] +
                             2.0 / 3.0 * dt * Dgamma * 100.0 / pow(LC, 2.0) *
                             (pCarbon[E_I] - pCarbon[C_I]);
            }
          }
        }

        if (EN_I > 0) {
          if(pPhase [EN_I]  < 2) {
            if(pGrain[C_I] == pGrain[EN_I]) {
              mCarbon[C_I] = mCarbon[C_I] +
                             2.0 / 3.0 * dt * Dgamma / pow(LC, 2.0) *
                             (pCarbon[EN_I] - pCarbon[C_I]);
            } else {
              mCarbon[C_I] = mCarbon[C_I] +
                             2.0 / 3.0 * dt * Dgamma * 100.0 / pow(LC, 2.0) *
                             (pCarbon[EN_I] - pCarbon[C_I]);
            }
          }
        }

        if (WN_I > 0) {
          if (pPhase[WN_I]  < 2) {
            if (pGrain[C_I] == pGrain[WN_I]) {
              mCarbon[C_I] = mCarbon[C_I] +
                             2.0 / 3.0 * dt * Dgamma / pow(LC, 2.0) *
                             (pCarbon[WN_I] - pCarbon[C_I]);
            } else {
              mCarbon[C_I] = mCarbon[C_I] +
                             2.0 / 3.0 * dt * Dgamma * 100.0 / pow(LC, 2.0) *
                             (pCarbon[WN_I] - pCarbon[C_I]);
            }
          }
        }

        if (W_I > 0) {
          if (pPhase[W_I]  < 2) {
            if (pGrain[C_I] == pGrain[W_I]) {
              mCarbon[C_I] = mCarbon[C_I] +
                             2.0 / 3.0 * dt * Dgamma / pow(LC, 2.0) *
                             (pCarbon[W_I] - pCarbon[C_I]);
            } else {
              mCarbon[C_I] = mCarbon[C_I] +
                             2.0 / 3.0 * dt * Dgamma * 100.0 / pow(LC, 2.0) *
                             (pCarbon[W_I] - pCarbon[C_I]);
            }
          }
        }

        if (WS_I > 0) {
          if (pPhase[WS_I]  < 2) {
            if (pGrain[C_I] == pGrain[WS_I]) {
              mCarbon[C_I] = mCarbon[C_I] +
                             2.0 / 3.0 * dt * Dgamma / pow(LC, 2.0) *
                             (pCarbon[WS_I] - pCarbon[C_I]);
            } else {
              mCarbon[C_I] = mCarbon[C_I] +
                             2.0 / 3.0 * dt * Dgamma * 100.0 / pow(LC, 2.0) *
                             (pCarbon[WS_I] - pCarbon[C_I]);
            }
          }
        }

        if (ES_I > 0) {
          if (pPhase[ES_I]  < 2) {
            if (pGrain[C_I] == pGrain[ES_I]) {
              mCarbon[C_I] = mCarbon[C_I] +
                             2.0 / 3.0 * dt * Dgamma / pow(LC, 2.0) *
                             (pCarbon[ES_I] - pCarbon[C_I]);
            } else {
              mCarbon[C_I] = mCarbon[C_I] +
                             2.0 / 3.0 * dt * Dgamma * 100.0 / pow(LC, 2.0) *
                             (pCarbon[ES_I] - pCarbon[C_I]);
            }
          }
        }
      }

      if (pPhase[C_I] == 2) {
        if (E_I > 0) {
          if (pPhase[E_I] == 2) {
            mCarbon[C_I] = mCarbon[C_I] +
                           2.0 / 3.0 * dt * Dalpha / pow(LC, 2.0) *
                           (pCarbon[E_I] - pCarbon[C_I]);
          }
          if (pPhase[E_I] == 1) {
            Calpha = -MAX_BCC_SOLUTE_C *
                     (pTemp[E_I] - PURE_FE_A3) / (PURE_FE_A3 - EUTECTOID_TEMP);
            mCarbon[C_I] = mCarbon[C_I] +
                           2.0 / 3.0 * dt * Dalpha / pow(LC, 2.0) *
                           (Calpha - pCarbon[C_I]);
          }
        }

        if (EN_I > 0) {
          if (pPhase[EN_I] == 2) {
            mCarbon[C_I] = mCarbon[C_I] +
                           2.0 / 3.0 * dt * Dalpha / pow(LC, 2.0) *
                           (pCarbon[EN_I] - pCarbon[C_I]);
          }
          if (pPhase[EN_I] == 1) {
            Calpha = -MAX_BCC_SOLUTE_C *
                     (pTemp[EN_I] - PURE_FE_A3) / (PURE_FE_A3 - EUTECTOID_TEMP);
            mCarbon[C_I] = mCarbon[C_I] +
                           2.0 / 3.0 * dt * Dalpha / pow(LC, 2.0) *
                           (Calpha - pCarbon[C_I]);
          }
        }

        if (WN_I > 0) {
          if (pPhase[WN_I] == 2) {
            mCarbon[C_I] = mCarbon[C_I] +
                           2.0 / 3.0 * dt * Dalpha / pow(LC, 2.0) *
                           (pCarbon[WN_I] - pCarbon[C_I]);
          }
          if (pPhase[WN_I] == 1) {
            Calpha = -MAX_BCC_SOLUTE_C *
                     (pTemp[WN_I] - PURE_FE_A3) / (PURE_FE_A3 - EUTECTOID_TEMP);
            mCarbon[C_I] = mCarbon[C_I] +
                           2.0 / 3.0 * dt * Dalpha / pow(LC, 2.0) *
                           (Calpha - pCarbon[C_I]);
          }
        }

        if (W_I > 0) {
          if(pPhase[W_I] == 2) {
            mCarbon[C_I] = mCarbon[C_I] +
                           2.0 / 3.0 * dt * Dalpha / pow(LC, 2.0) *
                           (pCarbon[W_I] - pCarbon[C_I]);
          }
          if (pPhase[W_I] == 1) {
            Calpha = -MAX_BCC_SOLUTE_C *
                     (pTemp[W_I] - PURE_FE_A3) / (PURE_FE_A3 - EUTECTOID_TEMP);
            mCarbon[C_I] = mCarbon[C_I] +
                           2.0 / 3.0 * dt * Dalpha / pow(LC, 2.0) *
                           (Calpha - pCarbon[C_I]);
          }
        }

        if(WS_I > 0) {
          if(pPhase[WS_I] == 2) {
            mCarbon[C_I] = mCarbon[C_I] +
                           2.0 / 3.0 * dt * Dalpha / pow(LC, 2.0) *
                           (pCarbon[WS_I] - pCarbon[C_I]);
          }
          if (pPhase[WS_I] == 1) {
            Calpha = -MAX_BCC_SOLUTE_C *
                     (pTemp[WS_I] - PURE_FE_A3) / (PURE_FE_A3 - EUTECTOID_TEMP);
            mCarbon[C_I] = mCarbon[C_I] +
                           2.0 / 3.0 * dt * Dalpha / pow(LC, 2.0) *
                           (Calpha - pCarbon[C_I]);
          }
        }

        if(ES_I > 0) {
          if(pPhase[ES_I] == 2) {
            mCarbon[C_I] = mCarbon[C_I] +
                           2.0 / 3.0 * dt * Dalpha / pow(LC, 2.0) *
                           (pCarbon[ES_I] - pCarbon[C_I]);
          }
          if (pPhase[ES_I] == 1) {
            Calpha = -MAX_BCC_SOLUTE_C *
                     (pTemp[ES_I] - PURE_FE_A3) / (PURE_FE_A3 - EUTECTOID_TEMP);
            mCarbon[C_I] = mCarbon[C_I] +
                           2.0 / 3.0 * dt * Dalpha / pow(LC, 2.0) *
                           (Calpha - pCarbon[C_I]);
          }
        }
      }

      if (pPhase[C_I] == 1) {
      Calpha = -MAX_BCC_SOLUTE_C *
               (pTemp[C_I] - PURE_FE_A3) / (PURE_FE_A3 - EUTECTOID_TEMP);
        if (E_I > 0) {
          if (pPhase[E_I] < 2) {
            mCarbon[C_I] = mCarbon[C_I] +
                           2.0 / 3.0 * dt * Dgamma * 100.0 / pow(LC, 2.0) *
                           (pCarbon[E_I] - pCarbon[C_I]);
          }
          if (pPhase[E_I] == 2) {
            mCarbon[C_I] = mCarbon[C_I] +
                           2.0 / 3.0 * dt * Dalpha / pow(LC, 2.0) *
                           (pCarbon[E_I] - Calpha);
          }
        }

        if (EN_I > 0) {
          if (pPhase[EN_I] < 2) {
            mCarbon[C_I] = mCarbon[C_I] +
                           2.0 / 3.0 * dt * Dgamma * 100.0 / pow(LC, 2.0) *
                           (pCarbon[EN_I] - pCarbon[C_I]);
          }
          if(pPhase[EN_I] == 2) {
            mCarbon[C_I] = mCarbon[C_I] +
                           2.0 / 3.0 * dt * Dalpha / pow(LC, 2.0) *
                           (pCarbon[EN_I] - Calpha);
          }
        }

        if (WN_I > 0) {
          if (pPhase[WN_I] < 2) {
            mCarbon[C_I] = mCarbon[C_I] +
                           2.0 / 3.0 * dt * Dgamma * 100.0 / pow(LC, 2.0) *
                           (pCarbon[WN_I] - pCarbon[C_I]);
          }
          if(pPhase[WN_I] == 2) {
            mCarbon[C_I] = mCarbon[C_I] +
                           2.0 / 3.0 * dt * Dalpha / pow(LC, 2.0) *
                           (pCarbon[WN_I] - Calpha);
          }
        }

        if (W_I > 0) {
          if (pPhase[W_I] < 2) {
            mCarbon[C_I] = mCarbon[C_I] +
                           2.0 / 3.0 * dt * Dgamma * 100.0 / pow(LC, 2.0) *
                           (pCarbon[W_I] - pCarbon[C_I]);
          }
          if(pPhase[W_I] == 2) {
            mCarbon[C_I] = mCarbon[C_I] +
                           2.0 / 3.0 * dt * Dalpha / pow(LC, 2.0) *
                           (pCarbon[W_I] - Calpha);
          }
        }

        if (WS_I > 0) {
          if (pPhase[WS_I] < 2) {
            mCarbon[C_I] = mCarbon[C_I] +
                           2.0 / 3.0 * dt * Dgamma * 100.0 / pow(LC, 2.0) *
                           (pCarbon[WS_I] - pCarbon[C_I]);
          }
          if(pPhase[WS_I] == 2) {
            mCarbon[C_I] = mCarbon[C_I] +
                           2.0 / 3.0 * dt * Dalpha / pow(LC, 2.0) *
                           (pCarbon[WS_I] - Calpha);
          }
        }

        if (ES_I > 0) {
          if (pPhase[ES_I] < 2) {
            mCarbon[C_I] = mCarbon[C_I] +
                           2.0 / 3.0 * dt * Dgamma * 100.0 / pow(LC, 2.0) *
                           (pCarbon[ES_I] - pCarbon[C_I]);
          }
          if(pPhase[ES_I] == 2) {
            mCarbon[C_I] = mCarbon[C_I] +
                           2.0 / 3.0 * dt * Dalpha / pow(LC, 2.0) *
                           (pCarbon[ES_I] - Calpha);
          }
        }
      }
    }
  }

  for (j = 1; j <= Cy; j++) {
    for (i = 1; i <= Cx; i++) {
      C_I = (j - 1) * Cx + i;
      pCarbon[C_I] = mCarbon[C_I];
    }
  }

  delete mCarbon;
}

void Coarsening(int Cx, int Cy, double dt, double dx,
						double dy, int * pGrain, double * pFraction, int * pPhase,
						int GrainNf)
{
	int i, j;
	int ii, jj, kk;
	int k, l, m;
	int C_I, C_Nei;
	int E_I, EN_I, WN_I, W_I, WS_I, ES_I;
	int m_Num;
	int m_Index;
	double m_Ve, m_Ven, m_Vwn, m_Vw, m_Vws, m_Ves;
	double m_Coef, m_Coef0, m_Coef1;
	double m_Curvature;
	int * m_Ori;
	int * m_CryOri;
	int * m_NumOri;
	long int m_Ran;
	int * m_Orinei;
	int * m_Ori1;
	double m_Theta;
	double m_Theta0;
	int m_Max, m_Maxi;
	double m_Raver;
	int m_Grainabnormal;
	int * p_Phase;
	int * pNei;

	m_Coef0=1.0e-10;//9.0e-11;
	m_Coef1=1.0e-10;//3.0e-10;
	m_Ran=19;
	m_CryOri=new int[9];
	m_NumOri=new int[9];
	m_Orinei=new int[26];
	m_Ori=new int[m_Grain+1];
	m_Ori1=new int[m_Grain+1];
	m_Theta0=70.0/180.0*3.14;
	m_Raver=0.0;
	m_Grainabnormal=0;//400;200
	p_Phase=new int[m_Celly*m_Cellx+1];
	pNei=new int[25];

	for(jj=1;jj<=m_Grain;jj++)
	{
		m_Ori[jj]=0;
		m_Ori1[jj]=0;
	}
	for(j=1;j<=m_Celly;j++)
	{
		for(i=1;i<=m_Cellx;i++)
		{
			C_I=(j-1)*m_Cellx+i;

			p_Phase[C_I]=0;
			for(jj=1;jj<=m_Grain;jj++)
			{
				if(pPhase[C_I]==2&&p_Grain[C_I]==jj)
				{
					m_Ori[jj]++;
					m_Ori1[jj]++;
					continue;
				}
			}
		}
	}

	for(ii=1;ii<=m_Grainabnormal;ii++)
	{
		m_Max=m_Ori1[ii];
		m_Maxi=ii;
		for(jj=ii+1;jj<=m_Grain;jj++)
		{
			if(m_Ori1[jj]>m_Max)
			{
				m_Max=m_Ori1[jj];
				m_Maxi=jj;
			}
		}
		if(m_Maxi!=ii)
		{
			m_Max=m_Ori1[ii];
			m_Ori1[ii]=m_Ori1[m_Maxi];
			m_Ori1[m_Maxi]=m_Max;
		}
	}

	for(j=1;j<=m_Celly;j++)
	{
		for(i=1;i<=m_Cellx;i++)
		{
			C_I=(j-1)*m_Cellx+i;
			for(jj=1;jj<=m_Grainabnormal;jj++)
			{
				if(pPhase[C_I]==2&&p_Grain[C_I]==m_Ori1[jj])
				{
					p_Phase[C_I]=1;
					continue;
				}
			}
		}
	}

	for(j=1;j<=m_Celly;j++)
	{
		for(i=1;i<=m_Cellx;i++)
		{
			C_I=(j-1)*m_Cellx+i;
			Neighbor(C_I, m_Cellx, m_Celly, pNei);
			E_I=pNei[1];
			EN_I=pNei[2];
			WN_I=pNei[3];
			W_I=pNei[4];
			WS_I=pNei[5];
			ES_I=pNei[6];

			m=0;
			if(pPhase[C_I]==2)
			{
				if(E_I>0)
				{
					if(p_Grain[C_I]!=p_Grain[E_I]&&pPhase[E_I]==2)m++;
				}
				if(EN_I>0)
				{
					if(p_Grain[C_I]!=p_Grain[EN_I]&&pPhase[EN_I]==2)m++;
				}
				if(WN_I>0)
				{
					if(p_Grain[C_I]!=p_Grain[WN_I]&&pPhase[WN_I]==2)m++;
				}
				if(W_I>0)
				{
					if(p_Grain[C_I]!=p_Grain[W_I]&&pPhase[W_I]==2)m++;
				}
				if(WS_I>0)
				{
					if(p_Grain[C_I]!=p_Grain[WS_I]&&pPhase[WS_I]==2)m++;
				}
				if(ES_I>0)
				{
					if(p_Grain[C_I]!=p_Grain[ES_I]&&pPhase[ES_I]==2)m++;
				}

				if(m>0)
				{
					m_Num=0;
					m_Index=0;
					m_Curvature=0.0;

					for(l=j-2;l<=j+2;l++)
					{
						if(l==j)
						{
							for(k=i-2;k<=i+2;k++)
							{
								if(k>0&&l>0&&k<=m_Cellx&&l<=m_Celly)
								{
									C_Nei=(l-1)*m_Cellx+k;
									m_Num++;
									if(pPhase[C_Nei]==2)m_Orinei[m_Num]=p_Grain[C_Nei];

									if(p_Grain[C_I]==p_Grain[C_Nei]&&pPhase[C_Nei]==2)
									{
										m_Index++;
									}
								}
							}
						}
						if(l==j-2||l==j+2)
						{
							for(k=i-1;k<=i+1;k++)
							{
								if(k>0&&l>0&&k<=m_Cellx&&l<=m_Celly)
								{
									C_Nei=(l-1)*m_Cellx+k;
									m_Num++;
									if(pPhase[C_Nei]==2)m_Orinei[m_Num]=p_Grain[C_Nei];

									if(p_Grain[C_I]==p_Grain[C_Nei]&&pPhase[C_Nei]==2)
									{
										m_Index++;
									}
								}
							}
						}
						if(l==j-1||l==j+1)
						{
							if(j%2==1)
							{
								for(k=i-2;k<=i+1;k++)
								{
									if(k>0&&l>0&&k<=m_Cellx&&l<=m_Celly)
									{
										C_Nei=(l-1)*m_Cellx+k;
										m_Num++;
										if(pPhase[C_Nei]==2)m_Orinei[m_Num]=p_Grain[C_Nei];

										if(p_Grain[C_I]==p_Grain[C_Nei]&&pPhase[C_Nei]==2)
										{
											m_Index++;
										}
									}
								}
							}
							else
							{
								for(k=i-1;k<=i+2;k++)
								{
									if(k>0&&l>0&&k<=m_Cellx&&l<=m_Celly)
									{
										C_Nei=(l-1)*m_Cellx+k;
										m_Num++;
										if(pPhase[C_Nei]==2)m_Orinei[m_Num]=p_Grain[C_Nei];

										if(p_Grain[C_I]==p_Grain[C_Nei]&&pPhase[C_Nei]==2)
										{
											m_Index++;
										}
									}
								}
							}
						}
					}

//					m_Orinei1=m_Orinei2=m_Orinei3=m_Orinei[1];
//					for(jj=1;jj<=m_Num;jj++)
//					{
//					    if(m_Orinei[jj]!=m_Orinei1)m_Orinei2=m_Orinei[jj];
//					}
//					for(jj=1;jj<=m_Num;jj++)
//					{
//					    if(m_Orinei[jj]!=m_Orinei1&&m_Orinei[jj]!=m_Orinei2)
//					    {
//					        m_Orinei3=m_Orinei[jj];
//					    }
//					}
//
//					if(m_Orinei1!=m_Orinei2&&m_Orinei2!=m_Orinei3&&m_Orinei1!=m_Orinei3)
//					{
//					    m_Ix=i;
//					    m_Iy=j;
//				    } //Position of triple junction

					m_Curvature=2.8/m_Deltx*((m_Num+1)/2-m_Index)/m_Num;

					if(m_Curvature>0.0)
					{
						m_Ve=0.0;
						m_Ven=0.0;
						m_Vwn=0.0;
						m_Vw=0.0;
						m_Vws=0.0;
						m_Ves=0.0;
						if(E_I>0)
						{
							if(pPhase[E_I]==2)
							{
								if(p_Grain[C_I]!=p_Grain[E_I]&&p_Phase[C_I]==p_Phase[E_I])
								{
									m_Theta=3.14*fabs(p_Grain[E_I]-p_Grain[C_I])/double(m_Grain-1);
									if(m_Theta<m_Theta0)
									{
										m_Coef=m_Coef0*m_Theta/m_Theta0*(1.0-log(m_Theta/m_Theta0));
									}
									else
									{
										m_Coef=m_Coef0;
									}
									m_Ve=m_Coef*m_Curvature;
								}
								if(p_Grain[C_I]!=p_Grain[E_I]&&p_Phase[C_I]!=p_Phase[E_I])
								{
									m_Coef=m_Coef1;
									m_Ve=m_Coef*m_Curvature;
								}
							}
						}
						if(EN_I>0)
						{
							if(pPhase[EN_I]==2)
							{
								if(p_Grain[C_I]!=p_Grain[EN_I]&&p_Phase[C_I]==p_Phase[EN_I])
								{
									m_Theta=3.14*fabs(p_Grain[EN_I]-p_Grain[C_I])/double(m_Grain-1);
									if(m_Theta<m_Theta0)
									{
										m_Coef=m_Coef0*m_Theta/m_Theta0*(1.0-log(m_Theta/m_Theta0));
									}
									else
									{
										m_Coef=m_Coef0;
									}
									m_Ven=m_Coef*m_Curvature;
								}
								if(p_Grain[C_I]!=p_Grain[EN_I]&&p_Phase[C_I]!=p_Phase[EN_I])
								{
									m_Coef=m_Coef1;
									m_Ven=m_Coef*m_Curvature;
								}
							}
						}
						if(WN_I>0)
						{
							if(pPhase[WN_I]==2)
							{
								if(p_Grain[C_I]!=p_Grain[WN_I]&&p_Phase[C_I]==p_Phase[WN_I])
								{
									m_Theta=3.14*fabs(p_Grain[WN_I]-p_Grain[C_I])/double(m_Grain-1);
									if(m_Theta<m_Theta0)
									{
										m_Coef=m_Coef0*m_Theta/m_Theta0*(1.0-log(m_Theta/m_Theta0));
									}
									else
									{
										m_Coef=m_Coef0;
									}
									m_Vwn=m_Coef*m_Curvature;
								}
								if(p_Grain[C_I]!=p_Grain[WN_I]&&p_Phase[C_I]!=p_Phase[WN_I])
								{
									m_Coef=m_Coef1;
									m_Vwn=m_Coef*m_Curvature;
								}
							}
						}
						if(W_I>0)
						{
							if(pPhase[W_I]==2)
							{
								if(p_Grain[C_I]!=p_Grain[W_I]&&p_Phase[C_I]==p_Phase[W_I])
								{
									m_Theta=3.14*fabs(p_Grain[W_I]-p_Grain[C_I])/double(m_Grain-1);
									if(m_Theta<m_Theta0)
									{
										m_Coef=m_Coef0*m_Theta/m_Theta0*(1.0-log(m_Theta/m_Theta0));
									}
									else
									{
										m_Coef=m_Coef0;
									}
									m_Vw=m_Coef*m_Curvature;
								}
								if(p_Grain[C_I]!=p_Grain[W_I]&&p_Phase[C_I]!=p_Phase[W_I])
								{
									m_Coef=m_Coef1;
									m_Vw=m_Coef*m_Curvature;
								}
							}
						}
						if(WS_I>0)
						{
							if(pPhase[WS_I]==2)
							{
								if(p_Grain[C_I]!=p_Grain[WS_I]&&p_Phase[C_I]==p_Phase[WS_I])
								{
									m_Theta=3.14*fabs(p_Grain[WS_I]-p_Grain[C_I])/double(m_Grain-1);
									if(m_Theta<m_Theta0)
									{
										m_Coef=m_Coef0*m_Theta/m_Theta0*(1.0-log(m_Theta/m_Theta0));
									}
									else
									{
										m_Coef=m_Coef0;
									}
									m_Vws=m_Coef*m_Curvature;
								}
								if(p_Grain[C_I]!=p_Grain[WS_I]&&p_Phase[C_I]!=p_Phase[WS_I])
								{
									m_Coef=m_Coef1;
									m_Vws=m_Coef*m_Curvature;
								}
							}
						}
						if(ES_I>0)
						{
							if(pPhase[ES_I]==2)
							{
								if(p_Grain[C_I]!=p_Grain[ES_I]&&p_Phase[C_I]==p_Phase[ES_I])
								{
									m_Theta=3.14*fabs(p_Grain[ES_I]-p_Grain[C_I])/double(m_Grain-1);
									if(m_Theta<m_Theta0)
									{
										m_Coef=m_Coef0*m_Theta/m_Theta0*(1.0-log(m_Theta/m_Theta0));
									}
									else
									{
										m_Coef=m_Coef0;
									}
									m_Ves=m_Coef*m_Curvature;
								}
								if(p_Grain[C_I]!=p_Grain[ES_I]&&p_Phase[C_I]!=p_Phase[ES_I])
								{
									m_Coef=m_Coef1;
									m_Ves=m_Coef*m_Curvature;
								}
							}
						}
						p_Frac[C_I]=p_Frac[C_I]-m_Ve*m_Deltt/m_Deltx-m_Ven*m_Deltt/m_Delty
							-m_Vwn*m_Deltt/m_Delty-m_Vw*m_Deltt/m_Deltx-m_Vws*m_Deltt/m_Delty
							-m_Ves*m_Deltt/m_Delty;
					}
				}
			}
		}
	}

    for(j=1;j<=m_Celly;j++)
    {
        for(i=1;i<=m_Cellx;i++)
        {
            C_I=(j-1)*m_Cellx+i;
			Neighbor(C_I, m_Cellx, m_Celly, pNei);
            E_I=pNei[1];
			EN_I=pNei[2];
			WN_I=pNei[3];
			W_I=pNei[4];
			WS_I=pNei[5];
			ES_I=pNei[6];

            if(p_Frac[C_I]<0.01)
            {
                m_CryOri[0]=0;
                m_CryOri[1]=0;
                m_CryOri[2]=0;
                m_CryOri[3]=0;
				m_CryOri[4]=0;
                m_CryOri[5]=0;

                m_NumOri[0]=0;
                m_NumOri[1]=0;
                m_NumOri[2]=0;
                m_NumOri[3]=0;
				m_NumOri[4]=0;
                m_NumOri[5]=0;

                if(E_I>0)
                {
                    if(pPhase[E_I]==2)m_CryOri[0]=p_Grain[E_I];
                }
                if(EN_I>0)
                {
                    if(pPhase[EN_I]==2)m_CryOri[1]=p_Grain[EN_I];
                }
				if(WN_I>0)
                {
                    if(pPhase[WN_I]==2)m_CryOri[2]=p_Grain[WN_I];
                }
                if(W_I>0)
                {
                    if(pPhase[W_I]==2)m_CryOri[3]=p_Grain[W_I];
                }
                if(WS_I>0)
                {
                    if(pPhase[WS_I]==2)m_CryOri[4]=p_Grain[WS_I];
                }
				if(ES_I>0)
                {
                    if(pPhase[ES_I]==2)m_CryOri[5]=p_Grain[ES_I];
                }

                for(l=j-2;l<=j+2;l++)
                {
					if(j%2==1)
					{
						if(l==j)
						{
							for(k=i-2;k<=i+2;k++)
							{
								if(k>0&&l>0&&k<=m_Cellx&&l<=m_Celly)
								{
									C_Nei=(l-1)*m_Cellx+k;
									if(pPhase[C_Nei]==2)
									{
										if(E_I>0)
										{
											if(p_Grain[E_I]==p_Grain[C_Nei]
												&&pPhase[E_I]==2)m_NumOri[0]++;
										}
										if(EN_I>0)
										{
											if(p_Grain[EN_I]==p_Grain[C_Nei]
												&&pPhase[EN_I]==2)m_NumOri[1]++;
										}
										if(WN_I>0)
										{
											if(p_Grain[WN_I]==p_Grain[C_Nei]
												&&pPhase[WN_I]==2)m_NumOri[2]++;
										}
										if(W_I>0)
										{
											if(p_Grain[W_I]==p_Grain[C_Nei]
												&&pPhase[W_I]==2)m_NumOri[3]++;
										}
										if(WS_I>0)
										{
											if(p_Grain[WS_I]==p_Grain[C_Nei]
												&&pPhase[WS_I]==2)m_NumOri[4]++;
										}
										if(ES_I>0)
										{
											if(p_Grain[ES_I]==p_Grain[C_Nei]
												&&pPhase[ES_I]==2)m_NumOri[5]++;
										}
									}
								}
							}
						}
						if(l==j-1||l==j+1)
						{
							for(k=i-2;k<=i+1;k++)
							{
								if(k>0&&l>0&&k<=m_Cellx&&l<=m_Celly)
								{
									C_Nei=(l-1)*m_Cellx+k;
									if(pPhase[C_Nei]==2)
									{
										if(E_I>0)
										{
											if(p_Grain[E_I]==p_Grain[C_Nei]
												&&pPhase[E_I]==2)m_NumOri[0]++;
										}
										if(EN_I>0)
										{
											if(p_Grain[EN_I]==p_Grain[C_Nei]
												&&pPhase[EN_I]==2)m_NumOri[1]++;
										}
										if(WN_I>0)
										{
											if(p_Grain[WN_I]==p_Grain[C_Nei]
												&&pPhase[WN_I]==2)m_NumOri[2]++;
										}
										if(W_I>0)
										{
											if(p_Grain[W_I]==p_Grain[C_Nei]
												&&pPhase[W_I]==2)m_NumOri[3]++;
										}
										if(WS_I>0)
										{
											if(p_Grain[WS_I]==p_Grain[C_Nei]
												&&pPhase[WS_I]==2)m_NumOri[4]++;
										}
										if(ES_I>0)
										{
											if(p_Grain[ES_I]==p_Grain[C_Nei]
												&&pPhase[ES_I]==2)m_NumOri[5]++;
										}
									}
								}
							}
						}
						if(l==j-2||l==j+2)
						{
							for(k=i-1;k<=i+1;k++)
							{
								if(k>0&&l>0&&k<=m_Cellx&&l<=m_Celly)
								{
									C_Nei=(l-1)*m_Cellx+k;
									if(pPhase[C_Nei]==2)
									{
										if(E_I>0)
										{
											if(p_Grain[E_I]==p_Grain[C_Nei]
												&&pPhase[E_I]==2)m_NumOri[0]++;
										}
										if(EN_I>0)
										{
											if(p_Grain[EN_I]==p_Grain[C_Nei]
												&&pPhase[EN_I]==2)m_NumOri[1]++;
										}
										if(WN_I>0)
										{
											if(p_Grain[WN_I]==p_Grain[C_Nei]
												&&pPhase[WN_I]==2)m_NumOri[2]++;
										}
										if(W_I>0)
										{
											if(p_Grain[W_I]==p_Grain[C_Nei]
												&&pPhase[W_I]==2)m_NumOri[3]++;
										}
										if(WS_I>0)
										{
											if(p_Grain[WS_I]==p_Grain[C_Nei]
												&&pPhase[WS_I]==2)m_NumOri[4]++;
										}
										if(ES_I>0)
										{
											if(p_Grain[ES_I]==p_Grain[C_Nei]
												&&pPhase[ES_I]==2)m_NumOri[5]++;
										}
									}
								}
							}
						}
					}
					if(j%2==0)
					{
						if(l==j)
						{
							for(k=i-2;k<=i+2;k++)
							{
								if(k>0&&l>0&&k<=m_Cellx&&l<=m_Celly)
								{
									C_Nei=(l-1)*m_Cellx+k;
									if(pPhase[C_Nei]==2)
									{
										if(E_I>0)
										{
											if(p_Grain[E_I]==p_Grain[C_Nei]
												&&pPhase[E_I]==2)m_NumOri[0]++;
										}
										if(EN_I>0)
										{
											if(p_Grain[EN_I]==p_Grain[C_Nei]
												&&pPhase[EN_I]==2)m_NumOri[1]++;
										}
										if(WN_I>0)
										{
											if(p_Grain[WN_I]==p_Grain[C_Nei]
												&&pPhase[WN_I]==2)m_NumOri[2]++;
										}
										if(W_I>0)
										{
											if(p_Grain[W_I]==p_Grain[C_Nei]
												&&pPhase[W_I]==2)m_NumOri[3]++;
										}
										if(WS_I>0)
										{
											if(p_Grain[WS_I]==p_Grain[C_Nei]
												&&pPhase[WS_I]==2)m_NumOri[4]++;
										}
										if(ES_I>0)
										{
											if(p_Grain[ES_I]==p_Grain[C_Nei]
												&&pPhase[ES_I]==2)m_NumOri[5]++;
										}
									}
								}
							}
						}
						if(l==j-1||l==j+1)
						{
							for(k=i-1;k<=i+2;k++)
							{
								if(k>0&&l>0&&k<=m_Cellx&&l<=m_Celly)
								{
									C_Nei=(l-1)*m_Cellx+k;
									if(pPhase[C_Nei]==2)
									{
										if(E_I>0)
										{
											if(p_Grain[E_I]==p_Grain[C_Nei]
												&&pPhase[E_I]==2)m_NumOri[0]++;
										}
										if(EN_I>0)
										{
											if(p_Grain[EN_I]==p_Grain[C_Nei]
												&&pPhase[EN_I]==2)m_NumOri[1]++;
										}
										if(WN_I>0)
										{
											if(p_Grain[WN_I]==p_Grain[C_Nei]
												&&pPhase[WN_I]==2)m_NumOri[2]++;
										}
										if(W_I>0)
										{
											if(p_Grain[W_I]==p_Grain[C_Nei]
												&&pPhase[W_I]==2)m_NumOri[3]++;
										}
										if(WS_I>0)
										{
											if(p_Grain[WS_I]==p_Grain[C_Nei]
												&&pPhase[WS_I]==2)m_NumOri[4]++;
										}
										if(ES_I>0)
										{
											if(p_Grain[ES_I]==p_Grain[C_Nei]
												&&pPhase[ES_I]==2)m_NumOri[5]++;
										}
									}
								}
							}
						}
						if(l==j-2||l==j+2)
						{
							for(k=i-1;k<=i+1;k++)
							{
								if(k>0&&l>0&&k<=m_Cellx&&l<=m_Celly)
								{
									C_Nei=(l-1)*m_Cellx+k;
									if(pPhase[C_Nei]==2)
									{
										if(E_I>0)
										{
											if(p_Grain[E_I]==p_Grain[C_Nei]
												&&pPhase[E_I]==2)m_NumOri[0]++;
										}
										if(EN_I>0)
										{
											if(p_Grain[EN_I]==p_Grain[C_Nei]
												&&pPhase[EN_I]==2)m_NumOri[1]++;
										}
										if(WN_I>0)
										{
											if(p_Grain[WN_I]==p_Grain[C_Nei]
												&&pPhase[WN_I]==2)m_NumOri[2]++;
										}
										if(W_I>0)
										{
											if(p_Grain[W_I]==p_Grain[C_Nei]
												&&pPhase[W_I]==2)m_NumOri[3]++;
										}
										if(WS_I>0)
										{
											if(p_Grain[WS_I]==p_Grain[C_Nei]
												&&pPhase[WS_I]==2)m_NumOri[4]++;
										}
										if(ES_I>0)
										{
											if(p_Grain[ES_I]==p_Grain[C_Nei]
												&&pPhase[ES_I]==2)m_NumOri[5]++;
										}
									}
								}
							}
						}
					}
                }
                if(m_CryOri[0]!=p_Grain[C_I])
                {
                    m_Max=m_NumOri[0];
                    m_Maxi=0;
                }
                if(m_CryOri[1]!=p_Grain[C_I])
                {
                    m_Max=m_NumOri[1];
                    m_Maxi=1;
                }
                if(m_CryOri[2]!=p_Grain[C_I])
                {
                    m_Max=m_NumOri[2];
                    m_Maxi=2;
                }
                if(m_CryOri[3]!=p_Grain[C_I])
                {
                    m_Max=m_NumOri[3];
                    m_Maxi=3;
                }
				if(m_CryOri[4]!=p_Grain[C_I])
                {
                    m_Max=m_NumOri[4];
                    m_Maxi=4;
                }
                if(m_CryOri[5]!=p_Grain[C_I])
                {
                    m_Max=m_NumOri[5];
                    m_Maxi=5;
                }

                for(kk=0;kk<6;kk++)
                {
                    if(m_NumOri[kk]>m_Max&&m_CryOri[kk]!=p_Grain[C_I])
                    {
                        m_Max=m_NumOri[kk];
                        m_Maxi=kk;
                    }
                }
                p_Grain[C_I]=m_CryOri[m_Maxi];
                p_Frac[C_I]=1.0;
                if(m_Maxi==0)p_Phase[C_I]=p_Phase[E_I];
                if(m_Maxi==1)p_Phase[C_I]=p_Phase[EN_I];
                if(m_Maxi==2)p_Phase[C_I]=p_Phase[WN_I];
                if(m_Maxi==3)p_Phase[C_I]=p_Phase[W_I];
				if(m_Maxi==4)p_Phase[C_I]=p_Phase[WS_I];
				if(m_Maxi==5)p_Phase[C_I]=p_Phase[ES_I];
            }
        }
    }

	delete m_CryOri;
	delete m_NumOri;
	delete m_Orinei;
	delete m_Ori, m_Ori1;
	delete p_Phase;
	delete pNei;
}

void Neighbor(int C_I, int Cx, int Cy, int Nei[6]) {
  int i, j;
  int E_I, W_I;
  int ES_I, EN_I, WS_I, WN_I;

  if (C_I % Cx == 0) {
    j = C_I / Cx;
  } else {
    j = C_I / Cx + 1;
  }

  i = C_I - (j - 1) * Cx;

  E_I = 0;
  EN_I = 0;
  WN_I = 0;
  W_I = 0;
  WS_I = 0;
  ES_I = 0;

  if((i > 1) && (i < Cx) && (j > 1) && (j < Cy)) {
    E_I = C_I + 1;
    W_I = C_I - 1;
    if (j % 2 == 0) {
      EN_I = C_I - Cx;
      WN_I = C_I - Cx - 1;
      WS_I = C_I + Cx - 1;
      ES_I = C_I + Cx;
    } else {
      EN_I = C_I - Cx + 1;
      WN_I = C_I - Cx;
      WS_I = C_I + Cx;
      ES_I = C_I + Cx + 1;
    }
  }
  if ((i == 1) && (j > 1) && (j < Cy)) {
  E_I = C_I + 1;
  if (j % 2 == 0) {
      EN_I = C_I - Cx;
      ES_I = C_I + Cx;
    } else {
      EN_I = C_I - Cx + 1;
      WN_I = C_I - Cx;
      WS_I = C_I + Cx;
      ES_I = C_I + Cx + 1;
    }
  }
  if ((i == Cx) && (j > 1) && (j < Cy)) {
  W_I = C_I - 1;
  if (j % 2 == 0) {
      EN_I = C_I - Cx;
      WN_I = C_I - Cx - 1;
      WS_I = C_I + Cx - 1;
      ES_I = C_I + Cx;
    } else {
      WN_I = C_I - Cx;
      WS_I = C_I + Cx;
    }
  }
  if ((i > 1) && (i < Cx) && (j == 1)) {
    E_I = C_I + 1;
    W_I = C_I - 1;
    WS_I = C_I + Cx;
    ES_I = C_I + Cx + 1;
  }
  if ((i > 1) && (i < Cx) && (j == Cy)) {
    E_I = C_I + 1;
    W_I = C_I - 1;
    EN_I = C_I - Cx;
    WN_I = C_I - Cx - 1;
  }
  if ((i == 1) && (j == 1)) {
    E_I = C_I + 1;
    WS_I = C_I + Cx;
    ES_I = C_I + Cx + 1;
  }
  if ((i == 1) && (j == Cy)) {
    E_I = C_I + 1;
    EN_I = C_I - Cy;
  }
  if ((i == Cx) && (j == 1)) {
    W_I = C_I - 1;
    WS_I = C_I + Cx;
  }
  if ((i == Cx) && (j == Cy)) {
    W_I = C_I - 1;
    EN_I = C_I - Cx;
    WN_I = C_I - Cx - 1;
  }

  Nei[0] = E_I;
  Nei[1] = EN_I;
  Nei[2] = WN_I;
  Nei[3] = W_I;
  Nei[4] = WS_I;
  Nei[5] = ES_I;
}

double RandomDouble(int seed[1]) {
  double ran;
  unsigned int i;

  i = 2053 * seed[0];
  i = i + 13849;
  seed[0] = fmod(double(i), double(2147483648));
  ran = double(seed[0]) / double(2147483648);

  return ran;
}

int RandomInt(int range) {
  int k;

  k = (int)((double)(range + 1) * (double)rand() / (double)(RAND_MAX + 1.0));
  return ((int)k);
}

void Save(int iStep, int Cx, int Cy, int *pGrain, int *pPhase,
		double *pTemp, double *pCarbon, double *pFraction) {
	int i, j;
	int C_I;
	char strFN1[256], strFN2[256], strFN3[256], strFN4[256], strFN5[256];

	sprintf_s(strFN1, "%s%d%s", "Grain-", iStep, ".txt");
	sprintf_s(strFN2, "%s%d%s", "Phase-", iStep, ".txt");
	sprintf_s(strFN3, "%s%d%s", "Temp-", iStep, ".txt");
	sprintf_s(strFN4, "%s%d%s", "Carbon-", iStep, ".txt");
	sprintf_s(strFN5, "%s%d%s", "Frac-", iStep, ".txt");

	ofstream OFile1(strFN1,ios::out);
	ofstream OFile2(strFN2,ios::out);
	ofstream OFile3(strFN3,ios::out);
	ofstream OFile4(strFN4,ios::out);
	ofstream OFile5(strFN5,ios::out);

	for (j = 1; j <= Cy; j++) {
		 # pragma omp parallel for private (i, C_I) ordered
		for (i = 1; i <= Cx; i++) {
			C_I = (j - 1) * Cx + i;

			# pragma omp ordered
			{
			OFile1 << pGrain[C_I] << "\n";
			OFile2 << pPhase[C_I] << "\n";
			OFile3 << pTemp[C_I] << "\n";
      OFile4 << pCarbon[C_I] << "\n";
			OFile5 << pFraction[C_I]	<< "\n";
			}
		}
	}

	OFile1.close();
	OFile2.close();
	OFile3.close();
	OFile4.close();
	OFile5.close();
}

void Result(int iStep, int Cx, int Cy, int *pGrain, int *pPhase, double *pTemp,
              double *pCarbon, double *pFraction, double time, double GrainNf) {
  int i, j, k;
	int iInterface = 0;
	int iFerrite = 0;
	double Raver = 0.0;
	int FinalGf = 0;
	char str[256];
	int *mGrainF;

  ofstream FFile("Fraction.txt", ios::ate);

	sprintf_s(str, "%s%d%s", "SizeDistribution-", iStep, ".txt");
	ofstream OFile1(str,ios::out);

	ofstream OFile2("AverageGrainSize.txt", ios::ate);

  mGrainF = new int[GrainNf + 1];
	for (i = 1; i <= GrainNf; i++) {
		mGrainF[i] = 0;
	}

	for (j = 1; j <= Cy; j++) {
		for (i = 1; i <= Cx; i++) {
			for(k = 1; k <= GrainNf; k++) {
				if ((pGrain[(j - 1) * Cx + i] == k) && (pPhase[(j - 1) * Cx + i] > 0)) {
					mGrainF[k] = mGrainF[k] + 1;
					continue;
				}
			}
		}
	}

	for (k = 1; k <= GrainNf; k++) {
		if (mGrainF[k] != 0) {
			Raver = Raver + pow((mGrainF[k] * SC / Pi), 0.5);
			FinalGf++;
		}
    OFile1 << pow((mGrainF[k] * SC / Pi), 0.5) <<"\n";     //2D individual ferrite grain radius
	}

	OFile2 << iStep << FinalGf << Raver / FinalGf << "\n";    //ferrite grain number, 2D average Ferrite Grian Radius

  OFile1.close();
  OFile2.close();

	for (j = 1; j <= Cy; j++) {
	  for (i = 1; i <= Cx; i++) {
		  if (pPhase[(j - 1) * Cx + i] == 2) {
				iFerrite = iFerrite + 1;
      }
			if (pPhase[(j - 1) * Cx + i] == 1) {
				iInterface = iInterface + 1;
			}
		}
	}

  FFile << double(iFerrite) / double(Cx * Cy) << "\n";
  FFile.close();
	delete  mGrainF;
}
