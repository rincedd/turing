#ifndef AVERAGESLOGGER_H_
#define AVERAGESLOGGER_H_

#include <largenet2.h>
#include <iomanip>
#include "Logger.h"
#include "../model/TuringModel.h"
#include "../model/measures/AverageConcentrations.h"
#include "../model/measures/PatternAmplitude.h"
#include "../model/measures/ConcentrationDifference.h"

class AveragesLogger: public StreamLogger<
		ode::ode_traits<TuringModel>::state_type,
		ode::ode_traits<TuringModel>::time_type>
{
public:
	AveragesLogger(const largenet::Graph& g, const state_type& s,
			time_type interval) :
			c_diff_(g, s), avg_act_(s), avg_inh_(s), amplitude_(s), interval_(
					interval), next_(0)
	{
	}
	virtual ~AveragesLogger()
	{
	}
	void reset()
	{
		next_ = 0;
		stream() << "\n\n";
	}
	void writeHeader(const time_type t)
	{
		stream() << "#t\tA\t<u>\t<v>\t<c_ij>\t[C_ij>0]\n";
	}
	void log(const state_type& state, const time_type t)
	{
		if (t >= next_)
		{
			c_diff_.update();
			stream() << std::setprecision(9) << t << "\t" << amplitude_.value()
					<< "\t" << avg_act_.value() << "\t" << avg_inh_.value()
					<< "\t" << c_diff_.value() << "\t"
					<< c_diff_.positiveFraction() << "\n";
			next_ += interval_;
		}
	}
private:
	ConcentrationDifference c_diff_;
	AverageActivatorConcentration avg_act_;
	AverageInhibitorConcentration avg_inh_;
	PatternAmplitude amplitude_;
	time_type interval_, next_;
};

#endif // AVERAGESLOGGER_H_
