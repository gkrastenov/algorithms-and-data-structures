#include "lexer.h"

#include "../Utils/errorlogger.h"

#include <algorithm>
#include <fstream>
#include <iterator>


bool Lexer::Init(const std::string& filename)
{
	std::ifstream stream(filename);
	if (stream)
	{
		buffer.assign(std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>());
		bufferIt = buffer.begin();
		return true;
	}
	return false;
}

Lexer::Token Lexer::GetNextToken()
{
	// Skipping any whitespaces
	while (bufferIt != buffer.end() && isspace(*bufferIt))
	{
		++bufferIt;
	}

	// End of file, we're done
	if (bufferIt == buffer.end())
		return Token::TOK_EOF;

	// Caching the current char because we will use it a lot
	const char currentChar = *bufferIt;

	switch (currentChar)
	{
	case ',':
		++bufferIt;
		return Token::COMMA;
	case '[':
		++bufferIt;
		return Token::LEFT_BRACKET;
	case '(':
		++bufferIt;
		return Token::LEFT_PAREN;
	case ']':
		++bufferIt;
		return Token::RIGHT_BRACKET;
	case ')':
		++bufferIt;
		return Token::RIGHT_PAREN;
	case '-':
		if (*(++bufferIt) == '>'){
			++bufferIt;
			return Token::ARROW;
		}
		else return Token::MINUS;
	default:
		// We have and identifier or a keyword
		if (isalpha(currentChar))
		{
			currentString = currentChar;
			while (++bufferIt != buffer.end() && isalnum(*bufferIt))
			{
				currentString += *bufferIt;
			}
			// TODO: search for exists function and return TOKEN::EXIST_FUNCTION
			// or maybe i have to return identifier RESEARCH
			if (currentString == "add")
				return Token::PLUS;
			if (currentString == "sub")
				return Token::MINUS;
			if (currentString == "mul")
				return Token::MULT;
			if (currentString == "div")
				return Token::DIVIDE;
			if (currentString == "sqrt")
				return Token::SQRT;
			if (currentString == "mod")
				return Token::MODULO;			
			else if (currentString == "if")
				return Token::IF;
			else
				return Token::IDENTIFIER;
		}
		// We have a numeric value
		else if (isdigit(currentChar, mLoc))
		{
			std::string numberStr;
			do
			{
				numberStr += *bufferIt;
			} while (isdigit(*(++bufferIt)));

			if (isalpha(*bufferIt))
			{
				// Error: letter in a number
				// Utils::ErrorLogger::PrintErrorAtLocation(Utils::ErrorLogger::ErrorType::NUMBER_BAD_SUFFIX, mSrcLoc);
				// Advance to the next statement
				while ((bufferIt != buffer.end()))
					++bufferIt;

				if (bufferIt != buffer.end())
					++bufferIt;

				return Token::UNKNOWN;
			}
			else
			{
				currentNumber = std::stoi(numberStr);
				return Token::NUMBER;
			}
		}
		return Token::UNKNOWN;
	}
}