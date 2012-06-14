/**
 * @file AverageEdgeWeight.cpp
 * @date 14.06.2012
 */

#include "AverageEdgeWeight.h"
#include "../EdgeWeights.h"
#include <boost/bind.hpp>
#include <boost/foreach.hpp>

using namespace largenet;

AverageEdgeWeight::AverageEdgeWeight(EdgeWeights& weights) :
		weights_(weights), value_(0)
{
	compute();
	weights_.weight_changed.connect(
			boost::bind(&AverageEdgeWeight::update, this, _1, _2, _3));
}

AverageEdgeWeight::~AverageEdgeWeight()
{
}

void AverageEdgeWeight::compute()
{
	value_ = 0;
	BOOST_FOREACH(const EdgeWeights::edge_weight_map_t::value_type w, weights_.weights())
	{
		value_ += w.second;
	}
	value_ /= weights_.weights().size();
}

void AverageEdgeWeight::update(const Edge& e, double old_weight,
		double new_weight)
{
	if (old_weight == 0.0)
	{
		compute();
	}
	else
	{
		value_ -= (old_weight - new_weight) / weights_.weights().size();
	}
}
