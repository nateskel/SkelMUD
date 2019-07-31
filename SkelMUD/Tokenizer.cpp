#include "Tokenizer.h"
#include "Utils.h"
#include <algorithm>
#include <sstream>

std::string Tokenizer::GetFirstToken(std::string &data, char delimiter, bool doPop) {
    std::string::size_type n = data.find(delimiter);
    std::string output;
    if (n != std::string::npos) {
        output = data.substr(0, n);
        if(doPop)
            data = data.substr(n + 1);
        return output;
    }
    else if (data != "") {
        output = data;
        data = "";
    }
    else
        output = "";
    Utils::RemoveEndline(output);
    return output;
}

std::string Tokenizer::GetFirstToken(std::string& data, bool doPop) {
    return GetFirstToken(data, ' ', doPop);
}


std::vector<std::string> Tokenizer::GetTokens(std::string &data, int number) {
    std::string result = GetFirstToken(data);
    std::vector<std::string> output;
    if (data != "" && number != 0) {
        output.push_back(result);
        number--;
        GetTokens(data, number);
    }
    return output;
}

std::vector<std::string> Tokenizer::GetAllTokens(std::string data) {
    std::vector<std::string> output;
    return GetAllTokens(data, ' ', output);
}

std::vector<std::string> Tokenizer::GetAllTokens(std::string data, char delimiter) {
    std::vector<std::string> output;
    return GetAllTokens(data, delimiter, output);
}

std::vector<std::string> Tokenizer::GetAllTokens(std::string data, char delimiter, std::vector<std::string> &output) {
    std::string result = GetFirstToken(data, delimiter, true);
    Utils::RemoveEndline(result);
    if (result != "")
        output.push_back(result);
    if (data != "")
        return GetAllTokens(data, delimiter, output);
    return output;
}

std::string Tokenizer::MakeString(std::vector<std::string> string_vector) {
    std::string output = "";
    if (string_vector.size() == 0)
        return output;
    int i = 0;
    for (i = 0; i < (int) string_vector.size() - 1; i++) {
        output.append(string_vector[i]);
        output.append(" ");
    }
    i++;
    output.append(string_vector[i]);
    return output;
}

std::string Tokenizer::UpperCase(std::string data) {
    std::transform(data.begin(), data.end(), data.begin(), ::toupper);
    return data;
}

std::string Tokenizer::LowerCase(std::string data) {
    std::transform(data.begin(), data.end(), data.begin(), ::tolower);
    return data;
}

std::string Tokenizer::GetFirstToken(std::string &data) {
    return GetFirstToken(data, true);
}

std::vector<int> Tokenizer::GetAllTokenIndexes(std::string data, std::vector<int> output, int index) {
    std::string result = GetFirstToken(data, true);
    Utils::RemoveEndline(result);
    if (result != "") {
        output.push_back(index);
        index += result.length();
    }
    if (data != "")
        return GetAllTokenIndexes(data, output, index);
    return output;
}

std::vector<int> Tokenizer::GetAllTokenIndexes(std::string data) {
    std::vector<int> output;
    return GetAllTokenIndexes(data, output, 0);
}

int Tokenizer::CountLines(std::string data) {
    if(data == "")
        return 0;
    int num = 0;
    std::stringstream ss;
    ss << data;
    std::string s;
    while(getline(ss, s)){
        ++num;
    }
    return num;

}
