#include <largenet2.h>
#include <cassert>
#include <boost/numeric/ublas/io.hpp>

#include "model/TuringModel.h"

using namespace largenet;

const double a = 35, b = 16, c = 9, d = 2.0 / 5.0;

double f(double u, double v)
{
	return ((a + b * u - u * u) / c - v) * u;
}

double g(double u, double v)
{
	return (u - (1 + d * v)) * v;
}

int main(int argc, char **argv)
{
	Graph net(1, 1);
	for (int i = 0; i < 20; ++i)
	{
		node_id_t id = net.addNode();
		assert(id == i);
	}

	TuringModel::Params p =
	{ 0.165, 15.5 };
	TuringModel m(net, p, f, g);
	return 0;
}
