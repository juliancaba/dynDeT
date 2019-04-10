DIROBJ := obj/
DIREXE := bin/
DIRSRCPRESENTATION := src/presentation/
DIRSRCDOMAIN := src/domain/
DIRSRCPERSISTENCE := src/persistence/
DIRGENERATEXML := generate_boards_xml/
DIRXML := xml/

VERSION=2016

GENERATEXML_FILE := generateBoardsXML_$(VERSION).tcl

CPPC := g++
CPPCFLAGS := -c -ansi -Wall -fPIC -O2 -std=c++11
CPPCFLAGS += -D VERSION=$(VERSION) 
CPPCFLAGS_EXEC := -Wl,-rpath,/usr/local/lib -lctemplate_nothreads -lQt5Widgets -lQt5Core -lQt5Gui -O2


#Debian 8
MOC := moc
INCDIRS := -I./$(DIRSRCDOMAIN) -I./$(DIRSRCPERSISTENCE) -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtCore

#Ubuntu 16.04
#MOC := moc
#INCDIRS := -I./$(DIRSRCDOMAIN) -I./$(DIRSRCPERSISTENCE) -I/usr/include/qt5 -I/usr/include/qt5/QtWidgets -I/usr/include/qt5/QtGui -I/usr/include/qt5/QtCore

#Fedora 23
#MOC := moc-qt5
#INCDIRS := -I./$(DIRSRCDOMAIN) -I./$(DIRSRCPERSISTENCE) -I/usr/include/qt5 -I/usr/include/qt5/QtWidgets -I/usr/include/qt5/QtGui -I/usr/include/qt5/QtCore


MOC_HFILES := $(shell find $(DIRSRCPRESENTATION) -name '*.h' -printf "%f\n")
MOC_OBJECTS := $(MOC_HFILES:.h=.o.moc)
MOC_OBJECTS := $(foreach x,$(MOC_OBJECTS),$(DIROBJ)$(x))

SOURCES := $(shell find $(DIRSRCDOMAIN) -name '*.cpp' -printf "%f\n") $(shell find $(DIRSRCPERSISTENCE) -name '*.cpp' -printf "%f\n") $(shell find $(DIRSRCPRESENTATION) -name '*.cpp' -printf "%f\n")
OBJECTS := $(SOURCES:.cpp=.o)
OBJECTS := $(foreach x,$(OBJECTS),$(DIROBJ)$(x))


build: 
	@echo "[INFO] Make sure to indicate the Vivado version <make build VERSION=2016> for 2016.4"
	@echo "[INFO] Make sure to export Vivado tools"
	make clean dirs generate_xml write_factory_bitstreams language dynDeT


dirs:
	mkdir -p $(DIROBJ) $(DIREXE) $(DIRXML)

generate_xml:
	cd $(DIRGENERATEXML) && vivado -mode batch -source $(GENERATEXML_FILE)
	mv $(DIRGENERATEXML)*.xml $(DIRXML)
	rm -f $(DIRGENERATEXML)*.log $(DIRGENERATEXML)*.jou

dynDeT: $(OBJECTS) $(MOC_OBJECTS)
	$(CPPC) -o $(DIREXE)$@ $(^:.o.moc=_moc.o) $(CPPCFLAGS_EXEC)

$(DIROBJ)%.o: $(DIRSRCPERSISTENCE)%.cpp
	$(CPPC) $(CPPCFLAGS) $(INCDIRS) $^ -o $@

$(DIROBJ)%.o: $(DIRSRCDOMAIN)%.cpp
	$(CPPC) $(CPPCFLAGS) $(INCDIRS) $^ -o $@

$(DIROBJ)%.o: $(DIRSRCPRESENTATION)%.cpp
	$(CPPC) $(CPPCFLAGS) $(INCDIRS) $^ -o $@

$(DIROBJ)%.o.moc: $(DIRSRCPRESENTATION)%.moc.cpp
	$(CPPC) $(CPPCFLAGS) $(INCDIRS) $^ -o $(@:.o.moc=_moc.o)

$(DIRSRCPRESENTATION)%.moc.cpp: $(DIRSRCPRESENTATION)%.h
	$(MOC) $^ -o $@

language:
	lconvert -i src/languages/dyndetLangQt_en.ts -o src/languages/dyndetLangQt_en.qm

run:
	./$(DIREXE)dynDeT

install: BASE_DIR = /usr/share/dynDeT

install: #generate_xml write_factory_bitstreams dynDeT
	@echo "[INFO] Make sure to run <make build VERSION=XXXX> before install dynDeT tool"
	@echo "[INFO] This rule must have root permissions"
	install -d $(BASE_DIR)/bin
	install -m 655 ../write_factory_bitstreams/write_factory_bitstreams $(BASE_DIR)/bin/
	install -d $(BASE_DIR)/devices
	install -d $(BASE_DIR)/static/TCL
	install -d $(BASE_DIR)/static/template
	install -d $(BASE_DIR)/static/languages
	install TCL/* $(BASE_DIR)/static/TCL
	install static/* $(BASE_DIR)/static/
	install xml/* $(BASE_DIR)/devices/
	install template/* $(BASE_DIR)/static/template/
	install src/languages//* $(BASE_DIR)/static/languages/		
	install -m 655 bin/dynDeT $(BASE_DIR)/bin
	install -m 655 bin/dynDeT /usr/bin

write_factory_bitstreams:
	@cd ../write_factory_bitstreams/ ;\
	make; cd -


clean:
	$(RM) -rf *~ $(DIROBJ) $(DIREXE) $(DIRXML) $(DIRSRCPRESENTATION)*~ $(DIRSRCDOMAIN)*~ 

cleanall: clean
	$(RM) -rf generate_boards_xml/*~ generate_boards_xml/*.xml generate_boards_xml/*.jou
	$(RM) -rf generate_boards_xml/*.log
