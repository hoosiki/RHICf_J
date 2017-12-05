# Find the Pythia8 includes and library.
#
# This module defines
# PYTHIA8_INCLUDE_DIR   where to locate Pythia.h file
# PYTHIA8_LIBRARY       where to find the libpythia8 library
# PYTHIA8_<lib>_LIBRARY Addicional libraries
# PYTHIA8_LIBRARIES     (not cached) the libraries to link against to use Pythia8
# PYTHIA8_FOUND         if false, you cannot build anything that requires Pythia8
# PYTHIA8_VERSION       version of Pythia8 if found

set(_pythia8dirs
    ${PYTHIA8}
    $ENV{PYTHIA8_ROOT_DIR}
    ${PYTHIA8_DIR}
    $ENV{PYTHIA8_LIBARY_DIR}
    /usr
    /opt/pythia8)

find_path(PYTHIA8_INCLUDE_DIR
    NAMES Pythia8/Pythia.h Pythia8Plugins/HepMC2.h
    HINTS ${_pythia8dirs} $ENV{PYHTIA8_INCLUDE_DIR}
    PATH_SUFFIXES include include/Pythia8 include/pythia8 include/Pythia8Plugins
    DOC "Specify the directory containing Pythia.h.")

#find_path(PYTHIA8PLUGIN_INCLUDE_DIR
#NAMES Pythia8Plugins/HepMC2.h
#HINTS ${_pythiaplugin8dirs} $ENV{PYHTIA8_INCLUDE_DIR}
#    PATH_SUFFIXES include include/Pythia8Plugins include/pythia8plugins
#    DOC "Specify the directory containing HepMC2.h.")

find_library(PYTHIA8_LIBRARY
    NAMES pythia8 Pythia8
    HINTS ${_pythia8dirs} $ENV{PYTHIA8_LIBRARY_DIR}
    PATH_SUFFIXES lib
    DOC "Specify the Pythia8 library here.")



foreach(_lib PYTHIA8_LIBRARY )
    if(${_lib})
        set(PYTHIA8_LIBRARIES ${PYTHIA8_LIBRARIES} ${${_lib}})
    endif()
endforeach()
set(PYTHIA8_INCLUDE_DIRS ${PYTHIA8_INCLUDE_DIR} ${PYTHIA8_INCLUDE_DIR}/Pythia8 ${PYTHIA8_INCLUDE_DIR}/Pythia8Plugins)

# handle the QUIETLY and REQUIRED arguments and set PYTHIA8_FOUND to TRUE if
# all listed variables are TRUE

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Pythia8 DEFAULT_MSG  PYTHIA8_INCLUDE_DIR PYTHIA8_LIBRARY)
#mark_as_advanced(PYTHIA8_INCLUDE_DIR PYTHIA8_LIBRARY PYTHIA8_hepmcinterface_LIBRARY PYTHIA8_lhapdfdummy_LIBRARY)