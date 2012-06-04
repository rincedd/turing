/**
 * @file Logger.h
 * @date 04.06.2012
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include "../model/types.h"

/**
 * 
 *
 */
class Logger
{
public:
	Logger() {}
	virtual ~Logger() {}
	virtual void log(const state_vector_t& state, const time_value_t t) = 0;
};

#endif /* LOGGER_H_ */
