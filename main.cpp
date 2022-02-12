#include <iostream>
#include "Tokens.hpp"
#include "Sequence.hpp"
#include "Reader.hpp"
#include "RPN.hpp"

int main()
{
    std::string expression;

    std::cout << "Supported operators:" << std::endl;
    for (const auto& t : Tokens::supportedOperators) std::cout << t.first << "\t";
    std::cout << std::endl << std::endl;

    while (true)
    {
        std::getline(std::cin, expression);

        Sequence example;
        try { example = Reader::Read(expression); }
        catch (char* e) { std::cout << e << std::endl; continue; }

        if (RPN::sequenceCheck(example))
        {
            RPN::convertToRPN(example);
            std::cout << RPN::calcRPN(example) << std::endl;
        }
        else std::cout << "\nIt's not RPN\n";
    }

    return 0;
}