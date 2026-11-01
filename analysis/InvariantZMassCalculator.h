//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Feb 28 10:11:03 2014 by ROOT version 5.34/09
// from TTree tree/tree
// found on file: ZZTo2e2mu_8TeV_tree.root
//////////////////////////////////////////////////////////

#ifndef InvariantZMassCalculator_h
#define InvariantZMassCalculator_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class InvariantZMassCalculator {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Int_t           isdata;
   Int_t           nElectrons;
   Double_t        pX_electron[3];   //[nElectrons]
   Double_t        pY_electron[3];   //[nElectrons]
   Double_t        pZ_electron[3];   //[nElectrons]
   Double_t        E_electron[3];   //[nElectrons]
   Int_t           charge_electron[3];   //[nElectrons]
   Int_t           nMuons;
   Double_t        pX_muon[3];   //[nMuons]
   Double_t        pY_muon[3];   //[nMuons]
   Double_t        pZ_muon[3];   //[nMuons]
   Double_t        E_muon[3];   //[nMuons]
   Int_t           charge_muon[3];   //[nMuons]
   Int_t           nJets;
   Double_t        pX_jet[7];   //[nJets]
   Double_t        pY_jet[7];   //[nJets]
   Double_t        pZ_jet[7];   //[nJets]
   Double_t        E_jet[7];   //[nJets]
   Double_t        missingEt;
   Double_t        pu_weight;

   // List of branches
   TBranch        *b_isdata;   //!
   TBranch        *b_nElectrons;   //!
   TBranch        *b_pX_electron;   //!
   TBranch        *b_pY_electron;   //!
   TBranch        *b_pZ_electron;   //!
   TBranch        *b_E_electron;   //!
   TBranch        *b_charge_electron;   //!
   TBranch        *b_nMuons;   //!
   TBranch        *b_pX_muon;   //!
   TBranch        *b_pY_muon;   //!
   TBranch        *b_pZ_muon;   //!
   TBranch        *b_E_muon;   //!
   TBranch        *b_charge_muon;   //!
   TBranch        *b_nJets;   //!
   TBranch        *b_pX_jet;   //!
   TBranch        *b_pY_jet;   //!
   TBranch        *b_pZ_jet;   //!
   TBranch        *b_E_jet;   //!
   TBranch        *b_missingEt;   //!
   TBranch        *b_pu_weight;   //!

   InvariantZMassCalculator(TTree *tree=0);
   virtual ~InvariantZMassCalculator();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual TH1D*     Loop(string channel, string leptonType);
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef InvariantZMassCalculator_cxx
InvariantZMassCalculator::InvariantZMassCalculator(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("ZZTo2e2mu_8TeV_tree.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("ZZTo2e2mu_8TeV_tree.root");
      }
      f->GetObject("tree",tree);

   }
   Init(tree);
}

InvariantZMassCalculator::~InvariantZMassCalculator()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t InvariantZMassCalculator::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t InvariantZMassCalculator::LoadTree(Long64_t entry)
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

void InvariantZMassCalculator::Init(TTree *tree)
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

   fChain->SetBranchAddress("isdata", &isdata, &b_isdata);
   fChain->SetBranchAddress("nElectrons", &nElectrons, &b_nElectrons);
   fChain->SetBranchAddress("pX_electron", pX_electron, &b_pX_electron);
   fChain->SetBranchAddress("pY_electron", pY_electron, &b_pY_electron);
   fChain->SetBranchAddress("pZ_electron", pZ_electron, &b_pZ_electron);
   fChain->SetBranchAddress("E_electron", E_electron, &b_E_electron);
   fChain->SetBranchAddress("charge_electron", charge_electron, &b_charge_electron);
   fChain->SetBranchAddress("nMuons", &nMuons, &b_nMuons);
   fChain->SetBranchAddress("pX_muon", pX_muon, &b_pX_muon);
   fChain->SetBranchAddress("pY_muon", pY_muon, &b_pY_muon);
   fChain->SetBranchAddress("pZ_muon", pZ_muon, &b_pZ_muon);
   fChain->SetBranchAddress("E_muon", E_muon, &b_E_muon);
   fChain->SetBranchAddress("charge_muon", charge_muon, &b_charge_muon);
   fChain->SetBranchAddress("nJets", &nJets, &b_nJets);
   fChain->SetBranchAddress("pX_jet", pX_jet, &b_pX_jet);
   fChain->SetBranchAddress("pY_jet", pY_jet, &b_pY_jet);
   fChain->SetBranchAddress("pZ_jet", pZ_jet, &b_pZ_jet);
   fChain->SetBranchAddress("E_jet", E_jet, &b_E_jet);
   fChain->SetBranchAddress("missingEt", &missingEt, &b_missingEt);
   fChain->SetBranchAddress("pu_weight", &pu_weight, &b_pu_weight);
   Notify();
}

Bool_t InvariantZMassCalculator::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void InvariantZMassCalculator::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t InvariantZMassCalculator::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
	fChain->Show(entry);
	return 1;
}
#endif // #ifdef InvariantZMassCalculator_cxx
