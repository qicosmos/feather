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
        void index(const cinatra::request& req, cinatra::response& res){
            int page_number = 0;
            auto page_s = req.get_query_value("page");
            if (!page_s.empty()) {
                std::string page = std::string(page_s.data(), page_s.length());
                page_number = atoi(page_s.data());
            }

            int page_size = 1;
            dao_t<dbng<mysql>> dao;
            auto tp = dao.query<std::tuple<int>>("select count(1) from article");
            int total = std::get<0>(tp[0]);
            if(total>10){
                page_size = total/10;
                if(total%10!=0)
                    page_size+=1;
            }

            nlohmann::json result;
            result["now_page"] = page_number;
            result["totalPage"] = page_size;

            std::string page_offset = std::to_string(page_number*10);
            std::string cond = std::string("limit ")+page_offset+", 10";

            nlohmann::json article_list;

            std::vector<article> v;
            bool r = dao.get_object(v, cond);
            if(!r){
                res.set_status_and_content(status_type::internal_server_error);
            }else{
                article_list = struct_to_json(v);
            }

            result["articleList"] = article_list;
            result["page"] = page_number;

            res.set_status_and_content(status_type::ok, render("/index.html", result));
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
