#pragma once

//Standard math min/max macros
#define MIN(a, b) (((a) < (b)) ? (a) : (b)) 
#define MAX(a, b) (((a) > (b)) ? (a) : (b)) 

//If value is larger than zero, return to. If value is smaller than zero, return -to. If value is zero, return zero.
#define SIGN(value, to) ((value < 0) ? -to : ((value > 0) ? to : 0))

//Returns value shifted toward target by amount, without crossing target
#define APPROACH(value, target, amount) ((value >= target) ? MAX(value - amount, target) : MIN(value + amount, target))