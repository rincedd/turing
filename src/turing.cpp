#include <largenet2.h>
#include <largenet2/generators/generators.h>
#include <cassert>
#include <boost/numeric/ublas/io.hpp>

#include "model/TuringModel.h"
#include "ode/RK4Stepper.h"

#include "myrng1.3/myrng.h"
#include <largenet2/measures/spectrum.h>

using namespace largenet;

const double a = 35, b = 16, c = 9, d = 2.0 / 5.0;

double f(double u, double v)
{
	return ((a + b * u - u * u) / c - v) * u;
}

double g(double u, double v)
{
	return (u - (1 + d * v)) * v;
}

int main(int argc, char **argv)
{
	Graph net(1, 1);
	myrng::WELL1024a rng;
	generators::randomGnm(net, 10, 20, rng, false);

	measures::sparse_dmatrix_t lap = measures::laplacian(net);

	std::cout << lap << "\n";

	TuringModel::Params p = { 0.165, 15 };
	TuringModel m(net, p, f, g);

	double t = 0;
	std::cout << "Setting u[3] = 1 and v[5] = 1.\n";
	std::cout << "k_3 = " << net.node(3)->degree() << ", k_5 = " << net.node(5)->degree() << ".\n";
	m.concentrations()[3] = 1.0;
	m.concentrations()[15] = 1.0;
	std::cout << t << "\t" << m.concentrations() << "\n";
	ode::dvector dydx(m.dim());
	ode::RK4Stepper stepper(t, m.concentrations(), dydx);
	double interval = 10, next = interval;
	double h = 0.05;
	while (t <= 500.0)
	{
		t += stepper.step(h, m);
		if (t >= next)
		{
			std::cout << t << "\t" << m.concentrations() << "\n";
			next += interval;
		}
	}
	return 0;
}
