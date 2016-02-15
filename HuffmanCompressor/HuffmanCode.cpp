//
//  HuffmanCode.cpp
//  HuffmanCompressor
//
//  Created by Latchezar Mladenov on 2/15/16.
//  Copyright © 2016 Latchezar Mladenov. All rights reserved.
//

#include "HuffmanCode.h"

HuffmanCode::HuffmanCode(vector<char>& chars, vector<double>& values) {
    if (chars.size()!=values.size()) return;
    for (unsigned i=0; i<chars.size(); ++i) {
        nodes.push_back(new Node);
        nodes[nodes.size()-1]->c = chars[i];
        nodes[nodes.size()-1]->value = values[i];
        nodes[nodes.size()-1]->code = "";
        nodes[nodes.size()-1]->right = nodes[nodes.size()-1]->left = NULL;
    }
    sort(nodes.begin(), nodes.end(), comparer);
}

HuffmanCode::~HuffmanCode(void) {
    vector<Node*> nodeList;
    for (unsigned i = 0; i < nodes.size(); ++i)
        nodeList.push_back(nodes[i]);
    unsigned long size = nodeList.size();
    for (int i = 0; i < size; ++i) {
        queue<Node*> Q;
        Q.push(nodeList[i]);
        while (Q.size()) {
            Node* temp;
            nodeList.push_back(temp = Q.front());
            Q.pop();
            if (temp->left != NULL)
                Q.push(temp->left);
            if (temp->right != NULL)
                Q.push(temp->right);
        }
    }
    for (int i = 0; i < size; ++i)
        for (int j = i+1; j < size; ++j) {
            if (nodeList[j] == nodeList[i])
                nodeList[j] = NULL;
            delete nodeList[i];
        }	
}

void HuffmanCode::getCodes(vector<string>& cdes, const vector<char>& chars) {
    generateCodes();
    unsigned long size = chars.size();
    for (int i = 0; i < size; ++i)
        for (int j = 0; j < size; ++j)
            if (codes[j].c == chars[i]) {
                cdes.push_back(codes[j].code);
                continue;
            }
}

void HuffmanCode::generateCodes() {
    while (nodes.size()>1) {
        Node *temp = nodes[0];
        nodes[0] = new Node;
        nodes[0]->c = 0;
        nodes[0]->value = temp->value+nodes[1]->value;
        nodes[0]->left = new Node;
        *(nodes[0]->left) = *temp;
        nodes[0]->right = new Node;
        *(nodes[0]->right) = *nodes[1];
        delete temp;
        delete nodes[1];
        nodes.erase(nodes.begin()+1);
        sort(nodes.begin(), nodes.end(), comparer);
    }
    queue<Node*> Q;
    Q.push(nodes[0]);
    while (Q.size()) {
        Node *temp = Q.front();
        Q.pop();
        if (temp->left == NULL && temp->right == NULL) {
            Element tmp;
            tmp.c = temp->c;
            tmp.code = temp->code;
            codes.push_back(tmp);
            continue;
        }
        if (temp->left != NULL)
            (temp->left->code = temp->code).append("0"), Q.push(temp->left);
        if (temp->right!=NULL)
            (temp->right->code = temp->code).append("1"), Q.push(temp->right);
    }
}