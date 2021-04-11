# makefile for building thid application

# default location of MHLIB is parent directory
ifndef MHLIB
  MHLIB=/home/djukanovic/Desktop/projects/mhlib/ #..
  export MHLIB
endif

include $(MHLIB)/makefile.common
CCFLAGS += -L$(MHLIB) -I$(MHLIB)

EXE=CLCS

HEADERS=

VPATH=structures

SRCS=CLCS.C CLCS_inst.C CLCS_solution.C BeamSearch.C AStar.C CLCS_evaluation.C 

OBJS=$(SRCS:.C=.o)

DEPS=$(SRCS:.C=.d)

$(EXE): $(OBJS) $(MHLIB)/libmh.a
	$(CPP) $(CCFLAGS) -o $(EXE) $(OBJS) -lmh

all: $(EXE)

clean:
	rm -f $(OBJS) $(DEPS) $(EXE)

sinclude $(DEPS)

