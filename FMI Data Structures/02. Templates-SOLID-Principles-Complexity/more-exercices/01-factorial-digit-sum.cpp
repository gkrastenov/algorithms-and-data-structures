// ////////////////////////////////////////////////////////
// # Title
// Factorial digit sum
//
// # URL
// https://projecteuler.net/problem=20
//
// # Problem
// `n!` means `n * (n - 1) * ... * 3 * 2 * 1`
//
// For example, `10! = 10 * 9 * ... * 3 * 2 * 1 = 3628800`,
// and the sum of the digits in the number `10!` is `3 + 6 + 2 + 8 + 8 + 0 + 0 = 27`.
//
// Find the sum of the digits in the number `100!`

#include <iostream>
#include <vector>

// store single digits in an array, lowest digit come first
typedef std::vector<unsigned int> Digits;

Digits factorial(uint32_t maxFactor)
{
    // 1! = 1
    Digits result = { 1 };

    // avoid further memory allocations
    result.reserve(2568); // 1000! has 2568 digits

    // go through all factors
    for (uint32_t factor = 2; factor <= maxFactor; factor++)
    {
        // multiply each digit with current factor
        // might overflow into next digit => carry
        uint32_t carry = 0;
        for (auto& digit : result)
        {
            digit = digit * factor + carry;

            if (digit >= 10)
            {
                carry = digit / 10;
                digit %= 10;
            }
            else
                carry = 0;
        }

        // add remaining carry
        while (carry != 0)
        {
            result.push_back(carry % 10);
            carry /= 10;
        }
    }
    return result;
}

int main()
{
    uint32_t number;
    std::cin >> number;

    // add all digits of the factorial
    uint32_t sum = 0;
    for (auto i : factorial(number))
        sum += i;
    std::cout << sum << std::endl;

    return 0;
}