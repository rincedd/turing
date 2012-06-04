/**
 * @file TuringModel.h
 * @date 29.05.2012
 */

#ifndef TURINGMODEL_H_
#define TURINGMODEL_H_

#include <largenet2.h>
#include <boost/function.hpp>

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix_sparse.hpp>

#include "../ode/ODE.h"

class TuringModel: public ode::ODE<boost::numeric::ublas::vector<double>, double>
{
public:
	typedef boost::numeric::ublas::compressed_matrix<double> laplacian_matrix_t;
	typedef boost::function<double(double u, double v)> coupling_function_t;
	struct Params
	{
		double activator_d; ///< diffusion constant of activator species
		double diff_ratio_inhibitor_activator; ///< diffusion constant ratio between inhibitor and activator species
	};

public:
	TuringModel(largenet::Graph& g, Params p,
			coupling_function_t activator_coupling,
			coupling_function_t inhibitor_coupling);
	virtual ~TuringModel();

	size_t dim() const
	{
		return 2 * graph_.numberOfNodes();
	}

	void recomputeLaplacian();

	void operator()(const state_type& y, state_type& dydx, const time_type x);

	const state_type& concentrations() const;
	state_type& concentrations();

	double patternAmplitude() const;
	double meanActivatorConcentration() const;
	double meanInhibitorConcentration() const;

private:
	largenet::Graph& graph_;
	Params par_;
	coupling_function_t activator_coupling_, inhibitor_coupling_;
	state_type concentrations_; //< activator and inhibitor concentrations
	laplacian_matrix_t laplacian_;
};

#endif /* TURINGMODEL_H_ */
