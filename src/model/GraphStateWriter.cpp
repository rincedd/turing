#include "GraphStateWriter.h"
#include <boost/foreach.hpp>

using namespace std;
using namespace largenet;

void GraphStateWriter::write(const Graph& g, ostream& strm)
{
	size_t K = concentrations_.size() / 2;
	BOOST_FOREACH(const Edge& e, g.edges())
	{
		strm << e.source()->id() << "\t" << e.target()->id() << "\t"
				<< weights_(e) << "\t" << concentrations_[e.source()->id()]
				<< "\t" << concentrations_[e.source()->id() + K] << "\t"
				<< concentrations_[e.target()->id()] << "\t"
				<< concentrations_[e.target()->id() + K] << "\n";
	}
}
