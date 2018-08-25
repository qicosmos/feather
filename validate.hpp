#pragma once
#include "feather.h"
#include "util.hpp"
using namespace cinatra;

namespace feather {

	inline std::vector<std::string> get_user_info(request& req) {
		auto ptr = req.get_session();
		auto session = ptr.lock();
		if (session == nullptr) {
			return {};
		}

		return { session->get_data<std::string>("user_name"), session->get_data<std::string>("userid"),
			session->get_data<std::string>("user_role") };
	}

	inline std::string get_value_from_session(request& req, const std::string& key) {
		auto ptr = req.get_session();
		auto session = ptr.lock();
		if (session == nullptr) {
			return "";
		}

		return session->get_data<std::string>(key);
	}

	inline std::string get_user_name_from_session(request& req) {
		return get_value_from_session(req, "user_name");
	}

	struct check_login {
		bool before(request& req, response& res) {
			auto v = get_user_info(req);
			if (v.empty() || v[0].empty()) {
				res.set_status_and_content(status_type::bad_request, "ÇëÏÈµÇÂ¼");
				return false;
			}
			req.set_aspect_data(std::move(v));
			return true;
		}
	};

	struct check_comment_input {
		bool before(request& req, response& res) {
			auto post_id = req.get_query_value("post_id");
			if (!is_integer(post_id)) {
				res.set_status_and_content(status_type::bad_request);
				return false;
			}

			return true;
		}
	};

	inline bool check_integer(request& req, response& res, std::string_view key) {
		auto val = req.get_query_value(key);
		if (val.empty()) {
			res.set_status_and_content(status_type::bad_request);
			return false;
		}

		if (!is_integer(val)) {
			res.set_status_and_content(status_type::bad_request);
			return false;
		}

		return true;
	}

	template<typename... T>
	inline bool check_integers(request& req, response& res, T... key) {
		bool r = true;
		((r&&(r=check_integer(req, res, key))), ...);
		return r;
	}

	struct check_remove_comment_input {
		bool before(request& req, response& res) {
			return check_integers(req, res, "id", "post_id");
		}
	};

	struct check_detail_input {
		bool before(request& req, response& res) {
			return check_integer(req, res, "id");
		}
	};

	inline bool set_start_end(request& req, response& res, std::string& s, std::string& lens) {
		auto start = req.get_query_value("start");
		auto len = req.get_query_value("len");

		if (!start.empty() && !start.empty()) {
			if (!check_integers(req, res, "start", "len"))
				return false;

			s = sv2s(start);
			lens = sv2s(len);
		}

		return true;
	}

	struct check_category_input {
		bool before(request& req, response& res) {
			std::string s = "0";
			std::string lens = "10";

			if (!set_start_end(req, res, s, lens))
				return false;

			if (!check_integer(req, res, "category")) {
				return false;
			}

			std::string category = sv2s(req.get_query_value("category"));
			req.set_aspect_data(std::move(s), std::move(lens), std::move(category));
			return true;
		}
	};

	struct check_search_input {
		bool before(request& req, response& res) {
			std::string s = "0";
			std::string lens = "10";

			if (!set_start_end(req, res, s, lens))
				return false;

			auto key_word = req.get_query_value("category");
			if (key_word.empty()) {
				res.set_status_and_content(status_type::bad_request);
				return false;
			}

			if (has_special_char(key_word)) {
				res.set_status_and_content(status_type::bad_request);
				return false;
			}

			req.set_aspect_data(std::move(s), std::move(lens), sv2s(key_word));
			return true;
		}
	};

	struct check_start_end_input {
		bool before(request& req, response& res) {
			std::string s = "0";
			std::string lens = "10";

			if (!set_start_end(req, res, s, lens))
				return false;

			req.set_aspect_data(std::move(s), std::move(lens));
			return true;
		}
	};

	template<typename T>
	inline bool check_input0(response& res, T t) {
		if (t.empty()) {
			res.set_status_and_content(status_type::bad_request);
			return false;
		}

		if (has_special_char(t)) {
			res.set_status_and_content(status_type::bad_request);
			return false;
		}

		return true;
	}

	template<typename... T>
	inline bool check_input(response& res, T... args) {
		bool r = true;
		((r = r && check_input0(res, args)), ...);

		return r;
	}

	struct check_login_input {
		bool before(request& req, response& res) {
			auto user_name = req.get_query_value("user_name");
			auto password = req.get_query_value("password");
			if (!check_input(res, user_name, password)) {
				return false;
			}

			req.set_aspect_data(sv2s(user_name), sv2s(password));
			return true;
		}
	};

	struct check_sign_out_input {
		bool before(request& req, response& res) {
			auto user_name = req.get_query_value("user_name");
			auto email = req.get_query_value("email");
			auto answer = req.get_query_value("answer");
			auto pwd = req.get_query_value("user_pwd");

			if (!check_input(res, user_name, answer, pwd)) {
				return false;
			}

			if (email.empty()) {
				res.set_status_and_content(status_type::bad_request);
				return false;
			}

			if (has_special_char(email, true)) {
				res.set_status_and_content(status_type::bad_request);
				return false;
			}

			req.set_aspect_data(sv2s(user_name), sv2s(email), sv2s(answer), sv2s(pwd));
			return true;
		}
	};

	struct check_member_edit_input {
		bool before(request& req, response& res) {
			auto old_password = req.get_query_value("old_password");
			auto new_pwd = req.get_query_value("new_password");
			if (new_pwd.empty()) {
				res.set_status_and_content(status_type::bad_request);
				return false;
			}

			if (!check_input(res, old_password, new_pwd)) {
				return false;
			}

			req.set_aspect_data(sv2s(old_password), sv2s(new_pwd));
			return true;
		}
	};

	struct check_edit_post_input {
		bool before(request& req, response& res) {
			if (!check_integer(req, res, "id")) {
				return false;
			}

			req.set_aspect_data(sv2s(req.get_query_value("id")));
			return true;
		}
	};
}