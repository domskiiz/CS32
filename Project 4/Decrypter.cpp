#include "provided.h"
#include "MyHash.h"
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>

using namespace std;

class DecrypterImpl
{
public:
    DecrypterImpl();
    bool load(string filename);
    vector<string> crack(const string& ciphertext);
private:
    WordList                       m_wordlist;
    Translator                     m_translator;
    Tokenizer                      m_tokenizer;
    vector<string>                 possibleTranslations;

};

DecrypterImpl::DecrypterImpl()
:m_tokenizer(" ,;:.!()[]{}-\"#$%^&1234567890")
{ }

bool DecrypterImpl::load(string filename)
{
    if (m_wordlist.loadWordList(filename))
        return true;
    return false;
}

vector<string> DecrypterImpl::crack(const string& ciphertext)
{
    // STEP 2: Tokenize ciphertext
    vector<string> cipherTokens, translatedTokens;
    cipherTokens = m_tokenizer.tokenize(ciphertext);
    for (auto it = cipherTokens.begin(); it != cipherTokens.end(); it++) {
        string baseTranslation;
        baseTranslation = m_translator.getTranslation(*it);
        cout << "translation in recursion: " << baseTranslation << endl;
        translatedTokens.push_back(baseTranslation);
    }
    
    // Put all words in priority queue based on how many untranslated characters there are
    MyHash<int, vector<string>> priorityQueue;
    int maxLength = 0;
    for (int i = 0; i < translatedTokens.size(); i++) {
        int countQMarks = 0;
        for (int j = 0; j < translatedTokens[i][j]; j++) {
            if (translatedTokens[i][j] == '?')
                countQMarks++;
        }
        if (countQMarks > maxLength)
            maxLength = countQMarks;
        vector<string> tokens;
        if (priorityQueue.find(countQMarks) != nullptr)
            (*priorityQueue.find(countQMarks)).push_back(cipherTokens[i]);
        else {
            tokens.push_back(cipherTokens[i]);
            priorityQueue.associate(countQMarks, tokens);
        }
    }
    
    // STEP 3: Get cipher with highest priority
    for (int i = maxLength; i > 0; i--) {
        if (priorityQueue.find(i) != nullptr) {
            vector<string> cipherWords = *(priorityQueue.find(i));
            
            // Iterate through cipher words with that priority
            for (auto it = cipherWords.begin(); it != cipherWords.end(); it++) {
                string cipherWord = *it;
                cout << "cipherWord " << *it << endl;
                string translatedWord = m_translator.getTranslation(cipherWord);
                cout << "translation of cipherWord: " << translatedWord << endl;
                
                // STEP 4: CREATE COLLECTION OF CANDIDATES
                vector<string> candidates;
                candidates = m_wordlist.findCandidates(cipherWord, translatedWord);
                
                // STEP 5: IF NO CANDIDATES THROW OUT MAPPING AND RETURN
                if (candidates.size() == 0) {
                    cout << "THERE WERE NO CANDIDATES " << endl;
                    m_translator.popMapping();
                    return possibleTranslations;
                }
                
                cout << "candidates for word " << cipherWord << ": ";
                for (auto c = candidates.begin(); c != candidates.end(); c++) {
                    cout << *c << ", ";
                }
                cout << endl;
                
                // STEP 6: FOR EACH CANDIDATE IN CANDIDATES
                for (auto c = candidates.begin(); c != candidates.end(); c++) {
                    cout << "candidate: " << *c << endl;
                    
                    // STEP 6A: Create mapping based on candidate
                    string cipherMapping, translatedMapping;
                    for (int i = 0; i < cipherWord.length(); i++) {
                        stringstream ss, bb;
                        string cipherWordCharAsString, translatedCharAsString;
                        ss << cipherWord[i];
                        ss >> cipherWordCharAsString;
                        char s = tolower(m_translator.getTranslation(cipherWordCharAsString)[0]);
                        // If mapping already exists for that letter, do nothing
                        if (s == tolower((*c)[i])) {
                            continue;
                        }
                        translatedMapping += tolower((*c)[i]);
                        cipherMapping += tolower(cipherWord[i]);
                        cout << "MAPPINGS: " << translatedMapping << " " << cipherMapping << endl;
                    }
                    if (m_translator.pushMapping(cipherMapping, translatedMapping)) {
                        cout << "Mapping pushed" << endl;
                    } else {
                        // If mapping won't insert
                        cout << "This word wont work" << endl;
                        continue;
                    }
                    
                    // STEP 6B: TRANSLATE ENTIRE MESSAGE USING NEW MAPPING
                    string partiallyTranslated;
                    partiallyTranslated = m_translator.getTranslation(ciphertext);
                    cout << partiallyTranslated << endl;
                    vector<string> partiallyTranslatedTokens = m_tokenizer.tokenize(partiallyTranslated);
                    
                    // STEP 6C: CHECK IF ALL FULLY TRANSLATED WORDS EXIST IN WORD LIST
                    vector<int> fullyTranslatedIndexes;
                    // Get indexes of fully translated words
                    for (auto i = 0; i != partiallyTranslatedTokens.size(); i++) {
                        bool fullyTranslated = true;
                        for (int j = 0; j < partiallyTranslatedTokens[i].length(); j++) {
                            if (partiallyTranslatedTokens[i][j] == '?')
                                fullyTranslated = false;
                        }
                        if (fullyTranslated) {
                            cout << partiallyTranslatedTokens[i] << " fully translated" << endl;
                            fullyTranslatedIndexes.push_back(i);
                        }
                    }
                    // Check if any fully translated words don't exist in word list
                    bool throwAwayCandidate = false;
                    for (int i = 0; i < fullyTranslatedIndexes.size(); i++) {
                        int index = fullyTranslatedIndexes[i];
                        if (!m_wordlist.contains(partiallyTranslatedTokens[index])) {
                            cout << partiallyTranslatedTokens[index] << " word not in list" << endl;
                            m_translator.popMapping();
                            throwAwayCandidate = true;
                            break;
                        }
                    }
                    if (throwAwayCandidate)
                        continue;
                    else if (fullyTranslatedIndexes.size() == cipherTokens.size()) {
                        cout << "HERE!!!" << endl;
                        cout << partiallyTranslated << endl;
                        possibleTranslations.push_back(partiallyTranslated);
                        m_translator.popMapping();
                        continue;
                    }
                    else
                        crack(ciphertext);
                }
                m_translator.popMapping();
                return possibleTranslations;
            }
        }
    }
    sort(possibleTranslations.begin(), possibleTranslations.end());
    return possibleTranslations;

}

//******************** Decrypter functions ************************************

// These functions simply delegate to DecrypterImpl's functions.
// You probably don't want to change any of this code.

Decrypter::Decrypter()
{
    m_impl = new DecrypterImpl;
}

Decrypter::~Decrypter()
{
    delete m_impl;
}

bool Decrypter::load(string filename)
{
    return m_impl->load(filename);
}

vector<string> Decrypter::crack(const string& ciphertext)
{
   return m_impl->crack(ciphertext);
}
