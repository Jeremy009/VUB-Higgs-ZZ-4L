#include <TH1.h>
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TLatex.h>
#include <TLegend.h>
#include <TLine.h>
#include <THStack.h>
#include <iostream>
#include <TGraph.h>
#include "InvariantZMassCalculator.h"
#include "InvariantZMassCalculator.C"
#include "InvariantZZMassCalculator.h"
#include "InvariantZZMassCalculator.C"
#include "RooStats/NumberCountingUtils.h"
#include "RooGlobalFunc.h"
#include "RooStats/RooStatsUtils.h"
using namespace RooFit;
using namespace RooStats ;
using namespace std ;




void Signal(int nBins=100, int lower=25, int upper=225)//Don't mess with this
{
	//Make some chains
    cout<<"Making chains"<<endl;
    TChain *chainGGF=new TChain("tree","tree");
    TChain *chainVBF=new TChain("tree","tree");
    
    
    //Add the simulation files to the chains
    cout<<"Adding simulations"<<endl;
    chainGGF->Add("Simulations/GluGluToHToZZTo4L_M-126_8TeV_tree.root");
    chainVBF->Add("Simulations/HToZZTo4L_M-126_8TeV_tree.root");
    
    
    //Make some objects
    cout<<"Making objects"<<endl;
    InvariantZZMassCalculator objectGGF (chainGGF);
    InvariantZZMassCalculator objectVBF (chainVBF);
    
    
    //Make the histograms for the different channels
    cout<<"Making histograms"<<endl;
    string channel="4e";
    TH1D *histGGFIn4eChannel=objectGGF.Loop(channel, nBins, lower, upper, 0, 0);
    histGGFIn4eChannel->SetName("histGGFIn4eChannel");
    TH1D *histVBFIn4eChannel=objectVBF.Loop(channel, nBins, lower, upper, 0, 0);
    
    channel="2e2mu";
    TH1D *histGGFIn2e2muChannel=objectGGF.Loop(channel, nBins, lower, upper, 0, 0);
    histGGFIn2e2muChannel->SetName("histGGFIn2e2muChannel");
    TH1D *histVBFIn2e2muChannel=objectVBF.Loop(channel, nBins, lower, upper, 0, 0);
    histVBFIn2e2muChannel->SetName("histVBFIn2e2muChannel");
	
    channel="4mu";
    TH1D *histGGFIn4muChannel=objectGGF.Loop(channel, nBins, lower, upper, 0, 0);
    histGGFIn4muChannel->SetName("histGGFIn4muChannel");
    TH1D *histVBFIn4muChannel=objectVBF.Loop(channel, nBins, lower, upper, 0, 0);
    histVBFIn4muChannel->SetName("histVBFIn4muChannel");
	
	
	//Normalise
    cout<<"Normalising"<<endl;
	float normalisator=1;
	float luminosity=19.6;//fb-1 AnalysisNote p1
	float BRHToZZ=1;//HiggsBosonBR p18
	float BRZTo2e=0.0363;//Pdg
	float BRZTo2mu=0.0366;//Pdg
	float generatedGGFEvents=299974;//DAS
	float generatedVBFEvents=49945;//DAS
	float sigmaGGF=18.970*pow(10,3);//fb
	float sigmaVBF=1.5680*pow(10,3);//fb
	
	//normalisator=2.35554;//pm 0.462752 (normalised from 110 to 225)
	histGGFIn4eChannel->Scale(((sigmaGGF*luminosity)/generatedGGFEvents)*(BRHToZZ*BRZTo2e*BRZTo2e)*normalisator);
	histVBFIn4eChannel->Scale(((sigmaVBF*luminosity)/generatedVBFEvents)*(BRHToZZ*BRZTo2e*BRZTo2e)*normalisator);
	//normalisator=1.8035;//pm 0.274127 (normalised from 110 to 225)
	histGGFIn2e2muChannel->Scale(((sigmaGGF*luminosity)/generatedGGFEvents)*(BRHToZZ*BRZTo2e*BRZTo2mu)*normalisator);
	histVBFIn2e2muChannel->Scale(((sigmaVBF*luminosity)/generatedVBFEvents)*(BRHToZZ*BRZTo2e*BRZTo2mu)*normalisator);
	//normalisator=1.88063;//pm 0.34284 (normalised from 110 to 225)
	histGGFIn4muChannel->Scale(((sigmaGGF*luminosity)/generatedGGFEvents)*(BRHToZZ*BRZTo2mu*BRZTo2mu)*normalisator);
	histVBFIn4muChannel->Scale(((sigmaVBF*luminosity)/generatedVBFEvents)*(BRHToZZ*BRZTo2mu*BRZTo2mu)*normalisator);

	
	//Stack the histograms
	cout<<"Stacking histograms"<<endl;
	THStack *stack=new THStack("stack","");
	stack->Add(histGGFIn4eChannel);
	stack->Add(histVBFIn4eChannel);
	stack->Add(histGGFIn2e2muChannel);
	stack->Add(histVBFIn2e2muChannel);
	stack->Add(histGGFIn4muChannel);
	stack->Add(histVBFIn4muChannel);
	TObject *objStackTemp=stack->GetStack()->Last();
    TH1D *histStack=(TH1D*)objStackTemp->Clone("histStack");


	//Draw the histograms
	cout<<"Saving histograms"<<endl;
	gStyle->SetOptStat("");
	TCanvas *canv=new TCanvas("canv","");
	canv->Divide(2,4);
	canv->cd(1);
	histGGFIn4eChannel->Draw("hist");
	canv->cd(2);
	histVBFIn4eChannel->Draw("hist");
	canv->cd(3);
	histGGFIn2e2muChannel->Draw("hist");
	canv->cd(4);
	histVBFIn2e2muChannel->Draw("hist");
	canv->cd(5);
	histGGFIn4muChannel->Draw("hist");
	canv->cd(6);
	histVBFIn4muChannel->Draw("hist");
	canv->cd(7);
	stack->Draw("hist");
	canv->cd(8);
	histStack->Draw("hist");
}




void ZPeak()//Don't mess with this
{
	//Make some chains
	cout<<"Making chains"<<endl;
	TChain *chain4e=new TChain("tree","tree");
	TChain *chain2e2mu=new TChain("tree","tree");
	TChain *chain4mu=new TChain("tree","tree");
	
	
	//Add the simulation files to the chains
	cout<<"Adding simulations"<<endl;
	chain4e->Add("Simulations/ZZTo4e_8TeV_tree.root");
	chain2e2mu->Add("Simulations/ZZTo2e2mu_8TeV_tree.root");
    chain4mu->Add("Simulations/ZZTo4mu_8TeV_tree.root");
	
	
	//Make some objects
	cout<<"Making objects"<<endl;
	InvariantZMassCalculator object4e (chain4e);
	InvariantZMassCalculator object2e2mu (chain2e2mu);
	InvariantZMassCalculator object4mu (chain4mu);
	
	
    //Make the histograms for the different channels
	cout<<"Making histograms"<<endl;
	TH1D *histZMassFrom4e=object4e.Loop("4e","irrelevant");
    histZMassFrom4e->SetName("histZMassFrom4e");
    
	TH1D *histEEMassFrom2e2mu=object2e2mu.Loop("2e2mu","EE");
    histEEMassFrom2e2mu->SetName("histEEMassFrom2e2mu");
    
	TH1D *histMuMuMassFrom2e2mu=object2e2mu.Loop("2e2mu","MuMu");
    histMuMuMassFrom2e2mu->SetName("histMuMuMassFrom2e2mu");
    
	TH1D *histZMassFrom4mu=object4mu.Loop("4mu","irrelevant");
    histZMassFrom4mu->SetName("histZMassFrom4mu");
	
	
	//Set line color options
	cout<<"Setting line color"<<endl;
	histZMassFrom4e->SetLineColor(kBlack);
	histEEMassFrom2e2mu->SetLineColor(kBlack);
	histMuMuMassFrom2e2mu->SetLineColor(kBlack);
	histZMassFrom4mu->SetLineColor(kBlack);
	
	
	//Set fill color options
	cout<<"Setting fill color"<<endl;
	histZMassFrom4e->SetFillColor(kGreen);
	histEEMassFrom2e2mu->SetFillColor(kGreen);
	histMuMuMassFrom2e2mu->SetFillColor(kRed);
	histZMassFrom4mu->SetFillColor(kRed);
    
    
    //Make the legends
    cout<<"Making legends"<<endl;
    TLegend* legend4e=new TLegend(0.6,0.65,0.88,0.85);
    TLegend* legend2e2muEE=new TLegend(0.6,0.65,0.88,0.85);
    TLegend* legend2e2muMuMu=new TLegend(0.6,0.65,0.88,0.85);
    TLegend* legend4mu=new TLegend(0.6,0.65,0.88,0.85);
    legend4e->SetTextFont(72);
    legend4e->SetTextSize(0.04);
    legend2e2muEE->SetTextFont(72);
    legend2e2muEE->SetTextSize(0.04);
    legend2e2muMuMu->SetTextFont(72);
    legend2e2muMuMu->SetTextSize(0.04);
    legend4mu->SetTextFont(72);
    legend4mu->SetTextSize(0.04);
    legend4e->AddEntry(histZMassFrom4e, "2e","F");
    legend2e2muEE->AddEntry(histEEMassFrom2e2mu, "2e","F");
    legend2e2muMuMu->AddEntry(histMuMuMassFrom2e2mu, "2#mu","F");
    legend4mu->AddEntry(histZMassFrom4mu, "2#mu","F");
	
	
	//Make the canvi
	cout<<"Saving canvasses"<<endl;
	gStyle->SetOptStat("");
	TCanvas *canvZMassFrom4e=new TCanvas("canvZMassFrom4e","");
	histZMassFrom4e->Draw("hist");
	histZMassFrom4e->GetXaxis()->SetTitle("M_{ee} [GeV]");
	histZMassFrom4e->GetYaxis()->SetTitle("# in bin");
    legend4e->Draw();
	canvZMassFrom4e->Modified();
	canvZMassFrom4e->Print("Figures/ZMassFrom4e.eps");
	gStyle->SetOptStat("");
	TCanvas *canvZMassFrom2e2mu=new TCanvas("canvZMassFrom2e2mu","");
	canvZMassFrom2e2mu->Divide(2,1);
	canvZMassFrom2e2mu->cd(1);
	histEEMassFrom2e2mu->Draw("hist");
	histEEMassFrom2e2mu->GetXaxis()->SetTitle("M_{ee} [GeV]");
	histEEMassFrom2e2mu->GetYaxis()->SetTitle("# in bin");
    legend2e2muEE->Draw();
	canvZMassFrom2e2mu->cd(2);
	histMuMuMassFrom2e2mu->Draw("hist");
	histMuMuMassFrom2e2mu->GetXaxis()->SetTitle("M_{#mu#mu} [GeV]");
	histMuMuMassFrom2e2mu->GetYaxis()->SetTitle("# in bin");
    legend2e2muMuMu->Draw();
	canvZMassFrom2e2mu->Modified();
	canvZMassFrom2e2mu->Print("Figures/ZMassFrom2e2mu.eps");
	gStyle->SetOptStat("");
	TCanvas *canvZMassFrom4mu=new TCanvas("canvZMassFrom4mu","");
	histZMassFrom4mu->Draw("hist");
	histZMassFrom4mu->GetXaxis()->SetTitle("M_{#mu#mu} [GeV]");
	histZMassFrom4mu->GetYaxis()->SetTitle("# in bin");
    legend4mu->Draw();
	canvZMassFrom4mu->Modified();
	canvZMassFrom4mu->Print("Figures/ZMassFrom4mu.eps");
    
    
    //Print the mean values
    cout<<"Mean of 4e           "<<histZMassFrom4e->GetMean(1)<<endl;
    cout<<"mean of 2e2mu in 2e  "<<histEEMassFrom2e2mu->GetMean(1)<<endl;
    cout<<"mean of 2e2mu in 2mu "<<histMuMuMassFrom2e2mu->GetMean(1)<<endl;
    cout<<"mean of 4mu          "<<histZMassFrom4mu->GetMean(1)<<endl;

}




