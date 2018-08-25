//
// Created by root on 4/24/18.
//

#ifndef FEATHER_UTIL_HPP
#define FEATHER_UTIL_HPP

#include "cinatra/nlohmann_json.hpp"

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

    template<typename T, typename = std::enable_if_t <iguana::is_reflection_v<T>>>
    inline nlohmann::json struct_to_json(const std::vector<T>& v){
        nlohmann::json list;
        for(auto& t : v){
            nlohmann::json val = struct_to_json(t);
            list.push_back(val);
        }

        return list;
    }

    inline std::string cur_time(){
        std::time_t t = std::time(nullptr);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&t), "%Y-%m-%d %H:%M:%S");
        return ss.str();
    }

	inline std::string sv2s(std::string_view sv) {
		return std::string(sv.data(), sv.length());
	}

	inline bool is_integer(const std::string& str) {
		char* p;
		long converted = strtol(str.data(), &p, 10);
		if (*p) {
			return false;
		}
		
		return true;
	}

	inline bool is_integer(std::string_view str) {
		for (auto ch : str) {
			if (!std::isdigit(ch))
				return false;
		}

		return true;
	}

	inline bool has_special_char(std::string_view str, bool is_email=false) {
		for (char ch : str) {
			if (ch == ';' || ch == ',' || ch == '%' || ch == '"'|| ch == '\\'||
				ch == '-' || ch == '|' || ch == '(' || ch == ')' || ch == '[' || ch == ']' ||
				ch == '{' || ch == '}' || (!is_email&&ch == '@') || ch == '*' || ch == '!')
				return true;
		}

		return false;
	}
}

#endif //FEATHER_UTIL_HPP
