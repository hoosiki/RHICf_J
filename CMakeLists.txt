#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(eRHICF)

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build eRHICF with Geant4 UI and Vis drivers" ON)
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
#----------------------------------------------------------------------------
# Locate sources and headers for this project
#
include_directories(${PROJECT_SOURCE_DIR}/include 
    ${Geant4_INCLUDE_DIR} ${ROOT_INCLUDE_DIR} ${HEPMC_INCLUDE_DIR} )

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)
add_executable(eRHICF eRHICF.cc ${sources} ${headers})
target_link_libraries(eRHICF ${Geant4_LIBRARIES} ${ROOT_LIBRARIES} ${HEPMC_LIBRARIES} ${HEPMC_FIO_LIBRARIES})

#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build RE06. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
set(eRHICF_SCRIPTS
     vis.mac icons.mac init.mac init_vis.mac run1.mac run2.mac vis1.mac run.png SimuData/py510G_pp.dat hepmc_ascii.in )

foreach(_script ${eRHICF_SCRIPTS})
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
#set(CMAKE_CXX_FLAGS "-g -O3")

#----------------------------------------------------------------------------
# Add program to the project targets
# (this avoids the need of typing the program name after make)
#
#add_custom_target(ex DEPENDS eRHICF)

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS eRHICF DESTINATION bin)

