/**
 * @file TuringApp.cpp
 * @date 04.06.2012
 */

#include "TuringApp.h"

#include "model/TuringModel.h"
#include "ode/Integrator.h"
#include "loggers/AveragesLogger.h"

#include <largenet2/generators/generators.h>

using namespace largenet;

TuringApp::TuringApp() :
		rng_(), opts_(), graph_(1, 1), model_(0)
{
}

TuringApp::~TuringApp()
{
	delete model_;
}

void TuringApp::parseCommandLine(int argc, char** argv)
{
	try
	{
		opts_.parseCommandLine(argc, argv);
	} catch (TuringOptions::UsageError& e)
	{
		opts_.printHelpText(std::cout);
		exit(0);
	} catch (TuringOptions::ParsingError& e)
	{
		std::cerr << e.what() << "\n";
		exit(1);
	}
}

namespace mimura
{

const double a = 35, b = 16, c = 9, d = 2.0 / 5.0;

double f(double u, double v)
{
	return ((a + b * u - u * u) / c - v) * u;
}

double g(double u, double v)
{
	return (u - (1 + d * v)) * v;
}

}

void TuringApp::setup()
{
	generators::randomGnm(graph_, opts_.params().num_nodes,
			opts_.params().average_degree * opts_.params().num_nodes / 2, rng_,
			false);
//	generators::randomBA(graph_, opts_.params().num_nodes,
//			opts_.params().average_degree / 2, rng_);
	std::cout << "# ER network with N = ";
//	std::cout << "# BA network with N = ";
	std::cout << graph_.numberOfNodes() << " and L = " << graph_.numberOfEdges()
			<< " (<k> = "
			<< 2.0 * graph_.numberOfEdges() / graph_.numberOfNodes() << ").\n";

	TuringModel::Params p =
	{ opts_.params().activator_diffusion,
			opts_.params().diffusion_ratio_inhibitor_activator };
	model_ = new TuringModel(graph_, p, mimura::f, mimura::g);
}

void TuringApp::initConcentrations()
{
	size_t K = model_->dim() / 2;
	for (size_t i = 0; i < K; ++i)
	{
		model_->concentrations()[i] = abs(
				rng_.GaussianPolar(opts_.params().activator_mean,
						opts_.params().activator_var));
		model_->concentrations()[i + K] = abs(
				rng_.GaussianPolar(opts_.params().inhibitor_mean,
						opts_.params().inhibitor_var));
	}
}

int TuringApp::exec()
{
	setup();
	initConcentrations();
	ode::Integrator<TuringModel, Logger> integ(*model_, model_->concentrations(),
			opts_.params().atol, opts_.params().rtol);
	integ.registerLogger(new AveragesLogger(*model_, opts_.params().integration_timestep));
	integ.integrate(0, opts_.params().integration_time, opts_.params().integration_timestep);
	return 0;
}
