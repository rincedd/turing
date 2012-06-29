/*
 * mimura.h
 *
 *  Created on: 29.06.2012
 *      Author: gerd
 */

#ifndef MIMURA_H_
#define MIMURA_H_

namespace mimura
{

const double a = 35, b = 16, c = 9, d = 2.0 / 5.0;

inline double f(double u, double v)
{
	return ((a + b * u - u * u) / c - v) * u;
}

inline double g(double u, double v)
{
	return (u - (1 + d * v)) * v;
}

}

#endif /* MIMURA_H_ */
