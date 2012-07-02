/**
 * @file AverageEdgeWeight.h
 * @date 14.06.2012
 */

#ifndef AVERAGEEDGEWEIGHT_H_
#define AVERAGEEDGEWEIGHT_H_

#include "OrderParameter.h"
#include <largenet2.h>
#include <boost/signals2/signal.hpp>

class EdgeWeights;

/**
 * 
 *
 */
class AverageEdgeWeight: public OrderParameter<double>
{
public:
	AverageEdgeWeight(EdgeWeights& weights);
	virtual ~AverageEdgeWeight();
	value_type value() { return value_; }
private:
	void compute();
	void update(largenet::edge_id_t e, double old_weight, double new_weight);

	EdgeWeights& weights_;
	value_type value_;
	boost::signals2::scoped_connection weight_change_connection_;
};

#endif /* AVERAGEEDGEWEIGHT_H_ */
