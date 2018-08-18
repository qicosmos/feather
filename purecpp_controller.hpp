#pragma once
#include "dao.hpp"
#include "entity.h"
#include "feather.h"
#include "md5.hpp"
using namespace ormpp;
using namespace cinatra;

namespace feather {
	class purecpp_controller {
	public:
		void home(request& req, response& res) {		
			std::string s = "0";
			std::string lens = "10";

			auto start = req.get_query_value("start");
			auto len = req.get_query_value("len");
			if (!start.empty() && !start.empty()) {
				s = std::string(start.data(), start.length());
				lens = std::string(len.data(), len.length());

				if (!is_integer(s) || !is_integer(lens)) {
					res.set_status_and_content(status_type::bad_request);
					return;
				}
			}

			dao_t<dbng<mysql>> dao;
			auto v = dao.query<std::tuple<int>>("SELECT count(1) from pp_posts t1,pp_post_views t3  where post_status = 'publish' "
				"AND t3.period = 'total' AND t3.ID = t1.ID");
			if (v.empty()) {
				res.set_status_and_content(status_type::internal_server_error);
				return;
			}

			size_t total = std::get<0>(v[0]);
			char* p;
			size_t cur_page = strtol(s.data(), &p, 10)/10+1;

			std::string sql = "SELECT t1.*, t2.user_login, t3.count from pp_posts t1, pp_user t2, pp_post_views t3  "
				"where post_status = 'publish' AND t1.post_author = t2.ID AND t3.period = 'total' AND t3.ID = t1.ID ORDER BY post_date DESC LIMIT "+s+","+lens;
			
			render_home(sql, res, cur_page, total);
		}

		void detail(request& req, response& res) {
			auto ids = req.get_query_value("id");
			if (ids.empty()) {
				res.set_status_and_content(status_type::bad_request);
				return;
			}

			if (!is_integer(std::string(ids.data(), ids.length()))) {
				res.set_status_and_content(status_type::bad_request);
				return;
			}

			std::string sql = "SELECT t1.*, t2.user_login, t3.count from pp_posts t1, pp_user t2, pp_post_views t3  "
				"where post_status = 'publish' AND t1.ID = " + std::string(ids.data(), ids.length()) + " AND t1.post_author = t2.ID AND t3.period = 'total' AND t3.ID = t1.ID ; ";

			dao_t<dbng<mysql>> dao;
			auto v = dao.query<std::tuple<pp_posts, std::string, int>>(sql);
			if (v.empty()) {
				res.set_status_and_content(status_type::bad_request, "");
				return;
			}

			nlohmann::json article;
			for (auto& o : v) {
				auto& post = std::get<0>(o);
				article["post_title"] = post.post_title;
				article["post_modified"] = post.post_modified;
				std::string user_login = std::get<1>(o);
				int total = std::get<2>(o);
				article["user_login"] = user_login;
				article["total"] = total;
				article["post_content"] = std::move(post.post_content);
			}

			res.add_header("Content-Type", "text/html; charset=utf-8");
			res.set_status_and_content(status_type::ok, render::render_file("./purecpp/html/detail.html", article));
		}

		void category(request& req, response& res) {
			auto category = req.get_query_value("category");
			if (category.empty()) {
				res.set_status_and_content(status_type::bad_request);
				return;
			}

			std::string category_s = std::string(category.data(), category.length());

			if (!is_integer(category_s)) {
				res.set_status_and_content(status_type::bad_request);
				return;
			}

			std::string sql = "SELECT t1.*, t2.user_login, t3.count from pp_posts t1, pp_user t2, pp_post_views t3  "
				"where post_status = 'publish' AND t1.category=" + category_s + " AND t1.post_author = t2.ID AND t3.period = 'total' AND t3.ID = t1.ID ORDER BY post_date DESC; ";

			render_home(sql, res);
		}

		void search(request& req, response& res) {
			auto key_word = req.get_query_value("keywords");
			if (key_word.empty()) {
				res.set_status_and_content(status_type::bad_request);
				return;
			}

			if (has_special_char(key_word)) {
				res.set_status_and_content(status_type::bad_request);
				return;
			}

			std::string key_word_s = std::string(key_word.data(), key_word.length());

			std::string sql = "SELECT t1.*, t2.user_login, t3.count from pp_posts t1, pp_user t2, pp_post_views t3  "
				"where post_status = 'publish' AND t1.post_author = t2.ID AND t3.period = 'total' AND t3.ID = t1.ID AND post_content like \"%" + key_word_s + "%\"" + " ORDER BY post_date; ";

			render_home(sql, res);
		}

		void comment(request& req, response& res) {
			auto key_word = req.get_query_value("editorContent");
			if (key_word.empty()) {
				res.set_status_and_content(status_type::bad_request);
				return;
			}

			if (has_special_char(key_word)) {
				res.set_status_and_content(status_type::bad_request);
				return;
			}

			auto user_login = req.get_query_value("user_login");
			if (user_login.empty()) {
				res.set_status_and_content(status_type::bad_request);
				return;
			}

			if (has_special_char(user_login)) {
				res.set_status_and_content(status_type::bad_request);
				return;
			}

			res.set_status_and_content(status_type::ok);
		}

		void login_page(request& req, response& res) {
			res.add_header("Content-Type", "text/html; charset=utf-8");
			res.render_view("./purecpp/html/login.html");
		}

