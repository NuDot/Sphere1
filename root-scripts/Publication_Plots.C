#include "Style.hh"

using namespace std;

void format_h(TH1F* h, int lc, int lw, int ls){
  h->SetLineColor(lc);
  h->SetLineWidth(lw);
  h->SetLineStyle(ls);
  //h->GetXaxis()->CenterTitle();
  //h->GetYaxis()->CenterTitle();
}


void Publication_Plots(){

  //-------------------------------------------------------------------------------------------------------
  //use the style defined by Julia

  Double_t N_events_double = 1000.;   // number of events to divide histograms by
  
  TStyle* jStyle = new TStyle("jStyle", "jStyle");
  Style(jStyle,1);

  //-------------------------------------------------------------------------------------------------------
  //user input
  //?
  //Double_t time_cut = 3.265; //0.65m diameter detector
  //Double_t time_cut = 33.0;  //6.5m diameter detector
  std::string simulation = "/home/christoph/geant4/MyG4_applications/Sphere1-build/output/December2013_JINST_referees/02January_2014_with_scattering_tagged";
  std::string out = simulation+"/PublicationPlots";
  Int_t width_pixels= 1400;  //for automatic output graphs  //??tbd
  Int_t height_pixels=1000;  // for automatic output graphs //??tbd

  //-------------------------------------------------------------------------------------------------------
  //define the Trees and fill them from the output files of the simulation
  //TTree *WL_Tree = new TTree("WL_Tree", "WL_Tree");
  TFile* input_file = new TFile(Form("%s/output.root",simulation.c_str()));
  TTree* Hits_Tree = (TTree*)input_file->Get("Hits_Tree"); 
  //TTree *Hits_Tree = new TTree("Hits_Tree", "Hits_Tree");      // Photon hits on sensitive elements
  //Hits_Tree->ReadFile(Form("%s/photon_hits.txt",simulation.c_str()),"x_hit:y_hit:z_hit:cos_theta:photon_wavelength:true_time:PE_creation:PE_time:detection_coverage_included:true_time_corrected:PE_time_corrected:cos_theta_reco:theta_reco:phi_reco:process:eventID");

  //-------------------------------------------------------------------------------------------------------
  //define the histograms which are output by default
  TH1F * time_afterTTS_Cerenkov = new TH1F("time_afterTTS_Cerenkov",";Time [ns]; PEs per event/0.1 ns",220,28.,50.);   
  TH1F * time_afterTTS_Cerenkov_full = new TH1F("time_afterTTS_Cerenkov_full",";Time [ns]; PEs per event/0.1 ns",10000,0.,1000.);  
  TH1F * time_afterTTS_Scintillation = new TH1F("time_afterTTS_Scintillation",";Time [ns]; PEs per event/0.1 ns",220,28.,50.);
  TH1F * time_afterTTS_Scintillation_full = new TH1F("time_afterTTS_Scintillation_full",";Time [ns]; PEs per event/0.1 ns",10000,0.,1000.);   

  TH1F * cos_psi_33_h = new TH1F("cos_psi_33_h",";cos(#Psi);PEs per event/0.02",100,-1.,1.);
  TH1F * cos_psi_34_h = new TH1F("cos_psi_34_h",";cos(#Psi);PEs per event/0.02",100,-1.,1.);

  //-------------------------------------------------------------------------------------------------------
  //fill histograms     
  Hits_Tree->Draw("PE_time>>time_afterTTS_Cerenkov","PE_creation==1 && process==1");
  Hits_Tree->Draw("PE_time>>time_afterTTS_Cerenkov_full","PE_creation==1 && process==1");
  Hits_Tree->Draw("PE_time>>time_afterTTS_Scintillation","PE_creation==1 && process==0");
  Hits_Tree->Draw("PE_time>>time_afterTTS_Scintillation_full","PE_creation==1 && process==0");
  // calculate the angle between the original electron direction (x direction) and the PE hits (after a time cut of 34 ns has been applied.)
  Hits_Tree->Draw("x_hit/sqrt(x_hit*x_hit + y_hit*y_hit + z_hit*z_hit) >> cos_psi_33_h","PE_creation == 1 && PE_time < 33.0");
  Hits_Tree->Draw("x_hit/sqrt(x_hit*x_hit + y_hit*y_hit + z_hit*z_hit) >> cos_psi_34_h","PE_creation == 1 && PE_time < 34.0");


  //------------------------------------------------------------------------------------
  // divide histograms by 1000 to get average per event
  time_afterTTS_Cerenkov->Scale(1./N_events_double);
  time_afterTTS_Cerenkov_full->Scale(1./N_events_double);
  time_afterTTS_Scintillation->Scale(1./N_events_double);
  time_afterTTS_Scintillation_full->Scale(1./N_events_double);
  cos_psi_33_h->Scale(1./N_events_double);
  cos_psi_34_h->Scale(1./N_events_double);

 
  //-------------------------------------------------------------------------------------------------------
  // calculate some integrals for the publication
  int count1=0;
  int count1_norm=0;
  int count2=0;
  int count2_norm=0;
  count1=time_afterTTS_Cerenkov->Integral(  time_afterTTS_Cerenkov->FindBin(0.)  ,  time_afterTTS_Cerenkov->FindBin(33.) -1 );  
  count1_norm=time_afterTTS_Cerenkov_full->Integral(  time_afterTTS_Cerenkov_full->FindBin(0.)  ,  time_afterTTS_Cerenkov_full->FindBin(1000.) -1);  
  count2=time_afterTTS_Scintillation->Integral(  time_afterTTS_Scintillation->FindBin(0.)  ,  time_afterTTS_Scintillation->FindBin(33.) -1 );
  count2_norm=time_afterTTS_Scintillation_full->Integral(  time_afterTTS_Scintillation_full->FindBin(0.)  ,  time_afterTTS_Scintillation_full->FindBin(1000.));
  
  int count3=0;
  int count4=0;
  count3=time_afterTTS_Cerenkov->Integral(  time_afterTTS_Cerenkov->FindBin(0.)  ,  time_afterTTS_Cerenkov->FindBin(34.) -1 );   
  count4=time_afterTTS_Scintillation->Integral(  time_afterTTS_Scintillation->FindBin(0.)  ,  time_afterTTS_Scintillation->FindBin(34.) -1 );

  cout << "Cerenkov integral [0.,33.] = " << count1 << ", Cerenkov integral [0.,1000.] = " << count1_norm << ", Cerenkov integral [0.,34.] = " << count3 << endl; 

  cout << "Scintillation integral [0.,33.] = " << count2 << ", Scintillation integral [0.,1000.] = " << count2_norm << ", Scintillation integral [0.,34.] = " << count4 << endl;


  TNtuple *tn_count = new TNtuple("tn_count","","int_cer_33:int_cer_34:int_cer_all:int_scint_33:int_scint_34:int_scint_all");
  tn_count->Fill(count1,count3,count1_norm,count2,count4,count2_norm);

  //output file
  TFile *tf = new TFile(Form("%s/PuplicationPlots.root",out.c_str()),"RECREATE");

  //make histograms nicer
  format_h(time_afterTTS_Cerenkov,1,2,1);
  format_h(time_afterTTS_Cerenkov_full,1,2,1);
  //format_h(time_afterTTS_Scintillation,14,2,1); //grey
  format_h(time_afterTTS_Scintillation,2,2,2); // red, dashed
  //format_h(time_afterTTS_Scintillation_full,14,2,1); //grey
  format_h(time_afterTTS_Scintillation_full,2,2,2); //red, dashed
  format_h(cos_psi_33_h,1,2,1);
  //format_h(cos_psi_34_h,14,2,1); //grey
  format_h(cos_psi_34_h,1,2,1);   

 
  //write Trees and histograms to file
  //WL_Tree->Write();
  //Hits_Tree->Write();
  //Events_Tree->Write();
  time_afterTTS_Cerenkov->Write();
  time_afterTTS_Cerenkov_full->Write();
  time_afterTTS_Scintillation->Write();
  time_afterTTS_Scintillation_full->Write();
  cos_psi_33_h->Write();
  cos_psi_34_h->Write();
  tn_count->Write();

  //create a few selected plots
  //plot1
  TCanvas* c1 = new TCanvas("c1", "Timing", width_pixels, height_pixels); 
  //time_afterTTS_Scintillation->SetFillColor(2);
  //time_afterTTS_Scintillation->SetFillStyle(3004);
  time_afterTTS_Scintillation->Draw();
  time_afterTTS_Cerenkov->Draw("same");
  gPad->Update();
  Double_t yrange = gPad->GetFrame()->GetY2();
  TLine *tl1 = new TLine(34.0,0.0,34.0,yrange);
  tl1->SetLineStyle(5);
  tl1->SetLineWidth(2);
  tl1->SetLineColor(1);
  tl1->Draw("same");
  jStyle->SetFrameLineWidth(3);  
  //theta_phi->SetMarkerColor(2);
  //theta_phi->SetMarkerStyle(7);
  time_afterTTS_Scintillation->GetXaxis() -> CenterTitle(true);
  time_afterTTS_Scintillation->GetYaxis() -> CenterTitle(true);
  time_afterTTS_Scintillation->GetXaxis()->SetTitleSize(0.08);
  //time_afterTTS_Scintillation->GetYaxis()->SetTitleSize(0.08);
  //time_afterTTS_Cerenkov->GetXaxis() -> CenterTitle(true);
  time_afterTTS_Cerenkov->GetYaxis() -> CenterTitle(true); 
  c1->Update();
  c1->SaveAs(Form("%s/time_afterTTS.png",out.c_str()),"png");
  c1->SaveAs(Form("%s/time_afterTTS.C",out.c_str()),"C");
  c1->SaveAs(Form("%s/time_afterTTS.pdf",out.c_str()),"pdf");

  //plot2
  TCanvas* c2 = new TCanvas("c2", "Cos Psi 33.0 ns time cut", width_pixels, height_pixels); 
  cos_psi_33_h->Draw();
  jStyle->SetFrameLineWidth(3); 
  //theta_phi_1event->SetMarkerColor(2);
  //theta_phi_1event->SetMarkerStyle(20);
  cos_psi_33_h->GetXaxis() -> CenterTitle(true);
  cos_psi_33_h->GetYaxis() -> CenterTitle(true);
  cos_psi_33_h->SetMinimum(0.);
  //cos_psi_33_h->GetXaxis()->SetTitleSize(0.08);
  //cos_psi_33_h->GetYaxis()->SetTitleSize(0.08);
  c2->Update();
  c2->SaveAs(Form("%s/cos_psi_33_h.png",out.c_str()),"png");
  c2->SaveAs(Form("%s/cos_psi_33_h.C",out.c_str()),"C");
  c2->SaveAs(Form("%s/cos_psi_33_h.pdf",out.c_str()),"pdf");

  //plot3
  TCanvas* c3 = new TCanvas("c3", "Cos Psi 34.0 ns time cut", width_pixels, height_pixels);
  cos_psi_34_h->Draw();
  jStyle->SetFrameLineWidth(3); 
  //theta_phi_1event->SetMarkerColor(2);
  //theta_phi_1event->SetMarkerStyle(20);
  cos_psi_34_h->GetXaxis() -> CenterTitle(true);
  cos_psi_34_h->GetYaxis() -> CenterTitle(true);
  cos_psi_34_h->SetMinimum(0.);
  //cos_psi_34_h->GetXaxis()->SetTitleSize(0.08);
  //cos_psi_34_h->GetYaxis()->SetTitleSize(0.08);
  c3->Update();
  c3->SaveAs(Form("%s/cos_psi_34_h.png",out.c_str()),"png");
  c3->SaveAs(Form("%s/cos_psi_34_h.C",out.c_str()),"C");
  c3->SaveAs(Form("%s/cos_psi_34_h.pdf",out.c_str()),"pdf");

  //plot4

  //format_h(cos_psi_34_h,14,2,1); //grey
  format_h(cos_psi_34_h,1,2,1);

  TCanvas* c4 = new TCanvas("c4", "Cos Psi 33.0 and 34.0 ns time cut comparison", width_pixels, height_pixels);
  //cos_psi_34_h->SetLineColor(2);
  //cos_psi_34_h->SetFillColor(2);
  //cos_psi_34_h->SetFillStyle(3004);
  cos_psi_34_h->Draw();
  cos_psi_34_h->SetMinimum(0.);
  cos_psi_33_h->Draw("same");
  jStyle->SetFrameLineWidth(3);
  cos_psi_34_h->GetXaxis() -> CenterTitle(true);
  cos_psi_34_h->GetYaxis() -> CenterTitle(true);
  c4->Update();
  c4->SaveAs(Form("%s/cos_psi_comp_h.png",out.c_str()),"png");
  c4->SaveAs(Form("%s/cos_psi_comp_h.C",out.c_str()),"C");
  c4->SaveAs(Form("%s/cos_psi_comp_h.pdf",out.c_str()),"pdf");




  tf->Close();

 
}
