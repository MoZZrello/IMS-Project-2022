//
// Created by Richard Harman on 1. 12. 2022.
//

#ifndef IMS_PROJECT_2022_MAIN_H
#define IMS_PROJECT_2022_MAIN_H

#include <simlib.h>
#include <getopt.h>
#include <cstdio>
#include <cstring>
#include "iostream"

#define SECOND * c_SECOND
#define MINUTE * c_MINUTE
#define HOUR * c_HOUR
#define DAY * c_DAY
#define MONTH * c_MONTH
#define YEAR * c_YEAR

long long times = 0;

const unsigned long long c_SECOND = 1;
const unsigned long long c_MINUTE = c_SECOND * 60;
const unsigned long long c_HOUR = c_MINUTE * 60;
const unsigned long long c_DAY = c_HOUR * 24;
const unsigned long long c_MONTH = c_DAY * 30;
const unsigned long long c_YEAR = c_MONTH * 12;

double pw = 0;
double up = 100;
double pp = 0;
double bc = 0;
double hw = 3000;
double kwp = 6.5;
double fs = 0.244;
double fp = 5000;
double rr = 4 YEAR;
double rl = 30 MINUTE;
double rp = 4000;

double systemAge = 0;
long long failureOccurance = 1 YEAR;
long long generatePower = 1 MINUTE;
long long generateUsage = 1 MINUTE;

unsigned long long simulation_time = 25 YEAR;

std::string output = "solar.dat";

Facility Revision("Revízor");
Facility Failure("Porucha");
Facility Time("Čas");
Facility Usage("Spotreba");
Facility Network("Sieť");

Store Battery("Batéria", (unsigned long)bc*60*1000);


#endif //IMS_PROJECT_2022_MAIN_H
