#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(RHICf)

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
list(APPEND CMAKE_MODULE_PATH ${CMAKEM_MODULE_PATH} ${PROJECT_SOURCE_DIR} $ENV{ADDITIONALCMAKE})
find_package(Pythia8 REQUIRED)
option(WITH_GEANT4_UIVIS "Build RHICf with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
#
include(${Geant4_USE_FILE})

find_package(ROOT REQUIRED)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
find_package(HepMC REQUIRED)

set(HEPMC_INCLUDE_DIR $ENV{HEPMC_INCLUDE_DIR})
set(HEPMC_LIBRARY $ENV{HEPMC_LIBRARY_DIR})

file(GLOB hepmc ${HEPMC_LIBRARY}/libHepMC.so)
#----------------------------------------------------------------------------
# Locate sources and headers for this project
#
include_directories(${PROJECT_SOURCE_DIR}/include 
    ${Geant4_INCLUDE_DIR} ${ROOT_INCLUDE_DIR} ${HEPMC_INCLUDE_DIR} )

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${HEPMC_INCLUDE_DIRS}/*.h ${PROJECT_SOURCE_DIR}/include/*.hh)
add_executable(TLwoW TLwoW.cc ${PYTHIA8_INCLUDE_DIR}/Pythia8 ${PYTHIA8PLUGIN_INCLUDE_DIR} ${sources} ${headers})
target_link_libraries(TLwoW ${Geant4_LIBRARIES} ${hepmc} ${ROOT_LIBRARIES} ${HEPMC_LIBRARIES} ${HEPMC_FIO_LIBRARIES} ${PYTHIA8PLUGIN_INCLUDE_DIR} ${PYTHIA8_LIBRARIES} -ldl)
add_executable(TSwoW TSwoW.cc ${PYTHIA8_INCLUDE_DIR}/Pythia8 ${PYTHIA8PLUGIN_INCLUDE_DIR} ${sources} ${headers})
target_link_libraries(TSwoW ${Geant4_LIBRARIES} ${hepmc} ${ROOT_LIBRARIES} ${HEPMC_LIBRARIES} ${HEPMC_FIO_LIBRARIES} ${PYTHIA8PLUGIN_INCLUDE_DIR} ${PYTHIA8_LIBRARIES} -ldl)
add_executable(TOPwoW TOPwoW.cc ${PYTHIA8_INCLUDE_DIR}/Pythia8 ${PYTHIA8PLUGIN_INCLUDE_DIR} ${sources} ${headers})
target_link_libraries(TOPwoW ${Geant4_LIBRARIES} ${hepmc} ${ROOT_LIBRARIES} ${HEPMC_LIBRARIES} ${HEPMC_FIO_LIBRARIES} ${PYTHIA8PLUGIN_INCLUDE_DIR} ${PYTHIA8_LIBRARIES} -ldl)

add_executable(eRHICF eRHICF.cc ${PYTHIA8_INCLUDE_DIR}/Pythia8 ${PYTHIA8PLUGIN_INCLUDE_DIR} ${sources} ${headers})
target_link_libraries(eRHICF ${Geant4_LIBRARIES} ${hepmc} ${ROOT_LIBRARIES} ${HEPMC_LIBRARIES} ${HEPMC_FIO_LIBRARIES} ${PYTHIA8PLUGIN_INCLUDE_DIR} ${PYTHIA8_LIBRARIES} -ldl)
#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build RE06. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
set(RHICf_SCRIPTS
     vis.mac icons.mac init.mac init_vis.mac run1.mac run2.mac vis1.mac run.png)

 foreach(_script ${RHICf_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()

#----------------------------------------------------------------------------
# Compiler & Option setting for optimization
# 
#
#set(CMAKE_C_COMPILER "/home/hoosiki/Packages/gcc/bin/gcc")
#set(CMAKE_CXX_COMPILER "/home/hoosiki/Packages/gcc/bin/g++")
set(CMAKE_CXX_FLAGS "-std=c++11")
#set(CMAKE_CXX_FLAGS "-g -ggdb -O3")

#----------------------------------------------------------------------------
# Add program to the project targets
# (this avoids the need of typing the program name after make)
#
#add_custom_target(ex DEPENDS RHICf)

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
#install(TARGETS RHICf DESTINATION bin)

