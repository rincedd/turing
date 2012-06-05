/**
 * @file Logger.h
 * @date 05.06.2012
 */

#ifndef LOGGER_H_
#define LOGGER_H_

/**
 * 
 *
 */
template<class State, class Time>
class Logger
{
public:
	typedef State state_type;
	typedef Time time_type;

	Logger()
	{
	}
	virtual ~Logger()
	{
	}
	virtual void log(const State& state, const Time t) = 0;
	virtual void writeHeader(const Time t) = 0;
};

#endif /* LOGGER_H_ */
