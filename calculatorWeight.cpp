#include <iostream>
#include <string>
#include <fstream>
#include <pybind11/pybind11.h>
#include <bits/stdc++.h>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cstring>
#include <regex>
#include <set>
#include <omp.h>
using namespace std;
namespace py = pybind11;

string base[] = {
    "4f4b", "6d73", "6d56", "636d", "6d41", "6d41", "467a", "437a", "507a",
    "6d2f73", "48503a", "4c503a", "454446", "726164", "646567", "524154",
    "55415254", "69646c65", "7374617274", "73746f70",
    "3030303030", "674e552f4c", "484f53543a",
    "3030303031", "627269646765",
    "30303030316b",
    "30316d696c6c696f6e",
    "30316d696c6c696f6e73",
    "30326d696c6c696f6e",
    "30326d696c6c696f6e73",
    "30687a"
};
// MQTT PAYLOAD

string btf[] = {
    "elect", "stim", "delar", "AgAgCl", "mario", "router"
};


void tokenize(std::string const &str, const char delim,
    std::vector<std::string> &out)
{
    std::stringstream ss(str);
    std::string s;
    while (std::getline(ss, s, delim)) {
        out.push_back(s);
    }
}

size_t remove_bad_utf8(char* s, size_t len) {
  char* in = s;
  /* Skip over the initial correct sequence. Avoid relying on mbtowc returning
   * zero if n is 0, since Posix is not clear whether mbtowc returns 0 or -1.
   */
  int seqlen;
  while (len && (seqlen = mbtowc(NULL, in, len)) > 0) { len -= seqlen; in += seqlen; }
  char* out = in;

  if (len && seqlen < 0) {
    ++in;
    --len;
    /* If we find an invalid sequence, we need to start shifting correct sequences.  */
    for (; len; in += seqlen, len -= seqlen) {
      seqlen = mbtowc(NULL, in, len);
      if (seqlen > 0) {
        /* Shift the valid sequence (if one was found) */
        memmove(out, in, seqlen);
        out += seqlen;
      }
      else if (seqlen < 0) seqlen = 1;
      else /* (seqlen == 0) */ break;
    }
    *out++ = 0;
  }
  return out - s;
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

// Pre-compile the regular expression used to split the data into words
std::regex word_regex("[^\\s]+");
std::regex json_regex("\\{(\"|\')[A-Za-z0-9_ ]*(\"|\'):(\"|\')[A-Za-z0-9_ ]*(\"|\'),*[^}]*\\}");
//std::string csv = R"(\\w+)(\\.)?(\\w+)@[\\w+]+(\\.(\\w+))+";
std::regex csv_regex("((,)+(\\d+|\\w+)){2,}");

std::string btfMain3(std::string id, std::string dictAttack, std::string frenchWord)
{
    // Read the dictionary words from the file and store them in a hash set
    std::unordered_set<std::string> dictionary;
    std::ifstream dictionary_file(dictAttack);
    std::string line;
    while (std::getline(dictionary_file, line))
    {
        dictionary.insert(line);
    }
    dictionary_file.close();

    std::vector<std::string> words;

    // Read the data from the binary file and split it into a list of words
    std::ifstream file(frenchWord, std::ios::binary);
    std::stringstream data;
    data << file.rdbuf();
    file.close();

    std::vector<std::string> FRENdict;
    std::string word;
    while (std::getline(data, word, ' '))
    {
        FRENdict.push_back(word);
    }

    // Create a lookup table for the XOR operations
    const int lookup_size = 256;
    int lookup[lookup_size][lookup_size];
    for (int i = 0; i < lookup_size; i++)
    {
        for (int j = 0; j < lookup_size; j++)
        {
            lookup[i][j] = i ^ j;
        }
    }

    for (const std::string &word : dictionary)
    {
        std::string resl;
        for (size_t i = 0; i < (id.length() - 1) / 2; i++)
        {
            int remainK = word[(i * 2) % word.length()];
            remainK = lookup[remainK][word[(i * 2 + 1) % word.length()]];
            int remainP = id[i * 2];
            remainP = lookup[remainP][id[i * 2 + 1]];
            resl += remainK ^ remainP;
        }
        std::transform(resl.begin(), resl.end(), resl.begin(), ::tolower);
        char arr[resl.length() + 1];
        strcpy(arr, resl.c_str());
        remove_bad_utf8(arr, sizeof(arr) / sizeof(arr[0]));
        resl = arr;
        words.push_back(resl);
    }

    std::string output;
    std::vector<std::string> vector;
    for (const std::string &word : words)
    {
        for (const std::string &dt : FRENdict)
        {
            if ((int)word.find(dt) > -1 and dt.length() > 3)
            {
                bool isInVect=false;
                isInVect = std::find(vector.begin(), vector.end(), dt) != vector.end();
                        
                if (isInVect == false)
                {
                    output+=dt+ " ";
                }
                vector.push_back(dt);
            }
        }
    }

    return output;
}

string btfMain(string id, string dictAttack, string frenchWord)
{
    std::unordered_set<std::string> dictionary;
    std::ifstream dictionary_file(dictAttack);
    std::string line;
    while (std::getline(dictionary_file, line))
    {
        dictionary.insert(line);
    }
    dictionary_file.close();

    // Open the binary file for reading
    std::ifstream file(frenchWord, std::ios::binary);
    
    // Read the data from the file
    std::string data((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    
    // Close the file
    file.close();

    std::sregex_iterator iterator(data.begin(), data.end(), word_regex);
    std::sregex_iterator end;
    std::vector<std::string> FRENdict;

    while (iterator != end)
    {
        FRENdict.push_back(iterator->str());
        ++iterator;
    }

    
    // Create a lookup table for the XOR operations
    const int lookup_size = 256;
    int lookup[lookup_size][lookup_size];
    for (int i = 0; i < lookup_size; i++)
    {
        for (int j = 0; j < lookup_size; j++)
        {
            lookup[i][j] = i ^ j;
        }
    }
    
    std::vector<std::string> words;

    for (const std::string &word : dictionary)
    {
        std::string resl;
        for (size_t i = 0; i < (id.length() - 1) / 2; i++)
        {
            int remainK = word[(i * 2) % word.length()];
            remainK = lookup[remainK][word[(i * 2 + 1) % word.length()]];
            int remainP = id[i * 2];
            remainP = lookup[remainP][id[i * 2 + 1]];
            resl += remainK ^ remainP;
        }
        std::transform(resl.begin(), resl.end(), resl.begin(), ::tolower);
        char arr[resl.length() + 1];
        strcpy(arr, resl.c_str());
        remove_bad_utf8(arr, sizeof(arr) / sizeof(arr[0]));
        resl = arr;
        words.push_back(resl);
    }
    std::regex r("([0-9a-zA-Z]+(,|)){2,}"); // "\\{[^}]+\\}"


    std::string output;
    std::vector<std::string> vector;
    for (const std::string &word : words)
    {
        for (const std::string &dt : FRENdict)
        {
            if ((int)word.find(dt) > -1 and dt.length() > 3)
            {
                bool isInVect=false;
                isInVect = std::find(vector.begin(), vector.end(), dt) != vector.end();
                        
                if (isInVect == false)
                {
                    output+=dt+ " ";
                }
                vector.push_back(dt);
            }
        }
    }

    return output;
}

string btfMain2(string id, string dict, string frenchWord)
{
    string matchingID(id); // "7f26374e1e5542b38fb825634c3daf47"
    string password;
    ifstream myfile (dict);
    string resl="";
    string remainP;
    string remainK;
    string finalStr="";

    if (myfile.is_open())
    {
        try {
            while ( getline (myfile,password) ) {
                // On definit des chaines de caractere de meme taill
                while (password.length()!=matchingID.length() and password.length() and matchingID.length()) {
                    if (password.length()>matchingID.length()) { password=password.substr(0, matchingID.length() ); }
                    else password = password + password;
                }

                resl="";
                for (size_t i = 0; i < (matchingID.length() -1) / 2; i++)
                {
                    remainK=password[i*2];
                    remainK+=password[i*2+1];
                    remainP=matchingID[i*2];
                    remainP+=matchingID[i*2+1];
                    resl+=(abs(strtol(remainK.c_str (), NULL, 16) ^ strtol(remainP.c_str (), NULL, 16) + 0));
                }
                // clean workspace RM unlnown 
                transform(resl.begin(), resl.end(), resl.begin(), ::tolower);
                char arr[resl.length()+1];
                strcpy(arr, resl.c_str());
                remove_bad_utf8(arr, sizeof(arr)/sizeof(arr[0]));
                resl=arr;
                std::regex r("\\{[^}]*\\}");
                std::smatch result;
                for (size_t j = 0; j < sizeof(btf)/sizeof(btf[0]); j++) 
                {
                    if ( (int) resl.find(btf[j]) > -1 ) {
                        if (std::regex_search(resl, result, r)) {
                            cout << result.str() << endl;
                            finalStr+=result.str();
                        }
                    }
                }
                /*
                ----------------- GET SENTENCE USING SPACING ---------------
                const char delim = ' ';
                std::vector<std::string> out;
                tokenize(resl, delim, out);
                for (size_t j = 0; j < sizeof(btf)/sizeof(btf[0]); j++) 
                {
                    if ( (int) resl.find(btf[j]) > -1 ) {
                        for (auto &s: out) {
                            string tmp=getMatchingCase(s, frenchWord);
                            if (tmp.length()>16) {
                                finalStr+=tmp;
                                finalStr+=";";
                            }
                        }
                    }
                }*/
            }
            myfile.close();
        } catch (const std::exception& e) {
            cerr << e.what();
            //myfile.close();
        }
        return finalStr;
    }
    return "";
}

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
            bool rK1=std::count(tmpRefKey.begin(), tmpRefKey.end(), (char)mostFrequent(tmpRefKey, tmpRefKey.length()));
            
            if ( rK1 < 3 )
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

int main() {
    return 0;
}

string getProximmityCase(string id, string dictAttack)
{
    string matchingID(id);
    long weight=0;
    long totalWeight=-1;
    long startWeight=0;
    long tmpBestStartWeight=0;
    string resl="";

    // Read the dictionary file and store the words in a set
	std::ifstream dictionaryFile(dictAttack);
	std::string dictionaryData((std::istreambuf_iterator<char>(dictionaryFile)), std::istreambuf_iterator<char>());
	dictionaryFile.close();
	//transform
	std::sregex_iterator iterator(dictionaryData.begin(), dictionaryData.end(), word_regex);
	std::sregex_iterator end;
	std::set<std::string> dictionary;
#pragma omp parallel for
	for (iterator; iterator != end; ++iterator)
	{
		dictionary.insert(iterator->str());
	}

    for (const std::string &tmp : dictionary)
	{
        string hex, mt;
        
        for (size_t i = 0; i < matchingID.length() / 2; i++)
        {
            hex=tmp[i*2];
            hex+=tmp[i*2+1];
            mt=matchingID[i*2];
            mt+=matchingID[i*2+1];
            
            weight+=abs(strtol(hex.c_str (), NULL, 16) - strtol(mt.c_str (), NULL, 16) + 0);
            if (i==0) startWeight=weight;
        }
        if (matchingID.length() == tmp.length()) startWeight=10000-startWeight;
        if (totalWeight == -1 || (totalWeight > weight && tmpBestStartWeight > startWeight)) {
            totalWeight=weight;
            resl=tmp;
            tmpBestStartWeight=startWeight;
        }
        weight=0;
    }
    
    return "ID: " + resl + ", Error: " + std::to_string(totalWeight) + ",";
}

// Function to remove bad UTF-8 characters from a string
void rm_bad_utf8(char *s, size_t len)
{
	char *src, *dst;
#pragma omp parallel for
	for (src = dst = s; len; src++, len--)
	{
		if ((*src & 0x80) == 0)
		{
			*dst++ = *src;
		}
		else if ((*src & 0xe0) == 0xc0)
		{
			if (len < 2 || (src[1] & 0xc0) != 0x80)
			{
				break;
			}
			*dst++ = *src;
			*dst++ = src[1];
			src++;
			len--;
		}
		else if ((*src & 0xf0) == 0xe0)
		{
			if (len < 3 || (src[1] & 0xc0) != 0x80 || (src[2] & 0xc0) != 0x80)
			{
				break;
			}
			*dst++ = *src;
			*dst++ = src[1];
			*dst++ = src[2];
			src += 2;
			len -= 2;
		}
		else if ((*src & 0xf8) == 0xf0)
		{
			if (len < 4 || (src[1] & 0xc0) != 0x80 || (src[2] & 0xc0) != 0x80 || (src[3] & 0xc0) != 0x80)
			{
				break;
			}
			*dst++ = *src;
			*dst++ = src[1];
			*dst++ = src[2];
			*dst++ = src[3];
			src += 3;
			len -= 3;
		}
		else
		{
			break;
		}
	}
	*dst = '\0';
}

std::string xorBTF(std::string id, std::string dictAttack, std::string frenchWord)
{
	// Read the dictionary file and store the words in a set
	std::ifstream dictionaryFile(dictAttack);
	std::string dictionaryData((std::istreambuf_iterator<char>(dictionaryFile)), std::istreambuf_iterator<char>());
	dictionaryFile.close();
	//transform
	std::sregex_iterator iterator(dictionaryData.begin(), dictionaryData.end(), word_regex);
	std::sregex_iterator end;
	std::set<std::string> dictionary;
#pragma omp parallel for
	for (iterator; iterator != end; ++iterator)
	{
		dictionary.insert(iterator->str());
	}

	// Read the binary file and store the words in a vector
	std::ifstream file(frenchWord, std::ios::binary);
	std::string data((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	file.close();
	iterator = std::sregex_iterator(data.begin(), data.end(), word_regex);
	std::vector<std::string> words;
	std::vector<std::string> FRENdict;
#pragma omp parallel for
	for (iterator; iterator != end; ++iterator)
	{
		FRENdict.push_back(iterator->str());
	}

    std::ofstream outFile("output.bin", std::ios::binary);
#pragma omp parallel for
	for (const std::string &word : dictionary)
	{
		std::string resl = "";
#pragma omp parallel for
		for (size_t i = 0; i < (id.length() - 1) / 2; i++)
		{
			std::string remainK;
			remainK = word[(i * 2) % word.length()];
			remainK += word[(i * 2 + 1) % word.length()];
			std::string remainP;
			remainP = id[i * 2];
			remainP += id[i * 2 + 1];
			resl += (strtol(remainK.c_str(), NULL, 16) ^ strtol(remainP.c_str(), NULL, 16));
		}
		transform(resl.begin(), resl.end(), resl.begin(), ::tolower);
        std::string dt;
        std::smatch result;
        for(const std::string &f : btf)
        {
            dt=f;
            transform(dt.begin(), dt.end(), dt.begin(), ::tolower);
            if (std::regex_search(resl, result, csv_regex)) {
                outFile << resl << endl;
            }
        }
        /*for (const std::string &dict : FRENdict)
	    {
            dt=dict;
            transform(dt.begin(), dt.end(), dt.begin(), ::tolower);
            if ((int)resl.find(dt) > -1 and dt.length() > 3)
            {
                outFile << dt << "   " << word << "   " << resl << endl;
                
            }
        }
        std::smatch result;
        if (std::regex_search(resl, result, csv_regex)) {
            cout << result.str() << endl;
        }*/
	}
    outFile.close();
    std::string outp = "";
	return outp;
}

PYBIND11_MODULE(calcWeight, m)
{
    m.def("getMatchingCase", &getProximmityCase);
    m.def("getEntry", &entry);
    m.def("getEntryMatch", &entryMatch);
    m.def("btfMain", &btfMain);
    m.def("xorBTF", &xorBTF);
}

