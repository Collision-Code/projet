# Makefile

OS = $(shell uname)

ifeq ($(OS),Linux)
CXX = g++
MOC = moc
else
CXX = g++.exe
MOC = moc.exe
endif

CFLAGS = -w -fexceptions -D__NO_INLINE__

ifeq ($(OS),Linux)
LIB = -Llib -Llib/qt -Ilib
LDLIBS = -lQt5Widgets -lQt5Gui -lQt5Core -lQt5Concurrent
else
LIB = -Llib -Llib\qt -Ilib
LDLIBS = -lqtmain -lQt5Widgets -lQt5Gui -lQt5Core -lQt5Concurrent
endif

OBJDIR = obj
OBJDIR_RELEASE = $(OBJDIR)/Release
OBJ_RELEASE = 	$(OBJDIR_RELEASE)/writer/StdFileWriter.o \
				$(OBJDIR_RELEASE)/reader/MolFileReader.o \
				$(OBJDIR_RELEASE)/reader/PdbFileReader.o \
				$(OBJDIR_RELEASE)/reader/MfjFileReader.o \
				$(OBJDIR_RELEASE)/reader/XyzFileReader.o \
				$(OBJDIR_RELEASE)/reader/LogFileReader.o \
				$(OBJDIR_RELEASE)/reader/ChgChargesReader.o \
				$(OBJDIR_RELEASE)/reader/StdExtractFactory.o \
				$(OBJDIR_RELEASE)/reader/StdExtractResources.o \
				$(OBJDIR_RELEASE)/molecule/StdMolecule.o \
				$(OBJDIR_RELEASE)/molecule/StdAtom.o \
				$(OBJDIR_RELEASE)/math/StdResult.o \
                $(OBJDIR_RELEASE)/math/StdMean.o \
				$(OBJDIR_RELEASE)/math/StdMathLib.o \
				$(OBJDIR_RELEASE)/math/StdCalculationOperator.o \
				$(OBJDIR_RELEASE)/math/Vector3D.o \
				$(OBJDIR_RELEASE)/math/RandomGenerator.o \
				$(OBJDIR_RELEASE)/math/MonoThreadCalculationOperator.o \
                $(OBJDIR_RELEASE)/math/MultiThreadCalculationOperator.o \
				$(OBJDIR_RELEASE)/general/AtomInformations.o \
                $(OBJDIR_RELEASE)/general/GlobalParameters.o \
                $(OBJDIR_RELEASE)/general/SystemParameters.o \
				$(OBJDIR_RELEASE)/general/StdCmdView.o \
				$(OBJDIR_RELEASE)/general/StdGeometryCalculator.o \
                $(OBJDIR_RELEASE)/observer/Observable.o \
                $(OBJDIR_RELEASE)/observer/Observer.o \
                $(OBJDIR_RELEASE)/observer/state/CalculationState.o
        
OBJ_RELEASE_IHM = $(OBJDIR_RELEASE)/mainQt.o \
				$(OBJDIR_RELEASE)/gui/CCFrame.o \
				$(OBJDIR_RELEASE)/gui/moc_CCFrame.o

OBJ_RELEASE_CALC = $(OBJDIR_RELEASE)/main.o \
                   $(OBJDIR_RELEASE)/console/ConsoleView.o
				
CFLAGS_RELEASE = $(CFLAGS) -std=c++11 -fopenmp -O3

ifeq ($(OS),Linux)
INCPATH = -I. \
			-Iinclude \
			-Iinclude/unix \
			-Iinclude/QtGui \
			-Iinclude/QtWidgets \
			-Iinclude/QtConcurrent \
			-Iinclude/unix/QtCore

OUT_RELEASE_IHM = ./Collision-Code-GUI
OUT_RELEASE_CALC = ./Collision-Code
else
INCPATH = -I. \
			-Iinclude \
			-Iinclude/windows \
			-Iinclude/QtGui \
			-Iinclude/QtWidgets \
			-Iinclude/QtConcurrent \
			-Iinclude/windows/QtCore \
			-Iinclude/windows/win32-g++

OUT_RELEASE_IHM = Collision-Code-GUI.exe
OUT_RELEASE_CALC = Collision-Code.exe
endif

all: ihm calc

