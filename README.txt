README.txt

3 May 2016, Andrey Elagin
I'm freezing this version and moving to Sphere2.
The updated Sphere1 code is relevant for any figure generated up-to April 2016.


29 June 2013, Christoph Aberle, UCLA

Purpose of the Sphere1 simulation:
---------------------------------- 
The Sphere1 simulation has a simple, rather generic geometry: A steel sphere is filled with liquid scintillator. The sphere surface acts as the photodetector. The purpose of this simulation is to study the main contributions to the timing of the photon hits. The results are used to set priorities for a demonstrator experiment to study the feasibility of direction reconstruction at the MeV scale in liquid scintillator detectors. See arXiv:1307.5813 for detailed information. 

Installation instructions: 
--------------------------
To help to find the hard-coded values and relevant pieces in the code which need to be changed frequently I marked the code with "?" characters. This helps to make sure to not forget to adjust important properties. Additionally, absolute paths and settings have to be adjusted before the following steps are executed: 
0) source /u/nobackup/lwinslow/apps/geant4/geant4.9.6.p02-install/bin/geant4.sh
1) cd /u/nobackup/lwinslow/apps/G4_applications/Sphere1/Sphere1-build
2) cmake -DGeant4_DIR=/u/nobackup/lwinslow/apps/geant4/geant4.9.6.p02-install/lib64/Geant4-9.6.2 /u/nobackup/lwinslow/apps/G4_applications/Sphere1/Sphere1-source
3) make
4) Run the simulation with "./sphere1" (interactively, slow) or "./sphere1 run1.mac", resulting text files will be put into /u/nobackup/lwinslow/apps/G4_applications/Sphere1/Sphere1-build/output

As a second step, ROOT output files are created and TTrees and histograms are filled:
5) module load cern_root
6) cd /u/nobackup/lwinslow/apps/G4_applications/Sphere1/
7) root -q -b Histograms.C (after changing time cut and absolute paths). 

The results in /u/nobackup/lwinslow/apps/G4_applications/Sphere1/Sphere1-build/output include output.root which contains histograms and trees. The most important information is in Hits_Tree whose entries correspond to photons which hit the sphere (=detector in our simulation). The output.root files can be used as input for the reconstruction software in /u/nobackup/lwinslow/apps/G4_applications/WC_code.  
