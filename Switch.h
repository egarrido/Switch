#include "TFile.h"
#include "TStyle.h"
#include "TSystem.h"
#include "Riostream.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TPad.h"
#include "TGraph.h"
#include "TH1.h"
#include "TF1.h"
#include "TF2.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TRandom.h"
#include "TVirtualFitter.h"
#include "TSpectrum.h"
#include "TMath.h"
#include "TROOT.h"
#include "TColor.h"

#include "TApplication.h"
#include "TDatime.h"
#include "TLine.h"
#include "TText.h"
#include "TPaveText.h"
#include "TLatex.h"
#include "TArrow.h"
#include "TGaxis.h"
#include "TFrame.h"
#include "TVirtualFitter.h"

#include <ctime>
#include <ratio>
#include <chrono>
#include <thread>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <sstream>
#include <fstream>
#include "Riostream.h"

#include "fasterac/fasterac.h"
#include "fasterac/electrometer.h"
#include "fasterac/scaler.h"

std::vector<string> Variable_init;
std::vector<double> Value_init;

std::vector<double> EOFFX;
std::vector<double> EOFFY;

std::vector<double> PreSFB;
std::vector<double> PostSFB;

std::vector<double> PreSFBprim;
std::vector<double> PostSFBprim;

std::vector<double> Integrated_X;
std::vector<double> Integrated_Y;

string data_folder;
string data_faster_tube_file;
string data_faster_out_file;

const int N_box_per_irradation_max=5000;
const int N_STRIPS=32;
const int FIRST_ELEC=1;
const int LAST_ELEC=31;
const int MAX_PEAK=10;
const int MAX_SMPL=1000;
const int MAX_INTEGR=1000000;

// Manip Mai 2014 : X->4 Y->6 Scaler->1009
const int LabelX=4;
const int LabelY=6;
const int LabelScaler=7;
const int LabelCount=1007;
const int LabelHVp=102;
const int LabelHVn=101;
const double strip_width=.3; //cm
const double gap_Dosion=5.05; //mm

int dosedistribution=0;
int bkgnd_param=0;
int fit_param=0;
int twoD_param=0;
int lissage_param=0;
int energy_used=0;
int calibrage_used=0;
int limit_used=0;
int integration_used=0;
int area_find_param=0;
int Strip_label;
int Strip_number;
int borne_m_x=16;
int borne_M_x=17;
int borne_m_y=16;
int borne_M_y=17;
int ind_en;
int ind_cal;
int ind_lim;
double itstimetofinish;
double bound_eoff;
double energy;
double calib_entry;
double refresh_time;
double integration_time;
double multiple_energy[25];
double multiple_calib[25];
double multiple_calib_th[25];
double multiple_limit[25];

// const double SamplingTime=40e-6;  	// 40   us
// const double SamplingTime=DBL_MAX;
// const double SamplingTime=1.;
const double default_offset=0.419;		// 0.418   pC
const double defaultRange_pC=24;    	// 24   pC
const double sizeStrip=0.3; 					// 0.3 cm
const	double ele2pC_coef=4.5777e-05; 
double corrXY[N_STRIPS][2]={{1.00000,1.00000},
														{1.00000,1.00000},
														{1.00000,1.00000},
														{1.00000,1.00000},
														{1.00000,1.00000},
														{1.00000,1.00000},
														{1.00287,1.00000},
														{0.99886,0.97900},
														{0.99496,0.99777},
														{1.00102,1.00786},
														{0.99815,1.00352},
														{1.00640,1.01185},
														{0.99210,1.00963},
														{1.01579,1.00328},
														{0.99180,0.99555},
														{1.00119,1.00284},
														{0.99623,0.99784},
														{0.99712,0.99653},
														{1.00402,0.98870},
														{1.00000,0.99598},
														{1.00000,0.99437},
														{1.00000,0.99987},
														{1.00000,1.01356},
														{1.00000,1.00000},
														{1.00000,1.00000},
														{1.00000,1.00000},
														{1.00000,1.00000},
														{1.00000,1.00000},
														{1.00000,1.00000},
														{1.00000,1.00000},
														{1.00000,1.00000},
														{1.00000,1.00000}};

double offXY[N_STRIPS][2];
double lissage_factor[N_STRIPS][2];

void set_plot_style()
{
	const Int_t NRGBs = 5;
	const Int_t NCont = 100;

	Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
	Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
	Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
	Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
	TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
	gStyle->SetNumberContours(NCont);
	// gStyle->SetPadGridX(kTRUE);
	// gStyle->SetPadGridY(kTRUE);
	gStyle->SetHistMinimumZero();
}

