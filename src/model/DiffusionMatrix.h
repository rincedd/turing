#ifndef DIFFUSIONMATRIX_H_
#define DIFFUSIONMATRIX_H_

#include <boost/numeric/ublas/matrix_sparse.hpp>
#include <largenet2.h>
#include <boost/signals2/signal.hpp>

class EdgeWeights;

/**
 * 
 *
 */
class DiffusionMatrix
{
public:
	typedef boost::numeric::ublas::compressed_matrix<double> d_matrix_t;

public:
	DiffusionMatrix(const largenet::Graph& g, EdgeWeights& w, double prefactor = 1);
	~DiffusionMatrix();
	const d_matrix_t& get() const { return matrix_; }
	void recompute(const EdgeWeights& w);
private:
	void updateEdgeWeight(const largenet::edge_id_t e, double old_weight,
			double new_weight);

private:
	const largenet::Graph& graph_;
	double prefactor_;
	d_matrix_t matrix_;
	boost::signals2::scoped_connection weight_change_connection_;
};

#endif /* DIFFUSIONMATRIX_H_ */
