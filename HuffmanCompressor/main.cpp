//
//  main.cpp
//  HuffmanCompressor
//
//  Created by Latchezar Mladenov on 2/15/16.
//  Copyright © 2016 Latchezar Mladenov. All rights reserved.
//

#include <iostream>
#include "CountSet.h"
#include "HuffmanCode.h"
#include "Compressor.h"
#include "Decompressor.h"
using namespace std;

int inspectFile(string);
void createMap();

CountSet cntS;
vector<double> pers;
vector<char> chrs;
vector<string> codes;

struct comparison {
    bool operator() (Pair& i, Pair& j) {
        return (i.code.length() < j.code.length());
    }
} comparer;

vector<Pair> map;

int main() {
    string fileName, compressName, decompressName;
    cout << "File name: ";
    cin >> fileName;
    cout << "Enter C to COMPRESS or D to DECOMPRESS: ";
    char input;
    cin >> input;
    if (input == 'C') {
        cout << "Enter archive name: ";
        compressName = fileName;
        while (compressName == fileName)
            cin >> compressName;
        if (!inspectFile(fileName)) {
            cout<<"Error inspecting file!";
            cin.get();
            return 0;
        }
        cntS.getPercentages(pers);
        cntS.getCharacters(chrs);
        HuffmanCode hCodes(chrs,pers);
        hCodes.getCodes(codes, chrs);
        createMap();
        Compressor comp(map);
        comp.compress(fileName, compressName);
        cout << "DONE." << endl;
    }
    
    else if (input == 'D') {
        cout << "Enter file name: ";
        decompressName = fileName;
        while (decompressName == fileName)
            cin >> decompressName;
        Decompressor decomp;
        decomp.decompress(fileName,decompressName);
        cout << "DONE." << endl;
    }
    else {
        cout<<"Wrong input.";
    }
    cin.get();
    return 0;
    
}


int inspectFile(string name) {
    ifstream dat;
    dat.open(name, ios::in);
    if(!dat.is_open())
        return 0;
    while(!dat.eof())
        cntS.addElement(dat.get());
    dat.close();
    cntS.removeLast();
    return 1;
}

void createMap() {
    for (unsigned i = 0; i < chrs.size(); ++i) {
        Pair temp;
        temp.ch = chrs[i];
        temp.code = codes[i];
        map.push_back(temp);
    }
    sort(map.begin(), map.end(), comparer);
}
