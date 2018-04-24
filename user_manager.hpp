//
// Created by root on 4/24/18.
//

#ifndef FEATHER_USER_MANAGER_HPP
#define FEATHER_USER_MANAGER_HPP

using namespace ormpp;
using namespace cinatra;
namespace feather{
class user_manager{
public:
    void add_user(const cinatra::request& req, cinatra::response& res){
        auto body = req.body();
        if (body.empty()) {
            res.set_status_and_content(cinatra::status_type::bad_request);
            return;
        }

        user u;
        bool r = iguana::json::from_json(u, body.data(), body.length());
        if (!r) {
            res.set_status_and_content(cinatra::status_type::bad_request);
            return;
        }

        dao_t<dbng<mysql>> dao;
        r = dao.add_object(u);
        if (!r) {
            res.set_status_and_content(status_type::internal_server_error);
        }
        else {
            res.set_status_and_content(status_type::ok);
        }
    }
};
}
#endif //FEATHER_USER_MANAGER_HPP
