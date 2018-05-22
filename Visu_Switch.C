#include "Switch.h"

using namespace std;
using namespace std::chrono;

int main(int argc, char** argv)
{
	fit_param=atoi(argv[1]);
	twoD_param=atoi(argv[2]);
	integration_used=atoi(argv[3]);
	borne_m_x=atoi(argv[4]);
	borne_M_x=atoi(argv[5]);
	borne_m_y=atoi(argv[6]);
	borne_M_y=atoi(argv[7]);
	// cout<<fit_param<<" "<<twoD_param<<" "<<integration_used<<" "<<borne_m_x<<" "<<borne_M_x<<" "<<borne_m_y<<" "<<borne_M_y<<endl;

	int tmp;
	ifstream fifo_visu_input("./Static_tube.tub");
	for(int i=0;i<10;i++)
	{
		fifo_visu_input>>tmp;
		cout<<i<<" "<<tmp<<endl;
	}
	fifo_visu_input.close();
/*
	set_plot_style();
	TApplication *application=new TApplication("Visualisation",&argc,argv);
	cout.precision(8);

	ifstream data_tube("./Static_tube.tub");
	if(!data_tube) 
	{
		printf ("Error opening tube of data for visualisation");
		exit(0);
	}
	else
		printf("Tube de données de visualisation bien ouvert\n");

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

	bckg=false;
	brkg_condition=false;
	count_eoffx=0;
	count_eoffy=0;

	// 		if((fasterTime-t_last_refresh)>=refresh_time)
	// 		{
	// 			t_last_refresh=fasterTime;
	
	// 			for(int i=0;i<N_STRIPS;i++)
	// 			{
	// 				if(label==LabelX)
	// 				{
	// 					hXbefore->SetBinContent(i+1,PreSFB.at(i));
	// 					hYbefore->SetBinContent(i+1,PreSFBprim.at(i));
	// 					hXafter->SetBinContent(i+1,PostSFB.at(i));
	// 					hYafter->SetBinContent(i+1,PostSFBprim.at(i));
	// 					for(int k=0;k<N_STRIPS;k++)
	// 					{
	// 						if(sum_val>0.)
	// 							pixel=PostSFB.at(i)*PostSFBprim.at(k)/sum_val;
	// 						else
	// 							pixel=0.;
	// 						h2DMap->SetBinContent(i+1,k+1,pixel);
	// 					}
	// 				}
	// 				if(label==LabelY)
	// 				{
	// 					hXbefore->SetBinContent(i+1,PreSFBprim.at(i));
	// 					hYbefore->SetBinContent(i+1,PreSFB.at(i));
	// 					hXafter->SetBinContent(i+1,PostSFBprim.at(i));
	// 					hYafter->SetBinContent(i+1,PostSFB.at(i));
	// 					for(int k=0;k<N_STRIPS;k++)
	// 					{
	// 						if(sum_val>0.)
	// 							pixel=PostSFB.at(i)*PostSFBprim.at(k)/sum_val;
	// 						else
	// 							pixel=0.;
	// 						h2DMap->SetBinContent(k+1,i+1,pixel);
	// 					}
	// 				}
	// 			}

	// 			// cVisu->cd();
	// 			pad1->cd();
	// 			if(fabs(hXbefore->GetBinContent(hXbefore->GetMaximumBin()))<1.&&fabs(hXbefore->GetBinContent(hXbefore->GetMinimumBin()))<1.)
	// 				hXbefore->GetYaxis()->SetRangeUser(-1.,1.);
	// 			else
	// 				hXbefore->GetYaxis()->UnZoom();
	// 			if(fit_param==true)
	// 			{
	// 				fit_line->DrawLine(borne_m_x-.5,hXbefore->GetYaxis()->GetXmin()/1.1,borne_m_x-.5,hXbefore->GetYaxis()->GetXmax()/1.1);
	// 				fit_line->DrawLine(borne_M_x+.5,hXbefore->GetYaxis()->GetXmin()/1.1,borne_M_x+.5,hXbefore->GetYaxis()->GetXmax()/1.1);
	// 			}
	// 			// hXbefore->Draw("b");
	// 			pad1->Modified();

	// 			pad2->cd();
	// 			if(fabs(hYbefore->GetBinContent(hYbefore->GetMaximumBin()))<1.&&fabs(hYbefore->GetBinContent(hYbefore->GetMinimumBin()))<1.)
	// 				hYbefore->GetYaxis()->SetRangeUser(-1.,1.);
	// 			else
	// 				hYbefore->GetYaxis()->UnZoom();
	// 			if(fit_param==true)
	// 			{
	// 				fit_line->DrawLine(borne_m_y-.5,hYbefore->GetYaxis()->GetXmin()/1.1,borne_m_y-.5,hYbefore->GetYaxis()->GetXmax()/1.1);
	// 				fit_line->DrawLine(borne_M_y+.5,hYbefore->GetYaxis()->GetXmin()/1.1,borne_M_y+.5,hYbefore->GetYaxis()->GetXmax()/1.1);
	// 			}
	// 			// hYbefore->Draw("b");
	// 			pad2->Modified();

	// 			if(visu_param==2||visu_param==4)
	// 			{
	// 				pad4->cd();
	// 				if(fabs(hXafter->GetBinContent(hXafter->GetMaximumBin()))<1.&&fabs(hXafter->GetBinContent(hXafter->GetMinimumBin()))<1.)
	// 					hXafter->GetYaxis()->SetRangeUser(-1.,1.);
	// 				else
	// 					hXafter->GetYaxis()->UnZoom();
	// 			if(fit_param==true)
	// 			{
	// 				fit_line->DrawLine(borne_m_x-.5,hXafter->GetYaxis()->GetXmin()/1.1,borne_m_x-.5,hXafter->GetYaxis()->GetXmax()/1.1);
	// 				fit_line->DrawLine(borne_M_x+.5,hXafter->GetYaxis()->GetXmin()/1.1,borne_M_x+.5,hXafter->GetYaxis()->GetXmax()/1.1);
	// 			}
	// 				// hXafter->Draw("b");
	// 				pad4->Modified();

	// 				pad5->cd();
	// 				if(fabs(hYafter->GetBinContent(hYafter->GetMaximumBin()))<1.&&fabs(hYafter->GetBinContent(hYafter->GetMinimumBin()))<1.)
	// 					hYafter->GetYaxis()->SetRangeUser(-1.,1.);
	// 				else
	// 					hYafter->GetYaxis()->UnZoom();
	// 			if(fit_param==true)
	// 			{
	// 				fit_line->DrawLine(borne_m_y-.5,hYafter->GetYaxis()->GetXmin()/1.1,borne_m_y-.5,hYafter->GetYaxis()->GetXmax()/1.1);
	// 				fit_line->DrawLine(borne_M_y+.5,hYafter->GetYaxis()->GetXmin()/1.1,borne_M_y+.5,hYafter->GetYaxis()->GetXmax()/1.1);
	// 			}
	// 				// hYafter->Draw("b");
	// 				pad5->Modified();
	// 			}

	// 			if(twoD_param==true)
	// 			{
	// 				// pad6->cd();
	// 				pad6->Modified();
	// 			}

	// 			pad0->cd();
	// 			TG_ChargeTime->Delete();
	// 			TG_ChargeTime=new TGraph(count-1,vect_time_tot,vect_charge_cumul);
	// 			TG_ChargeTime->SetMarkerColor(6);
	// 			TG_ChargeTime->SetLineColor(6);
	// 			TG_ChargeTime->SetLineWidth(1.5);
	// 			TG_ChargeTime->SetTitle("Charge total over time");
	// 			TG_ChargeTime->GetXaxis()->SetTitle("Time (s)");
	// 			TG_ChargeTime->GetXaxis()->CenterTitle();
	// 			TG_ChargeTime->GetXaxis()->SetTitleOffset(0.45);
	// 			TG_ChargeTime->GetXaxis()->SetTickSize(0.01);
	// 			TG_ChargeTime->GetXaxis()->SetTitleSize(0.06);
	// 			TG_ChargeTime->GetXaxis()->SetLabelSize(0.035);
	// 			TG_ChargeTime->GetYaxis()->CenterTitle();
	// 			TG_ChargeTime->GetYaxis()->SetTitleOffset(0.65);
	// 			TG_ChargeTime->GetYaxis()->SetTickSize(0.01);
	// 			TG_ChargeTime->GetYaxis()->SetTitleSize(0.0);
	// 			TG_ChargeTime->GetYaxis()->CenterTitle();
	// 			TG_ChargeTime->GetYaxis()->SetLabelSize(0.035);
	// 			TG_ChargeTime->Draw("AP");
	// 			stop_line->SetLineColor(2);
	// 			if(brkg_condition==true)
	// 				stop_line->DrawLine(time_limit,TG_ChargeTime->GetYaxis()->GetXmin()/1.1,time_limit,TG_ChargeTime->GetYaxis()->GetXmax()/1.1);

	// 			pad3->cd();
	// 			pad3->Clear();
	// 			xt=.99;
	// 			yt=.95;
	// 			if(visu_param==2||visu_param==4)
	// 				ydecal=0.02;
	// 			if(visu_param==1||visu_param==3)
	// 				ydecal=0.025;
	// 			Texte->SetTextAlign(31);
	// 			Texte->SetTextFont(43);
	// 			Texte->SetTextSize(15);
	// 			if(!bckg)
	// 			{
	// 				Texte->SetTextColor(2);
	// 				Texte->DrawText(xt,yt,"Acquisition");
	// 				yt-=ydecal;
	// 				Texte->DrawText(xt,yt,"bruit electronique");
	// 				yt-=ydecal;
	// 				text_tmp.Form("%2.2lf sec",bound_eoff-fasterTime);
	// 				Texte->DrawText(xt,yt,text_tmp);
	// 				yt-=ydecal;
	// 				yt-=ydecal;
	// 			}
	// 			Texte->SetTextColor(1);
	// 			Texte->DrawText(xt,yt,"Charge limite");
	// 			yt-=ydecal;
	// 			text_tmp.Form("%2.2lf pC",dose_limit);
	// 			Texte->DrawText(xt,yt,text_tmp);
	// 			yt-=ydecal;
	// 			yt-=ydecal;
	// 			Texte->DrawText(xt,yt,"Charge totale");
	// 			yt-=ydecal;
	// 			text_tmp.Form("%2.2lf pC",charge_totale);
	// 			Texte->DrawText(xt,yt,text_tmp);
	// 			yt-=ydecal;
	// 			yt-=ydecal;
	// 			Texte->DrawText(xt,yt,"Temps d'Irradiation");
	// 			yt-=ydecal;
	// 			text_tmp.Form("%2.2lf sec",time_limit);
	// 			Texte->DrawText(xt,yt,text_tmp);
	// 			if(brkg_condition==true)
	// 			{
	// 				Texte->SetTextColor(2);
	// 				yt-=ydecal;
	// 				yt-=ydecal;
	// 				Texte->DrawText(xt,yt,"Limite atteinte");
	// 				yt-=ydecal;
	// 				yt-=ydecal;
	// 				Texte->DrawText(xt,yt,"Charge totale");
	// 				yt-=ydecal;
	// 				text_tmp.Form("%2.2lf pC",charge_totale);
	// 				Texte->DrawText(xt,yt,text_tmp);
	// 				yt-=ydecal;
	// 				yt-=ydecal;
	// 				Texte->DrawText(xt,yt,"Temps d'Irradiation");
	// 				yt-=ydecal;
	// 				text_tmp.Form("%2.2lf sec",fasterTime);
	// 				Texte->DrawText(xt,yt,text_tmp);
	// 			}

	// 			// cVisu->Modified();
	// 			// cVisu->cd();
	// 			cVisu->Update();

	// 			if(integration_used==true)
	// 			{
	// 				sum_val=0.;
	// 				for(int i=0;i<N_STRIPS;i++)
	// 				{
	// 					hXint->SetBinContent(i+1,Integrated_X.at(i));
	// 					hYint->SetBinContent(i+1,Integrated_Y.at(i));
	// 					sum_val+=(Integrated_X.at(i)+Integrated_Y.at(i))/2.;
	// 				}
	// 				for(int i=0;i<N_STRIPS;i++)
	// 				{
	// 					for(int k=0;k<N_STRIPS;k++)
	// 					{
	// 						if(sum_val>0.)
	// 							pixel=Integrated_X.at(i)*Integrated_Y.at(k)/sum_val;
	// 						else
	// 							pixel=0.;
	// 						h2DMap_int->SetBinContent(i+1,k+1,pixel);
	// 					}
	// 				}
	// 				// cInte->cd();
	// 				// pad_int0->cd();
	// 				if(fabs(hXint->GetBinContent(hXint->GetMaximumBin()))<1.&&fabs(hXint->GetBinContent(hXint->GetMinimumBin()))<1.)
	// 					hXint->GetYaxis()->SetRangeUser(-1.,1.);
	// 				else
	// 					hXint->GetYaxis()->UnZoom();
	// 				pad_int0->Modified();

	// 				// pad_int1->cd();
	// 				if(fabs(hYint->GetBinContent(hYint->GetMaximumBin()))<1.&&fabs(hYint->GetBinContent(hYint->GetMinimumBin()))<1.)
	// 					hYint->GetYaxis()->SetRangeUser(-1.,1.);
	// 				else
	// 					hYint->GetYaxis()->UnZoom();
	// 				pad_int1->Modified();

	// 				// pad_int2->cd();
	// 				pad_int2->Modified();

	// 				// cInte->cd();
	// 				cInte->Update();
	// 			}
	// 		}

	// 		if((fasterTime-t_last_int)>=integration_time)
	// 		{
	// 			t_last_int=fasterTime;
	// 			Integrated_X.clear();
	// 			Integrated_X.resize(N_STRIPS);
	// 			Integrated_Y.clear();
	// 			Integrated_Y.resize(N_STRIPS);
	// 		}
	// 	}
	// }
	cVisu->Destructor();
	TG_ChargeTime->Delete();
	// free(vect_time_tot);
	// free(vect_charge_cumul);
	*/
}
