#include "Switch.h"

using namespace std;

void EntryParameters(int config_simu)
{
	stringstream ss;
	ss<<config_simu;
	string indice=ss.str();
	stringstream ss2;
	int n;

	int iint;
	int ichar;
	int ivar;
	char next;
	string data_file;
	string path_file;
	string filename="./Entry/Entry_param_"+indice+".txt";
	// string filename="./Entry/Entry_param_1.txt";
	string tmp;

	Variable_init.clear();
	Value_init.clear();
	Variable_init.push_back("File");
	Value_init.push_back(0.);	//	0
	Variable_init.push_back("Path for the file");
	Value_init.push_back(0.);	//	1
	Variable_init.push_back("Time sampling (s)");
	Value_init.push_back(0.);	//	2

	cout<<endl;
	// ifstream datafile_param("./Entry/Entry_param_1.txt");
	ifstream datafile_param(filename.c_str());
	if(!datafile_param)
	{
		cout<<"No entry parameters file for this configuration: "<<config_simu<<endl;
		cout<<"No file: "<<filename<<endl;
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
						data_file=buffer;
					if(ind_value==1)
						path_file=buffer;
					if(ind_value==2)
					{
						value=(double)atof(buffer.c_str());
						if(value==Value_init[ind_value])
							cout<<Variable_init[ind_value]<<" default value keeped: "<<Value_init[ind_value]<<endl;
						else	
							cout<<Variable_init[ind_value]<<" default value: "<<Value_init[ind_value]<<"; new value: "<<value<<endl;
						Value_init[ind_value]=value;
					}
				}	
				else
					cout<<"Value not define for "<<variable<<". Keeping the default one: "<<Value_init[ind_value]<<". Maybe check the entry file."<<endl;
			}	
			if(datafile_param.eof()) break;
		}
		datafile_param.close();
	}	

	data_faster_file="../DATA/gaussian_0001.fast";
	filename=path_file+data_file; 
	ifstream datafile_faster(filename.c_str());
	if(!datafile_faster)
	{
		cout<<"No data file: "<<filename<<endl;
		cout<<"Keeping the default one: "<<data_faster_file<<". Maybe check the entry file."<<endl;
	}
	else
		data_faster_file=filename;
	datafile_faster.close();

	cout<<endl;
	cout<<"=================================================="<<endl;
	cout<<" Parameters for the initialisation; config: "<<config_simu<<endl;
	cout<<"--------------------------------------------------"<<endl;
	cout<<" File of data: "<<data_faster_file<<endl;
	ivar=2;
	cout<<" "<<Variable_init[ivar]<<": "<<Value_init[ivar]<<endl;
	cout<<"=================================================="<<endl;
	cout<<endl;

	SamplingTime=Value_init[2];
}

int main(int argc, char** argv)
{
	cout.precision(8);
	set_plot_style();
	char *filename=(char*)malloc(80);
	char *tubename=(char*)malloc(80);
	
	faster_file_reader_p reader;
	faster_file_writer_p writer;
	faster_data_p data;
	electrometer_data electro;

	unsigned short label;
	int count=0;
	double t0;
	double fasterTime=0.;
	double charge;

	int config_simu=1;
	if(argc>1)
		config_simu=(int)atof(argv[1]);

	EntryParameters(config_simu);
	strcpy(filename,data_faster_file.c_str());
	tubename="../DATA/Tube.fast";

	char command_line[80];
	sprintf(command_line,"faster_disfast %s -I",filename);
	system(command_line);

	reader=faster_file_reader_open(filename);
	
	if(reader==NULL) 
	{
		printf ("Error opening file %s\n", filename);
		faster_file_reader_close(reader);
		return 0;
	}
	else
		printf("Fichier de données bien ouvert\n");

	t0=-1;
	writer=faster_file_writer_open(tubename);


	while((data=faster_file_reader_next(reader))!=NULL) 
	{
		faster_file_writer_next(writer,data);
		usleep(2400);
		count++;
		// cout<<count<<endl;
	}

	faster_file_reader_close(reader);
	faster_file_writer_close(writer);

	// sprintf(command_line,"faster_disfast %s -I",tubename);
	// system(command_line);
	return(EXIT_SUCCESS);
}
