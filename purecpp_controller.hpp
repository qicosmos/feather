#pragma once
#include "dao.hpp"
#include "entity.h"
#include "feather.h"
#include "md5.hpp"
#include "validate.hpp"
using namespace ormpp;
using namespace cinatra;

namespace feather {
	using Dao = dao_t<dbng<mysql>>;
	class purecpp_controller {
	public:
		purecpp_controller() {
			Dao dao;
			auto v = dao.query<std::tuple<std::string, std::string>>("SELECT term_id, name from pp_terms  where term_id in (1, 2,3,4,5,7,8,25,47,58)");
			for (auto tp : v) {
				category_map_.emplace(std::get<0>(tp), std::get<1>(tp));
			}
		}

		void home(request& req, response& res) {	
			const auto& params = req.get_aspect_data();
			const auto& s = params.size()==2 ? params[0] : params[3];
			const auto& lens = params.size() == 2 ? params[1] : params[4];

			if (total_post_count_ == 0) {
				Dao dao;
				auto v = dao.query<std::tuple<int>>("SELECT count(1) from pp_posts  where post_status = 'publish'");
				if (v.empty()) {
					res.set_status_and_content(status_type::not_found);
					return;
				}

				total_post_count_ = std::get<0>(v[0]);
			}
			
			auto session = req.get_session().lock();
			if (session == nullptr || session->get_data<std::string>("user_name").empty()) {
				auto new_session = res.start_session();
				new_session->set_max_age(-1);
			}

			size_t cur_page = atoi(s.data())/10+1;

			std::string sql = "SELECT t1.*, t2.user_login from pp_posts t1, pp_user t2 "
				"where post_status = 'publish' AND t1.post_author = t2.ID ORDER BY t1.ID DESC LIMIT "+s+","+lens;
			
			render_page(sql, req, res, "./purecpp/html/home.html", "all", cur_page, total_post_count_);
		}

		void upload(request& req, response& res) {
			assert(req.get_content_type() == content_type::multipart);
			std::string filenames = "";
			auto& files = req.get_upload_files();
			for (auto& file : files) {
				filenames += file.get_file_path();
			}
			nlohmann::json result;
			result["code"] = 0;
			result["msg"] = "";
			nlohmann::json data;
			data["src"] = "http://119.61.16.32:8080" + filenames.substr(1);
			result["data"] = data;
			res.render_json(result);
		}

		void detail(request& req, response& res) {
			auto post_id = sv2s(req.get_query_value("id"));
			std::string sql = "SELECT t1.*, t2.user_login from pp_posts t1, pp_user t2 "
				"where post_status = 'publish' AND t1.ID = " + post_id+" and t1.post_author=t2.ID";
			
			Dao dao;
			auto v = dao.query<std::tuple<pp_posts, std::string>>(sql);
			if (v.empty()) {
				res.set_status_and_content(status_type::not_found);
				return;
			}

			std::string comment_sql = "SELECT t1.*, t2.user_login from pp_comment t1, pp_user t2 "
				"where post_id= " + post_id + " and comment_status<>'trash' and t1.user_id=t2.ID";
	
			const auto& params = get_user_info(req);
			const auto& login_user_name = params.empty() ? "" : params[0];
			const auto& user_id = params.empty() ? "" : params[1];
			const auto& user_role = params.empty() ? "" : params[2];
			bool is_admin = (!login_user_name.empty()) && (user_role == "3" || user_role == "6");
			auto comments = dao.query<std::tuple<pp_comment, std::string>>(comment_sql);
			nlohmann::json comment_list;
			for (auto& item : comments) {
				pp_comment comment = std::get<0>(item);
				nlohmann::json json;
				std::string comment_user = std::get<1>(item);
				json["comment_id"] = comment.ID;
				json["comment_date"] = std::move(comment.comment_date);
				json["comment_content"] = std::move(comment.comment_content);
				json["comment_user"] = comment_user;
				json["is_owner"] = is_admin ? false : login_user_name == comment_user;
				json["is_admin"] = is_admin;
				comment_list.push_back(std::move(json));
			}
			
			nlohmann::json article;
			article["comment_list"] = std::move(comment_list);
			auto& post = std::get<0>(v[0]);
			article["post_id"] = post_id;
			article["post_title"] = post.post_title;
			article["post_modified"] = post.post_modified;
			article["user_login"] = std::get<1>(v[0]);
			article["total"] = comments.size();
			article["has_comment"] = !comments.empty();
			article["post_content"] = std::move(post.post_content);
			article["has_login"] = !login_user_name.empty();
			article["login_user_name"] = login_user_name;
			res.add_header("Content-Type", "text/html; charset=utf-8");
			res.set_status_and_content(status_type::ok, render::render_file("./purecpp/html/detail.html", article));
		}

