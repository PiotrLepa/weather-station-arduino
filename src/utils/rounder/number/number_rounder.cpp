#include "number_rounder.h"

double NumberRounder::roundToOneDecimalPlace(double value) { return round(value * 10) / 10; }