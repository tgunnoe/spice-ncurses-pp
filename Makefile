## file Makefile
CXX= g++
CXXSOURCES= main.cpp src/app.cpp src/startup.cpp src/mainWindow.cpp src/HD_Wallet.cpp
CXXOBJECTS= $(patsubst %.cpp, %.o, $(CXXSOURCES))
OPTIMFLAGS= -g -O
PACKAGES= libbitcoin libbitcoin-client
PKGCONFIG= pkg-config
CXXFLAGS= -std=c++11 -Wall $(OPTIMFLAGS) -Iinclude/
CPPFLAGS:=  $(shell $(PKGCONFIG) --cflags $(PACKAGES))
LIBES:= $(shell $(PKGCONFIG) --libs $(PACKAGES)) -lncurses++w -lformw -lmenuw -lpanelw -lncursesw
.PHONY: all clean

all: spice
spice: $(CXXOBJECTS)
	$(LINK.cc) -rdynamic $^ $(LIBES) -o $@

$(CXXOBJECTS): include/spice/*.hpp

clean:
	$(RM) src/*.o *.o *.so *.orig *~ spice core* *.hh.gch
## eof Makefile
