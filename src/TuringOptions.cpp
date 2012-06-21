/**
 * @file TuringOptions.cpp
 * @date 31.05.2012
 */

#include "TuringOptions.h"
#include <boost/format.hpp>

namespace po = boost::program_options;
using namespace std;

TuringOptions::TuringOptions() :
		par_(), allOptions_("Allowed options")
{
	allOptions_.add_options()("help,h", "This help message.");
	init();
}

void TuringOptions::init()
{
	po::options_description modelOpts("Model parameters"), iniOpts(
			"Initial conditions"), simOpts("Simulation parameters"),
			intOpts("Integration parameters");

	modelOpts.add_options()
			("epsilon,e", po::value<double>(&par_.activator_diffusion)->default_value(0.12), "Diffusion constant of activator.")
			("sigma,s", po::value<double>(&par_.diffusion_ratio_inhibitor_activator)->default_value(13), "Inhibitor to activator ratio of diffusion constants.");

	iniOpts.add_options()
			("initial-activator-mean", po::value<double>(&par_.activator_mean)->default_value(5), "Initial mean activator concentration.")
			("initial-activator-var", po::value<double>(&par_.activator_var)->default_value(0.05), "Initial variance of activator concentration.")
			("initial-inhibitor-mean", po::value<double>(&par_.inhibitor_mean)->default_value(10), "Initial mean inhibitor concentration.")
			("initial-inhibitor-var", po::value<double>(&par_.inhibitor_var)->default_value(0.05), "Initial variance of inhibitor concentration.");

	simOpts.add_options()
			("network", po::value<std::string>(&par_.net_type)->default_value("random"), "Network type (random | BA).")
			("nodes,n", po::value<size_t>(&par_.num_nodes)->default_value(100), "Number of nodes.")
			("avg-degree,k", po::value<double>(&par_.average_degree)->default_value(20), "Intended average degree of network.")
			("iterations", po::value<size_t>(&par_.num_iterations)->default_value(10), "Number of topological update steps.")
			("increment", po::value<double>(&par_.weight_increment)->default_value(0.1), "Edge weight increment")
			("decrement", po::value<double>(&par_.weight_decrement)->default_value(0.01), "Edge weight decrement")
			("output-interval", po::value<size_t>(&par_.integration_output_interval)->default_value(5), "Log integration output every ARG iterations.");

	intOpts.add_options()
			("integration-time", po::value<double>(&par_.integration_time)->default_value(20.0), "Integration time.")
			("timestep", po::value<double>(&par_.integration_timestep)->default_value(0.5), "Integration time step.")
			("atol", po::value<double>(&par_.atol)->default_value(1e-4), "Absolute error tolerance for integration.")
			("rtol", po::value<double>(&par_.rtol)->default_value(1e-3), "Relative error tolerance for integration.");

	allOptions_.add(modelOpts).add(iniOpts).add(simOpts).add(intOpts);
}

string TuringOptions::toStr() const
{
	return str(
			boost::format(
					"%1%-n%2%-k%3%--eps%4%-sigma%5%--t%6%-i%7%--I%8%-du%9%-dd%10%")
					% par_.net_type % par_.num_nodes % par_.average_degree
					% par_.activator_diffusion
					% par_.diffusion_ratio_inhibitor_activator
					% par_.integration_time % par_.integration_timestep
					% par_.num_iterations % par_.weight_increment
					% par_.weight_decrement);
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

