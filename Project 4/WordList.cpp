#include "provided.h"
#include "MyHash.h"
#include <string>
#include <cctype>
#include <vector>
#include <functional>
#include <iostream>
#include <fstream>
using namespace std;

class WordListImpl
{
public:
    WordListImpl();
    bool loadWordList(string filename);
    bool contains(string word) const;
    vector<string> findCandidates(string cipherWord, string currTranslation) const;
private:
    MyHash<string, vector<string>> stringsToPatterns;
};

WordListImpl::WordListImpl()
{

}

bool WordListImpl::loadWordList(string filename)
{
    // Clear the hash table each time this is called
    stringsToPatterns.reset();
    
    // Find file and read in each string
    ifstream infile(filename);
    if ( ! infile ) {
        cerr << "Error: Cannot open " << filename << endl;
        return false;
    }
    string s;
    while (getline(infile, s)) {
        // Check if string should be inserted into hash table
        bool insertIntoMap = true;
        string lowerCaseWord = "";
        for (int i = 0; i < s.length(); i++) {
            if (!isalpha(s[i]) && s[i] != 39)
                insertIntoMap = false;
            else
                lowerCaseWord += tolower(s[i]);
        }
        if (insertIntoMap) {
            // Create letter pattern and store in hash table
            vector<string> words;
            string pattern;
            int count = 0;
            MyHash<char, char> storePattern;
            for (int i = 0; i < lowerCaseWord.length(); i++) {
                if (storePattern.find(lowerCaseWord[i]) == nullptr) {
                    storePattern.associate(lowerCaseWord[i], '0' + count);
                    pattern += *storePattern.find(lowerCaseWord[i]);
                    count++;
                }
                else
                    pattern += *storePattern.find(lowerCaseWord[i]);
            }
            if (stringsToPatterns.find(pattern) == nullptr) {
                words.push_back(lowerCaseWord);
                stringsToPatterns.associate(pattern, words);
            }
            else
                stringsToPatterns.find(pattern)->push_back(lowerCaseWord);
        }
    }
    return true;
}

bool WordListImpl::contains(string word) const
{
    string constCaseWord;
    for (int i = 0; i < word.length(); i++)
        constCaseWord += tolower(word[i]);
    if (stringsToPatterns.find(constCaseWord) == nullptr)
        return false;
    
    return true;
}

vector<string> WordListImpl::findCandidates(string cipherWord, string currTranslation) const
{
    string pattern;
    int count = 0;
    MyHash<char, char> storePattern;
    for (int i = 0; i < cipherWord.length(); i++) {
        if (storePattern.find(tolower(cipherWord[i])) == nullptr) {
            storePattern.associate(tolower(cipherWord[i]), '0' + count);
            pattern += *storePattern.find(tolower(cipherWord[i]));
            count++;
        }
        else
            pattern += *storePattern.find(tolower(cipherWord[i]));
    }
    vector<string> candidates;
    if (stringsToPatterns.find(pattern) == nullptr) {
        return candidates;
    }
    else {
        for (int i = 0; i < stringsToPatterns.find(pattern)->size(); i++)
            candidates.push_back(stringsToPatterns.find(pattern)->operator[](i));
    }
    return candidates;
}

//***** hash functions for string, int, and char *****

unsigned int hash(const std::string& s)
{
    return std::hash<std::string>()(s);
}

unsigned int hash(const int& i)
{
    return std::hash<int>()(i);
}

unsigned int hash(const char& c)
{
    return std::hash<char>()(c);
}

//******************** WordList functions ************************************

// These functions simply delegate to WordListImpl's functions.
// You probably don't want to change any of this code.

WordList::WordList()
{
    m_impl = new WordListImpl;
}

WordList::~WordList()
{
    delete m_impl;
}

bool WordList::loadWordList(string filename)
{
    return m_impl->loadWordList(filename);
}

bool WordList::contains(string word) const
{
    return m_impl->contains(word);
}

vector<string> WordList::findCandidates(string cipherWord, string currTranslation) const
{
   return m_impl->findCandidates(cipherWord, currTranslation);
}
