#pragma once
#include <memory>
#include <cmath>
#include <unordered_map>
#include <sstream>

namespace Tokens
{
	class Token;
	using TokenMap = std::unordered_map<std::string, std::unique_ptr<Token>>;

	enum class Type : int
	{
		NUMBER,
		U_OPERATOR,
		B_OPERATOR,
		L_PARENTHESIS,
		R_PARENTHESIS
	};

	const int operatorMaxLength = 4;

	class Token {
	public:
		Token(Type type) { this->type = type; }
		Type getType() const { return type; }
		virtual std::string toString() { return std::string("#"); }
		virtual int getPriority() const { return 0; }
		virtual std::unique_ptr<Token> clone() { return nullptr; }

	private:
		Type type;
	};

	class LParenthesis : public Token {
	public:
		LParenthesis() : Token(Type::L_PARENTHESIS) {}
		std::string toString() override { return std::string("("); }
		std::unique_ptr<Token> clone() override { return std::make_unique<LParenthesis>(); }
	};

	class RParenthesis : public Token {
	public:
		RParenthesis() : Token(Type::R_PARENTHESIS) {}
		std::string toString() override { return std::string(")"); }
		std::unique_ptr<Token> clone() override { return std::make_unique<RParenthesis>(); }
	};

	class Number : public Token {
	public:
		Number(double value) : Token(Type::NUMBER) { this->value = value; }

		double getValue() const { return value; }
		std::string toString() override
		{
			std::ostringstream o;
			o << value;
			return o.str();
		}
		std::unique_ptr<Token> clone() override { return std::move(std::make_unique<Number>(getValue())); }

	private:
		double value;
	};

	class UOperator : public Token {
	public:
		UOperator() : Token(Type::U_OPERATOR) {}
		virtual Number calc(const Number& A) const { return Number(0); }
	};

	class BOperator : public Token {
	public:
		BOperator(int priority) : Token(Type::B_OPERATOR) { this->priority = priority; }
		virtual Number calc(const Number& A, const Number& B) const { return Number(0); }
		int getPriority() const override { return priority; }

	private:
		int priority;
	};

	class Plus : public BOperator {
	public:
		Plus() : BOperator(1) {}
		Number calc(const Number& A, const Number& B) const override
		{
			return Number(A.getValue() + B.getValue());
		}
		std::string toString() override { return std::string("+"); }
		std::unique_ptr<Token> clone() override { return std::make_unique<Plus>(); }
	};

	class Minus : public BOperator {
	public:
		Minus() : BOperator(1) {}
		Number calc(const Number& A, const Number& B) const override
		{
			return Number(A.getValue() - B.getValue());
		}
		std::string toString() override { return std::string("-"); }
		std::unique_ptr<Token> clone() override { return std::make_unique<Minus>(); }
	};

	class Multiplication : public BOperator {
	public:
		Multiplication() : BOperator(2) {}
		Number calc(const Number& A, const Number& B) const override
		{
			return Number(A.getValue() * B.getValue());
		}
		std::string toString() override { return std::string("*"); }
		std::unique_ptr<Token> clone() override { return std::make_unique<Multiplication>(); }
	};

	class Division : public BOperator {
	public:
		Division() : BOperator(2) {}
		Number calc(const Number& A, const Number& B) const override
		{
			return Number(A.getValue() / B.getValue());
		}
		std::string toString() override { return std::string("/"); }
		std::unique_ptr<Token> clone() override { return std::make_unique<Division>(); }
	};

	class Exponentiation : public BOperator {
	public:
		Exponentiation() : BOperator(3) {}
		Number calc(const Number& A, const Number& B) const override
		{
			return Number(std::pow(A.getValue(), B.getValue()));
		}
		std::string toString() override { return std::string("^"); }
		std::unique_ptr<Token> clone() override { return std::make_unique<Exponentiation>(); }
	};

	class UMinus : public UOperator {
	public:
		Number calc(const Number& A) const override
		{
			return Number(-1 * A.getValue());
		}
		std::string toString() override { return std::string("-u"); }
		std::unique_ptr<Token> clone() override { return std::make_unique<UMinus>(); }
	};

