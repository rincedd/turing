/**
 * @file SnapshotLogger.h
 * @date 26.06.2012
 */

#ifndef SNAPSHOTLOGGER_H_
#define SNAPSHOTLOGGER_H_

#include "Logger.h"
#include "../ode/ode_traits.h"
#include "../model/TuringModel.h"
#include "../model/EdgeWeights.h"
#include <largenet2.h>

/**
 * 
 *
 */
class SnapshotLogger: public StreamLogger<
		ode::ode_traits<TuringModel>::state_type,
		ode::ode_traits<TuringModel>::time_type>
{
public:
	SnapshotLogger(const TuringModel& model, const largenet::Graph& g,
			const EdgeWeights& w, time_type interval) :
			model_(model), graph_(g), weights_(w), interval_(interval), next_(0)
	{
	}
	virtual ~SnapshotLogger()
	{
	}
	void reset()
	{
		next_ = 0;
		stream() << "\n\n";
	}
	void writeHeader(const time_type t)
	{
		stream() << "# source\ttarget\tweight\tu_s\tv_s\tu_t\tv_t\n";
	}
	void log(const state_type& state, const time_type t);
private:
	const TuringModel& model_;
	const largenet::Graph& graph_;
	const EdgeWeights& weights_;
	time_type interval_, next_;
};

#endif /* SNAPSHOTLOGGER_H_ */
