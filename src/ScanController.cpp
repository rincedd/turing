/*
 * ScanController.cpp
 *
 *  Created on: 29.06.2012
 *      Author: gerd
 */

#include "ScanController.h"

#include "model/EdgeWeights.h"
#include "ode/Integrator.h"
#include "loggers/Loggers.h"
#include "loggers/PatternLogger.h"
#include "Filename.h"
#include "mimura.h"

#include "model/GraphStateReader.h"
#include "loggers/ScanLogger.h"

#include <fstream>
#include <algorithm>
#include "model/measures/AverageEdgeWeight.h"

using namespace std;

ScanController::ScanController(TuringOptions& opts) :
		rng_(), opts_(opts), graph_(1, 1), weights_(0), orig_weights_(0), model_(
				0), streams_()
{
}

ScanController::~ScanController()
{
	delete model_;
	delete weights_;
	delete orig_weights_;
}

void ScanController::loadNetwork()
{
	ifstream in(opts_.params().file.c_str());
	if (!in)
		throw runtime_error(
				"Cannot read input file " + opts_.params().file + ".");

	GraphStateReader reader;
	weights_ = new EdgeWeights(opts_.params().num_nodes,
			opts_.params().num_nodes * opts_.params().average_degree / 2);
	ode::ode_traits<TuringModel>::state_type c;
	reader.createFromStream(in, graph_, *weights_, c);
	orig_weights_ = new EdgeWeights(*weights_);
	model_ = createModel(c);
}

TuringModel* ScanController::createModel(
		ode::ode_traits<TuringModel>::state_type& concentrations)
{
	TuringModel::Params p =
	{ opts_.params().activator_diffusion,
			opts_.params().diffusion_ratio_inhibitor_activator };
	TuringModel::Coupling c =
	{ mimura::f, mimura::g };
	TuringModel* m = new TuringModel(graph_, *weights_, p, c);
	copy(concentrations.begin(), concentrations.end(),
			m->concentrations().begin());
	return m;
}

void ScanController::setup()
{
	if (opts_.params().seed > 0)
		rng_.seed(opts_.params().seed);

	loadNetwork();

	std::cout << "Successfully loaded network with N = "
			<< graph_.numberOfNodes() << " and L = " << graph_.numberOfEdges()
			<< " (<k> = "
			<< 2.0 * graph_.numberOfEdges() / graph_.numberOfNodes() << ").\n";

	std::cout << "Loaded " << weights_->size() << " edge weights.\n";
}

void ScanController::writeInfo(ostream& strm) const
{
	strm << "# Edge weight scan for evolved Turing model on network with "
			<< graph_.numberOfNodes() << " nodes and " << graph_.numberOfEdges()
			<< " edges.";
	strm << "\n# Model parameters:" << "\n# epsilon = "
			<< opts_.params().activator_diffusion << "\n# <sigma> = "
			<< AverageEdgeWeight(*weights_).value();
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
			<< " topological updates" << "\n# Lower value of scan parameter = "
			<< opts_.params().scan_low << "\n# Upper value of scan parameter = "
			<< opts_.params().scan_high << "\n";
}

void ScanController::initConcentrations()
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

void ScanController::updateTopology(double scan_par)
{
	weights_->assign(*orig_weights_);
	weights_->scale(scan_par);
	model_->refresh(*weights_);
}

int ScanController::exec()
{
	setup();

	Loggers<ode::ode_traits<TuringModel>::state_type,
			ode::ode_traits<TuringModel>::time_type> loggers;

	PatternLogger* plog = new PatternLogger(*model_, graph_,
			opts_.params().integration_time);
	plog->setStream(streams_.openStream(Filename(name(), opts_, "patterns")));
	loggers.registerLogger(plog);
	loggers.writeHeaders(0.0);

	ScanLogger slog(graph_, *weights_, model_->concentrations());
	ostream& avg_strm = streams_.openStream(
			Filename(name(), opts_, "averages"));
	writeInfo(avg_strm);
	slog.setStream(avg_strm);
	slog.writeHeader(0);

	ode::Integrator<TuringModel> integ(*model_, model_->concentrations(),
			opts_.params().atol, opts_.params().rtol);

	size_t iterations = 0, next = 0;
	double step = (opts_.params().scan_high - opts_.params().scan_low)
			/ static_cast<double>(opts_.params().num_iterations);
	double scan_par = opts_.params().scan_low;
	updateTopology(scan_par);
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
		slog.log(model_->concentrations(), scan_par);
		scan_par += step;
		updateTopology(scan_par);
	}
	return 0;
}
