#pragma once
#include "../Common/type.h"

#include <locale>
#include <string>

class Lexer
{
public:
    enum class Token : unsigned int
    {
        // Keywords
        FUNCTION,
        IF,
        PRINT,
        RETURN,
        SCAN,
        SPAWN,
        SLEEP,
        SYNC,
        TYPE,
        VAR,
        WHILE,

        // Boolean values
        FALSE,
        TRUE,

        // Operators
        AND_BOOL,
        AND_INT,
        ASSIGN,
        DIVIDE,
        EQUAL,
        GREATER_THAN,
        LEFT_SHIFT,
        LESS_THAN,
        MINUS,
        MODULO,
        MULT,
        SQRT,
        NOT,
        OR_BOOL,
        OR_INT,
        PLUS,
        RIGHT_SHIFT,
        OP_START = AND_BOOL,
        OP_END = RIGHT_SHIFT,

        // Misc
        ARROW,
        COMMA,
        COMMENT,
        COLON,
        IDENTIFIER,
        LEFT_BRACE,
        LEFT_BRACKET,
        LEFT_PAREN,
        ML_COMMENT,
        NUMBER,
        STRING_LITERAL,
        SEMI_COLON,
        RIGHT_BRACE,
        RIGHT_BRACKET,
        RIGHT_PAREN,

        // End of file
        TOK_EOF,

        // Unknown
        UNKNOWN
    };

private:
    Type currentType;          /*!< Current type recognized by the lexer */

    int currentNumber;                 /*!< Current number in the lexer buffer */
    std::string currentString;            /*!< Current string in the lexer buffer */

    std::string buffer;                /*!< The lexer buffer */
    std::string::const_iterator bufferIt;    /*!< Iterator of the lexer buffer */
    std::locale mLoc;                   /*!< The source locale */
public:
    Lexer() : currentType{ Type::ERROR }, currentNumber{ 0 }, mLoc{ "" } { };

public:
    /*
    * \brief            Initialize the lexer by acquiring the content of a ListFunc file
    * \param filename   Name of a file containing a ListFunc program
    * \return           Has the initialization been successful?
    */
    bool Init(const std::string& filename);

    /*
    * \brief    Get the next token in the program
    * \return   A ListFunc token
    */
    Token GetNextToken();

    /*
    * \brief    Gives the current type
    * \return   The current type
    */
    Type GetCurrentType() const { return currentType; }


    /*
    * \brief    Gives the current number
    * \return   The current number
    */
    int GetCurrentNumber() const { return currentNumber; }

    /*
    * \brief    Gives the current string
    * \return   The current string
    */
    const std::string& GetCurrentStr() const { return currentString; }
};  