#include <largenet2.h>
#include <largenet2/generators/generators.h>
#include <cassert>

#include "model/TuringModel.h"
#include "myrng1.3/myrng.h"

#include <boost/numeric/odeint.hpp>

using namespace largenet;
namespace bno = boost::numeric::odeint;

const double a = 35, b = 16, c = 9, d = 2.0 / 5.0;

double f(double u, double v)
{
	return ((a + b * u - u * u) / c - v) * u;
}

double g(double u, double v)
{
	return (u - (1 + d * v)) * v;
}

class Output
{
public:
	Output(const TuringModel& model) :
			model_(model)
	{
	}
	void operator()(const ode::dvector& state, double t)
	{
		std::cout << t << "\t" << model_.patternAmplitude() << "\n";
	}
private:
	const TuringModel& model_;
};

int main(int argc, char **argv)
{
	Graph net(1, 1);
	myrng::WELL1024a rng;
	generators::randomBA(net, 1000, 10, rng);
	std::cout << "BA network with N = " << net.numberOfNodes() << " and L = "
			<< net.numberOfEdges() << " (<k> = "
			<< 2.0 * net.numberOfEdges() / net.numberOfNodes() << ").\n";

	TuringModel::Params p =
	{ 0.12, 14 };
	TuringModel m(net, p, f, g);

	double t = 0;
	for (ode::ODE::size_type i = 0; i < m.dim(); ++i)
	{
		m.concentrations()[i] = rng.GaussianPolar(0, 1e-8);
	}

	typedef bno::runge_kutta_cash_karp54<ode::dvector> error_stepper_t;
	//bno::runge_kutta4<ode::dvector> stepper = bno::runge_kutta4<ode::dvector>();
	bno::result_of::make_controlled<error_stepper_t>::type stepper =
			bno::make_controlled(1e-4, 1e-3, error_stepper_t());

	Output out(m);
	bno::integrate_const(stepper, m, m.concentrations(), 0.0, 20.0, 0.5, out);

	return 0;
}
