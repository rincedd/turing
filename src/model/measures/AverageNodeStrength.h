/**
 * @file AverageNodeStrength.h
 * @date 14.06.2012
 */

#ifndef AVERAGENODESTRENGTH_H_
#define AVERAGENODESTRENGTH_H_

#include "OrderParameter.h"
#include <largenet2.h>
#include <boost/signals2/signal.hpp>

class EdgeWeights;

/**
 * 
 *
 */
class AverageNodeStrength: public OrderParameter<double>
{
public:
	AverageNodeStrength(EdgeWeights& weights);
	virtual ~AverageNodeStrength();
	value_type value() { return value_; }

private:
	void compute();
	void update(const largenet::edge_id_t e, double old_weight, double new_weight);

	EdgeWeights& weights_;
	value_type value_;
	boost::signals2::scoped_connection weight_change_connection_;
};

#endif /* AVERAGENODESTRENGTH_H_ */
