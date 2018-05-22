#include "Switch.h"

using namespace std;
using namespace std::chrono;

bool CheckDouble(std::string const& s) 
{
  std::istringstream ss(s);
  double d;
  return (ss>>d)&&(ss>>std::ws).eof();
}

void EntryParameters(int config_simu)
{
	stringstream ss;
	ss<<config_simu;
	string indice=ss.str();
	stringstream ss2;
	int n;
	double m;

	int iint;
	int ichar;
	int ivar;
	char next;
	string data_tube;
	string path_tube;
	string data_out;
	string path_out;
	string filename="./Entry/Entry_param_"+indice+".txt";
	// string filename="./Entry/Entry_param_1.txt";
	string tmp;

	Variable_init.clear();
	Value_init.clear();
	Variable_init.push_back("Tube"); // 0
	Value_init.push_back(0.);	//	0
	Variable_init.push_back("Path for the tube"); // 1
	Value_init.push_back(0.);	//	1
	Variable_init.push_back("Output file"); // 2
	Value_init.push_back(0.);	//	2
	Variable_init.push_back("Path for the file"); // 3
	Value_init.push_back(0.);	//	3
	Variable_init.push_back("Background extraction"); // 4
	Value_init.push_back(0.);	//	4
	Variable_init.push_back("Acquisition time for electronic offset (s)"); // 5
	Value_init.push_back(20.);	//	5
	Variable_init.push_back("Fitting function"); // 6
	Value_init.push_back(0.);	//	6
	Variable_init.push_back("Exclusion strips X"); // 7
	Value_init.push_back(0.);	//	7
	Variable_init.push_back("Exclusion strips Y"); // 8
	Value_init.push_back(0.);	//	8
	Variable_init.push_back("Calibrate data"); // 9
	Value_init.push_back(0.);	//	9
	Variable_init.push_back("Particle energy"); // 10
	Value_init.push_back(0.);	//	10
	Variable_init.push_back("Map 2D"); // 11
	Value_init.push_back(0.);	//	11
	Variable_init.push_back("Limit"); // 12
	Value_init.push_back(0.);	//	12
	Variable_init.push_back("Refreshing time (s)"); // 13
	Value_init.push_back(0.);	//	13
	Variable_init.push_back("Smoothing"); // 14
	Value_init.push_back(0.);	//	14
	Variable_init.push_back("Integration"); // 15
	Value_init.push_back(0.);	//	15


	ifstream datafile_param(filename.c_str());
	if(!datafile_param)
	{
		cout<<"No entry parameters file for this configuration: "<<config_simu<<endl;
		cout<<"No file: "<<filename<<endl;
		exit(0);
	}
	else
	{
		while(true)
		{
			string buffer;
			string variable;
			double value;
			getline(datafile_param,tmp);
			for(ichar=0;ichar<tmp.size();ichar++)
			{
				next=tmp[ichar];
				if(next!=':')
					variable+=next;
				else
				{	
					for(iint=ichar+2;iint<tmp.size();iint++)
					{	
						next=tmp[iint];
						buffer+=next;
					}
					ichar=tmp.size();	
				}	
			}
			int ind_value=-1;
			for(ivar=0;ivar<Variable_init.size();ivar++)
			{
				if(!variable.compare(Variable_init[ivar]))
					ind_value=ivar;
			}
			if(ind_value==-1)
				cout<<"Variable "<<variable<<" not defined. Maybe check the entry file."<<endl;
			else
			{
				if(buffer.find_first_not_of(' ')!=std::string::npos)
				{
					if(ind_value==0)
						data_tube=buffer;
					if(ind_value==1)
						path_tube=buffer+data_tube;

					if(ind_value==2)
						data_out=buffer;
					if(ind_value==3)
						path_out=buffer+data_out;

					if(ind_value==4)
					{
						bkgnd_param=0;
						if(CheckDouble(buffer)==1)
						{
							value=(double)atof(buffer.c_str());
							Value_init[ind_value]=value;
							bkgnd_param=1;
						}
						else
						{
							std::size_t found=buffer.find_first_of(' ');
							std::string bufferofbuffer=buffer.substr(0,found);
							if(!bufferofbuffer.compare("yes")||!bufferofbuffer.compare("Yes"))
								bkgnd_param=3;
							if(!bufferofbuffer.compare("no"))
								bkgnd_param=0;
							if(!bufferofbuffer.compare("file"))
							{
								int ind=0;
								int ligne;
								buffer=buffer.substr(found+1);
								cout<<"Multiple offset values stored in file: "<<buffer<<endl;
								ifstream offset_file(buffer.c_str());
								if(!offset_file)
								{
									cout<<"No offset file"<<endl;
									bkgnd_param=0;
								}
								else
								{
									while(true)
									{
										offset_file>>ligne>>offXY[ind][0]>>offXY[ind][1];
										ind++;
										if(offset_file.eof()||ind>N_STRIPS) break;
									}
									offset_file.close();
									bkgnd_param=2;
								}
							}
						}
					}

					if(ind_value==5||ind_value==13)
					{
						value=(double)atof(buffer.c_str());
						if(value==Value_init[ind_value])
							cout<<Variable_init[ind_value]<<" default value keeped: "<<Value_init[ind_value]<<endl;
						else	
							cout<<Variable_init[ind_value]<<" default value: "<<Value_init[ind_value]<<"; new value: "<<value<<endl;
						Value_init[ind_value]=value;
					}

					if(ind_value==6||ind_value==11)
					{
						if(!buffer.compare("no")||!buffer.compare("No"))
							Value_init[ind_value]=false;
						if(!buffer.compare("yes")||!buffer.compare("Yes"))
							Value_init[ind_value]=true;
					}

					if(ind_value==7)
					{
						ss2<<buffer;
						ss2>>n;
						borne_m_x=n;
						ss2>>n;
						borne_M_x=n;
						ss2.clear();
					}
					if(ind_value==8)
					{
						ss2<<buffer;
						ss2>>n;
						borne_m_y=n;
						ss2>>n;
						borne_M_y=n;
						ss2.clear();
					}
				
					if(ind_value==9)
					{
						Value_init[ind_value]=0.;
						calibrage_used=0;
						if(CheckDouble(buffer)==1)
						{
							value=(double)atof(buffer.c_str());
							if(value<=0.)
								cout<<"No valid input data for calibration:"<<value<<endl;
							else
							{
								cout<<Variable_init[ind_value]<<" default value: "<<Value_init[ind_value]<<" fC/part.; new value: "<<value<<" fC/part."<<endl;
								Value_init[ind_value]=value;
								calibrage_used=1;
							}
						}
						else
						{
							std::size_t found=buffer.find_first_of(' ');
							std::string bufferofbuffer=buffer.substr(0,found);
							if(!bufferofbuffer.compare("nothing"))
								cout<<"No input data for calibration"<<endl;
							if(!bufferofbuffer.compare("proton_Cyrce"))
								calibrage_used=2;
							if(!bufferofbuffer.compare("proton_Arronax"))
								calibrage_used=3;
							if(!bufferofbuffer.compare("proton_Arronax_68_MeV"))
								calibrage_used=4;
							if(!bufferofbuffer.compare("multiple"))
							{
								calibrage_used=5;
								cout<<"Multiple calibration values for primary particles"<<endl;
								buffer=buffer.substr(found+1);
								ss2<<buffer;
								ind_cal=0;
								do
								{
									ss2>>multiple_calib[ind_cal];
									ind_cal++;
								}while(!ss2.eof()&&ind_cal<=25);
								ss2.clear();
								// ind_cal--;
								if(ind_cal>0)							
									calibrage_used=5;
								if(ind_cal==1)							
								{
									Value_init[ind_value]=multiple_calib[0];
									calibrage_used=1;
								}
							}
							if(!bufferofbuffer.compare("file"))
							{
								buffer=buffer.substr(found+1);
								cout<<"Multiple calibration values for primary particles stored in file: "<<buffer<<endl;
								ind_cal=0;
								ifstream calib_file(buffer.c_str());
								if(!calib_file)
								{
									cout<<"No calibration file"<<endl;
									calibrage_used=0;
								}
								else
								{
									while(true)
									{
										calib_file>>multiple_calib[ind_cal];
										ind_cal++;
										if(calib_file.eof()||ind_cal>25) break;
									}
									calib_file.close();
									if(ind_cal>0)							
										calibrage_used=5;
									if(ind_cal==1)							
									{
										Value_init[ind_value]=multiple_calib[0];
										calibrage_used=1;
									}
								}
							}
						}
					}

					if(ind_value==10)
					{
						energy_used=0.;
						if(CheckDouble(buffer)==1)
						{
							value=(double)atof(buffer.c_str());
							if(value<=0.)
								cout<<"No valid input data for primary particles energy:"<<value<<endl;
							else
							{
								cout<<Variable_init[ind_value]<<" default value: "<<Value_init[ind_value]<<" MeV; new value: "<<value<<" MeV"<<endl;
								energy=value;
								energy_used=1;
							}
						}
						else
						{
							std::size_t found=buffer.find_first_of(' ');
							std::string bufferofbuffer=buffer.substr(0,found);
							if(!bufferofbuffer.compare("nothing"))
								cout<<"No input data for primary particles energy"<<endl;
							if(!bufferofbuffer.compare("multiple"))
							{
								cout<<"Multiple energy values for primary particles"<<endl;
								buffer=buffer.substr(found+1);
								ss2<<buffer;
								ind_en=0;
								do
								{
									ss2>>multiple_energy[ind_en];
									ind_en++;
								}while(!ss2.eof()&&ind_en<=25);
								ss2.clear();
								// ind_en--;
								if(ind_en>0)
									energy_used=2;
								if(ind_en==1)
								{
									energy=multiple_energy[0];
									energy_used=1;
								}
							}
							if(!bufferofbuffer.compare("file"))
							{
								buffer=buffer.substr(found+1);
								cout<<"Multiple energy values for primary particles stored in file: "<<buffer<<endl;
								ind_en=0;
								ifstream energy_file(buffer.c_str());
								if(!energy_file)
								{
									cout<<"No energy file"<<endl;
									energy_used=0;
								}
								else
								{
									while(true)
									{
										energy_file>>multiple_energy[ind_en];
										ind_en++;
										if(energy_file.eof()||ind_en>25) break;
									}
									energy_file.close();
									if(ind_en>0)
										energy_used=2;
									if(ind_en==1)
									{
										energy=multiple_energy[0];
										energy_used=1;
									}
								}							
							}
						}
					}

					if(ind_value==12)
					{
						limit_used=0.;
						std::size_t found=buffer.find_first_of(' ');
						std::string bufferofbuffer=buffer.substr(0,found);
						if(!bufferofbuffer.compare("nothing"))
							cout<<"No limit defined"<<endl;
						else
						{
							if(CheckDouble(bufferofbuffer)==1)
							{
								itstimetofinish=(double)atof(bufferofbuffer.c_str());
								limit_used=1;
							}
							if(!bufferofbuffer.compare("file"))
								limit_used=2;
							buffer=buffer.substr(found+1);
							found=buffer.find_first_of(' ');
							bufferofbuffer=buffer.substr(0,found);
							if(!bufferofbuffer.compare("s")||!bufferofbuffer.compare("sec"))
							{
								if(limit_used==1)
									limit_used=11;
								if(limit_used==2)
									limit_used=21;
							}
							if(!bufferofbuffer.compare("Gy")||!bufferofbuffer.compare("gray"))
							{
								if(limit_used==1)
									limit_used=12;
								if(limit_used==2)
									limit_used=22;
							}
							if(!bufferofbuffer.compare("pC"))
							{
								if(limit_used==1)
									limit_used=13;
								if(limit_used==2)
									limit_used=23;
							}
							if(limit_used==1)
								limit_used=12;
							if(limit_used==2)
								limit_used=22;
							if(limit_used>20)
							{
								buffer=buffer.substr(found+1);
								cout<<"Multiple limit values stored in file: "<<buffer<<endl;
								ind_lim=0;
								ifstream limit_file(buffer.c_str());
								if(!limit_file)
								{
									cout<<"No limit file"<<endl;
									limit_used=0;
								}
								else
								{
									while(true)
									{
										limit_file>>multiple_limit[ind_lim];
										ind_lim++;
										if(limit_file.eof()||ind_lim>25) break;
									}
									limit_file.close();
									if(ind_lim==0)
											limit_used=0;
									if(ind_lim==1)
									{
										itstimetofinish=multiple_limit[0];
										limit_used-=10;
									}
								}							
							}
						}
					}

					if(ind_value==14)
					{
						lissage_param=0;
						std::size_t found=buffer.find_first_of(' ');
						std::string bufferofbuffer=buffer.substr(0,found);
						if(!bufferofbuffer.compare("no"))
							lissage_param=0;
						if(!bufferofbuffer.compare("yes")||!bufferofbuffer.compare("Yes"))
						{
							int ind=0;
							buffer=buffer.substr(found+1);
							cout<<"Smoothing values stored in file: "<<buffer<<endl;
							ifstream smooth_file(buffer.c_str());
							if(!smooth_file)
							{
								cout<<"No smoothing file"<<endl;
								lissage_param=0;
							}
							else
							{
								while(true)
								{
									smooth_file>>lissage_factor[ind][0]>>lissage_factor[ind][1];
									ind++;
									if(smooth_file.eof()||ind>N_STRIPS) break;
								}
								smooth_file.close();
								lissage_param=1;
							}
						}
					}

					if(ind_value==15)
					{
						integration_used=false;
						std::size_t found=buffer.find_first_of(' ');
						std::string bufferofbuffer=buffer.substr(0,found);
						if(!bufferofbuffer.compare("no"))
							cout<<"No integration used"<<endl;
						else
						{
							if(CheckDouble(bufferofbuffer)==1)
							{
								integration_time=(double)atof(bufferofbuffer.c_str());
								buffer=buffer.substr(found+1);
								found=buffer.find_first_of(' ');
								bufferofbuffer=buffer.substr(0,found);
								if(!bufferofbuffer.compare("ms"))
								{
									integration_time*=1.E-3;
									integration_used=true;
								}
								if(!bufferofbuffer.compare("s")||!bufferofbuffer.compare("sec"))
								{
									integration_time*=1.;
									integration_used=true;
								}
								if(integration_used==false)
								{
									cout<<"No or unknown unit; seconds used by default"<<endl;
									integration_time*=1.;
									integration_used=true;
								}
							}
						}
					}

				}	
				else
					cout<<"Value not define for "<<variable<<". Keeping the default one: "<<Value_init[ind_value]<<". Maybe check the entry file."<<endl;
			}	
			if(datafile_param.eof()) break;
		}
		datafile_param.close();
	}	

	if(borne_m_x>borne_M_x||borne_m_x<0||borne_m_x>32||borne_M_x<0||borne_M_x>32)
	{
		cout<<"Error with strips exclusion for X, "<<borne_m_x<<" - "<<borne_M_x<<" default values used"<<endl;
		borne_m_x=16;
		borne_M_x=17;
	}
	if(borne_m_y>borne_M_y||borne_m_y<0||borne_m_y>32||borne_M_y<0||borne_M_y>32)
	{
		cout<<"Error with strips exclusion for Y, "<<borne_m_y<<" - "<<borne_M_y<<" default values used"<<endl;
		borne_m_y=16;
		borne_M_y=17;
	}

	cout<<endl;
	cout<<"=================================================="<<endl;
	cout<<" Parameters for the initialisation; config: "<<config_simu<<endl;
	cout<<"--------------------------------------------------"<<endl;

	cout<<" Gap size: "<<setprecision(2)<<fixed<<gap_Dosion<<" mm"<<endl;

	cout<<" Path to the tube: "<<path_tube<<endl;
	cout<<" Output file: "<<path_out<<endl;
	data_faster_tube_file=path_tube;
	data_faster_out_file=path_out;

	ivar=4;
	if(bkgnd_param==0)
		cout<<" No background extraction"<<endl;
	if(bkgnd_param==1)
		cout<<" Substraction of a constant value: "<<setprecision(2)<<fixed<<Value_init[ivar]<<" pC"<<endl;
	if(bkgnd_param==2)
	{
		cout<<" Background stored in file"<<endl;
		cout<<"  St.| X(pC) | Y(pC) "<<endl;
		cout<<" ====|=======|======="<<endl;
		for(int i=0;i<N_STRIPS;i++)
			cout<<"  "<<std::setw(2)<<std::setfill('0')<<i+1<<" | "<<setprecision(2)<<fixed<<offXY[i][0]<<" | "<<offXY[i][1]<<endl;
		cout<<" --------------------"<<endl;
	}
	if(bkgnd_param==3)
	{
		bound_eoff=Value_init[5];
		cout<<" Background extraction during the first "<<bound_eoff<<" seconds"<<endl;
	}

	ivar=9;
	dosedistribution=true;
	if(calibrage_used==5)
	{
		cout<<" Multiple calibration values: "<<ind_cal<<" values"<<endl;
		for(int ii=0;ii<ind_cal;ii++)
			cout<<" "<<setprecision(5)<<fixed<<multiple_calib[ii]<<" fC/part."<<endl;
	}	
	if(calibrage_used==0&&energy_used==0)
	{
		cout<<" No calibration and energy values"<<endl;
		dosedistribution=false;
	}
	if(calibrage_used==0&&energy_used==1)
	{
		cout<<" Calibration from energy used"<<endl;
		calibrage_used=6;
	}
	if(calibrage_used==0&&energy_used==2)
	{
		cout<<" Calibration from energies used"<<endl;
		calibrage_used=6;
	}
	calib_entry=Value_init[ivar];
	if(calibrage_used==1)
		cout<<" Calibration value: "<<setprecision(5)<<fixed<<calib_entry<<" fC/part."<<endl;
	if(calibrage_used==2)
		cout<<" Cyrce calibration values used"<<endl;
	if(calibrage_used==3)
		cout<<" Arronax calibration values used"<<endl;
	if(calibrage_used==4)
		cout<<" Arronax calibration value used for 68 MeV protons"<<endl;

	ivar=10;
	if(energy_used==2)
	{
		cout<<" Multiple energies for primary particles: "<<ind_en<<" values"<<endl;
		for(int ii=0;ii<ind_en;ii++)
			cout<<" "<<setprecision(2)<<fixed<<multiple_energy[ii]<<" MeV"<<endl;
	}	
	if(energy_used==0.)
	{
		cout<<" No energy for the primary particles"<<endl;
		dosedistribution=false;
	}
	if(energy_used==1)
		cout<<" Energy of the particles: "<<setprecision(2)<<fixed<<energy<<" MeV"<<endl;

	if(ind_en!=ind_cal)
	{
		cout<<" Be aware that the numbers of calibration and energy values are different"<<endl;
		if(ind_cal>ind_en)
			dosedistribution=false;
		else
		{
			cout<<" Calibration values complete with energie values"<<endl;
			calibrage_used=7;
		}
	}

	ivar=6;
	if(Value_init[ivar]==true)
	{
		cout<<" Fitting function used"<<endl;
		cout<<" Exclusion strips for X: "<<borne_m_x<<" - "<<borne_M_x<<endl;
		cout<<" Exclusion strips for Y: "<<borne_m_y<<" - "<<borne_M_y<<endl;
		fit_param=true;
	}
	else
	{
		cout<<" No fitting function"<<endl;
		fit_param=false;
	}

	ivar=11;
	if(Value_init[ivar]==true)
	{
		cout<<" Two-dimensional map used"<<endl;
		twoD_param=true;
	}
	else
	{
		cout<<" Two-dimensional map not used"<<endl;
		twoD_param=false;
	}

	ivar=13;
	cout<<" "<<Variable_init[ivar]<<": "<<setprecision(2)<<fixed<<Value_init[ivar]<<endl;
	refresh_time=Value_init[ivar];

	ivar=12;
	string unit;
	if(limit_used==0)
		cout<<" No limit defined, so he can fly away until the end of time"<<endl;
	if(limit_used==11||limit_used==21)
		unit="sec";
	if(limit_used==12||limit_used==22)
		unit="Gy";
	if(limit_used==13||limit_used==23)
		unit="pC";
	if(limit_used<20&&limit_used!=0)
		cout<<" Limit defined at: "<<setprecision(2)<<fixed<<itstimetofinish<<" "<<unit<<endl;
	if(limit_used>20)
		for(int i=0;i<ind_lim;i++)
			cout<<" Limit "<<setw(2)<<setfill('0')<<i+1<<": "<<setprecision(2)<<fixed<<multiple_limit[i]<<" "<<unit<<endl;

	ivar=14;
	if(lissage_param==0)
		cout<<" No smoothing applied"<<endl;
	if(lissage_param==1)
		cout<<" Smoothing applied"<<endl;

	ivar=15;
	if(integration_used==false)
		cout<<" No time integration used"<<endl;
	if(integration_used==true)
		cout<<" Time integration of "<<setprecision(2)<<fixed<<integration_time<<" sec used"<<endl;

	// // for(ivar=4;ivar<Variable_init.size();ivar++)
	// // 	cout<<" "<<Variable_init[ivar]<<": "<<Value_init[ivar]<<endl;
	cout<<"=================================================="<<endl;
	cout<<endl;

}

