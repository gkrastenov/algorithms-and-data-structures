#include "errorlogger.h"
#include <iostream>

const std::string ErrorLogger::getErrorMessages()
{
    return  errorMessages[errorType];
}