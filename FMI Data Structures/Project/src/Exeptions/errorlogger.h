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

    LIST_MISSING_ARGUMENTS,
    LIST_INCORRECT_ARGUMENTS,

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
    std::unordered_map<ErrorType, std::string> errorMessages;    /*!< Maps the error types to the error messages */
    ErrorType errorType;
public:
    ErrorLogger() = default;
    ErrorLogger(const ErrorLogger&) = delete;
    ErrorLogger& operator=(const ErrorLogger&) = delete;
    ~ErrorLogger() = default;

public:
    const std::string getErrorMessages() { return errorMessages[errorType]; };
    const ErrorType getErrorType() const { return errorType; }
    void setErrorType(const ErrorType type) { errorType = type; }
};