void SubFittingBackground(int binl,int binr,double *sum_val)
{
	// int binl=12;
	// int binr=23;
	int i_par=9;
	double scale_value;
	double bdf_SFB;
	double par[i_par];
	double y_min;
	double y_max;

	TH1F* Profile=new TH1F("Profile","Profile in charge",N_STRIPS,.5,32.5);
	TH1F* ProfExc=new TH1F("ProfExc","Profile in charge without excluded points",N_STRIPS,1,33);
	TGraph *TG_Prof_Exc=new TGraph();
	TGraph *TG_Post=new TGraph();
	TF1* PolNProfile=new TF1("PolNProfile","pol8",2,31);

	for(int i=FIRST_ELEC;i<LAST_ELEC;i++)
	{
		scale_value=PreSFB.at(i);
		Profile->SetBinContent(i+1,PreSFB.at(i));
		if(i+1<=binl||i+1>=binr)
		{
			ProfExc->SetBinContent(i+1,scale_value);
			TG_Prof_Exc->SetPoint(i,i+1,scale_value);
		}
	}
	ProfExc->Fit(PolNProfile,"Q");

	for(int ii=0;ii<i_par;ii++)
		par[ii]=PolNProfile->GetParameter(ii);

	*sum_val=0.;
	for(int i=FIRST_ELEC;i<LAST_ELEC;i++)
	{
		double x=i+1.5;
		// PostSFBprim.at(i)=PostSFB.at(i);
		bdf_SFB=0.;
		for(int ii=0;ii<i_par;ii++)
			bdf_SFB+=par[ii]*pow(x,ii);
		PostSFB.at(i)=PreSFB.at(i)-bdf_SFB;
		if(i+1>binl&&i+1<binr)
			*sum_val+=PostSFB.at(i);
		// *sum_val=TMath::Max(*sum_val,0.);
	}

	Profile->Delete();
	ProfExc->Delete();
	PolNProfile->Delete();
	TG_Prof_Exc->Delete();
	TG_Post->Delete();
}

