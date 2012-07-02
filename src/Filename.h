/*
 * Filename.h
 *
 *  Created on: 02.07.2012
 *      Author: gerd
 */

#ifndef FILENAME_H_
#define FILENAME_H_

#include "TuringOptions.h"
#include <string>

class Filename
{
public:
	explicit Filename(const std::string& name, const TuringOptions& opts,
			const std::string& tag = "") :
			value_(name)
	{
		if (!value_.empty())
			value_ += "-";
		value_ += opts.toStr();
		if (!tag.empty())
			value_ += "-" + tag;
		value_ += ".dat";
	}
	~Filename()
	{
	}

	operator std::string() const
	{
		return value_;
	}

private:
	std::string value_;
};

#endif /* FILENAME_H_ */
