#include "Switch.h"

using namespace std;

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

	tubename="../DATA/Tube_0001.fast";
	filename="../Cyrce/04_2017/C_1000_position_pt_1_0001.fast";

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
		if(faster_data_label(data)==4||faster_data_label(data)==6)
			usleep(1200);
		count++;
		// cout<<count<<endl;
	}

	faster_file_reader_close(reader);
	faster_file_writer_close(writer);

	// sprintf(command_line,"faster_disfast %s -I",tubename);
	// system(command_line);
	return(EXIT_SUCCESS);
}
