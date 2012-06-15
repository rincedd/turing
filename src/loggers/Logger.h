/**
 * @file Logger.h
 * @date 05.06.2012
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include <iostream>

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
	virtual void reset() {};
};

struct StreamOutput
{
	StreamOutput()
	{
		setStream(std::cout);
	}
	void setStream(std::ostream& s)
	{
		stream_ = &s;
	}
	std::ostream& stream()
	{
		return *stream_;
	}
protected:
	~StreamOutput() {}
private:
	std::ostream* stream_;
};

template<class State, class Time, class OutputPolicy = StreamOutput>
class StreamLogger: public Logger<State, Time>, public OutputPolicy
{
public:
	StreamLogger() : Logger<State, Time>(), OutputPolicy() {}
	virtual ~StreamLogger() {}
};

#endif /* LOGGER_H_ */
