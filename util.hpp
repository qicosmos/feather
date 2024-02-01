//
// Created by root on 4/24/18.
//

#ifndef FEATHER_UTIL_HPP
#define FEATHER_UTIL_HPP

namespace feather {

inline std::string cur_time() {
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

inline bool has_special_char(std::string_view str, bool is_email = false) {
  for (char ch : str) {
    if (ch == ';' || ch == ',' || ch == '%' || ch == '"' || ch == '\\' ||
        ch == '-' || ch == '|' || ch == '(' || ch == ')' || ch == '[' ||
        ch == ']' || ch == '{' || ch == '}' || (!is_email && ch == '@') ||
        ch == '*' || ch == '!' || ch == '\'' || ch == '#')
      return true;
  }

  return false;
}

inline std::string_view utf8substr(std::string_view str, int sub_len) {
  int len = 0;
  int byte_index = 0;
  for (byte_index = 0; byte_index < str.length(); byte_index++) {
    if ((str[byte_index] & 0xc0) != 0x80)
      len += 1;

    if (len >= sub_len)
      break;
  }

  return str.substr(0, byte_index);
}
}  // namespace feather

#endif  // FEATHER_UTIL_HPP
