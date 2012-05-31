/**
 * @file TuringOptions.cpp
 * @date 31.05.2012
 */

#include "TuringOptions.h"

namespace po = boost::program_options;

TuringOptions::TuringOptions() :
		par_(), allOptions_("Allowed options")
{
	allOptions_.add_options()("help,h", "This help message.");
	init();
}

void TuringOptions::init()
{
	po::options_description modelOpts("Model parameters"), iniOpts(
			"Initial conditions"), simOpts("Simulation parameters");

	modelOpts.add_options()
			("epsilon,e", po::value<double>(&par_.activator_diffusion)->default_value(0.12), "Diffusion constant of activator.")
			("sigma,s", po::value<double>(&par_.diffusion_ratio_inhibitor_activator)->default_value(13), "Inhibitor to activator ratio of diffusion constants.");

	iniOpts.add_options()
			("initial-activator-mean", po::value<double>(&par_.activator_mean)->default_value(2), "Initial mean activator concentration.")
			("initial-activator-var", po::value<double>(&par_.activator_var)->default_value(0.05), "Initial variance of activator concentration.")
			("initial-inhibitor-mean", po::value<double>(&par_.inhibitor_mean)->default_value(2), "Initial mean inhibitor concentration.")
			("initial-inhibitor-var", po::value<double>(&par_.inhibitor_var)->default_value(0.05), "Initial variance of inhibitor concentration.");

	simOpts.add_options()
			("nodes,n", po::value<size_t>(&par_.num_nodes)->default_value(100), "Number of nodes.")
			("avg-degree,k", po::value<double>(&par_.average_degree)->default_value(20), "Intended average degree of network.")
			("timestep", po::value<double>(&par_.integration_timestep)->default_value(0.5), "Integration time step.");

	allOptions_.add(modelOpts).add(iniOpts).add(simOpts);

}

void TuringOptions::parseCommandLine(int argc, char** argv)
{
	try
	{
		po::store(po::parse_command_line(argc, argv, allOptions_), vm_);
		po::notify(vm_);
	} catch (po::error& e)
	{
		throw ParsingError(e);
	}
	if (vm_.count("help"))
		throw UsageError(allOptions_);
}

