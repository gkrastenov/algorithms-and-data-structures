#include "errorlogger.h"
#include <iostream>

std::unordered_map<ErrorType, std::string> ErrorLogger::errorMessages =
{
    // Operations
    { ErrorType::WRONG_USE_OPERATION_MODIFIED_BUILT_IN_FUNCTION,   "ERROR: Built-in function must not be modified." },
    { ErrorType::WRONG_CAN_NOT_CONVERT_STRING_TO_NUMBER,   "ERROR: Can not convert string to number." },

    // Syntax
    { ErrorType::SYNTAX_INVALID,         "SYNTAX ERROR: Invalid syntax." },
    { ErrorType::SYNTAX_MISSING_LBRACE,         "SYNTAX ERROR: Expected '('." },
    { ErrorType::SYNTAX_MISSING_RBRACE,         "SYNTAX ERROR: Expected ')'." },
    { ErrorType::SYNTAX_MISSING_LIST_LBRACE,    "SYNTAX ERROR: Expected '['." },
    { ErrorType::SYNTAX_MISSING_LIST_RBRACE,    "SYNTAX ERROR: Expected ']'." },
    { ErrorType::SYNTAX_MINUS_BEFORE_LIST_LBRACE,    "SYNTAX ERROR: Not an acceptabl '-' before '['." },
    { ErrorType::SYNTAX_MINUS_BEFORE_LIST_RBRACE,    "SYNTAX ERROR: Not an acceptabl '-' before ']'." },

    // Arguments
    { ErrorType::ARGUMENT_ONLY_NUMBERS,    "ARGUMENT ERROR: Arguments have to be only numbers." },
    { ErrorType::ARGUMENT_EXEPTION,    "ARGUMENT ERROR: Invalid arguments." },

    // Built-in functions
    { ErrorType::SQRT_MISSING_ARGUMENT,    "ARGUMENT ERROR: Missing arguments for built-in function: \"sqrt\"." },
    { ErrorType::SQRT_INCORRECT_ARGUMENTS,    "ARGUMENT ERROR: Argument for built-in function \"sub\" have to be number." },

    // Built-in functions
    { ErrorType::LIST_MISSING_ARGUMENTS,    "ARGUMENT ERROR: Missing arguments for built-in function: \"list\"." },
    { ErrorType::LIST_INCORRECT_ARGUMENTS,    "ARGUMENT ERROR: Argument for built-in function \"list\" have to be number." },

    { ErrorType::DIV_MISSING_ARGUMENTS,    "ARGUMENT ERROR: Missing arguments for built-in function: \"div\"." },
    { ErrorType::DIV_INCORRECT_ARGUMENTS,    "ARGUMENT ERROR: Argument for built-in function \"div\" have to be number." },
    { ErrorType::DIVIDE_BY_ZERO,                "ERROR: Trying to divide by zero." },

    { ErrorType::EQ_MISSING_ARGUMENTS,    "ARGUMENT ERROR: Missing arguments for built-in function: \"eq\"." },
    { ErrorType::LE_MISSING_ARGUMENTS,    "ARGUMENT ERROR: Missing arguments for built-in function: \"le\"." },
    { ErrorType::NAND_MISSING_ARGUMENTS,    "ARGUMENT ERROR: Missing arguments for built-in function: \"nand\"." },
   

 
    { ErrorType::HEAD_MISSING_ARGUMENTS,    "ARGUMENT ERROR: Mssing arguments for built-in function: \"head\"." },,
    { ErrorType::HEAD_NOT_MATCHING_CORRECT_ARGUMENT,    "ARGUMENT ERROR: not matching correct argument for built-in function: \"head\"." },
    { ErrorType::HEAD_EMPTY_LIST_ARGUMENT,    "ARGUMENT ERROR: Argument is empty list." },,

    { ErrorType::TAIL_TOO_MUCH_ARGUMENTS,    "ARGUMENT ERROR: Too much arguments for built-in function: \"tail\"." },
    { ErrorType::TAIL_NOT_MATCHING_CORRECT_ARGUMENT,    "ARGUMENT ERROR: not matching correct argument for built-in function: \"tail\"." },

    { ErrorType::LENGTH_MISSING_ARGUMENTS,    "ARGUMENT ERROR: Missing arguments for built-in function: \"length\"." },
    { ErrorType::LENGTH_INCORRECT_ARGUMENTS,    "ARGUMENT ERROR: Arguments for built-in function \"length\" have to be list." },
    { ErrorType::LENGTH_LOOP_LIST_ARGUMENT,    "ARGUMENT ERROR: Arguments for built-in function \"length\" is loop list." },

    { ErrorType::CONCAT_MISSING_ARGUMENTS,    "ARGUMENT ERROR: Missing arguments for built-in function: \"concat\"." },
    { ErrorType::CONCAT_INCORRECT_ARGUMENTS,    "ARGUMENT ERROR: Arguments for built-in function \"concat\" have to be list." },
    { ErrorType::CONCAT_LOOP_LIST,    "ARGUMENT ERROR: Can not be concat if first argument is loop list." },

    { ErrorType::SUB_MISSING_ARGUMENTS,    "ARGUMENT ERROR: Missing arguments for built-in function: \"sub\"." },
    { ErrorType::SUB_INCORRECT_ARGUMENTS,    "ARGUMENT ERROR: Arguments for built-in function \"sub\" have to be numbers." },

    { ErrorType::ADD_MISSING_ARGUMENTS,    "ARGUMENT ERROR: Missing arguments for built-in function: \"add\"." },
    { ErrorType::ADD_INCORRECT_ARGUMENTS,    "ARGUMENT ERROR: Arguments for built-in function \"add\" have to be numbers." },
    
    { ErrorType::IF_MISSING_ARGUMENTS,    "ARGUMENT ERROR: Missing arguments for built-in function: \"if\"." },

    { ErrorType::INT_MISSING_ARGUMENTS,    "ARGUMENT ERROR: Missing arguments for built-in function: \"int\"." },
    { ErrorType::INT_INCORRECT_ARGUMENTS,    "ARGUMENT ERROR: Argument for built-in function \"int\" have to be number." },

    { ErrorType::MOD_MISSING_ARGUMENTS,    "ARGUMENT ERROR: Missing arguments for built-in function: \"mod\"." },
    { ErrorType::MOD_INCORRECT_ARGUMENTS,    "ARGUMENT ERROR: Argument for built-in function \"mod\" have to be number." },

    { ErrorType::WRITE_MISSING_ARGUMENTS,    "ARGUMENT ERROR: Missing arguments for built-in function: \"write\"." },
    { ErrorType::WRITE_INCORRECT_ARGUMENTS,    "ARGUMENT ERROR: Argument for built-in function \"write\" have to be number." },

    { ErrorType::MOD_WORK_ONLY_WITH_INTEGERS,    "ARGUMENT ERROR: Mod work only with integers." },

    // Function
    { ErrorType::NOT_FOUNDED_FUNCTION,                   "FUNCTION ERROR: Not founded function." },

    // File
    { ErrorType::WRONG_FILE_TYPE,               "FILE ERROR: Wrong file type" },
    { ErrorType::ERROR_OPENING_FILE,            "FILE ERROR: Problem opening the specified file" },
};

const std::string ErrorLogger::getErrorMessages()
{
    if(errorType)
    return  errorMessages[errorType];
}