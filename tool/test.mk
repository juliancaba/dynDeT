DIROBJ := obj/
DIREXE := bin/
DIRSRCDOMAIN := src/domain/
DIRSRCPERSISTENCE := src/persistence/
DIRTEST := test/

CPPC := g++


# Debian 8
CPPFLAGS_TEST := -c -ansi -Wall -fPIC -fprofile-arcs -ftest-coverage -I$(DIRSRCDOMAIN) -I$(DIRSRCPERSISTENCE) -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtCore

# Ubuntu 16.04 & Fedora 23
#CPPFLAGS_TEST := -c -ansi -Wall -fPIC -fprofile-arcs -ftest-coverage -I$(DIRSRCDOMAIN) -I$(DIRSRCPERSISTENCE) -I/usr/include/qt5 -I/usr/include/qt5/QtWidgets -I/usr/include/qt5/QtGui -I/usr/include/qt5/QtCore


CPPFLAGS_TEST_EXEC := -Wl,-rpath,/usr/local/lib -lgtest -lgtest_main -lpthread -lgcov --coverage -lctemplate_nothreads -lQt5Widgets -lQt5Core -lQt5Gui

SOURCES := $(shell find $(DIRSRCDOMAIN) -name '*.cpp' -printf "%f\n") $(shell find $(DIRSRCPERSISTENCE) -name '*.cpp' -printf "%f\n") $(shell find $(DIRTEST) -name '*.cpp' -printf "%f\n")
OBJECTS := $(SOURCES:.cpp=.o)
OBJECTS := $(foreach x,$(OBJECTS),$(DIROBJ)$(x))


all : clean dirs tests

dirs:
	mkdir -p $(DIROBJ) $(DIREXE)

tests: $(OBJECTS)
	$(CPPC) -o $(DIREXE)$@ $^ $(CPPFLAGS_TEST_EXEC)

$(DIROBJ)%.o: $(DIRSRCDOMAIN)%.cpp
	$(CPPC) $(CPPFLAGS_TEST) $^ -o $@

$(DIROBJ)%.o: $(DIRSRCPERSISTENCE)%.cpp
	$(CPPC) $(CPPFLAGS_TEST) $^ -o $@

$(DIROBJ)%.o: $(DIRTEST)%.cpp
	$(CPPC) $(CPPFLAGS_TEST) $^ -o $@

run_tests:
	./$(DIREXE)tests

clean:
	rm -rf *~ *.gcov $(DIROBJ) $(DIREXE)

