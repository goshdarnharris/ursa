#pragma once

#include <meta>
#include <format.hh>

namespace ursa::customize {

template <typename I, typename T>
struct customization {};

template <typename I, typename T>
inline consteval auto opt_in()
{
    auto context = std::meta::access_context::unchecked();
    auto customization_scope = std::meta::parent_of(^^T);
    auto interface_info = ^^I;

    for(auto&& interface_member : std::meta::members_of(interface_info, context)) {
        if(std::meta::is_special_member_function(interface_member) and not std::meta::is_user_declared(interface_member)) {
            //skip compiler-generated special member functions; 
            //if it's a customization point it must be user-declared.
            continue;
        }
        bool customization_found = false;
        for(auto customization_member : std::meta::members_of(customization_scope, context)) {
            //FIXME: need to handle templated customizations if possible.
            //might need to be at callsite.
            if(std::meta::is_function(customization_member) or std::meta::is_function_template(customization_member)) {
                for(auto annotation : std::meta::annotations_of(customization_member)) {
                    if(std::meta::extract<std::meta::info>(annotation) == interface_member) {
                        constexpr_print(FMT_COMPILE("Found customization for {}"), std::meta::display_string_of(interface_member));
                        //FIXME: check any other constraints that can be handled at this
                        //point
                        customization_found = true;
                        break;
                    }
                }
            }
            if(customization_found) break;
        }
        if(not customization_found) {
            throw std::runtime_error(fmt::format(FMT_COMPILE("No customization found for interface member: {}"), std::meta::display_string_of(interface_member)));
        }
    }
    return customization<I,T>{};
}

// inline consteval auto instantiate_template_customization(std::meta::info customization_template, std::meta::info instance,
//     std::vector<std::meta::info> explicit_template_args, std::vector<std::meta::info> function_args)
// {
//     return std::meta
// }

template <auto... Params>
inline consteval bool check_customization(std::meta::info cpo_info, std::meta::info instance_type_info, std::meta::info customization_info, auto&&... args)
{
    std::vector<std::meta::info> annotations;
    // constexpr_print(FMT_COMPILE("Checking customization: {}"), std::meta::display_string_of(customization_info));
    if(std::meta::is_function_template(customization_info) and std::meta::can_substitute(customization_info, {Params..., instance_type_info})) {
        auto instantiated_template = std::meta::substitute(customization_info, {Params..., instance_type_info});
        annotations = std::meta::annotations_of(instantiated_template);
    } else if(std::meta::is_function(customization_info)) {
        if(sizeof...(Params) > 0) {
            //if the customization is a function, but the user has provided template parameters, then this is not a match.
            return false;
        }
        annotations = std::meta::annotations_of(customization_info);
    } else {
        return false;
    }

    for(auto annotation : annotations) {
        if(std::meta::extract<std::meta::info>(annotation) == cpo_info) {

            
            //FIXME needs more constraint checking
            // - number of arguments
            // - argument types
            // - return type
            // - template parameters - if the cpo is a template the customization must be as well (number, kind, etc)

            return true;
        }
    }
    return false;
}

template <auto... Params>
inline consteval auto find_customizations(std::meta::info cpo_info, auto&& instance, auto&&... args)
{
    auto instance_info = std::meta::remove_cvref(std::meta::type_of(^^instance));
    auto customization_scope = std::meta::parent_of(instance_info);

    // constexpr_print(FMT_COMPILE("Looking for customizations for CPO {} with instance {} in scope: {}"), 
    //     std::meta::display_string_of(cpo_info), 
    //     std::meta::display_string_of(instance_info),
    //     std::meta::display_string_of(customization_scope)
    // );

    for(auto customization_member : std::meta::members_of(customization_scope, std::meta::access_context::unchecked())) {
        if(check_customization<Params...>(cpo_info, instance_info, customization_member, std::forward<decltype(args)>(args)...)) {
            return customization_member;
        }
    }
    return ^^void;
}

template <auto cpo>
inline constexpr decltype(auto) invoke(auto&& instance, auto&&... args)
{
    constexpr auto customization = find_customizations<>(cpo, std::forward<decltype(instance)>(instance), std::forward<decltype(args)>(args)...);
    return [:customization:](std::forward<decltype(instance)>(instance), std::forward<decltype(args)>(args)...);
}


template <auto cpo_info>
struct customization_point {
    constexpr decltype(auto) operator()(auto&& instance, auto&&... args) const
    {
        constexpr auto customization = find_customizations<>(cpo_info, std::forward<decltype(instance)>(instance), std::forward<decltype(args)>(args)...);
        return [:customization:](std::forward<decltype(instance)>(instance), std::forward<decltype(args)>(args)...);
    }
};

// inline consteval void define_interface(std::meta::info interface_info, std)
// {
    
// }

template <typename T>
struct interface_definition {
    struct interface;

    consteval {
        auto interface_info = ^^T;
        std::vector<std::meta::info> interface_members;
        for(auto interface_member : std::meta::members_of(interface_info, std::meta::access_context::unchecked())) {
            if(std::meta::is_special_member_function(interface_member) and not std::meta::is_user_declared(interface_member)) {
                //skip compiler-generated special member functions; 
                //if it's a customization point it must be user-declared.
                continue;
            }
            if(std::meta::is_function_template(interface_member)) {
                //FIXME: once we can define templated members, we can support customization points with explicit templates
                //FIXME: once we can define static members, we can make these static for interface::function style access
                //       instead of interface.function

                auto cpo = std::meta::substitute(^^customization_point, {std::meta::reflect_constant(interface_member)});
                interface_members.push_back(std::meta::data_member_spec(cpo, {
                    .name = std::meta::identifier_of(interface_member),
                    .no_unique_address = true
                }));
            }
            if(std::meta::is_function(interface_member)) {
                auto cpo = std::meta::substitute(^^customization_point, {std::meta::reflect_constant(interface_member)});
                interface_members.push_back(std::meta::data_member_spec(cpo, {
                    .name = std::meta::identifier_of(interface_member),
                    .no_unique_address = true
                }));
            }
        }

        std::meta::define_aggregate(^^interface, interface_members);
    }
};

template <typename T>
using interface = typename interface_definition<T>::interface;

} //namespace customize
