#include "EvolutionController.h"
#include "model/EdgeWeights.h"
#include "ode/Integrator.h"
#include "loggers/AveragesLogger.h"
#include "loggers/PatternLogger.h"
#include "loggers/AverageEvolutionLogger.h"
#include "loggers/SnapshotLogger.h"
#include "model/measures/ConcentrationDifference.h"
#include "mimura.h"
#include <largenet2/generators/generators.h>

using namespace largenet;
using namespace std;

EvolutionController::EvolutionController(TuringOptions& opts) :
		rng_(), opts_(opts), graph_(1, 1), weights_(0), model_(0), streams_()
{
}

EvolutionController::~EvolutionController()
{
	delete model_;
	delete weights_;
}

void EvolutionController::setup()
{
	if (opts_.params().seed > 0)
		rng_.seed(opts_.params().seed);
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

void EvolutionController::initConcentrations()
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

string EvolutionController::makeFilename(string tag) const
{
	string s(name());
	if (!s.empty())
		s += "-";
	s += opts_.toStr() + "-" + tag + ".dat";
	return s;
}

void EvolutionController::updateTopology()
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

void EvolutionController::writeInfo(ostream& strm) const
{
	strm << "# Turing model on " << opts_.params().net_type << " network with "
			<< graph_.numberOfNodes() << " nodes and " << graph_.numberOfEdges()
			<< " edges.";
	strm << "\n# Model parameters:" << "\n# epsilon = "
			<< opts_.params().activator_diffusion << "\n# sigma = "
			<< opts_.params().diffusion_ratio_inhibitor_activator;
	strm << "\n#\n# Simulation parameters:"
			<< "\n# Random number generator seed: " << opts_.params().seed
			<< "\n# Integration time: " << opts_.params().integration_time
			<< "\n# Integration output interval: "
			<< opts_.params().integration_timestep
			<< "\n# Integration tolerances: atol = " << opts_.params().atol
			<< ", rtol = " << opts_.params().rtol
			<< "\n# Number of topological updates: "
			<< opts_.params().num_iterations << "\n# Logging interval: every "
			<< opts_.params().integration_output_interval
			<< " topological updates" << "\n# Weight increment delta_up = "
			<< opts_.params().weight_increment
			<< "\n# Weight decrement delta_down = "
			<< opts_.params().weight_decrement << "\n";
}

int EvolutionController::exec()
{
	setup();

	Loggers<ode::ode_traits<TuringModel>::state_type,
			ode::ode_traits<TuringModel>::time_type> loggers;
	AveragesLogger* alog = new AveragesLogger(graph_, model_->concentrations(),
			opts_.params().integration_timestep);
	ostream& avg_strm = streams_.openStream(makeFilename("averages"));
	writeInfo(avg_strm);
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

	SnapshotLogger slog(*model_, graph_, *weights_,
			opts_.params().snapshot_interval);
	slog.setStream(streams_.openStream(makeFilename("network")));
	slog.writeHeader(0);

	ode::Integrator<TuringModel> integ(*model_, model_->concentrations(),
			opts_.params().atol, opts_.params().rtol);

	size_t iterations = 1, next = 1;
	for (; iterations <= opts_.params().num_iterations; ++iterations)
	{
		initConcentrations();
		if (iterations >= next)
		{
			integ.integrate(0.0, opts_.params().integration_time,
					opts_.params().integration_timestep, &loggers);
			loggers.reset();
			next += opts_.params().integration_output_interval;
		}
		else
		{
			integ.integrate(0.0, opts_.params().integration_time,
					opts_.params().integration_timestep);
		}
		updateTopology();
		ae_log.log(model_->concentrations(), iterations);
		slog.log(model_->concentrations(), iterations);
	}
	slog.log(model_->concentrations(), iterations);
	return 0;
}
