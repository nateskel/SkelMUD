#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <string>
#include <vector>

class Tokenizer {
public:
    static std::string GetFirstToken(std::string &data, char delimiter, bool doPop);

    static std::string GetFirstToken(std::string &data, bool doPop);

    static std::string GetFirstToken(std::string &data);

    static std::vector<std::string> GetTokens(std::string &data, int number);

    static std::vector<std::string> GetAllTokens(std::string data, char delimiter);

    static std::vector<std::string> GetAllTokens(std::string data);

    static std::vector<int> GetAllTokenIndexes(std::string data, std::vector<int> output, int index);

    static std::vector<int> GetAllTokenIndexes(std::string data);

    static std::string MakeString(std::vector<std::string> string_vector);

    static std::string UpperCase(std::string data);

    static std::string LowerCase(std::string data);

protected:
private:
    static std::vector<std::string> GetAllTokens(std::string data, char delimiter, std::vector<std::string> &output);
};

#endif // TOKENIZER_H
