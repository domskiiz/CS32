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
    bool loadWordList(string filename);
    bool contains(string word) const;
    vector<string> findCandidates(string cipherWord, string currTranslation) const;
private:
    MyHash<string, vector<string>> stringsToPatterns;
    string getLetterPattern(string word) const;

};

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
            string pattern = getLetterPattern(lowerCaseWord);
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
    string pattern = getLetterPattern(word);
    if (stringsToPatterns.find(pattern) == nullptr)
        return false;
    vector<string> candidates = *stringsToPatterns.find(pattern);
    for (auto it = candidates.begin(); it != candidates.end(); it++)
        if (*it == word)
            return true;
    return false;
}

vector<string> WordListImpl::findCandidates(string cipherWord, string currTranslation) const
{
    // Create an empty vector and a vector of candidate strings
    vector<string> candidates;
    vector<string> empty;
    
    // Check that translation + cipher are same length
    if (cipherWord.length() != currTranslation.length())
        return empty;
    // Check that cipherword and translation only have letters, apostrophes, and question marks
    for (int i = 0; i < cipherWord.length(); i++) {
        if (!isalpha(cipherWord[i]) && cipherWord[i] != 39)
            return empty;
        if (!isalpha(currTranslation[i]) && currTranslation[i] != 39 && currTranslation[i] != '?')
            return empty;
    }
    // Get letter pattern for cipher word
    string pattern = getLetterPattern(cipherWord);

    // Return strings in word list with matching letter patterns
    if (stringsToPatterns.find(pattern) == nullptr)
        return empty;
    else {
        // Parse through each of the i strings returned in vector of possible words
        // that map to that letter pattern
        for (int i = 0; i < stringsToPatterns.find(pattern)->size(); i++) {
            bool addCandidate = true;
            string candidate = stringsToPatterns.find(pattern)->operator[](i);
            // Rule out possible strings not consistent with current translation
            for (int j = 0; j < candidate.length(); j++) {
                if (isalpha(currTranslation[j])) {
                    if (tolower(currTranslation[j]) != tolower(candidate[j]))
                        addCandidate = false;
                    if (!(isalpha(cipherWord[j]))) {
                        return empty;
                    }
                }
                if (currTranslation[j] == '?') {
                    if (!isalpha(cipherWord[j])) {
                        return empty;
                    }
                }
                if (currTranslation[j] == 39) {
                    if (cipherWord[j] != 39 || candidate[j] != 39) {
                        return empty;
                    }
                }
            }
            if (addCandidate) {
                candidates.push_back(candidate);
            }
        }
    }
    // Return the vector of any strings that could be the translation of cipherWord
    // consistent with the current translation
    return candidates;
}

string WordListImpl::getLetterPattern(string word) const
{
    string pattern;
    int count = 0;
    MyHash<char, char> storePattern;
    for (int i = 0; i < word.length(); i++) {
        if (storePattern.find(tolower(word[i])) == nullptr) {
            storePattern.associate(tolower(word[i]), '0' + count);
            pattern += *storePattern.find(tolower(word[i]));
            count++;
        }
        else
            pattern += *storePattern.find(tolower(word[i]));
    }
    return pattern;
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
