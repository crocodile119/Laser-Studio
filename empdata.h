/*
 * empspdata.h
 *
 *  Created on: 16/gen/2018
 *      Author: GIORCARM
 */

#ifndef EMPPDATA_H
#define EMPPDATA_H

//using namespace std;

struct empdata {
    int formulaNumber;
    double wavelenght1;
    double wavelenght2;
    double time1;
    double time2;
    int CA;
    int CB;
    int CC;
    int CE;
    int t;
    int effects;
    double formula;
    int sort;

    enum empData{TABLE_EMP};
    };
#endif /* EMPPDATA_H */
