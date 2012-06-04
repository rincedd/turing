/**
 * @file ConcentrationDifference.h
 * @date 04.06.2012
 */

#ifndef CONCENTRATIONDIFFERENCE_H_
#define CONCENTRATIONDIFFERENCE_H_

#include <largenet2.h>

#include "OrderParameter.h"
#include "types.h"

/**
 * 
 *
 */
class ConcentrationDifference : public OrderParameter<double>
{
public:
	ConcentrationDifference(const largenet::Graph& g, const state_vector_t& concentrations);
	virtual ~ConcentrationDifference() {}
	value_type value();
	void update();
private:
	const largenet::Graph& graph_;
	const state_vector_t& concentrations_;
	state_vector_t activator_diffs_, inhibitor_diffs_;
};

#endif /* CONCENTRATIONDIFFERENCE_H_ */
