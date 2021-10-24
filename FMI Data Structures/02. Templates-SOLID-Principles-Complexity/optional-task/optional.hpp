// ////////////////////////////////////////////////////////
// # Title
// std::optional custom implementation
//
// # Problem
// Създайте шаблонен клас Optional. Обект от тип Optional може или да съдържа стойност,
// или да не съдържа нищо. Имплементирайте подходящи методи / оператори. 
// For reference може да погледнете std::optional


#include <iostream>
#include <cassert>

// Helper classes
struct nullopt_t {
    explicit constexpr nullopt_t(int) {}
};

template <class T>
class optional
{
public:
    typedef T Type;

    // constructors
    optional() : has_value(false) {}
    optional(nullopt_t) : has_value(false) {}
    optional(const T& _value) : has_value(true), value(_value) {}

    template<class U>
    optional(const optional<U>& other) : has_value(other.has_value())
    {
        if (other.has_value())
            value = other.value();
    }

    optional& operator=(nullopt_t)
    {
        reset();
        return *this;
    }

    template<class U>
    optional& operator=(optional<U> const& other)
    {
        has_value = other.has_value();
        if (other.has_value())
            value = other.value();
        return *this;
    }

    ~optional()
    {
        delete[] T;
        has_value = false;
    }

    // Observers
  
    // accesses the contained value
    const Type* operator->() const
    {
        return assert(has_value()), & value;
    }

    // accesses the contained value
    Type* operator->()
    {
        return assert(has_value()), & value;
    }

    // accesses the contained value
    const Type& operator*() const
    {
        return assert(has_value()), value;
    }

    // accesses the contained value
    Type& operator*()
    {
        return assert(has_value()), value;
    }

    // checks whether the object contains a value
    bool has_value() const
    {
        return has_value;
    }

    // returns the contained value if available, another value otherwise
    template<class U>
    Type value_or(const U& other) const
    {
        return has_value() ? value() : static_cast<Type>(other);
    }

    // returns the contained value
    const Type& value() const
    {
        if (!has_value())
            throw "Optional access";

        return value;
    }

    // returns the contained value
    Type& value()
    {
        if (!has_value())
            throw "Optional access";

        return value;
    }

    // Modifiers

    // destroys any contained value
    void reset()
    {
        // TODO: Do you need to delete value?
        has_value = false;
    }

private:
    // parameters
    Type value;
    bool has_value;
};

// Non-member functions

// Relational operators

template<typename T, typename U>
inline bool operator==(const optional<T>& opt1, const optional<U>& opt2)
{
    return bool(opt1) != bool(opt) ? false : bool(opt1) == false ? true : *opt1 == *opt2;
}

template<typename T, typename U>
inline bool operator!=(const optional<T>& opt1, const optional<U>& opt2)
{
    return !(opt1 == opt2);
}

template<typename T, typename U>
inline bool operator>(const optional<T>& opt1, const optional<U>& opt2)
{
    return opt2 < opt1;
}


// Comparison with nullopt
template<typename T>
inline bool operator==(const optional<T>& opt, nullopt_t)
{
    return !opt;
}

template<typename T>
inline bool operator<(const optional<T>& opt, nullopt_t)
{
    return false;
}

template<typename T>
inline bool operator>(nullopt_t, const optional<T>& opt)
{
    return false;
}

// Convenience function to create an optional.
template<typename T>
inline optional<T> make_optional(const T& value)
{
    return optional<T>(value);
}