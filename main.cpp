//
// Created by qiyu on 4/24/18.
//
#include <iostream>
#include "feather.h"
#include "feather_cfg.hpp"
#include "entity.h"
#include "dao.hpp"
#include "user_manager.hpp"
#include "article_manager.hpp"
using namespace feather;
using namespace ormpp;
using namespace cinatra;

struct person{
    std::string name;
    int age;
    std::string address;
};

REFLECTION(person, name, age, address);

void test_struct_to_json(){
    person p{"aa", 20, "zhu"};
    auto o = feather::struct_to_json(p);
    for (auto it = o.begin(); it != o.end(); ++it) {
        std::cout << it.key() << " : " << it.value() << "\n";
    }
}

void init_db(bool clear_all){
    dao_t<dbng<mysql>> dao;
    bool r = true;
    if(clear_all){
        bool r1 = dao.drop_table<user>();
        bool r2 = dao.drop_table<article>();
        assert(r1&&r2);
    }

    {
        bool r1 = dao.create_table<user>({ ID(user::id) });
        bool r2 = dao.create_table<article>({ ID(article::id) });
        assert(r1&&r2);
    }

//    {
//        //add some test data to tables
//        user u1{0, "tom"};
//        user u2{0, "mike"};
//        std::vector<user> v{u1, u2};
//        int n = dao.add_objects(v);
//        assert(n==2);
//        bool r1 = dao.add_object(u1);
//        bool r2 = dao.add_object(u2);
//    }

//    {
//        article ar1{0, 1, "test", 1, std::time(nullptr)};
//        article ar2{0, 1, "hello", 2, std::time(nullptr)};
//        bool r1 = dao.add_object(ar1);
//        bool r2 = dao.add_object(ar2);
//        assert(r1&&r2);
//    }
//
//    {
//        std::vector<user> v;
//        bool r = dao.get_object(v, "id<3");
//        assert(r);
//
//        std::vector<article> v1;
//        bool r1 = dao.get_object(v1);
//        assert(r1);
//    }
}

void init(const feather_cfg& cfg){

    nanolog::initialize(nanolog::GuaranteedLogger(), cfg.log_path, cfg.log_name, cfg.roll_file_size);

    dao_t<dbng<mysql>>::init(cfg.db_conn_num, cfg.db_ip.data(), cfg.user_name.data(), cfg.pwd.data(),
                             cfg.db_name.data(), cfg.timeout);
    init_db(cfg.clear_all_table);
}

int main(){
    feather_cfg cfg{};
    bool ret = config_manager::from_file(cfg, "./cfg/feather.cfg");
    if (!ret) {
        return -1;
    }

    init(cfg);

    cinatra::http_server server(cfg.thread_num);
    bool r = server.listen("0.0.0.0", cfg.port);
    if (!r) {
        LOG_INFO << "listen failed";
        return -1;
    }

    user_manager user_mgr;
    server.set_http_handler<POST>("/add_user", &user_manager::add_user, &user_mgr);

    article_manager article_mgr;
    server.set_http_handler<POST>("/add_article", &article_manager::add_article, &article_mgr);
    server.set_http_handler<GET, POST>("/get_article", &article_manager::get_article, &article_mgr);
    server.set_http_handler<GET, POST>("/remove_article", &article_manager::remove_article, &article_mgr);
    server.set_http_handler<POST>("/update_article", &article_manager::update_article, &article_mgr);

    server.run();

    return 0;
}