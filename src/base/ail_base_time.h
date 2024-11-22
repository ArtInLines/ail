/*
*** Basic Timing Functions ***
* Only includes ail_time_now() for now
*
* Timing functions are usually included in the `time` module
* Functions are only included in ail_base_time.h if modules other than `time` also require them
*/

#ifndef _AIL_BASE_TIME_H_
#define _AIL_BASE_TIME_H_

#include "ail_base.h"

// @Note: Returns the amount
internal u64 ail_time_now(void);

#endif // _AIL_BASE_TIME_H_
