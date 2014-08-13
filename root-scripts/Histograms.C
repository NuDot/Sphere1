#include "Style.hh"

using namespace std;

void format_h(TH1F* h, int lc, int lw){
  h->SetLineColor(lc);
  h->SetLineWidth(lw);
  //h->GetXaxis()->CenterTitle();
  //h->GetYaxis()->CenterTitle();
}


void Histograms(){

  //-------------------------------------------------------------------------------------------------------
  //use the style defined by Julia
  TStyle* jStyle = new TStyle("jStyle", "jStyle");
  Style(jStyle,1);

  //-------------------------------------------------------------------------------------------------------
  //user input
  //?
  //Double_t time_cut = 3.265; //0.65m diameter detector
  Double_t time_cut = 34.0;  //6.5m diameter detector
  std::string out = "/home/christoph/geant4/MyG4_applications/Sphere1-build/output/Plots";
  Int_t width_pixels= 700;  //for automatic output graphs
  Int_t height_pixels=500;  // for automatic output graphs
  std::string coverage_string = "off"; 
  Int_t coverage = 0;

  //-------------------------------------------------------------------------------------------------------
  //define the Trees and fill them from the output files of the simulation
  //TTree *WL_Tree = new TTree("WL_Tree", "WL_Tree");
  TTree *Hits_Tree = new TTree("Hits_Tree", "Hits_Tree");      // Photon hits on sensitive elements
  TTree *Events_Tree = new TTree("Events_Tree","Events_Tree"); // From file with event information
  //WL_Tree->ReadFile("/home/christoph/geant4/MyG4_applications/Sphere1-build/output/wavelengths.txt", "wavelength");
  Hits_Tree->ReadFile("/home/christoph/geant4/MyG4_applications/Sphere1-build/output/photon_hits.txt","x_hit:y_hit:z_hit:cos_theta:photon_wavelength:true_time:PE_creation:PE_time:detection_coverage_included:true_time_corrected:PE_time_corrected:cos_theta_reco:theta_reco:phi_reco:process:eventID:N_Rayleigh_Scatters:x_pos_pre_scat");
  Events_Tree->ReadFile("/home/christoph/geant4/MyG4_applications/Sphere1-build/output/events.txt","eventID_ET:Nprimaries:pos_x:pos_y:pos_z:fake_x:fake_y:fake_z:pdg1:Ekin1:dir_x1:dir_y1:dir_z1:pdg2:Ekin2:dir_x2:dir_y2:dir_z2:Npe:forward_hits:backward_hits");

  //-------------------------------------------------------------------------------------------------------
  //define the histograms which are output by default
  TH1F * wl_created = new TH1F("wl_created","Photons created originally (before propagation);wavelength [nm];entries per bin",175,100.,800.); //1
  TH1F * wl_hit = new TH1F("wl_hit","Photons which arrive at the sphere;wavelength [nm];entries per bin",175,100.,800.); //2  
  TH1F * cos_theta_detected = new TH1F("cos_theta_detected","True cos(#Theta), coverage and QE applied ;cos(#Theta);entries per bin",100,-1.,1.); //4
  TH1F * wl_hit_detected = new TH1F("wl_hit_detected","Photon hits which create photoelectrons (coverage and QE applied);wavelength [nm];entries per bin",175,100.,800.); //3
  TH1F * time_hit_detected = new TH1F("time_hit_detected","True time of photon hits which create photoelectrons (coverage and QE applied);true time [ns];entries per bin",3000,0.,300.); 
  TH1F * cos_theta_detected_cut = new TH1F("cos_theta_detected_cut","True cos(#Theta) of photon hits (coverage and QE and time cut applied);true cos(#Theta);entries per bin",100,-1.,1.); //6
  TH1F * time_afterTTS = new TH1F("time_afterTTS","Hit time after TTS has been applied (coverage and QE applied);time [ns];entries per bin",3000,0.,300.);  //5 
  TH1F * forward_hits_h = new TH1F("forward_hits_h","Number of detected forward hits (after time cut); Nhits; entries",300,0.,300.);
  TH1F * backward_hits_h = new TH1F("backward_hits_h","Number of detected backward hits (after time cut); Nhits; entries",300,0.,300.);
  TH1F * forward_over_backward_h = new TH1F("forward_over_backward_h","Ratio forward/backward hits (after reconstructed time cut);N_{forward}/N_{backward};entries",100,0.,3.); 
  TH1F * time_afterTTS_fakeVertex = new TH1F("time_afterTTS_fakeVertex","Time after correction (fake recoed vertex used) and after TTS applied, QE and coverage applied;time [ns];entries per bin",3000,0.,300);
  TH1F * time_afterTTS_fakeVertex_Cerenkov = new TH1F("time_afterTTS_fakeVertex_Cerenkov","Cerenkov only: Time after correction (fake recoed vertex used) and after TTS applied, QE applied, coverage not applied;time [ns];entries per bin",600,0.,60.);
  TH1F * time_afterTTS_fakeVertex_Scintillation = new TH1F("time_afterTTS_fakeVertex_Scintillation","Scintillation only: Time after correction (fake recoed vertex used) and after TTS applied, QE applied, coverage not applied;time [ns];entries per bin",600,0.,60.);
  TH1F * cos_theta_reco_detected = new TH1F("cos_theta_reco_detected","cos(#Theta) with reco vertex, QE and coverage applied;cos(#Theta);entries per bin",100,-1.,1.);
  TH1F * cos_theta_reco_detected_cut = new TH1F("cos_theta_reco_detected_cut","cos(#Theta) with reco vertex, QE and coverage applied, time cut applied;cos(#Theta);entries per bin",100,-1.,1.);  
  TH2F * theta_phi = new TH2F("theta_phi","all events: cos(#Theta) vs. #Phi (using reco vertex), QE applied, coverage not applied, time cut applied;#Phi;cos(#Theta);",1000,-TMath::Pi(),TMath::Pi(),1000,-1.0,1.0);
  TH2F * theta_phi_1event = new TH2F("theta_phi_1event","1 event: cos(#Theta) vs. #Phi (using reco vertex), QE applied, coverage not applied, time cut applied;#Phi;cos(#Theta);",1000,-TMath::Pi(),TMath::Pi(),1000,-1.0,1.0);
  TH2F * theta_phi_ideal = new TH2F("theta_phi_ideal","cos(#Theta) vs. #Phi (using reco vertex), QE not applied, coverage not applied, time cut applied;#Phi;cos(#Theta);",1000,-TMath::Pi(),TMath::Pi(),1000,-1.0,1.0);
  TH1F * cos_psi_h = new TH1F("cos_psi_h","cos(#Psi): Angle between original particle direction and direction from reco vertex to average hits, QE applied, coverage not applied, time cut applied ;cos(#Psi);entries",100,-1.,1.);
  //WL_Tree->Draw("photon_wavelength>>wl_created");


  //-------------------------------------------------------------------------------------------------------
  //fill histograms     
  Hits_Tree->Draw("photon_wavelength>>wl_hit");    
  Hits_Tree->Draw("cos_theta>>cos_theta_detected","detection_coverage_included==1 && PE_creation==1");
  Hits_Tree->Draw("cos_theta_reco>>cos_theta_reco_detected","detection_coverage_included==1 && PE_creation==1");
  Hits_Tree->Draw("photon_wavelength>>wl_hit_detected","detection_coverage_included==1 && PE_creation==1");
  Hits_Tree->Draw("true_time>>time_hit_detected","detection_coverage_included==1 && PE_creation==1");
  Hits_Tree->Draw("cos_theta>>cos_theta_detected_cut",Form("detection_coverage_included==1 && PE_creation==1 && PE_time_corrected < %f",time_cut));
  Hits_Tree->Draw("cos_theta_reco>>cos_theta_reco_detected_cut",Form("detection_coverage_included==1 && PE_creation==1 && PE_time_corrected < %f",time_cut));
  Hits_Tree->Draw("PE_time>>time_afterTTS","detection_coverage_included==1 && PE_creation==1");
  Hits_Tree->Draw("PE_time_corrected>>time_afterTTS_fakeVertex","detection_coverage_included==1 && PE_creation==1");
  Hits_Tree->Draw("PE_time_corrected>>time_afterTTS_fakeVertex_Cerenkov","PE_creation==1 && process==1");
  Hits_Tree->Draw("PE_time_corrected>>time_afterTTS_fakeVertex_Scintillation","PE_creation==1 && process==0");
  Events_Tree->Draw("forward_hits>>forward_hits_h");
  Events_Tree->Draw("backward_hits>>backward_hits_h");
  Events_Tree->Draw("forward_hits/backward_hits>>forward_over_backward_h");
  Hits_Tree->Draw("cos(theta_reco):phi_reco>>theta_phi",Form("PE_creation==1 && PE_time_corrected < %f",time_cut),"");
  Hits_Tree->Draw("cos(theta_reco):phi_reco>>theta_phi_1event",Form("eventID==0 && PE_creation==1 && PE_time_corrected < %f",time_cut),"");
  Hits_Tree->Draw("cos(theta_reco):phi_reco>>theta_phi_ideal",Form("PE_time_corrected < %f",time_cut),"");



  //-------------------------------------------------------------------------------------------------------
  // calculate some integrals, this is deprecated, if I need it again I would count the numbers at simulation runtime  
  // ?deprecated 
  int count1=0;
  int count2=0;
  count1=wl_created->Integral(  wl_created->FindBin(360.)  ,  wl_created->FindBin(550.) -1 );
  count2=wl_created->Integral(  wl_created->FindBin(400.)  ,  wl_created->FindBin(550.) -1 );
  
  int count3=0;
  int count4=0;
  count3=wl_hit->Integral(  wl_hit->FindBin(360.)  ,  wl_hit->FindBin(550.) -1 );
  count4=wl_hit->Integral(  wl_hit->FindBin(400.)  ,  wl_hit->FindBin(550.) -1 );

  int count1_detected=0;
  int count2_detected=0;
  count1_detected=wl_hit_detected->Integral(  wl_hit_detected->FindBin(360.)  ,  wl_hit_detected->FindBin(550.) -1 );
  count2_detected=wl_hit_detected->Integral(  wl_hit_detected->FindBin(400.)  ,  wl_hit_detected->FindBin(550.) -1 );

  //-------------------------------------------------------------------------------------------------------
  //calculate the number of photons which are detected in forward direction and backward direction, respectively (after time cut)
  int count_forward=0;
  int count_backward=0;
  count_backward=cos_theta_detected_cut->Integral( cos_theta_detected_cut->FindBin(-0.99999999),cos_theta_detected_cut->FindBin(-0.00000001));
  count_forward=cos_theta_detected_cut->Integral( cos_theta_detected_cut->FindBin(0.00000001),cos_theta_detected_cut->FindBin(0.99999999)); 
  TNtuple *tn1 = new TNtuple("tn1","","nphot_created_360_550:nphot_created_400_550:nphot_hit_360_550:nphot_hit_400_550:nphot_detected_360_550:nphot_detected_400_550:count_forward:count_backward");
  tn1->Fill(count1,count2,count3,count4,count1_detected,count2_detected,count_forward,count_backward);


  //-----------------------------------------------------------------------------------------------------------------------
  //analysis: calculate the mean hit position to get an estimate of the direction
  //step1: define variables and branches for the TTrees
  Float_t x_hit_v = 0.0;
  Float_t y_hit_v = 0.0;
  Float_t z_hit_v = 0.0;
  Float_t cos_theta_v = 0.0;
  Float_t photon_wavelength_v = 0.0;
  Float_t true_time_v = 0.0;
  Float_t PE_creation_v = 0.0;
  Float_t PE_time_v = 0.0;
  Float_t detection_coverage_included_v = 0.0;
  Float_t true_time_corrected_v = 0.0;
  Float_t PE_time_corrected_v = 0.0;
  Float_t cos_theta_reco_v = 0.0;
  Float_t theta_reco_v = 0.0; 
  Float_t phi_reco_v = 0.0;
  Float_t process_v = 0.0;
  Float_t eventID_v = 0.0;

  TBranch  *x_hit_b = 0;
  TBranch  *y_hit_b = 0;
  TBranch  *z_hit_b = 0;
  TBranch  *cos_theta_b = 0;
  TBranch  *photon_wavelength_b = 0;
  TBranch  *true_time_b = 0;
  TBranch  *PE_creation_b = 0;
  TBranch  *PE_time_b = 0;
  TBranch  *detection_coverage_included_b = 0;
  TBranch  *true_time_corrected_b = 0;
  TBranch  *PE_time_corrected_b = 0;
  TBranch  *cos_theta_reco_b = 0;
  TBranch  *theta_reco_b = 0;
  TBranch  *phi_reco_b = 0;
  TBranch  *process_b = 0;
  TBranch  *eventID_b = 0;

  Hits_Tree->SetBranchAddress("x_hit", &x_hit_v, &x_hit_b);
  Hits_Tree->SetBranchAddress("y_hit", &y_hit_v, &y_hit_b);
  Hits_Tree->SetBranchAddress("z_hit", &z_hit_v, &z_hit_b);  
  Hits_Tree->SetBranchAddress("cos_theta", &cos_theta_v, &cos_theta_b);
  Hits_Tree->SetBranchAddress("photon_wavelength", &photon_wavelength_v, &photon_wavelength_b);
  Hits_Tree->SetBranchAddress("true_time", &true_time_v, &true_time_b);
  Hits_Tree->SetBranchAddress("PE_creation", &PE_creation_v, &PE_creation_b);
  Hits_Tree->SetBranchAddress("PE_time", &PE_time_v, &PE_time_b);
  Hits_Tree->SetBranchAddress("detection_coverage_included", &detection_coverage_included_v, &detection_coverage_included_b);
  Hits_Tree->SetBranchAddress("true_time_corrected", &true_time_corrected_v, &true_time_corrected_b);
  Hits_Tree->SetBranchAddress("PE_time_corrected", &PE_time_corrected_v, &PE_time_corrected_b);
  Hits_Tree->SetBranchAddress("cos_theta_reco", &cos_theta_reco_v, &cos_theta_reco_b);
  Hits_Tree->SetBranchAddress("theta_reco", &theta_reco_v, &theta_reco_b);
  Hits_Tree->SetBranchAddress("phi_reco", &phi_reco_v, &phi_reco_b);
  Hits_Tree->SetBranchAddress("process", &process_v, &process_b);
  Hits_Tree->SetBranchAddress("eventID", &eventID_v, &eventID_b);
 
  //Event Tree
  Float_t eventID_ET_v = 0.0;
  Float_t Nprimaries_v = 0.0;
  Float_t pos_x_v = 0.0;
  Float_t pos_y_v = 0.0;
  Float_t pos_z_v = 0.0;
  Float_t fake_x_v = 0.0;
  Float_t fake_y_v = 0.0;
  Float_t fake_z_v = 0.0;
  Float_t pdg1_v = 0.0;
  Float_t Ekin1_v = 0.0;
  Float_t dir_x1_v = 0.0;
  Float_t dir_y1_v = 0.0;
  Float_t dir_z1_v = 0.0;
  Float_t pdg2_v = 0.0;
  Float_t Ekin2_v = 0.0;
  Float_t dir_x2_v = 0.0;
  Float_t dir_y2_v = 0.0;
  Float_t dir_z2_v = 0.0;
  Float_t Npe_v = 0.0;
  Float_t forward_hits_v = 0.0;
  Float_t backward_hits_v = 0.0;

  TBranch *eventID_ET_b = 0;
  TBranch *Nprimaries_b = 0;
  TBranch *pos_x_b = 0;
  TBranch *pos_y_b = 0;
  TBranch *pos_z_b = 0;
  TBranch *fake_x_b = 0;
  TBranch *fake_y_b = 0;
  TBranch *fake_z_b = 0;
  TBranch *pdg1_b = 0;
  TBranch *Ekin1_b = 0;
  TBranch *dir_x1_b = 0;
  TBranch *dir_y1_b = 0;
  TBranch *dir_z1_b = 0;
  TBranch *pdg2_b = 0;
  TBranch *Ekin2_b = 0;
  TBranch *dir_x2_b = 0;
  TBranch *dir_y2_b = 0;
  TBranch *dir_z2_b = 0;
  TBranch *Npe_b = 0;
  TBranch *forward_hits_b = 0;
  TBranch *backward_hits_b = 0;

  Events_Tree->SetBranchAddress("eventID_ET", &eventID_ET_v, &eventID_ET_b);
  Events_Tree->SetBranchAddress("Nprimaries", &Nprimaries_v, &Nprimaries_b);
  Events_Tree->SetBranchAddress("pos_x", &pos_x_v, &pos_x_b);
  Events_Tree->SetBranchAddress("pos_y", &pos_y_v, &pos_y_b);
  Events_Tree->SetBranchAddress("pos_z", &pos_z_v, &pos_z_b);
  Events_Tree->SetBranchAddress("fake_x", &fake_x_v, &fake_x_b);
  Events_Tree->SetBranchAddress("fake_y", &fake_y_v, &fake_y_b);
  Events_Tree->SetBranchAddress("fake_z", &fake_z_v, &fake_z_b);
  Events_Tree->SetBranchAddress("pdg1", &pdg1_v, &pdg1_b);
  Events_Tree->SetBranchAddress("Ekin1", &Ekin1_v, &Ekin1_b);
  Events_Tree->SetBranchAddress("dir_x1", &dir_x1_v, &dir_x1_b);
  Events_Tree->SetBranchAddress("dir_y1", &dir_y1_v, &dir_y1_b);
  Events_Tree->SetBranchAddress("dir_z1", &dir_z1_v, &dir_z1_b);
  Events_Tree->SetBranchAddress("pdg2", &pdg2_v, &pdg2_b);
  Events_Tree->SetBranchAddress("Ekin2", &Ekin2_v, &Ekin2_b);
  Events_Tree->SetBranchAddress("dir_x2", &dir_x2_v, &dir_x2_b);
  Events_Tree->SetBranchAddress("dir_y2", &dir_y2_v, &dir_y2_b);
  Events_Tree->SetBranchAddress("dir_z2", &dir_z2_v, &dir_z2_b);
  Events_Tree->SetBranchAddress("Npe", &Npe_v, &Npe_b);
  Events_Tree->SetBranchAddress("forward_hits", &forward_hits_v, &forward_hits_b);
  Events_Tree->SetBranchAddress("backward_hits", &backward_hits_v, &backward_hits_b);


  //-----------------------------------------------------------------------------------------------------------------------
  //step2: Loop over Hits and calculate mean hit position for hits which create PEs and fulfill the time cut. 
  Long64_t nentries = Hits_Tree->GetEntries(); 
  cout << "nentries = " << nentries << endl; 

  Float_t mean_x_hit=0.0;
  Float_t mean_y_hit=0.0;
  Float_t mean_z_hit=0.0;
  Int_t current_event=0.0;
  Int_t hit_count=0;
  Double_t cos_psi=0.0;
  Int_t N_no_hits=0;

  for (Long64_t i=0;i<nentries;i++) 
    {
      Hits_Tree->GetEntry(i);
      //y_hit_b->GetEntry(i);
      //z_hit_b->GetEntry(i);
      //cout << "i= " << i << ", x_hit_v= " << x_hit_v << ", y_hit_v=" << y_hit_v << ", z_hit_v=" << z_hit_v << endl; 

      if(current_event!=eventID_v)
        {
          if(hit_count>0)
            {
              mean_x_hit=mean_x_hit/hit_count;
              mean_y_hit=mean_y_hit/hit_count;
              mean_z_hit=mean_z_hit/hit_count;

              //calculation of cos(angle) between reconstructed direction and real direction (1,0,0)=x
              //cos_psi = mean_x_hit/sqrt(mean_x_hit*mean_x_hit + mean_y_hit*mean_y_hit + mean_z_hit*mean_z_hit); //This is if we knew the vertex (0,0,0) 
              Events_Tree->GetEntry(current_event);
              //hardcoded direction of original particle: x-direction!! 
              //?
              cos_psi = (mean_x_hit-fake_x_v)/std::sqrt((mean_x_hit-fake_x_v)*(mean_x_hit-fake_x_v)+(mean_y_hit-fake_y_v)*(mean_y_hit-fake_y_v)+(mean_z_hit-fake_z_v)*(mean_z_hit-fake_z_v));           

              cos_psi_h->Fill(cos_psi); 
              cout << "current_event= " << current_event << ", hit_count= " << hit_count << ", mean_x_hit= " << mean_x_hit << ", mean_y_hit= " << mean_y_hit << ", mean_z_hit= " << mean_z_hit << ", fake_x_v= " << fake_x_v << ", fake_y_v= " << fake_y_v << ", fake_z_v= " << fake_z_v << endl;

              mean_x_hit=0.0;
              mean_y_hit=0.0;
              mean_z_hit=0.0;
              hit_count=0;
            }
          else
            {
              cout << "Event " << current_event << " has no hits which make the time cut. Nothing will be filled into cos_psi_h." << endl; 
              N_no_hits+=1;
            }
      }

      if(PE_creation_v == 1 && PE_time_corrected_v < time_cut)
        {
          mean_x_hit+=x_hit_v;
          mean_y_hit+=y_hit_v;
          mean_z_hit+=z_hit_v;
          hit_count+=1;
        }
      current_event=eventID_v;
       
    }
  //get the cos_psi value for the last event
  if(hit_count>0)
    {
      mean_x_hit=mean_x_hit/hit_count;
      mean_y_hit=mean_y_hit/hit_count;
      mean_z_hit=mean_z_hit/hit_count;
      //calculation of cos(angle) between reconstructed direction and real direction (1,0,0):
      Events_Tree->GetEntry(current_event);
      cos_psi = (mean_x_hit-fake_x_v)/std::sqrt((mean_x_hit-fake_x_v)*(mean_x_hit-fake_x_v)+(mean_y_hit-fake_y_v)*(mean_y_hit-fake_y_v)+(mean_z_hit-fake_z_v)*(mean_z_hit-fake_z_v));
      cos_psi_h->Fill(cos_psi);
      cout << "current_event= " << current_event << ", hit_count= " << hit_count << ", mean_x_hit= " << mean_x_hit << ", mean_y_hit= " << mean_y_hit << ", mean_z_hit= " << mean_z_hit << ", fake_x_v= " << fake_x_v << ", fake_y_v= " << fake_y_v << ", fake_z_v= " << fake_z_v << endl;
    }
  else
    {
      cout << "Event " << current_event << " has no hits which make the time cut. Nothing will be filled into cos_psi_h." << endl; 
      N_no_hits+=1;
    }	


  cout << "The number of events without hits that pass the time_cut of " << time_cut << " ns is: " << N_no_hits << "." << endl;
  //-----------------------------------------------------------------------------------------------------------------------
  //
  TFile *tf = new TFile("/home/christoph/geant4/MyG4_applications/Sphere1-build/output/output.root","NEW");

  //make histograms nicer
  format_h(wl_hit,1,2);
  format_h(wl_hit_detected,1,2);
  format_h(cos_theta_detected,1,2);
  format_h(cos_theta_reco_detected,1,2);
  format_h(time_hit_detected,1,2);
  format_h(cos_theta_detected_cut,1,2);
  format_h(time_afterTTS,1,2);
  format_h(time_afterTTS_fakeVertex,1,2);
  format_h(time_afterTTS_fakeVertex_Cerenkov,1,2);
  format_h(time_afterTTS_fakeVertex_Scintillation,1,2);
  format_h(forward_hits_h,1,2);
  format_h(backward_hits_h,1,2);
  format_h(forward_over_backward_h,1,2);
  format_h(cos_psi_h,1,2);
 
  //write Trees and histograms to file
  //WL_Tree->Write();
  Hits_Tree->Write();
  Events_Tree->Write();
  tn1->Write();
  wl_created->Write();
  wl_hit->Write();
  wl_hit_detected->Write();
  cos_theta_detected->Write();
  cos_theta_reco_detected->Write();
  time_hit_detected->Write();
  cos_theta_detected_cut->Write();
  time_afterTTS->Write();
  time_afterTTS_fakeVertex->Write();
  time_afterTTS_fakeVertex_Cerenkov->Write();
  time_afterTTS_fakeVertex_Scintillation->Write();
  forward_hits_h->Write();
  backward_hits_h->Write();
  forward_over_backward_h->Write();
  theta_phi->Write();
  theta_phi_1event->Write();
  theta_phi_ideal->Write();
  cos_psi_h->Write();

  //create a few selected plots
  //plot1
  TCanvas* c1 = new TCanvas("c1", "source energy", width_pixels, height_pixels); 
  theta_phi->Draw("goff");
  jStyle->SetFrameLineWidth(2);
  theta_phi->SetMarkerColor(2);
  //theta_phi->SetMarkerStyle(7);
  c1->Update();
  c1->SaveAs(Form("%s/theta_phi.png",out.c_str()),"png");
  c1->SaveAs(Form("%s/theta_phi.C",out.c_str()),"C");

  //plot2
  theta_phi_1event->Draw("goff");
  jStyle->SetFrameLineWidth(2); 
  theta_phi_1event->SetMarkerColor(2);
  theta_phi_1event->SetMarkerStyle(20);
  c1->Update();
  c1->SaveAs(Form("%s/theta_phi_1event.png",out.c_str()),"png");
  c1->SaveAs(Form("%s/theta_phi_1event.C",out.c_str()),"C");
 
  //plot3
  cos_psi_h->Draw();
  jStyle->SetFrameLineWidth(2);
  cos_psi_h->SetLineColor(2); 
  c1->Update();
  c1->SaveAs(Form("%s/cos_psi_h.png",out.c_str()),"png");
  c1->SaveAs(Form("%s/cos_psi_h.C",out.c_str()),"C");  

  //plot4
  time_afterTTS_fakeVertex_Scintillation->SetLineColor(2);
  time_afterTTS_fakeVertex_Scintillation->Draw();
  time_afterTTS_fakeVertex_Cerenkov->SetLineColor(4);
  time_afterTTS_fakeVertex_Cerenkov->Draw("same");
  TLine * tl1 = new TLine(3.265,0.0,3.265,10000000.);
  tl1->SetLineWidth(2);
  tl1->SetLineStyle(2);
  //tl1->Draw("same");
  c1->Update();
  c1->SaveAs(Form("%s/time_afterTTS_fakeVertex.png",out.c_str()),"png");
  c1->SaveAs(Form("%s/time_afterTTS_fakeVertex.C",out.c_str()),"C");  


  tf->Close();


  
}
