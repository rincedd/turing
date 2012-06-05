/**
 * @file ConcentrationDifference.cpp
 * @date 04.06.2012
 */

#include "ConcentrationDifference.h"

#include <boost/foreach.hpp>

using namespace largenet;
namespace bnu = boost::numeric::ublas;

ConcentrationDifference::ConcentrationDifference(const Graph& g,
		const state_vector_t& concentrations) :
		OrderParameter(), graph_(g), concentrations_(concentrations), activator_diffs_(
				g.numberOfEdges(), 0), inhibitor_diffs_(g.numberOfEdges(), 0)
{
	update();
}

void ConcentrationDifference::update()
{
	if (activator_diffs_.size() != graph_.numberOfEdges())
	{
		activator_diffs_.resize(graph_.numberOfEdges(), false);
		inhibitor_diffs_.resize(graph_.numberOfEdges(), false);
	}
	size_t i = 0;
	size_t K = concentrations_.size() / 2;
	BOOST_FOREACH(const Edge& e, graph_.edges())
	{
		activator_diffs_[i] = abs(
				concentrations_[e.source()->id()]
						- concentrations_[e.target()->id()]);
		inhibitor_diffs_[i] = abs(
				concentrations_[e.source()->id() + K]
						- concentrations_[e.target()->id() + K]);
		++i;
	}
}

ConcentrationDifference::value_type ConcentrationDifference::value()
{
	return (bnu::sum(activator_diffs_) + bnu::sum(inhibitor_diffs_))
			/ activator_diffs_.size();
}
