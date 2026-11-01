#define InvariantZMassCalculator_cxx
#include "InvariantZMassCalculator.h"
#include<TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream.h>

TH1D* InvariantZMassCalculator::Loop(string channel, string leptonType)
{
    // Make sure fchain is non-zero.
	cout<<"Checkpoint A1"<<endl;
    if(fChain==0) return 0;
    
	
    //Define some needed variables
	cout<<"Checkpoint A2"<<endl;
	float nbytes=0,nb=0;
    float nentries=fChain->GetEntriesFast();
    TH1D *histZMass=new TH1D("histZMass", "", 100, 40, 140);
    
	
    //loop over all the entries
	cout<<"Checkpoint A3"<<endl;
	for(int jentry=0; jentry<nentries; jentry++)
    {
		//Debugging
		if(jentry%1000==0)
		{
			cout<<"Checkpoint A3."<<jentry<<endl;
		}

		//Loading the entry
		Long64_t ientry = LoadTree(jentry);
        if (ientry < 0) break;
        nb = fChain->GetEntry(jentry);
		nbytes += nb;
        
		
		//Check conservation of charge
		int totalCharge=0;
		if(channel=="4e"&&nElectrons==4)
		{
			totalCharge=charge_electron[0]
			+charge_electron[1]
			+charge_electron[2]
			+charge_electron[3];
		}
		if(channel=="2e2mu"&&nElectrons==2&&nMuons==2)
		{
			totalCharge=charge_electron[0]
			+charge_electron[1]
			+charge_muon[0]
			+charge_muon[1];
		}
		if(channel=="4mu"&&nMuons==4)
		{
			totalCharge=charge_muon[0]
			+charge_muon[1]
			+charge_muon[2]
			+charge_muon[3];
		}
		
		
		//Make the 4e histogram
		if(channel=="4e")
		{
			float mass0=pow(
							pow(E_electron[0]+E_electron[1],2)-
							pow(pX_electron[0]+pX_electron[1],2)-
							pow(pY_electron[0]+pY_electron[1],2)-
							pow(pZ_electron[0]+pZ_electron[1],2)
							,0.5);
			float mass1=pow(
							pow(E_electron[0]+E_electron[2],2)-
							pow(pX_electron[0]+pX_electron[2],2)-
							pow(pY_electron[0]+pY_electron[2],2)-
							pow(pZ_electron[0]+pZ_electron[2],2)
							,0.5);
			float mass2=pow(
							pow(E_electron[0]+E_electron[3],2)-
							pow(pX_electron[0]+pX_electron[3],2)-
							pow(pY_electron[0]+pY_electron[3],2)-
							pow(pZ_electron[0]+pZ_electron[3],2)
							,0.5);
			float mass3=pow(
							pow(E_electron[1]+E_electron[2],2)-
							pow(pX_electron[1]+pX_electron[2],2)-
							pow(pY_electron[1]+pY_electron[2],2)-
							pow(pZ_electron[1]+pZ_electron[2],2)
							,0.5);
			float mass4=pow(
							pow(E_electron[1]+E_electron[3],2)-
							pow(pX_electron[1]+pX_electron[3],2)-
							pow(pY_electron[1]+pY_electron[3],2)-
							pow(pZ_electron[1]+pZ_electron[3],2)
							,0.5);
			float mass5=pow(
							pow(E_electron[2]+E_electron[3],2)-
							pow(pX_electron[2]+pX_electron[3],2)-
							pow(pY_electron[2]+pY_electron[3],2)-
							pow(pZ_electron[2]+pZ_electron[3],2)
							,0.5);
			
			if(charge_electron[0]==charge_electron[1]) mass0=0;
			if(charge_electron[0]==charge_electron[2]) mass1=0;
			if(charge_electron[0]==charge_electron[3]) mass2=0;
			if(charge_electron[1]==charge_electron[2]) mass3=0;
			if(charge_electron[1]==charge_electron[3]) mass4=0;
			if(charge_electron[2]==charge_electron[3]) mass5=0;
			
			float delta0=91-mass0;
			float delta1=91-mass1;
			float delta2=91-mass2;
			float delta3=91-mass3;
			float delta4=91-mass4;
			float delta5=91-mass5;
			
			float baseline=91;
			if(delta0<baseline) baseline=delta0;
			if(delta1<baseline) baseline=delta1;
			if(delta2<baseline) baseline=delta2;
			if(delta3<baseline) baseline=delta3;
			if(delta4<baseline) baseline=delta4;
			if(delta5<baseline) baseline=delta5;

			if(mass0==baseline) histZMass->Fill(mass0); histZMass->Fill(mass5);
			if(mass1==baseline) histZMass->Fill(mass1); histZMass->Fill(mass4);
			if(mass2==baseline) histZMass->Fill(mass2); histZMass->Fill(mass3);
			if(mass3==baseline) histZMass->Fill(mass3); histZMass->Fill(mass2);
			if(mass4==baseline) histZMass->Fill(mass4); histZMass->Fill(mass1);
			if(mass5==baseline) histZMass->Fill(mass5); histZMass->Fill(mass0);
		}
		
		
		//Make the 2e2mu histogram
		if(channel=="2e2mu")
		{
			if(leptonType=="EE")
			{
				float mass=pow(
							   pow(E_electron[0]+E_electron[1],2)-
							   pow(pX_electron[0]+pX_electron[1],2)-
							   pow(pY_electron[0]+pY_electron[1],2)-
							   pow(pZ_electron[0]+pZ_electron[1],2)
							   ,0.5);
				histZMass->Fill(mass);
			}
			if(leptonType=="MuMu")
			{
				float mass=pow(
							   pow(E_muon[0]+E_muon[1],2)-
							   pow(pX_muon[0]+pX_muon[1],2)-
							   pow(pY_muon[0]+pY_muon[1],2)-
							   pow(pZ_muon[0]+pZ_muon[1],2)
							   ,0.5);
				histZMass->Fill(mass);
			}
		}
		
		
		//Make the 4mu histogram
		if(channel=="4mu")
		{
			float mass0=pow(
							pow(E_muon[0]+E_muon[1],2)-
							pow(pX_muon[0]+pX_muon[1],2)-
							pow(pY_muon[0]+pY_muon[1],2)-
							pow(pZ_muon[0]+pZ_muon[1],2)
							,0.5);
			float mass1=pow(
							pow(E_muon[0]+E_muon[2],2)-
							pow(pX_muon[0]+pX_muon[2],2)-
							pow(pY_muon[0]+pY_muon[2],2)-
							pow(pZ_muon[0]+pZ_muon[2],2)
							,0.5);
			float mass2=pow(
							pow(E_muon[0]+E_muon[3],2)-
							pow(pX_muon[0]+pX_muon[3],2)-
							pow(pY_muon[0]+pY_muon[3],2)-
							pow(pZ_muon[0]+pZ_muon[3],2)
							,0.5);
			float mass3=pow(
							pow(E_muon[1]+E_muon[2],2)-
							pow(pX_muon[1]+pX_muon[2],2)-
							pow(pY_muon[1]+pY_muon[2],2)-
							pow(pZ_muon[1]+pZ_muon[2],2)
							,0.5);
			float mass4=pow(
							pow(E_muon[1]+E_muon[3],2)-
							pow(pX_muon[1]+pX_muon[3],2)-
							pow(pY_muon[1]+pY_muon[3],2)-
							pow(pZ_muon[1]+pZ_muon[3],2)
							,0.5);
			float mass5=pow(
							pow(E_muon[2]+E_muon[3],2)-
							pow(pX_muon[2]+pX_muon[3],2)-
							pow(pY_muon[2]+pY_muon[3],2)-
							pow(pZ_muon[2]+pZ_muon[3],2)
							,0.5);
			
			if(charge_muon[0]==charge_muon[1]) mass0=0;
			if(charge_muon[0]==charge_muon[2]) mass1=0;
			if(charge_muon[0]==charge_muon[3]) mass2=0;
			if(charge_muon[1]==charge_muon[2]) mass3=0;
			if(charge_muon[1]==charge_muon[3]) mass4=0;
			if(charge_muon[2]==charge_muon[3]) mass5=0;
			
			float delta0=91-mass0;
			float delta1=91-mass1;
			float delta2=91-mass2;
			float delta3=91-mass3;
			float delta4=91-mass4;
			float delta5=91-mass5;
			
			float baseline=91;
			if(delta0<baseline) baseline=delta0;
			if(delta1<baseline) baseline=delta1;
			if(delta2<baseline) baseline=delta2;
			if(delta3<baseline) baseline=delta3;
			if(delta4<baseline) baseline=delta4;
			if(delta5<baseline) baseline=delta5;
			
			if(mass0==baseline) histZMass->Fill(mass0); histZMass->Fill(mass5);
			if(mass1==baseline) histZMass->Fill(mass1); histZMass->Fill(mass4);
			if(mass2==baseline) histZMass->Fill(mass2); histZMass->Fill(mass3);
			if(mass3==baseline) histZMass->Fill(mass3); histZMass->Fill(mass2);
			if(mass4==baseline) histZMass->Fill(mass4); histZMass->Fill(mass1);
			if(mass5==baseline) histZMass->Fill(mass5); histZMass->Fill(mass0);
		}
	}
	
	//Return the histogram
	cout<<"Checkpoint A4"<<endl;
	cout<<""<<endl;
	cout<<""<<endl;
	cout<<""<<endl;
	
	return	histZMass;
}
