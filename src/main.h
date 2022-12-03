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
#include <cmath>

#define SECOND * c_SECOND
#define MINUTE * c_MINUTE
#define HOUR * c_HOUR
#define DAY * c_DAY
#define MONTH * c_MONTH
#define YEAR * c_YEAR

long long times = 0;
double powerGenerated = 0;
double powerUsed = 0;
double failure = 0;
double revision = 0;

const unsigned long long c_SECOND = 1;
const unsigned long long c_MINUTE = c_SECOND * 60;
const unsigned long long c_HOUR = c_MINUTE * 60;
const unsigned long long c_DAY = c_HOUR * 24;
const unsigned long long c_MONTH = c_DAY * 30;
const unsigned long long c_YEAR = c_MONTH * 12;

double pw = 0;         // Sila solárneho panelu (kWp)
double up = 100;       // Percento využitia energie (Percent)
double pp = 0;         // Cena panelu (Kč)
double bc = 0;         // Kapacita batérie (kWh)
double hw = 3000;      // Spotreba domu (kWh)
double kwp = 6.5;      // Cena za kilowatt (Kč)
double fs = 0.244;     // Pravdepodobnost chyby systému (Percent)
double fp = 5000;      // Cena opravy chyby (Kč)
double rp = 4000;      // Cena revízie (Kč)
double bp = 0;         //battery power

double systemAge = 0;
double failureOccurance = 1 YEAR;
double generatePower = 1 MINUTE;
double generateUsage = 1 MINUTE;
double revisionOccurance = 4 YEAR;
double revisionLength = 30 MINUTE;

unsigned long long simulation_time = 25 YEAR;

std::string output = "solar.dat"; // Názov súboru pre zápis výsledkov

Facility Revision("Revízor");
Facility Failure("Porucha");
Facility Time_f("Čas");
Facility Usage("Spotreba");
Facility Network("Sieť");

//Store Battery("Batéria", (unsigned long)bc*60*1000);


#endif //IMS_PROJECT_2022_MAIN_H
