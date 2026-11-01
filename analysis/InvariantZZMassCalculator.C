//Preprocessor directives
#define InvariantZZMassCalculator_cxx
#include <TLorentzVector.h>
#include "InvariantZZMassCalculator.h"
#include <TH1.h>
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>
using namespace std;

TH1D* InvariantZZMassCalculator::Loop(string channel, int nBins, int lower, int upper, bool blind, bool cut)
{
    if(channel=="4e")
    {
        //Make sure fchain is not zero
        cout<<"Checkpoint A1"<<endl;
        if(fChain==0) return 0;
        
        //Define some needed variables
        cout<<"Checkpoint A2"<<endl;
        float nbytes=0, nb=0;
        float nentries=fChain->GetEntriesFast();
        
        //Initialise the histogram
        cout<<"Checkpoint A3"<<endl;
        TH1D *eeeeMass=new TH1D("eeeeMass", "", nBins, lower, upper);
        
        //Enabeling bin errors
        cout<<"Checkpoint A4"<<endl;
        eeeeMass->Sumw2();
        
        //Loop over all entries
        cout<<"Checkpoint A5"<<endl;
        for(int jentry=0; jentry<nentries; jentry++)
        {
            //Debugging
            if(jentry%1000==0)
            {
                cout<<"Checkpoint A5."<<jentry<<endl;
            }
            
            //Load the entry
            float ientry=LoadTree(jentry);
            if (ientry<0) break;
            nb=fChain->GetEntry(jentry);
            nbytes+=nb;
            
            //Check conservation of charge with pile up
            int totalElectronCharge;
            int goodElectrons;
            int goodElectron1, goodElectron2, goodElectron3, goodElectron4;
            totalElectronCharge=0;
            goodElectrons=0;
            goodElectron1=goodElectron2=goodElectron3=goodElectron4=-1;
            
            //Loop over all electrons in the entry, checking charge and pile up
            for(int electronNumber=0; electronNumber<nElectrons; electronNumber++)
            {
                if(pfIso_electron[electronNumber]>0.05) continue;
                goodElectrons++;
                totalElectronCharge+=charge_electron[electronNumber];
                if(goodElectron1==-1) goodElectron1=electronNumber;
                else goodElectron2=electronNumber;
                if(goodElectron3==-1) goodElectron3=electronNumber;
                else goodElectron4=electronNumber;
            }
            
            //Select the desired eeee events
            if(goodElectrons==4 && totalElectronCharge==0)
            {
                //Define the needed variables
                float Z1Mass=0.0;
                float Z2Mass=0.0;
                float ZZMass=0.0;
                TLorentzVector Lorentz0(0.0,0.0,0.0,0.0);
                TLorentzVector Lorentz1(0.0,0.0,0.0,0.0);
                TLorentzVector Lorentz2(0.0,0.0,0.0,0.0);
                TLorentzVector Lorentz3(0.0,0.0,0.0,0.0);
                
                //Set the needed Lorentz vectors
                Lorentz0.SetPx(pX_electron[goodElectron1]);
                Lorentz0.SetPy(pY_electron[goodElectron1]);
                Lorentz0.SetPz(pZ_electron[goodElectron1]);
                Lorentz0.SetE(  E_electron[goodElectron1]);
                
                Lorentz1.SetPx(pX_electron[goodElectron2]);
                Lorentz1.SetPy(pY_electron[goodElectron2]);
                Lorentz1.SetPz(pZ_electron[goodElectron2]);
                Lorentz1.SetE(  E_electron[goodElectron2]);
                
                Lorentz2.SetPx(pX_electron[goodElectron3]);
                Lorentz2.SetPy(pY_electron[goodElectron3]);
                Lorentz2.SetPz(pZ_electron[goodElectron3]);
                Lorentz2.SetE(  E_electron[goodElectron3]);
                
                Lorentz3.SetPx(pX_electron[goodElectron4]);
                Lorentz3.SetPy(pY_electron[goodElectron4]);
                Lorentz3.SetPz(pZ_electron[goodElectron4]);
                Lorentz3.SetE(  E_electron[goodElectron4]);
                
                Z1Mass=(Lorentz0+Lorentz1).M();
                Z2Mass=(Lorentz2+Lorentz3).M();
                ZZMass=(Lorentz0+Lorentz1+Lorentz2+Lorentz3).M();
                
                if(ZZMass<100  && cut) continue;
                if(ZZMass>140 && cut) continue;
                if(blind)
                    if(ZZMass>110 && ZZMass<140) continue;
                eeeeMass->Fill(ZZMass,pu_weight);
            }
        }
        
        //Return the histogram
        cout<<"Checkpoint A6"<<endl;
        cout<<""<<endl;
        cout<<""<<endl;
        cout<<""<<endl;
        
        return eeeeMass;
    }
    
    else if(channel=="2e2mu")
    {
        //Make sure fchain is not zero
        cout<<"Checkpoint A1"<<endl;
        if(fChain==0) return 0;
        
        //Define some needed variables
        cout<<"Checkpoint A2"<<endl;
        float nbytes=0, nb=0;
        float nentries=fChain->GetEntriesFast();
        
        //Initialise the histogram
        cout<<"Checkpoint A3"<<endl;
        TH1D *eemumuMass=new TH1D("eemumuMass", "", nBins, lower, upper);
        
        //Enabeling bin errors
        cout<<"Checkpoint A4"<<endl;
        eemumuMass->Sumw2();
        
        //Loop over all entries
        cout<<"Checkpoint A5"<<endl;
        for(int jentry=0; jentry<nentries; jentry++)
        {
            //Debugging
            if (jentry%1000==0)
            {
                cout<<"Checkpoint A5."<<jentry<<endl;
            }
            
            //Load the entry
            float ientry=LoadTree(jentry);
            if(ientry<0) break;
            nb=fChain->GetEntry(jentry);
            nbytes+=nb;
            
            //Check conservation of charge with pile up
            int totalElectronCharge, totalMuonCharge;
            int goodElectrons, goodMuons;
            int goodElectron1, goodElectron2, goodMuon1, goodMuon2;
            totalElectronCharge=totalMuonCharge=0;
            goodElectrons=goodMuons=0;
            goodElectron1=goodElectron2=goodMuon1=goodMuon2=-1;
            
            //Loop over all electrons, muons in the entry, checking charge and pile up
            for(int electronNumber=0; electronNumber<nElectrons; electronNumber++)
            {
                if(pfIso_electron[electronNumber]>0.05) continue;
                goodElectrons++;
                totalElectronCharge+=charge_electron[electronNumber];
                if(goodElectron1==-1) goodElectron1=electronNumber;
                else goodElectron2=electronNumber;
            }
            for(int muonNumber=0; muonNumber<nMuons; muonNumber++)
            {
                if(pfIso_muon[muonNumber]>0.05) continue;
                goodMuons++;
                totalMuonCharge += charge_muon[muonNumber];
                if(goodMuon1==-1)goodMuon1=muonNumber;
                else goodMuon2=muonNumber;
            }
            
            //Select the desired eemumu event
            if(goodElectrons==2 && totalElectronCharge==0 && goodMuons==2 && totalMuonCharge==0)
            {
                //Define the needed variables
                float Z1Mass=0.0;
                float Z2Mass=0.0;
                float ZZMass=0.0;
                TLorentzVector Lorentz0(0.0,0.0,0.0,0.0);
                TLorentzVector Lorentz1(0.0,0.0,0.0,0.0);
                TLorentzVector Lorentz2(0.0,0.0,0.0,0.0);
                TLorentzVector Lorentz3(0.0,0.0,0.0,0.0);
                
                //Set the needed Lorentz vectors
                Lorentz0.SetPx(pX_electron[goodElectron1]);
                Lorentz0.SetPy(pY_electron[goodElectron1]);
                Lorentz0.SetPz(pZ_electron[goodElectron1]);
                Lorentz0.SetE(  E_electron[goodElectron1]);
                
                Lorentz1.SetPx(pX_electron[goodElectron2]);
                Lorentz1.SetPy(pY_electron[goodElectron2]);
                Lorentz1.SetPz(pZ_electron[goodElectron2]);
                Lorentz1.SetE(  E_electron[goodElectron2]);
                
                Lorentz2.SetPx(pX_muon[goodMuon1]);
                Lorentz2.SetPy(pY_muon[goodMuon1]);
                Lorentz2.SetPz(pZ_muon[goodMuon1]);
                Lorentz2.SetE(  E_muon[goodMuon1]);
                
                Lorentz3.SetPx(pX_muon[goodMuon2]);
                Lorentz3.SetPy(pY_muon[goodMuon2]);
                Lorentz3.SetPz(pZ_muon[goodMuon2]);
                Lorentz3.SetE(  E_muon[goodMuon2]);
                
                Z1Mass=(Lorentz0+Lorentz1).M();
                Z2Mass=(Lorentz2+Lorentz3).M();
                ZZMass=(Lorentz0+Lorentz1+Lorentz2+Lorentz3).M();
                
                if(ZZMass<100 && cut) continue;
                if(ZZMass>140 && cut) continue;
                if(blind)
                    if(ZZMass>110 && ZZMass<140) continue;
                eemumuMass->Fill(ZZMass,pu_weight);
            }
        }
        
        //Return the histogram
        cout<<"Checkpoint A6"<<endl;
        cout<<""<<endl;
        cout<<""<<endl;
        cout<<""<<endl;
        
        return eemumuMass;
    }
    
    else if(channel=="4mu")
    {
        //Make sure fchain is not zero
        cout<<"Checkpoint A1"<<endl;
        if(fChain==0) return 0;
        
        //Define some needed variables
        cout<<"Checkpoint A2"<<endl;
        float nbytes=0, nb=0;
        float nentries=fChain->GetEntriesFast();
        
        //Initialise the histogram
        cout<<"Checkpoint A3"<<endl;
        TH1D *mumumumuMass=new TH1D("mumumumuMass", "", nBins, lower, upper);
        
        //Enabeling bin errors
        cout<<"Checkpoint A4"<<endl;
        mumumumuMass->Sumw2();
        
        //Loop over all entries
        cout<<"Checkpoint A5"<<endl;
        for(int jentry=0; jentry<nentries; jentry++)
        {
            //Debugging
            if(jentry%1000==0)
            {
                cout<<"Checkpoint A5."<<jentry<<endl;
            }
            
            //Load the entry
            float ientry=LoadTree(jentry);
            if (ientry<0) break;
            nb=fChain->GetEntry(jentry);
            nbytes+=nb;
            
            //Check conservation of charge with pile up
            int totalMuonCharge;
            int goodMuons;
            int goodMuon1, goodMuon2, goodMuon3, goodMuon4;
            totalMuonCharge=0;
            goodMuons=0;
            goodMuon1=goodMuon2=goodMuon3=goodMuon4=-1;
            
            //Loop over all  muons in the entry, checking charge and pile up
            for(int muonNumber=0; muonNumber<nMuons; muonNumber++)
            {
                if(pfIso_muon[muonNumber]>0.05) continue;
                goodMuons++;
                totalMuonCharge+=charge_muon[muonNumber];
                if(goodMuon1==-1) goodMuon1=muonNumber;
                else goodMuon2=muonNumber;
                if(goodMuon3==-1) goodMuon3=muonNumber;
                else goodMuon4=muonNumber;
            }
            
            //Select the desired eeee events
            if(goodMuons==4 && totalMuonCharge==0)
            {
                //Define the needed variables
                float Z1Mass=0.0;
                float Z2Mass=0.0;
                float ZZMass=0.0;
                TLorentzVector Lorentz0(0.0,0.0,0.0,0.0);
                TLorentzVector Lorentz1(0.0,0.0,0.0,0.0);
                TLorentzVector Lorentz2(0.0,0.0,0.0,0.0);
                TLorentzVector Lorentz3(0.0,0.0,0.0,0.0);
                
                //Set the needed Lorentz vectors
                Lorentz0.SetPx(pX_muon[goodMuon1]);
                Lorentz0.SetPy(pY_muon[goodMuon1]);
                Lorentz0.SetPz(pZ_muon[goodMuon1]);
                Lorentz0.SetE(  E_muon[goodMuon1]);
                
                Lorentz1.SetPx(pX_muon[goodMuon2]);
                Lorentz1.SetPy(pY_muon[goodMuon2]);
                Lorentz1.SetPz(pZ_muon[goodMuon2]);
                Lorentz1.SetE(  E_muon[goodMuon2]);
                
                Lorentz2.SetPx(pX_muon[goodMuon3]);
                Lorentz2.SetPy(pY_muon[goodMuon3]);
                Lorentz2.SetPz(pZ_muon[goodMuon3]);
                Lorentz2.SetE(  E_muon[goodMuon3]);
                
                Lorentz3.SetPx(pX_muon[goodMuon4]);
                Lorentz3.SetPy(pY_muon[goodMuon4]);
                Lorentz3.SetPz(pZ_muon[goodMuon4]);
                Lorentz3.SetE(  E_muon[goodMuon4]);
                
                Z1Mass=(Lorentz0+Lorentz1).M();
                Z2Mass=(Lorentz2+Lorentz3).M();
                ZZMass=(Lorentz0+Lorentz1+Lorentz2+Lorentz3).M();
                
                if(ZZMass<100  && cut) continue;
                if(ZZMass>140 && cut) continue;
                if(blind)
                    if(ZZMass>110 && ZZMass<140) continue;
                mumumumuMass->Fill(ZZMass,pu_weight);
            }
        }
        
        //Return the histogram
        cout<<"Checkpoint A6"<<endl;
        cout<<""<<endl;
        cout<<""<<endl;
        cout<<""<<endl;
        
        return mumumumuMass;
    }
	
	else
	{
		return 0;
	}
}
