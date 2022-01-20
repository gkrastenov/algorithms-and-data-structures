#include "compiler.h"

void Compiler::Run(const std::string& programFile)
{
    std::ifstream stream{ filename };
    // Tough we are going to perform magic,
    // we can't summon a program out of thin air
    if (!stream)
    {
        std::cout << "Can't open " << programFile << std::endl;
        return;
    }

    std::string buffer{ std::istreambuf_iterator<char>(stream),
                        std::istreambuf_iterator<char>{} };


    std::vector<Token> toks = lexer.Tokenize(buffer);

    // Further arcane incantations to come...
}