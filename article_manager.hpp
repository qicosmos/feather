//
// Created by root on 4/24/18.
//
using namespace ormpp;
using namespace cinatra;

#ifndef FEATHER_ARTICLE_MANAGER_HPP
#define FEATHER_ARTICLE_MANAGER_HPP
namespace feather{
    class article_manager{
    public:
        void add_article(const cinatra::request& req, cinatra::response& res){
            auto body = req.body();
            if (body.empty()) {
                res.set_status_and_content(cinatra::status_type::bad_request);
                return;
            }

            article ar;
            bool r = iguana::json::from_json(ar, body.data(), body.length());
            if (!r) {
                res.set_status_and_content(cinatra::status_type::bad_request);
                return;
            }

            dao_t<dbng<mysql>> dao;
            r = dao.add_object(ar);
            if (!r) {
                res.set_status_and_content(status_type::internal_server_error);
            }
            else {
                res.set_status_and_content(status_type::ok);
            }
        }

        //get article by id
        void get_article_list(const cinatra::request& req, cinatra::response& res){
            auto id_s = req.get_query_value("id");
            if (id_s.empty()) {
                res.set_status_and_content(cinatra::status_type::bad_request);
                return;
            }

            int id = atoi(id_s.data());
            if(id==0){
                res.set_status_and_content(cinatra::status_type::bad_request);
                return;
            }

            dao_t<dbng<mysql>> dao;
            std::vector<article> v;
            bool r = dao.get_object(v, "id="+std::string(id_s.data(), id_s.length()));
            if(!r){
                res.set_status_and_content(status_type::internal_server_error);
            }else{
                iguana::string_stream ss;
                iguana::json::to_json(ss, v);
                res.set_status_and_content(status_type::ok, std::move(ss.str()));
            }
        }

        void article_detail(const cinatra::request& req, cinatra::response& res){

        }

        void remove_article(const cinatra::request& req, cinatra::response& res){
            auto id_s = req.get_query_value("id");
            if (id_s.empty()) {
                res.set_status_and_content(cinatra::status_type::bad_request);
                return;
            }

            int id = atoi(id_s.data());
            if(id==0){
                res.set_status_and_content(cinatra::status_type::bad_request);
                return;
            }

            dao_t<dbng<mysql>> dao;
            bool r = dao.remove_object<article>("id="+std::string(id_s.data(), id_s.length()));
            if(!r){
                res.set_status_and_content(status_type::internal_server_error);
            }else{
                res.set_status_and_content(status_type::ok);
            }
        }

        void update_article(const cinatra::request& req, cinatra::response& res){
            auto body = req.body();
            if (body.empty()) {
                res.set_status_and_content(cinatra::status_type::bad_request);
                return;
            }

            article ar;
            bool r = iguana::json::from_json(ar, body.data(), body.length());
            if (!r) {
                res.set_status_and_content(cinatra::status_type::bad_request);
                return;
            }

            dao_t<dbng<mysql>> dao;
            r = dao.update_object(ar);
            if(!r){
                res.set_status_and_content(status_type::internal_server_error);
            }else{
                res.set_status_and_content(status_type::ok);
            }
        }

    private:

    };
}
#endif //FEATHER_ARTICLE_MANAGER_HPP
