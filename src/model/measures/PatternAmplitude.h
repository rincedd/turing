/**
 * @file PatternAmplitude.h
 * @date 14.06.2012
 */

#ifndef PATTERNAMPLITUDE_H_
#define PATTERNAMPLITUDE_H_

#include <boost/numeric/ublas/vector_proxy.hpp>
#include "../TuringModel.h"
#include "../../ode/ode_traits.h"
#include "OrderParameter.h"
#include "AverageConcentrations.h"

class PatternAmplitude: public OrderParameter<double>
{
public:
	explicit PatternAmplitude(
			const ode::ode_traits<TuringModel>::state_type& concentrations) :
			concentrations_(concentrations)
	{
	}
	virtual ~PatternAmplitude()
	{
	}
	value_type value()
	{
		AverageActivatorConcentration avg_act(concentrations_);
		AverageInhibitorConcentration avg_inh(concentrations_);
		double act_val = avg_act.value(), inh_val = avg_inh.value();
		size_t K = concentrations_.size() / 2;
		double a = 0;
		for (size_t i = 0; i < K; ++i)
		{
			a += (concentrations_[i] - act_val) * (concentrations_[i] - act_val)
					+ (concentrations_[i + K] - inh_val)
							* (concentrations_[i + K] - inh_val);
		}
		return sqrt(a);
	}
private:
	const ode::ode_traits<TuringModel>::state_type& concentrations_;
};

#endif /* PATTERNAMPLITUDE_H_ */
