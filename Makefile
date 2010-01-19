SRC=$(wildcard *.cpp)
OBJ=$(SRC:.cpp=.o)
$(info $(SRC))
CPPFLAGS+= -pedantic -Wall -Wextra -g 

ifeq ($(shell uname -s),Darwin)
LDFLAGS+=-framework CoreFoundation -framework CoreServices
else
# win32
LDFLAGS+=-mconsole
endif


all: autobuild
autobuild: $(OBJ)
	$(LINK.cc) $(LDFLAGS) $^ -o $@

clean:
	$(RM) $(OBJ) autobuild
