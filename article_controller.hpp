//
// Created by root on 4/24/18.
//
using namespace ormpp;
using namespace cinatra;

#ifndef FEATHER_ARTICLE_MANAGER_HPP
#define FEATHER_ARTICLE_MANAGER_HPP
namespace feather{

    struct article_from_req{
        article arti;
        std::string content;
    };
    REFLECTION(article_from_req, arti, content);

    struct article_list_res{
        std::vector<article> v;
        int current_page;
        int total_page;
    };
    REFLECTION(article_list_res, v, current_page, total_page);

    struct result_res{
        bool success;
    };
    REFLECTION(result_res, success);

    class article_controller{
    public:
        void add_article(cinatra::request& req, cinatra::response& res){
            auto body = req.body();
            if (body.empty()) {
                res.set_status_and_content(cinatra::status_type::bad_request);
                return;
            }

            article_from_req ar;
            bool r = iguana::json::from_json0(ar, body.data(), body.length());
            if (!r) {
                res.set_status_and_content(cinatra::status_type::bad_request);
                return;
            }

            //add to article and article_detail
            article arti = ar.arti;
            dao_t<dbng<mysql>> dao;
            r = dao.add_object(arti);
            if (r) {
                article_detail detail{0, arti.id, arti.title, ar.content, arti.create_time};
                r = dao.add_object(detail);
            }

            result_res result{r};
            iguana::string_stream ss;
            iguana::json::to_json(ss, result);
            res.add_header("Access-Control-Allow-origin", "*");
            if (!r) {
                res.set_status_and_content(status_type::internal_server_error, ss.str());
            }
            else {
                res.set_status_and_content(status_type::ok, ss.str());
            }
        }

        void index(cinatra::request& req, cinatra::response& res){
            int page_number = 0;
            auto page_s = req.get_query_value("page");
            if (!page_s.empty()) {
                std::string page = std::string(page_s.data(), page_s.length());
                page_number = atoi(page_s.data());
            }

            dao_t<dbng<mysql>> dao;
            auto tp = dao.query<std::tuple<int>>("select count(1) from article");
            int total = std::get<0>(tp[0]);

            int page_size = total_page(total);
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

            res.set_status_and_content(status_type::ok, render::render_file("/index.html", result));
        }

        int total_page(size_t size){
            if(size<=10)
                return  1;

            int total_page = size/10;
            if(size%10!=0)
                total_page+=1;

            return total_page;
        }

        void get_article_list(cinatra::request& req, cinatra::response& res){
            int current_page = 0;
            auto page_s = req.get_query_value("page");
            if (!page_s.empty()) {
                std::string page = std::string(page_s.data(), page_s.length());
                current_page = atoi(page_s.data());
            }

            dao_t<dbng<mysql>> dao;
            std::vector<article> v;
            bool r = dao.get_object(v);
            if (!r) {
                res.set_status_and_content(status_type::internal_server_error);
            }

            article_list_res list{std::move(v), current_page, total_page(v.size())};

            iguana::string_stream ss;
            iguana::json::to_json(ss, list);
            res.add_header("Access-Control-Allow-origin", "*");
            res.set_status_and_content(status_type::ok, ss.str());
        }

        void get_article_detail(cinatra::request& req, cinatra::response& res){
            auto id_s = req.get_query_value("id");
            if (id_s.empty()) {
                res.set_status_and_content(cinatra::status_type::bad_request);
                return;
            }

            std::vector<article_detail> v;

            bool r = false;

            {
                dao_t<dbng<mysql>> dao;
                r = dao.get_object(v, "id="+std::string(id_s.data(), id_s.length()));
            }

            if(!r){
                res.set_status_and_content(status_type::internal_server_error);
            }else{
                article_detail at{};
                if(!v.empty())
                    at = std::move(v[0]);

                nlohmann::json result = struct_to_json(at);
                res.add_header("Access-Control-Allow-origin", "*");
                res.set_status_and_content(status_type::ok, render::render_file("/show.html", result));
            }
        }

        void remove_article(cinatra::request& req, cinatra::response& res){
            auto id_s = req.get_query_value("id");
            if (id_s.empty()) {
                res.set_status_and_content(cinatra::status_type::bad_request);
                return;
            }


            std::string id = std::string(id_s.data(), id_s.length());
            dao_t<dbng<mysql>> dao;
            bool r1 = dao.remove_object<article>("id="+id);
            bool r2 = dao.remove_object<article_detail>("parant_id="+id);
            bool r = r1&&r2;
            result_res result{r};
            iguana::string_stream ss;
            iguana::json::to_json(ss, result);
            res.add_header("Access-Control-Allow-origin", "*");
            if(!r){
                res.set_status_and_content(status_type::internal_server_error, ss.str());
            }else{
                res.set_status_and_content(status_type::ok, ss.str());
            }
        }

        void update_article(cinatra::request& req, cinatra::response& res){
            auto body = req.body();
            if (body.empty()) {
                res.set_status_and_content(cinatra::status_type::bad_request);
                return;
            }

            article_detail detail;
            bool r = iguana::json::from_json(detail, body.data(), body.length());
            if (!r) {
                res.set_status_and_content(cinatra::status_type::bad_request);
                return;
            }

            dao_t<dbng<mysql>> dao;
            r = dao.update_object(detail);
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
