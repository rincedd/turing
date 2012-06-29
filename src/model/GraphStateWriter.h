#ifndef GRAPHSTATEWRITER_H_
#define GRAPHSTATEWRITER_H_

#include <largenet2/io/GraphWriter.h>
#include "../ode/ode_traits.h"
#include "EdgeWeights.h"
#include "TuringModel.h"

class GraphStateWriter: public largenet::io::GraphWriter
{
public:
	GraphStateWriter(const EdgeWeights& weights,
			const ode::ode_traits<TuringModel>::state_type& concentrations) :
			GraphWriter(), weights_(weights), concentrations_(concentrations)
	{
	}
	virtual ~GraphStateWriter()
	{
	}
	void write(const largenet::Graph& g, std::ostream& strm);
private:
	const EdgeWeights& weights_;
	const ode::ode_traits<TuringModel>::state_type& concentrations_;
};

#endif /* GRAPHSTATEWRITER_H_ */
