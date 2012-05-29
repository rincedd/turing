ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS} -I m4

bin_PROGRAMS = turing

turing_SOURCES = \
	src/turing.cpp \
	src/model/TuringModel.h \
	src/model/TuringModel.cpp \
	src/ode/ODE.h \
	src/ode/Stepper.h

turing_CXXFLAGS = $(AM_CXXFLAGS) $(BOOST_CXXFLAGS) $(DEPS_CFLAGS)
turing_LDADD = $(DEPS_LIBS)