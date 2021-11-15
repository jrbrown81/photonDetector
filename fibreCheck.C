{
// Simple macro to plot data taken with various fibres/plastic scintillators connected to sipm/chn 1

	gStyle->SetOptStat(0);

	TFile *file[2][16];
	file[0][0] = TFile::Open("run_50/FILTERED/DataF_run_50.root"); // fibre 0
	file[1][0] = TFile::Open("run_51/FILTERED/DataF_run_51.root");
	file[0][1] = TFile::Open("run_35/FILTERED/DataF_run_35.root"); // fibre 1
	file[1][1] = TFile::Open("run_33/FILTERED/DataF_run_33.root");
	file[0][2] = TFile::Open("run_36/FILTERED/DataF_run_36.root"); // fibre 2
	file[1][2] = TFile::Open("run_37/FILTERED/DataF_run_37.root");
	file[0][3] = TFile::Open("run_39/FILTERED/DataF_run_39.root"); // fibre 3
	file[1][3] = TFile::Open("run_38/FILTERED/DataF_run_38.root");
	file[0][4] = TFile::Open("run_41/FILTERED/DataF_run_41.root"); // fibre 4
	file[1][4] = TFile::Open("run_42/FILTERED/DataF_run_42.root");
	file[0][5] = TFile::Open("run_44/FILTERED/DataF_run_44.root"); // fibre 5
	file[1][5] = TFile::Open("run_43/FILTERED/DataF_run_43.root");
	file[0][6] = TFile::Open("run_46/FILTERED/DataF_run_46.root"); // fibre 6
	file[1][6] = TFile::Open("run_47/FILTERED/DataF_run_47.root");
	file[0][7] = TFile::Open("run_49/FILTERED/DataF_run_49.root"); // fibre 7
	file[1][7] = TFile::Open("run_48/FILTERED/DataF_run_48.root");

	TH1D* spec[8];
	TH1D* back[8];
	
	TCanvas* c1 = new TCanvas("c1","");
	c1->Divide(4,2);
	for(int i=0;i<8;i++) {
		c1->cd(i+1);
		gPad->SetLogy(1);
		file[1][i]->cd();
		Data_F->Draw("Energy>>htemp(1000,0,2500)");
		spec[i] = (TH1D*)gPad->GetPrimitive("htemp");
		file[0][i]->cd();
		Data_F->Draw("Energy>>htemp");
		back[i] = (TH1D*)gPad->GetPrimitive("htemp");
		
		spec[i]->SetTitle(Form("Fibre %i Co60 spectrum",i));
		spec[i]->GetXaxis()->SetTitle("channel");
		spec[i]->GetYaxis()->SetTitle("Counts");
		back[i]->SetTitle(Form("Fibre %i background spectrum",i));
		spec[i]->SetLineColor(2);
		spec[i]->Draw();
		back[i]->Draw("same");
	}
	
}
