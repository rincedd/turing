#ifndef DIFFUSIONMATRIX_H_
#define DIFFUSIONMATRIX_H_

#include <boost/numeric/ublas/matrix_sparse.hpp>
#include <largenet2.h>

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

private:
	void recompute(const largenet::Graph& g, const EdgeWeights& w);
	void updateEdgeWeight(const largenet::Edge& e, double old_weight,
			double new_weight);

private:
	double prefactor_;
	d_matrix_t matrix_;
};

#endif /* DIFFUSIONMATRIX_H_ */
