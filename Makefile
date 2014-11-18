# Makefile for the ROOT test programs.
# This Makefile shows nicely how to compile and link applications
# using the ROOT libraries on all supported platforms.
#
# Copyright (c) 2000 Rene Brun and Fons Rademakers
#
# Author: Fons Rademakers, 29/2/2000

ARCH          = linuxegcs

CXX           =
ObjSuf        = o
SrcSuf        = cxx
ExeSuf        =
DllSuf        = so
OutPutOpt     = -o 
DictSuf       = Dict.o

ROOTCFLAGS   := $(shell root-config --cflags)
ROOTLIBS     := $(shell root-config --libs)
ROOTGLIBS    := $(shell root-config --glibs)



ifeq ($(ARCH),linux)
# Linux with gcc 2.7.2.x
CXX           = g++
CXXFLAGS      = -O -Wall -fPIC
LD            = g++
LDFLAGS       = -O
SOFLAGS       = -shared
endif

ifeq ($(ARCH),linuxrh42)
# Linux with gcc 2.7.2.x (RedHat 4.2)
CXX           = g++
CXXFLAGS      = -O -Wall -fPIC
LD            = g++
LDFLAGS       = -O
SOFLAGS       = -shared
endif

ifeq ($(ARCH),linuxdeb)
# Linux with gcc 2.7.2.x
CXX           = g++
CXXFLAGS      = -O -Wall -fPIC
LD            = g++
LDFLAGS       = -O
SOFLAGS       = -shared
endif

ifeq ($(ARCH),linuxdeb2)
# Linux with gcc 2.7.2.x
CXX           = g++
CXXFLAGS      = -O -Wall -fPIC
LD            = g++
LDFLAGS       = -O
SOFLAGS       = -shared
endif

ifeq ($(ARCH),linuxsuse6)
# Linux with gcc 2.7.2.x
CXX           = g++
CXXFLAGS      = -O -Wall -fPIC
LD            = g++
LDFLAGS       = -O
SOFLAGS       = -shared
endif

ifeq ($(ARCH),linuxegcs)
# Linux with egcs (>= RedHat 5.2)
CXX           = g++
CXXFLAGS      = -O -Wall -fPIC
LD            = g++
LDFLAGS       = -O
SOFLAGS       = -shared
endif

ifeq ($(ARCH),linuxkcc)
# Linux with the KAI compiler
CXX           = KCC
CXXFLAGS      = -fPIC +K0
LD            = KCC
LDFLAGS       = -O
SOFLAGS       =
endif

ifeq ($(ARCH),linuxppcegcs)
# MkLinux with egcs/glibc
CXX           = g++
CXXFLAGS      = -O -Wall -fPIC
LD            = g++
LDFLAGS       = -O
SOFLAGS       = -shared -Wl,-soname,
endif

ifeq ($(ARCH),linuxia64gcc)
# Itanium Linux with gcc 2.9x
CXX           = g++
CXXFLAGS      = -O -Wall -fPIC
LD            = g++
LDFLAGS       = -O
SOFLAGS       = -shared
endif

ifeq ($(ARCH),linuxia64sgi)
# Itanium Linux with sgiCC
CXX           = sgiCC
CXXFLAGS      = -O -Wall -fPIC
LD            = gsgiCC
LDFLAGS       = -O
SOFLAGS       = -shared
endif

ifeq ($(ARCH),linuxalphaegcs)
# Alpha Linux with egcs
CXX           = g++
CXXFLAGS      = -O -Wall -fPIC
LD            = g++
LDFLAGS       = -O
SOFLAGS       = -shared
endif

ifeq ($(ARCH),linuxarm)
# ARM Linux with egcs
CXX           = g++
CXXFLAGS      = -O -Wall -fPIC
LD            = g++
LDFLAGS       = -O
SOFLAGS       = -shared
endif

ifeq ($(CXX),)
$(error $(ARCH) invalid architecture)
endif

DVCSFLAGS     = -I/afs/in2p3.fr/home/throng/clas/franck/theory/INCLUDE
DVCSLIBS      = -L/afs/in2p3.fr/home/throng/clas/franck/theory/LIB -lmydvcs -lTHEO -lMRST -lGPDQ
#CXXFLAGS     += $(ROOTCFLAGS) $(DVCSFLAGS)
CXXFLAGS     += $(ROOTCFLAGS)
LIBS          = $(ROOTLIBS) $(SYSLIBS)
GLIBS         = $(ROOTGLIBS) $(SYSLIBS)

#------------------------------------------------------------------------------

TGVKellyO           =TGVKelly.$(ObjSuf) \
                       TGVKellyDict.$(ObjSuf)

TGVKellyS           =TGVKelly.$(SrcSuf) \
                       TGVKellyDict.$(SrcSuf)

TGVKellySO          =libTGVKelly.$(DllSuf)

TGenGeoO           =TGenGeo.$(ObjSuf) \
                       TGenGeoDict.$(ObjSuf)

TGenGeoS           =TGenGeo.$(SrcSuf) \
                       TGenGeoDict.$(SrcSuf)

TGenGeoSO          =libTGenGeo.$(DllSuf)

TGenBaseO           =TGenBase.$(ObjSuf) \
                       TGenBaseDict.$(ObjSuf)

TGenBaseS           =TGenBase.$(SrcSuf) \
                       TGenBaseDict.$(SrcSuf)

TGenBaseSO          =libTGenBase.$(DllSuf)

TGenDVCSO           =TGenDVCS.$(ObjSuf) \
                       TGenDVCSDict.$(ObjSuf)

TGenDVCSS           =TGenDVCS.$(SrcSuf) \
                       TGenDVCSDict.$(SrcSuf)

TGenDVCSSO          =libTGenDVCS.$(DllSuf)

