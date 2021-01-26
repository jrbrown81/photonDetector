#define photonSorter_cxx
#include "photonSorter.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

TString filename;

//
//	Base code generated from DataF_run_10.root using MakeClass().
// To run on other files do:

// run("filename.root")

/*
	TFile* file = TFile::Open("filename.root")
	TTree* tree
	file->GetObject("Data_F",tree)
	.L photonSorter.C
	photonSorter t(tree)
	t.Loop()
*/

void photonSorter::Loop(Int_t toProcess=0)
{
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();
   cout << "Found " << nentries << " entries in file" << endl;
   if(toProcess!=0) {
		nentries = toProcess;
		cout << "Processing " << nentries << " entries" << endl;
	}
	else cout << "Processing all entries" << endl;

   float m[8]={34.98255,86.9675,84.3465,87.3825,84.1565,88.5615,84.5895,90.3245};
   float c[8] = {4.011,5.753,7.893,-2.089,3.837,2.271,1.871,0.329};

   mult = 1;
   tBuff = 0;
   chnBuff = -1;
   lastHit=-1;
   firstHit=10;
   eBuff = 0;

	for(int i=0;i<8;i++) multOverThresh[i]=0;

//   top=0;
//   bottom=0;
   
   hitPattern_h = new TH1F("hitPattern_h","Hit Pattern",9,0,9);
   hitPattern_eCut_h = new TH1F("hitPattern_eCut_h",Form("Hit Pattern, Energy>%f",eCut),9,0,9);
   EvsChn_h = new TH2F("EvsChn_h","Energy vs Channel",9,0,9,100,0,50);
   for(int i=0; i<8; i++) {
		energyRaw_h[i] = new TH1F(Form("energyRaw_%i_h",i),Form("Energy (Raw), channel %i",i),1000,0,5000);
		energy_h[i] = new TH1F(Form("energy_%i_h",i),Form("Energy, channel %i",i),1000,0,50);
		energy2_h[i] = new TH1F(Form("energy2_%i_h",i),Form("Energy, channel %i (mult>=2)",i),1000,0,5000);
		energy3_h[i] = new TH1F(Form("energy3_%i_h",i),Form("Energy, channel %i (top and bottom hit)",i),1000,0,5000);
//   	energyCal_h[i] = new TH1F(Form("energyCal_%i_h",i),Form("Energy (calibrated), channel %i",i),1000,0,50);
		hitPattArr_h[i] = new TH1F(Form("hitPattern_%i_h",i),Form("Hit Pattern for mult=%i",i+1),9,0,9);
	}
   
   mult_h = new TH1F("mult_h",Form("Multiplicity (tDiff<%llu ns)",tCut),9,0,9);
   mult2_h = new TH1F("mult2_h",Form("Multiplicity (tDiff<%llu ns, top and bottom hit)",tCut),9,0,9);
   tDiff_h = new TH1F("tDiff_h","Time Difference",1000,0,1e3);
   tDiff_h->GetXaxis()->SetTitle("Time Difference (ns)");
   tDiffLong_h = new TH1F("tDiffLong_h","Time Difference (long)",10000,0,5e8);
   tDiffLong_h->GetXaxis()->SetTitle("Time Difference (ns)");
   coincPattern_h = new TH2F("coincPattern_h","Coincidence Pattern",9,0,9,9,0,9);
   coincPattern_h->GetXaxis()->SetTitle("Channel");
   coincPattern_h->GetYaxis()->SetTitle("Channel");
   coincEnergy_h = new TH2F("coincEnergy_h","Coincidnece Energy Matrix",100,0,50,100,0,50);
   coincEnergy_h->GetXaxis()->SetTitle("Energy (p.e.)");
   coincEnergy_h->GetYaxis()->SetTitle("Energy (p.e.)");
   
   mult_c_h = (TH1F*)mult_h->Clone("mult_c_h");
   mult_c_h->SetTitle(Form("Multiplicity (E>%f)",eCut));
   tDiff_c_h = (TH1F*)tDiff_h->Clone("tDiff_c_h");
   tDiff_c_h->SetTitle(Form("Time Difference (E>%f)",eCut));
   coincPattern_c_h = (TH2F*)coincPattern_h->Clone("coincPattern_c_h");
   coincPattern_c_h->SetTitle(Form("Coincidence Pattern (E>%f)",eCut));
//   coincEnergy_e_h = (TH2F*)coincEnergy_h->Clone("coincEnergy_h");
//   coincEnergy_e_h->SetTitle(Form("Coincidnece Energy Matrix (E>%f)",eCut));
   coincEnergy_n_h = (TH2F*)coincEnergy_h->Clone("coincEnergy_n_h");
   coincEnergy_n_h->SetTitle("Coincidnece Energy Matrix for neighbouring chns");
	coincEnergy_g_h = (TH2F*)coincEnergy_h->Clone("coincEnergy_g_h");
   coincEnergy_g_h->SetTitle("Coincidnece Energy Matrix for good chns");
	coincEnergy_c_h = (TH2F*)coincEnergy_h->Clone("coincEnergy_c_h");
   coincEnergy_c_h->SetTitle("Coincidnece Energy Matrix for good, neighbouring chns for multiplicity>2");

   for(int i=0;i<8;i++) coincPatternThresh_h[i] = new TH2F(Form("coincPatternThresh%i_h",i),Form("Coincidence Pattern, E>%f p.e.",thresh[i]),9,0,9,9,0,9);
   
   lastHit_h = new TH1F("lastHit_h",Form("Last scintillator hit in event (E>%f)",eCut),9,0,9);
   lastHitVsMult_h = new TH2F("lastHitVsMult_h",Form("Last scintillator hit vs multiplicity (E>%f)",eCut),9,0,9,9,0,9);
   lastHitVsMult_h->GetXaxis()->SetTitle("Last scintillator hit");
   lastHitVsMult_h->GetYaxis()->SetTitle("Multiplicity");
	
	firstHit_h = new TH1F("firstHit_h",Form("First scintillator hit in event (E>%f)",eCut),9,0,9);
//   firstHitVsMult_h = new TH2F("firstHitVsMult_h","Last scintillator hit vs multiplicity (E cuts)",9,0,9,9,0,9);
//   firstHitVsMult_h->GetXaxis()->SetTitle("Last scintillator hit");
//   firstHitVsMult_h->GetYaxis()->SetTitle("Multiplicity");

	multVsThresh_h = new TH2F("multVsThresh_h","Multiplicity vs Threshold",10,0,50,9,0,9);
	
   Long64_t nbytes = 0, nb = 0;
// Loop over entries
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      if(jentry!=0 && jentry%100000==0) cout << jentry << endl;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;
      
      tDiff = (Timestamp - tBuff)/1000;
      energyRaw_h[Channel]->Fill(Energy);
		e=(Energy-c[Channel])/m[Channel];
      
//      if(tDiff==0) {
      if(tDiff<tCut) {		// coincidnece condition
			mult++;
			hits[Channel]++;
			energyBuff[Channel]=Energy;
      
			coincPattern_h->Fill(chnBuff,Channel);
			coincPattern_h->Fill(Channel,chnBuff);
//			if(mult>2) {
				coincEnergy_h->Fill(eBuff,e);
				coincEnergy_h->Fill(e,eBuff);
//			}
			
//			if(Channel!=4 && chnBuff!=4){
				coincEnergy_g_h->Fill(eBuff,e);
				coincEnergy_g_h->Fill(e,eBuff);
//			}
			
			if(TMath::Abs(Channel-chnBuff)==1) {
				coincEnergy_n_h->Fill(eBuff,e);
				coincEnergy_n_h->Fill(e,eBuff);
//				if(Channel!=4 && chnBuff!=4 && mult>2){
					coincEnergy_c_h->Fill(eBuff,e);
					coincEnergy_c_h->Fill(e,eBuff);
//				}
			}
			
			if(e>eCut) {
				mult_eCut++;
				if(Channel>lastHit) lastHit=Channel;
				if(Channel<firstHit) firstHit=Channel;
				if(eBuff>eCut) {
					coincPattern_c_h->Fill(chnBuff,Channel);
					coincPattern_c_h->Fill(Channel,chnBuff);
				}
//				if(Channel==1) topHit=1;
//				if(Channel==7) bottomHit=1;
			}
			for(int i=0;i<8;i++) {
				if(e>thresh[i]) {
					multOverThresh[i]++;
					if(eBuff>thresh[i]) {
						coincPatternThresh_h[i]->Fill(chnBuff,Channel);
						coincPatternThresh_h[i]->Fill(Channel,chnBuff);
					}
				} else break;
			}
		} else {				// not coincidence
			mult_h->Fill(mult);
//			if(e>eCut && eBuff>eCut) mult_c_h->Fill(mult);
			mult_c_h->Fill(mult_eCut);
			lastHit_h->Fill(lastHit);
			firstHit_h->Fill(firstHit);
			lastHitVsMult_h->Fill(lastHit,mult_eCut);
			for(int i=0;i<8;i++) {
				if(multOverThresh[i]!=0) multVsThresh_h->Fill(thresh[i],multOverThresh[i]);
				if(e>thresh[i]) multOverThresh[i]=1;
				else multOverThresh[i]=0;
			}
//			if(topHit && bottomHit) hitPattern_full_h->
			if(e>eCut) {
				mult_eCut = 1;
				lastHit=Channel;
				firstHit=Channel;
//				if(Channel==1) {
//					topHit=1;
//					bottomHit=0;
//				}
//				if(Channel==7) {
//					topHit=0;
//					bottomHit=1;
//				}
			}
			else {
				mult_eCut = 0;
				lastHit = -1;
				firstHit = 10;
//				topHit=0;
//				bottomHit=0;
			}
			for(int fibre=0;fibre<8;fibre++) if(hits[fibre]==1) hitPattArr_h[mult-1]->Fill(fibre);
			if(hits[0]!=0 && hits[7]!=0) {
				mult2_h->Fill(mult);
				for(int i=0;i<8;i++) energy3_h[i]->Fill(energyBuff[i]);
			}
			
			// Reset variables
			mult = 1;
			for(int i=0;i<8;i++) {
				hits[i]=0;
				energyBuff[i]=0;
			}
			hits[Channel]++;
			energyBuff[Channel]=Energy;
		}	// end of not coincidence
      
      tDiff_h->Fill(tDiff);
      tDiffLong_h->Fill(tDiff);
            
      energy_h[Channel]->Fill(e);
//      if(mult>=2) energy2_h[Channel]->Fill(e);
		if(mult>=2) energy2_h[Channel]->Fill(Energy);
//      energyCal_h[Channel]->Fill((Energy-c[Channel])/m[Channel]);
      EvsChn_h->Fill(Channel,e);
      hitPattern_h->Fill(Channel);
      if(e>eCut) hitPattern_eCut_h->Fill(Channel);
      
      // Set buffers
      tBuff = Timestamp;
      chnBuff = Channel;
      eBuff = e;
   }	// End of loop over entries
   
   TCanvas* c1 = new TCanvas("basics_c","c1");
   c1->Divide(4,2);
   c1->cd(1);
   c1->GetPad(1)->SetLogy(1);
   tDiffLong_h->Draw();
   c1->cd(2);
   mult_h->Draw();
   c1->cd(3);
   coincPattern_h->SetStats(0);
   coincPattern_h->Draw("colz");
   c1->cd(4);
   coincEnergy_h->SetStats(0);
   coincEnergy_h->Draw("colz");
   c1->cd(5);
   c1->GetPad(5)->SetLogy(1);
   tDiff_h->Draw();
   c1->cd(6);
   mult_c_h->Draw();
   c1->cd(7);
   coincPattern_c_h->SetStats(0);
   coincPattern_c_h->Draw("colz");
   c1->cd(8);
   mult2_h->Draw();
