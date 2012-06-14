/**
 * @file AverageNodeStrength.h
 * @date 14.06.2012
 */

#ifndef AVERAGENODESTRENGTH_H_
#define AVERAGENODESTRENGTH_H_

#include "OrderParameter.h"
#include <largenet2.h>

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
	void update(const largenet::Edge& e, double old_weight, double new_weight);

	EdgeWeights& weights_;
	value_type value_;
};

#endif /* AVERAGENODESTRENGTH_H_ */
