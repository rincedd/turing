/**
 * @file TuringApp.cpp
 * @date 04.06.2012
 */

#include "TuringApp.h"
#include "model/EdgeWeights.h"
#include "ode/Integrator.h"
#include "loggers/AveragesLogger.h"
#include "loggers/PatternLogger.h"
#include "loggers/AverageEvolutionLogger.h"
#include "model/measures/ConcentrationDifference.h"

#include <largenet2/generators/generators.h>

using namespace largenet;
using namespace std;

TuringApp::TuringApp() :
		name_(""), rng_(), opts_(), graph_(1, 1), weights_(0), model_(0), streams_()
{
}

TuringApp::~TuringApp()
{
	delete model_;
	delete weights_;
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
	if (opts_.params().net_type == "BA")
	{
		generators::randomBA(graph_, opts_.params().num_nodes,
				opts_.params().average_degree / 2, rng_);
		std::cout << "# BA network with N = ";
	}
	else
	{
		generators::randomGnm(graph_, opts_.params().num_nodes,
				opts_.params().average_degree * opts_.params().num_nodes / 2,
				rng_, false);
		std::cout << "# ER network with N = ";
	}
	/// TODO use some logger/logging framework for this
	std::cout << graph_.numberOfNodes() << " and L = " << graph_.numberOfEdges()
			<< " (<k> = "
			<< 2.0 * graph_.numberOfEdges() / graph_.numberOfNodes() << ").\n";

	weights_ = new EdgeWeights(graph_.numberOfEdges(), graph_.numberOfNodes());
	BOOST_FOREACH(const Edge& e, graph_.edges())
	{
		weights_->setWeight(e,
				opts_.params().diffusion_ratio_inhibitor_activator);
//		weights_->setWeight(e, abs(
//				rng_.GaussianPolar(opts_.params().diffusion_ratio_inhibitor_activator,
//						0.01)));
	}

	TuringModel::Params p =
	{ opts_.params().activator_diffusion,
			opts_.params().diffusion_ratio_inhibitor_activator };
	TuringModel::Coupling c =
	{ mimura::f, mimura::g };
	model_ = new TuringModel(graph_, *weights_, p, c);
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

string TuringApp::makeFilename(string tag) const
{
	string s(name_);
	if (!s.empty())
		s += "-";
	s += opts_.toStr() + "-" + tag + ".dat";
	return s;
}

void TuringApp::integrate(ode::ode_traits<TuringModel>::time_type from,
		ode::ode_traits<TuringModel>::time_type to,
		ode::ode_traits<TuringModel>::time_type dt, loggers_t* loggers)
{
	ode::Integrator<TuringModel> integ(*model_, model_->concentrations(),
			opts_.params().atol, opts_.params().rtol);
	integ.integrate(from, to, dt, loggers);

}

void TuringApp::updateTopology()
{
	Edge* e = 0;
	while (true)
	{
		e = graph_.randomEdge(rng_);
		LocalConcentrationDifference c_diff(*e, model_->concentrations());
		if (c_diff.value() == 0.0)
		{
			weights_->setWeight(*e,
					weights_->weight(*e) + opts_.params().weight_increment);
			break;
		}
		else if (weights_->weight(*e) >= opts_.params().weight_decrement)
		{
			weights_->setWeight(*e,
					weights_->weight(*e) - opts_.params().weight_decrement);
			break;
		}
	}
}

int TuringApp::exec()
{
	setup();

	Loggers<ode::ode_traits<TuringModel>::state_type,
			ode::ode_traits<TuringModel>::time_type> loggers;
	AveragesLogger* alog = new AveragesLogger(graph_, model_->concentrations(),
			opts_.params().integration_timestep);

	ostream& avg_strm = streams_.openStream(makeFilename("averages"));
	avg_strm << "# Turing model on " << opts_.params().net_type
			<< " network with " << graph_.numberOfNodes() << " nodes and "
			<< graph_.numberOfEdges() << " edges.\n# Parameters:"
			<< "\n# epsilon = " << opts_.params().activator_diffusion
			<< "\n# sigma = "
			<< opts_.params().diffusion_ratio_inhibitor_activator << "\n";
	alog->setStream(avg_strm);
	loggers.registerLogger(alog);
	PatternLogger* plog = new PatternLogger(*model_, graph_,
			opts_.params().integration_time);
	plog->setStream(streams_.openStream(makeFilename("patterns")));
	loggers.registerLogger(plog);
	loggers.writeHeaders(0.0);

	AverageEvolutionLogger ae_log(graph_, *weights_, model_->concentrations());
	ae_log.setStream(streams_.openStream(makeFilename("evolution")));
	ae_log.writeHeader(0);
	initConcentrations();
	ae_log.log(model_->concentrations(), 0);
	size_t iterations = 1, next = 1;
	for (; iterations <= opts_.params().num_iterations; ++iterations)
	{
		initConcentrations();
		if (iterations >= next)
		{
			integrate(0.0, opts_.params().integration_time,
					opts_.params().integration_timestep, &loggers);
			loggers.reset();
			next += opts_.params().integration_output_interval;
		}
		else
		{
			integrate(0.0, opts_.params().integration_time,
					opts_.params().integration_timestep);
		}
		updateTopology();
		ae_log.log(model_->concentrations(), iterations);
	}
	return 0;
}
