# This makefile is described by the following link
# https://hiltmon.com/blog/2015/09/28/the-simple-c-plus-plus-makefile-executable-edition/

CC := g++-5  #main compiler

# Project Directory
SRCDIR := src
BUILDDIR := obj
TARGETDIR := bin/debug
TARGET := $(TARGETDIR)/$(SRC)

# Auto get source files
SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))

# General Requirments
ifeq ($(CPU), x86)
	CPUFLAGS := -m32
elseif ($(CPU), arm)
	CPUFLAGS :=
	PILIBS := -lwiringPi
else
	CPUFLAGS :=
endif


CFLAGS := -Wall -pipe -std=c++11 -Wno-long-long -Wno-deprecated -g -DQCC_OS_LINUX -DQCC_OS_GROUP_POSIX -DQCC_DBG $(CPUFLAGS)
LIB := -lstdc++ -lpthread -lrt -lm $(PILIBS)
INC := -I src/include

# AllJoyn Requirments
ifeq ($(AJ_ROUTER), stand-alone)
	AJFLAGS := -DROUTER
else
	AJFLAGS :=
endif

CFLAGS += $(AJFLAGS)
LIB += -L$(AJ_LIB) -lalljoyn -lajrouter
INC += -I$(AJ_INC)

$(TARGET) : $(OBJECTS)
	@mkdir -p $(TARGETDIR)
	@echo "\n\tLinking $(TARGET)\n"; $(CC) $^ -o $(TARGET) $(LIB)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo "\n\tCompiling $<...\n"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	@echo "\n\tCleaning $(TARGET)\n"; $(RM) -r $(BUILDDIR) $(TARGET)

.PHONY: clean
