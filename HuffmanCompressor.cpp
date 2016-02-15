#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <string>
#include <fstream>
#include <cmath>
using namespace std;

struct Pair {
    char ch;
    string code;
};


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

class Compressor {
private:
    bool done;
    short endBlanks;
    vector<Pair> map;
    vector<unsigned char> head;
    string bodyBuffer;
    string fromFile;
    string toFile;
    void createHead();
    void createBody();
    void clearBuffer();
    string loadBuffer(ifstream&);
public:
    Compressor(vector<Pair>&);
    ~Compressor(void);
    void compress(string, string);
};


Compressor::Compressor(vector<Pair>& inMap) {
    for (unsigned i = 0; i < inMap.size(); ++i)
        map.push_back(inMap[i]);
    bodyBuffer = "";
    bodyBuffer.reserve(512);
    done = false;
    endBlanks = 0;
}

Compressor::~Compressor(void) { }

void Compressor::clearBuffer() {
    if (bodyBuffer.size()>256) {
        string temp = bodyBuffer.substr(256);
        bodyBuffer=temp;
    }
    if (bodyBuffer.size()==256) {
        bodyBuffer="";
    }
    if (done)
        while (bodyBuffer.size()%8!=0)
            bodyBuffer += "0", ++endBlanks;
}

void Compressor::compress(string from, string to) {
    fromFile = from;
    toFile = to;
    createHead();
    createBody();
}

void Compressor::createHead() {
    head.push_back(0);
    head.push_back(map.size());
    for (unsigned i = 0; i < map.size(); ++i)
        head.push_back(map[i].ch);
    for (unsigned i = 0; i < map.size(); ++i)
        head.push_back(map[i].code.size());
    for (unsigned i = 0; i < map.size(); ++i)
        bodyBuffer += map[i].code;
}

string Compressor::loadBuffer(ifstream& strm) {
    if (bodyBuffer.size() < 256) {
        char chr;
        while (bodyBuffer.size() < 256) {
            chr = strm.get();
            if (strm.eof()) break;
            for (unsigned j = 0; j < map.size(); ++j)
                if (map[j].ch == chr) {
                    bodyBuffer += map[j].code;
                    break;
                }
        }
        if (strm.eof())	done = true;
        if (done) {
            clearBuffer();
            return bodyBuffer;
        }
    }
    string temp = "";
    temp.reserve(256);
    for (int i = 0; i < 256; ++i)
        temp += bodyBuffer[i];
    clearBuffer();
    return temp;
}

void Compressor::createBody() {
    ofstream file;
    ifstream dat;
    file.open(toFile, ios::out|ios::binary);
    dat.open(fromFile, ios::in|ios::binary);
    if (!file.is_open()) return;
    for (unsigned i = 0; i < head.size(); ++i) {
        char buffer[1];
        buffer[0] = head[i];
        file.write(buffer,1);
    }
    while (!done) {
        string tmp = loadBuffer(dat);
        char buffer[256];
        int intBuffer[256];
        for (unsigned i = 0; i < tmp.size()/8; ++i)
            intBuffer[i] = 0;
        for (unsigned i = 0; i < tmp.size(); i += 8)
            for (unsigned j = i; j < i + 8; ++j) {
                if (tmp[j] == '1')
                    intBuffer[i/8] += (int)pow((double)2, (double)(7 - j % 8));
            }
        for (unsigned i = 0; i < tmp.size(); ++i)
            buffer[i] = intBuffer[i];
        file.write(buffer, tmp.size() / 8);
        file.flush();
    }
    file.seekp(ios::beg);
    char buffer[1];
    buffer[0] = (char)endBlanks;
    file.write(buffer, 1);
    file.close();
    dat.close();
}


class Decompressor {
private:
    bool done;
    short endBlanks;
    int maxSize;
    vector<Pair> chrMap;
    vector<unsigned char> head;
    string bodyBuffer;
    string fromFile, toFile;
    void extract();
    string getFromBuffer(ifstream&, int);
    char getNext(ifstream&);
    string charToBin(unsigned char);
    int fileSize(const string);
public:
    Decompressor(void);
    ~Decompressor(void);
    void decompress(string, string);
    void testFile(ostream&);
};

Decompressor::Decompressor(void) {
    bodyBuffer = "";
    bodyBuffer.reserve(256);
    maxSize = 0;
    done = false;
}

