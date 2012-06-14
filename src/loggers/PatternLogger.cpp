/**
 * @file PatternLogger.cpp
 * @date 05.06.2012
 */

#include "PatternLogger.h"

#include <boost/foreach.hpp>

using namespace largenet;

PatternLogger::PatternLogger(const TuringModel& model, const Graph& g, time_type interval) :
		model_(model), interval_(interval), next_(0), nodes_()
{
	BOOST_FOREACH(const Node& n, g.nodes())
	{
		nodes_.insert(std::make_pair(n.degree(), n.id()));
	}
}

