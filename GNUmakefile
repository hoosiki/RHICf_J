name := example
G4TARGET := $(name)
G4EXLIB := true
G4WORKDIR := ./

SOFLAGS     += -shared
ROOTLIBS    := $(shell root-config --libs)
ROOTCFLAGS  := $(shell root-config --cflags)
ROOTGLIBS   := $(shell root-config --glibs)
CXXFLAGS     = -m64 -O -Wall -fPIC
CXXFLAGS    += $(ROOTCFLAGS)
CPPFLAGS     = -m64 -O -Wall -fPIC
CPPFLAGS    += $(ROOTCFLAGS)

CPPFLAGS    += -DG4ANALYSIS_USE_ROOT
CPPFLAGS    += -D_REENTRANT -I$(ROOTSYS)/include
CPPFLAGS    += $(shell $(ROOTSYS)/bin/root-config --cflags)
CPPFLAGS    += -pthread -I$(ROOTSYS)/include
ROOTLIBS = $(shell $(ROOTSYS)/bin/root-config --glibs)
EXTRALIBS := $(ROOTLIBS)
EXTRALIBS += -L$(ROOTSYS)/lib \
-lCore -lCint \
-lHist -lGraf -lGraf3d -lGpad \
-lTree -lRint -lPostscript \
-lMatrix -lPhysics \
-lm -ldl -lpthread -rdynamic
EXTRALIBS += $(ROOTLIBS)
#ifndef G4INSTALL
#G4INSTALL = ../
#endif

#ifndef G4WORKDIR
#G4WORKDIR = .
#endif



.PHONY: all
all: lib bin
    
include $(G4INSTALL)/config/binmake.gmk
