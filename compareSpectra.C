void compareSpectra(Int_t run1, Int_t run2=0, Int_t run3=0, Int_t run4=0, Int_t run5=0, Int_t run6=0, TString opt="F"){

	gStyle->SetOptStat(0);
	Int_t nFiles=0;
/*	if(run2==0) nFiles=1;
	else if(run3==0) nFiles=2;
	else if(run4==0) nFiles=3;
	else if(run5==0) nFiles=4;
	else if(run6==0) nFiles=5;
	else nFiles=6;
*/
	Int_t run[6]; //={33,56,58,60,61,62,63};
	run[0] = run1;
	run[1] = run2;
	run[2] = run3;
	run[3] = run4;
	run[4] = run5;
	run[5] = run6;
	while(run[nFiles]!=0) nFiles++;

	TFile *file[nFiles];
	if(opt=="F") for(int i=0;i<nFiles;i++) file[i] = TFile::Open(Form("run_%i/FILTERED/DataF_run_%i_out.root",run[i],run[i]));
	if(opt=="U") for(int i=0;i<nFiles;i++) file[i] = TFile::Open(Form("run_%i/UNFILTERED/Data_run_%i_out.root",run[i],run[i]));
	if(opt=="R") for(int i=0;i<nFiles;i++) file[i] = TFile::Open(Form("run_%i/RAW/DataR_run_%i_out.root",run[i],run[i]));

	TH1F* spec[nFiles][8];

	TCanvas* c1=new TCanvas("c1","c1");
	c1->Divide(4,2);

	for(int f=0;f<nFiles;f++) {
		file[f]->cd();
		for(int i=0;i<8;i++) {
			c1->cd(i+1);
			spec[f][i]=(TH1F*)file[f]->Get(Form("energyRaw_%i_h",i));
			spec[f][i]->SetLineColor(f+1);
			spec[f][i]->SetTitle(Form("chn %i, run %i",i,run[f]));
			if(f==0) {
//				spec[f][i]->GetXaxis()->SetRangeUser(0,2500);
				spec[f][i]->Draw();
				gPad->SetLogy(1);
			}
			else spec[f][i]->Draw("same");
			if(f==4) gPad->BuildLegend(0.4,0.5,0.9,0.9);
		}
	}

}

void help() {
	cout << "\n compareSpectra(Int_t run1, Int_t run2=0, Int_t run3=0, Int_t run4=0, Int_t run5=0, Int_t run6=0, TString opt=\"F\")\n" << endl;
	cout << "Overlay spectra from up to 6 runs. Gets data from processed data files (i.e. '..._out.root') so need to have run photonSorter.C first.\n" << endl;
	cout << "Options 'opt=F/U/R' to select FILTERED, UNFILTERED or RAW data." << endl;
}
void Help(){help();}
void usage(){help();}
void Usage(){help();}
