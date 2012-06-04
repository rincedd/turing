/**
 * @file OrderParameter.h
 * @date 04.06.2012
 */

#ifndef ORDERPARAMETER_H_
#define ORDERPARAMETER_H_

/**
 * 
 *
 */
template<class Value>
class OrderParameter
{
public:
	typedef Value value_type;
public:
	OrderParameter() {}
	virtual ~OrderParameter() {}
	virtual value_type value() = 0;
};

#endif /* ORDERPARAMETER_H_ */
