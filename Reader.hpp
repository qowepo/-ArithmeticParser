#pragma once
#include <memory>
#include <vector>
#include <string>
#include <unordered_map>
#include "Tokens.hpp"
#include "Sequence.hpp"

using namespace Tokens;

class Reader
{
public:
	Reader() = delete;

	static Sequence Read(std::string s) {

		Sequence seq;

		for (size_t i = 0; i < s.length(); i++)
		{
			if (isDigit(s[i]) || s[i] == '.' || s[i] == ',') exploreNumber(s, seq, i);
			else if (s[i] == '(' || s[i] == ')') exploreParenthesis(s, seq, i);
			else if (s[i] != ' ') exploreOperator(s, seq, i);
		}

		return seq;
	}

private:

	static void exploreParenthesis(std::string& s, Sequence& seq, size_t& i)
	{
		std::string temp;
		temp += s[i];
		seq.push_back(parentheses.at(temp)->clone());
	}

	static void exploreNumber(std::string& s, Sequence& seq, size_t& i) {
		std::string temp;
		bool point = false;

		if (s[i] == '.' || s[i] == ',') { point = true; temp += "."; }
		else temp += s[i];

		while (i + 1 < s.length() && (isDigit(s[i + 1]) || s[i + 1] == '.' || s[i + 1] == ','))
		{
			if (s[i + 1] == '.' || s[i + 1] == ',')
				if (point) throw "incorrect input"; else { point = true; temp += "."; }
			else temp += s[i + 1];
			i++;
		}

		if (temp == ".") throw "incorrect input";
		seq.pushNumber(std::stod(temp));
	}

	static void exploreOperator(std::string& s, Sequence& seq, size_t& i) {
		std::string temp;
		bool detected = false;

		do {
			temp += s[i];
			i++;
			detected = detectOperator(temp);
		} while (
			i < s.length()
			&& s[i] != ' ' && s[i] != '.' && s[i] != ',' && s[i] != '(' && s[i] != ')'
			&& !isDigit(s[i])
			&& !detected
			&& temp.length() <= operatorMaxLength
			);
		i--;

		if (detected) {
			if (temp == "-") {
				if (i == 0) temp = "-u";
				else if (!(seq.back()->getType() == Type::NUMBER || seq.back()->getType() == Type::R_PARENTHESIS))
					temp = "-u";
			}
			seq.push_back(supportedOperators.at(temp)->clone());
		}

		else throw "incorrect input";
	}

	static bool detectOperator(std::string s) {
		return supportedOperators.find(s) != supportedOperators.end();
	}

	static bool isDigit(int C) { if (C >= -1 && C <= 255) return isdigit(C); return false; }
};