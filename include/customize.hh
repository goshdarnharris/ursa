#pragma once

namespace customize {

[[deprecated]] constexpr void print_type(auto&&) {}

namespace detail {

    





    template <auto V>
    struct constant_value {
        static constexpr decltype(V) value = V;
    };

    template <typename T>
    concept constexpr_function = requires(T&& v) { constant_value<v()>{}; };

    template <typename T>
    struct function;

    // template <typename T>
    //     requires requires(T&& v) { constant_value<v()>{}; }
    
} //namespace detail

template <auto Info>
struct function {
    static constexpr decltype(auto) operator()(auto&& v, auto&&... args) {
        //use v to:
        //  - check that it implements the interface
        //  - look up the overload set for the function specified by Info
        //     - how to find overloads that are declared elsewhere?
        //     - like, if you implement an interface outside of the namespace of a type,
        //       and opt it into the interface, it should be eligible
        //     - consider super types that have opted into the interface
        //  - invoke the overload set
    }
};


//keep it simple to start.
template <typename T>
struct interface;



template <typename T, typename I>
concept implements = true;

template <typename T>
consteval auto make_interface()
{
    //need to:
    //iterate over members of T
    //for functions, extract:
    //  - static or not (?)
    //  - template, if it is one
    //  - any attributes we can extract [[]]
    //  - constexpr-ness? throwing? etc?
    //  - signature
    //  - name
    //  - implementation, if it has one
    //then, define interface<T> by giving it function<> members for each.
    //function<> objects are callable, and will resolve their overload
    //set on invocation.
}

template <typename T>
using interface_t = decltype(make_interface<T>());




} //namespace customize