ifeq ($(OS),Linux)
prepare:
	if [ ! -d $(OBJDIR) ]; then mkdir $(OBJDIR); fi
	if [ ! -d $(OBJDIR_RELEASE) ]; then mkdir $(OBJDIR_RELEASE); fi
	if [ ! -d $(OBJDIR_RELEASE)/writer ]; then mkdir $(OBJDIR_RELEASE)/writer; fi
	if [ ! -d $(OBJDIR_RELEASE)/reader ]; then mkdir $(OBJDIR_RELEASE)/reader; fi
	if [ ! -d $(OBJDIR_RELEASE)/molecule ]; then mkdir $(OBJDIR_RELEASE)/molecule; fi
	if [ ! -d $(OBJDIR_RELEASE)/math ]; then mkdir $(OBJDIR_RELEASE)/math; fi
	if [ ! -d $(OBJDIR_RELEASE)/general ]; then mkdir $(OBJDIR_RELEASE)/general; fi
	if [ ! -d $(OBJDIR_RELEASE)/gui ]; then mkdir $(OBJDIR_RELEASE)/gui; fi
	if [ ! -d $(OBJDIR_RELEASE)/console ]; then mkdir $(OBJDIR_RELEASE)/console; fi
	if [ ! -d $(OBJDIR_RELEASE)/observer ]; then mkdir $(OBJDIR_RELEASE)/observer; fi
	if [ ! -d $(OBJDIR_RELEASE)/observer/state ]; then mkdir $(OBJDIR_RELEASE)/observer/state; fi
else
prepare:
	cmd /c if not exist $(OBJDIR_RELEASE) md $(OBJDIR)\\Release
	cmd /c if not exist $(OBJDIR_RELEASE)\\writer md $(OBJDIR)\\Release\\writer
	cmd /c if not exist $(OBJDIR_RELEASE)\\reader md $(OBJDIR)\\Release\\reader
	cmd /c if not exist $(OBJDIR_RELEASE)\\molecule md $(OBJDIR)\\Release\\molecule
	cmd /c if not exist $(OBJDIR_RELEASE)\\math md $(OBJDIR)\\Release\\math
	cmd /c if not exist $(OBJDIR_RELEASE)\\general md $(OBJDIR)\\Release\\general
	cmd /c if not exist $(OBJDIR_RELEASE)\\gui md $(OBJDIR)\\Release\\gui
	cmd /c if not exist $(OBJDIR_RELEASE)\\console md $(OBJDIR)\\Release\\console
	cmd /c if not exist $(OBJDIR_RELEASE)\\observer md $(OBJDIR)\\Release\\observer
	cmd /c if not exist $(OBJDIR_RELEASE)\\observer\\state md $(OBJDIR)\\Release\\observer\\state
endif

ihm: prepare gui/moc_CCFrame.cpp out_ihm
calc: prepare out_calc
	
out_ihm: $(OBJ_RELEASE) $(OBJ_RELEASE_IHM)
	$(CXX) -fopenmp -o $(OUT_RELEASE_IHM) $(OBJ_RELEASE) $(OBJ_RELEASE_IHM) $(INCPATH) $(LIB) $(LDLIBS) -s
  
out_calc: $(OBJ_RELEASE) $(OBJ_RELEASE_CALC)
	$(CXX) -fopenmp -o $(OUT_RELEASE_CALC) $(OBJ_RELEASE) $(OBJ_RELEASE_CALC) -s
	
$(OBJDIR_RELEASE)/writer/StdFileWriter.o: writer/StdFileWriter.cpp
	$(CXX) $(CFLAGS_RELEASE) $(LIB) -c writer/StdFileWriter.cpp -o $(OBJDIR_RELEASE)/writer/StdFileWriter.o
	
$(OBJDIR_RELEASE)/reader/MolFileReader.o: reader/MolFileReader.cpp
	$(CXX) $(CFLAGS_RELEASE) $(LIB) -c reader/MolFileReader.cpp -o $(OBJDIR_RELEASE)/reader/MolFileReader.o

$(OBJDIR_RELEASE)/reader/MfjFileReader.o: reader/MfjFileReader.cpp
	$(CXX) $(CFLAGS_RELEASE) $(LIB) -c reader/MfjFileReader.cpp -o $(OBJDIR_RELEASE)/reader/MfjFileReader.o
	
$(OBJDIR_RELEASE)/reader/LogFileReader.o: reader/LogFileReader.cpp
	$(CXX) $(CFLAGS_RELEASE) $(LIB) -c reader/LogFileReader.cpp -o $(OBJDIR_RELEASE)/reader/LogFileReader.o
	
$(OBJDIR_RELEASE)/reader/XyzFileReader.o: reader/XyzFileReader.cpp
	$(CXX) $(CFLAGS_RELEASE) $(LIB) -c reader/XyzFileReader.cpp -o $(OBJDIR_RELEASE)/reader/XyzFileReader.o
	
$(OBJDIR_RELEASE)/reader/PdbFileReader.o: reader/PdbFileReader.cpp
	$(CXX) $(CFLAGS_RELEASE) $(LIB) -c reader/PdbFileReader.cpp -o $(OBJDIR_RELEASE)/reader/PdbFileReader.o
	
