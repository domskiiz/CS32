#include "provided.h"
#include <string>
#include <list>
#include <set>
#include <cctype>
#include <iostream>
using namespace std;

class TranslatorImpl
{
public:
    TranslatorImpl();
    bool pushMapping(string ciphertext, string plaintext);
    bool popMapping();
    string getTranslation(const string& ciphertext) const;
private:
    struct Mapping  // helper struct
    {
        Mapping()
        : plainAlphabet{ 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
                         'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                         'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
                         'Y', 'Z' },
        cipherAlphabet{ '?', '?', '?', '?', '?', '?', '?', '?',
                        '?', '?', '?', '?', '?', '?', '?', '?',
                        '?', '?', '?', '?', '?', '?', '?', '?',
                        '?', '?' }
        {}
        
        Mapping(char ca[26])
        : plainAlphabet{ 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
            'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
            'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
            'Y', 'Z' }
        {
            for (int i = 0; i < 26; i++)
                cipherAlphabet[i] = ca[i];
        }
        char plainAlphabet[26];
        char cipherAlphabet[26];
    };
    list<Mapping> m_mappings;
};

TranslatorImpl::TranslatorImpl()
: m_mappings(1)
{ }


// TODO: GET THIS TO RUN IN O(1) TIME
bool TranslatorImpl::pushMapping(string ciphertext, string plaintext)
{
    // Check if ciphertext + plaintext not same length
    if (ciphertext.length() != plaintext.length())
        return false;
    
    // Check if any of ciphertext already exists in mapping,
    // or plaintext doesn't already have something mapped to it
    set<char> ciphertextExists;
    for (int i = 0; i < 26; i++) {
        if (m_mappings.front().cipherAlphabet[i] != '?')
            ciphertextExists.insert(m_mappings.front().cipherAlphabet[i]);
    }
    for (int i = 0; i < plaintext.length(); i++) {
        if (ciphertextExists.count(ciphertext[i]) != 0)
            return false;
        int plaintextPos = plaintext[i] - 65;
        if (m_mappings.front().cipherAlphabet[plaintextPos] != '?')
            return false;
    }
    
    // Otherwise, make a new mapping and add it to the front of the list
    char newCipherAlphabet[26];
    for (int i = 0; i < 26; i++)
        newCipherAlphabet[i] = m_mappings.front().cipherAlphabet[i];
    for (int i = 0; i < ciphertext.length(); i++) {
        int plaintextPos = plaintext[i] - 65;
        newCipherAlphabet[plaintextPos] = ciphertext[i];
    }
    m_mappings.push_front(newCipherAlphabet);
    
    
// test
//    list<Mapping>::iterator it;
//    it = m_mappings.begin();
//    while (it != m_mappings.end()) {
//        for (int i = 0; i < 26; i++) {
//            cout << (*it).plainAlphabet[i] << (*it).cipherAlphabet[i] << endl;
//        }
//        it++;
//        cout << "=====" << endl;
//    }
    
    return true;
}

bool TranslatorImpl::popMapping()
{
    if (m_mappings.size() == 1)
        return false;
    m_mappings.pop_front();
    return true;
}

string TranslatorImpl::getTranslation(const string& ciphertext) const
{
    string translation;
    for (int i = 0; i < ciphertext.length(); i++) {
        // If it's a letter, look up its corresponding plaintext translation
        if (isalpha(ciphertext[i])) {
            bool isUpper = false;
            if (ciphertext[i] <= 90 && ciphertext[i] >= 65)
                isUpper = true;
            // Search for according plaintext translation
            bool isFound = false;
            for (int j = 0; j < 26; j++) {
                if (m_mappings.front().cipherAlphabet[j] == toupper(ciphertext[i])) {
                    isFound = true;
                    if (isUpper)
                        translation += m_mappings.front().plainAlphabet[j];
                    else
                        translation += tolower(m_mappings.front().plainAlphabet[j]);
                }
            }
            // If not there, add a questionmark
            if (!isFound)
                translation += '?';
        }
        // Leave non-alpha characters as is
        else
            translation += ciphertext[i];
    }
    return translation;
}

//******************** Translator functions ************************************

// These functions simply delegate to TranslatorImpl's functions.
// You probably don't want to change any of this code.

Translator::Translator()
{
    m_impl = new TranslatorImpl;
}

Translator::~Translator()
{
    delete m_impl;
}

bool Translator::pushMapping(string ciphertext, string plaintext)
{
    return m_impl->pushMapping(ciphertext, plaintext);
}

bool Translator::popMapping()
{
    return m_impl->popMapping();
}

string Translator::getTranslation(const string& ciphertext) const
{
    return m_impl->getTranslation(ciphertext);
}
