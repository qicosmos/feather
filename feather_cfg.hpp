//
// Created by root on 4/24/18.
//

#ifndef FEATHER_FEATHER_CFG_HPP
#define FEATHER_FEATHER_CFG_HPP
namespace feather{
    struct feather_cfg{
        int thread_num;
        std::string port;
        std::string db_ip;
        std::string user_name;
        std::string pwd;
        std::string db_name;
        int timeout;
        int db_conn_num;
        bool clear_all_table;
        std::string log_path;
        std::string log_name;
        int roll_file_size;
    };
    REFLECTION(feather_cfg, thread_num, port, db_ip, user_name, pwd, db_name, timeout, db_conn_num,
               clear_all_table, log_path, log_name, roll_file_size);

    class config_manager {
    public:
        config_manager() = delete;

        template<typename T>
        inline static T get(std::string_view key, std::string_view file_path) {
            feather_cfg cfg{};
            bool r = from_file(cfg, file_path);
            if (!r) {
                return {};
            }

            T val{};
            bool has_key = false;
            iguana::for_each(cfg, [key, &val, &cfg, &has_key](const auto& item, auto I) {
                if (key == iguana::get_name<feather_cfg>(decltype(I)::value)) {
                assign(val, cfg.*item);
                has_key = true;
            }
            });

            if (!has_key) {
                return {};
            }

            return val;
        }

        template<typename T>
        inline static bool set(std::string_view key, T&& val, std::string_view file_path) {
            feather_cfg cfg{};
            bool r = from_file(cfg, file_path);
            if (!r) {
                return false;
            }

            bool has_key = false;
            iguana::for_each(cfg, [key, &val, &cfg, &has_key](auto& item, auto I) {
                if (key == iguana::get_name<feather_cfg>(decltype(I)::value)) {
                assign(cfg.*item, val);
                has_key = true;
            }
            });

            if (!has_key)
                return false;

            return to_file(cfg, file_path);
        }

        template<typename T>
        inline static bool from_file(T& t, std::string_view file_path) {
            std::ifstream in(file_path.data(), std::ios::binary);
            if (!in.is_open()) {
                return false;
            }

            in.seekg(0, std::ios::end);
            size_t len = (size_t)in.tellg();
            in.seekg(0);
            std::string str;
            str.resize(len);

            in.read(str.data(), len);

            bool r = iguana::json::from_json(t, str.data(), len);
            if (!r) {
                return false;
            }

            return true;
        }

        template<typename T, typename U>
        inline static void assign(T& t, U& u) {
            if constexpr(std::is_same_v<U, T>) {
                    t = u;
            }
        }

        template<typename T>
        inline static bool to_file(T& t, std::string_view file_path) {
            iguana::string_stream ss;
            iguana::json::to_json(ss, t);
            std::ofstream out(file_path.data(), std::ios::binary);
            if (!out.is_open()) {
                return false;
            }
            out.write(ss.str().data(), ss.str().size());
            out.close();
            return true;
        }
    };
}
#endif //FEATHER_FEATHER_CFG_HPP
