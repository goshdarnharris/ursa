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


} //namespace customize
