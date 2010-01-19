SRC=$(wildcard *.cpp)
OBJ=$(SRC:.cpp=.o)
$(info $(SRC))
CPPFLAGS+= -pedantic -Wall -Wextra -g 
LDFLAGS+=-framework CoreFoundation -framework CoreServices

all: autobuild
autobuild: $(OBJ)
	$(LINK.cc) $(LDFLAGS) $^ -o $@

clean:
	$(RM) $(OBJ) autobuild