void RatioPlots()//Don't mess with this
{
	//Define some constants
	int nBins=100;
	int lower=25;
	int upper=225;
	int lowerRatio=110;
	bool blind=true;
	bool cut=false;
	
	//Make some chains
    cout<<"Making chains"<<endl;
    TChain *chain4e=new TChain("tree","tree");
    TChain *chain4mu=new TChain("tree","tree");
    TChain *chain4tau=new TChain("tree","tree");
    TChain *chain2e2mu=new TChain("tree","tree");
    TChain *chain2e2tau=new TChain("tree","tree");
    TChain *chain2mu2tau=new TChain("tree","tree");
    TChain *chainData=new TChain("tree","tree");
    
    
    //Add the simulation files to the chains
    cout<<"Adding simulations"<<endl;
    chain4e->Add("Simulations/ZZTo4e_8TeV_tree.root");
    chain4mu->Add("Simulations/ZZTo4mu_8TeV_tree.root");
    chain4tau->Add("Simulations/ZZTo4tau_8TeV_tree.root");
    chain2e2mu->Add("Simulations/ZZTo2e2mu_8TeV_tree.root");
    chain2e2tau->Add("Simulations/ZZTo2e2tau_8TeV_tree.root");
    chain2mu2tau->Add("Simulations/ZZTo2mu2tau_8TeV_tree.root");
    chainData->Add("Data/Data*.root");
    
    
    //Make some objects
    cout<<"Making objects"<<endl;
    InvariantZZMassCalculator object4e (chain4e);
    InvariantZZMassCalculator object4mu (chain4mu);
    InvariantZZMassCalculator object4tau (chain4tau);
    InvariantZZMassCalculator object2e2mu (chain2e2mu);
    InvariantZZMassCalculator object2e2tau (chain2e2tau);
    InvariantZZMassCalculator object2mu2tau (chain2mu2tau);
    InvariantZZMassCalculator objectData (chainData);
    
    
    //Make the histograms for the different channels
    cout<<"Making histograms"<<endl;
    string channel="4e";
    TH1D* hist4eIn4eChannel=object4e.Loop(channel, nBins, lower, upper, blind, cut);
    hist4eIn4eChannel->SetName("hist4eIn4eChannel");
    TH1D* hist4muIn4eChannel=object4mu.Loop(channel, nBins, lower, upper, blind, cut);
    hist4muIn4eChannel->SetName("hist4muIn4eChannel");
    TH1D* hist4tauIn4eChannel=object4tau.Loop(channel, nBins, lower, upper, blind, cut);
    hist4tauIn4eChannel->SetName("hist4tauIn4eChannel");
    TH1D* hist2e2muIn4eChannel=object2e2mu.Loop(channel, nBins, lower, upper, blind, cut);
    hist2e2muIn4eChannel->SetName("hist2e2muIn4eChannel");
    TH1D* hist2e2tauIn4eChannel=object2e2tau.Loop(channel, nBins, lower, upper, blind, cut);
    hist2e2tauIn4eChannel->SetName("hist2e2tauIn4eChannel");
    TH1D* hist2mu2tauIn4eChannel=object2mu2tau.Loop(channel, nBins, lower, upper, blind, cut);
    hist2mu2tauIn4eChannel->SetName("hist2mu2tauIn4eChannel");
    TH1D* histDataIn4eChannel=objectData.Loop(channel, nBins, lower, upper, blind, cut);
    histDataIn4eChannel->SetName("histDataIn4eChannel");
	
    channel="2e2mu";
    TH1D* hist4eIn2e2muChannel=object4e.Loop(channel, nBins, lower, upper, blind, cut);
    hist4eIn2e2muChannel->SetName("hist4eIn2e2muChannel");
    TH1D* hist4muIn2e2muChannel=object4mu.Loop(channel, nBins, lower, upper, blind, cut);
    hist4muIn2e2muChannel->SetName("hist4muIn2e2muChannel");
    TH1D* hist4tauIn2e2muChannel=object4tau.Loop(channel, nBins, lower, upper, blind, cut);
    hist4tauIn2e2muChannel->SetName("hist4tauIn2e2muChannel");
    TH1D* hist2e2muIn2e2muChannel=object2e2mu.Loop(channel, nBins, lower, upper, blind, cut);
    hist2e2muIn2e2muChannel->SetName("hist2e2muIn2e2muChannel");
    TH1D* hist2e2tauIn2e2muChannel=object2e2tau.Loop(channel, nBins, lower, upper, blind, cut);
    hist2e2tauIn2e2muChannel->SetName("hist2e2tauIn2e2muChannel");
    TH1D* hist2mu2tauIn2e2muChannel=object2mu2tau.Loop(channel, nBins, lower, upper, blind, cut);
    hist2mu2tauIn2e2muChannel->SetName("hist2mu2tauIn2e2muChannel");
    TH1D* histDataIn2e2muChannel=objectData.Loop(channel, nBins, lower, upper, blind, cut);
    histDataIn2e2muChannel->SetName("histDataIn2e2muChannel");
	
    channel="4mu";
    TH1D* hist4eIn4muChannel=object4e.Loop(channel, nBins, lower, upper, blind, cut);
    hist4eIn4muChannel->SetName("hist4eIn4muChannel");
    TH1D* hist4muIn4muChannel=object4mu.Loop(channel, nBins, lower, upper, blind, cut);
    hist4muIn4muChannel->SetName("hist4muIn4muChannel");
    TH1D* hist4tauIn4muChannel=object4tau.Loop(channel, nBins, lower, upper, blind, cut);
    hist4tauIn4muChannel->SetName("hist4tauIn4muChannel");
    TH1D* hist2e2muIn4muChannel=object2e2mu.Loop(channel, nBins, lower, upper, blind, cut);
    hist2e2muIn4muChannel->SetName("hist2e2muIn4muChannel");
    TH1D* hist2e2tauIn4muChannel=object2e2tau.Loop(channel, nBins, lower, upper, blind, cut);
    hist2e2tauIn4muChannel->SetName("hist2e2tauIn4muChannel");
    TH1D* hist2mu2tauIn4muChannel=object2mu2tau.Loop(channel, nBins, lower, upper, blind, cut);
    hist2mu2tauIn4muChannel->SetName("hist2mu2tauIn4muChannel");
    TH1D* histDataIn4muChannel=objectData.Loop(channel, nBins, lower, upper, blind, cut);
    histDataIn4muChannel->SetName("histDataIn4muChannel");
    
	
    //Normalisation
    cout<<"Normalising"<<endl;
	float normalisator=1;
	float luminosity=19.6;//fb-1 AnalysisNote p1
	float ZZTo4e=76.91;//fb AnalysisNote p11
	float ZZTo4mu=76.91;//fb AnalysisNote p11
	float ZZTo4tau=76.91;//fb AnalysisNote p11
	float ZZTo2e2mu=176.7;//fb AnalysisNote p11
	float ZZTo2e2tau=176.7;//fb AnalysisNote p11
	float ZZTo2mu2tau=176.7;//fb AnalysisNote p11
	float BREToMu=0;//Particles decay 'downwards'
	float BRMuToE=0;//Assume the mean life time of the muon great enough!
	float BRTauToE=0.1785;//TauBR p4
	float BRTauToMu=0.1736;//TauBR p4
	float generated4eEvents=1499093;//DAS
	float generated4muEvents=1499064;//DAS
	float generated4tauEvents=824466;//DAS
	float generated2e2muEvents=1497445;//DAS
	float generated2e2tauEvents=823911;//DAS
	float generated2mu2tauEvents=823922;//DAS
	
	normalisator=1.19573;//pm 0.458907 (normalised from 110 to 225)(errors are non zero)
	hist4eIn4eChannel->Scale((ZZTo4e*luminosity*normalisator)/generated4eEvents);
	hist4muIn4eChannel->Scale((ZZTo4mu*BRMuToE*BRMuToE*BRMuToE*BRMuToE*luminosity*normalisator)/generated4muEvents);
	hist4tauIn4eChannel->Scale((ZZTo4tau*BRTauToE*BRTauToE*BRTauToE*BRTauToE*luminosity*normalisator)/generated4tauEvents);
	hist2e2muIn4eChannel->Scale((ZZTo2e2mu*2*BRMuToE*BRMuToE*luminosity*normalisator)/generated2e2muEvents);
	hist2e2tauIn4eChannel->Scale((ZZTo2e2tau*2*BRTauToE*BRTauToE*luminosity*normalisator)/generated2e2tauEvents);
	hist2mu2tauIn4eChannel->Scale((ZZTo2mu2tau*2*BRMuToE*BRMuToE*BRTauToE*BRTauToE*luminosity*normalisator)/generated2mu2tauEvents);
	
	normalisator=1.49419;//pm 0.205079 (normalised from 110 to 225)(errors are non zero)
	hist4eIn2e2muChannel->Scale((ZZTo4e*5*BREToMu*BREToMu*luminosity*normalisator)/generated4eEvents);
	hist4muIn2e2muChannel->Scale((ZZTo4mu*5*BRMuToE*BRMuToE*luminosity*normalisator)/generated4muEvents);
	hist4tauIn2e2muChannel->Scale((ZZTo4tau*5*BRTauToE*BRTauToE*BRTauToMu*BRTauToMu*luminosity*normalisator)/generated4tauEvents);
	hist2e2muIn2e2muChannel->Scale((ZZTo2e2mu*2*luminosity*normalisator)/generated2e2muEvents);
	hist2e2tauIn2e2muChannel->Scale((ZZTo2e2tau*2*BRTauToMu*BRTauToMu*luminosity*normalisator)/generated2e2tauEvents);
	hist2mu2tauIn2e2muChannel->Scale((ZZTo2mu2tau*2*BRTauToE*BRTauToE*luminosity*normalisator)/generated2mu2tauEvents);
	
	normalisator=1.26262;//pm 0.261496 (normalised from 110 to 225)(errors are non zero)
	hist4eIn4muChannel->Scale((ZZTo4e*BREToMu*BREToMu*BREToMu*BREToMu*luminosity*normalisator)/generated4eEvents);
	hist4muIn4muChannel->Scale((ZZTo4mu*luminosity*normalisator)/generated4muEvents);
	hist4tauIn4muChannel->Scale((ZZTo4tau*BRTauToMu*BRTauToMu*BRTauToMu*BRTauToMu*luminosity*normalisator)/generated4tauEvents);
	hist2e2muIn4muChannel->Scale((ZZTo2e2mu*2*BREToMu*BREToMu*luminosity*normalisator)/generated2e2muEvents);
	hist2e2tauIn4muChannel->Scale((ZZTo2e2tau*2*BREToMu*BREToMu*BRTauToMu*BRTauToMu*luminosity*normalisator)/generated2e2tauEvents);
	hist2mu2tauIn4muChannel->Scale((ZZTo2mu2tau*2*BRTauToMu*BRTauToMu*luminosity*normalisator)/generated2mu2tauEvents);
	
	
	//Set line color options
	cout<<"Setting line color"<<endl;
	hist4eIn4eChannel->SetLineColor(kBlack);
	hist4muIn4eChannel->SetLineColor(kBlack);
	hist4tauIn4eChannel->SetLineColor(kBlack);
	hist2e2muIn4eChannel->SetLineColor(kBlack);
	hist2e2tauIn4eChannel->SetLineColor(kBlack);
	hist2mu2tauIn4eChannel->SetLineColor(kBlack);
	
	hist4eIn2e2muChannel->SetLineColor(kBlack);
	hist4muIn2e2muChannel->SetLineColor(kBlack);
	hist4tauIn2e2muChannel->SetLineColor(kBlack);
	hist2e2muIn2e2muChannel->SetLineColor(kBlack);
	hist2e2tauIn2e2muChannel->SetLineColor(kBlack);
	hist2mu2tauIn2e2muChannel->SetLineColor(kBlack);
	
	hist4eIn4muChannel->SetLineColor(kBlack);
	hist4muIn4muChannel->SetLineColor(kBlack);
	hist4tauIn4muChannel->SetLineColor(kBlack);
	hist2e2muIn4muChannel->SetLineColor(kBlack);
	hist2e2tauIn4muChannel->SetLineColor(kBlack);
	hist2mu2tauIn4muChannel->SetLineColor(kBlack);
	
	
	//Set fill color options
	cout<<"Setting fill color"<<endl;
	hist4eIn4eChannel->SetFillColor(kGreen);
	hist4muIn4eChannel->SetFillColor(kGreen);
	hist4tauIn4eChannel->SetFillColor(kGreen);
	hist2e2muIn4eChannel->SetFillColor(kGreen);
	hist2e2tauIn4eChannel->SetFillColor(kGreen);
	hist2mu2tauIn4eChannel->SetFillColor(kGreen);
	
	hist4eIn2e2muChannel->SetFillColor(kYellow);
	hist4muIn2e2muChannel->SetFillColor(kYellow);
	hist4tauIn2e2muChannel->SetFillColor(kYellow);
	hist2e2muIn2e2muChannel->SetFillColor(kYellow);
	hist2e2tauIn2e2muChannel->SetFillColor(kYellow);
	hist2mu2tauIn2e2muChannel->SetFillColor(kYellow);
	
	hist4eIn4muChannel->SetFillColor(kRed);
	hist4muIn4muChannel->SetFillColor(kRed);
	hist4tauIn4muChannel->SetFillColor(kRed);
	hist2e2muIn4muChannel->SetFillColor(kRed);
	hist2e2tauIn4muChannel->SetFillColor(kRed);
	hist2mu2tauIn4muChannel->SetFillColor(kRed);
	
		
	//Make the required background stacks and histograms
	cout<<"Making background stacks"<<endl;
	THStack *stack4eBackground=new THStack("stack4eBackground","");
	stack4eBackground->Add(hist4eIn4eChannel);
	stack4eBackground->Add(hist4muIn4eChannel);
	stack4eBackground->Add(hist4tauIn4eChannel);
	stack4eBackground->Add(hist2e2muIn4eChannel);
	stack4eBackground->Add(hist2e2tauIn4eChannel);
	stack4eBackground->Add(hist2mu2tauIn4eChannel);
	TObject* objStack4eBackgroundTemp=stack4eBackground->GetStack()->Last();
	TH1D* histStack4eBackground=(TH1D*)objStack4eBackgroundTemp->Clone("histStack4eBackground");
	
	THStack *stack2e2muBackground=new THStack("stack2e2muBackground","");
	stack2e2muBackground->Add(hist4eIn2e2muChannel);
	stack2e2muBackground->Add(hist4muIn2e2muChannel);
	stack2e2muBackground->Add(hist4tauIn2e2muChannel);
	stack2e2muBackground->Add(hist2e2muIn2e2muChannel);
	stack2e2muBackground->Add(hist2e2tauIn2e2muChannel);
	stack2e2muBackground->Add(hist2mu2tauIn2e2muChannel);
	TObject* objStack2e2muBackgroundTemp=stack2e2muBackground->GetStack()->Last();
	TH1D* histStack2e2muBackground=(TH1D*)objStack2e2muBackgroundTemp->Clone("histStack2e2muBackground");
	
	THStack *stack4muBackground=new THStack("stack4muBackground","");
	stack4muBackground->Add(hist4eIn4muChannel);
	stack4muBackground->Add(hist4muIn4muChannel);
	stack4muBackground->Add(hist4tauIn4muChannel);
	stack4muBackground->Add(hist2e2muIn4muChannel);
	stack4muBackground->Add(hist2e2tauIn4muChannel);
	stack4muBackground->Add(hist2mu2tauIn4muChannel);
    TObject* objStack4muBackgroundTemp=stack4muBackground->GetStack()->Last();
	TH1D* histStack4muBackground=(TH1D*)objStack4muBackgroundTemp->Clone("histStack4muBackground");
	
	
	//Making ratio plots
	cout<<"Making ratio plot"<<endl;
	gStyle->SetOptStat("");
	TCanvas* canvRatio4e=new TCanvas("canvRatio4e","");
	canvRatio4e->Divide(1,2);
	canvRatio4e->cd(1);
	histDataIn4eChannel->Draw();
	histStack4eBackground->Draw("same hist");
	histDataIn4eChannel->Draw("same");
	histStack4eBackground->GetXaxis()->SetTitle("M_{4e}");
	histDataIn4eChannel->GetXaxis()->SetTitle("M_{4e}");
	canvRatio4e->cd(2);
	TH1D* histRatio4e=new TH1D("histRatio4e","",nBins,lower,upper);
	histRatio4e->Divide(histDataIn4eChannel,histStack4eBackground);
	histRatio4e->SetLineColor(kBlack);
	histRatio4e->SetMarkerStyle(20);
	histRatio4e->SetMarkerSize(0.6);
	histRatio4e->Draw();
	histRatio4e->GetXaxis()->SetTitle("Bin nr.");
	histRatio4e->GetYaxis()->SetTitle("Ratio");
	histRatio4e->Fit("pol0","","",lowerRatio,upper);
	canvRatio4e->Modified();
	canvRatio4e->Print("Figures/Ratio4e.eps");
	
	gStyle->SetOptStat("");
	TCanvas* canvRatio2e2mu=new TCanvas("canvRatio2e2mu","");
	canvRatio2e2mu->Divide(1,2);
	canvRatio2e2mu->cd(1);
	histDataIn2e2muChannel->Draw();
	histStack2e2muBackground->Draw("same hist");
	histDataIn2e2muChannel->Draw("same");
	histStack2e2muBackground->GetXaxis()->SetTitle("M_{2e2#mu}");
	histDataIn2e2muChannel->GetXaxis()->SetTitle("M_{2e2#mu}");
	canvRatio2e2mu->cd(2);
	TH1D* histRatio2e2mu=new TH1D("histRatio2e2mu","",nBins,lower,upper);
	histRatio2e2mu->Divide(histDataIn2e2muChannel,histStack2e2muBackground);
	histRatio2e2mu->SetLineColor(kBlack);
	histRatio2e2mu->SetMarkerStyle(20);
	histRatio2e2mu->SetMarkerSize(0.6);
	histRatio2e2mu->Draw();
	histRatio2e2mu->GetXaxis()->SetTitle("Bin nr.");
	histRatio2e2mu->GetYaxis()->SetTitle("Ratio");
	histRatio2e2mu->Fit("pol0","","",lowerRatio,upper);
	canvRatio2e2mu->Modified();
	canvRatio2e2mu->Print("Figures/Ratio2e2mu.eps");
	
	gStyle->SetOptStat("");
	TCanvas* canvRatio4mu=new TCanvas("canvRatio4mu","");
	canvRatio4mu->Divide(1,2);
	canvRatio4mu->cd(1);
	histDataIn4muChannel->Draw();
	histStack4muBackground->Draw("same hist");
	histDataIn4muChannel->Draw("same");
	histStack4muBackground->GetXaxis()->SetTitle("M_{4#mu}");
	histDataIn4muChannel->GetXaxis()->SetTitle("M_{4#mu}");
	canvRatio4mu->cd(2);
	TH1D* histRatio4mu=new TH1D("histRatio4mu","",nBins,lower,upper);
	histRatio4mu->Divide(histDataIn4muChannel,histStack4muBackground);
	histRatio4mu->SetLineColor(kBlack);
	histRatio4mu->SetMarkerStyle(20);
	histRatio4mu->SetMarkerSize(0.6);
	histRatio4mu->Draw();
	histRatio4mu->GetXaxis()->SetTitle("Bin nr.");
	histRatio4mu->GetYaxis()->SetTitle("Ratio");
	histRatio4mu->Fit("pol0","","",lowerRatio,upper);
	canvRatio4mu->Modified();
	canvRatio4mu->Print("Figures/Ratio4mu.eps");
}




