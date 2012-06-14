#ifndef AVERAGECONCENTRATIONS_H_
#define AVERAGECONCENTRATIONS_H_

#include <boost/numeric/ublas/vector_proxy.hpp>
#include "../TuringModel.h"
#include "../../ode/ode_traits.h"
#include "OrderParameter.h"

namespace bnu = boost::numeric::ublas;

/**
 * 
 *
 */
class AverageActivatorConcentration: public OrderParameter<double>
{
public:
	explicit AverageActivatorConcentration(
			const ode::ode_traits<TuringModel>::state_type& concentrations) :
			concentrations_(concentrations)
	{
	}
	virtual ~AverageActivatorConcentration()
	{
	}
	value_type value()
	{
		return bnu::sum(
				bnu::subrange(concentrations_, 0, concentrations_.size() / 2))
				/ (0.5 * concentrations_.size());

	}
private:
	const ode::ode_traits<TuringModel>::state_type& concentrations_;
};

class AverageInhibitorConcentration: public OrderParameter<double>
{
public:
	explicit AverageInhibitorConcentration(
			const ode::ode_traits<TuringModel>::state_type& concentrations) :
			concentrations_(concentrations)
	{
	}
	virtual ~AverageInhibitorConcentration()
	{
	}
	value_type value()
	{
		return bnu::sum(
				bnu::subrange(concentrations_, concentrations_.size() / 2,
						concentrations_.size()))
				/ (0.5 * concentrations_.size());
	}
private:
	const ode::ode_traits<TuringModel>::state_type& concentrations_;
};

#endif /* AVERAGECONCENTRATIONS_H_ */
