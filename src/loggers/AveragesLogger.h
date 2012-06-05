#ifndef AVERAGESLOGGER_H_
#define AVERAGESLOGGER_H_

#include "../ode/ode_traits.h"
#include "../model/TuringModel.h"
#include "../model/ConcentrationDifference.h"

class AveragesLogger
{
public:
	AveragesLogger(const TuringModel& model,
			ode::ode_traits<TuringModel>::time_type interval) :
			model_(model), c_diff_(model.graph(), model.concentrations()), interval_(
					interval), next_(0)
	{
	}
	virtual ~AveragesLogger()
	{
	}
	void writeHeader()
	{
		std::cout << "#t\tA\t<u>\t<v>\t<c_ij>\n";
	}
	void operator()(const ode::ode_traits<TuringModel>::state_type& state,
			const ode::ode_traits<TuringModel>::time_type t)
	{
		if (t >= next_)
		{
			c_diff_.update();
			std::cout << t << "\t" << model_.patternAmplitude() << "\t"
					<< model_.meanActivatorConcentration() << "\t"
					<< model_.meanInhibitorConcentration() << "\t"
					<< c_diff_.value() << "\n";
			next_ += interval_;
		}
	}
private:
	const TuringModel& model_;
	ConcentrationDifference c_diff_;
	ode::ode_traits<TuringModel>::time_type interval_, next_;
};

#endif // AVERAGESLOGGER_H_
