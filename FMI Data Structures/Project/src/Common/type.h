// Possible types of a variable or identifier
#pragma once
enum class Type : int
{
    // Default
    ERROR,

    // Builtins
    BOOL,
    NUMBER,
    STRING,
    VOID,

    // Arrays
    NUMBER_ARRAY,

    UNKNOWN,
};

        /*
        * \fn           GetArrayVersion
        * \param type   Type to convert
        * \brief        Converts a scalar type to an array type
        * \return       Array version of the type given
        */
 // Type GetArrayVersion(const Type type);