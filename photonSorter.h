//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Tue Sep 22 11:40:25 2020 by ROOT version 6.17/01
// from TTree Data_F/CoMPASS FILTERED events TTree
// found on file: DataF_run_10.root
//////////////////////////////////////////////////////////

#ifndef photonSorter_h
#define photonSorter_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

class photonSorter {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

	// Cuts
	Float_t eCut = 10;
	ULong64_t tCut = 20; // ns

   // Declaration of leaf types
   UShort_t        Channel;
   ULong64_t       Timestamp;
   UShort_t        Board;
   UShort_t        Energy;
   UInt_t          Flags;

   // Other variables
   UShort_t        chnBuff;
   ULong64_t       tBuff;
   ULong64_t       tDiff;
   Float_t         eBuff;
   Float_t 			 e;
   
   UInt_t          mult;
   UInt_t          mult_eCut;
   UInt_t			 lastHit;
   UInt_t			 firstHit;
   
   Float_t			thresh[8]={5,10,15,20,25,30,35,40};
   Float_t			multOverThresh[8];
   
//   Bool_t			top;
//   Bool_t			bottom;
   
   // List of branches
   TBranch        *b_Channel;   //!
   TBranch        *b_Timestamp;   //! in picoseconds
   TBranch        *b_Board;   //!
   TBranch        *b_Energy;   //!
   TBranch        *b_Flags;   //!
   
   TH1F* hitPattern_h;
   TH1F* hitPattern_eCut_h;
   TH1F* mult_h;
   TH1F* tDiff_h;
   TH1F* tDiffLong_h;
   TH2F* coincPattern_h;
   TH2F* coincEnergy_h;

   TH2F* coincPatternThresh_h[8];

   TH1F* mult_c_h;
   TH1F* tDiff_c_h;
   TH2F* coincPattern_c_h;
//   TH2F* coincEnergy_e_h;
   TH2F* coincEnergy_n_h;
   TH2F* coincEnergy_g_h;
   TH2F* coincEnergy_c_h;

   TH1F* lastHit_h;
   TH1F* firstHit_h;
   TH2F* lastHitVsMult_h;
   
   TH1F* energy_h[8];
   TH1F* energy2_h[8];
   TH1F* energyRaw_h[8];
   TH2F* EvsChn_h;
   
   TH2F* multVsThresh_h;

   photonSorter(TTree *tree=0);
   virtual ~photonSorter();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop(Int_t toProcess);
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef photonSorter_cxx
photonSorter::photonSorter(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("DataF_run_10.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("DataF_run_10.root");
      }
      f->GetObject("Data_F",tree);

   }
   Init(tree);
}

photonSorter::~photonSorter()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t photonSorter::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t photonSorter::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void photonSorter::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("Channel", &Channel, &b_Channel);
   fChain->SetBranchAddress("Timestamp", &Timestamp, &b_Timestamp);
   fChain->SetBranchAddress("Board", &Board, &b_Board);
   fChain->SetBranchAddress("Energy", &Energy, &b_Energy);
   fChain->SetBranchAddress("Flags", &Flags, &b_Flags);
   Notify();
   
}

Bool_t photonSorter::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void photonSorter::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t photonSorter::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef photonSorter_cxx
