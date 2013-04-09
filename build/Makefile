#
# Makefile to make the file libclientserver.a, containing
# connection.o and server.o
#

ifeq ($(shell uname), Darwin)
CXX		 = clang++
CC		 = clang++
# CXXFLAGS = -stdlib=libc++
else
CXX		 = g++
CC		 = g++
endif

CXXFLAGS += -pipe -Wall -Werror -pedantic-errors
CXXFLAGS += -Wextra -std=c++0x
# The following option cannot be used since some of the socket
# macros give warnings on "old-style-cast"
CXXFLAGS	+= -Wold-style-cast
ifeq ($(DEBUG), true)
	CXXFLAGS += -ggdb
else
	CXXFLAGS += -O2 
endif

ifeq ($(MEM_DB), true)
	CXXFLAGS += -D AMNESIA_DB
endif

SRC_DIR   = ../src
VPATH     = $(SRC_DIR)
SRC       = $(wildcard $(SRC_DIR)/*.cc)

.PHONY: all clean cleaner test_memdb test_filedb install

all: client mem_server file_server install

include $(notdir $(SRC:.cc=.d))

# Create the library; ranlib is for Darwin and maybe other systems.
# Doesn't seem to do any damage on other systems.
libclientserver.a: connection.o server.o
	ar rv libclientserver.a \
	connection.o server.o
	ranlib libclientserver.a

ng_server_mem.o: ng_server.cc
	$(CXX) $(CXXFLAGS) -D AMNESIA_DB -c -o $@ $<

# Doesn't work without these explicit rules, weird...
mem_server: server_msg_handler.o msg_handler.o ng_server_mem.o memdb.o libclientserver.a
	$(CXX) $^ -o $@

file_server: server_msg_handler.o msg_handler.o ng_server.o filedb.o libclientserver.a
	$(CXX) $^ -o $@

client: ng_client.o art.o ng.o client_msg_handler.o msg_handler.o libclientserver.a
	$(CXX) $^ -o $@

test_memdb: memdb_test
	./$<

memdb_test: memdb_test.o memdb.o art.o ng.o

test_filedb: filedb_test
	./$<

filedb_test: filedb_test.o filedb.o art.o ng.o

install: mem_server file_server client
	mkdir -p ../bin
	cp mem_server ../bin
	cp file_server ../bin
	cp client ../bin

%.d: %.cc
	@set -e; rm -f $(@F); \
		$(CXX) -MM $(CXXFLAGS) $< > $(@F).$$$$; \
		sed 's,\($*\)\.o[ :]*,\1.o $(@F) : ,g' < $(@F).$$$$ > $(@F); \
		rm -f $(@F).$$$$

clean:
	$(RM) *.o *.d* file_server mem_server main client

cleaner: clean
	$(RM) libclientserver.a
	$(RM) -r ../bin
	$(RM) -r database

