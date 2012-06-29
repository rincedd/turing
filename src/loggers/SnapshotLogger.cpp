/**
 * @file SnapshotLogger.cpp
 * @date 26.06.2012
 */

#include "SnapshotLogger.h"
#include <boost/foreach.hpp>
#include "../model/GraphStateWriter.h"

using namespace largenet;

void SnapshotLogger::log(const state_type& state, const time_type t)
{
	if (t >= next_)
	{
		stream() << "# t = " << t << "\n";
		GraphStateWriter w(weights_, state);
		w.write(graph_, stream());
		stream() << "\n\n";
		next_ += interval_;
	}

}