#TGenDVCSCLASO           =TGenDVCSCLAS.$(ObjSuf) \
#                       TGenDVCSCLASDict.$(ObjSuf)

#TGenDVCSCLASS           =TGenDVCSCLAS.$(SrcSuf) \
#                       TGenDVCSCLASDict.$(SrcSuf)

#TGenDVCSCLASSO          =libTGenDVCSCLAS.$(DllSuf)

TGenPi0O           =TGenPi0.$(ObjSuf) \
                       TGenPi0Dict.$(ObjSuf)

TGenPi0S           =TGenPi0.$(SrcSuf) \
                       TGenPi0Dict.$(SrcSuf)

TGenPi0SO          =libTGenPi0.$(DllSuf)

TGenDeltPi0O           =TGenDeltPi0.$(ObjSuf) \
                       TGenDeltPi0Dict.$(ObjSuf)

TGenDeltPi0S           =TGenDeltPi0.$(SrcSuf) \
                       TGenDeltPi0Dict.$(SrcSuf)

TGenDeltPi0SO          =libTGenDeltPi0.$(DllSuf)

TGenRhoO           =TGenRho.$(ObjSuf) \
                       TGenRhoDict.$(ObjSuf)

TGenRhoS           =TGenRho.$(SrcSuf) \
                       TGenRhoDict.$(SrcSuf)

TGenRhoSO          =libTGenRho.$(DllSuf)

TGenOmeO           =TGenOme.$(ObjSuf) \
                       TGenRhoDict.$(ObjSuf)

TGenOmeS           =TGenOme.$(SrcSuf) \
                       TGenRhoDict.$(SrcSuf)

TGenOmeSO          =libTGenOme.$(DllSuf)

TGenElasO           =TGenElas.$(ObjSuf) \
                       TGenElasDict.$(ObjSuf)

TGenElasS           =TGenElas.$(SrcSuf) \
                       TGenElasDict.$(SrcSuf)

TGenElasSO          =libTGenElas.$(DllSuf)

TGenPipS           =TGenPip.$(SrcSuf) \
                       TGenPipDict.$(SrcSuf)

TGenPipSO          =libTGenPip.$(DllSuf)

MAINO               = Gen.$(ObjSuf)
MAINS               = Gen.$(SrcSuf)
MAIN                = Gen$(ExeSuf)

MAIN2O               = Genclas.$(ObjSuf)
MAIN2S               = Genclas.$(SrcSuf)
MAIN2                = Genclas$(ExeSuf)

MAIN3O               = Genpi0.$(ObjSuf)
MAIN3S               = Genpi0.$(SrcSuf)
MAIN3                = Genpi0$(ExeSuf)

MAIN4O               = Genela.$(ObjSuf)
MAIN4S               = Genela.$(SrcSuf)
MAIN4                = Genela$(ExeSuf)

MAIN5O               = Gendeltpi0.$(ObjSuf)
MAIN5S               = Gendeltpi0.$(SrcSuf)
MAIN5                = Gendeltpi0$(ExeSuf)

MAIN6O               = GenPip.$(ObjSuf)
MAIN6S               = GenPip.$(SrcSuf)
MAIN6                = GenPip$(ExeSuf)

OBJS          = $(TGVKellyO) $(TGenGeoO) $(TGenBaseO) $(TGenDVCSO) $(TGenDVCSCLASO) $(TGenPi0O) $(TGenDeltPi0O)  $(TGenRhoO) $(TGenOmeO) $(TGenElasO) $(MAINO) $(MAIN2O) $(MAIN3O) $(MAIN4O) $(MAIN5O) $(MAIN6O)

SLIB          = $(TGVKellySO) $(TGenGeoSO) $(TGenBaseSO) $(TGenDVCSSO) $(TGenDVCSCLASSO) $(TGenPi0SO) $(TGenDeltPi0SO) $(TGenRhoSO) $(TGenOmeSO) $(TGenElasSO) $(TGenPipSO)

PROGRAMS      = $(MAIN) $(MAIN3) $(MAIN4) $(MAIN5) $(MAIN6)

#------------------------------------------------------------------------------

.SUFFIXES: .$(SrcSuf) .$(ObjSuf) .$(DllSuf)

all:            $(PROGRAMS) doc

lib:            $(ROOTGLIBS) $(SLIB)

doc:
	root.exe -b -q make_html.C

%.$(ObjSuf):%.$(SrcSuf)
	$(CXX) $(CXXFLAGS) -c $<

%Dict.$(SrcSuf):%.h
	rootcint -f $@ -c $< 

$(MAIN): $(MAINO) $(SLIB)
	$(LD) $(CXXFLAGS) $(LIBS) $^ -o $@ 

$(MAIN2): $(MAIN2O) $(SLIB)
	$(LD) $(CXXFLAGS) $(LIBS) $^ -o $@ 

$(MAIN3): $(MAIN3O) $(SLIB)
	$(LD) $(CXXFLAGS) $(LIBS) $^ -o $@ 

$(MAIN4): $(MAIN4O) $(SLIB)
	$(LD) $(CXXFLAGS) $(LIBS) $^ -o $@ 

$(MAIN5): $(MAIN5O) $(SLIB)
	$(LD) $(CXXFLAGS) $(LIBS) $^ -o $@ 

$(MAIN6): $(MAIN6O) $(SLIB)
	$(LD) $(CXXFLAGS) $(LIBS) $^ -o $@ 

lib%.$(DllSuf): %Dict.$(ObjSuf) %.$(ObjSuf)
	$(LD) $(ROOTGLIBS) $(SOFLAGS) $(CXXFLAGS) $^ -o $@

clean:
		@rm -f $(OBJS) core

distclean:      clean
		@rm -f $(PROGRAMS) $(SLIB) *Dict.* *.def


























