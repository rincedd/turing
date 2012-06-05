/**
 * @file PatternLogger.h
 * @date 05.06.2012
 */

#ifndef PATTERNLOGGER_H_
#define PATTERNLOGGER_H_

#include "Logger.h"
#include "../ode/ode_traits.h"
#include "../model/TuringModel.h"
#include <map>
#include <largenet2.h>

/**
 * 
 *
 */
class PatternLogger: public StreamLogger<
		ode::ode_traits<TuringModel>::state_type,
		ode::ode_traits<TuringModel>::time_type>
{
private:
	typedef std::multimap<largenet::degree_t, largenet::node_id_t,
			std::greater<largenet::degree_t> > degree_map_t;
public:
	PatternLogger(const TuringModel& model, time_type interval);
	virtual ~PatternLogger()
	{
	}
	void writeHeader(const time_type t)
	{
		stream() << "#t\tID\tk\tu\tv\n";
	}
	void log(const state_type& state, const time_type t)
	{
		/// FIXME we must update our node map if the graph changes! This can
		/// be done using a custom GraphListener
		if (t >= next_)
		{
			size_t K = model_.concentrations().size() / 2;
			for (degree_map_t::iterator it = nodes_.begin(); it != nodes_.end();
					++it)
			{
				stream() << t << "\t" << it->second << "\t" << it->first << "\t"
						<< model_.concentrations()[it->second] << "\t"
						<< model_.concentrations()[it->second + K] << "\n";
			}
			stream() << "\n\n";
			next_ += interval_;
		}
	}
private:
	const TuringModel& model_;
	time_type interval_, next_;
	degree_map_t nodes_;
};

#endif /* PATTERNLOGGER_H_ */