int main(int argc, char** argv)
{
	double temps=0.;
	time_t t1,t2;
	long long microseconds;
	long long tot_prog_milli=0;
	long long tot_fasterTime_milli=0;
	high_resolution_clock::time_point hct0;
	high_resolution_clock::time_point hct1;
	high_resolution_clock::time_point hct2;

	set_plot_style();
	TApplication *application=new TApplication("Visualisation",&argc,argv);
	cout.precision(8);
	int ii;
	
	faster_file_reader_p reader;
	faster_file_writer_p writer;
	faster_data_p data;
	electrometer_data electro;

	EOFFX.clear();
	EOFFX.resize(N_STRIPS);
	EOFFY.clear();
	EOFFY.resize(N_STRIPS);
	PreSFB.clear();
	PreSFB.resize(N_STRIPS);
	PostSFB.clear();
	PostSFB.resize(N_STRIPS);
	PreSFBprim.clear();
	PreSFBprim.resize(N_STRIPS);
	PostSFBprim.clear();
	PostSFBprim.resize(N_STRIPS);
	Integrated_X.clear();
	Integrated_X.resize(N_STRIPS);
	Integrated_Y.clear();
	Integrated_Y.resize(N_STRIPS);

	double* vect_time_tot=(double*)malloc(MAX_INTEGR*sizeof(double));
	double* vect_charge_cumul=(double*)malloc(MAX_INTEGR*sizeof(double));

	bool bckg;
	bool brkg_condition=false;
	unsigned short label;
	int count=0;
	int count_foireux=0;
	int count_eoffx=0;
	int count_eoffy=0;
	int isLabelX=0;
	int isLabelY=0;
	int integration=400;
	int binr;
	int binl;
	double t0=-1.;
	double t_last_refresh=-1.;
	double t_last_int=0.;
	double calib=1.;
	double fasterTime=0.;
	double inter_fasterTime;
	double last_fasterTime;
	double time_limit;
	double dose_limit=1000.;
	double charge;
	double val;
	double sum_val=0.;
	double charge_limite=0.;
	double charge_totale=0.;
	double pixel;

	int visu_param=0;

	EntryParameters(1);

	reader=faster_file_reader_open(data_faster_tube_file.c_str());

	if(reader==NULL) 
	{
		printf ("Error opening tube file %s\n", data_faster_tube_file.c_str());
		faster_file_reader_close(reader);
		exit(0);
	}
	else
		printf("Tube de données bien ouvert\n");

	Int_t w_canvas,h_canvas;
	Int_t size_unit=250;
	w_canvas=250;
	h_canvas=250;
	Double_t x0,x1,x2,x3,x4,y0,y1,y2,y3,it;
	Double_t xt,yt,ydecal;
	Double_t margin_l,margin_r,margin_t,margin_b;
	TString text_tmp;
	TCanvas * cVisu = new TCanvas();
	TPad * pad0 = new TPad();
	TPad * pad1 = new TPad();
	TPad * pad2 = new TPad();
	TPad * pad3 = new TPad();
	TPad * pad4 = new TPad();
	TPad * pad5 = new TPad();
	TPad * pad6 = new TPad();

	TCanvas * cInte = new TCanvas();
	TPad * pad_int0 = new TPad();
	TPad * pad_int1 = new TPad();
	TPad * pad_int2 = new TPad();
	TPad * pad_int3 = new TPad();

	if(integration_used==true)
	{
		cout<<"Fenêtre intégrale"<<endl;
		w_canvas=4*size_unit;
		h_canvas=2*size_unit;
		cInte->Destructor();
		cInte->Constructor("Integrale","",w_canvas,h_canvas);
		// cInte->SetCanvasSize(w_canvas,h_canvas);
		cInte->SetWindowSize(w_canvas*1.03,h_canvas*1.03);
		x0=0.;	y0=0.;	x1=.33;	y1=.5;	x2=.84;	y2=1.;	x3=1.;
		it=.00;
		pad_int0->SetCanvas(cInte);
		pad_int1->SetCanvas(cInte);
		pad_int2->SetCanvas(cInte);
		pad_int3->SetCanvas(cInte);
		pad_int0->SetPad("pad_int0","pad_int0",x0+it,y0+it,x1-it,y1-it);
		pad_int1->SetPad("pad_int1","pad_int1",x0+it,y1+it,x1-it,y2-it);
		pad_int2->SetPad("pad_int2","pad_int2",x1+it,y0+it,x2-it,y2-it);
		pad_int3->SetPad("pad_int3","pad_int3",x2+it,y0+it,x3-it,y2-it);
		pad_int0->SetFillColor(0);
		pad_int1->SetFillColor(0);
		pad_int2->SetFillColor(0);
		pad_int3->SetFillColor(0);

		pad_int0->Draw();
		pad_int1->Draw();
		pad_int2->Draw();
		pad_int3->Draw();
		cInte->Update();
	}
	else
		cInte->Destructor();


	if(fit_param==false&&twoD_param==false)
	{
		visu_param=1;
		cout<<"Visualisation 1"<<endl;
		w_canvas=3*size_unit;
		h_canvas=2*size_unit;
		cVisu->Destructor();
		cVisu->Constructor("Visualisation","",w_canvas,h_canvas);
		// cVisu->SetCanvasSize(w_canvas,h_canvas);
		cVisu->SetWindowSize(w_canvas*1.03,h_canvas*1.03);
		x0=0.;	y0=0.;	x1=.4;	y1=.5;	x2=.8;	y2=1.;	x3=1.;
		it=.00;
		pad0->SetCanvas(cVisu);
		pad1->SetCanvas(cVisu);
		pad2->SetCanvas(cVisu);
		pad3->SetCanvas(cVisu);
		pad0->SetPad("pad0","pad0",x0+it,y0+it,x2-it,y1-it);
		pad1->SetPad("pad1","pad1",x0+it,y1+it,x1-it,y2-it);
		pad2->SetPad("pad2","pad2",x1+it,y1+it,x2-it,y2-it);
		pad3->SetPad("pad3","pad3",x2+it,y0+it,x3-it,y2-it);
		pad0->SetFillColor(0);
		pad1->SetFillColor(0);
		pad2->SetFillColor(0);
		pad3->SetFillColor(0);

		margin_r=0.02;	margin_l=0.02;	
		margin_t=0.015;	margin_b=0.015;

		pad0->SetLeftMargin(pad1->GetLeftMargin()*x1/x2);
		pad0->SetRightMargin(pad2->GetLeftMargin()*(x2-x1)/x2);
		pad1->SetRightMargin(margin_r);
		pad2->SetLeftMargin(margin_l);

		pad0->Draw();
		pad1->Draw();
		pad2->Draw();
		pad3->Draw();
		cVisu->Update();
	}
	
	if(fit_param!=false&&twoD_param==false)
	{
		visu_param=2;
		cout<<"Visualisation 2"<<endl;
		w_canvas=4*size_unit;
		h_canvas=3*size_unit;
		cVisu->Destructor();
		cVisu->Constructor("Visualisation","",w_canvas,h_canvas);
		// cVisu->SetCanvasSize(w_canvas,h_canvas);
		cVisu->SetWindowSize(w_canvas*1.03,h_canvas*1.03);
		x0=0.;	y0=0.;	x1=.4;	y1=.33;	x2=.8;	y2=.66;	x3=1.;	y3=1.;
		it=.00;
		pad0->SetCanvas(cVisu);
		pad1->SetCanvas(cVisu);
		pad2->SetCanvas(cVisu);
		pad3->SetCanvas(cVisu);
		pad4->SetCanvas(cVisu);
		pad5->SetCanvas(cVisu);
		pad0->SetPad("pad0","pad0",x0+it,y0+it,x2-it,y1-it);
		pad1->SetPad("pad1","pad1",x0+it,y2+it,x1-it,y3-it);
		pad2->SetPad("pad2","pad2",x1+it,y2+it,x2-it,y3-it);
		pad3->SetPad("pad3","pad3",x2+it,y0+it,x3-it,y3-it);
		pad4->SetPad("pad4","pad4",x0+it,y1+it,x2-it,y2-it);
		pad5->SetPad("pad5","pad5",x0+it,y1+it,x2-it,y2-it);
		pad0->SetFillColor(0);
		pad1->SetFillColor(0);
		pad2->SetFillColor(0);
		pad3->SetFillColor(0);
		pad4->SetFillColor(0);
		pad5->SetFillColor(0);

		margin_r=0.02;	margin_l=0.02;	
		margin_t=0.015;	margin_b=0.015;
		pad1->SetBottomMargin(margin_b);
		pad2->SetBottomMargin(margin_b);
		pad4->SetTopMargin(margin_t);
		pad5->SetTopMargin(margin_t);

		pad0->SetLeftMargin(pad1->GetLeftMargin()*x1/x2);
		pad0->SetRightMargin(pad2->GetLeftMargin()*(x2-x1)/x2);
		pad1->SetRightMargin(margin_r);
		pad4->SetRightMargin(margin_r);
		pad2->SetLeftMargin(margin_l);
		pad5->SetLeftMargin(margin_l);

		pad0->Draw();
		pad1->Draw();
		pad2->Draw();
		pad3->Draw();
		pad4->Draw();
		pad5->Draw();
		cVisu->Update();
	}
	
	if(fit_param==false&&twoD_param!=false)
	{
		visu_param=3;
		cout<<"Visualisation 3"<<endl;
		w_canvas=5*size_unit;
		h_canvas=2*size_unit;
		cVisu->Destructor();
		cVisu->Constructor("Visualisation","",w_canvas,h_canvas);
		// cVisu->SetCanvasSize(w_canvas,h_canvas);
		cVisu->SetWindowSize(w_canvas*1.03,h_canvas*1.03);
		x0=0.;	y0=0.;	x1=.25;	y1=.5;	x2=.5;	y2=1.;	x3=.875;	x4=1.;
		it=.00;
		pad0->SetCanvas(cVisu);
		pad1->SetCanvas(cVisu);
		pad2->SetCanvas(cVisu);
		pad3->SetCanvas(cVisu);
		pad6->SetCanvas(cVisu);
		pad0->SetPad("pad0","pad0",x0+it,y0+it,x2-it,y1-it);
		pad1->SetPad("pad1","pad1",x0+it,y1+it,x1-it,y2-it);
		pad2->SetPad("pad2","pad2",x1+it,y1+it,x2-it,y2-it);
		pad3->SetPad("pad3","pad3",x3+it,y0+it,x4-it,y2-it);
		pad6->SetPad("pad6","pad6",x2+it,y0+it,x3-it,y2-it);
		pad0->SetFillColor(0);
		pad1->SetFillColor(0);
		pad2->SetFillColor(0);
		pad3->SetFillColor(0);
		pad6->SetFillColor(0);

		margin_r=0.02;	margin_l=0.02;	
		margin_t=0.015;	margin_b=0.015;

		pad0->SetLeftMargin(pad1->GetLeftMargin()*x1/x2);
		pad0->SetRightMargin(pad2->GetLeftMargin()*(x2-x1)/x2);
		pad1->SetRightMargin(margin_r);
		pad2->SetLeftMargin(margin_l);

		pad0->Draw();
		pad1->Draw();
		pad2->Draw();
		pad3->Draw();
		pad6->Draw();
		cVisu->Update();
	}
	
	if(fit_param!=false&&twoD_param!=false)
	{
		visu_param=4;
		cout<<"Visualisation 4"<<endl;
		w_canvas=5*size_unit;
		h_canvas=3*size_unit;
		cVisu->Destructor();
		cVisu->Constructor("Visualisation","",w_canvas,h_canvas);
		// cVisu->SetCanvasSize(w_canvas,h_canvas);
		cVisu->SetWindowSize(w_canvas*1.03,h_canvas*1.03);
		x0=0.;	y0=0.;	x1=.25;	y1=.33;	x2=.5;	y2=.66;	x3=.875;	y3=1.;	x4=1.;
		it=.00;
		pad0->SetCanvas(cVisu);
		pad1->SetCanvas(cVisu);
		pad2->SetCanvas(cVisu);
		pad3->SetCanvas(cVisu);
		pad4->SetCanvas(cVisu);
		pad5->SetCanvas(cVisu);
		pad6->SetCanvas(cVisu);
		pad0->SetPad("pad0","pad0",x0+it,y0+it,x3-it,y1-it);
		pad1->SetPad("pad1","pad1",x0+it,y2+it,x1-it,y3-it);
		pad2->SetPad("pad2","pad2",x1+it,y2+it,x2-it,y3-it);
		pad3->SetPad("pad3","pad3",x3+it,y0+it,x4-it,y3-it);
		pad4->SetPad("pad4","pad4",x0+it,y1+it,x1-it,y2-it);
		pad5->SetPad("pad5","pad5",x1+it,y1+it,x2-it,y2-it);
		pad6->SetPad("pad6","pad6",x2+it,y1+it,x3-it,y3-it);
		pad0->SetFillColor(0);
		pad1->SetFillColor(0);
		pad2->SetFillColor(0);
		pad3->SetFillColor(0);
		pad4->SetFillColor(0);
		pad5->SetFillColor(0);
		pad6->SetFillColor(0);

		margin_r=0.02;	margin_l=0.02;	
		margin_t=0.015;	margin_b=0.015;
		pad1->SetBottomMargin(margin_b);
		pad2->SetBottomMargin(margin_b);
		pad4->SetTopMargin(margin_t);
		pad5->SetTopMargin(margin_t);

		pad0->SetLeftMargin(pad1->GetLeftMargin()*x1/x3);
		pad0->SetRightMargin(pad6->GetLeftMargin()*(x3-x2)/x3);
		pad1->SetRightMargin(margin_r);
		pad4->SetRightMargin(margin_r);
		pad2->SetLeftMargin(margin_l);
		pad5->SetLeftMargin(margin_l);

		pad0->Draw();
		pad1->Draw();
		pad2->Draw();
		pad3->Draw();
		pad4->Draw();
		pad5->Draw();
		pad6->Draw();
		cVisu->Update();
	}
	if(visu_param==0)
		exit(0);
				
	TH2F* h2DMap=new TH2F("h2DMap","Map 2D",N_STRIPS,.5,32.5,N_STRIPS,.5,32.5);
	TH1F* hXbefore=new TH1F("hXbefore","Profile in X",N_STRIPS,.5,32.5);
	TH1F* hYbefore=new TH1F("hYbefore","Profile in Y",N_STRIPS,.5,32.5);
	TH1F* hXafter=new TH1F("hXafter","",N_STRIPS,.5,32.5);
	TH1F* hYafter=new TH1F("hYafter","",N_STRIPS,.5,32.5);
	TGraph* TG_ChargeTime=new TGraph();
	TText* ChargePC=new TText();
	TText* Texte=new TText();
	TLine* fit_line=new TLine();
	TLine* stop_line=new TLine();

	TH2F* h2DMap_int=new TH2F("h2DMap_int","Map 2D integrale",N_STRIPS,.5,32.5,N_STRIPS,.5,32.5);
	TH1F* hXint=new TH1F("hXint","Profile integrale in X",N_STRIPS,.5,32.5);
	TH1F* hYint=new TH1F("hYint","Profile integrale in Y",N_STRIPS,.5,32.5);

	fit_line->SetLineColor(6);
	fit_line->SetLineWidth(1.5);
	fit_line->SetLineStyle(2);

	pad1->cd();
	hXbefore->SetFillColor(2);
	hXbefore->GetXaxis()->SetNdivisions(N_STRIPS);
	hXbefore->GetXaxis()->SetTickSize(0.01);
	hXbefore->GetXaxis()->SetLabelSize(0.0);
	hXbefore->GetYaxis()->CenterTitle();
	hXbefore->GetYaxis()->SetTickSize(0.01);
	hXbefore->GetYaxis()->SetTitleSize(0.0);
	hXbefore->GetYaxis()->SetLabelSize(0.035);
	hXbefore->SetBarWidth(0.8);
	hXbefore->SetBarOffset(0.1);
	hXbefore->SetStats(0);
	hXbefore->Draw("b");
	if(visu_param==1||visu_param==3)
		hXbefore->GetXaxis()->SetTitle("Strip");

	pad2->cd();
	hYbefore->SetFillColor(2);
	hYbefore->GetXaxis()->SetNdivisions(N_STRIPS);
	hYbefore->GetXaxis()->SetTickSize(0.01);
	hYbefore->GetXaxis()->SetLabelSize(0.0);
	hYbefore->GetYaxis()->CenterTitle();
	hYbefore->GetYaxis()->SetTickSize(0.01);
	hYbefore->GetYaxis()->SetTitleSize(0.0);
	hYbefore->GetYaxis()->SetLabelSize(0.035);
	hYbefore->SetBarWidth(0.8);
	hYbefore->SetBarOffset(0.1);
	hYbefore->SetStats(0);
	hYbefore->Draw("b");
	if(visu_param==1||visu_param==3)
		hYbefore->GetXaxis()->SetTitle("Strip");

	
	if(visu_param==2||visu_param==4)
	{
		pad4->cd();
		hXafter->SetFillColor(4);
		hXafter->GetXaxis()->SetNdivisions(N_STRIPS);
		hXafter->GetXaxis()->SetTitle("Strip");
		hXafter->GetXaxis()->SetTitleOffset(0.65);
		hXafter->GetXaxis()->CenterTitle();
		hXafter->GetXaxis()->SetTickSize(0.01);
		hXafter->GetXaxis()->SetTitleSize(0.06);
		hXafter->GetXaxis()->SetLabelSize(0.035);
		hXafter->GetYaxis()->CenterTitle();
		hXafter->GetYaxis()->SetTickSize(0.01);
		hXafter->GetYaxis()->SetTitleSize(0.0);
		hXafter->GetYaxis()->SetLabelSize(0.035);
		hXafter->SetBarWidth(0.8);
		hXafter->SetBarOffset(0.1);
		hXafter->SetStats(0);
		hXafter->Draw("b");

		pad5->cd();
		hYafter->SetFillColor(4);
		hYafter->GetXaxis()->SetNdivisions(N_STRIPS);
		hYafter->GetXaxis()->SetTitle("Strip");
		hYafter->GetXaxis()->SetTitleOffset(0.65);
		hYafter->GetXaxis()->CenterTitle();
		hYafter->GetXaxis()->SetTickSize(0.01);
		hYafter->GetXaxis()->SetTitleSize(0.06);
		hYafter->GetXaxis()->SetLabelSize(0.035);
		hYafter->GetYaxis()->CenterTitle();
		hYafter->GetYaxis()->SetTickSize(0.01);
		hYafter->GetYaxis()->SetTitleSize(0.0);
		hYafter->GetYaxis()->SetLabelSize(0.035);
		hYafter->SetBarWidth(0.8);
		hYafter->SetBarOffset(0.1);
		hYafter->SetStats(0);
		hYafter->Draw("b");
	}

	if(visu_param==3||visu_param==4)
	{
		pad6->cd();
		h2DMap->SetTitle("");//Fluency map (particle/cm2)");
		h2DMap->SetTitleSize(0.0);
		h2DMap->SetStats(0);
		h2DMap->GetXaxis()->SetTitle("Channel X");
		h2DMap->GetXaxis()->SetNdivisions(N_STRIPS);
		h2DMap->GetXaxis()->SetTickSize(0.01);
		h2DMap->GetXaxis()->SetTitleSize(0.036);
		h2DMap->GetXaxis()->CenterTitle();
		h2DMap->GetXaxis()->SetLabelSize(0.02);
		h2DMap->GetYaxis()->SetTitle("Channel Y");
		h2DMap->GetYaxis()->SetNdivisions(N_STRIPS);
		h2DMap->GetYaxis()->SetTickSize(0.01);
		h2DMap->GetYaxis()->SetTitleSize(0.036);
		h2DMap->GetYaxis()->CenterTitle();
		h2DMap->GetYaxis()->SetLabelSize(0.02);
		h2DMap->Draw("colz");
	}

	if(integration_used==true)
	{
		// cInte->cd();
		pad_int0->cd();
		hXint->SetFillColor(2);
		hXint->GetXaxis()->SetNdivisions(N_STRIPS);
		hXint->GetXaxis()->SetTickSize(0.01);
		hXint->GetXaxis()->SetLabelSize(0.0);
		hXint->GetYaxis()->CenterTitle();
		hXint->GetYaxis()->SetTickSize(0.01);
		hXint->GetYaxis()->SetTitleSize(0.0);
		hXint->GetYaxis()->SetLabelSize(0.035);
		hXint->SetBarWidth(0.8);
		hXint->SetBarOffset(0.1);
		hXint->SetStats(0);
		hXint->Draw("b");
		
		pad_int1->cd();
		hYint->SetFillColor(2);
		hYint->GetXaxis()->SetNdivisions(N_STRIPS);
		hYint->GetXaxis()->SetTickSize(0.01);
		hYint->GetXaxis()->SetLabelSize(0.0);
		hYint->GetYaxis()->CenterTitle();
		hYint->GetYaxis()->SetTickSize(0.01);
		hYint->GetYaxis()->SetTitleSize(0.0);
		hYint->GetYaxis()->SetLabelSize(0.035);
		hYint->SetBarWidth(0.8);
		hYint->SetBarOffset(0.1);
		hYint->SetStats(0);
		hYint->Draw("b");

		pad_int2->cd();
		h2DMap_int->SetTitle("");//Fluency map (particle/cm2)");
		h2DMap_int->SetTitleSize(0.0);
		h2DMap_int->SetStats(0);
		h2DMap_int->GetXaxis()->SetTitle("Channel X");
		h2DMap_int->GetXaxis()->SetNdivisions(N_STRIPS);
		h2DMap_int->GetXaxis()->SetTickSize(0.01);
		h2DMap_int->GetXaxis()->SetTitleSize(0.036);
		h2DMap_int->GetXaxis()->CenterTitle();
		h2DMap_int->GetXaxis()->SetLabelSize(0.02);
		h2DMap_int->GetYaxis()->SetTitle("Channel Y");
		h2DMap_int->GetYaxis()->SetNdivisions(N_STRIPS);
		h2DMap_int->GetYaxis()->SetTickSize(0.01);
		h2DMap_int->GetYaxis()->SetTitleSize(0.036);
		h2DMap_int->GetYaxis()->CenterTitle();
		h2DMap_int->GetYaxis()->SetLabelSize(0.02);
		h2DMap_int->Draw("colz");
	}

	writer=faster_file_writer_open(data_faster_out_file.c_str());

	bckg=false;
	brkg_condition=false;
	count_eoffx=0;
	count_eoffy=0;

	t1=time(NULL);
	hct0=high_resolution_clock::now();
	hct1=high_resolution_clock::now();

	while((data=faster_file_reader_next(reader))!=NULL) 
	// while((data=faster_file_reader_next(reader))!=NULL&&brkg_condition==false) 
	{
		faster_file_writer_next(writer,data);
		if(t0==-1)
		{
			t0=faster_data_clock_sec(data);
			last_fasterTime=t0;
		}
		fasterTime=faster_data_clock_sec(data)-t0;
		if(t_last_refresh==-1)
			t_last_refresh=fasterTime;
		sum_val=0.;
		label=faster_data_label(data);
		if(label==LabelX||label==LabelY)
		{
			for(int j=FIRST_ELEC;j<LAST_ELEC;j++) 
			{
				PreSFBprim.at(j)=PreSFB.at(j);
				PostSFBprim.at(j)=PostSFB.at(j);
				faster_data_load(data,&electro);
				charge=electrometer_channel_charge_pC(electro,j+1);
				PreSFB.at(j)=charge;
				switch(label) 
				{
					case LabelX:
						binl=borne_m_x;
						binr=borne_M_x;
						if(!bckg)
						{
							EOFFX.at(j)+=charge;
							if(j==FIRST_ELEC)
								count_eoffx++;
							sum_val=0.;
							PostSFB.at(j)=0.;
						}
						else
						{
							if(bkgnd_param!=false)
								PreSFB.at(j)=charge-offXY[j][0];
							sum_val+=PreSFB.at(j);
							PostSFB.at(j)=PreSFB.at(j);
						}
						isLabelX=1;
					break;

					case LabelY:
						binl=borne_m_y;
						binr=borne_M_y;
						if(!bckg)
						{
							EOFFY.at(j)+=charge;
							if(j==FIRST_ELEC)
								count_eoffy++;
							sum_val=0.;
							PostSFB.at(j)=0.;
						}
						else
						{
							if(bkgnd_param!=false)
								PreSFB.at(j)=charge-offXY[j][1];
							sum_val+=PreSFB.at(j);
							PostSFB.at(j)=PreSFB.at(j);
						}
						isLabelY=1;
					break;
				}
			}
			if(bckg&&fit_param==true)
				SubFittingBackground(binl,binr,&sum_val);
			charge_totale+=calib*sum_val/2.;

			if(integration_used==true)
				for(int i=FIRST_ELEC;i<LAST_ELEC;i++)
				{
					if(label==LabelX)
						Integrated_X.at(i)+=PostSFB.at(i);
					if(label==LabelY)
						Integrated_Y.at(i)+=PostSFB.at(i);
				}

		}
		if(isLabelX==1&&isLabelY==1)
		{
			hct1=high_resolution_clock::now();

			if(fasterTime>bound_eoff&&bckg==false)
			{
				for(int j=FIRST_ELEC;j<LAST_ELEC;j++)
				{
					offXY[j][0]=EOFFX.at(j)/count_eoffx;
					offXY[j][1]=EOFFY.at(j)/count_eoffy;
					// cout<<j<<" "<<offXY[j][0]<<" "<<offXY[j][1]<<endl;
				}
				bckg=true;
				cout<<"Offsets électroniques calculés"<<endl;
				if(t_last_int==-1)
					t_last_int=fasterTime;
			}
			// bckg=true;
			vect_time_tot[count]=fasterTime;
			vect_charge_cumul[count]=charge_totale;
			if(charge_totale>dose_limit&&brkg_condition==false)
			{
				charge_limite=charge_totale;
				time_limit=fasterTime;
				brkg_condition=true;
			}
			count++;
			isLabelX=0;
			isLabelY=0;
		
			if((fasterTime-t_last_refresh)>=refresh_time)
			{
				t_last_refresh=fasterTime;
	
				for(int i=0;i<N_STRIPS;i++)
				{
					if(label==LabelX)
					{
						hXbefore->SetBinContent(i+1,PreSFB.at(i));
						hYbefore->SetBinContent(i+1,PreSFBprim.at(i));
						hXafter->SetBinContent(i+1,PostSFB.at(i));
						hYafter->SetBinContent(i+1,PostSFBprim.at(i));
						for(int k=0;k<N_STRIPS;k++)
						{
							if(sum_val>0.)
								pixel=PostSFB.at(i)*PostSFBprim.at(k)/sum_val;
							else
								pixel=0.;
							h2DMap->SetBinContent(i+1,k+1,pixel);
						}
					}
					if(label==LabelY)
					{
						hXbefore->SetBinContent(i+1,PreSFBprim.at(i));
						hYbefore->SetBinContent(i+1,PreSFB.at(i));
						hXafter->SetBinContent(i+1,PostSFBprim.at(i));
						hYafter->SetBinContent(i+1,PostSFB.at(i));
						for(int k=0;k<N_STRIPS;k++)
						{
							if(sum_val>0.)
								pixel=PostSFB.at(i)*PostSFBprim.at(k)/sum_val;
							else
								pixel=0.;
							h2DMap->SetBinContent(k+1,i+1,pixel);
						}
					}
				}

				// cVisu->cd();
				pad1->cd();
				if(fabs(hXbefore->GetBinContent(hXbefore->GetMaximumBin()))<1.&&fabs(hXbefore->GetBinContent(hXbefore->GetMinimumBin()))<1.)
					hXbefore->GetYaxis()->SetRangeUser(-1.,1.);
				else
					hXbefore->GetYaxis()->UnZoom();
				if(fit_param==true)
				{
					fit_line->DrawLine(borne_m_x-.5,hXbefore->GetYaxis()->GetXmin()/1.1,borne_m_x-.5,hXbefore->GetYaxis()->GetXmax()/1.1);
					fit_line->DrawLine(borne_M_x+.5,hXbefore->GetYaxis()->GetXmin()/1.1,borne_M_x+.5,hXbefore->GetYaxis()->GetXmax()/1.1);
				}
				// hXbefore->Draw("b");
				pad1->Modified();

				pad2->cd();
				if(fabs(hYbefore->GetBinContent(hYbefore->GetMaximumBin()))<1.&&fabs(hYbefore->GetBinContent(hYbefore->GetMinimumBin()))<1.)
					hYbefore->GetYaxis()->SetRangeUser(-1.,1.);
				else
					hYbefore->GetYaxis()->UnZoom();
				if(fit_param==true)
				{
					fit_line->DrawLine(borne_m_y-.5,hYbefore->GetYaxis()->GetXmin()/1.1,borne_m_y-.5,hYbefore->GetYaxis()->GetXmax()/1.1);
					fit_line->DrawLine(borne_M_y+.5,hYbefore->GetYaxis()->GetXmin()/1.1,borne_M_y+.5,hYbefore->GetYaxis()->GetXmax()/1.1);
				}
				// hYbefore->Draw("b");
				pad2->Modified();

				if(visu_param==2||visu_param==4)
				{
					pad4->cd();
					if(fabs(hXafter->GetBinContent(hXafter->GetMaximumBin()))<1.&&fabs(hXafter->GetBinContent(hXafter->GetMinimumBin()))<1.)
						hXafter->GetYaxis()->SetRangeUser(-1.,1.);
					else
						hXafter->GetYaxis()->UnZoom();
				if(fit_param==true)
				{
					fit_line->DrawLine(borne_m_x-.5,hXafter->GetYaxis()->GetXmin()/1.1,borne_m_x-.5,hXafter->GetYaxis()->GetXmax()/1.1);
					fit_line->DrawLine(borne_M_x+.5,hXafter->GetYaxis()->GetXmin()/1.1,borne_M_x+.5,hXafter->GetYaxis()->GetXmax()/1.1);
				}
					// hXafter->Draw("b");
					pad4->Modified();

					pad5->cd();
					if(fabs(hYafter->GetBinContent(hYafter->GetMaximumBin()))<1.&&fabs(hYafter->GetBinContent(hYafter->GetMinimumBin()))<1.)
						hYafter->GetYaxis()->SetRangeUser(-1.,1.);
					else
						hYafter->GetYaxis()->UnZoom();
				if(fit_param==true)
				{
					fit_line->DrawLine(borne_m_y-.5,hYafter->GetYaxis()->GetXmin()/1.1,borne_m_y-.5,hYafter->GetYaxis()->GetXmax()/1.1);
					fit_line->DrawLine(borne_M_y+.5,hYafter->GetYaxis()->GetXmin()/1.1,borne_M_y+.5,hYafter->GetYaxis()->GetXmax()/1.1);
				}
					// hYafter->Draw("b");
					pad5->Modified();
				}

				if(twoD_param==true)
				{
					// pad6->cd();
					pad6->Modified();
				}

				pad0->cd();
				TG_ChargeTime->Delete();
				TG_ChargeTime=new TGraph(count-1,vect_time_tot,vect_charge_cumul);
				TG_ChargeTime->SetMarkerColor(6);
				TG_ChargeTime->SetLineColor(6);
				TG_ChargeTime->SetLineWidth(1.5);
				TG_ChargeTime->SetTitle("Charge total over time");
				TG_ChargeTime->GetXaxis()->SetTitle("Time (s)");
				TG_ChargeTime->GetXaxis()->CenterTitle();
				TG_ChargeTime->GetXaxis()->SetTitleOffset(0.45);
				TG_ChargeTime->GetXaxis()->SetTickSize(0.01);
				TG_ChargeTime->GetXaxis()->SetTitleSize(0.06);
				TG_ChargeTime->GetXaxis()->SetLabelSize(0.035);
				TG_ChargeTime->GetYaxis()->CenterTitle();
				TG_ChargeTime->GetYaxis()->SetTitleOffset(0.65);
				TG_ChargeTime->GetYaxis()->SetTickSize(0.01);
				TG_ChargeTime->GetYaxis()->SetTitleSize(0.0);
				TG_ChargeTime->GetYaxis()->CenterTitle();
				TG_ChargeTime->GetYaxis()->SetLabelSize(0.035);
				TG_ChargeTime->Draw("AP");
				stop_line->SetLineColor(2);
				if(brkg_condition==true)
					stop_line->DrawLine(time_limit,TG_ChargeTime->GetYaxis()->GetXmin()/1.1,time_limit,TG_ChargeTime->GetYaxis()->GetXmax()/1.1);

				pad3->cd();
				pad3->Clear();
				xt=.99;
				yt=.95;
				if(visu_param==2||visu_param==4)
					ydecal=0.02;
				if(visu_param==1||visu_param==3)
					ydecal=0.025;
				Texte->SetTextAlign(31);
				Texte->SetTextFont(43);
				Texte->SetTextSize(15);
				if(!bckg)
				{
					Texte->SetTextColor(2);
					Texte->DrawText(xt,yt,"Acquisition");
					yt-=ydecal;
					Texte->DrawText(xt,yt,"bruit electronique");
					yt-=ydecal;
					text_tmp.Form("%2.2lf sec",bound_eoff-fasterTime);
					Texte->DrawText(xt,yt,text_tmp);
					yt-=ydecal;
					yt-=ydecal;
				}
				Texte->SetTextColor(1);
				Texte->DrawText(xt,yt,"Charge limite");
				yt-=ydecal;
				text_tmp.Form("%2.2lf pC",dose_limit);
				Texte->DrawText(xt,yt,text_tmp);
				yt-=ydecal;
				yt-=ydecal;
				Texte->DrawText(xt,yt,"Charge totale");
				yt-=ydecal;
				text_tmp.Form("%2.2lf pC",charge_totale);
				Texte->DrawText(xt,yt,text_tmp);
				yt-=ydecal;
				yt-=ydecal;
				Texte->DrawText(xt,yt,"Temps d'Irradiation");
				yt-=ydecal;
				text_tmp.Form("%2.2lf sec",time_limit);
				Texte->DrawText(xt,yt,text_tmp);
				if(brkg_condition==true)
				{
					Texte->SetTextColor(2);
					yt-=ydecal;
					yt-=ydecal;
					Texte->DrawText(xt,yt,"Limite atteinte");
					yt-=ydecal;
					yt-=ydecal;
					Texte->DrawText(xt,yt,"Charge totale");
					yt-=ydecal;
					text_tmp.Form("%2.2lf pC",charge_totale);
					Texte->DrawText(xt,yt,text_tmp);
					yt-=ydecal;
					yt-=ydecal;
					Texte->DrawText(xt,yt,"Temps d'Irradiation");
					yt-=ydecal;
					text_tmp.Form("%2.2lf sec",fasterTime);
					Texte->DrawText(xt,yt,text_tmp);
				}

				// cVisu->Modified();
				// cVisu->cd();
				cVisu->Update();

				if(integration_used==true)
				{
					sum_val=0.;
					for(int i=0;i<N_STRIPS;i++)
					{
						hXint->SetBinContent(i+1,Integrated_X.at(i));
						hYint->SetBinContent(i+1,Integrated_Y.at(i));
						sum_val+=(Integrated_X.at(i)+Integrated_Y.at(i))/2.;
					}
					for(int i=0;i<N_STRIPS;i++)
					{
						for(int k=0;k<N_STRIPS;k++)
						{
							if(sum_val>0.)
								pixel=Integrated_X.at(i)*Integrated_Y.at(k)/sum_val;
							else
								pixel=0.;
							h2DMap_int->SetBinContent(i+1,k+1,pixel);
						}
					}
					// cInte->cd();
					// pad_int0->cd();
					if(fabs(hXint->GetBinContent(hXint->GetMaximumBin()))<1.&&fabs(hXint->GetBinContent(hXint->GetMinimumBin()))<1.)
						hXint->GetYaxis()->SetRangeUser(-1.,1.);
					else
						hXint->GetYaxis()->UnZoom();
					pad_int0->Modified();

					// pad_int1->cd();
					if(fabs(hYint->GetBinContent(hYint->GetMaximumBin()))<1.&&fabs(hYint->GetBinContent(hYint->GetMinimumBin()))<1.)
						hYint->GetYaxis()->SetRangeUser(-1.,1.);
					else
						hYint->GetYaxis()->UnZoom();
					pad_int1->Modified();

					// pad_int2->cd();
					pad_int2->Modified();

					// cInte->cd();
					cInte->Update();
				}
			}

			if((fasterTime-t_last_int)>=integration_time)
			{
				t_last_int=fasterTime;
				Integrated_X.clear();
				Integrated_X.resize(N_STRIPS);
				Integrated_Y.clear();
				Integrated_Y.resize(N_STRIPS);
			}
			
			hct2=high_resolution_clock::now();
			auto time_span=duration_cast<std::chrono::microseconds>(hct2-hct1);
			auto time_span_tot=duration_cast<std::chrono::microseconds>(hct2-hct0);
			microseconds=time_span.count();
			inter_fasterTime=(fasterTime-last_fasterTime)*1.E6;
			if(microseconds>inter_fasterTime&&inter_fasterTime>0.)
			{
				cout<<fasterTime<<endl;
				temps+=microseconds/1.E6;
				count_foireux++;
				cout<<"Ca tient pas la route, ça tient pas la route "<<setprecision(1)<<fixed<<microseconds<<" "<<inter_fasterTime<<endl;
				cout<<tot_prog_milli<<" "<<tot_fasterTime_milli<<endl;
				cout<<time_span_tot.count()/1E6<<endl;
			}
			tot_prog_milli+=(int)microseconds/1000;
			tot_fasterTime_milli+=(int)inter_fasterTime/1000;
			last_fasterTime=fasterTime;
		}
	}
	// cVisu->SaveAs("Image_finale.png");
	cout<<"Temps dépassement : "<<temps<<" pour "<<count_foireux<<" soit en moyenne "<<temps/count_foireux<<endl;
	hct2=high_resolution_clock::now();
	auto time_span_tot=duration_cast<std::chrono::microseconds>(hct2-hct0);
	cout<<"Durée totale d'exécution du programme : "<<time_span_tot.count()/1E6<<" sec"<<endl;
	cout<<"Durée faster : "<<fasterTime<<" sec"<<endl;

	if(bkgnd_param==3)
	{
		ofstream bdf_file("Offset.txt");
		for(int i=0;i<N_STRIPS;i++)
			bdf_file<<i<<offXY[i][0]<<offXY[i][1]<<endl;
		bdf_file.close();
	}

	cout<<charge_totale<<endl;
	cout<<"Irradiation terminée "<<count<<endl;
	if(brkg_condition==false)
		cout<<"Condition limite non atteinte"<<endl;

	faster_file_reader_close(reader);
	faster_file_writer_close(writer);

	char command_line[80];
	sprintf(command_line,"faster_disfast %s -I",data_faster_out_file.c_str());
	system(command_line);

	cVisu->Destructor();
	TG_ChargeTime->Delete();
	free(vect_time_tot);
	free(vect_charge_cumul);
}
