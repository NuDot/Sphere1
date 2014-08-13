{

  //TTree *WL_Tree = new TTree("WL_Tree", "WL_Tree");
  //TTree *Hits_Tree = new TTree("Hits_Tree", "Hits_Tree");
  TTree *Events_Tree = new TTree("Events_Tree","Events_Tree");

  //WL_Tree->ReadFile("/home/christoph/geant4/MyG4_applications/Sphere1-build/output/wavelengths.txt", "wavelength");
  //Hits_Tree->ReadFile("/home/christoph/geant4/MyG4_applications/Sphere1-build/output/photon_hits.txt","x_hit:y_hit:z_hit:phi:wavelength:true_time:PE_creation:PE_time:detection_coverage_included:true_time_corrected:PE_time_corrected:phi_reco");
  Events_Tree->ReadFile("/home/christoph/geant4/MyG4_applications/Sphere1-build/output/events.txt","eventID:Nprimaries:pos_x:pos_y:pos_z:fake_x:fake_y:fake_z:pdg1:Ekin1:dir_x1:dir_y1:dir_z1:pdg2:Ekin2:dir_x2:dir_y2:dir_z2:Npe:forward_hits:backward_hits");

}