//   coincEnergy_e_h->SetStats(0);
//   coincEnergy_e_h->Draw("colz");

	TCanvas* coinc_c = new TCanvas("coinc_c","Coincidence plots");
	coinc_c->Divide(2,2);
//	coinc_c->cd(1);
//   coincPattern_h->Draw("colz");
	coinc_c->cd(1);
   coincEnergy_h->Draw("colz");
	coinc_c->cd(2);
   coincEnergy_g_h->Draw("colz");
	coinc_c->cd(3);
	coincEnergy_n_h->Draw("colz");
	coinc_c->cd(4);
	coincEnergy_c_h->Draw("colz");
	
	TCanvas* energyRaw_c=new TCanvas("energyRaw_c","Energy Spectra");
	TCanvas* energy_c=new TCanvas("energy_c","Raw Energy Spectra");
	TCanvas* energyMult_c=new TCanvas("energyMult_c","Energy Spectra for mult>=2");
	TCanvas* c2=new TCanvas("energyTopBottom_c","Energy Spectra for top and bottom hits");
	energyRaw_c->Divide(4,2);
	energy_c->Divide(4,2);
	energyMult_c->Divide(4,2);
	c2->Divide(4,2);
	for(int i=0; i<8; i++) {
		energy_c->cd(i+1);
		energy_h[i]->Draw();
		energyRaw_c->cd(i+1);
		gPad->SetLogy(1);
		energyRaw_h[i]->Draw();
		energyMult_c->cd(i+1);
		gPad->SetLogy(1);
		energyRaw_h[i]->Draw();
		energy2_h[i]->SetLineColor(2);
		energy2_h[i]->Draw("same");
		c2->cd(i+1);
		gPad->SetLogy(1);
		energyRaw_h[i]->Draw();
		energy2_h[i]->Draw("same");
		energy3_h[i]->SetLineColor(3);
		energy3_h[i]->Draw("same");
	}
	
	TCanvas* c3=new TCanvas("energySpec_c","Energy Spectra (log)");
	c3->Divide(4,2);
	for(int i=0; i<8; i++) {
		c3->cd(i+1);
		c3->GetPad(i+1)->SetLogy(1);
		energy_h[i]->DrawCopy();
	}
	
	TCanvas* c4 = new TCanvas("hitPatt_c","Hit Patterns");
	c4->Divide(2,2);
	c4->cd(1);
	hitPattern_h->Draw();
	c4->cd(2);
	hitPattern_eCut_h->Draw();
	c4->cd(3);
	c4->GetPad(3)->SetLogz(1);
	EvsChn_h->SetStats(0);
	EvsChn_h->Draw("colz");
	
	TCanvas* c5 = new TCanvas("hitPattMult_c","Hit Patterns for different multiplicities");
	c5->Divide(4,2);
	for(int i=0;i<8;i++) {
		c5->cd(i+1);
		hitPattArr_h[i]->Draw();
	}

	TCanvas* c6 = new TCanvas("lastHits_c","Last hit plots");
	c6->Divide(2,2);
	c6->cd(1);
	hitPattern_eCut_h->Draw();
	c6->cd(2);
	mult_c_h->Draw();
	c6->cd(3);
	lastHit_h->Draw();
	c6->cd(4);
	lastHitVsMult_h->SetStats(0);
	lastHitVsMult_h->Draw("colz");
