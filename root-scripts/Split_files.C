#include "Style.hh"

using namespace std;

void Split_files(){

  //adjust this: 
  std::string simulation = "/home/christoph/geant4/MyG4_applications/Sphere1-build/output/24Sep2013_1p4MeV_electrons_default_settings";
  Int_t N_total = 1000;
  Double_t N_early_C_array[1000];
  Double_t N_early_S_array[1000];
  Double_t RCS_array[1000];
  Int_t N_split_files=10;
  std::string output_file_name="Split_Files/1p4MeV_default_simulation";
  Double_t time_cut = 34.0;
  //end of user input

  for(Int_t kk=0;kk<N_total;kk++)
    {
      N_early_C_array[kk] = 0.0;
      N_early_S_array[kk] = 0.0;
      RCS_array[kk] = 0.0;
    }

  //read in the original trees
  TFile* input_file = new TFile(Form("%s/output.root",simulation.c_str()));
  TTree* Hits_Tree_original = (TTree*)input_file->Get("Hits_Tree");
  TTree* Events_Tree_original = (TTree*)input_file->Get("Events_Tree");  

  //create output trees for first output file
  TFile* outfile = new TFile(Form("%s/%s_1.root",simulation.c_str(),output_file_name.c_str()), "RECREATE");
  TTree *Hits_Tree_out = Hits_Tree_original->CloneTree(0);
  TTree *Events_Tree_out = Events_Tree_original->CloneTree(0);
  //output file for NPE
  ofstream NPE_out;
  NPE_out.open(Form("%s/Split_Files/NPE_out.txt",simulation.c_str()));
 

  Int_t N_hits_total = Hits_Tree_original->GetEntries();
  float Hit_Tree_eventID;
  float PE_time_v;
  float process_v;
  float PE_creation_v;
  Hits_Tree_original->SetBranchAddress("eventID",&Hit_Tree_eventID); 
  Hits_Tree_original->SetBranchAddress("PE_time",&PE_time_v);
  Hits_Tree_original->SetBranchAddress("process",&process_v);
  Hits_Tree_original->SetBranchAddress("PE_creation",&PE_creation_v);  


  Int_t jj=0;

  for(Int_t i=0;i<N_total;i++)
    {
    if(i!=0 && i%(N_total/N_split_files) == 0)
      {
      Events_Tree_out->Write();
      Hits_Tree_out->Write();
      delete Events_Tree_out;
      delete Hits_Tree_out;
      delete outfile;
      Int_t file_number=i/(N_total/N_split_files)+1;
      TFile* outfile = new TFile(Form("%s/%s_%d.root",simulation.c_str(),output_file_name.c_str(),file_number), "RECREATE");
      TTree *Events_Tree_out = Events_Tree_original->CloneTree(0);
      TTree *Hits_Tree_out = Hits_Tree_original->CloneTree(0);
      }

    Events_Tree_original->GetEntry(i);   
    Events_Tree_out->Fill(); 
    

    //count the number of early Cerenkov and Scintillation hits here. Needed for error calculation for publication plots.
    //Also make sure Hits_Tree is split correctly 
    Int_t N_early_C = 0;
    Int_t N_early_S = 0;
    while(Int_t(Hit_Tree_eventID)==i && jj<N_hits_total)
      {
        Hits_Tree_original->GetEntry(jj);
        if(Int_t(Hit_Tree_eventID)==i)
          {
            Hits_Tree_out->Fill(); 
            jj+=1;
          }
        if(Int_t(Hit_Tree_eventID)==i && PE_time_v < time_cut && Int_t(PE_creation_v)==1)
          {
            if(Int_t(process_v)==0) 
              { N_early_S+=1;
                //cout << "event# = " << i << ", jj= " << jj << ", process_v = " << process_v << ", PE_time_v=  " << PE_time_v << endl;
              }
            if(Int_t(process_v)==1) N_early_C+=1;  
          }
      }

      N_early_C_array[i]=Double_t(N_early_C);
      N_early_S_array[i]=Double_t(N_early_S);
      RCS_array[i]=N_early_C_array[i]/N_early_S_array[i];

      cout << "Event #= " << i << ", N_early_C= " << N_early_C << ", N_early_S = " << N_early_S << endl;
      Double_t RCS=Double_t(N_early_C)/Double_t(N_early_S);
      NPE_out << N_early_C << " " << N_early_S << " " << RCS <<"\n"; 

    }

    //calculate the mean number of early Cerenkov (and Scintillation) PEs
    Double_t mean_C= 0.;
    Double_t mean_S= 0.;
    Double_t mean_RCS=0;
    Double_t ssd_C = 0.; //sample standard deviation
    Double_t ssd_S = 0.;
    Double_t ssd_RCS = 0.; 
    for (Int_t kk=0;kk<N_total;kk++)
      {
        mean_C+=N_early_C_array[kk];
        mean_S+=N_early_S_array[kk];
        mean_RCS+=RCS_array[kk];
      }
    mean_C=mean_C/Double_t(N_total);
    mean_S=mean_S/Double_t(N_total);
    mean_RCS=mean_RCS/Double_t(N_total);    

    for (Int_t kk=0;kk<N_total;kk++)
      {
        ssd_C+=(N_early_C_array[kk]-mean_C)*(N_early_C_array[kk]-mean_C);
        ssd_S+=(N_early_S_array[kk]-mean_S)*(N_early_S_array[kk]-mean_S);
        ssd_RCS+=(RCS_array[kk]-mean_RCS)*(RCS_array[kk]-mean_RCS);
      }
    ssd_C=sqrt(ssd_C)/sqrt(N_total-1);
    ssd_S=sqrt(ssd_S)/sqrt(N_total-1);
    ssd_RCS=sqrt(ssd_RCS)/sqrt(N_total-1);
    

    cout << "Results: Mean number of Cerenkov PE after time cut: mean_C= " << mean_C << ", Mean number of Scint PE after time cut: mean_S= " << mean_S << endl;
    cout << "Sample standard deviation for early Cherenkov PEs= " << ssd_C << ", Sample standard deviation for early Scint PEs= " << ssd_S << endl;
    cout << "RCS: mean = "<< mean_RCS << ", sample standard deviation = " << ssd_RCS << endl;

    Events_Tree_out->Write();
    Hits_Tree_out->Write();      
    delete Events_Tree_out;
    delete Hits_Tree_out;
    delete outfile;
    NPE_out.close();
}

