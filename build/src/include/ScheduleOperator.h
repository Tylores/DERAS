#ifndef SCHEDULEOPERATOR_H_INCLUDED
#define SCHEDULEOPERATOR_H_INCLUDED
#include "tsu.h"
#include "electric_water_heater.h"
#include <string>

class ScheduleOperator {
public:
	ScheduleOperator (std::string input, Aggregator *agg);
	virtual ~ScheduleOperator ();

	Aggregator *ptr_;
	tsu::string_matrix schedule_;

	void Idle ();
	void Export (std::string amount);
	void Import (std::string amount);
	void Loop ();

private:
	std::string file_name_;
	unsigned int index_;
};


#endif // SCHEDULEOPERATOR_H_INCLUDED
