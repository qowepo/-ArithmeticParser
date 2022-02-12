#pragma once
#include <vector>
#include <memory>
#include "Tokens.hpp"


namespace Tokens
{

	class Sequence {
	public:
		Sequence() { }
		Sequence(Sequence& a) = delete;
		Sequence(Sequence&& a) { data = std::move(a.data); }

		auto begin() { return data.begin(); }
		auto end() { return data.end(); }
		auto begin() const { return data.begin(); }
		auto end() const { return data.end(); }
		auto cbegin() const { return data.cbegin(); }
		auto cend() const { return data.cend(); }

		auto& at(size_t pos) { return data.at(pos); }
		auto& at(size_t pos) const { return data.at(pos); }
		auto& front() { return data.front(); }
		auto& front() const { return data.front(); }
		auto& back() { return data.back(); }
		auto& back() const { return data.back(); }

		bool empty() const { return data.empty(); }
		auto size() const { return data.size(); }

		void pop_back() { data.pop_back(); }
		void push_back(std::unique_ptr<Token>&& ptr) { data.push_back(std::move(ptr)); }

		void pushNumber(double value) { data.push_back(std::make_unique<Number>(value)); }

		Sequence& operator=(Sequence& a) = delete;
		Sequence& operator=(Sequence&& a)
		{
			if (&a != this) { data = std::move(a.data); }
			return *this;
		}

	private:
		std::vector<std::unique_ptr<Token>> data;
	};
}

