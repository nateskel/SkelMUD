#include "Tokenizer.h"
#include <algorithm>

std::string Tokenizer::GetFirstToken(std::string &data)
{
	std::string::size_type n = data.find(' ');
	std::string output;
	if (n != std::string::npos)
	{
		output = data.substr(0, n);
		data = data.substr(n + 1);
		return output;
	}
	else if (data != "")
	{
		output = data;
		data = "";
	}
	else
		output = "";
	return output;
}

std::vector<std::string> Tokenizer::GetTokens(std::string &data, int number)
{
	std::string result = GetFirstToken(data);
	std::vector<std::string> output;
	if (data != "" && number != 0)
	{
		output.push_back(result);
		number--;
		GetTokens(data, number);
	}
	return output;

}

std::vector<std::string> Tokenizer::GetAllTokens(std::string data)
{
	std::vector<std::string> output;
	return GetAllTokens(data, output);
}

std::vector<std::string> Tokenizer::GetAllTokens(std::string data, std::vector<std::string> &output)
{
	std::string result = GetFirstToken(data);
	if (result != "")
		output.push_back(result);
	if (data != "")
		return GetAllTokens(data, output);
	return output;
}

std::string Tokenizer::MakeString(std::vector<std::string> string_vector)
{
	std::string output = "";
	if (string_vector.size() == 0)
		return output;
	int i = 0;
	for (i = 0; i < (int)string_vector.size() - 1; i++)
	{
		output.append(string_vector[i]);
		output.append(" ");
	}
	i++;
	output.append(string_vector[i]);
	return output;
}

std::string Tokenizer::UpperCase(std::string data)
{
	std::transform(data.begin(), data.end(), data.begin(), ::toupper);
	return data;
}

std::string Tokenizer::LowerCase(std::string data)
{
	std::transform(data.begin(), data.end(), data.begin(), ::tolower);
	return data;
}