		void category(request& req, response& res) {
			const auto& params = req.get_aspect_data();
			const auto& s = params[0];
			const auto& lens = params[1];
			const auto& category = params[2];

			Dao dao;
			auto v = dao.query<std::tuple<int>>("SELECT count(1) from pp_posts t1 where post_status = 'publish' "
				"AND t1.category=" + category);
			if (v.empty()) {
				res.set_status_and_content(status_type::internal_server_error);
				return;
			}

			size_t cur_page = atoi(s.data()) / 10 + 1;
			size_t total = std::get<0>(v[0]);

			std::string sql = "SELECT t1.*, t2.user_login from pp_posts t1, pp_user t2 "
				"where post_status = 'publish' AND t1.category=" + category + " AND t1.post_author = t2.ID ORDER BY t1.ID DESC LIMIT " + s + "," + lens;

			render_page(sql, req, res, "./purecpp/html/category.html", category, cur_page, total);
		}

		void remove_post(request& req, response& res) {
			const auto& params = req.get_aspect_data();
			const auto& login_user_name = params[0];
			const auto& post_id = params[3];

			std::string sql = "update pp_posts set post_status='trash' where ID="+ post_id;
			Dao dao;
			bool r = dao.execute(sql);

			if (r) {
				total_post_count_--;
				res.redirect("/home");
			}
			else {
				res.set_status_and_content(status_type::internal_server_error);
			}
		}

		void edit_post_page(request& req, response& res) {
			const auto& params = req.get_aspect_data();
			const auto& login_user_name = params[0];
			const auto& post_id = params[3];

			std::string sql = "select post_title, category, post_content, raw_content from pp_posts where ID=" + post_id;
			Dao dao;
			auto r = dao.query<std::tuple<std::string, std::string, std::string, std::string>>(sql);
			if (r.empty()) {
				res.set_status_and_content(status_type::bad_request);
				return;
			}

			nlohmann::json result;
			result["has_login"] = !login_user_name.empty();
			result["login_user_name"] = login_user_name;
			result["post_id"] = post_id;
			result["title"] = std::move(std::get<0>(r[0]));
			result["category"] = std::move(std::get<1>(r[0]));
			result["post_content"] = std::move(std::get<2>(r[0]));
			result["raw_content"] = std::move(std::get<3>(r[0]));

			res.add_header("Content-Type", "text/html; charset=utf-8");
			res.set_status_and_content(status_type::ok, render::render_file("./purecpp/html/edit_post.html", result));
		}

		void pass_post(request& req, response& res) {
			const auto& params = req.get_aspect_data();
			const auto& post_id = params[3];

			std::string sql = "update pp_posts set post_status='publish' where ID="+ post_id;
			Dao dao;
			bool r = dao.execute(sql);
			if (r) {
				res.redirect("/home");
			}
			else {
				res.set_status_and_content(status_type::internal_server_error);
			}
		}

