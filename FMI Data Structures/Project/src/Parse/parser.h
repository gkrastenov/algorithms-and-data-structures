#pragma once
#include "lexer.h"

#include "../AST/ast.h"
#include "../AST/statements.h"
#include "../AST/declarations.h"
#include "../AST/expressions.h"

#include <memory>


// Part of the ListFunc compiler used to generate an AST
class Parser
{
private:
    Lexer lexer;                               /*!< Lexer used by the parser to acquire tokens */
    Lexer::Token currentToken;                 /*!< Current token being treated by the parser */

public:
    explicit Parser() : currentToken{ Lexer::Token::UNKNOWN } { }
    ~Parser() = default;

    /*
    * \fn               ParseProgram
    * \brief            Generate an AST from
    * \param filename   Name of a TosLang file
    * \return           Root node of the AST
    */
    std::unique_ptr<ASTNode> ParseProgram(const std::string& filename);

private:	// Declarations
    /*
    * \fn       ParseProgramDecl
    * \brief    programdecl ::= decls
    * \return   A node representing a program
    */
    std::unique_ptr<ASTNode> ParseProgramDecl();

    /*
    * \fn       ParseFunctionDecl
    * \brief    functiondecl ::= 'fn' identifierexpr '(' ( vardecl, )* ')' '->' typeexpr compoundstmt
    * \return   A node representing a function declaration or definition
    */
    std::unique_ptr<FunctionDeclaration> ParseFunctionDecl();

    /*
    * \fn       ParseVarDecl
    * \brief    vardecl ::= 'var' identifierexpr ':' typeexpr ( '=' expr )? ';'
    * \return   A node representing a variable declaration with potentially an initializing expression
    */
    std::unique_ptr<VarDeclaration> ParseVarDecl();

private:	// Expressions
    /*
    * \fn       ParseExpr
    * \brief    expr
    *               ::= arrayexpr
    *               ::= binopexpr
    *               ::= booleanexpr
    *               ::= identifierexpr
    *               ::= indexedexpr
    *               ::= numberexpr
    *               ::= spawnexpr
    *               ::= stringexpr
    * \return   A node representing an expression
    */
    std::unique_ptr<Expression> ParseExpr();

    /*
    * \fn           ParseArrayExpr
    * \brief        arrayexpr
    *                   ::= '{' '}'
    *                   ::= '{' expr(',' expr)* '}'
    * \param lhs    The left hand side expression of the binary expression
    * \param op     The operation in the binary expression
    * \return       A node representing a binary expression
    */
    std::unique_ptr<Expression> ParseArrayExpr();

    /*
    * \fn           ParseBinaryOpExpr
    * \brief        binopexpr ::= expr OP expr
    * \param lhs    The left hand side expression of the binary expression
    * \param op     The operation in the binary expression
    * \return       A node representing a binary expression
    */
    std::unique_ptr<Expression> ParseBinaryOpExpr(Lexer::Token operationToken, std::unique_ptr<Expr>&& lhs);

    /*
    * \fn                   ParseCallExpr
    * \brief                callexpr ::= identifierexpr '(' expr* ')'
    * \param fn             The function to be called
    * \param isSpawnedExpr  Is this a regular function call or one that'll spawn a thread?
    * \return               A node representing function call expression
    */
    std::unique_ptr<Expression> ParseCallExpr(std::unique_ptr<Expr>&& fn, const bool isSpawnedExpr);

private:    // Statements
    /*
    * \fn           ParseCompoundStmt
    * \brief        compoundstmt ::= '{' stmt* '}'
    * \return       A node representing a compound statement i.e. an aggregation of statements
    */
    std::unique_ptr<CompoundStatement> ParseCompoundStmt();

    /*
    * \fn           ParseIfStmt
    * \brief        ifstmt ::= 'if' expr compoundstmt
    * \return       A node representing an if statement
    */
    std::unique_ptr<IfStatement> ParseIfStmt();

private:    // Helpers
    /*
    * \fn                       ParseArrayType
    * \brief                    Parses an array type specification
    * \param[OUT]    arraySize  Size of the array
    * \param[IN/OUT] arrayType  Type of the array
    * \return                   Success
    */
    bool ParseArrayType(int& arraySize, Type& arrayType);
};