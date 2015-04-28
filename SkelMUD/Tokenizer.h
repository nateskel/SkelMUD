#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <string>
#include <vector>

class Tokenizer {
public:
    static std::string GetFirstToken(std::string &data);

    static std::vector<std::string> GetTokens(std::string &data, int number);

    static std::vector<std::string> GetAllTokens(std::string data);

    static std::string MakeString(std::vector<std::string> string_vector);

    static std::string UpperCase(std::string data);

    static std::string LowerCase(std::string data);

protected:
private:
    static std::vector<std::string> GetAllTokens(std::string data, std::vector<std::string> &output);
};

#endif // TOKENIZER_H
