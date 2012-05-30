ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS} -I m4

bin_PROGRAMS = turing

turing_SOURCES = \
	src/turing.cpp \
	src/model/TuringModel.h \
	src/model/TuringModel.cpp \
	src/ode/ODE.h \
	src/ode/Stepper.h \
	src/ode/RK4Stepper.h \
	src/ode/RK4Stepper.cpp \
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
turing_LDADD = $(DEPS_LIBS)
