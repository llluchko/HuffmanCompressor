//
//  HuffmanCode.hpp
//  HuffmanCompressor
//
//  Created by Latchezar Mladenov on 2/15/16.
//  Copyright © 2016 Latchezar Mladenov. All rights reserved.
//

#ifndef HuffmanCode_h
#define HuffmanCode_h
#include <vector>
#include <algorithm>
#include <queue>
#include <string>
using namespace std;

class HuffmanCode {
private:
    struct Element {
        char c;
        string code;
    };
    
    struct Node {
        char c;
        string code;
        double value;
        Node *left, *right;
    };
    
    struct comparison {
        bool operator() (Node*& i, Node*& j) {
            return (i->value < j->value);
        }
    } comparer;
    
    vector<Element> codes;
    vector<Node*> nodes;
    Node* root;
    void generateCodes();
public:
    HuffmanCode(vector<char>&, vector<double>&);
    ~HuffmanCode(void);
    void getCodes(vector<string>&, const vector<char>&);
};

#endif /* HuffmanCode_h */
