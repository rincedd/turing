/**
 * @file ConcentrationDifference.h
 * @date 04.06.2012
 */

#ifndef CONCENTRATIONDIFFERENCE_H_
#define CONCENTRATIONDIFFERENCE_H_

#include <largenet2.h>

#include "OrderParameter.h"
#include "../../ode/ode_traits.h"
#include "../TuringModel.h"

class LocalConcentrationDifference: public OrderParameter<double>
{
public:
	LocalConcentrationDifference(const largenet::Edge& e,
			const ode::ode_traits<TuringModel>::state_type& concentrations)
	{
		size_t K = concentrations.size() / 2;
		value_ = abs(
				concentrations[e.source()->id()]
						- concentrations[e.target()->id()])
				+ abs(
						concentrations[e.source()->id() + K]
								- concentrations[e.target()->id() + K]);
	}
	virtual ~LocalConcentrationDifference()
	{
	}
	value_type value()
	{
		return value_;
	}
private:
	value_type value_;
};

/**
 * 
 *
 */
class ConcentrationDifference: public OrderParameter<double>
{
public:
	ConcentrationDifference(const largenet::Graph& g,
			const ode::ode_traits<TuringModel>::state_type& concentrations);
	virtual ~ConcentrationDifference()
	{
	}
	value_type value();
	value_type positiveFraction();
	void update();
private:
	const largenet::Graph& graph_;
	const ode::ode_traits<TuringModel>::state_type& concentrations_;
	ode::ode_traits<TuringModel>::state_type activator_diffs_, inhibitor_diffs_;
};

#endif /* CONCENTRATIONDIFFERENCE_H_ */