		void edit_post(request& req, response& res) {
			const auto& params = req.get_aspect_data();
			const auto& login_user_name = params[0];
			const auto& user_id = params[1];
			const auto& role = params[2];

			auto post_id = req.get_query_value("post_id");
			auto title = req.get_query_value("title");
			auto type = req.get_query_value("type");
			auto post_content = req.get_query_value("post_content");
			auto raw_content = req.get_query_value("md_post_content");

			Dao dao;
			pp_posts post{};
			post.post_title = std::string(title.data(), title.length());
			post.category = std::string(type.data(), type.length());
			post.ID = atoi(post_id.data());
			post.post_author = atoi(user_id.data());
			post.post_modified = cur_time();
			post.post_content = std::string(post_content.data(), post_content.length());
			post.post_status = role == "0" ? "waiting" : "publish";
			post.raw_content = std::string(raw_content.data(), raw_content.length());

			size_t pos = raw_content.find_first_of("<");
			auto substr = raw_content.substr(0, pos<200 ? pos : 200);
			post.content_abstract = std::string(substr.data(), substr.length()) + "...";

			auto r = dao.update_object(post);
			if (r < 0)
				res.set_status_and_content(status_type::internal_server_error);
			else
				res.redirect("/home");
		}

		void my_post(request& req, response& res) {
			const auto& params = req.get_aspect_data();
			const auto& login_user_name = params[0];
			const auto& user_id = params[1];
			const auto& s = params[3];
			const auto& lens = params[4];

			Dao dao;
			auto v = dao.query<std::tuple<int>>("SELECT count(1) from pp_posts t1 where post_status = 'publish' "
				"AND t1.post_author=" + user_id);
			if (v.empty()) {
				res.set_status_and_content(status_type::internal_server_error);
				return;
			}

			size_t cur_page = atoi(s.data()) / 10 + 1;
			size_t total = std::get<0>(v[0]);

			std::string sql = "SELECT t1.*, t2.user_login from pp_posts t1, pp_user t2 "
				"where post_status = 'publish' AND t1.post_author=" + user_id + " AND t1.post_author = t2.ID ORDER BY t1.ID DESC LIMIT " + s + "," + lens;

			render_page(sql, req, res, "./purecpp/html/my_post.html", user_id, cur_page, total);
		}

		void search(request& req, response& res) {
			const auto& params = req.get_aspect_data();
			const auto& s = params[0];
			const auto& lens = params[1];
			const auto& key_word = params[2];

			Dao dao;
			std::string count_sql = "SELECT count(1) from pp_posts t1, pp_user t2 "
				"where post_status = 'publish' AND t1.post_author = t2.ID AND post_content like \"%" + key_word + "%\"" + " ORDER BY t1.ID DESC; ";

			auto v = dao.query<std::tuple<int>>(count_sql);
			if (v.empty()) {
				res.set_status_and_content(status_type::internal_server_error);
				return;
			}

			size_t cur_page = atoi(s.data()) / 10 + 1;
			size_t total = std::get<0>(v[0]);

			std::string sql = "SELECT t1.*, t2.user_login from pp_posts t1, pp_user t2 "
				"where post_status = 'publish' AND t1.post_author = t2.ID AND post_content like \"%" + key_word + "%\"" + " ORDER BY t1.ID DESC LIMIT " + s + "," + lens;

			render_page(sql, req, res, "./purecpp/html/search.html", key_word, cur_page, total);
		}

		void comment(request& req, response& res) {
			auto post_id_sv = req.get_query_value("post_id");
			auto post_id = sv2s(post_id_sv);
			auto comment_content = req.get_query_value("editorContent");

			pp_comment comment{};
			comment.comment_content = sv2s(comment_content);
			comment.comment_date = cur_time();
			comment.post_id = atoi(post_id.data());
			const auto& params = req.get_aspect_data();
			comment.user_id = atoi(params[1].data());
			comment.comment_status = "publish";

			Dao dao;
			int r = dao.add_object(comment);
			if (r < 0) {
				res.set_status_and_content(status_type::internal_server_error);
			}
			else {
				res.redirect("./detail?id=" + post_id);
			}
		}

