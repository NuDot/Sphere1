cp Publication_Plots_template_small_Det.C Publication_Plots_small_Det.C
sed -i "s/SIMULATION_INPUT_STRING/\/home\/christoph\/geant4\/MyG4_applications\/Sphere1-build\/output\/16May2013_smallDet_compare18April\/0p65m_radius/g" Publication_Plots_small_Det.C
root -q -b Publication_Plots_small_Det.C

cp Publication_Plots_template_small_Det.C Publication_Plots_small_Det.C
sed -i "s/SIMULATION_INPUT_STRING/\/home\/christoph\/geant4\/MyG4_applications\/Sphere1-build\/output\/03June_smallDet_bialkali_QD450_emission\/QD450nm_shifted/g" Publication_Plots_small_Det.C
root -q -b Publication_Plots_small_Det.C

cp Publication_Plots_template_small_Det.C Publication_Plots_small_Det.C
sed -i "s/SIMULATION_INPUT_STRING/\/home\/christoph\/geant4\/MyG4_applications\/Sphere1-build\/output\/03June_smallDet_bialkali_QD450_emission\/QD450nm_unshifted/g" Publication_Plots_small_Det.C
root -q -b Publication_Plots_small_Det.C

cp Publication_Plots_template_small_Det.C Publication_Plots_small_Det.C
sed -i "s/SIMULATION_INPUT_STRING/\/home\/christoph\/geant4\/MyG4_applications\/Sphere1-build\/output\/20May2013_smallDet_redQE/g" Publication_Plots_small_Det.C
root -q -b Publication_Plots_small_Det.C