		void login(request& req, response& res) {
			auto user_name = req.get_query_value("user_name");
			if (user_name.empty()) {
				res.set_status_and_content(status_type::bad_request);
				return;
			}

			if (has_special_char(user_name)) {
				res.set_status_and_content(status_type::bad_request);
				return;
			}

			auto password = req.get_query_value("password");
			if (password.empty()) {
				res.set_status_and_content(status_type::bad_request);
				return;
			}

			if (has_special_char(password)) {
				res.set_status_and_content(status_type::bad_request);
				return;
			}

			std::string user_name_s = std::string(user_name.data(), user_name.length());
			std::string password_s = std::string(password.data(), password.length());

			std::string sql = "select count(1) from pp_user where user_login='" + user_name_s + "' and user_pass=md5('" + password_s+"')";
			dao_t<dbng<mysql>> dao;
			auto r = dao.query<std::tuple<int>>(sql);
			if (std::get<0>(r[0])==0) {
				res.set_status_and_content(status_type::ok, "用户名或密码不正确");
				return;
			}

			auto session = res.start_session();
			session->set_data("userid", user_name_s);
			session->set_max_age(-1);
			res.set_status_and_content(status_type::ok, "login");
		}

		void logout_page(request& req, response& res) {
			res.add_header("Content-Type", "text/html; charset=utf-8");
			res.render_view("./purecpp/html/logout.html");
		}

		void is_login(request& req, response& res) {
			bool has_login = check_login(req);

			if (has_login) {
				res.set_status_and_content(status_type::ok, "已经登录");
			}
			else {
				res.set_status_and_content(status_type::ok, "没有登录");
			}
			
		}

		void logout(request& req, response& res) {
			auto user_name = req.get_query_value("user_name");
			if (user_name.empty()) {
				res.set_status_and_content(status_type::bad_request);
				return;
			}

			if (has_special_char(user_name)) {
				res.set_status_and_content(status_type::bad_request);
				return;
			}

			auto email = req.get_query_value("email");
			if (email.empty()) {
				res.set_status_and_content(status_type::bad_request);
				return;
			}

			if (has_special_char(email, true)) {
				res.set_status_and_content(status_type::bad_request);
				return;
			}

			auto answer = req.get_query_value("answer");
			if (answer.empty()) {
				res.set_status_and_content(status_type::bad_request);
				return;
			}

			if (has_special_char(answer)) {
				res.set_status_and_content(status_type::bad_request);
				return;
			}

			auto pwd = req.get_query_value("user_pwd");
			if (pwd.empty()) {
				res.set_status_and_content(status_type::bad_request);
				return;
			}

			if (has_special_char(pwd)) {
				res.set_status_and_content(status_type::bad_request);
				return;
			}

			std::string user_name_s = std::string(user_name.data(), user_name.length());
			std::string email_s = std::string(email.data(), email.length());
			std::string answer_s = std::string(answer.data(), answer.length());
			std::string pwd_s = std::string(pwd.data(), pwd.length());

			std::string sql = "select count(1) from pp_user where user_login='" + user_name_s + "' or user_email='" + email_s + "'";
			dao_t<dbng<mysql>> dao;
			auto r = dao.query<std::tuple<int>>(sql);
			if (std::get<0>(r[0]) > 0) {
				res.set_status_and_content(status_type::ok, "用户名或邮箱已经存在");
				return;
			}

			auto r1 = dao.query<std::tuple<int>>("select count(1) from pp_logout_answer where ID=1 and answer like \"%"+ answer_s+"%\"");
			if (std::get<0>(r1[0]) == 0) {
				res.set_status_and_content(status_type::ok, "验证问题答案错误");
				return;
			}
			
			std::string data = pwd_s;
			std::string data_hex_digest;

			md5 hash;
			hash.update(data.begin(), data.end());
			hash.hex_digest(data_hex_digest);

			pp_user user = {};
			user.user_email = email_s;
			user.user_login = user_name_s;
			user.user_nickname = user_name_s;
			user.user_pass = data_hex_digest;

			bool ok = dao.add_object(user);
			if (!ok) {
				res.set_status_and_content(status_type::internal_server_error);
				return;
			}
			
			res.set_status_and_content(status_type::ok, "注册成功");
		}

		void new_post(request& req, response& res) {
			bool has_login = check_login(req);
			if (!has_login) {
				res.set_status_and_content(status_type::ok, "请先登录");
			}
			else {
				res.set_status_and_content(status_type::ok);
			}
		}

	private:
		void render_home(const std::string& sql, response& res, size_t cur_page=0, size_t total=0) {
			dao_t<dbng<mysql>> dao;
			auto v = dao.query<std::tuple<pp_posts, std::string, int>>(sql);
			if (v.empty()) {
				res.set_status_and_content(status_type::ok, "");
				return;
			}

			nlohmann::json article_list;
			for (auto& o : v) {
				nlohmann::json item;
				auto& post = std::get<0>(o);
				item["ID"] = post.ID;
				item["post_author"] = post.post_author;
				item["content_abstract"] = post.content_abstract;
				item["post_date"] = post.post_date;
				item["post_title"] = post.post_title;
				item["category"] = post.category;
				item["comment_count"] = post.comment_count;

				std::string user_login = std::get<1>(o);
				int total = std::get<2>(o);
				item["user_login"] = user_login;
				item["total"] = total;
				article_list.push_back(item);
			}

			if (total == 0) {
				total = v.size();
			}

			nlohmann::json result;
			result["article_list"] = article_list;
			result["has_login"] = true;
			result["current_page"] = cur_page;
			result["total"] = total;
			res.add_header("Content-Type", "text/html; charset=utf-8");
			res.set_status_and_content(status_type::ok, render::render_file("./purecpp/html/home.html", result));
		}

		bool check_login(request& req) {
			auto ptr = req.get_session();
			auto session = ptr.lock();
			if (session == nullptr || session->get_data<std::string>("userid") != "1") {
				return false;
			}

			return true;
		}
	};
}