#ifndef InvariantZZMassCalculator_h
#define InvariantZZMassCalculator_h
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <string>
#include <iostream>
using namespace std;

class InvariantZZMassCalculator
{
public:
    TTree           *fChain;      //!pointer to the analyzed TTree or TChain
    Int_t           fCurrent;    //!current Tree number in a TChain
    
    // Declaration of leaf types
    Int_t           isdata;
    Int_t           nElectrons;
    Double_t        pX_electron[10];             //[nElectrons]
    Double_t        pY_electron[10];             //[nElectrons]
    Double_t        pZ_electron[10];             //[nElectrons]
    Double_t        E_electron[10];              //[nElectrons]
    Double_t        pfIso_electron[10];          //[nElectrons]
    Int_t           charge_electron[10];         //[nElectrons]
    Int_t           nMuons;
    Double_t        pX_muon[10];                 //[nMuons]
    Double_t        pY_muon[10];                 //[nMuons]
    Double_t        pZ_muon[10];                 //[nMuons]
    Double_t        E_muon[10];                  //[nMuons]
    Double_t        pfIso_muon[10];              //[nMuons]
    Int_t           charge_muon[10];             //[nMuons]
    Int_t           nJets;
    Double_t        pX_jet[10];                  //[nJets]
    Double_t        pY_jet[10];                  //[nJets]
    Double_t        pZ_jet[10];                  //[nJets]
    Double_t        E_jet[10];                   //[nJets]
    Double_t        missingEt;
    Double_t        pu_weight;
    
    // List of branches
    TBranch        *b_isdata;                   //!
    TBranch        *b_nElectrons;               //!
    TBranch        *b_pX_electron;              //!
    TBranch        *b_pY_electron;              //!
    TBranch        *b_pZ_electron;              //!
    TBranch        *b_E_electron;               //!
    TBranch        *b_pfIso_electron;           //!
    TBranch        *b_charge_electron;          //!
    TBranch        *b_nMuons;                   //!
    TBranch        *b_pX_muon;                  //!
    TBranch        *b_pY_muon;                  //!
    TBranch        *b_pZ_muon;                  //!
    TBranch        *b_E_muon;                   //!
    TBranch        *b_pfIso_muon;               //!
    TBranch        *b_charge_muon;              //!
    TBranch        *b_nJets;                    //!
    TBranch        *b_pX_jet;                   //!
    TBranch        *b_pY_jet;                   //!
    TBranch        *b_pZ_jet;                   //!
    TBranch        *b_E_jet;                    //!
    TBranch        *b_missingEt;                //!
    TBranch        *b_pu_weight;                //!
    
    InvariantZZMassCalculator(TTree *tree=0);
    virtual ~InvariantZZMassCalculator();
    virtual Int_t    Cut(Long64_t entry);
    virtual Int_t    GetEntry(Long64_t entry);
    virtual Long64_t LoadTree(Long64_t entry);
    virtual void     Init(TTree *tree);
    virtual TH1D*    Loop(string channel, int bins, int lower, int upper, bool blind, bool cut);
    virtual Bool_t   Notify();
    virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef InvariantZZMassCalculator_cxx
//Constructor syntax: ClassName ObjectName (Arguments)
InvariantZZMassCalculator::InvariantZZMassCalculator(TTree *tree)
{
    if (tree == 0)
    {
        cout<<"ERROR: No input tree defined!"<<endl;
    }
    else
    {
        Init(tree);
    }
}

InvariantZZMassCalculator::~InvariantZZMassCalculator()
{
    if (!fChain) return;
    delete fChain->GetCurrentFile();
}

Int_t InvariantZZMassCalculator::GetEntry(Long64_t entry)
{
    // Read contents of entry.
    if (!fChain) return 0;
    return fChain->GetEntry(entry);
}

Long64_t InvariantZZMassCalculator::LoadTree(Long64_t entry)
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

void InvariantZZMassCalculator::Init(TTree *tree)
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
    fChain->SetBranchAddress("pfIso_electron", pfIso_electron, &b_pfIso_electron);
    fChain->SetBranchAddress("charge_electron", charge_electron, &b_charge_electron);
    fChain->SetBranchAddress("nMuons", &nMuons, &b_nMuons);
    fChain->SetBranchAddress("pX_muon", pX_muon, &b_pX_muon);
    fChain->SetBranchAddress("pY_muon", pY_muon, &b_pY_muon);
    fChain->SetBranchAddress("pZ_muon", pZ_muon, &b_pZ_muon);
    fChain->SetBranchAddress("E_muon", E_muon, &b_E_muon);
    fChain->SetBranchAddress("pfIso_muon", pfIso_muon, &b_pfIso_muon);
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

Bool_t InvariantZZMassCalculator::Notify()
{
    // The Notify() function is called when a new file is opened. This
    // can be either for a new TTree in a TChain or when when a new TTree
    // is started when using PROOF. It is normally not necessary to make changes
    // to the generated code, but the routine can be extended by the
    // user if needed. The return value is currently not used.
    
    return kTRUE;
}

void InvariantZZMassCalculator::Show(Long64_t entry)
{
    // Print contents of entry.
    // If entry is not specified, print current entry
    if (!fChain) return;
    fChain->Show(entry);
}


Int_t InvariantZZMassCalculator::Cut(Long64_t entry)
{
    // This function may be called from Loop.
    // returns  1 if entry is accepted.
    // returns -1 otherwise.
	fChain->Show(entry);

    return 1;
}
#endif //#ifdef InvariantZZMassCalculator_cxx
