# Helper macros to convert spaces into question marks and back again
e := 
sp := $(e) $(e)
qs = $(subst ?,$(sp),$1)
sq = $(subst $(sp),?,$1)

# Get name of this Makefile (avoid getting word 0 and a starting space)
makefile_name := $(wordlist 1,1000,$(MAKEFILE_LIST))

# Get path of this Makefile
makefile_path := $(call qs,$(dir $(call sq,$(abspath $(call sq,$(makefile_name))))))

# Get path where the Application is
application_path := $(call qs,$(abspath $(call sq,$(makefile_path)../..)))

.PHONY: clean assets all

ifneq ($(words $(makefile_path))$(words $(MAKEFILE_LIST)),11)
all clean assets:
$(error Spaces not allowed in path)
else

ADDITIONAL_SOURCES := 	LibJPEG/source/jaricom.c	\
						LibJPEG/source/jcomapi.c \
						LibJPEG/source/jdapimin.c \
						LibJPEG/source/jdapistd.c \
						LibJPEG/source/jdarith.c \
						LibJPEG/source/jdatasrc.c \
						LibJPEG/source/jdcoefct.c \
						LibJPEG/source/jdcolor.c \
						LibJPEG/source/jddctmgr.c \
						LibJPEG/source/jdhuff.c \
						LibJPEG/source/jdinput.c \
						LibJPEG/source/jdmainct.c \
						LibJPEG/source/jdmarker.c \
						LibJPEG/source/jdmaster.c \
						LibJPEG/source/jdmerge.c \
						LibJPEG/source/jdpostct.c \
						LibJPEG/source/jdsample.c \
						LibJPEG/source/jdtrans.c \
						LibJPEG/source/jerror.c \
						LibJPEG/source/jidctflt.c \
						LibJPEG/source/jidctfst.c \
						LibJPEG/source/jidctint.c \
						LibJPEG/source/jmemmgr.c \
						LibJPEG/source/jmemnobs.c \
						LibJPEG/source/jquant1.c \
						LibJPEG/source/jquant2.c \
						LibJPEG/source/jutils.c \
						widgets/src/JPEGFile.cpp \
						widgets/src/MJPEGReader.cpp \
						widgets/src/MJPEGWidget.cpp \
						widgets/src/StdFileInput.cpp
						
ADDITIONAL_INCLUDE_PATHS := LibJPEG\include \
							widgets\include \
							widgets\Config
							
ADDITIONAL_LIBRARY_PATHS := 
ADDITIONAL_LIBRARIES :=
export ADDITIONAL_SOURCES ADDITIONAL_INCLUDE_PATHS ADDITIONAL_LIBRARY_PATHS ADDITIONAL_LIBRARIES

all: $(filter assets,$(MAKECMDGOALS))
all assets: $(filter clean,$(MAKECMDGOALS))
all clean assets:
	@cd "$(application_path)" && $(MAKE) -r -f generated/simulator/gcc/Makefile -s $(MFLAGS) $@
endif

