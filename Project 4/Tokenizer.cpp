#include "provided.h"
#include <string>
#include <vector>
#include <set>
using namespace std;

class TokenizerImpl
{
public:
    TokenizerImpl(string separators);
    vector<string> tokenize(const std::string& s) const;
private:
    set<char> m_tokens;
};

TokenizerImpl::TokenizerImpl(string separators)
{
    for (int i = 0; i < separators.size(); i++)
        m_tokens.insert(separators[i]);
}

vector<string> TokenizerImpl::tokenize(const std::string& s) const
{
    vector<string> tokenized;
    set<char>::iterator it;
    
    int pos = 0;
    string token;
    while (pos != s.size()) {
        it = m_tokens.find(s[pos]);
        if (it == m_tokens.end())
            token += s[pos];
        else {
            if (token != "")
                tokenized.push_back(token);
            token = "";
        }
        pos++;
    }
    if (token != "")
        tokenized.push_back(token);
    
    return tokenized;
}

//******************** Tokenizer functions ************************************

// These functions simply delegate to TokenizerImpl's functions.
// You probably don't want to change any of this code.

Tokenizer::Tokenizer(string separators)
{
    m_impl = new TokenizerImpl(separators);
}

Tokenizer::~Tokenizer()
{
    delete m_impl;
}

vector<string> Tokenizer::tokenize(const std::string& s) const
{
    return m_impl->tokenize(s);
}

