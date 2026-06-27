#pragma once

#define FMT_HEADER_ONLY
#include <fmt/format.h>
#include <fmt/compile.h>
#include <meta>

constexpr auto constexpr_print_str(std::string_view msg) noexcept -> void {
    if consteval {
        __builtin_constexpr_diag(0, "", msg);
    }
}

consteval auto constexpr_print(auto format, auto&&... args) {
    __builtin_constexpr_diag(0, "", fmt::format(format, std::forward<decltype(args)>(args)...));
}

// #define CONSTEXPR_PRINT(fmt, ...) constexpr_print_str(fmt::format(FMT_COMPILE(fmt), __VA_ARGS__))

template <typename T>
void pretty_print_struct(const T& obj) {
    constexpr auto ctx = std::meta::access_context::unchecked();
    constexpr auto struct_name = std::meta::display_string_of(^^T);

    // consteval {
    //     CONSTEXPR_PRINT("{}", std::meta::display_string_of(^^nodiscard_test_true));
    // }

    fmt::println("{} {{", struct_name);
    
    static constexpr auto members = std::define_static_array(std::meta::members_of(^^T, ctx));

    static_assert(std::meta::is_destructor(^^T::~T));
    
    template for (constexpr auto member : members) {
        if constexpr(std::meta::is_function(member) or std::meta::is_constructor(member) or std::meta::is_destructor(member)) {
            
        } else {

       
            constexpr auto member_type = std::meta::type_of(member);

            if constexpr(std::meta::is_reflection_type(member_type)) {
                fmt::println("  {} = <std::meta::info>", std::meta::display_string_of(member));
            } else if constexpr(std::meta::has_identifier(member)) {
                constexpr auto member_name = std::meta::identifier_of(member);
                if constexpr(std::meta::is_function(member)) {
                    fmt::println("  {} = <function>", member_name);
                } else {
                    fmt::println("  {} = {}", member_name, obj.[:member:]);
                }
            } else {
                constexpr auto member_name = std::meta::display_string_of(member);
                if constexpr(std::meta::is_function(member)) {
                    fmt::println("  {} = <function>", member_name);
                } else {
                    fmt::println("  {} = {}", member_name, obj.[:member:]);
                }
            }
        }
    }
    fmt::println("}};");
}
