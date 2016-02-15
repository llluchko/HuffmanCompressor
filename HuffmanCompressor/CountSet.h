//
//  CountSet.h
//  HuffmanCompressor
//
//  Created by Latchezar Mladenov on 2/15/16.
//  Copyright © 2016 Latchezar Mladenov. All rights reserved.
//

#ifndef CountSet_h
#define CountSet_h
#include <vector>
using namespace std;

class CountSet {
private:
    struct Element {
        char c;
        int count;
    };
    vector<Element> elems;
public:
    CountSet(void);
    ~CountSet(void);
    void addElement(char);
    void removeLast();
    void getPercentages(vector<double>&);
    void getCharacters(vector<char>&);
};


#endif /* CountSet_h */
