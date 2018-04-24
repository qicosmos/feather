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
        int visible;
        std::string content;
        std::string abstract;
        int user_id;
        int64_t post_time;
    };
    REFLECTION(article, id, visible, content, abstract, user_id, post_time);
}

#endif //FEATHER_ENTITY_H
