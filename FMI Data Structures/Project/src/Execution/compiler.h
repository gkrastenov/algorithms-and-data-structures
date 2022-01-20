#include <memory>
#include <string>
#include <vector>

class Compiler
{
public:
    void Run(const std::string& programFile);

private:
    Lexer lex;
};
