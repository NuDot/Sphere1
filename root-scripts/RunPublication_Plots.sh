cp Publication_Plots_template.C Publication_Plots.C
sed -i "s/SIMULATION_INPUT_STRING/\/home\/christoph\/geant4\/MyG4_applications\/Sphere1-build\/output\/18April2013_first_reco/g" Publication_Plots.C
root -q -b Publication_Plots.C

cp Publication_Plots_template.C Publication_Plots.C
sed -i "s/SIMULATION_INPUT_STRING/\/home\/christoph\/geant4\/MyG4_applications\/Sphere1-build\/output\/03June_largeDet_bialkali_smaller_energies\/1MeV/g" Publication_Plots.C
root -q -b Publication_Plots.C

cp Publication_Plots_template.C Publication_Plots.C
sed -i "s/SIMULATION_INPUT_STRING/\/home\/christoph\/geant4\/MyG4_applications\/Sphere1-build\/output\/04June2013_6p5Meter_5MeVElectrons_Bialkali_QD461ScintSpec\/6p5Meter_5MeVElectrons_Bialkali_QD384nmScintSpec/g" Publication_Plots.C
root -q -b Publication_Plots.C

cp Publication_Plots_template.C Publication_Plots.C
sed -i "s/SIMULATION_INPUT_STRING/\/home\/christoph\/geant4\/MyG4_applications\/Sphere1-build\/output\/04June2013_6p5Meter_5MeVElectrons_Bialkali_QD461ScintSpec\/6p5Meter_5MeVElectrons_Bialkali_QD461nmScintSpec/g" Publication_Plots.C
root -q -b Publication_Plots.C

cp Publication_Plots_template.C Publication_Plots.C
sed -i "s/SIMULATION_INPUT_STRING/\/home\/christoph\/geant4\/MyG4_applications\/Sphere1-build\/output\/04June2013_6p5Meter_5MeVElectrons_Bialkali_QD461ScintSpec\/6p5Meter_5MeVElectrons_RedSensitiveQE_KamlandScintSpec/g" Publication_Plots.C
root -q -b Publication_Plots.C

cp Publication_Plots_template.C Publication_Plots.C
sed -i "s/SIMULATION_INPUT_STRING/\/home\/christoph\/geant4\/MyG4_applications\/Sphere1-build\/output\/16June_6p5Meter_5MeVElectrons_Bialkali_KamlandScintSpec_1p28nsTTS/g" Publication_Plots.C
root -q -b Publication_Plots.C