		void remove_comment(request& req, response& res) {
			std::string id = sv2s(req.get_query_value("id"));
			std::string sql = "update pp_comment set comment_status='trash' where ID=" + id;

			Dao dao;
			bool r = dao.execute(sql);

			if (r) {
				std::string post_id = sv2s(req.get_query_value("post_id"));
				res.redirect("./detail?id="+ post_id);
			}
			else {
				res.set_status_and_content(status_type::internal_server_error);
			}
		}

		void login_page(request& req, response& res) {
			render_simple_page(req, res, "./purecpp/html/login.html");
		}

		void login(request& req, response& res) {
			const auto& params = req.get_aspect_data();
			const auto& user_name = params[0];
			const auto& password = params[1];

			std::string sql = "select ID, user_role from pp_user where user_login='" + user_name + "' and user_pass=md5('" + password+"')";
			Dao dao;
			auto r = dao.query<std::tuple<std::string, std::string>>(sql);
			if (r.empty()) {
				res.set_status_and_content(status_type::ok, "user name or password is not right");
				return;
			}

			std::string user_id = std::get<0>(r[0]);
			std::string user_role = std::get<1>(r[0]);
			auto session = req.get_session().lock();
			if (session == nullptr) {
				auto new_session = res.start_session();
				new_session->set_data("user_name", user_name);
				new_session->set_data("userid", user_id);
				new_session->set_data("user_role", user_role);
				new_session->set_max_age(-1);
			}
			else {
				if (session->get_data<std::string>("user_name").empty()) {
					session->set_data("user_name", user_name);
					session->set_data("userid", user_id);
					session->set_data("user_role", user_role);
				}
			}

			res.redirect("/home");
		}

		void quit(request& req, response& res) {
			auto ptr = req.get_session();
			auto session = ptr.lock();
			if (session) {
				session->set_max_age(0);
			}

			res.redirect("./home");
		}

		void sign_out_page(request& req, response& res) {
			render_simple_page(req, res, "./purecpp/html/sign_out.html");
		}

		void member_edit_page(request& req, response& res) {
			render_simple_page(req, res, "./purecpp/html/member_edit.html");
		}

		void member_edit(request& req, response& res) {
			const auto& params = req.get_aspect_data();
			const auto& login_user_name = params[0];
			const auto& old_pwd = params[3];
			const auto& new_pwd = params[4];

			std::string sql = "select ID from pp_user where user_login='" + login_user_name + "' and user_pass=md5('" + old_pwd + "')";
			Dao dao;
			auto r = dao.query<std::tuple<std::string>>(sql);
			if (r.empty()) {
				res.set_status_and_content(status_type::ok, "the old password is not right");
				return;
			}

			std::string id = std::get<0>(r[0]);
			std::string new_pwd_s = std::string(new_pwd.data(), new_pwd.length());
			std::string sql1 = "update pp_user set user_pass=md5('"+ new_pwd +"')"+" where ID="+id;
			bool result = dao.execute(sql1);
			if (result) {
				res.redirect("/home");
			}
			else {
				res.set_status_and_content(status_type::bad_request);
			}
		}

		void sign_out(request& req, response& res) {
			const auto& params = req.get_aspect_data();
			const auto& user_name = params[0];
			const auto& email = params[1];
			const auto& answer = params[2];
			const auto& pwd = params[3];

			std::string sql = "select count(1) from pp_user where user_login='" + user_name + "' or user_email='" + email + "'";
			Dao dao;
			auto r = dao.query<std::tuple<int>>(sql);
			if (std::get<0>(r[0]) > 0) {
				res.set_status_and_content(status_type::ok, "用户名或邮箱已经存在");
				return;
			}

			auto r1 = dao.query<std::tuple<int>>("select count(1) from pp_sign_out_answer where ID=1 and answer like \"%"+ answer+"%\"");
			if (std::get<0>(r1[0]) == 0) {
				res.set_status_and_content(status_type::ok, "验证问题答案错误");
				return;
			}
			
			std::string data_hex_digest = md5::md5_string(pwd);

			pp_user user = {};
			user.user_email = email;
			user.user_login = user_name;
			user.user_nickname = user_name;
			user.user_pass = std::move(data_hex_digest);

			int ret = dao.add_object(user);
			if (ret<0) {
				res.set_status_and_content(status_type::internal_server_error);
				return;
			}
			
			res.redirect("./login_page");
		}

