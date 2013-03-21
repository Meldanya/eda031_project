#
# Makefile to make the test programs myserver and myclient,
# using the classes Connection and Server.
#
# The executables are created in the current directory.
#

# Define the compiler
CXX	 = clang++

# Define the linker. This must be done since the implicit rule
# for linking .o-files uses CC as the linker.
CC	 = clang++

# Define preprocessor and compiler flags
CPPFLAGS  = -I../src
CXXFLAGS  = -ggdb  -Wall -W -pedantic-errors
CXXFLAGS += -Wmissing-braces -Wparentheses -Wold-style-cast -std=c++0x

# Define linker flags
LDFLAGS  = -ggdb -L../build

# Libraries: -lclientserver is always necessary
LDLIBS   = -lclientserver 

# Define source files
SRC = $(wildcard *.cc)

# Targets
PROGS  = myserver myclient

# Phony targets
.PHONY: all clean cleaner lib

all: lib $(PROGS)

# Targets rely on implicit rules for compiling and linking.
# The dependency on libclientserver.a is not defined.
myserver: myserver.o
myclient: myclient.o

# Standard clean and cleaner
clean:
	$(RM) *.o *.d

cleaner: clean
	$(RM) $(PROGS)

lib:
	@make -C ../build

# Generate dependencies in *.d files
%.d: %.cc
	@set -e; rm -f $@; \
	 $(CXX) -MM $(CPPFLAGS) $< > $@.$$$$; \
	 sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	 rm -f $@.$$$$

# Include dependencies in *.d files
include $(SRC:.cc=.d)
