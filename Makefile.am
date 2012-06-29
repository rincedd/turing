ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS} -I m4

bin_PROGRAMS = turing

turing_SOURCES = \
	src/mimura.h \
	src/turing.cpp \
	src/TuringOptions.cpp \
	src/TuringOptions.h \
	src/TuringApp.h \
	src/TuringApp.cpp \
	src/Controller.h \
	src/EvolutionController.cpp \
	src/EvolutionController.h \
	src/ScanController.cpp \
	src/ScanController.h \
	src/OStreamCollection.h \
	src/ode/ODE.h \
	src/ode/ode_traits.h \
	src/ode/Integrator.h \
	src/loggers/Logger.h \
	src/loggers/Loggers.h \
	src/loggers/AveragesLogger.h \
	src/loggers/AverageEvolutionLogger.h \
	src/loggers/PatternLogger.h \
	src/loggers/PatternLogger.cpp \
	src/loggers/SnapshotLogger.h \
	src/loggers/SnapshotLogger.cpp \
	src/model/types.h \
	src/model/TuringModel.h \
	src/model/TuringModel.cpp \
	src/model/measures/OrderParameter.h \
	src/model/measures/ConcentrationDifference.h \
	src/model/measures/ConcentrationDifference.cpp \
	src/model/DiffusionMatrix.h \
	src/model/DiffusionMatrix.cpp \
	src/model/GraphStateWriter.h \
	src/model/GraphStateWriter.cpp \
	src/model/GraphStateReader.h \
	src/model/GraphStateReader.cpp \
	src/model/measures/AverageEdgeWeight.h \
	src/model/measures/AverageEdgeWeight.cpp \
	src/model/measures/AverageNodeStrength.h \
	src/model/measures/AverageNodeStrength.cpp \
	src/model/measures/AverageConcentrations.h \
	src/model/measures/PatternAmplitude.h \
	src/model/EdgeWeights.h \
	src/myrng/MTEngine.cpp \
	src/myrng/MTEngine.h \
	src/myrng/myrng.h \
	src/myrng/RandomVariates.h \
	src/myrng/util.h \
	src/myrng/WELLEngine.cpp \
	src/myrng/WELLEngine.h

if USE_BUILTIN_ODEINT
ODEINT_CXXFLAGS = -I ${srcdir}/odeint
else
ODEINT_CXXFLAGS = 
endif

turing_CXXFLAGS = $(AM_CXXFLAGS) $(ODEINT_CXXFLAGS) $(BOOST_CXXFLAGS) $(DEPS_CFLAGS)
turing_LDFLAGS = $(DEPS_LDFLAGS) $(BOOST_PROGRAM_OPTIONS_LDFLAGS)
turing_LDADD = $(DEPS_LIBS) $(BOOST_PROGRAM_OPTIONS_LIBS)

if DEBUGMODE
else
turing_CXXFLAGS += -DNDEBUG
endif