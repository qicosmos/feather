//
// Created by root on 4/24/18.
//

#ifndef FEATHER_UTIL_HPP
#define FEATHER_UTIL_HPP

#include "inja/test/thirdparty/nlohmann/json.hpp"

namespace feather{

    template<typename T, typename = std::enable_if_t <iguana::is_reflection_v<T>>>
    inline nlohmann::json struct_to_json(T&& t){
        nlohmann::json val;
        iguana::for_each(std::forward<T>(t), [&t, &val](const auto& v, auto i){
            auto name = iguana::get_name<T>(decltype(i)::value);
            val[name.data()] = t.*v;
        });
        return val;
    }
}

#endif //FEATHER_UTIL_HPP
