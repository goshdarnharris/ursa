#pragma once

#include <array>
#include <format.hh>
#include <fmt/compile.h>
#include <meta>

using namespace fmt::literals;

constexpr auto constexpr_print_str(std::string_view msg) noexcept -> void {
    if consteval {
        __builtin_constexpr_diag(0, "", msg);
    }
}

#define CONSTEXPR_PRINT(fmt, ...) constexpr_print_str(FMT_STATIC_FORMAT(fmt, __VA_ARGS__).c_str())

namespace ursa {

//attribute reflection is theoretically coming in the future: https://github.com/cplusplus/papers/issues/2042


// template <auto T>
// struct has_nodiscard_impl;// { static constexpr bool value = false; };

// template <typename R, typename... Args, R(V)(Args...)>
//     requires requires{ V(std::declval<Args>()...); }
// struct has_nodiscard_impl<V> { static constexpr bool value = false; };

// template <typename R, typename... Args, R(V)(Args...)>
//     requires (not requires{ V(std::declval<Args>()...); })
// struct has_nodiscard_impl<V> { static constexpr bool value = true; };

// template <auto T>
// concept has_nodiscard = has_nodiscard_impl<T>::value;

// int nodiscard_test_false() { return 0; }
// [[nodiscard]] int nodiscard_test_true() { return 0; }

// static_assert(not has_nodiscard<nodiscard_test_false>);
// static_assert(has_nodiscard<nodiscard_test_true>);


template <typename T>
void pretty_print(const T& obj) {
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


template <std::meta::info T>
struct function_traits { 
    static constexpr auto function_info = T;
    static constexpr auto type_info = std::meta::type_of(function_info);

    static constexpr bool is_public = std::meta::is_public(function_info);
    static constexpr bool is_protected = std::meta::is_protected(function_info);
    static constexpr bool is_private = std::meta::is_private(function_info);
    static constexpr bool is_virtual = std::meta::is_virtual(function_info);
    static constexpr bool is_pure_virtual = std::meta::is_pure_virtual(function_info);
    static constexpr bool is_override = std::meta::is_override(function_info);
    static constexpr bool is_final = std::meta::is_final(function_info);
    static constexpr bool is_deleted = std::meta::is_deleted(function_info);
    static constexpr bool is_defaulted = std::meta::is_defaulted(function_info);
    static constexpr bool is_user_provided = std::meta::is_user_provided(function_info);
    static constexpr bool is_user_declared = std::meta::is_user_declared(function_info);
    static constexpr bool is_explicit = std::meta::is_explicit(function_info);
    static constexpr bool is_noexcept = std::meta::is_noexcept(function_info);
    static constexpr bool is_bit_field = std::meta::is_bit_field(function_info);
    static constexpr bool is_enumerator = std::meta::is_enumerator(function_info);
    static constexpr bool is_const = std::meta::is_const(function_info);
    static constexpr bool is_volatile = std::meta::is_volatile(function_info);
    static constexpr bool is_mutable_member = std::meta::is_mutable_member(function_info);
    static constexpr bool is_lvalue_reference_qualified = std::meta::is_lvalue_reference_qualified(function_info);
    static constexpr bool is_rvalue_reference_qualified = std::meta::is_rvalue_reference_qualified(function_info);
    static constexpr bool has_static_storage_duration = std::meta::has_static_storage_duration(function_info);
    static constexpr bool has_thread_storage_duration = std::meta::has_thread_storage_duration(function_info);
    static constexpr bool has_automatic_storage_duration = std::meta::has_automatic_storage_duration(function_info);
    static constexpr bool has_internal_linkage = std::meta::has_internal_linkage(function_info);
    static constexpr bool has_module_linkage = std::meta::has_module_linkage(function_info);
    static constexpr bool has_external_linkage = std::meta::has_external_linkage(function_info);
    static constexpr bool has_c_language_linkage = std::meta::has_c_language_linkage(function_info);
    static constexpr bool has_linkage = std::meta::has_linkage(function_info);
    static constexpr bool is_complete_type = std::meta::is_complete_type(function_info);
    static constexpr bool is_enumerable_type = std::meta::is_enumerable_type(function_info);
    static constexpr bool is_variable = std::meta::is_variable(function_info);
    static constexpr bool is_type = std::meta::is_type(function_info);
    static constexpr bool is_namespace = std::meta::is_namespace(function_info);
    static constexpr bool is_type_alias = std::meta::is_type_alias(function_info);
    static constexpr bool is_namespace_alias = std::meta::is_namespace_alias(function_info);
    static constexpr bool is_function = std::meta::is_function(function_info);
    static constexpr bool is_conversion_function = std::meta::is_conversion_function(function_info);
    static constexpr bool is_operator_function = std::meta::is_operator_function(function_info);
    static constexpr bool is_literal_operator = std::meta::is_literal_operator(function_info);
    static constexpr bool is_special_member_function = std::meta::is_special_member_function(function_info);
    static constexpr bool is_constructor = std::meta::is_constructor(function_info);
    static constexpr bool is_default_constructor = std::meta::is_default_constructor(function_info);
    static constexpr bool is_copy_constructor = std::meta::is_copy_constructor(function_info);
    static constexpr bool is_move_constructor = std::meta::is_move_constructor(function_info);
    static constexpr bool is_assignment = std::meta::is_assignment(function_info);
    static constexpr bool is_copy_assignment = std::meta::is_copy_assignment(function_info);
    static constexpr bool is_move_assignment = std::meta::is_move_assignment(function_info);
    static constexpr bool is_destructor = std::meta::is_destructor(function_info);
    static constexpr bool is_template = std::meta::is_template(function_info);
    static constexpr bool is_function_template = std::meta::is_function_template(function_info);
    static constexpr bool is_variable_template = std::meta::is_variable_template(function_info);
    static constexpr bool is_class_template = std::meta::is_class_template(function_info);
    static constexpr bool is_alias_template = std::meta::is_alias_template(function_info);
    static constexpr bool is_conversion_function_template = std::meta::is_conversion_function_template(function_info);
    static constexpr bool is_operator_function_template = std::meta::is_operator_function_template(function_info);
    static constexpr bool is_literal_operator_template = std::meta::is_literal_operator_template(function_info);
    static constexpr bool is_constructor_template = std::meta::is_constructor_template(function_info);
    static constexpr bool is_concept = std::meta::is_concept(function_info);
    static constexpr bool is_value = std::meta::is_value(function_info);
    static constexpr bool is_object = std::meta::is_object(function_info);
    static constexpr bool is_structured_binding = std::meta::is_structured_binding(function_info);
    static constexpr bool is_class_member = std::meta::is_class_member(function_info);
    static constexpr bool is_namespace_member = std::meta::is_namespace_member(function_info);
    static constexpr bool is_nonstatic_data_member = std::meta::is_nonstatic_data_member(function_info);
    static constexpr bool is_static_member = std::meta::is_static_member(function_info);
    static constexpr bool is_base = std::meta::is_base(function_info);
    static constexpr bool has_default_member_initializer = std::meta::has_default_member_initializer(function_info);
    static constexpr bool has_parent = std::meta::has_parent(function_info);
    static constexpr bool has_template_arguments = std::meta::has_template_arguments(function_info);
    
    static constexpr bool is_void_type = std::meta::is_void_type(type_info);
    static constexpr bool is_null_pointer_type = std::meta::is_null_pointer_type(type_info);
    static constexpr bool is_integral_type = std::meta::is_integral_type(type_info);
    static constexpr bool is_floating_point_type = std::meta::is_floating_point_type(type_info);
    static constexpr bool is_array_type = std::meta::is_array_type(type_info);
    static constexpr bool is_pointer_type = std::meta::is_pointer_type(type_info);
    static constexpr bool is_lvalue_reference_type = std::meta::is_lvalue_reference_type(type_info);
    static constexpr bool is_rvalue_reference_type = std::meta::is_rvalue_reference_type(type_info);
    static constexpr bool is_member_object_pointer_type = std::meta::is_member_object_pointer_type(type_info);
    static constexpr bool is_member_function_pointer_type = std::meta::is_member_function_pointer_type(type_info);
    static constexpr bool is_enum_type = std::meta::is_enum_type(type_info);
    static constexpr bool is_union_type = std::meta::is_union_type(type_info);
    static constexpr bool is_class_type = std::meta::is_class_type(type_info);
    static constexpr bool is_function_type = std::meta::is_function_type(type_info);
    static constexpr bool is_reflection_type = std::meta::is_reflection_type(type_info);
    static constexpr bool is_reference_type = std::meta::is_reference_type(type_info);
    static constexpr bool is_arithmetic_type = std::meta::is_arithmetic_type(type_info);
    static constexpr bool is_fundamental_type = std::meta::is_fundamental_type(type_info);
    static constexpr bool is_object_type = std::meta::is_object_type(type_info);
    static constexpr bool is_scalar_type = std::meta::is_scalar_type(type_info);
    static constexpr bool is_compound_type = std::meta::is_compound_type(type_info);
    static constexpr bool is_member_pointer_type = std::meta::is_member_pointer_type(type_info);
    static constexpr bool is_const_type = std::meta::is_const_type(type_info);
    static constexpr bool is_volatile_type = std::meta::is_volatile_type(type_info);
    static constexpr bool is_trivially_copyable_type = std::meta::is_trivially_copyable_type(type_info);
    // static constexpr bool is_trivially_relocatable_type = std::meta::is_trivially_relocatable_type(type_info);
    // static constexpr bool is_replaceable_type = std::meta::is_replaceable_type(type_info);
    static constexpr bool is_standard_layout_type = std::meta::is_standard_layout_type(type_info);
    static constexpr bool is_empty_type = std::meta::is_empty_type(type_info);
    static constexpr bool is_polymorphic_type = std::meta::is_polymorphic_type(type_info);
    static constexpr bool is_abstract_type = std::meta::is_abstract_type(type_info);
    static constexpr bool is_final_type = std::meta::is_final_type(type_info);
    static constexpr bool is_aggregate_type = std::meta::is_aggregate_type(type_info);
    // static constexpr bool is_consteval_only_type = std::meta::is_consteval_only_type(type_info);
    static constexpr bool is_signed_type = std::meta::is_signed_type(type_info);
    static constexpr bool is_unsigned_type = std::meta::is_unsigned_type(type_info);
    static constexpr bool is_bounded_array_type = std::meta::is_bounded_array_type(type_info);
    static constexpr bool is_unbounded_array_type = std::meta::is_unbounded_array_type(type_info);
    static constexpr bool is_scoped_enum_type = std::meta::is_scoped_enum_type(type_info);
    static constexpr bool is_default_constructible_type = std::meta::is_default_constructible_type(type_info);
    static constexpr bool is_copy_constructible_type = std::meta::is_copy_constructible_type(type_info);
    static constexpr bool is_move_constructible_type = std::meta::is_move_constructible_type(type_info);
    static constexpr bool is_copy_assignable_type = std::meta::is_copy_assignable_type(type_info);
    static constexpr bool is_move_assignable_type = std::meta::is_move_assignable_type(type_info);
    static constexpr bool is_swappable_type = std::meta::is_swappable_type(type_info);
    static constexpr bool is_destructible_type = std::meta::is_destructible_type(type_info);
    static constexpr bool is_trivially_default_constructible_type = std::meta::is_trivially_default_constructible_type(type_info);
    static constexpr bool is_trivially_copy_constructible_type = std::meta::is_trivially_copy_constructible_type(type_info);
    static constexpr bool is_trivially_move_constructible_type = std::meta::is_trivially_move_constructible_type(type_info);
    static constexpr bool is_trivially_copy_assignable_type = std::meta::is_trivially_copy_assignable_type(type_info);
    static constexpr bool is_trivially_move_assignable_type = std::meta::is_trivially_move_assignable_type(type_info);
    static constexpr bool is_trivially_destructible_type = std::meta::is_trivially_destructible_type(type_info);
    static constexpr bool is_nothrow_default_constructible_type = std::meta::is_nothrow_default_constructible_type(type_info);
    static constexpr bool is_nothrow_copy_constructible_type = std::meta::is_nothrow_copy_constructible_type(type_info);
    static constexpr bool is_nothrow_move_constructible_type = std::meta::is_nothrow_move_constructible_type(type_info);
    static constexpr bool is_nothrow_copy_assignable_type = std::meta::is_nothrow_copy_assignable_type(type_info);
    static constexpr bool is_nothrow_move_assignable_type = std::meta::is_nothrow_move_assignable_type(type_info);
    static constexpr bool is_nothrow_swappable_type = std::meta::is_nothrow_swappable_type(type_info);
    static constexpr bool is_nothrow_destructible_type = std::meta::is_nothrow_destructible_type(type_info);
    // static constexpr bool is_nothrow_relocatable_type = std::meta::is_nothrow_relocatable_type(type_info);
    static constexpr bool is_implicit_lifetime_type = std::meta::is_implicit_lifetime_type(type_info);
    static constexpr bool has_virtual_destructor = std::meta::has_virtual_destructor(type_info);
    static constexpr bool has_unique_object_representations = std::meta::has_unique_object_representations(type_info);
};


// template <typename R, typename... Args>
// struct function_traits<R(Args...)> {
//     static constexpr auto return_type = ^^R;
//     static constexpr auto argument_types = std::array{^^Args...};
// };






} //namespace ursa
