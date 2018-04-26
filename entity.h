//
// Created by root on 4/24/18.
//

#ifndef FEATHER_ENTITY_H
#define FEATHER_ENTITY_H

namespace feather{
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
}

#endif //FEATHER_ENTITY_H
