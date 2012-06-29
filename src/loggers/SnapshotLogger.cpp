/**
 * @file SnapshotLogger.cpp
 * @date 26.06.2012
 */

#include "SnapshotLogger.h"
#include <boost/foreach.hpp>

using namespace largenet;

void SnapshotLogger::log(const state_type& state, const time_type t)
{
	if (t >= next_)
	{
		size_t K = state.size() / 2;
		stream() << "# t = " << t << "\n";
		BOOST_FOREACH(const Edge& e, graph_.edges())
		{
			stream() << e.source()->id() << "\t" << e.target()->id() << "\t"
					<< weights_(e) << "\t" << state[e.source()->id()] << "\t"
					<< state[e.source()->id() + K] << "\t"
					<< state[e.target()->id()] << "\t"
					<< state[e.target()->id() + K] << "\n";
		}
		stream() << "\n\n";
		next_ += interval_;
	}

}
