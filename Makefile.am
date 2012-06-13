ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS} -I m4

bin_PROGRAMS = turing

turing_SOURCES = \
	src/turing.cpp \
	src/TuringOptions.cpp \
	src/TuringOptions.h \
	src/TuringApp.h \
	src/TuringApp.cpp \
	src/OStreamCollection.h \
	src/ode/ODE.h \
	src/ode/ode_traits.h \
	src/ode/Integrator.h \
	src/loggers/Logger.h \
	src/loggers/Loggers.h \
	src/loggers/AveragesLogger.h \
	src/loggers/PatternLogger.h \
	src/loggers/PatternLogger.cpp \
	src/model/types.h \
	src/model/TuringModel.h \
	src/model/TuringModel.cpp \
	src/model/OrderParameter.h \
	src/model/ConcentrationDifference.h \
	src/model/ConcentrationDifference.cpp \
	src/model/DiffusionMatrix.h \
	src/model/DiffusionMatrix.cpp \
	src/model/EdgeWeights.h \
	src/myrng1.3/MTEngine.cpp \
	src/myrng1.3/MTEngine.h \
	src/myrng1.3/myrng.h \
	src/myrng1.3/RandomVariates.h \
	src/myrng1.3/util.h \
	src/myrng1.3/WELLEngine.cpp \
	src/myrng1.3/WELLEngine.h

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