TCanvas *Bachelorproef(int nBins=50,
					   int lower=0,
					   int upper=500,
					   int normalise=1,
					   int lowerRatio=140,
					   bool blindAll=true,
					   bool blindData=true,
					   bool addData=false,
					   bool addSignal=true,
					   bool withRatio=true,
                       int separate=false,
                       bool only4e=false,
                       bool only2e2mu=false,
                       bool only4mu=false,
					   bool suppressOutput=false,
					   bool zoomData=false,
					   bool cut=false,
					   bool fast=false)
{
	//Define some constants
	cout<<"Defining constants"<<endl;
	bool pretty=true;
	
    //Make some chains
    cout<<"Making chains"<<endl;
    TChain *chain4e=new TChain("tree","tree");
    TChain *chain4mu=new TChain("tree","tree");
    TChain *chain4tau=new TChain("tree","tree");
    TChain *chain2e2mu=new TChain("tree","tree");
    TChain *chain2e2tau=new TChain("tree","tree");
    TChain *chain2mu2tau=new TChain("tree","tree");
    TChain *chainGGF=new TChain("tree","tree");
    TChain *chainVBF=new TChain("tree","tree");
    TChain *chainData=new TChain("tree","tree");
    
    
    //Add the simulation files to the chains
    cout<<"Adding simulations"<<endl;
    chain4e->Add("Simulations/ZZTo4e_8TeV_tree.root");
    chain4mu->Add("Simulations/ZZTo4mu_8TeV_tree.root");
    chain4tau->Add("Simulations/ZZTo4tau_8TeV_tree.root");
    chain2e2mu->Add("Simulations/ZZTo2e2mu_8TeV_tree.root");
    chain2e2tau->Add("Simulations/ZZTo2e2tau_8TeV_tree.root");
    chain2mu2tau->Add("Simulations/ZZTo2mu2tau_8TeV_tree.root");
    chainGGF->Add("Simulations/GluGluToHToZZTo4L_M-126_8TeV_tree.root");
    chainVBF->Add("Simulations/HToZZTo4L_M-126_8TeV_tree.root");
    chainData->Add("Data/Data*.root");
    
    
    //Make some objects
    cout<<"Making objects"<<endl;
    InvariantZZMassCalculator object4e (chain4e);
    InvariantZZMassCalculator object4mu (chain4mu);
    InvariantZZMassCalculator object4tau (chain4tau);
    InvariantZZMassCalculator object2e2mu (chain2e2mu);
    InvariantZZMassCalculator object2e2tau (chain2e2tau);
    InvariantZZMassCalculator object2mu2tau (chain2mu2tau);
    InvariantZZMassCalculator objectGGF (chainGGF);
    InvariantZZMassCalculator objectVBF (chainVBF);
    InvariantZZMassCalculator objectData (chainData);
    
    
    //Make the histograms for the different channels
    cout<<"Making histograms"<<endl;
    string channel="4e";
    TH1D *hist4eIn4eChannel=object4e.Loop(channel, nBins, lower, upper, blindAll, cut);
    hist4eIn4eChannel->SetName("hist4eIn4eChannel");
    TH1D *hist4muIn4eChannel=object4mu.Loop(channel, nBins, lower, upper, blindAll, cut);
    hist4muIn4eChannel->SetName("hist4muIn4eChannel");
    TH1D *hist4tauIn4eChannel=object4tau.Loop(channel, nBins, lower, upper, blindAll, cut);
    hist4tauIn4eChannel->SetName("hist4tauIn4eChannel");
    TH1D *hist2e2muIn4eChannel=object2e2mu.Loop(channel, nBins, lower, upper, blindAll, cut);
    hist2e2muIn4eChannel->SetName("hist2e2muIn4eChannel");
    TH1D *hist2e2tauIn4eChannel=object2e2tau.Loop(channel, nBins, lower, upper, blindAll, cut);
    hist2e2tauIn4eChannel->SetName("hist2e2tauIn4eChannel");
    TH1D *hist2mu2tauIn4eChannel=object2mu2tau.Loop(channel, nBins, lower, upper, blindAll, cut);
    hist2mu2tauIn4eChannel->SetName("hist2mu2tauIn4eChannel");
    TH1D *histGGFIn4eChannel=objectGGF.Loop(channel, nBins, lower, upper, blindAll, cut);
    histGGFIn4eChannel->SetName("histGGFIn4eChannel");
    TH1D *histVBFIn4eChannel=objectVBF.Loop(channel, nBins, lower, upper, blindAll, cut);
    histVBFIn4eChannel->SetName("histVBFIn4eChannel");
    TH1D *histDataIn4eChannel=objectData.Loop(channel, nBins, lower, upper, blindData, cut);
    histDataIn4eChannel->SetName("histDataIn4eChannel");
	
    channel="2e2mu";
    TH1D *hist4eIn2e2muChannel=object4e.Loop(channel, nBins, lower, upper, blindAll, cut);
    hist4eIn2e2muChannel->SetName("hist4eIn2e2muChannel");
    TH1D *hist4muIn2e2muChannel=object4mu.Loop(channel, nBins, lower, upper, blindAll, cut);
    hist4muIn2e2muChannel->SetName("hist4muIn2e2muChannel");
    TH1D *hist4tauIn2e2muChannel=object4tau.Loop(channel, nBins, lower, upper, blindAll, cut);
    hist4tauIn2e2muChannel->SetName("hist4tauIn2e2muChannel");
    TH1D *hist2e2muIn2e2muChannel=object2e2mu.Loop(channel, nBins, lower, upper, blindAll, cut);
    hist2e2muIn2e2muChannel->SetName("hist2e2muIn2e2muChannel");
    TH1D *hist2e2tauIn2e2muChannel=object2e2tau.Loop(channel, nBins, lower, upper, blindAll, cut);
    hist2e2tauIn2e2muChannel->SetName("hist2e2tauIn2e2muChannel");
    TH1D *hist2mu2tauIn2e2muChannel=object2mu2tau.Loop(channel, nBins, lower, upper, blindAll, cut);
    hist2mu2tauIn2e2muChannel->SetName("hist2mu2tauIn2e2muChannel");
    TH1D *histGGFIn2e2muChannel=objectGGF.Loop(channel, nBins, lower, upper, blindAll, cut);
    histGGFIn2e2muChannel->SetName("histGGFIn2e2muChannel");
    TH1D *histVBFIn2e2muChannel=objectVBF.Loop(channel, nBins, lower, upper, blindAll, cut);
    histVBFIn2e2muChannel->SetName("histVBFIn2e2muChannel");
    TH1D *histDataIn2e2muChannel=objectData.Loop(channel, nBins, lower, upper, blindData, cut);
    histDataIn2e2muChannel->SetName("histDataIn2e2muChannel");
	
    channel="4mu";
    TH1D *hist4eIn4muChannel=object4e.Loop(channel, nBins, lower, upper, blindAll, cut);
    hist4eIn4muChannel->SetName("hist4eIn4muChannel");
    TH1D *hist4muIn4muChannel=object4mu.Loop(channel, nBins, lower, upper, blindAll, cut);
    hist4muIn4muChannel->SetName("hist4muIn4muChannel");
    TH1D *hist4tauIn4muChannel=object4tau.Loop(channel, nBins, lower, upper, blindAll, cut);
    hist4tauIn4muChannel->SetName("hist4tauIn4muChannel");
    TH1D *hist2e2muIn4muChannel=object2e2mu.Loop(channel, nBins, lower, upper, blindAll, cut);
    hist2e2muIn4muChannel->SetName("hist2e2muIn4muChannel");
    TH1D *hist2e2tauIn4muChannel=object2e2tau.Loop(channel, nBins, lower, upper, blindAll, cut);
    hist2e2tauIn4muChannel->SetName("hist2e2tauIn4muChannel");
    TH1D *hist2mu2tauIn4muChannel=object2mu2tau.Loop(channel, nBins, lower, upper, blindAll, cut);
    hist2mu2tauIn4muChannel->SetName("hist2mu2tauIn4muChannel");
    TH1D *histGGFIn4muChannel=objectGGF.Loop(channel, nBins, lower, upper, blindAll, cut);
    histGGFIn4muChannel->SetName("histGGFIn4muChannel");
    TH1D *histVBFIn4muChannel=objectVBF.Loop(channel, nBins, lower, upper, blindAll, cut);
    histVBFIn4muChannel->SetName("histVBFIn4muChannel");
    TH1D *histDataIn4muChannel=objectData.Loop(channel, nBins, lower, upper, blindData, cut);
    histDataIn4muChannel->SetName("histDataIn4muChannel");
    
	
    //Normalisation
    cout<<"Normalising"<<endl;
    if(normalise>=1)//Normalise to theory
    {
		float normalisator=1;
        float luminosity=19.6;//fb-1 AnalysisNote p1
        float ZZTo4e=76.91;//fb AnalysisNote p11
        float ZZTo4mu=76.91;//fb AnalysisNote p11
        float ZZTo4tau=76.91;//fb AnalysisNote p11
        float ZZTo2e2mu=176.7;//fb AnalysisNote p11
        float ZZTo2e2tau=176.7;//fb AnalysisNote p11
        float ZZTo2mu2tau=176.7;//fb AnalysisNote p11
		float BRZTo2e=0.0363;//Pdg
		float BRZTo2mu=0.0366;//Pdg
        float BREToMu=0;//Particles decay 'downwards'
        float BRMuToE=0;//Assume the mean life time of the muon great enough!
        float BRTauToE=0.1785;//TauBR p4
        float BRTauToMu=0.1736;//TauBR p4
        float generated4eEvents=1499093;//DAS
        float generated4muEvents=1499064;//DAS
        float generated4tauEvents=824466;//DAS
        float generated2e2muEvents=1497445;//DAS
        float generated2e2tauEvents=823911;//DAS
        float generated2mu2tauEvents=823922;//DAS
		float generatedGGFEvents=299974;//DAS
		float generatedVBFEvents=49945;//DAS
		float sigmaGGF=18.970*pow(10,3);//fb
		float sigmaVBF=1.5680*pow(10,3);//fb
		//float BRHToZZ=2.64*pow(10,-2);//HiggsBosonBR p18
		
		if(normalise==2) normalisator=1.19573;//pm 0.458907 (normalised from 110 to 225)(errors are non zero)
        hist4eIn4eChannel->Scale((ZZTo4e*luminosity*normalisator)/generated4eEvents);
        hist4muIn4eChannel->Scale((ZZTo4mu*BRMuToE*BRMuToE*BRMuToE*BRMuToE*luminosity*normalisator)/generated4muEvents);
        hist4tauIn4eChannel->Scale((ZZTo4tau*BRTauToE*BRTauToE*BRTauToE*BRTauToE*luminosity*normalisator)/generated4tauEvents);
        hist2e2muIn4eChannel->Scale((ZZTo2e2mu*2*BRMuToE*BRMuToE*luminosity*normalisator)/generated2e2muEvents);
        hist2e2tauIn4eChannel->Scale((ZZTo2e2tau*2*BRTauToE*BRTauToE*luminosity*normalisator)/generated2e2tauEvents);
        hist2mu2tauIn4eChannel->Scale((ZZTo2mu2tau*2*BRMuToE*BRMuToE*BRTauToE*BRTauToE*luminosity*normalisator)/generated2mu2tauEvents);
		
		if(normalise==2) normalisator=1.49419;//pm 0.205079 (normalised from 110 to 225)(errors are non zero)
        hist4eIn2e2muChannel->Scale((ZZTo4e*5*BREToMu*BREToMu*luminosity*normalisator)/generated4eEvents);
        hist4muIn2e2muChannel->Scale((ZZTo4mu*5*BRMuToE*BRMuToE*luminosity*normalisator)/generated4muEvents);
        hist4tauIn2e2muChannel->Scale((ZZTo4tau*5*BRTauToE*BRTauToE*BRTauToMu*BRTauToMu*luminosity*normalisator)/generated4tauEvents);
        hist2e2muIn2e2muChannel->Scale((ZZTo2e2mu*2*luminosity*normalisator)/generated2e2muEvents);
        hist2e2tauIn2e2muChannel->Scale((ZZTo2e2tau*2*BRTauToMu*BRTauToMu*luminosity*normalisator)/generated2e2tauEvents);
        hist2mu2tauIn2e2muChannel->Scale((ZZTo2mu2tau*2*BRTauToE*BRTauToE*luminosity*normalisator)/generated2mu2tauEvents);
		
		if(normalise==2) normalisator=1.26262;//pm 0.261496 (normalised from 110 to 225)(errors are non zero)
		hist4eIn4muChannel->Scale((ZZTo4e*BREToMu*BREToMu*BREToMu*BREToMu*luminosity*normalisator)/generated4eEvents);
        hist4muIn4muChannel->Scale((ZZTo4mu*luminosity*normalisator)/generated4muEvents);
        hist4tauIn4muChannel->Scale((ZZTo4tau*BRTauToMu*BRTauToMu*BRTauToMu*BRTauToMu*luminosity*normalisator)/generated4tauEvents);
        hist2e2muIn4muChannel->Scale((ZZTo2e2mu*2*BREToMu*BREToMu*luminosity*normalisator)/generated2e2muEvents);
        hist2e2tauIn4muChannel->Scale((ZZTo2e2tau*2*BREToMu*BREToMu*BRTauToMu*BRTauToMu*luminosity*normalisator)/generated2e2tauEvents);
        hist2mu2tauIn4muChannel->Scale((ZZTo2mu2tau*2*BRTauToMu*BRTauToMu*luminosity*normalisator)/generated2mu2tauEvents);
		
		histGGFIn4eChannel->Scale(((sigmaGGF*luminosity)/generatedGGFEvents)*(BRZTo2e*BRZTo2e));
		histVBFIn4eChannel->Scale(((sigmaVBF*luminosity)/generatedVBFEvents)*(BRZTo2e*BRZTo2e));
		histGGFIn2e2muChannel->Scale(((sigmaGGF*luminosity)/generatedGGFEvents)*(BRZTo2e*BRZTo2mu));
		histVBFIn2e2muChannel->Scale(((sigmaVBF*luminosity)/generatedVBFEvents)*(BRZTo2e*BRZTo2mu));
		histGGFIn4muChannel->Scale(((sigmaGGF*luminosity)/generatedGGFEvents)*(BRZTo2mu*BRZTo2mu));
		histVBFIn4muChannel->Scale(((sigmaVBF*luminosity)/generatedVBFEvents)*(BRZTo2mu*BRZTo2mu));
        
        
        //Display the normalisation factors
        if(!fast)
        {
            normalisator=1;
            cout<<"N1= "<<((ZZTo4e*luminosity*normalisator)/generated4eEvents)<<endl;
            cout<<"N2= "<<((ZZTo4mu*luminosity*normalisator)/generated4muEvents)<<endl;
            cout<<"N3= "<<((ZZTo2e2mu*2*luminosity*normalisator)/generated2e2muEvents)<<endl;
            cout<<"N4= "<<((ZZTo4tau*BRTauToE*BRTauToE*BRTauToE*BRTauToE*luminosity*normalisator)/generated4tauEvents)<<endl;
            cout<<"N5= "<<((ZZTo4tau*5*BRTauToE*BRTauToE*BRTauToMu*BRTauToMu*luminosity*normalisator)/generated4tauEvents)<<endl;
            cout<<"N6= "<<((ZZTo4tau*BRTauToMu*BRTauToMu*BRTauToMu*BRTauToMu*luminosity*normalisator)/generated4tauEvents)<<endl;
            cout<<"N7= "<<((ZZTo2e2tau*2*BRTauToE*BRTauToE*luminosity*normalisator)/generated2e2tauEvents)<<endl;
            cout<<"N8= "<<((ZZTo2e2tau*2*BRTauToMu*BRTauToMu*luminosity*normalisator)/generated2e2tauEvents)<<endl;
            cout<<"N9= "<<((ZZTo2mu2tau*2*BRTauToE*BRTauToE*luminosity*normalisator)/generated2mu2tauEvents)<<endl;
            cout<<"N10="<<((ZZTo2mu2tau*2*BRTauToMu*BRTauToMu*luminosity*normalisator)/generated2mu2tauEvents)<<endl;
        }
    }
    
	
	//Set line color options
	if(pretty)
	{
		cout<<"Setting line color"<<endl;
		hist4eIn4eChannel->SetLineColor(kBlack);
		hist4muIn4eChannel->SetLineColor(kBlack);
		hist4tauIn4eChannel->SetLineColor(kBlack);
		hist2e2muIn4eChannel->SetLineColor(kBlack);
		hist2e2tauIn4eChannel->SetLineColor(kBlack);
		hist2mu2tauIn4eChannel->SetLineColor(kBlack);
		histGGFIn4eChannel->SetLineColor(kBlack);
		histVBFIn4eChannel->SetLineColor(kBlack);
		
		hist4eIn2e2muChannel->SetLineColor(kBlack);
		hist4muIn2e2muChannel->SetLineColor(kBlack);
		hist4tauIn2e2muChannel->SetLineColor(kBlack);
		hist2e2muIn2e2muChannel->SetLineColor(kBlack);
		hist2e2tauIn2e2muChannel->SetLineColor(kBlack);
		hist2mu2tauIn2e2muChannel->SetLineColor(kBlack);
		histGGFIn2e2muChannel->SetLineColor(kBlack);
		histVBFIn2e2muChannel->SetLineColor(kBlack);
		
		hist4eIn4muChannel->SetLineColor(kBlack);
		hist4muIn4muChannel->SetLineColor(kBlack);
		hist4tauIn4muChannel->SetLineColor(kBlack);
		hist2e2muIn4muChannel->SetLineColor(kBlack);
		hist2e2tauIn4muChannel->SetLineColor(kBlack);
		hist2mu2tauIn4muChannel->SetLineColor(kBlack);
		histGGFIn4muChannel->SetLineColor(kBlack);
		histVBFIn4muChannel->SetLineColor(kBlack);
		
		
		//Set fill color options
		cout<<"Setting fill color"<<endl;
		hist4eIn4eChannel->SetFillColor(kGreen);
		hist4muIn4eChannel->SetFillColor(kGreen);
		hist4tauIn4eChannel->SetFillColor(kGreen);
		hist2e2muIn4eChannel->SetFillColor(kGreen);
		hist2e2tauIn4eChannel->SetFillColor(kGreen);
		hist2mu2tauIn4eChannel->SetFillColor(kGreen);
		
		hist4eIn4muChannel->SetFillColor(kRed);
		hist4muIn4muChannel->SetFillColor(kRed);
		hist4tauIn4muChannel->SetFillColor(kRed);
		hist2e2muIn4muChannel->SetFillColor(kRed);
		hist2e2tauIn4muChannel->SetFillColor(kRed);
		hist2mu2tauIn4muChannel->SetFillColor(kRed);
        
        hist4eIn2e2muChannel->SetFillColor(kYellow);
        hist4muIn2e2muChannel->SetFillColor(kYellow);
        hist4tauIn2e2muChannel->SetFillColor(kYellow);
        hist2e2muIn2e2muChannel->SetFillColor(kYellow);
        hist2e2tauIn2e2muChannel->SetFillColor(kYellow);
        hist2mu2tauIn2e2muChannel->SetFillColor(kYellow);
		
		histGGFIn4eChannel->SetFillColor(kBlack);
		histVBFIn4eChannel->SetFillColor(kBlack);
		histGGFIn2e2muChannel->SetFillColor(kBlack);
		histVBFIn2e2muChannel->SetFillColor(kBlack);
		histGGFIn4muChannel->SetFillColor(kBlack);
		histVBFIn4eChannel->SetFillColor(kBlack);
	}
	

	//Make the required background stacks and histograms
	cout<<"Making background stacks"<<endl;
	THStack *stack4eBackground=new THStack("stack4eBackground","");
	stack4eBackground->Add(hist4eIn4eChannel);
	stack4eBackground->Add(hist4muIn4eChannel);
	stack4eBackground->Add(hist4tauIn4eChannel);
	stack4eBackground->Add(hist2e2muIn4eChannel);
	stack4eBackground->Add(hist2e2tauIn4eChannel);
	stack4eBackground->Add(hist2mu2tauIn4eChannel);
	TObject *objStack4eBackgroundTemp=stack4eBackground->GetStack()->Last();
	TH1D *histStack4eBackground=(TH1D*)objStack4eBackgroundTemp->Clone("histStack4eBackground");
		
	THStack *stack2e2muBackground=new THStack("stack2e2muBackground","");
	stack2e2muBackground->Add(hist4eIn2e2muChannel);
	stack2e2muBackground->Add(hist4muIn2e2muChannel);
	stack2e2muBackground->Add(hist4tauIn2e2muChannel);
	stack2e2muBackground->Add(hist2e2muIn2e2muChannel);
	stack2e2muBackground->Add(hist2e2tauIn2e2muChannel);
	stack2e2muBackground->Add(hist2mu2tauIn2e2muChannel);
	TObject *objStack2e2muBackgroundTemp=stack2e2muBackground->GetStack()->Last();
	TH1D *histStack2e2muBackground=(TH1D*)objStack2e2muBackgroundTemp->Clone("histStack2e2muBackground");
		
	THStack *stack4muBackground=new THStack("stack4muBackground","");
	stack4muBackground->Add(hist4eIn4muChannel);
	stack4muBackground->Add(hist4muIn4muChannel);
	stack4muBackground->Add(hist4tauIn4muChannel);
	stack4muBackground->Add(hist2e2muIn4muChannel);
	stack4muBackground->Add(hist2e2tauIn4muChannel);
	stack4muBackground->Add(hist2mu2tauIn4muChannel);
    TObject *objStack4muBackgroundTemp=stack4muBackground->GetStack()->Last();
	TH1D *histStack4muBackground=(TH1D*)objStack4muBackgroundTemp->Clone("histStack4muBackground");
		
		
	//Make the required signal stacks
	cout<<"Making signal stacks"<<endl;
	THStack *stack4eSignal=new THStack("stack4eSignal","");
	stack4eSignal->Add(histGGFIn4eChannel);
	stack4eSignal->Add(histVBFIn4eChannel);
	TObject *objStack4eSignalTemp=stack4eSignal->GetStack()->Last();
	TH1D *histStack4eSignal=(TH1D*)objStack4eSignalTemp->Clone("histStack4eSignal");
		
	THStack *stack2e2muSignal=new THStack("stack2e2muSignal","");
	stack2e2muSignal->Add(histGGFIn2e2muChannel);
	stack2e2muSignal->Add(histVBFIn2e2muChannel);
	TObject *objStack2e2muSignalTemp=stack2e2muSignal->GetStack()->Last();
	TH1D *histStack2e2muSignal=(TH1D*)objStack2e2muSignalTemp->Clone("histStack2e2muSignal");
		
	THStack *stack4muSignal=new THStack("stack4muSignal","");
	stack4muSignal->Add(histGGFIn4muChannel);
	stack4muSignal->Add(histVBFIn4muChannel);
    TObject *objStack4muSignalTemp=stack4muSignal->GetStack()->Last();
    TH1D *histStack4muSignal=(TH1D*)objStack4muSignalTemp->Clone("histStack4muSignal");

		
	//Make the background+signal histogram
	cout<<"Making total stack"<<endl;
	THStack *stack4e=new THStack("stack4e","");
	stack4e->Add(histStack4eBackground);
	stack4e->Add(histStack4eSignal);
		
	THStack *stack2e2mu=new THStack("stack2e2mu","");
	stack2e2mu->Add(histStack2e2muBackground);
	stack2e2mu->Add(histStack2e2muSignal);
	
	THStack *stack4mu=new THStack("stack4mu","");
	stack4mu->Add(histStack4muBackground);
	stack4mu->Add(histStack4muSignal);
    
    
    //Making the expected stack/hist
    cout<<"Making expected stack"<<endl;
    THStack *stackExpected=new THStack("stackExpected","");
    stackExpected->Add(histStack4eBackground);
    stackExpected->Add(histStack4muBackground);
    stackExpected->Add(histStack2e2muBackground);
    if(addSignal) stackExpected->Add(histStack4eSignal);
    if(addSignal) stackExpected->Add(histStack2e2muSignal);
    if(addSignal) stackExpected->Add(histStack4muSignal);
    TObject *objStackExpected=stackExpected->GetStack()->Last();
    TH1D *histExpected=(TH1D*)objStackExpected->Clone("histExpected");
    
    //Making the observed stack/hist
    cout<<"Making observed stack"<<endl;
    THStack *stackObserved=new THStack("stackObserved","");
    stackObserved->Add(histDataIn4eChannel);
    stackObserved->Add(histDataIn2e2muChannel);
    stackObserved->Add(histDataIn4muChannel);
    TObject *objStackObserved=stackObserved->GetStack()->Last();
    TH1D *histObserved=(TH1D*)objStackObserved->Clone("histObserved");
		
	//Making ratio plots
	if(!fast)
	{
		cout<<"Making ratio plot"<<endl;
		gStyle->SetOptStat("");
		TCanvas* canvRatio4e=new TCanvas("canvRatio4e","");
		canvRatio4e->Divide(1,2);
		canvRatio4e->cd(1);
		histDataIn4eChannel->Draw();
		histStack4eBackground->Draw("same hist");
		histDataIn4eChannel->Draw("same");
		canvRatio4e->cd(2);
		TH1D* histRatio4e=new TH1D("histRatio4e","",nBins,lower,upper);
		histRatio4e->Divide(histDataIn4eChannel,histStack4eBackground);
		histRatio4e->SetFillColor(kBlue);
		histRatio4e->GetXaxis()->SetTitle("Bin nr.");
		histRatio4e->GetYaxis()->SetTitle("Ratio");
		histRatio4e->Fit("pol0","","",lowerRatio,upper);
		canvRatio4e->Modified();
		
		gStyle->SetOptStat("");
		TCanvas* canvRatio2e2mu=new TCanvas("canvRatio2e2mu","");
		canvRatio2e2mu->Divide(1,2);
		canvRatio2e2mu->cd(1);
		histDataIn2e2muChannel->Draw();
		histStack2e2muBackground->Draw("same hist");
		histDataIn2e2muChannel->Draw("same");
		canvRatio2e2mu->cd(2);
		TH1D* histRatio2e2mu=new TH1D("histRatio2e2mu","",nBins,lower,upper);
		histRatio2e2mu->Divide(histDataIn2e2muChannel,histStack2e2muBackground);
		histRatio2e2mu->SetFillColor(kBlue);
		histRatio2e2mu->GetXaxis()->SetTitle("Bin nr.");
		histRatio2e2mu->GetYaxis()->SetTitle("Ratio");
		histRatio2e2mu->Fit("pol0","","",lowerRatio,upper);
		canvRatio2e2mu->Modified();
		
		gStyle->SetOptStat("");
		TCanvas* canvRatio4mu=new TCanvas("canvRatio4mu","");
		canvRatio4mu->Divide(1,2);
		canvRatio4mu->cd(1);
		histDataIn4muChannel->Draw();
		histStack4muBackground->Draw("same hist");
		histDataIn4muChannel->Draw("same");
		canvRatio4mu->cd(2);
		TH1D* histRatio4mu=new TH1D("histRatio4mu","",nBins,lower,upper);
		histRatio4mu->Divide(histDataIn4muChannel,histStack4muBackground);
		histRatio4mu->SetFillColor(kBlue);
		histRatio4mu->GetXaxis()->SetTitle("Bin nr.");
		histRatio4mu->GetYaxis()->SetTitle("Ratio");
		histRatio4mu->Fit("pol0","","",lowerRatio,upper);
		canvRatio4mu->Modified();
	}
	
	
	//Make the canvas to be returned
	gStyle->SetOptStat("");
	TCanvas *canv=new TCanvas("canv","");
	cout<<separate<<endl;
    if(separate==1)
    {
        //Divide the canvas
        if(only4e||only2e2mu||only4mu)
        {
            if(!withRatio)canv->Divide(1,1);
            else if(withRatio)canv->Divide(1,2);
        }
        else if(!only4e&&!only2e2mu&&!only4mu)
        {
            if(!withRatio)canv->Divide(3,1);
            else if(withRatio) canv->Divide(3,2);
        }
        
        
        if(only4e)
        {
            //Do the 4e part
            canv->cd(1);
            
            
            //Make the legend for 4e
            cout<<"Making legend for 4e"<<endl;
            THStack *stack4eSingleChannel=new THStack("stack4eSingleChannel","");
            hist2e2tauIn4eChannel->SetFillColor(kCyan);
            hist4tauIn4eChannel->SetFillColor(kBlue);
            hist4eIn4eChannel->SetFillColor(kGreen);
            stack4eSingleChannel->Add(hist2e2tauIn4eChannel);
            stack4eSingleChannel->Add(hist4tauIn4eChannel);
            stack4eSingleChannel->Add(hist4eIn4eChannel);
            if(addSignal) stack4eSingleChannel->Add(histVBFIn4eChannel);
            if(addSignal) stack4eSingleChannel->Add(histGGFIn4eChannel);
            TLegend* legend4e=new TLegend(0.6,0.65,0.88,0.85);
            legend4e->SetTextFont(72);
            legend4e->SetTextSize(0.04);
            if(addData) legend4e->AddEntry(histDataIn4eChannel,"Data","P E L");
            if(addSignal) legend4e->AddEntry(histVBFIn4eChannel,"Higgs signal","F");
            legend4e->AddEntry(hist4eIn4eChannel,"4e","F");
            legend4e->AddEntry(hist4tauIn4eChannel,"4#tau","F");
            legend4e->AddEntry(hist2e2tauIn4eChannel,"2e2#tau","F");
            
            
            //Making the canvas for 4e
            cout<<"Making canvas for 4e"<<endl;
            if(!addData&&zoomData) zoomData=false;
            if(zoomData) histDataIn4eChannel->Draw(); stack4eSingleChannel->Draw("same hist");
            if(!zoomData) stack4eSingleChannel->Draw("hist");
            if(addData) histDataIn4eChannel->Draw("same");
            legend4e->Draw();
            stack4eSingleChannel->GetXaxis()->SetTitle("M_{4e} [GeV]");
            if(addData) histDataIn4eChannel->GetXaxis()->SetTitle("M_{4e} [GeV]");
            if(withRatio)
            {
                canv->cd(2);
                TObject *objStackExpected4e=stack4eSingleChannel->GetStack()->Last();
                TH1D *histExpected4e=(TH1D*)objStackExpected4e->Clone("histExpected4e");
                TH1D *ratio4e=new TH1D("ratio","",nBins,lower,upper);
                ratio4e->Divide(histDataIn4eChannel,histExpected4e);
                ratio4e->SetLineColor(kBlack);
                ratio4e->SetMarkerStyle(20);
                ratio4e->SetMarkerSize(0.6);
                ratio4e->Draw();
                ratio4e->GetXaxis()->SetRangeUser(lower,upper);
                ratio4e->GetXaxis()->SetTitle("Bin nr.");
                ratio4e->GetYaxis()->SetTitle("Ratio");
                ratio4e->Fit("pol0","","",lowerRatio,upper);
            }
        }
        if(only2e2mu)
        {
            //Do the 2e2mu part
            canv->cd(1);
            
            
            //Make the legend for 2e2mu
            cout<<"Making legend for 2e2mu"<<endl;
            hist4tauIn2e2muChannel->SetFillColor(kBlue);
            hist2e2muIn2e2muChannel->SetFillColor(kYellow);
            hist2e2tauIn2e2muChannel->SetFillColor(kCyan);
            hist2mu2tauIn2e2muChannel->SetFillColor(kMagenta);
            THStack *stack2e2muSingleChannel=new THStack("stack2e2muSingleChannel","");
            stack2e2muSingleChannel->Add(hist4tauIn2e2muChannel);
            stack2e2muSingleChannel->Add(hist2mu2tauIn2e2muChannel);
            stack2e2muSingleChannel->Add(hist2e2tauIn2e2muChannel);
            stack2e2muSingleChannel->Add(hist2e2muIn2e2muChannel);
            if(addSignal) stack2e2muSingleChannel->Add(histVBFIn2e2muChannel);
            if(addSignal) stack2e2muSingleChannel->Add(histGGFIn2e2muChannel);
            TLegend* legend2e2mu=new TLegend(0.6,0.65,0.88,0.85);
            legend2e2mu->SetTextFont(72);
            legend2e2mu->SetTextSize(0.04);
            if(addData) legend2e2mu->AddEntry(histDataIn2e2muChannel,"Data","P E L");
            if(addSignal) legend2e2mu->AddEntry(histVBFIn2e2muChannel,"Higgs signal","F");
            legend2e2mu->AddEntry(hist2e2muIn2e2muChannel,"2e2#mu","F");
            legend2e2mu->AddEntry(hist2e2tauIn2e2muChannel,"2e2#tau","F");
            legend2e2mu->AddEntry(hist2mu2tauIn2e2muChannel,"2#mu2#tau","F");
            legend2e2mu->AddEntry(hist4tauIn2e2muChannel,"4#tau","F");
            
            
            //Making the canvas for 2e2mu
            cout<<"Making canvas for 2e2mu"<<endl;
            if(!addData&&zoomData) zoomData=false;
            if(zoomData) histDataIn2e2muChannel->Draw(); stack2e2muSingleChannel->Draw("same hist");
            if(!zoomData) stack2e2muSingleChannel->Draw("hist");
            if(addData) histDataIn2e2muChannel->Draw("same");
            legend2e2mu->Draw();
            stack2e2muSingleChannel->GetXaxis()->SetTitle("M_{2e2#mu} [GeV]");
            if(addData) histDataIn2e2muChannel->GetXaxis()->SetTitle("M_{2e2#mu} [GeV]");
            if(withRatio)
            {
                canv->cd(2);
                TObject *objStackExpected2e2mu=stack2e2muSingleChannel->GetStack()->Last();
                TH1D *histExpected2e2mu=(TH1D*)objStackExpected2e2mu->Clone("histExpected2e2mu");
                TH1D *ratio2e2mu=new TH1D("ratio","",nBins,lower,upper);
                ratio2e2mu->Divide(histDataIn2e2muChannel,histExpected2e2mu);
                ratio2e2mu->SetLineColor(kBlack);
                ratio2e2mu->SetMarkerStyle(20);
                ratio2e2mu->SetMarkerSize(0.6);
                ratio2e2mu->Draw();
                ratio2e2mu->GetXaxis()->SetRangeUser(lower,upper);
                ratio2e2mu->GetXaxis()->SetTitle("Bin nr.");
                ratio2e2mu->GetYaxis()->SetTitle("Ratio");
                ratio2e2mu->Fit("pol0","","",lowerRatio,upper);
            }

        }
        if(only4mu)
        {
            //Do the 4mu part
            canv->cd(1);
            
            
            //Make the legend for 4mu
            cout<<"Making legend for 4mu"<<endl;
            THStack *stack4muSingleChannel=new THStack("stack4muSingleChannel","");
            hist2mu2tauIn4muChannel->SetFillColor(kMagenta);
            hist4tauIn4muChannel->SetFillColor(kBlue);
            hist4muIn4muChannel->SetFillColor(kRed);
            stack4muSingleChannel->Add(hist2mu2tauIn4muChannel);
            stack4muSingleChannel->Add(hist4tauIn4muChannel);
            stack4muSingleChannel->Add(hist4muIn4muChannel);
            if(addSignal) stack4muSingleChannel->Add(histVBFIn4muChannel);
            if(addSignal) stack4muSingleChannel->Add(histGGFIn4muChannel);
            TLegend* legend4mu=new TLegend(0.6,0.65,0.88,0.85);
            legend4mu->SetTextFont(72);
            legend4mu->SetTextSize(0.04);
            if(addData) legend4mu->AddEntry(histDataIn4muChannel,"Data","P E L");
            if(addSignal) legend4mu->AddEntry(histVBFIn4muChannel,"Higgs signal","F");
            legend4mu->AddEntry(hist4muIn4muChannel,"4#mu","F");
            legend4mu->AddEntry(hist4tauIn4muChannel,"4#tau","F");
            legend4mu->AddEntry(hist2mu2tauIn4muChannel,"2e2#tau","F");
            
            
            //Making the canvas for 4mu
            cout<<"Making canvas for 4mu"<<endl;
            if(!addData&&zoomData) zoomData=false;
            if(zoomData) histDataIn4muChannel->Draw(); stack4muSingleChannel->Draw("same hist");
            if(!zoomData) stack4muSingleChannel->Draw("hist");
            if(addData) histDataIn4muChannel->Draw("same");
            legend4mu->Draw();
            stack4muSingleChannel->GetXaxis()->SetTitle("M_{4#mu} [GeV]");
            if(addData) histDataIn4muChannel->GetXaxis()->SetTitle("M_{4#mu} [GeV]");
            if(withRatio)
            {
                canv->cd(2);
                TObject *objStackExpected4mu=stack4muSingleChannel->GetStack()->Last();
                TH1D *histExpected4mu=(TH1D*)objStackExpected4mu->Clone("histExpected4mu");
                TH1D *ratio4mu=new TH1D("ratio","",nBins,lower,upper);
                ratio4mu->Divide(histDataIn4muChannel,histExpected4mu);
                ratio4mu->SetLineColor(kBlack);
                ratio4mu->SetMarkerStyle(20);
                ratio4mu->SetMarkerSize(0.6);
                ratio4mu->Draw();
                ratio4mu->GetXaxis()->SetRangeUser(lower,upper);
                ratio4mu->GetXaxis()->SetTitle("Bin nr.");
                ratio4mu->GetYaxis()->SetTitle("Ratio");
                ratio4mu->Fit("pol0","","",lowerRatio,upper);
            }
        }
        if(!only4e&&!only2e2mu&&!only4mu&&separate)
        {
        //Do the 4e part
        canv->cd(1);
        
        
        //Make the legend for 4e
        cout<<"Making legend for 4e"<<endl;
        THStack *stack4eSingleChannel=new THStack("stack4eSingleChannel","");
        hist2e2tauIn4eChannel->SetFillColor(kCyan);
        hist4tauIn4eChannel->SetFillColor(kBlue);
        hist4eIn4eChannel->SetFillColor(kGreen);
        stack4eSingleChannel->Add(hist2e2tauIn4eChannel);
        stack4eSingleChannel->Add(hist4tauIn4eChannel);
        stack4eSingleChannel->Add(hist4eIn4eChannel);
        if(addSignal) stack4eSingleChannel->Add(histVBFIn4eChannel);
        if(addSignal) stack4eSingleChannel->Add(histGGFIn4eChannel);
        TLegend* legend4e=new TLegend(0.6,0.65,0.88,0.85);
        legend4e->SetTextFont(72);
        legend4e->SetTextSize(0.04);
        if(addData) legend4e->AddEntry(histDataIn4eChannel,"Data","P E L");
        if(addSignal) legend4e->AddEntry(histVBFIn4eChannel,"Higgs signal","F");
        legend4e->AddEntry(hist4eIn4eChannel,"4e","F");
        legend4e->AddEntry(hist4tauIn4eChannel,"4#tau","F");
        legend4e->AddEntry(hist2e2tauIn4eChannel,"2e2#tau","F");
        
        
        //Making the canvas for 4e
        cout<<"Making canvas for 4e"<<endl;
        if(!addData&&zoomData) zoomData=false;
        if(zoomData) histDataIn4eChannel->Draw(); stack4eSingleChannel->Draw("same hist");
        if(!zoomData) stack4eSingleChannel->Draw("hist");
        if(addData) histDataIn4eChannel->Draw("same");
        legend4e->Draw();
        stack4eSingleChannel->GetXaxis()->SetTitle("M_{4e} [GeV]");
        if(addData) histDataIn4eChannel->GetXaxis()->SetTitle("M_{4e} [GeV]");
        if(withRatio)
        {
            canv->cd(4);
            TObject *objStackExpected4e=stack4eSingleChannel->GetStack()->Last();
            TH1D *histExpected4e=(TH1D*)objStackExpected4e->Clone("histExpected4e");
            TH1D *ratio4e=new TH1D("ratio","",nBins,lower,upper);
            ratio4e->Divide(histDataIn4eChannel,histExpected4e);
            ratio4e->SetLineColor(kBlack);
            ratio4e->SetMarkerStyle(20);
            ratio4e->SetMarkerSize(0.6);
            ratio4e->Draw();
            ratio4e->GetXaxis()->SetRangeUser(lower,upper);
            ratio4e->GetXaxis()->SetTitle("Bin nr.");
            ratio4e->GetYaxis()->SetTitle("Ratio");
            ratio4e->Fit("pol0","","",lowerRatio,upper);
        }
        
        
        //Do the 2e2mu part
        canv->cd(2);
        
        
        //Make the legend for 2e2mu
        cout<<"Making legend for 2e2mu"<<endl;
        hist4tauIn2e2muChannel->SetFillColor(kBlue);
        hist2e2muIn2e2muChannel->SetFillColor(kYellow);
        hist2e2tauIn2e2muChannel->SetFillColor(kCyan);
        hist2mu2tauIn2e2muChannel->SetFillColor(kMagenta);
        THStack *stack2e2muSingleChannel=new THStack("stack2e2muSingleChannel","");
        stack2e2muSingleChannel->Add(hist4tauIn2e2muChannel);
        stack2e2muSingleChannel->Add(hist2mu2tauIn2e2muChannel);
        stack2e2muSingleChannel->Add(hist2e2tauIn2e2muChannel);
        stack2e2muSingleChannel->Add(hist2e2muIn2e2muChannel);
        if(addSignal) stack2e2muSingleChannel->Add(histVBFIn2e2muChannel);
        if(addSignal) stack2e2muSingleChannel->Add(histGGFIn2e2muChannel);
        TLegend* legend2e2mu=new TLegend(0.6,0.65,0.88,0.85);
        legend2e2mu->SetTextFont(72);
        legend2e2mu->SetTextSize(0.04);
        if(addData) legend2e2mu->AddEntry(histDataIn2e2muChannel,"Data","P E L");
        if(addSignal) legend2e2mu->AddEntry(histVBFIn2e2muChannel,"Higgs signal","F");
        legend2e2mu->AddEntry(hist2e2muIn2e2muChannel,"2e2#mu","F");
        legend2e2mu->AddEntry(hist2e2tauIn2e2muChannel,"2e2#tau","F");
        legend2e2mu->AddEntry(hist2mu2tauIn2e2muChannel,"2#mu2#tau","F");
        legend2e2mu->AddEntry(hist4tauIn2e2muChannel,"4#tau","F");
        
        
        //Making the canvas for 2e2mu
        cout<<"Making canvas for 2e2mu"<<endl;
        if(!addData&&zoomData) zoomData=false;
        if(zoomData) histDataIn2e2muChannel->Draw(); stack2e2muSingleChannel->Draw("same hist");
        if(!zoomData) stack2e2muSingleChannel->Draw("hist");
        if(addData) histDataIn2e2muChannel->Draw("same");
        legend2e2mu->Draw();
        stack2e2muSingleChannel->GetXaxis()->SetTitle("M_{2e2#mu} [GeV]");
        if(addData) histDataIn2e2muChannel->GetXaxis()->SetTitle("M_{2e2#mu} [GeV]");
        if(withRatio)
        {
            canv->cd(5);
            TObject *objStackExpected2e2mu=stack2e2muSingleChannel->GetStack()->Last();
            TH1D *histExpected2e2mu=(TH1D*)objStackExpected2e2mu->Clone("histExpected2e2mu");
            TH1D *ratio2e2mu=new TH1D("ratio","",nBins,lower,upper);
            ratio2e2mu->Divide(histDataIn2e2muChannel,histExpected2e2mu);
            ratio2e2mu->SetLineColor(kBlack);
            ratio2e2mu->SetMarkerStyle(20);
            ratio2e2mu->SetMarkerSize(0.6);
            ratio2e2mu->Draw();
            ratio2e2mu->GetXaxis()->SetRangeUser(lower,upper);
            ratio2e2mu->GetXaxis()->SetTitle("Bin nr.");
            ratio2e2mu->GetYaxis()->SetTitle("Ratio");
            ratio2e2mu->Fit("pol0","","",lowerRatio,upper);
        }

        
        //Do the 4mu part
        canv->cd(3);
        
        
        //Make the legend for 4mu
        cout<<"Making legend for 4mu"<<endl;
        THStack *stack4muSingleChannel=new THStack("stack4muSingleChannel","");
        hist2mu2tauIn4muChannel->SetFillColor(kMagenta);
        hist4tauIn4muChannel->SetFillColor(kBlue);
        hist4muIn4muChannel->SetFillColor(kRed);
        stack4muSingleChannel->Add(hist2mu2tauIn4muChannel);
        stack4muSingleChannel->Add(hist4tauIn4muChannel);
        stack4muSingleChannel->Add(hist4muIn4muChannel);
        if(addSignal) stack4muSingleChannel->Add(histVBFIn4muChannel);
        if(addSignal) stack4muSingleChannel->Add(histGGFIn4muChannel);
        TLegend* legend4mu=new TLegend(0.6,0.65,0.88,0.85);
        legend4mu->SetTextFont(72);
        legend4mu->SetTextSize(0.04);
        if(addData) legend4mu->AddEntry(histDataIn4muChannel,"Data","P E L");
        if(addSignal) legend4mu->AddEntry(histVBFIn4muChannel,"Higgs signal","F");
        legend4mu->AddEntry(hist4muIn4muChannel,"4#mu","F");
        legend4mu->AddEntry(hist4tauIn4muChannel,"4#tau","F");
        legend4mu->AddEntry(hist2mu2tauIn4muChannel,"2e2#tau","F");
        
        
        //Making the canvas for 4mu
        cout<<"Making canvas for 4mu"<<endl;
        if(!addData&&zoomData) zoomData=false;
        if(zoomData) histDataIn4muChannel->Draw(); stack4muSingleChannel->Draw("same hist");
        if(!zoomData) stack4muSingleChannel->Draw("hist");
        if(addData) histDataIn4muChannel->Draw("same");
        legend4mu->Draw();
        stack4muSingleChannel->GetXaxis()->SetTitle("M_{4#mu} [GeV]");
        if(addData) histDataIn4muChannel->GetXaxis()->SetTitle("M_{4#mu} [GeV]");
        if(withRatio)
        {
            canv->cd(6);
            TObject *objStackExpected4mu=stack4muSingleChannel->GetStack()->Last();
            TH1D *histExpected4mu=(TH1D*)objStackExpected4mu->Clone("histExpected4mu");
            TH1D *ratio4mu=new TH1D("ratio","",nBins,lower,upper);
            ratio4mu->Divide(histDataIn4muChannel,histExpected4mu);
            ratio4mu->SetLineColor(kBlack);
            ratio4mu->SetMarkerStyle(20);
            ratio4mu->SetMarkerSize(0.6);
            ratio4mu->Draw();
            ratio4mu->GetXaxis()->SetRangeUser(lower,upper);
            ratio4mu->GetXaxis()->SetTitle("Bin nr.");
            ratio4mu->GetYaxis()->SetTitle("Ratio");
            ratio4mu->Fit("pol0","","",lowerRatio,upper);
        }
        }
        
        
        //Saving the canvas
        cout<<"Saving canvas"<<endl;
        canv->Modified();
    }
	else if(separate==2)
	{
		//Making the legend
		cout<<"Making legend"<<endl;
		TLegend* legend=new TLegend(0.6,0.65,0.88,0.85);
		legend->SetTextFont(72);
		legend->SetTextSize(0.04);
		if(addData) legend->AddEntry(histObserved,"Data","P E L");
		if(addSignal) legend->AddEntry(histStack4eSignal,"Signal","F");
		legend->AddEntry(histStack2e2muBackground,"2e2#mu","F");
		legend->AddEntry(histStack4muBackground,"4#mu","F");
		legend->AddEntry(histStack4eBackground,"4e","F");
		
		
		//Making the canvas
		cout<<"Making canvas"<<endl;
		if(withRatio) canv->Divide(1,2);
		canv->cd(1);
		if(!addData&&zoomData) zoomData=false;
		if(zoomData) histObserved->Draw(); stackExpected->Draw("same hist");
		if(!zoomData) stackExpected->Draw("hist");
		if(addData) histObserved->Draw("same");
		legend->Draw();
		stackExpected->GetXaxis()->SetTitle("M_{4l} [GeV]");
		if(addData) histObserved->GetXaxis()->SetTitle("M_{4l} [GeV]");
		if(withRatio)
		{
			canv->cd(2);
			TH1D *ratio=new TH1D("ratio","",nBins,lower,upper);
			ratio->Divide(histObserved,histExpected);
			ratio->SetLineColor(kBlack);
			ratio->SetMarkerStyle(20);
			ratio->SetMarkerSize(0.6);
			ratio->Draw();
			ratio->GetXaxis()->SetRangeUser(lower,upper);
			ratio->GetXaxis()->SetTitle("Bin nr.");
			ratio->GetYaxis()->SetTitle("Ratio");
			ratio->Fit("pol0","","",lowerRatio,upper);
		}
		
		
		//Saving the canvas
		cout<<"Saving canvas"<<endl;
		canv->Modified();
	}
	else if(separate==3)
	{
		//Making the canvas
		cout<<"Making the canvas"<<endl;
		canv->cd(1);
		TH1D *ratioSignal=new TH1D("ratioSignal","",nBins,lower,upper);
		ratioSignal->GetXaxis()->SetRangeUser(lowerRatio,upper);
		ratioSignal->Divide(histObserved,histExpected);
		ratioSignal->SetLineColor(kBlack);
		ratioSignal->SetMarkerStyle(20);
		ratioSignal->SetMarkerSize(0.6);
		ratioSignal->Draw();
		ratioSignal->GetXaxis()->SetTitle("Bin nr.");
		ratioSignal->GetYaxis()->SetTitle("Ratio");
		ratioSignal->Fit("pol0","","",lowerRatio,upper);
		ratioSignal->GetXaxis()->SetRangeUser(lower,upper);
		
		
		//Saving the canvas
		cout<<"Saving canvas"<<endl;
		canv->Modified();
	}
	
	
	//Get the statistical results (code provided by the promoter)
	if(!suppressOutput)
	{
		//Define the needed variables
		cout<<"Defining statistical variables"<<endl;
        int lowerTestLimit=109;
        int upperTestLimit=141;
        int testBins=(upperTestLimit-lowerTestLimit)/2;
        TH1D *histPExpected=new TH1D("histPExpected","",testBins,lowerTestLimit,upperTestLimit);
        TH1D *histPObserved=new TH1D("histPObserved","",testBins,lowerTestLimit,upperTestLimit);
        TH1D *histSigmaExpected=new TH1D("histSigmaExpected","",testBins,lowerTestLimit,upperTestLimit);
        TH1D *histSigmaObserved=new TH1D("histSigmaObserved","",testBins,lowerTestLimit,upperTestLimit);
		
        
        //Make a histogram for the background only
        cout<<"Making background hist"<<endl;
        THStack *stackBackground=new THStack("stackBackground","");
		stackBackground->Add(histStack4eBackground);
		stackBackground->Add(histStack2e2muBackground);
		stackBackground->Add(histStack4muBackground);
		TObject *objStackBackgroundTemp=stackBackground->GetStack()->Last();
		TH1D *histStackBackground=(TH1D*)objStackBackgroundTemp->Clone("histStackBackground");
		
        
        //Make a histogram for the signal only
        cout<<"Making signal hist"<<endl;
		THStack *stackSignal=new THStack("stackSignal","");
		stackSignal->Add(histStack4eSignal);
		stackSignal->Add(histStack2e2muSignal);
		stackSignal->Add(histStack4muSignal);
		TObject *objStackSignalTemp=stackSignal->GetStack()->Last();
		TH1D *histStackSignal=(TH1D*)objStackSignalTemp->Clone("histStackSignal");
		
        
		//Loop over all the interresting bins
		cout<<"Looping over bins"<<endl;
        for(int iHiggs=lowerTestLimit; iHiggs<upperTestLimit; iHiggs=iHiggs+2)
        {
            //Define and compute the needed variables
            int iBinCenter=histObserved->GetXaxis()->FindBin(iHiggs);
			double integralValData,integralValCombined,integralValBackground,integralValSignal;
            double errorValData,errorValCombined,errorValBackground,errorValSignal;
            
            integralValBackground=histStackBackground->IntegralAndError(iBinCenter-1,iBinCenter+1,errorValBackground);
            integralValSignal=histStackSignal->IntegralAndError(iBinCenter-1,iBinCenter+1,errorValSignal );
            integralValCombined=histExpected->IntegralAndError(iBinCenter-1,iBinCenter+1,errorValCombined);
            integralValData=histObserved->IntegralAndError(iBinCenter-1,iBinCenter+1,errorValData);
            
            double signalExpected=integralValSignal;
            double backgroundExpected=integralValBackground;
            double relativeBkgUncert=errorValBackground/integralValBackground;
            double pExp=NumberCountingUtils::BinomialExpP(signalExpected, backgroundExpected, relativeBkgUncert);
            double zExp=NumberCountingUtils::BinomialExpZ(signalExpected, backgroundExpected, relativeBkgUncert);
            double observed=integralValData;
            double pObs=NumberCountingUtils::BinomialObsP(observed, backgroundExpected, relativeBkgUncert);
            double zObs=NumberCountingUtils::BinomialObsZ(observed, backgroundExpected, relativeBkgUncert);

            
			//Display the statistical results
            cout<<"****************************************"<<endl;
            cout<<"Bin "<<iBinCenter<< " goes with Higgs mass"<<iHiggs<<endl;
			cout<<"1 bin int: "<<"Background: "<<histStackBackground->GetBinContent(iBinCenter)<<"±"<<histStackBackground->GetBinError(iBinCenter)<<endl;
            cout<<"1 bin int: "<<"Signal: "	   <<histStackSignal->GetBinContent(iBinCenter)	   <<"±"<<histStackSignal->GetBinError(iBinCenter)<<endl;
            cout<<"1 bin int: "<<"Expected: "  <<histExpected->GetBinContent(iBinCenter)	   <<"±"<<histExpected->GetBinError(iBinCenter)<<endl;
            cout<<"1 bin int: "<<"Observed: "  <<histObserved->GetBinContent(iBinCenter)	   <<"±"<<histObserved->GetBinError(iBinCenter)<<endl;
            cout<<endl;
            cout<<"3 bin int: "<<"Background: "<<integralValBackground<<"±"<<errorValBackground<<endl;
            cout<<"3 bin int: "<<"Signal: "    <<integralValSignal    <<"±"<<errorValSignal    <<endl;
            cout<<"3 bin int: "<<"Expected: "  <<integralValCombined  <<"±"<<errorValCombined  <<endl;
            cout<<"3 bin int: "<<"Data: "      <<integralValData      <<"±"<<errorValData      <<endl;
			cout<<endl;
			cout<<"Expected P-value: "<<pExp<<endl;
			cout<<"Observed P-value: "<<pObs<<endl;
			cout<<"Expected Z-value: "<<zExp<<endl;
			cout<<"Observed Z-value: "<<zObs<<endl;
            cout<<endl;
			
			
			//Fill the histograms
            histPObserved->Fill(iHiggs,pObs);
            histPExpected->Fill(iHiggs,pExp);
            histSigmaExpected->Fill(iHiggs,zExp);
            histSigmaObserved->Fill(iHiggs,zObs);
        }
		
		
		//Making the legend
		cout<<"Making legend"<<endl;
		TLegend* legendP=new TLegend(0.6,0.65,0.88,0.85);
		legendP->SetTextFont(72);
		legendP->SetTextSize(0.04);
		legendP->AddEntry(histPExpected,"Expected","L");
		legendP->AddEntry(histPObserved,"Observed","L");
		
		TLegend* legendSigma=new TLegend(0.6,0.65,0.88,0.85);
		legendSigma->SetTextFont(72);
		legendSigma->SetTextSize(0.04);
		legendSigma->AddEntry(histSigmaObserved,"Observed","L");
		legendSigma->AddEntry(histSigmaExpected,"Expected","L");


        //Drawing the statistical results
        TCanvas *canvP=new TCanvas("canvP","");
        gStyle->SetOptStat("");
        gPad->SetLogy();
        histPObserved->Draw("hist");
        histPExpected->SetLineColor(kRed);
        histPExpected->SetLineWidth(2);
        histPExpected->Draw("same hist");
		legendP->Draw();
        histPObserved->GetXaxis()->SetTitle("M_{4l} [GeV]");
        histPExpected->GetXaxis()->SetTitle("4l} [GeV]");
        histPObserved->GetYaxis()->SetTitle("P value");
        histPExpected->GetYaxis()->SetTitle("P value");
        canvP->Modified();
        canvP->Print("Figures/PValue.eps");
        
        
        TCanvas *canvSigma=new TCanvas("canvSigma","");
        gStyle->SetOptStat("");
        histSigmaObserved->Draw("  hist");
        histSigmaExpected->SetLineColor(kRed);
        histSigmaExpected->SetLineWidth(2);
		histSigmaExpected->Draw("same hist");
		legendSigma->Draw();
        histSigmaObserved->GetXaxis()->SetTitle("M_{4l} [GeV]");
        histSigmaExpected->GetXaxis()->SetTitle("M_{4l} [GeV]");
        histSigmaObserved->GetYaxis()->SetTitle("#sigma (data-simulation)");
        histSigmaExpected->GetYaxis()->SetTitle("#sigma (data-simulation)");
        canvSigma->Modified();
        canvSigma->Print("Figures/Sigma.eps");
	}
	 
    
    //Debuggers
	if(!fast)
	{
		cout<<"Debugging"<<endl;
		TCanvas *debugger1=new TCanvas("Debugger1","");
		debugger1->Divide(3,3);
		debugger1->cd(1);
		hist4eIn4eChannel->Draw("hist");
		debugger1->cd(2);
		hist4muIn4eChannel->Draw("hist");
		debugger1->cd(3);
		hist4tauIn4eChannel->Draw("hist");
		debugger1->cd(4);
		hist2e2muIn4eChannel->Draw("hist");
		debugger1->cd(5);
		hist2e2tauIn4eChannel->Draw("hist");
		debugger1->cd(6);
		hist2mu2tauIn4eChannel->Draw("hist");
		debugger1-> Modified();
		debugger1-> Print("Debugging/debugger1.eps");
		
		TCanvas *debugger2=new TCanvas("Debugger2","");
		debugger2->Divide(3,3);
		debugger2->cd(1);
		hist4eIn2e2muChannel->Draw("hist");
		debugger2->cd(2);
		hist4muIn2e2muChannel->Draw("hist");
		debugger2->cd(3);
		hist4tauIn2e2muChannel->Draw("hist");
		debugger2->cd(4);
		hist2e2muIn2e2muChannel->Draw("hist");
		debugger2->cd(5);
		hist2e2tauIn2e2muChannel->Draw("hist");
		debugger2->cd(6);
		hist2mu2tauIn2e2muChannel->Draw("hist");
		debugger2-> Modified();
		debugger2-> Print("Debugging/debugger2.eps");
		
		TCanvas *debugger3=new TCanvas("Debugger3","");
		debugger3->Divide(3,3);
		debugger3->cd(1);
		hist4eIn4muChannel->Draw("hist");
		debugger3->cd(2);
		hist4muIn4muChannel->Draw("hist");
		debugger3->cd(3);
		hist4tauIn4muChannel->Draw("hist");
		debugger3->cd(4);
		hist2e2muIn4muChannel->Draw("hist");
		debugger3->cd(5);
		hist2e2tauIn4muChannel->Draw("hist");
		debugger3->cd(6);
		hist2mu2tauIn4muChannel->Draw("hist");
		debugger3-> Modified();
		debugger3-> Print("Debugging/debugger3.eps");
		
		TCanvas *debugger4=new TCanvas("Debugger4","");
		debugger4->Divide(3,1);
		debugger4->cd(1);
		stack4eBackground->Draw("hist");
		debugger4->cd(2);
		stack2e2muBackground->Draw("hist");
		debugger4->cd(3);
		stack4muBackground->Draw("hist");
		debugger4-> Modified();
		debugger4-> Print("Debugging/debugger4.eps");
		
		TCanvas *debugger5=new TCanvas("Debugger5","");
		debugger5->Divide(2,3);
		debugger5->cd(1);
		histVBFIn4eChannel->Draw("hist");
		debugger5->cd(2);
		histVBFIn2e2muChannel->Draw("hist");
		debugger5->cd(3);
		histVBFIn4muChannel->Draw("hist");
		debugger5->cd(4);
		histGGFIn4eChannel->Draw("hist");
		debugger5->cd(5);
		histGGFIn2e2muChannel->Draw("hist");
		debugger5->cd(6);
		histGGFIn4muChannel->Draw("hist");
		debugger5-> Modified();
		debugger5-> Print("Debugging/debugger5.eps");
	}
	
	
	//Return the desired canvas
	cout<<"Returning canvas"<<endl;
	return canv;
}




