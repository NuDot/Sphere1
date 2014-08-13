#include "Style.hh"

using namespace std;

void format_g(TGraphErrors* g, int color, int ms){
  g->SetMarkerColor(color);
  g->SetMarkerStyle(ms);
  g->SetLineColor(color); 
  //h->SetLineColor(lc);
  //h->SetLineWidth(lw);
  //h->SetLineStyle(ls);
  //h->GetXaxis()->CenterTitle();
  //h->GetYaxis()->CenterTitle();
}

void Energy_Dependency_Plots(){

  TStyle* jStyle = new TStyle("jStyle", "jStyle");
  Style(jStyle,1);

  std::string out ="Energy_Dependency_Plots";
  std::string folder ="/home/christoph/geant4/MyG4_applications/Sphere1-build/output";
  TFile *tf = new TFile(Form("%s/%s.root",folder.c_str(),out.c_str()),"RECREATE");
  TGraphErrors * NpeCer_vs_energy = new TGraphErrors();
  TGraphErrors * NpeScint_vs_energy = new TGraphErrors();
  TGraphErrors * RCS1_vs_energy = new TGraphErrors();
  TGraphErrors * RCS2_vs_energy = new TGraphErrors();
  TMultiGraph *mg = new TMultiGraph;
  TMultiGraph *mg2 = new TMultiGraph;  

 
  //input: time cut of 34 ns is used
  Double_t energy[3]={1.4,2.1,5.0};
  Double_t N_Cherenkov_timecut[3]={21.81,38.379,107.591};
  Double_t N_Cherenkov_timecut_error[3]={5.54497,7.5181,14.7953};
  Double_t N_Scint_timecut[3]={52.082,76.783,171.445};
  Double_t N_Scint_timecut_error[3]={7.25244,8.50731,14.4241};
  Double_t RCS1[3]; //RCS1 is the ratio of the mean number of PE from Cerenkov over the mean number of PE from Scintillation
  Double_t RCS1_error[3]; // this error is calculated using gaussian error propagation which assumes that the number of PE from Cerenkov and Scint are independent (which seems to be OK approximation)
  Double_t RCS2[3]={0.426642,0.505487,0.631718}; // RCS2 is the mean of the ratios of NPE from Cerenkov over NPE from Scintillation for single events  (NOTE difference to RCS1)
  Double_t RCS2_error[3]={0.123741,0.113794,0.100085};
  
  //calculate RCS1 and fill TGraphs
  for(Int_t ii=0;ii<3;ii++)
    { 
      //use the following three lines if standard error of the mean should be displayed
      N_Cherenkov_timecut_error[ii]=N_Cherenkov_timecut_error[ii]/sqrt(1000.);
      N_Scint_timecut_error[ii]=N_Scint_timecut_error[ii]/sqrt(1000.); 
      RCS2_error[ii]=RCS2_error[ii]/sqrt(1000.);
      //now fill graphs 
      NpeCer_vs_energy->SetPoint(ii,energy[ii],N_Cherenkov_timecut[ii]);
      NpeCer_vs_energy->SetPointError(ii,0.,N_Cherenkov_timecut_error[ii]);
      NpeScint_vs_energy->SetPoint(ii,energy[ii],N_Scint_timecut[ii]);
      NpeScint_vs_energy->SetPointError(ii,0.,N_Scint_timecut_error[ii]);
      RCS1[ii]=N_Cherenkov_timecut[ii]/N_Scint_timecut[ii]; //Here the ratio of the means is taken. Then we have to calculate an error. We assume that the errors of the mean are independent and do gaussian error propagation.
      RCS1_error[ii]=sqrt((N_Cherenkov_timecut_error[ii]/N_Scint_timecut[ii])*(N_Cherenkov_timecut_error[ii]/N_Scint_timecut[ii]) + (N_Cherenkov_timecut[ii]*N_Scint_timecut_error[ii]/(N_Scint_timecut[ii]*N_Scint_timecut[ii]))*(N_Cherenkov_timecut[ii]*N_Scint_timecut_error[ii]/(N_Scint_timecut[ii]*N_Scint_timecut[ii]))); 
      //cout << ii << " " << RCS1_error[ii] << endl; 
      RCS1_vs_energy->SetPoint(ii,energy[ii],RCS1[ii]);
      RCS1_vs_energy->SetPointError(ii,0.,RCS1_error[ii]);
      
      RCS2_vs_energy->SetPoint(ii,energy[ii],RCS2[ii]);
      RCS2_vs_energy->SetPointError(ii,0.,RCS2_error[ii]);
    }

  //set graph details
  Int_t width_pixels= 700;  //for automatic output graphs  //??tbd
  Int_t height_pixels=500;  // for automatic output graphs //??tbd

  NpeCer_vs_energy->SetName("NpeCer_vs_energy");
  NpeScint_vs_energy->SetName("NpeScint_vs_energy"); 
  RCS1_vs_energy->SetName("RCS1_vs_energy");
  RCS2_vs_energy->SetName("RCS2_vs_energy");


  TCanvas* c1 = new TCanvas("c1", "Cherenkov", width_pixels, height_pixels);
  c1->SetFrameLineWidth(3);
  //choice of colors, symboles
  format_g(NpeCer_vs_energy,1,20);
  //NpeCer_vs_energy->Draw("ap");
  NpeCer_vs_energy->GetXaxis()->SetLimits(0.,6.0);
  NpeCer_vs_energy->GetYaxis()->SetRangeUser(0.,200.0);
  NpeCer_vs_energy->GetXaxis()->SetTitle("E [MeV]");
  NpeCer_vs_energy->GetYaxis()->SetTitle("PEs per event");
  NpeCer_vs_energy->GetXaxis() -> CenterTitle(true);
  NpeCer_vs_energy->GetYaxis() -> CenterTitle(true);
  NpeCer_vs_energy->Draw("ap");
  c1->Update();
  c1->SaveAs(Form("%s/NpeCer_vs_energy.png",folder.c_str()),"png");
  c1->SaveAs(Form("%s/NpeCer_vs_energy.C",folder.c_str()),"C");
  c1->SaveAs(Form("%s/NpeCer_vs_energy.pdf",folder.c_str()),"pdf");  
    
  TCanvas* c2 = new TCanvas("c2", "Scint", width_pixels, height_pixels); 
  c2->SetFrameLineWidth(3);
  format_g(NpeScint_vs_energy,2,20);
  //NpeCer_vs_energy->Draw("ap");
  NpeScint_vs_energy->GetXaxis()->SetLimits(0.,6.0);
  NpeScint_vs_energy->GetYaxis()->SetRangeUser(0.,200.0);
  NpeScint_vs_energy->GetXaxis()->SetTitle("E [MeV]");
  NpeScint_vs_energy->GetYaxis()->SetTitle("PEs per event");
  NpeScint_vs_energy->GetXaxis() -> CenterTitle(true);
  NpeScint_vs_energy->GetYaxis() -> CenterTitle(true);
  NpeScint_vs_energy->Draw("ap");
  c2->Update();
  c2->SaveAs(Form("%s/NpeScint_vs_energy.png",folder.c_str()),"png");
  c2->SaveAs(Form("%s/NpeScint_vs_energy.C",folder.c_str()),"C");
  c2->SaveAs(Form("%s/NpeScint_vs_energy.pdf",folder.c_str()),"pdf");

  TCanvas* c3 = new TCanvas("c3", "S plus C", width_pixels, height_pixels);
  c3->SetFrameLineWidth(3);
  mg->Add(NpeScint_vs_energy);
  mg->Add(NpeCer_vs_energy);
  mg->Draw("ap");
  mg->GetXaxis()->SetLimits(0.,6.0);
  mg->GetYaxis()->SetRangeUser(0.,200.0);
  mg->GetXaxis()->SetTitle("E [MeV]");
  mg->GetYaxis()->SetTitle("PEs per event");
  mg->GetXaxis() -> CenterTitle(true);
  mg->GetYaxis() -> CenterTitle(true);
  mg->Draw("ap");
  c3->Update();
  c3->SaveAs(Form("%s/SplusC_vs_energy.png",folder.c_str()),"png");
  c3->SaveAs(Form("%s/SplusC_vs_energy.C",folder.c_str()),"C");
  c3->SaveAs(Form("%s/SplusC_vs_energy.pdf",folder.c_str()),"pdf");

  TCanvas* c4 = new TCanvas("c4", "R_CS1", width_pixels, height_pixels);
  c4->SetFrameLineWidth(3);
  format_g(RCS1_vs_energy,1,20);
  RCS1_vs_energy->GetXaxis()->SetLimits(0.,6.0);
  RCS1_vs_energy->GetYaxis()->SetRangeUser(0.,0.8);
  RCS1_vs_energy->GetXaxis()->SetTitle("E [MeV]");
  RCS1_vs_energy->GetYaxis()->SetTitle("R_{C/S}");
  RCS1_vs_energy->GetXaxis() -> CenterTitle(true);
  RCS1_vs_energy->GetYaxis() -> CenterTitle(true);
  RCS1_vs_energy->Draw("ap");
  c4->Update();
  c4->SaveAs(Form("%s/RCS1_vs_energy.png",folder.c_str()),"png");
  c4->SaveAs(Form("%s/RCS1_vs_energy.C",folder.c_str()),"C");
  c4->SaveAs(Form("%s/RCS1_vs_energy.pdf",folder.c_str()),"pdf");

  TCanvas* c5 = new TCanvas("c5", "R_CS2", width_pixels, height_pixels);
  c5->SetFrameLineWidth(3);
  format_g(RCS2_vs_energy,2,20);
  RCS2_vs_energy->GetXaxis()->SetLimits(0.,6.0);
  RCS2_vs_energy->GetYaxis()->SetRangeUser(0.,0.8);
  RCS2_vs_energy->GetXaxis()->SetTitle("E [MeV]");
  RCS2_vs_energy->GetYaxis()->SetTitle("R_{C/S}");
  RCS2_vs_energy->GetXaxis() -> CenterTitle(true);
  RCS2_vs_energy->GetYaxis() -> CenterTitle(true);
  RCS2_vs_energy->Draw("ap");
  c5->Update();
  c5->SaveAs(Form("%s/RCS2_vs_energy.png",folder.c_str()),"png");
  c5->SaveAs(Form("%s/RCS2_vs_energy.C",folder.c_str()),"C");
  c5->SaveAs(Form("%s/RCS2_vs_energy.pdf",folder.c_str()),"pdf"); 

  TCanvas* c6 = new TCanvas("c6", "RCS1 vs RCS2", width_pixels, height_pixels);
  c6->SetFrameLineWidth(3);
  mg2->Add(RCS1_vs_energy);
  mg2->Add(RCS2_vs_energy);
  mg2->Draw("ap");
  mg2->GetXaxis()->SetLimits(0.,6.0);
  mg2->GetYaxis()->SetRangeUser(0.,0.8);
  mg2->GetXaxis()->SetTitle("E [MeV]");
  mg2->GetYaxis()->SetTitle("R_{C/S}");
  mg2->GetXaxis() -> CenterTitle(true);
  mg2->GetYaxis() -> CenterTitle(true);
  mg2->Draw("ap");
  c6->Update();
  c6->SaveAs(Form("%s/RCSs_vs_energy.png",folder.c_str()),"png");
  c6->SaveAs(Form("%s/RCSs_vs_energy.C",folder.c_str()),"C");
  c6->SaveAs(Form("%s/RCSs_vs_energy.pdf",folder.c_str()),"pdf");

  NpeCer_vs_energy->Write();
  NpeScint_vs_energy->Write();
  RCS1_vs_energy->Write();
}
