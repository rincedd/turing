#include <largenet2.h>
#include <largenet2/generators/generators.h>

#include <cassert>
#include <cmath>
#include <iostream>

#include <boost/numeric/odeint.hpp>

#include "TuringOptions.h"
#include "model/TuringModel.h"
#include "myrng1.3/myrng.h"

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
	Output(const TuringModel& model, TuringModel::time_type interval) :
			model_(model), interval_(interval), next_(0)
	{
	}
	void operator()(const TuringModel::state_type& state,
			const TuringModel::time_type t)
	{
		if (t >= next_)
		{
			std::cout << t << "\t" << model_.patternAmplitude() << "\n";
			next_ += interval_;
		}
	}
private:
	const TuringModel& model_;
	TuringModel::time_type interval_, next_;
};

int main(int argc, char **argv)
{
	TuringOptions opts;
	try
	{
		opts.parseCommandLine(argc, argv);
	} catch (TuringOptions::UsageError& e)
	{
		opts.printHelpText(std::cout);
		exit(0);
	} catch (TuringOptions::ParsingError& e)
	{
		std::cerr << e.what() << "\n";
		exit(1);
	}

	Graph net(1, 1);
	myrng::WELL1024a rng;
//	generators::randomGnm(net, opts.params().num_nodes,
//			opts.params().average_degree * opts.params().num_nodes / 2, rng,
//			false);
	generators::randomBA(net, opts.params().num_nodes,
			opts.params().average_degree / 2, rng);
	//std::cout << "# ER network with N = ";
	std::cout << "# BA network with N = ";
	std::cout << net.numberOfNodes() << " and L = " << net.numberOfEdges()
			<< " (<k> = " << 2.0 * net.numberOfEdges() / net.numberOfNodes()
			<< ").\n";

	TuringModel::Params p =
	{ opts.params().activator_diffusion,
			opts.params().diffusion_ratio_inhibitor_activator };
	TuringModel m(net, p, f, g);

	double t = 0;
	size_t K = m.dim() / 2;
	for (size_t i = 0; i < K; ++i)
	{
		m.concentrations()[i] = fabs(
				rng.GaussianPolar(opts.params().activator_mean,
						opts.params().activator_var));
		m.concentrations()[i + K] = fabs(
				rng.GaussianPolar(opts.params().inhibitor_mean,
						opts.params().inhibitor_var));
	}

	typedef bno::runge_kutta_dopri5<TuringModel::state_type> error_stepper_t;
	typedef bno::result_of::make_controlled<error_stepper_t>::type stepper_t;

	stepper_t stepper = bno::make_controlled<error_stepper_t>(
			opts.params().atol, opts.params().rtol);

	Output out(m, opts.params().integration_timestep);
	size_t n_steps = bno::integrate_adaptive(stepper, m, m.concentrations(),
			0.0, opts.params().integration_time,
			opts.params().integration_timestep, out);

	std::cout << "Integration took " << n_steps << " steps.\n";

	return 0;
}