//	firstHit_h->Draw();

	TCanvas* c7=new TCanvas("multOverThresh_c","Multiplicity over threshold");
	multVsThresh_h->Draw("colz");
	
	TCanvas* c8 = new TCanvas("coincPatt_c","Coincidence patterns over thresholds");
	c8->Divide(4,2);
	for(int i=0;i<8;i++) {
		c8->cd(i+1);
		coincPatternThresh_h[i]->SetStats(0);
		coincPatternThresh_h[i]->Draw("colz");
	}
	
//	TString outfile=filename;
	filename.ReplaceAll(".root","_out.root");
	cout << "Writing output file: " << filename << endl;
	TFile* outFile=TFile::Open(filename,"RECREATE");
	c1->Write();
	coinc_c->Write();
	energyRaw_c->Write();
	energyMult_c->Write();
	c2->Write();
	c3->Write();
	c4->Write();
	c5->Write();
	c6->Write();
	c7->Write();
	c8->Write();
	for(int i=0;i<8;i++) energyRaw_h[i]->Write();

	delete outFile;
}

void run(TString input, Int_t toProcess=0)
{
	filename=input;
	TFile f1(filename);
	TTree* tree;
	f1.GetObject("Data_F",tree);
	if(tree) cout << "data tree found in file: " << filename << endl;
	else cout << "Error!" << endl;
	photonSorter phS(tree);
	
//	cout << "Switching to Batch mode" << endl;
//	gROOT->SetBatch(1);
	phS.Loop(toProcess);
//	gROOT->SetBatch(0);
//	cout << "Return to normal mode" << endl;

	delete tree;
}
