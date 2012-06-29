/*
 * GraphStateReader.h
 *
 *  Created on: 29.06.2012
 *      Author: gerd
 */

#ifndef GRAPHSTATEREADER_H_
#define GRAPHSTATEREADER_H_

#include <largenet2.h>
#include <iostream>
#include "EdgeWeights.h"
#include "TuringModel.h"
#include "../ode/ode_traits.h"

class GraphStateReader
{
public:
	GraphStateReader()
	{
	}
	virtual ~GraphStateReader()
	{
	}
	void createFromStream(std::istream& strm, largenet::Graph& graph,
			EdgeWeights& weights,
			ode::ode_traits<TuringModel>::state_type& states);
};

#endif /* GRAPHSTATEREADER_H_ */
