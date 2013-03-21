#
# Makefile to make the file libclientserver.a, containing
# connection.o and server.o
#

CXX		 = clang++
CC		 = clang++
CXXFLAGS  = -pipe -Wall -Werror -pedantic-errors
CXXFLAGS += -Wextra -std=c++0x
# The following option cannot be used since some of the socket
# macros give warnings on "old-style-cast"
CXXFLAGS	+= -Wold-style-cast
ifeq ($(DEBUG), true)
	CXXFLAGS += -ggdb
else
	CXXFLAGS += -O2 
endif

SRC_DIR   = ../src
BIN_DIR   = ../bin
VPATH     = $(SRC_DIR)
SRC       = $(wildcard $(SRC_DIR)/*.cc)

.PHONY: all clean cleaner

all: libclientserver.a

include $(notdir $(SRC:.cc=.d))

# Create the library; ranlib is for Darwin and maybe other systems.
# Doesn't seem to do any damage on other systems.

libclientserver.a: connection.o server.o
	ar rv libclientserver.a \
	connection.o server.o
	ranlib libclientserver.a

server: msg_handler.o ng_server.o libclientserver.a

%.d: %.cc
	@set -e; rm -f $(@F); \
		$(CXX) -MM $(CXXFLAGS) $< > $(@F).$$$$; \
		sed 's,\($*\)\.o[ :]*,\1.o $(@F) : ,g' < $(@F).$$$$ > $(@F); \
		rm -f $(@F).$$$$

clean:
	$(RM) -rf *.o *.d

cleaner: clean
	$(RM) libclientserver.a

