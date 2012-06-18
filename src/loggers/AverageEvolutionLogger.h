/**
 * @file AverageEvolutionLogger.h
 * @date 15.06.2012
 */

#ifndef AVERAGEEVOLUTIONLOGGER_H_
#define AVERAGEEVOLUTIONLOGGER_H_

#include <largenet2.h>
#include <iomanip>
#include "Logger.h"
#include "../model/TuringModel.h"
#include "../model/EdgeWeights.h"
#include "../model/measures/AverageConcentrations.h"
#include "../model/measures/PatternAmplitude.h"
#include "../model/measures/ConcentrationDifference.h"
#include "../model/measures/AverageEdgeWeight.h"
#include "../model/measures/AverageNodeStrength.h"

/**
 * 
 *
 */
class AverageEvolutionLogger: public StreamLogger<
		ode::ode_traits<TuringModel>::state_type,
		ode::ode_traits<TuringModel>::time_type>
{
public:
	AverageEvolutionLogger(const largenet::Graph& g, EdgeWeights& w,
			const state_type& s) :
			avg_sigma_(w), avg_strength_(w), c_diff_(g, s), avg_act_(s), avg_inh_(
					s), amplitude_(s)
	{
	}

	virtual ~AverageEvolutionLogger()
	{
	}

	void writeHeader(const time_type t)
	{
		stream() << "#n\t<sigma>\t<strength>\tA\t<u>\t<v>\t<c_ij>\t[C_ij>0]\n";
	}
	void log(const state_type& state, const time_type t)
	{
		c_diff_.update();
		stream() << std::setprecision(9) << t << "\t" << avg_sigma_.value() << "\t"
				<< avg_strength_.value() << "\t" << amplitude_.value() << "\t"
				<< avg_act_.value() << "\t" << avg_inh_.value() << "\t"
				<< c_diff_.value() << "\t" << c_diff_.positiveFraction()
				<< "\n";
	}
private:
	AverageEdgeWeight avg_sigma_;
	AverageNodeStrength avg_strength_;
	ConcentrationDifference c_diff_;
	AverageActivatorConcentration avg_act_;
	AverageInhibitorConcentration avg_inh_;
	PatternAmplitude amplitude_;
};

#endif /* AVERAGEEVOLUTIONLOGGER_H_ */
