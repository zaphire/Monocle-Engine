#pragma once

/*	MIN and MAX
		Standard math min/max macros.
		MIN returns the smaller of the arguments.
		MAX returns the larger of the arguments.
*/
#define MIN(a, b) (((a) < (b)) ? (a) : (b)) 
#define MAX(a, b) (((a) > (b)) ? (a) : (b)) 

/*	CLAMP
		Combines min and max in one macro for convenience.
*/
#define CLAMP(a, min, max) MIN(MAX(a, max), min)

/* SIGN
		If value is larger than 0, return to. 
		If value is smaller than 0, return -to. 
		If value is 0, return 0.
*/
#define SIGN(value, to) ((value < 0) ? -to : ((value > 0) ? to : 0))

/* SIGN
		If value is positive or zero, return 1
		If negative return -1
*/
#define SIGNOF(value) ((value >= 0) ? 1 : -1)

/*	APPROACH
		Returns value shifted toward target by amount, without crossing target.
		For example, APPROACH(2, 0, 3) will perform 2 - 3 = -1, but return 0 because -1 crossed the target of 0.
		APPROACH(-4, 0, 3) will return -1, because it will perform -4 + 3 = -1.
		*/
#define APPROACH(value, target, amount) ((value >= target) ? MAX(value - amount, target) : MIN(value + amount, target))

/*	LERP
		Linearly interpolate between two values.
		For example LERP(0, 10, 0.5) = 5, because 5 is 50% between 0 and 10.
		LERP(0, -4, 0.75) = -3, because -3 is 75% between 0 and -4.
*/
#define LERP(a, b, t) (a + (b - a) * t)

#define WEIGHTED_AVERAGE(current, target, slowdown) ((current * (slowdown - 1)) + target) / slowdown