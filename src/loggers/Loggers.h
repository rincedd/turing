/**
 * @file Loggers.h
 * @date 05.06.2012
 */

#ifndef LOGGERS_H_
#define LOGGERS_H_

#include "Logger.h"
#include <boost/ptr_container/ptr_list.hpp>
#include <algorithm>

/**
 * 
 *
 */
template<class State, class Time>
class Loggers
{
public:
	typedef Logger<State, Time> logger_type;
	typedef typename Logger<State, Time>::state_type state_type;
	typedef typename Logger<State, Time>::time_type time_type;
private:
	typedef boost::ptr_list<logger_type> loggers_l;
public:
	Loggers()
	{
	}
	void registerLogger(logger_type* o)
	{
		loggers_.push_back(o);
	}
	logger_type* unregisterLogger(logger_type* o)
	{
		typename loggers_l::iterator it = std::find(loggers_.begin(),
				loggers_.end(), *o);
		return loggers_.release(it);
	}
	void writeHeaders(const time_type t)
	{
		for (typename loggers_l::iterator it = loggers_.begin();
				it != loggers_.end(); ++it)
		{
			it->writeHeader(t);
		}
	}
	void reset()
	{
		for (typename loggers_l::iterator it = loggers_.begin();
				it != loggers_.end(); ++it)
		{
			it->reset();
		}
	}
	void operator()(const state_type& state, const time_type t)
	{
		for (typename loggers_l::iterator it = loggers_.begin();
				it != loggers_.end(); ++it)
		{
			it->log(state, t);
		}
	}
private:
	loggers_l loggers_;
};

#endif /* LOGGERS_H_ */
