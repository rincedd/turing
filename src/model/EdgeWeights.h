/**
 * @file EdgeWeights.h
 * @date 12.06.2012
 */

#ifndef EDGEWEIGHTS_H_
#define EDGEWEIGHTS_H_

#include <largenet2/base/GraphListener.h>
#include <largenet2.h>
#include <boost/unordered_map.hpp>
#include <boost/signals2/signal.hpp>
#include <boost/foreach.hpp>

/**
 * 
 *
 */
class EdgeWeights: public largenet::GraphListener
{
public:
	typedef boost::unordered_map<largenet::edge_id_t, double> edge_weight_map_t;
	typedef boost::unordered_map<largenet::node_id_t, double> node_strength_map_t;

public: // signals
	boost::signals2::signal<void (const largenet::Edge&, double, double)> weight_changed;

public:
	/**
	 * Constructor.
	 * @param n (initial) number of edges to allocate space for
	 */
	EdgeWeights(largenet::edge_size_t num_edges, largenet::node_size_t num_nodes) :
			weights_(num_edges / 2), strengths_(num_nodes / 2)
	{
	}
	virtual ~EdgeWeights()
	{
	}

	void setWeight(const largenet::Edge& e, double value)
	{
		double old_weight = weights_[e.id()];	// will default-construct weight if e is not present
		weights_[e.id()] = value;

		/// FIXME this could be problematic due to round-off errors
		strengths_[e.source()->id()] += value - old_weight;
		strengths_[e.target()->id()] += value - old_weight;
		weight_changed(e, old_weight, value);
	}

	double operator()(const largenet::edge_id_t e) const
	{
		return weight(e);
	}

	double operator()(const largenet::Edge& e) const
	{
		return weight(e);
	}

	double weight(const largenet::edge_id_t e) const
	{
		return weights_.at(e); // throws if e is not present
	}

	double weight(const largenet::Edge& e) const
	{
		return weight(e.id());
	}

	/**
	 * Total weight of edges attached to node @p n.
	 * @param n node ID
	 * @return node strength of node @p n
	 */
	double strength(largenet::node_id_t n) const
	{
		return strengths_.at(n);
	}

	const edge_weight_map_t& weights() const { return weights_; }
	const node_strength_map_t& strengths() const { return strengths_; }

private:
//	void afterEdgeAddEvent(largenet::Graph& g, largenet::Edge& e);
	void beforeEdgeRemoveEvent(largenet::Graph& g, largenet::Edge& e)
	{
		weights_.erase(e.id()); // does nothing if e is not present
	}
	void beforeGraphClearEvent(largenet::Graph& g)
	{
		weights_.clear();
	}
//	void afterEdgeStateChangeEvent(largenet::Graph& g, largenet::Edge& e,
//			largenet::edge_state_t oldState, largenet::edge_state_t newState);
private:
	edge_weight_map_t weights_;
	node_strength_map_t strengths_;
};

#endif /* EDGEWEIGHTS_H_ */