$(OBJDIR_RELEASE)/reader/ChgChargesReader.o: reader/ChgChargesReader.cpp
	$(CXX) $(CFLAGS_RELEASE) $(LIB) -c reader/ChgChargesReader.cpp -o $(OBJDIR_RELEASE)/reader/ChgChargesReader.o
	
$(OBJDIR_RELEASE)/reader/StdExtractFactory.o: reader/StdExtractFactory.cpp
	$(CXX) $(CFLAGS_RELEASE) $(LIB) -c reader/StdExtractFactory.cpp -o $(OBJDIR_RELEASE)/reader/StdExtractFactory.o

$(OBJDIR_RELEASE)/reader/StdExtractResources.o: reader/StdExtractResources.cpp
	$(CXX) $(CFLAGS_RELEASE) $(LIB) -c reader/StdExtractResources.cpp -o $(OBJDIR_RELEASE)/reader/StdExtractResources.o

$(OBJDIR_RELEASE)/molecule/StdMolecule.o: molecule/StdMolecule.cpp
	$(CXX) $(CFLAGS_RELEASE) $(LIB) -c molecule/StdMolecule.cpp -o $(OBJDIR_RELEASE)/molecule/StdMolecule.o

$(OBJDIR_RELEASE)/molecule/StdAtom.o: molecule/StdAtom.cpp
	$(CXX) $(CFLAGS_RELEASE) $(LIB) -c molecule/StdAtom.cpp -o $(OBJDIR_RELEASE)/molecule/StdAtom.o

$(OBJDIR_RELEASE)/math/StdResult.o: math/StdResult.cpp
	$(CXX) $(CFLAGS_RELEASE) $(LIB) -c math/StdResult.cpp -o $(OBJDIR_RELEASE)/math/StdResult.o
    
$(OBJDIR_RELEASE)/math/StdMean.o: math/StdMean.cpp
	$(CXX) $(CFLAGS_RELEASE) $(LIB) -c math/StdMean.cpp -o $(OBJDIR_RELEASE)/math/StdMean.o

$(OBJDIR_RELEASE)/math/StdMathLib.o: math/StdMathLib.cpp
	$(CXX) $(CFLAGS_RELEASE) $(LIB) -c math/StdMathLib.cpp -o $(OBJDIR_RELEASE)/math/StdMathLib.o

$(OBJDIR_RELEASE)/math/StdCalculationOperator.o: math/StdCalculationOperator.cpp
	$(CXX) $(CFLAGS_RELEASE) $(LIB) -c math/StdCalculationOperator.cpp -o $(OBJDIR_RELEASE)/math/StdCalculationOperator.o
	
$(OBJDIR_RELEASE)/math/Vector3D.o: math/Vector3D.cpp
	$(CXX) $(CFLAGS_RELEASE) $(LIB) -c math/Vector3D.cpp -o $(OBJDIR_RELEASE)/math/Vector3D.o
	
$(OBJDIR_RELEASE)/math/RandomGenerator.o: math/RandomGenerator.cpp
	$(CXX) $(CFLAGS_RELEASE) $(LIB) -c math/RandomGenerator.cpp -o $(OBJDIR_RELEASE)/math/RandomGenerator.o

ifeq ($(OS),Linux)
$(OBJDIR_RELEASE)/mainQt.o: mainQt.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INCPATH) $(LIB) -fPIC -c mainQt.cpp -o $(OBJDIR_RELEASE)/mainQt.o
else
$(OBJDIR_RELEASE)/mainQt.o: mainQt.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INCPATH) $(LIB) -c mainQt.cpp -o $(OBJDIR_RELEASE)/mainQt.o
endif
  
$(OBJDIR_RELEASE)/main.o: main.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INCPATH) $(LIB) -c main.cpp -o $(OBJDIR_RELEASE)/main.o

$(OBJDIR_RELEASE)/general/AtomInformations.o: general/AtomInformations.cpp
	$(CXX) $(CFLAGS_RELEASE) $(LIB) -c general/AtomInformations.cpp -o $(OBJDIR_RELEASE)/general/AtomInformations.o

$(OBJDIR_RELEASE)/general/GlobalParameters.o: general/GlobalParameters.cpp
	$(CXX) $(CFLAGS_RELEASE) $(LIB) -c general/GlobalParameters.cpp -o $(OBJDIR_RELEASE)/general/GlobalParameters.o

$(OBJDIR_RELEASE)/general/SystemParameters.o: general/SystemParameters.cpp
	$(CXX) $(CFLAGS_RELEASE) $(LIB) -c general/SystemParameters.cpp -o $(OBJDIR_RELEASE)/general/SystemParameters.o
	