	class Logarithm10 : public UOperator {
	public:
		Number calc(const Number& A) const override
		{
			return Number(std::log10(A.getValue()));
		}
		std::string toString() override { return std::string("log"); }
		std::unique_ptr<Token> clone() override { return std::make_unique<Logarithm10>(); }
	};

	class LogarithmE : public UOperator {
	public:
		Number calc(const Number& A) const override
		{
			return Number(std::log(A.getValue()));
		}
		std::string toString() override { return std::string("ln"); }
		std::unique_ptr<Token> clone() override { return std::make_unique<LogarithmE>(); }
	};

	class Sqrt : public UOperator {
	public:
		Number calc(const Number& A) const override
		{
			return Number(std::sqrt(A.getValue()));
		}
		std::string toString() override { return std::string("sqrt"); }
		std::unique_ptr<Token> clone() override { return std::make_unique<Sqrt>(); }
	};

	class Sine : public UOperator {
	public:
		Number calc(const Number& A) const override
		{
			return Number(std::sin(A.getValue()));
		}
		std::string toString() override { return std::string("sin"); }
		std::unique_ptr<Token> clone() override { return std::make_unique<Sine>(); }
	};

	class Cosine : public UOperator {
	public:
		Number calc(const Number& A) const override
		{
			return Number(std::cos(A.getValue()));
		}
		std::string toString() override { return std::string("cos"); }
		std::unique_ptr<Token> clone() override { return std::make_unique<Cosine>(); }
	};

	class Tangent : public UOperator {
	public:
		Number calc(const Number& A) const override
		{
			return Number(std::tan(A.getValue()));
		}
		std::string toString() override { return std::string("tan"); }
		std::unique_ptr<Token> clone() override { return std::make_unique<Tangent>(); }
	};

	class Cotangent : public UOperator {
	public:
		Number calc(const Number& A) const override
		{
			return Number(std::cos(A.getValue()) / std::sin(A.getValue()));
		}
		std::string toString() override { return std::string("cot"); }
		std::unique_ptr<Token> clone() override { return std::make_unique<Cotangent>(); }
	};

	class Arcsine : public UOperator {
	public:
		Number calc(const Number& A) const override
		{
			return Number(std::asin(A.getValue()));
		}
		std::string toString() override { return std::string("asin"); }
		std::unique_ptr<Token> clone() override { return std::make_unique<Arcsine>(); }
	};

	class Arccosine : public UOperator {
	public:
		Number calc(const Number& A) const override
		{
			return Number(std::acos(A.getValue()));
		}
		std::string toString() override { return std::string("acos"); }
		std::unique_ptr<Token> clone() override { return std::make_unique<Arccosine>(); }
	};

	class Arctangent : public UOperator {
	public:
		Number calc(const Number& A) const override
		{
			return Number(std::atan(A.getValue()));
		}
		std::string toString() override { return std::string("atan"); }
		std::unique_ptr<Token> clone() override { return std::make_unique<Arctangent>(); }
	};

	class Arccotangent : public UOperator {
	public:
		Number calc(const Number& A) const override
		{
			return Number(std::atan(1. / A.getValue()));
		}
		std::string toString() override { return std::string("acot"); }
		std::unique_ptr<Token> clone() override { return std::make_unique<Arccotangent>(); }
	};

	class Secant : public UOperator {
	public:
		Number calc(const Number& A) const override
		{
			return Number(1. / std::cos(A.getValue()));
		}
		std::string toString() override { return std::string("sec"); }
		std::unique_ptr<Token> clone() override { return std::make_unique<Secant>(); }
	};

	class Cosecant : public UOperator {
	public:
		Number calc(const Number& A) const override
		{
			return Number(1. / std::sin(A.getValue()));
		}
		std::string toString() override { return std::string("csc"); }
		std::unique_ptr<Token> clone() override { return std::make_unique<Cosecant>(); }
	};

