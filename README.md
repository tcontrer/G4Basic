# G4Basic: Teflon study

# To run geant on Odyssey, use these setup commands first (and every new login):
#    setup cmake v3_14_3
#    setup gsl v2_5 -q prof
#    setup geant4 v4_10_6_p01 -q e19:prof
#    setup root v6_18_04 -q e19:prof


# To install:
#    mkdir build
#    cd build
#    cmake ..
#    cmake --build .

# Whenever you edit the code you must rebuild (from the build directory):
#    cmake --build .

# To Run (from build directory):
#    ./app/G4Basic

# Then you can to shoot a single photon:
#     /run/beamOn 1

# To run a macro (here named testrun.mac) that tells how many photons to shoot, where, etc:
#     ./app/G4Basic testrun.mac
