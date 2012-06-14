/**
 * @file AverageEdgeWeight.h
 * @date 14.06.2012
 */

#ifndef AVERAGEEDGEWEIGHT_H_
#define AVERAGEEDGEWEIGHT_H_

#include "OrderParameter.h"
#include <largenet2.h>

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
	void update(const largenet::Edge& e, double old_weight, double new_weight);

	EdgeWeights& weights_;
	value_type value_;
};

#endif /* AVERAGEEDGEWEIGHT_H_ */
