//
// Created by qiyu on 4/24/18.
//
#include <iostream>
#include "feather.h"
#include "feather_cfg.hpp"
#include "entity.h"
#include "purecpp_controller.hpp"
#include "validate.hpp"
#include "dao.hpp"

using namespace feather;
using namespace ormpp;
using namespace cinatra;

void init(const feather_cfg& cfg) {
	nanolog::Logger::initialize(nanolog::GuaranteedLogger(), cfg.log_path, cfg.log_name, cfg.roll_file_size);

	dao_t<dbng<mysql>>::init(cfg.db_conn_num, cfg.db_ip.data(), cfg.user_name.data(), cfg.pwd.data(),
		cfg.db_name.data(), cfg.timeout);

	Dao dao;
	ormpp_auto_key key{ SID(cncppcon2018_user::id) };
	ormpp_unique uk{ SID(cncppcon2018_user::phone) };
	bool r = dao.create_table<cncppcon2018_user>(key, uk);
	assert(r);

	r = dao.create_table<visit_counter>(ormpp_auto_key{ SID(visit_counter::id) });
	assert(r);

	ormpp_auto_key member_key{ SID(user_group_member::id) };
	r = dao.create_table<user_group_member>(member_key);
	assert(r);

	ormpp_auto_key book_key{ SID(book_course_member::id) };
	ormpp_unique uniq_key{ SID(book_course_member::phone) };
	r = dao.create_table<book_course_member>(member_key, uniq_key);
	assert(r);

	{
		ormpp_auto_key key{ SID(speaker::id) };
		r = dao.create_table<speaker>(uniq_key);
		assert(r);
	}
}

int main() {
	feather_cfg cfg{};
	bool ret = config_manager::from_file(cfg, "./cfg/feather.cfg");
	if (!ret) {
		return -1;
	}

	init(cfg);

	cinatra::http_server server(cfg.thread_num);
	server.set_res_cache_max_age(86400);
	server.set_public_root_directory("");
	server.set_static_dir("static");
	server.enable_http_cache(false);//set global cache
	bool r = server.listen("0.0.0.0", cfg.port);
	if (!r) {
		LOG_CRIT << "listen failed";
		return -1;
	}

	purecpp_controller purecpp_ctl;
	server.set_http_handler<GET, POST>("/login_page", &purecpp_controller::login_page, &purecpp_ctl);
	server.set_http_handler<GET, POST>("/login", &purecpp_controller::login, &purecpp_ctl, check_login_input{});
	server.set_http_handler<GET, POST>("/sign_up_page", &purecpp_controller::sign_up_page, &purecpp_ctl);
	server.set_http_handler<GET, POST>("/sign_up", &purecpp_controller::sign_up, &purecpp_ctl, check_sign_out_input{});
	server.set_http_handler<GET, POST>("/member_edit_page", &purecpp_controller::member_edit_page, &purecpp_ctl, check_login{});
	server.set_http_handler<GET, POST>("/member_edit", &purecpp_controller::member_edit, &purecpp_ctl, check_login{}, check_member_edit_input{});
	server.set_http_handler<GET, POST>("/quit", &purecpp_controller::quit, &purecpp_ctl, check_login{});

	server.set_http_handler<GET, POST>("/course_page", &purecpp_controller::course_page, &purecpp_ctl);
	server.set_http_handler<GET, POST>("/book_course", &purecpp_controller::book_course, &purecpp_ctl, check_join_cncppcon2018{}, check_book_course{});

	server.set_http_handler<GET, POST>("/cncppcon_page2019", &purecpp_controller::cncppcon_page2019, &purecpp_ctl);
	server.set_http_handler<GET, POST>("/api/enrol/speaker", &purecpp_controller::enrol_speakers, &purecpp_ctl);

	server.set_http_handler<GET, POST>("/cncppcon_query_page2018", &purecpp_controller::cncppcon_query_page2018, &purecpp_ctl);
	server.set_http_handler<GET, POST>("/join_cncppcon2018", &purecpp_controller::join_cncppcon2018, &purecpp_ctl, check_join_cncppcon2018{});
	server.set_http_handler<GET, POST>("/query_cncppcon2018", &purecpp_controller::query_cncppcon2018, &purecpp_ctl, check_query_cncppcon2018{});
	server.set_http_handler<GET, POST>("/register_user_group", &purecpp_controller::register_user_group, &purecpp_ctl);
	server.set_http_handler<GET, POST>("/join_user_group", &purecpp_controller::join_user_group, &purecpp_ctl, check_join_cncppcon2018{});
	
	server.set_http_handler<GET>("/", &purecpp_controller::home, &purecpp_ctl, check_start_end_input{});
	server.set_http_handler<GET>("/home", &purecpp_controller::home, &purecpp_ctl, check_start_end_input{});
	server.set_http_handler<GET, POST>("/detail", &purecpp_controller::detail, &purecpp_ctl, check_detail_input{});
	server.set_http_handler<GET>("/category", &purecpp_controller::category, &purecpp_ctl, check_category_input{});
	server.set_http_handler<GET, POST>("/search", &purecpp_controller::search, &purecpp_ctl, check_search_input{});

	server.set_http_handler<POST>("/comment", &purecpp_controller::comment, &purecpp_ctl, check_login{}, check_comment_input{});
	server.set_http_handler<GET, POST>("/remove_comment", &purecpp_controller::remove_comment, &purecpp_ctl, check_login{}, check_remove_comment_input{});

	server.set_http_handler<GET, POST>("/add_post_page", &purecpp_controller::add_post_page, &purecpp_ctl, check_login{});
	server.set_http_handler<GET, POST>("/add_post", &purecpp_controller::add_post, &purecpp_ctl, check_login{});
	server.set_http_handler<GET, POST>("/remove_post", &purecpp_controller::remove_post, &purecpp_ctl, check_login{}, check_edit_post_input{});
	server.set_http_handler<GET, POST>("/edit_post_page", &purecpp_controller::edit_post_page, &purecpp_ctl, check_login{}, check_edit_post_input{});
	server.set_http_handler<GET, POST>("/edit_post", &purecpp_controller::edit_post, &purecpp_ctl, check_login{});
	server.set_http_handler<GET, POST>("/pass_post", &purecpp_controller::pass_post, &purecpp_ctl, check_login{}, check_edit_post_input{});
	server.set_http_handler<GET, POST>("/my_post", &purecpp_controller::my_post, &purecpp_ctl, check_login{}, check_start_end_input{});
	server.set_http_handler<GET, POST>("/upload_file", &purecpp_controller::upload, &purecpp_ctl, check_login{});

	server.run();

	return 0;
}
