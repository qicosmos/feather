//
// Created by root on 4/24/18.
//

#ifndef FEATHER_ENTITY_H
#define FEATHER_ENTITY_H

namespace feather{
	struct pp_posts {
		int ID; 
		int post_author; 
		std::string post_date; 
		std::string post_title; 
		std::string post_content;
		std::string post_status; 
		std::string post_modified; 
		std::string content_abstract; 
		std::string url;
		int comment_count;
		std::string category;
		std::string raw_content;
	};
	REFLECTION(pp_posts, ID, post_author, post_date, post_title, post_content, post_status, post_modified, content_abstract, url, comment_count, category, raw_content);

	struct pp_user {
		int ID; 
		std::string user_login; 
		std::string user_nickname; 
		std::string user_email; 
		std::string user_registered; 
		std::string user_icon;
		std::string user_pass;
		int user_role;
	};
	REFLECTION(pp_user, ID, user_login, user_nickname, user_email, user_registered, user_icon, user_pass, user_role);

	struct pp_comment {
		int ID; 
		int user_id; 
		int post_id;
		int comment_parant;
		std::string comment_content; 
		std::string comment_date;
		std::string comment_status;
	};
	REFLECTION(pp_comment, ID, user_id, post_id, comment_parant, comment_content, comment_date, comment_status);

	struct pp_terms {
		int term_id;
		std::string name;
		std::string slug;
		std::string term_group;
	};
	REFLECTION(pp_terms, term_id, name, slug, term_group);

	struct pp_post_views {
		int ID; 
		int type; 
		int count; 
		std::string period;
	};
	REFLECTION(pp_post_views, ID, type, count, period);

    struct user{
        int id;
        std::string		user_name;
        std::string		nick_name;
        std::string     token;
        int32_t			gender;
        int32_t			role;
        std::string		avatar;
        int64_t			phone;
        std::string		email;
        std::string		qq;
        std::string		location;
    };
    REFLECTION(user, id, user_name, nick_name, token, gender, role, avatar, phone, email, qq, location);

    struct article{
        int id;
        std::string title;
        std::string introduce;
        int user_id;
        int visible;
        std::string create_time;
    };
    REFLECTION(article, id, title, introduce, user_id, visible, create_time);

    struct article_detail{
        int id;
        int parant_id;
        std::string title;
        std::string content;
        std::string update_time;
    };
    REFLECTION(article_detail, id, parant_id, title, content, update_time);

	struct cncppcon2018_user {
		int id;
		std::string		user_name;
		int64_t		    phone;
		std::string		email;
		std::string		user_group;
		std::string		join_time;
	};
	REFLECTION(cncppcon2018_user, id, user_name, phone, email, user_group, join_time);

	struct visit_counter {
		int id;
		std::string save_hour;
		int counter;
	};
	REFLECTION(visit_counter, id, save_hour, counter);
}

#endif //FEATHER_ENTITY_H
