/**
 * @file TuringOptions.h
 * @date 31.05.2012
 */

#ifndef TURINGOPTIONS_H_
#define TURINGOPTIONS_H_

#include <boost/program_options.hpp>
#include <string>
#include <iostream>

struct TuringParameters
{
	double activator_diffusion;
	double diffusion_ratio_inhibitor_activator;

	double activator_mean, activator_var;
	double inhibitor_mean, inhibitor_var;

	std::string net_type;
	size_t num_nodes;
	double average_degree;

	double integration_timestep;
	double integration_time;
	double atol, rtol;

	size_t num_iterations;
	double weight_increment, weight_decrement;
	size_t integration_output_interval;
	size_t snapshot_interval;
	unsigned long seed;
};

/**
 * 
 *
 */
class TuringOptions
{
public:
	class ParsingError: public boost::program_options::error
	{
	public:
		ParsingError(const boost::program_options::error& e) : error(e) {}
		explicit ParsingError(const std::string& xwhat) : error(xwhat) {}
	};
	class UsageError: public boost::program_options::error
	{
	public:
		UsageError(boost::program_options::options_description& opts) : error(""), opts_(opts) {}
		const char* what() const throw()
		{
			return "Invalid usage.";
		}
	private:
		boost::program_options::options_description& opts_;
	};
public:
	TuringOptions();
	void parseCommandLine(int argc, char** argv);
	const TuringParameters& params() const { return par_; }
	void printHelpText(std::ostream& out) const { out << allOptions_ << "\n"; }
	std::string toStr() const;
private:
	void init();
private:
	TuringParameters par_;
	boost::program_options::options_description allOptions_;
	boost::program_options::variables_map vm_;
};

#endif /* TURINGOPTIONS_H_ */
