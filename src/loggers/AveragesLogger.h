#ifndef AVERAGESLOGGER_H_
#define AVERAGESLOGGER_H_

#include "Logger.h"
#include "../model/TuringModel.h"
#include "../model/ConcentrationDifference.h"

class AveragesLogger: public StreamLogger<
		ode::ode_traits<TuringModel>::state_type,
		ode::ode_traits<TuringModel>::time_type>
{
public:
	AveragesLogger(const TuringModel& model, time_type interval) :
			model_(model), c_diff_(model.graph(), model.concentrations()), interval_(
					interval), next_(0)
	{
	}
	virtual ~AveragesLogger()
	{
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
			stream() << t << "\t" << model_.patternAmplitude() << "\t"
					<< model_.meanActivatorConcentration() << "\t"
					<< model_.meanInhibitorConcentration() << "\t"
					<< c_diff_.value() << "\t" << c_diff_.positiveFraction()
					<< "\n";
			next_ += interval_;
		}
	}
private:
	const TuringModel& model_;
	ConcentrationDifference c_diff_;
	time_type interval_, next_;
};

#endif // AVERAGESLOGGER_H_
