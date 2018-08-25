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
}