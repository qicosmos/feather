//
// Created by root on 4/24/18.
//

#ifndef FEATHER_DAO_HPP
#define FEATHER_DAO_HPP
namespace feather{
    template<typename DB>
    class dao_t final{
    public:
        dao_t() : conn_(ormpp::connection_pool<DB>::instance().get()), guard_(conn_)
        {
            if(!conn_)
                LOG_WARN << "db get connection failed";
        }

        template<typename T>
        bool create_table(const ormpp_auto_key& key) {
            if(!conn_)
                return false;

            return conn_->template create_datatable<T>(key);
        }

        template<typename T, typename... Args>
        bool create_table(Args&&... args) {
            if(!conn_)
                return false;

            return conn_->template create_datatable<T>(std::forward<Args>(args)...);
        }

        template<typename T>
        bool drop_table() {
            if(!conn_)
                return false;

            constexpr auto name = iguana::get_name<T>();
            return conn_->execute("drop table if exists "+ std::string(name.data(), name.length()));
        }

        template<typename T>
        bool add_object(T& t) {
            if (!conn_)
                return false;

            conn_->begin();
            int n = conn_->insert(t);
            if (n < 0) {
                conn_->rollback();
                LOG_WARN << "insert role failed";
                return false;
            }
            auto v = conn_->template query<std::tuple<int>>("SELECT LAST_INSERT_ID();");
            if (v.empty()) {
                conn_->rollback();
                LOG_WARN << "db failed";
                return false;
            }
            conn_->commit();
            int seq = std::get<0>(v[0]);
            t.id = seq;
            return true;
        }

        template<typename T>
        int add_objects(const T& t) {
            if (!conn_)
                return -1;

            int n = conn_->insert(t);
            if (n < 0) {
                LOG_WARN << "insert failed";
                return -1;
            }

            return n;
        }

        template<typename T>
        bool remove_object(const std::string& condition) {
            if(!conn_)
                return false;

            bool r = conn_->template delete_records<T>(condition);
            if (!r) {
                LOG_WARN << "insert role failed";
                return false;
            }

            return true;
        }

        template<typename T, typename... Args>
        bool get_object(std::vector<T>& v, std::string where_condition = "", Args&&... args) {
            if(!conn_)
                return false;

            v = conn_->template query<T>(std::move(where_condition), std::forward<Args>(args)...);
            if (conn_->has_error())
                return false;

            return true;
        }

        template<typename T>
        int update_object(const T& t) {
            if(!conn_)
                return -1;

            int n = conn_->update(t);
            if (n < 0) {
                LOG_WARN << "insert role failed";
                return -1;
            }
            return n;
        }

        template<typename T>
        auto query(const std::string& sql){
            return conn_->template query<T>(sql);
        }

        //non query sql, such as update, delete, insert
        bool execute(const std::string& sql) {
            if (!conn_)
                return false;

            bool r = conn_->execute(sql);
            if (!r) {
                LOG_WARN << "insert role failed";
                return false;
            }
            return r;
        }

        bool begin() {
            if(!conn_)
                return false;
            return conn_->begin();
        }
        bool rollback() {
            if(!conn_)
                return false;
            return conn_->rollback();
        }
        bool commit() {
            if(!conn_)
                return false;
            return conn_->commit();
        }

        static void init(int max_conns, const char* ip, const char* usr, const char* pwd, const char* db_name, int timeout = 1) {
            ormpp::connection_pool<DB>::instance().init(max_conns, ip, usr, pwd, db_name, timeout);
        }

    private:
        dao_t(const dao_t&) = delete;
        dao_t& operator= (const dao_t&) = delete;

        std::shared_ptr<DB>		conn_;
        ormpp::conn_guard<DB>	guard_;
    };
}
#endif //FEATHER_DAO_HPP
