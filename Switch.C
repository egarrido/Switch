#include "Switch.h"

using namespace std;

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
		scale_value=PreSFB[i];
		Profile->SetBinContent(i+1,PreSFB[i]);
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
		PostSFBprim[i]=PostSFB[i];
		bdf_SFB=0.;
		for(int ii=0;ii<i_par;ii++)
			bdf_SFB+=par[ii]*pow(x,ii);
		PostSFB[i]=PreSFB[i]-bdf_SFB;
		if(i+1>binl&&i+1<binr)
			*sum_val+=PostSFB[i];
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
	cout.precision(8);
	set_plot_style();
	int ii;
	char *tubename=(char*)malloc(80);
	char *outname=(char*)malloc(80);
	
	faster_file_reader_p reader;
	faster_file_writer_p writer;
	faster_data_p data;
	electrometer_data electro;

	EOFFX.resize(N_STRIPS);
	EOFFX.clear();
	EOFFY.resize(N_STRIPS);
	EOFFY.clear();
	PreSFB.resize(N_STRIPS);
	PostSFB.resize(N_STRIPS);
	PreSFBprim.resize(N_STRIPS);
	PostSFBprim.resize(N_STRIPS);

	double* vect_time_tot=(double*)malloc(MAX_INTEGR*sizeof(double));
	double* vect_charge_cumul=(double*)malloc(MAX_INTEGR*sizeof(double));

	bool bckg=false;
	unsigned short label;
	int count=0;
	int count_eoffx=0;
	int count_eoffy=0;
	int isLabelX=0;
	int isLabelY=0;
	int integration=400;
	double t0=-1;
	double calib=1.;
	double fasterTime=0.;
	double time_limit;
	double eoff_limit=10.;
	double charge;
	double val;
	double sum_val=0.;
	double charge_limite=0.;
	double charge_totale=0.;

	int brkg_condition;
	// double dose_limit=DBL_MAX;
	double dose_limit=10000.;

	ifstream bdf_file("Offset.txt");
	double tmpx,tmpy;
	while(!bdf_file.eof())
	{
		bdf_file>>ii>>tmpx>>tmpy;
		offXY[ii][0]=tmpx;
		offXY[ii][1]=tmpy;
	}
	bdf_file.close();


	TApplication *application=new TApplication("Pilou",&argc, argv);
	Double_t x0=.05,
					 x1=.01,
					 x2=.43,
					 x3=.35,
					 x4=.66,
					 x5=.68,
					 y1=.45,
					 y2=.99,
					 y3=.83,
					 y4=.33,
					 y5=.85;
	Double_t xt,yt,ydecal=.018;
	TString text_tmp;
				
	TCanvas * cVisu = new TCanvas("Visualisation","",1000,1000);
	TPad * pad0 = new TPad("pad0","pad0",x1,x0,x0,y2);
	TPad * pad1 = new TPad("pad1","pad1",x0,x5,x2,y2);
	TPad * pad2 = new TPad("pad2","pad2",x0,x3,x2,x4);
	TPad * pad3 = new TPad("pad3","pad3",y1,x5,y3,y2);
	TPad * pad4 = new TPad("pad4","pad4",y1,x3,y3,x4);
	TPad * pad5 = new TPad("pad5","pad5",x0,x1,y3,y4);
	TPad * pad6 = new TPad("pad6","pad6",y5,x1,y2,y2);
	pad0->SetLeftMargin(0.001);
	pad0->SetRightMargin(0.001);
	pad0->SetTopMargin(0.01);
	pad0->SetBottomMargin(0.01);
	pad0->Draw();
	pad1->SetLeftMargin(0.06);
	pad1->SetRightMargin(0.05);
	pad1->SetTopMargin(0.1);
	pad1->SetBottomMargin(0.01);
	pad1->Draw();
	pad2->SetLeftMargin(0.06);
	pad2->SetRightMargin(0.05);
	pad2->SetTopMargin(0.01);
	pad2->SetBottomMargin(0.1);
	pad2->Draw();
	pad3->SetLeftMargin(0.05);
	pad3->SetRightMargin(0.06);
	pad3->SetTopMargin(0.1);
	pad3->SetBottomMargin(0.01);
	pad3->Draw();
	pad4->SetLeftMargin(0.05);
	pad4->SetRightMargin(0.06);
	pad4->SetTopMargin(0.01);
	pad4->SetBottomMargin(0.1);
	pad4->Draw();
	pad5->SetLeftMargin(0.03);
	pad5->SetRightMargin(0.03);
	pad5->SetTopMargin(0.1);
	pad5->SetBottomMargin(0.1);
	pad5->Draw();
	pad6->SetLeftMargin(0.01);
	pad6->SetRightMargin(0.01);
	pad6->SetTopMargin(0.01);
	pad6->SetBottomMargin(0.01);
	pad6->Draw();

	TH1F* hXbefore=new TH1F("hXbefore","Profile in X",N_STRIPS,.5,32.5);
	TH1F* hYbefore=new TH1F("hYbefore","Profile in Y",N_STRIPS,.5,32.5);
	TH1F* hXafter=new TH1F("hXafter","",N_STRIPS,.5,32.5);
	TH1F* hYafter=new TH1F("hYafter","",N_STRIPS,.5,32.5);
	TGraph* TG_ChargeTime=new TGraph();
	TText* ChargePC=new TText();
	TText* Texte=new TText();
	TLine* stop_line=new TLine();

	pad0->cd();
	ChargePC->SetTextAlign(22);
	ChargePC->SetTextColor(1);
	ChargePC->SetTextFont(43);
	ChargePC->SetTextSize(18);
	ChargePC->SetTextAngle(90);
	ChargePC->DrawText(.5,.82,"Charge (pC)");
	ChargePC->DrawText(.5,.5,"Charge (pC)");
	ChargePC->DrawText(.5,.13,"Charge (pC)");

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

	pad2->cd();
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

	pad3->cd();
	hYbefore->SetFillColor(2);
	hYbefore->GetXaxis()->SetNdivisions(N_STRIPS);
	hYbefore->GetXaxis()->SetTickSize(0.01);
	hYbefore->GetXaxis()->SetLabelSize(0.0);
	hYbefore->GetYaxis()->SetTickSize(0.01);
	hYbefore->GetYaxis()->SetLabelSize(0.035);
	hYbefore->SetBarWidth(0.8);
	hYbefore->SetBarOffset(0.1);
	hYbefore->SetStats(0);
	hYbefore->Draw("b");

	pad4->cd();
	hYafter->SetFillColor(4);
	hYafter->GetXaxis()->SetNdivisions(N_STRIPS);
	hYafter->GetXaxis()->SetTitle("Strip");
	hYafter->GetXaxis()->SetTitleOffset(0.65);
	hYafter->GetXaxis()->CenterTitle();
	hYafter->GetXaxis()->SetTickSize(0.01);
	hYafter->GetXaxis()->SetTitleSize(0.06);
	hYafter->GetXaxis()->SetLabelSize(0.035);
	hYafter->GetYaxis()->SetTickSize(0.01);
	hYafter->GetYaxis()->SetLabelSize(0.035);
	hYafter->SetBarWidth(0.8);
	hYafter->SetBarOffset(0.1);
	hYafter->SetStats(0);
	hYafter->Draw("b");

	pad5->cd();
	
	tubename="../DATA/Tube.fast";
	outname="../DATA/speed_bis_0001.fast";

	char command_line[80];

	reader=faster_file_reader_open(tubename);
	
	if(reader==NULL) 
	{
		printf ("Error opening tube file %s\n", tubename);
		faster_file_reader_close(reader);
		return 0;
	}
	else
		printf("Tube de données bien ouvert\n");

	writer=faster_file_writer_open(outname);
	brkg_condition=false;

	count_eoffx=0;
	count_eoffy=0;
	while((data=faster_file_reader_next(reader))!=NULL) 
	// while((data=faster_file_reader_next(reader))!=NULL&&brkg_condition==false) 
	{
		faster_file_writer_next(writer,data);
		if(t0==-1)
			t0=faster_data_clock_sec(data);
		fasterTime=faster_data_clock_sec(data)-t0;
		PreSFB.clear();
		PostSFB.clear();
		label=faster_data_label(data);
		if(label==LabelX||label==LabelY)
		{
			for(int j=FIRST_ELEC;j<LAST_ELEC;j++) 
			{
				faster_data_load(data,&electro);
				charge=electrometer_channel_charge_pC(electro,j+1);
				switch(label) 
				{
					case LabelX:
						PreSFBprim[j]=PreSFB[j];
						if(!bckg)
						{
							EOFFX[j]+=charge;
							if(j==FIRST_ELEC)
								count_eoffx++;
							val=charge;
							sum_val=0.;
						}
						else
							val=charge-offXY[j][0];
						PreSFB[j]=val;
						isLabelX=1;
					break;
					case LabelY:
						PreSFBprim[j]=PreSFB[j];
						if(!bckg)
						{
							EOFFY[j]+=charge;
							if(j==FIRST_ELEC)
								count_eoffy++;
							val=charge;
							sum_val=0.;
						}
						else
							val=charge-offXY[j][1];
						PreSFB[j]=val;
						isLabelY=1;
					break;
				}
			}
			if(bckg)
				SubFittingBackground(14,20,&sum_val);
			charge_totale+=calib*sum_val/2.;
		}
		if(isLabelX==1&&isLabelY==1)
		{
			if(fasterTime>eoff_limit&&bckg==false)
			{
				for(int j=FIRST_ELEC;j<LAST_ELEC;j++)
				{
					offXY[j][0]=EOFFX[j]/count_eoffx;
					offXY[j][1]=EOFFY[j]/count_eoffy;
					// cout<<j<<" "<<offXY[j][0]<<" "<<offXY[j][1]<<endl;
				}
				bckg=true;
			}
			// bckg=true;
			vect_time_tot[count]=fasterTime;
			vect_charge_cumul[count]=charge_totale;
			if(charge_totale>dose_limit)
				brkg_condition=true;
			else
			{
				charge_limite=charge_totale;
				time_limit=fasterTime;
			}
			count++;
			isLabelX=0;
			isLabelY=0;
		
			if(count%integration==0)
			{
				for(int i=0;i<N_STRIPS;i++)
				{
					if(label==LabelX)
					{
						hXbefore->SetBinContent(i+1,PreSFB[i]);
						hYbefore->SetBinContent(i+1,PreSFBprim[i]);
						hXafter->SetBinContent(i+1,PostSFB[i]);
						hYafter->SetBinContent(i+1,PostSFBprim[i]);
					}
					if(label==LabelY)
					{
						hXbefore->SetBinContent(i+1,PreSFBprim[i]);
						hYbefore->SetBinContent(i+1,PreSFB[i]);
						hXafter->SetBinContent(i+1,PostSFBprim[i]);
						hYafter->SetBinContent(i+1,PostSFB[i]);
					}
				}

				pad1->cd();
				if(fabs(hXbefore->GetBinContent(hXbefore->GetMaximumBin()))<1.&&fabs(hXbefore->GetBinContent(hXbefore->GetMinimumBin()))<1.)
					hXbefore->GetYaxis()->SetRangeUser(-1.,1.);
				else
					hXbefore->GetYaxis()->UnZoom();
				// hXbefore->Draw("b");
				pad1->Modified();

				pad2->cd();
				if(fabs(hXafter->GetBinContent(hXafter->GetMaximumBin()))<1.&&fabs(hXafter->GetBinContent(hXafter->GetMinimumBin()))<1.)
					hXafter->GetYaxis()->SetRangeUser(-1.,1.);
				else
					hXafter->GetYaxis()->UnZoom();
				// hXafter->Draw("b");
				pad2->Modified();

				pad3->cd();
				if(fabs(hYbefore->GetBinContent(hYbefore->GetMaximumBin()))<1.&&fabs(hYbefore->GetBinContent(hYbefore->GetMinimumBin()))<1.)
					hYbefore->GetYaxis()->SetRangeUser(-1.,1.);
				else
					hYbefore->GetYaxis()->UnZoom();
				// hYbefore->Draw("b");
				pad3->Modified();

				pad4->cd();
				if(fabs(hYafter->GetBinContent(hYafter->GetMaximumBin()))<1.&&fabs(hYafter->GetBinContent(hYafter->GetMinimumBin()))<1.)
					hYafter->GetYaxis()->SetRangeUser(-1.,1.);
				else
					hYafter->GetYaxis()->UnZoom();
				// hYafter->Draw("b");
				pad4->Modified();

				pad5->cd();
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
				
				pad6->cd();
				pad6->Clear();
				xt=.95;
				yt=.95;
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
					text_tmp.Form("%2.2lf sec",eoff_limit-time_limit);
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
				text_tmp.Form("%2.2lf pC",charge_limite);
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
				cVisu->Update();
			}
		}
	}
	cVisu->SaveAs("Bordel.png");

	cout<<charge_totale<<endl;
	cout<<"Irradiation terminée "<<count<<endl;
	if(brkg_condition==false)
		cout<<"Condition limite non atteinte"<<endl;

	// while((data=faster_file_reader_next(reader))) 
	// 	faster_file_writer_next(writer,data);

	faster_file_reader_close(reader);
	faster_file_writer_close(writer);

	sprintf(command_line,"faster_disfast %s -I",outname);
	system(command_line);

	cVisu->Destructor();
	TG_ChargeTime->Delete();
	free(vect_time_tot);
	free(vect_charge_cumul);
}
