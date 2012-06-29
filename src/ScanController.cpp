/*
 * ScanController.cpp
 *
 *  Created on: 29.06.2012
 *      Author: gerd
 */

#include "ScanController.h"
#include "model/EdgeWeights.h"
#include "model/GraphStateReader.h"
#include "mimura.h"

#include <fstream>
#include <algorithm>

using namespace std;

ScanController::ScanController(TuringOptions& opts) :
		rng_(), opts_(opts), graph_(1, 1), weights_(0), model_(0)
{
}

ScanController::~ScanController()
{
	delete model_;
	delete weights_;
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
	createModel(c);
}

void ScanController::createModel(
		ode::ode_traits<TuringModel>::state_type& concentrations)
{
	TuringModel::Params p =
	{ opts_.params().activator_diffusion,
			opts_.params().diffusion_ratio_inhibitor_activator };
	TuringModel::Coupling c =
	{ mimura::f, mimura::g };
	model_ = new TuringModel(graph_, *weights_, p, c);
	copy(concentrations.begin(), concentrations.end(),
			model_->concentrations().begin());
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
}

int ScanController::exec()
{
	setup();
	return 0;
}