Decompressor::~Decompressor(void) { }

void Decompressor::extract() {
    ifstream cFile;
    cFile.open(fromFile,ios::in | ios::binary);
    if (!cFile.is_open()) return;
    endBlanks = cFile.get();
    int mapSize;
    mapSize = cFile.get();
    vector<int> codeSizes;
    for (int i = 0; i < mapSize; ++i) {
        Pair temp;
        temp.ch = cFile.get();
        chrMap.push_back(temp);
    }
    for (int i = 0; i < mapSize; ++i)
        codeSizes.push_back(cFile.get());
    for (int i = 0; i < mapSize; ++i)
        if (maxSize < codeSizes[i])
            maxSize = codeSizes[i];
    for (int i = 0; i < mapSize; ++i)
        chrMap[i].code=getFromBuffer(cFile, codeSizes[i]);
    ofstream uFile;
    uFile.open(toFile, ios::out);
    while (!done)
        uFile<<getNext(cFile);
    cFile.close();
    uFile.close();
}

int Decompressor::fileSize(const string fileName) {
    std::ifstream f;
    f.open(fileName, std::ios_base::binary | std::ios_base::in);
    if (!f.good() || f.eof() || !f.is_open()) return 0;
    f.seekg(0, std::ios_base::beg);
    std::ifstream::pos_type begin_pos = f.tellg();
    f.seekg(0, std::ios_base::end);
    int temp = static_cast<int>(f.tellg() - begin_pos);
    f.close();
    return temp;
}

string Decompressor::getFromBuffer(ifstream& str, int leng) {
    while ((unsigned)leng > bodyBuffer.size()) {
        unsigned char tmp = (unsigned char)str.get();
        bodyBuffer += charToBin(tmp);
    }
    string ret = "";
    ret.reserve(leng);
    for (int i = 0; i < leng; ++i)
        ret += bodyBuffer[i];
    bodyBuffer = bodyBuffer.substr(leng);
    return ret;
}

void Decompressor::decompress(string from, string to) {
    fromFile = from;
    toFile = to;
    extract();
}

string Decompressor::charToBin(unsigned char input) {
    unsigned temp=(unsigned)input;
    string tmp = "", tm= "";
    tmp.reserve(8);
    tm.reserve(8);
    while (temp) {
        tmp += '0' + temp % 2;
        temp /= 2;
    }
    reverse(tmp.begin(), tmp.end());
    while (tmp.size() < 8) {
        tm = "0";
        tm += tmp;
        tmp = tm;
    }
    return tmp;
}

char Decompressor::getNext(ifstream& str) {
    while ((unsigned)maxSize>bodyBuffer.size() && !str.eof()) {
        unsigned char tmp = (unsigned char)str.get();
        bodyBuffer += charToBin(tmp);
    }
    if (str.eof()) {
        reverse(bodyBuffer.begin(),bodyBuffer.end());
        bodyBuffer = bodyBuffer.substr(endBlanks);
        reverse(bodyBuffer.begin(), bodyBuffer.end());
        done = true;
    }
    string s = "";
    s.reserve(maxSize);
    for (int i = 0; i < maxSize; ++i) {
        s += bodyBuffer[i];
        for (unsigned j=0; j<chrMap.size(); ++j)
            if (s == chrMap[j].code) {
                bodyBuffer=bodyBuffer.substr(i+1);
                char chr = chrMap[j].ch;
                return chr;
            }
    }
    return -1;
}

void Decompressor::testFile(ostream& str) {
    ifstream file;
    file.open(fromFile, ios::in);
    str<<"\n\nTest:\n\nMap:\n";
    for (unsigned i = 0; i < chrMap.size(); ++i)
        str<<chrMap[i].ch<<": "<<chrMap[i].code<<endl;
    str<<"\nBytes:\n";
    while (!file.eof()) {
        unsigned char chr = (unsigned char)file.get();
        string bin = charToBin(chr);
        str<<chr<<": "<<bin<<endl;
    }
    file.clear();
    file.seekg(0, ios::beg);
    str<<"\nFile contents:\n";
    while (!file.eof())
        str<<charToBin((unsigned char)file.get());
    str<<"\n\nTest ended.\n";
    file.close();
}

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
