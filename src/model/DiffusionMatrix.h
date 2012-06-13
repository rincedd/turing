#ifndef DIFFUSIONMATRIX_H_
#define DIFFUSIONMATRIX_H_

#include <boost/numeric/ublas/matrix_sparse.hpp>
#include <largenet2.h>

#include "EdgeWeights.h"

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
	void onWeightChange(largenet::edge_id_t e, double old_weight,
			double new_weight);

private:
	double prefactor_;
	d_matrix_t matrix_;
};

#endif /* DIFFUSIONMATRIX_H_ */