void Run(int nBins=100,
         int lower=25,
         int upper=225,
         int lowerRatio=110,
         bool suppressOutput=true,
         bool cut=false,
         bool fast=true)
{
    //Small functions first
	Signal();
	ZPeak();
	RatioPlots();
    //                              n            b  b  a  a  w  s                           z
    //                              o            l  l  d  d  i  e                           o
    //                              r            i  i  d  d  t  p  4  2  4                  o
    //                              m            n  n        h  a  e  e  m                  m
    //                              a            d  d  d  s     r     2  u
    //                              l                  a  i  r  a     m                     d
    //                              i            a  d  t  g  a  t     u                     a
    //                              s            l  a  a  n  t  e                           t
    //                              e            l  t  .  .  .                              a
    Bachelorproef(nBins,    0,  500,0,lowerRatio,1, 1, 0, 0, 0, 1, 0, 0, 0, suppressOutput, 0, cut, fast)->Print("Figures/mass1.eps");
    Bachelorproef(nBins,lower,upper,1,lowerRatio,1, 1, 0, 0, 0, 1, 0, 0, 0, suppressOutput, 0, cut, fast)->Print("Figures/mass2.eps");
    Bachelorproef(nBins,lower,upper,1,lowerRatio,1, 1, 1, 0, 1, 2, 1, 0, 0, suppressOutput, 1, cut, fast)->Print("Figures/mass3a.eps");
    Bachelorproef(nBins,lower,upper,1,lowerRatio,1, 1, 1, 0, 1, 2, 0, 1, 0, suppressOutput, 1, cut, fast)->Print("Figures/mass4a.eps");
    Bachelorproef(nBins,lower,upper,1,lowerRatio,1, 1, 1, 0, 1, 2, 0, 0, 1, suppressOutput, 1, cut, fast)->Print("Figures/mass5a.eps");
    Bachelorproef(nBins,lower,upper,2,lowerRatio,1, 1, 1, 0, 1, 2, 1, 0, 0, suppressOutput, 1, cut, fast)->Print("Figures/mass3b.eps");
    Bachelorproef(nBins,lower,upper,2,lowerRatio,1, 1, 1, 0, 1, 2, 0, 1, 0, suppressOutput, 1, cut, fast)->Print("Figures/mass4b.eps");
    Bachelorproef(nBins,lower,upper,2,lowerRatio,1, 1, 1, 0, 1, 2, 0, 0, 1, suppressOutput, 1, cut, fast)->Print("Figures/mass5b.eps");
    Bachelorproef(nBins,lower,upper,2,lowerRatio,1, 1, 1, 0, 0, 2, 0, 0, 0, suppressOutput, 1, cut, fast)->Print("Figures/mass6.eps");
    Bachelorproef(nBins,lower,upper,2,lowerRatio,0, 0, 1, 1, 0, 2, 0, 0, 0, suppressOutput, 1, cut, fast)->Print("Figures/mass7.eps");
    Bachelorproef(nBins,lower,upper,2,lowerRatio,0, 0, 1, 1, 1, 2, 0, 0, 0,              0, 1, cut,    0)->Print("Figures/mass8.eps");
	Bachelorproef(nBins,lower,upper,2,lowerRatio,0, 0, 1, 0, 0, 3, 0, 0, 0, suppressOutput, 1, cut, fast)->Print("Figures/ratioBackground.eps");
	Bachelorproef(nBins,lower,upper,2,lowerRatio,0, 0, 1, 1, 0, 3, 0, 0, 0, suppressOutput, 1, cut, fast)->Print("Figures/ratioSignal.eps");
}
