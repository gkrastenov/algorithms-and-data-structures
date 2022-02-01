#pragma once
#include <string>
#include <type_traits>
#include <unordered_map>

// Every type of error that can be logged by the ErrorLogger
enum class ErrorType : unsigned int
{
    // Operations
    WRONG_USE_OPERATION_MODIFIED_BUILT_IN_FUNCTION,
    WRONG_CAN_NOT_CONVERT_STRING_TO_NUMBER,

    // Syntax
    SYNTAX_INVALID,
    SYNTAX_MISSING_LBRACE,
    SYNTAX_MISSING_RBRACE,
    SYNTAX_MISSING_LIST_LBRACE,
    SYNTAX_MISSING_LIST_RBRACE,
    SYNTAX_MINUS_BEFORE_LIST_LBRACE,
    SYNTAX_MINUS_BEFORE_LIST_RBRACE,

    // Arguments
    ARGUMENT_ONLY_NUMBERS,
    ARGUMENT_EXEPTION,

    // Built-int
    SQRT_MISSING_ARGUMENT,
    SQRT_INCORRECT_ARGUMENTS,

    DIV_MISSING_ARGUMENTS,
    DIV_INCORRECT_ARGUMENTS,

    EQ_MISSING_ARGUMENTS,
    LE_MISSING_ARGUMENTS,
    NAND_MISSING_ARGUMENTS,

    HEAD_EMPTY_LIST_ARGUMENT,
    HEAD_MISSING_ARGUMENTS,
    HEAD_NOT_MATCHING_CORRECT_ARGUMENT,

    TAIL_TOO_MUCH_ARGUMENTS,
    TAIL_MISSING_ARGUMENTS,
    TAIL_NOT_MATCHING_CORRECT_ARGUMENT,

    LENGTH_MISSING_ARGUMENTS,
    LENGTH_INCORRECT_ARGUMENTS,
    LENGTH_LOOP_LIST_ARGUMENT,

    LIST_MISSING_ARGUMENTS,
    LIST_INCORRECT_ARGUMENTS,

    READ_INCORRECT_ARGUMENTS,
    READ_MISSING_ARGUMENTS,

    SUB_MISSING_ARGUMENTS,
    SUB_INCORRECT_ARGUMENTS,

    ADD_MISSING_ARGUMENTS,
    ADD_INCORRECT_ARGUMENTS,

    MOD_MISSING_ARGUMENTS,
    MOD_INCORRECT_ARGUMENTS,

    CONCAT_MISSING_ARGUMENTS,
    CONCAT_INCORRECT_ARGUMENTS,
    CONCAT_LOOP_LIST,

    IF_MISSING_ARGUMENTS,

    INT_MISSING_ARGUMENTS,
    INT_INCORRECT_ARGUMENTS,

    WRITE_MISSING_ARGUMENTS,
    WRITE_INCORRECT_ARGUMENTS,

    MOD_WORK_ONLY_WITH_INTEGERS,

    // File
    WRONG_FILE_TYPE,
    ERROR_OPENING_FILE,

    NOT_FOUNDED_FUNCTION,

    DIVIDE_BY_ZERO,

    NOT_HANDLED_ERROR,
};


// Singleton class providing error logging services for the whole ListFunc errors
class ErrorLogger
{
private:

private:
     /*!< Maps the error types to the error messages */
      std::unordered_map<ErrorType, std::string> errorMessages{
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



            { ErrorType::HEAD_MISSING_ARGUMENTS,    "ARGUMENT ERROR: Mssing arguments for built-in function: \"head\"." },
            { ErrorType::HEAD_NOT_MATCHING_CORRECT_ARGUMENT,    "ARGUMENT ERROR: not matching correct argument for built-in function: \"head\"." },
            { ErrorType::HEAD_EMPTY_LIST_ARGUMENT,    "ARGUMENT ERROR: Argument is empty list." },

            { ErrorType::TAIL_TOO_MUCH_ARGUMENTS,    "ARGUMENT ERROR: Too much arguments for built-in function: \"tail\"." },
            { ErrorType::TAIL_NOT_MATCHING_CORRECT_ARGUMENT,    "ARGUMENT ERROR: not matching correct argument for built-in function: \"tail\"." },

            { ErrorType::LENGTH_MISSING_ARGUMENTS,    "ARGUMENT ERROR: Missing arguments for built-in function: \"length\"." },
            { ErrorType::LENGTH_INCORRECT_ARGUMENTS,    "ARGUMENT ERROR: Arguments for built-in function \"length\" have to be list." },
            { ErrorType::LENGTH_LOOP_LIST_ARGUMENT,    "ARGUMENT ERROR: Arguments for built-in function \"length\" is loop list." },

            { ErrorType::CONCAT_MISSING_ARGUMENTS,    "ARGUMENT ERROR: Missing arguments for built-in function: \"concat\"." },
            { ErrorType::CONCAT_INCORRECT_ARGUMENTS,    "ARGUMENT ERROR: Arguments for built-in function \"concat\" have to be list." },
            { ErrorType::CONCAT_LOOP_LIST,    "ARGUMENT ERROR: Can not be concat if first argument is loop list." },

            { ErrorType::READ_INCORRECT_ARGUMENTS,    "ARGUMENT ERROR: Incorrect argument for read" },
            { ErrorType::READ_MISSING_ARGUMENTS,    "ARGUMENT ERROR: Missing arguments for built-in function: \"read\"." },

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
    ErrorType errorType;
public:
    ErrorLogger() = default;
    ErrorLogger(const ErrorLogger&) = delete;
    ErrorLogger& operator=(const ErrorLogger&) = delete;
    ~ErrorLogger() = default;

public:
    const std::string getErrorMessages();
    const ErrorType getErrorType() const { return errorType; }
    void setErrorType(const ErrorType& type) { errorType = type; }
};
