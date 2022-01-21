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
    SQRT_ARGUMENT,
    EQ_MISSING_ARGUMENTS,
    LE_MISSING_ARGUMENTS,
    NAND_MISSING_ARGUMENTS,

    HEAD_TOO_MUCH_ARGUMENTS,
    HEAD_ZERO_ARGUMENTS,
    HEAD_NOT_MATCHING_CORRECT_ARGUMENT,

    TAIL_TOO_MUCH_ARGUMENTS,
    TAIL_ZERO_ARGUMENTS,
    TAIL_NOT_MATCHING_CORRECT_ARGUMENT,

    LENGTH_MISSING_ARGUMENTS,
    LIST_MISSING_ARGUMENTS,
    CONCAT_MISSING_ARGUMENTS,
    IF_MISSING_ARGUMENTS,
    INT_MISSING_ARGUMENTS,
    WRITE_MISSING_ARGUMENTS,
    MOD_WORK_ONLY_WITH_INTEGERS,
    // Array
    ARRAY_EMPTY_EXPR,
    ARRAY_UNEXPECTED_END,
    ARRAY_MISSING_COMMA,
    ARRAY_MISSING_NUMBER,
    ARRAY_MISSING_RIGHT_BRACKET,
    ARRAY_MULTIPLE_TYPES,
    ARRAY_NOT_A_NUMBER,
    ARRAY_ZERO_LENGTH,

    // Binary Expr
    MISSING_RHS,
    WRONG_OPERATION,
    WRONG_USE_OPERATION,

    // Call
    CALL_ARG_ERROR,
    CALL_MISSING_PAREN,
    CALL_PARAM_TYPE_ERROR,
    CALL_RETURN_ERROR,
    CALL_NB_ARGS_ERROR,

    // File
    WRONG_FILE_TYPE,
    ERROR_OPENING_FILE,

    // Function
    NOT_FOUNDED_FUNCTION,
    FN_INTERNAL,
    FN_MISSING_ARROW,
    FN_MISSING_TYPE,
    FN_MISSING_IDENTIFIER,
    FN_MISSING_LEFT_PAREN,
    FN_MISSING_RIGHT_PAREN,
    FN_MISSING_RETURN,
    FN_MISSING_RETURN_TYPE,
    FN_REDEFINITION,
    FN_UNDECLARED,

    // If
    IF_MISSING_BODY,
    IF_MISSING_COND,

    // IO
    PRINT_WRONG_INPUT_TYPE,
    SCAN_MISSING_INPUT_VAR,
    SCAN_WRONG_INPUT_TYPE,

    // Literal
    NEW_LINE_IN_LITERAL,
    MISSING_CLOSING_QUOTE,
    NUMBER_BAD_SUFFIX,

    // Param
    PARAM_MISSING_NAME,
    PARAM_MISSING_COLON,
    PARAM_MISSING_TYPE,

    // Type
    WRONG_BIN_EXPR_TYPE,
    WRONG_COND_EXPR_TYPE,
    WRONG_EXPR_TYPE,
    WRONG_INIT_ARRAY_SCALAR,
    WRONG_INIT_SCALAR_ARRAY,
    WRONG_LITERAL_TYPE,
    WRONG_RETURN_TYPE,
    WRONG_VARIABLE_TYPE,

    // Var
    PARAM_REDEFINITION,
    VAR_MISSING_TYPE,
    VAR_MISSING_COLON,
    VAR_MISSING_IDENTIFIER,
    VAR_REDEFINITION,
    VAR_UNDECLARED_IDENTIFIER,
    VAR_VOID_TYPE,

    // While
    WHILE_MISSING_BODY,
    WHILE_MISSING_COND,

    // Misc
    EXPECTED_DECL,
    MISSING_SEMI_COLON,
    UNCLOSED_ML_COMMENT,
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
