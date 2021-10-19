CC=g++
CFLAGS=-Wall -fPIC
SOURCES=  TNA62richFrontend.cpp \
          TNA62richMap.cpp \
          TNA62richGeo.cpp \
          TNA62richScaler.cpp \
          TNA62richPixel.cpp \
          TNA62richThrScan.cpp
OBJ=$(SOURCES:.cpp=.o)
HEADERS=$(SOURCES:.cpp=.h) # for backup, target bk and gzip
LIB=libna62rich.so
LDFLAGS=-L. -lna62rich
CFLAGS+= -std=c++11   # required by ROOT 6
ROOTINC=-I $(shell root-config --incdir) # headers
ROOTLIB=-L $(shell root-config --libdir)  -lCore -lHist -lTree -lRIO -lGpad -lGraf # libraries
LDFLAGS+=$(ROOTLIB)

ARCHIVE 	= na62rich_sw.0.200205.tar

EXE=display1 display2 display3 translate mapping supercell

all : $(EXE)
	rm -f $(OBJ)
$(EXE): %: %.cpp $(LIB)
	$(CC) $(CFLAGS) -o $@  $< $(LDFLAGS) $(ROOTINC)

$(LIB):	$(OBJ)
	ar cr $(LIB) $(OBJ)

.cpp.o:
	$(CC) $(CFLAGS) -c $< $(ROOTINC)

clean:
	rm -f  $(EXE) $(LIB) *~ *.log
	rm -r -f *.dSYM *.pdf *.txt

bk	: gzip
	mv $(ARCHIVE) ..

gzip	:
	tar cvfz $(ARCHIVE) display* $(HEADERS) $(SOURCES) Makefile translation_table.txt \


# Build in "Make" variables
#  < - source file
#  @ - the target file
#  ^ - the dependancies files
# Archivio di backup


