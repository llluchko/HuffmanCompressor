//
//  CountSet.cpp
//  HuffmanCompressor
//
//  Created by Latchezar Mladenov on 2/15/16.
//  Copyright © 2016 Latchezar Mladenov. All rights reserved.
//

#include "CountSet.h"

CountSet::CountSet(void) { }

CountSet::~CountSet(void) { }

void CountSet::addElement(char e) {
    for (unsigned i = 0; i < elems.size(); ++i)
        if (elems[i].c == e) {
            ++elems[i].count;
            return;
        }
    Element temp;
    temp.c = e;
    temp.count = 1;
    elems.push_back(temp);
}

void CountSet::getPercentages(vector<double>& percs) {
    int count = 0;
    for (unsigned i=0; i<elems.size(); ++i) {
        count+=elems[i].count;
        percs.push_back(elems[i].count*100);
    }
    for (unsigned i=0; i<elems.size(); ++i)
        percs[i]/=count;
}

void CountSet::getCharacters(vector<char>& chars) {
    for (unsigned i=0; i<elems.size(); ++i)
        chars.push_back(elems[i].c);
}

void CountSet::removeLast() {
    elems.pop_back();
}

