#pragma once
#include <memory>
#include <vector>
#include "Tokens.hpp"
#include "Sequence.hpp"

using namespace Tokens;

//Reverse Polish notation
class RPN {
public:
	RPN() = delete;

	static void convertToRPN(Sequence& seq) {
		Sequence output;
		Sequence stack;

		if (!sequenceCheck(seq)) throw "wrong sequence";

		for (auto& token : seq)
		{
			auto type = token->getType();
			if (type == Type::NUMBER)
				output.push_back(std::move(token));
			else if (type == Type::U_OPERATOR || type == Type::L_PARENTHESIS)
				stack.push_back(std::move(token));
			else if (type == Type::R_PARENTHESIS) {
				while (stack.back()->getType() != Type::L_PARENTHESIS)
				{
					output.push_back(std::move(stack.back()));
					stack.pop_back();
				}
				stack.pop_back();
			}
			else if (type == Type::B_OPERATOR)
			{
				while (
					!stack.empty() && (
						stack.back()->getType() == Type::U_OPERATOR
						|| (stack.back()->getType() == Type::B_OPERATOR
							&& stack.back()->getPriority() >= token->getPriority())
						))
				{
					output.push_back(std::move(stack.back()));
					stack.pop_back();
				}
				stack.push_back(std::move(token));
			}
		}
		while (!stack.empty())
		{
			output.push_back(std::move(stack.back()));
			stack.pop_back();
		}

		seq = std::move(output);
	}

	static double calcRPN(const Sequence& rpn)
	{
		std::vector<Number> stack;

		for (const auto& token : rpn)
		{
			auto type = token->getType();
			if (type == Type::NUMBER)
				stack.push_back(((Number*)token.get())->getValue());
			if (type == Type::U_OPERATOR)
			{
				auto arg = stack.back();
				stack.pop_back();
				stack.push_back(((UOperator*)token.get())->calc(arg));
			}
			if (type == Type::B_OPERATOR)
			{
				auto arg2 = stack.back(); stack.pop_back();
				auto arg1 = stack.back(); stack.pop_back();
				stack.push_back(((BOperator*)token.get())->calc(arg1, arg2));
			}
		}

		return stack.back().getValue();
	}

	static bool sequenceCheck(const Sequence& seq) {
		int countParenthesis = 0;

		if (seq.size() == 0) return false;
		else if (seq.size() == 0) {
			if (seq.front()->getType() == Type::NUMBER) return true;
			else return false;
		}
		else if (seq.front()->getType() == Type::B_OPERATOR
			|| seq.front()->getType() == Type::R_PARENTHESIS) return false;
		else if (seq.back()->getType() == Type::B_OPERATOR
			|| seq.back()->getType() == Type::L_PARENTHESIS) return false;
		else
		{
			if (seq.front()->getType() == Type::L_PARENTHESIS) countParenthesis++;
			auto prev = seq.front()->getType();
			auto t = prev;
			for (size_t i = 1; i < seq.size(); i++)
			{
				t = seq.at(i)->getType();
				switch (t)
				{
				case Type::L_PARENTHESIS:
					countParenthesis++;
				case Type::NUMBER:
				case Type::U_OPERATOR:
					if (prev == Type::R_PARENTHESIS || prev == Type::NUMBER) return false; break;
				case Type::R_PARENTHESIS:
					countParenthesis--;
				case Type::B_OPERATOR:
					if (prev != Type::R_PARENTHESIS && prev != Type::NUMBER) return false; break;
				}
				prev = t;
				if (countParenthesis < 0) return false;
			}
		}
		if (countParenthesis != 0) return false;
		return true;
	}
};