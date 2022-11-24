#include <iostream>
#include <string>
#include <fstream>
#include <pybind11/pybind11.h>
using namespace std;
namespace py = pybind11;

string base[] = { "746370", "207564" };

string entry(string chaine) {
    string matchingID(chaine);
    string hex, mt;
    long weight=0;
    long resError=-1;
    string resl="";
    int k=0;
    string xored="";
    string kor="";
    
    for (size_t j = 0; j < size(base); j++) {
        for (size_t i = 0; i < matchingID.length() / 2; i++)
        {
            hex=base[j][i*2];
            hex+=base[j][i*2+1];
            mt=matchingID[i*2];
            mt+=matchingID[i*2+1];

            long r=strtol(mt.c_str (), NULL, 16);
            long b=strtol(hex.c_str (), NULL, 16);
            k=0;
            while ( (r^k) <= b || abs((r^k) - b) >= b ) { k=1+k; }
            r=r^k;
            kor+="|"+std::to_string(k)+","+std::to_string(-(b-r));
            weight+=abs(b - r);
        }
        if (resError == -1 || resError > weight) {
            resError=weight;
            resl=base[j];
            xored=kor;
        }
        weight=0;
        kor="";
    }
    
    return std::to_string(resError) + ",'" + resl + "', '" + xored;
}

string getMatchingCase(string id, string dict)
{
    string matchingID(id); // "7f26374e1e5542b38fb825634c3daf47"
    string line;
    ifstream myfile (dict);
    long weight=0;
    long resError=-1;
    string resl="";

    if (myfile.is_open())
    {
        while ( getline (myfile,line) )
        {
            string tmp = line;
            string hex, mt;
            for (size_t i = 0; i < matchingID.length() / 2; i++)
            {
                hex=tmp[i*2];
                hex+=tmp[i*2+1];
                mt=matchingID[i*2];
                mt+=matchingID[i*2+1];
                
                weight+=abs(strtol(hex.c_str (), NULL, 16) - strtol(mt.c_str (), NULL, 16) + 0);
            }
            if (resError == -1 || resError > weight) {
                resError=weight;
                resl=tmp;
            }
            weight=0;
        }
        myfile.close();
    }
    
    return "ID: " + resl + ", Error: " + std::to_string(resError) + ",";
}

int main() {
    return 0;
}

PYBIND11_MODULE(calcWeight, m)
{
    m.def("getMatchingCase", &getMatchingCase);
    m.def("getEntry", &entry);
}

