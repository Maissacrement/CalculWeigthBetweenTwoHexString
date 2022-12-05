#include <iostream>
#include <string>
#include <fstream>
#include <pybind11/pybind11.h>
#include <bits/stdc++.h>
using namespace std;
namespace py = pybind11;

string base[] = {
    "4f4b", "6d73", "6d56", "636d", "6d41", "6d41", "467a", "437a", "507a",
    "6d2f73", "48503a", "4c503a", "454446", "726164", "646567", "524154",
    "41674167436c", "4368313d"
};
// MQTT PAYLOAD
/*
string base[] = { 
    "434f4e4e454354", "434f4e4e41434b", "444953434f4e4e454354",
    "5055424c495348", "50554241434b", "505542524543",
    "50554252454c", "505542434f4d50", "535542534352494245", "53554241434b", 
    "554e535542534352494245", "554e53554241434b", "50494e47524551", "50494e4752455350",
    "636f6e6e656374696f6e", "436f6e74656e742d547970653a",
    "485454502f312e31", "323030204f4b", "656e636f64696e67", "343030206261642072657175657374",
    "486f73743a20", "474554202f20", "4d55535420", "4d555354204e4f5420", "524551554952454420", "5348414c4c204e4f5420", "5348414c4c20",
    "7573657220", "726573706f6e736520"
};*/

char mostFrequent(string arr, int n)
{
    // Find the max frequency using linear traversal
    int max_count = 1, curr_count = 1;
    char res = arr[0];
    for (int i = 1; i < n; i++) {
        if (arr[i] == arr[i - 1])
            curr_count++;
        else
            curr_count = 1;

        if (curr_count > max_count) {
            max_count = curr_count;
            res = arr[i - 1];
        }
    }
    return res;
}

string entry(string chaine) {
    string matchingID(chaine);
    string hex, mt;
    long weight=0;
    long resError=-1;
    string resl="";
    int k=0;
    string xored="";
    string kor="";
    int i=0;
    int datalen=0;
    string tmpRefKey="";
    int arrlen=0;
    
    // Size of based research payload
    for (size_t j = 0; j < sizeof(base)/sizeof(base[0]); j++) 
    {
        datalen = base[j].length();
        // Research from length of based referrence key
        for (size_t c = 0; c + datalen < matchingID.length(); c++)
        {
            tmpRefKey=matchingID.substr(c, datalen);
            // bool rK1=tmpRefKey.length() - std::count(tmpRefKey.begin(), tmpRefKey.end(), '2') > 3;
            // bool rK2=tmpRefKey.length() - std::count(tmpRefKey.begin(), tmpRefKey.end(), '0') > 3;
            bool rK1=std::count(tmpRefKey.begin(), tmpRefKey.end(), (char)mostFrequent(tmpRefKey, tmpRefKey.length())) < 3;
            if ( rK1 )
            {
                arrlen=tmpRefKey.length() / 2;
                // Foreach change of value of the key 'matchingId' 
                // determine according to the size of the evaluation chain 'base'
                for (size_t i = 0; i < arrlen; i++)
                {
                    hex=base[j][i*2];
                    hex+=base[j][i*2+1];
                    mt=tmpRefKey[i*2];
                    mt+=tmpRefKey[i*2+1];

                    long r=strtol(mt.c_str (), NULL, 16);
                    long b=strtol(hex.c_str (), NULL, 16);
                    k=0;
                    while ( (r^k) <= b || abs((r^k) - b) >= b ) { k=1+k; }
                    r=r^k;
                    if (i + 1 < arrlen) kor+=std::to_string(k)+";"+std::to_string(-(b-r))+";";
                    else kor+=std::to_string(k)+";"+std::to_string(-(b-r));
                    weight+=abs(b - r);
                }
                if (resError == -1 || resError > weight) {
                    resError=weight;
                    resl=base[j];
                    xored=kor+","+std::to_string(c);
                }
                weight=0;
                kor="";
            }
        }
    }
    
    return std::to_string(resError) + "," + resl + "," + xored;
}

string entryMatch(string chaine, string match) {
    string matchingID(chaine);
    string hex, mt;
    long weight=0;
    long resError=-1;
    string resl="";
    int k=0;
    string xored="";
    string kor="";
    int i=0;
    int datalen=0;
    string tmpRefKey="";
    int arrlen=0;
    
    datalen = match.length();
    // Research from length of based referrence key
    for (size_t c = 0; c + datalen < matchingID.length(); c++)
    {
        tmpRefKey=matchingID.substr(c, datalen);
        // bool rK1=tmpRefKey.length() - std::count(tmpRefKey.begin(), tmpRefKey.end(), '2') > 3;
        // bool rK2=tmpRefKey.length() - std::count(tmpRefKey.begin(), tmpRefKey.end(), '0') > 3;
        bool rK1=std::count(tmpRefKey.begin(), tmpRefKey.end(), (char)mostFrequent(tmpRefKey, tmpRefKey.length())) < 3;
        if ( rK1 )
        {
            arrlen=tmpRefKey.length() / 2;
            // Foreach change of value of the key 'matchingId' 
            // determine according to the size of the evaluation chain 'base'
            for (size_t i = 0; i < arrlen; i++)
            {
                hex=match[i*2];
                hex+=match[i*2+1];
                mt=tmpRefKey[i*2];
                mt+=tmpRefKey[i*2+1];
                long r=strtol(mt.c_str (), NULL, 16);
                long b=strtol(hex.c_str (), NULL, 16);
                k=0;
                while ( (r^k) <= b || abs((r^k) - b) >= b ) { k=1+k; }
                r=r^k;
                if (i + 1 < arrlen) kor+=std::to_string(k)+";"+std::to_string(-(b-r))+";";
                else kor+=std::to_string(k)+";"+std::to_string(-(b-r));
                weight+=abs(b - r);
            }
            if (resError == -1 || resError > weight) {
                resError=weight;
                resl=match;
                xored=kor+","+std::to_string(c);
            }
            weight=0;
            kor="";
        }
    }
    
    return std::to_string(resError) + "," + resl + "," + xored;
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
    m.def("getEntryMatch", &entryMatch);
}

