# - Locate HepMC library
# in a directory defined via  HEPMC_ROOT_DIR or HEPMC_DIR environment variable
# Defines:
#
#  HEPMC_FOUND
#  HEPMC_INCLUDE_DIR
#  HEPMC_INCLUDE_DIRS (not cached)
#  HEPMC_LIBRARIES
#  HEPMC_FIO_LIBRARIES

find_path(HEPMC_INCLUDE_DIR HepMC/GenEvent.h
    HINTS $ENV{HEPMC_ROOT_DIR}/include ${HEPMC_ROOT_DIR}/include
    $ENV{HEPMC_DIR}/include ${HEPMC_DIR}/include)


find_library(HEPMC_LIBRARY
    NAMES HepMC
    HINTS $ENV{HEPMC_ROOT_DIR}/lib ${HEPMC_ROOT_DIR}/lib
    HINTS ${_hepmcdirs} $ENV{HEPMC_LIBRARY_DIR}
    PATH_SUFFIXES lib
    DOC "Specify the HepMC library here.")

set(HEPMC_FIO_LIBRARIES "-L${HEPMC_LIBRARY_DIR} -lHepMCfio")

foreach(_lib HEPMC_LIBRARY )
    if(${_lib})
        set(HEPMC_LIBRARIES ${HEPMC_LIBRARIES} ${${_lib}})
    endif()
endforeach()

set(HEPMC_INCLUDE_DIRS ${HEPMC_INCLUDE_DIR})

# handle the QUIETLY and REQUIRED arguments and set HEPMC_FOUND to TRUE if
# all listed variables are TRUE
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(HepMC DEFAULT_MSG HEPMC_INCLUDE_DIR HEPMC_LIBRARIES)

#mark_as_advanced(HEPMC_FOUND HEPMC_INCLUDE_DIR HEPMC_LIBRARIES)