	class Arcsecant : public UOperator {
	public:
		Number calc(const Number& A) const override
		{
			return Number(std::acos(1. / A.getValue()));
		}
		std::string toString() override { return std::string("asec"); }
		std::unique_ptr<Token> clone() override { return std::make_unique<Arcsecant>(); }
	};

	class Arccosecant : public UOperator {
	public:
		Number calc(const Number& A) const override
		{
			return Number(std::asin(1. / A.getValue()));
		}
		std::string toString() override { return std::string("acsc"); }
		std::unique_ptr<Token> clone() override { return std::make_unique<Arccosecant>(); }
	};

	class HyperbolicSine : public UOperator {
	public:
		Number calc(const Number& A) const override
		{
			return Number((std::exp(A.getValue()) - std::exp(-A.getValue())) / 2.);
		}
		std::string toString() override { return std::string("hsin"); }
		std::unique_ptr<Token> clone() override { return std::make_unique<HyperbolicSine>(); }
	};

	class HyperbolicCosine : public UOperator {
	public:
		Number calc(const Number& A) const override
		{
			return Number((std::exp(A.getValue()) + std::exp(-A.getValue())) / 2.);
		}
		std::string toString() override { return std::string("hcos"); }
		std::unique_ptr<Token> clone() override { return std::make_unique<HyperbolicCosine>(); }
	};

	class HyperbolicTangent : public UOperator {
	public:
		Number calc(const Number& A) const override
		{
			return Number((std::exp(2. * A.getValue()) - 1) / (std::exp(2. * A.getValue()) + 1));
		}
		std::string toString() override { return std::string("htan"); }
		std::unique_ptr<Token> clone() override { return std::make_unique<HyperbolicTangent>(); }
	};

	class HyperbolicCotangent : public UOperator {
	public:
		Number calc(const Number& A) const override
		{
			return Number((std::exp(2. * A.getValue()) + 1) / (std::exp(2. * A.getValue()) - 1));
		}
		std::string toString() override { return std::string("hcot"); }
		std::unique_ptr<Token> clone() override { return std::make_unique<HyperbolicCotangent>(); }
	};

	class TokenMapInitializer
	{
	public:
		TokenMapInitializer() = delete;

		static TokenMap parenthesesInit()
		{
			TokenMap parentheses;
			add(parentheses, std::make_unique<LParenthesis>());
			add(parentheses, std::make_unique<RParenthesis>());
			return parentheses;
		}

		static TokenMap supportedOperatorsInit()
		{
			TokenMap operators;
			add(operators, std::make_unique<Plus>());
			add(operators, std::make_unique<Minus>());
			add(operators, std::make_unique<UMinus>());
			add(operators, std::make_unique<Multiplication>());
			add(operators, std::make_unique<Division>());
			add(operators, std::make_unique<Exponentiation>());
			add(operators, std::make_unique<Sqrt>());
			add(operators, std::make_unique<Logarithm10>());
			add(operators, std::make_unique<LogarithmE>());

			add(operators, std::make_unique<Sine>());
			add(operators, std::make_unique<Cosine>());
			add(operators, std::make_unique<Tangent>());
			add(operators, std::make_unique<Cotangent>());

			add(operators, std::make_unique<Arcsine>());
			add(operators, std::make_unique<Arccosine>());
			add(operators, std::make_unique<Arctangent>());
			add(operators, std::make_unique<Arccotangent>());

			add(operators, std::make_unique<Secant>());
			add(operators, std::make_unique<Cosecant>());
			add(operators, std::make_unique<Arcsecant>());
			add(operators, std::make_unique<Arccosecant>());

			add(operators, std::make_unique<HyperbolicSine>());
			add(operators, std::make_unique<HyperbolicCosine>());
			add(operators, std::make_unique<HyperbolicTangent>());
			add(operators, std::make_unique<HyperbolicCotangent>());

			return operators;
		}

	private:
		static void add(TokenMap& m, std::unique_ptr<Token> token)
		{
			m.emplace(std::make_pair(token->toString(), std::move(token)));
		}
	};

	TokenMap supportedOperators = TokenMapInitializer::supportedOperatorsInit();
	TokenMap parentheses = TokenMapInitializer::parenthesesInit();
}