#ifndef AVERAGESLOGGER_H_
#define AVERAGESLOGGER_H_

#include "Logger.h"
#include "../model/TuringModel.h"
#include "../model/ConcentrationDifference.h"

class AveragesLogger: public Logger
{
public:
	AveragesLogger(const TuringModel& model, time_value_t interval) :
			model_(model), c_diff_(model.graph(), model.concentrations()), interval_(
					interval), next_(0)
	{
	}
	virtual ~AveragesLogger() {}
	void writeHeader()
	{
		std::cout << "#t\tA\t<u>\t<v>\t<c_ij>\n";
	}
	void log(const state_vector_t& state, const time_value_t t)
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
	time_value_t interval_, next_;
};

#endif // AVERAGESLOGGER_H_
