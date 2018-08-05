//
// Created by root on 4/26/18.
//

#ifndef FEATHER_STATIC_RES_CONTROLLER_HPP
#define FEATHER_STATIC_RES_CONTROLLER_HPP
namespace feather{
    class static_res_controller{
    public:
        void static_resource(cinatra::request& req, cinatra::response& res){
            auto fileName =req.get_res_path();
            if (fileName.empty()) {
                res.set_status_and_content(cinatra::status_type::bad_request);
                return;
            }

            std::string file_path = std::string(fileName.data(), fileName.length());
            std::ifstream file(file_path, std::ios_base::binary);
            if(!file.is_open()){
                res.set_status_and_content(cinatra::status_type::bad_request);
                return;
            }

            file.seekg(0, std::ios_base::end);
            size_t size = file.tellg();
            if(size>3*1024*1024){
                res.set_status_and_content(cinatra::status_type::bad_request);
                return;
            }

            file.seekg(0, std::ios_base::beg);
            std::string str;
            str.resize(size);

            file.read(str.data(), size);
            res.set_status_and_content(status_type::ok, std::move(str), res_content_type::none, content_encoding::gzip);
        }
    };
}
#endif //FEATHER_STATIC_RES_CONTROLLER_HPP