$(OBJDIR_RELEASE)/math/MonoThreadCalculationOperator.o: math/MonoThreadCalculationOperator.cpp
	$(CXX) $(CFLAGS_RELEASE) $(LIB) -c math/MonoThreadCalculationOperator.cpp -o $(OBJDIR_RELEASE)/math/MonoThreadCalculationOperator.o

$(OBJDIR_RELEASE)/math/MultiThreadCalculationOperator.o: math/MultiThreadCalculationOperator.cpp
	$(CXX) $(CFLAGS_RELEASE) $(LIB) -c math/MultiThreadCalculationOperator.cpp -o $(OBJDIR_RELEASE)/math/MultiThreadCalculationOperator.o

$(OBJDIR_RELEASE)/general/StdCmdView.o: general/StdCmdView.cpp
	$(CXX) $(CFLAGS_RELEASE) $(LIB) -c general/StdCmdView.cpp -o $(OBJDIR_RELEASE)/general/StdCmdView.o

$(OBJDIR_RELEASE)/general/StdGeometryCalculator.o: general/StdGeometryCalculator.cpp
	$(CXX) $(CFLAGS_RELEASE) $(LIB) -c general/StdGeometryCalculator.cpp -o $(OBJDIR_RELEASE)/general/StdGeometryCalculator.o

$(OBJDIR_RELEASE)/observer/Observer.o: observer/Observer.cpp
	$(CXX) $(CFLAGS_RELEASE) $(LIB) -c observer/Observer.cpp -o $(OBJDIR_RELEASE)/observer/Observer.o
    
$(OBJDIR_RELEASE)/observer/Observable.o: observer/Observable.cpp
	$(CXX) $(CFLAGS_RELEASE) $(LIB) -c observer/Observable.cpp -o $(OBJDIR_RELEASE)/observer/Observable.o

$(OBJDIR_RELEASE)/observer/state/CalculationState.o: observer/state/CalculationState.cpp
	$(CXX) $(CFLAGS_RELEASE) $(LIB) -c observer/state/CalculationState.cpp -o $(OBJDIR_RELEASE)/observer/state/CalculationState.o

$(OBJDIR_RELEASE)/console/ConsoleView.o: console/ConsoleView.cpp
	$(CXX) $(CFLAGS_RELEASE) $(LIB) -c console/ConsoleView.cpp -o $(OBJDIR_RELEASE)/console/ConsoleView.o
    
gui/moc_CCFrame.cpp: include/QtWidgets gui/CCFrame.h
	$(MOC) $(INCPATH) gui/CCFrame.h -o gui/moc_CCFrame.cpp
	
ifeq ($(OS),Linux)
$(OBJDIR_RELEASE)/gui/CCFrame.o: gui/CCFrame.cpp gui/CCFrame.h include/QtWidgets
	$(CXX) $(CFLAGS_RELEASE) $(INCPATH) $(LIB) -fPIC -c gui/CCFrame.cpp -o $(OBJDIR_RELEASE)/gui/CCFrame.o
	
$(OBJDIR_RELEASE)/gui/moc_CCFrame.o: gui/moc_CCFrame.cpp gui/CCFrame.h include/QtWidgets
	$(CXX) $(CFLAGS_RELEASE) $(INCPATH) $(LIB) -fPIC -c gui/moc_CCFrame.cpp -o $(OBJDIR_RELEASE)/gui/moc_CCFrame.o
else
$(OBJDIR_RELEASE)/gui/CCFrame.o: gui/CCFrame.cpp gui/CCFrame.h include/QtWidgets
	$(CXX) $(CFLAGS_RELEASE) $(INCPATH) $(LIB) -c gui/CCFrame.cpp -o $(OBJDIR_RELEASE)/gui/CCFrame.o
	
$(OBJDIR_RELEASE)/gui/moc_CCFrame.o: gui/moc_CCFrame.cpp gui/CCFrame.h include/QtWidgets
	$(CXX) $(CFLAGS_RELEASE) $(INCPATH) $(LIB) -c gui/moc_CCFrame.cpp -o $(OBJDIR_RELEASE)/gui/moc_CCFrame.o
endif
	
ifeq ($(OS),Linux)
clean:
	rm -f $(OBJ_RELEASE_IHM) $(OBJ_RELEASE_CALC) $(OBJ_RELEASE) $(OUT_RELEASE_IHM) $(OUT_RELEASE_CALC)
	rm -r -f $(OBJDIR)
else
clean:
	cmd /c if exist $(OUT_RELEASE_IHM) del /f $(OUT_RELEASE_IHM)
	cmd /c if exist $(OUT_RELEASE_CALC) del /f $(OUT_RELEASE_CALC)
	cmd /c if exist gui\\moc_CCFrame.cpp del /f gui\\moc_CCFrame.cpp
	cmd /c rd /s /q $(OBJDIR)
endif
