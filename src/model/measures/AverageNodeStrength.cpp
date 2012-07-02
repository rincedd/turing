/**
 * @file AverageNodeStrength.cpp
 * @date 14.06.2012
 */

#include "AverageNodeStrength.h"
#include "../EdgeWeights.h"
#include <boost/foreach.hpp>
#include <boost/bind.hpp>

using namespace largenet;

AverageNodeStrength::AverageNodeStrength(EdgeWeights& weights) :
		weights_(weights), value_(0)
{
	compute();
	weight_change_connection_ = weights_.weight_changed.connect(
			boost::bind(&AverageNodeStrength::update, this, _1, _2, _3));
}

AverageNodeStrength::~AverageNodeStrength()
{
}

void AverageNodeStrength::compute()
{
	value_ = 0;
	BOOST_FOREACH(const EdgeWeights::node_strength_map_t::value_type s, weights_.strengths())
	{
		value_ += s.second;
	}
	value_ /= weights_.strengths().size();
}

void AverageNodeStrength::update(const edge_id_t e, double old_weight,
		double new_weight)
{
	compute();
}
