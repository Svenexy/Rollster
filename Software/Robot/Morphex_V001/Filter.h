#ifndef _FILTER_H
#define _FILTER_H

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

class Filter {
	public:
		int16_t firstOrderFilter (int16_t inputValue, int16_t currentValue, int16_t filter);
}


#endif