		void add_post_page(request& req, response& res) {
			render_simple_page(req, res, "./purecpp/html/create_post.html");
		}

		void add_post(request& req, response& res) {
			const auto& params = req.get_aspect_data();
			const auto& login_user_name = params[0];
			const auto& user_id = params[1];
			const auto& role = params[1];

			auto title = req.get_query_value("title");
			auto type = req.get_query_value("type");
			auto post_content = req.get_query_value("post_content");
			auto raw_content = req.get_query_value("md_post_content");

			Dao dao;
			pp_posts post{};
			post.post_title = sv2s(title);
			post.category = sv2s(type);
			post.post_author = atoi(user_id.data());
			post.post_date = cur_time();
			post.post_modified = post.post_date;
			post.post_content = sv2s(post_content);
			post.post_status = /*role == "0" ? "waiting" : */"publish";
			post.raw_content = sv2s(raw_content);

			size_t pos = raw_content.find_first_of("<");
			auto substr = raw_content.substr(0, pos<200?pos:200);
			post.content_abstract = std::string(substr.data(), substr.length()) + "...";
			
			auto r = dao.add_object(post);
			if (r < 0) {
				res.set_status_and_content(status_type::internal_server_error);
			}				
			else {
				total_post_count_++;
				res.redirect("/home");
			}
		}

	private:
		void render_page(const std::string& sql, request& req, response& res, std::string html_file, std::string categroy, size_t cur_page=0, size_t total=0) {
			Dao dao;
			auto v = dao.query<std::tuple<pp_posts, std::string>>(sql);
			if (v.empty()) {
				res.set_status_and_content(status_type::ok, "");
				return;
			}

			const auto& params = get_user_info(req);
			const auto& login_user_name = params.empty() ? "" : params[0];
			const auto& user_role = params.empty() ? "" : params[2];

			bool is_admin = (!login_user_name.empty()) && (user_role == "3" || user_role == "6");

			nlohmann::json article_list;
			for (auto& o : v) {
				nlohmann::json item;
				auto& post = std::get<0>(o);
				item["ID"] = post.ID;
				item["post_author"] = post.post_author;
				item["content_abstract"] = post.content_abstract;
				item["post_date"] = post.post_date;
				item["post_title"] = post.post_title;
				item["category"] = category_map_[post.category];
				item["comment_count"] = post.comment_count;

				std::string user_login = std::get<1>(o);
				item["user_login"] = user_login;
				item["is_owner"] = is_admin?false:login_user_name == user_login;
				item["is_admin"] = is_admin;
				article_list.push_back(item);
			}

			if (total == 0) {
				total = v.size();
			}

			nlohmann::json result;
			result["article_list"] = article_list;
			result["has_login"] = !login_user_name.empty();
			result["login_user_name"] = login_user_name;
			result["current_page"] = cur_page;
			result["total"] = total;
			result["category"] = categroy;
			res.add_header("Content-Type", "text/html; charset=utf-8");
			res.set_status_and_content(status_type::ok, render::render_file(html_file, result));
		}

		void render_simple_page(request& req, response& res, std::string html_file) {
			auto login_user_name = get_user_name_from_session(req);
			nlohmann::json result;
			result["has_login"] = !login_user_name.empty();
			result["login_user_name"] = login_user_name;
			res.add_header("Content-Type", "text/html; charset=utf-8");
			res.set_status_and_content(status_type::ok, render::render_file(std::move(html_file), result));
		}

		private:
			std::atomic<size_t> total_post_count_ = 0;
			std::map<std::string, std::string> category_map_;
	};
}
