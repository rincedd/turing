/*
 * GraphStateReader.cpp
 *
 *  Created on: 29.06.2012
 *      Author: gerd
 */

#include "GraphStateReader.h"
#include <sstream>
#include <utility>
#include <vector>
#include <boost/foreach.hpp>

using namespace std;
using namespace largenet;

namespace detail
{

struct node_info
{
	node_id_t id;
	double u, v;
};

struct edge_info
{
	node_info source;
	node_info target;
	double weight;
};

}

void GraphStateReader::createFromStream(istream& strm, Graph& graph,
		EdgeWeights& weights, ode::ode_traits<TuringModel>::state_type& states)
{
	if (!strm)
		return;

	typedef vector<detail::edge_info> edge_v;
	node_id_t maxNodeID = 0;
	edge_v edges;
	string line;
	stringstream ss;

	// now read: source target weight u_s v_s u_t v_t

	while (getline(strm, line))
	{
		detail::edge_info e;
		line += " ";
		ss.str(line);
		ss >> e.source.id >> e.target.id;
		if (ss.fail())
			throw std::runtime_error("Cannot read input file");

		maxNodeID = max(e.source.id, maxNodeID);
		maxNodeID = max(e.target.id, maxNodeID);

		ss >> e.weight >> e.source.u >> e.source.v >> e.target.u >> e.target.v;
		if (ss.fail())
			throw std::runtime_error("Cannot read input file");

		edges.push_back(e);
	}

	graph.clear();
	while (graph.numberOfNodes() < maxNodeID + 1)
		graph.addNode();

	size_t K = states.size() / 2;
	BOOST_FOREACH(detail::edge_info& e, edges)
	{
		edge_id_t eid = graph.addEdge(e.source.id, e.target.id, false);
		weights.setWeight(*graph.edge(eid), e.weight);
		states[e.source.id] = e.source.u;
		states[e.source.id + K] = e.source.v;
		states[e.target.id + K] = e.target.v;
	}
}
