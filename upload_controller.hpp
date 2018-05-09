//
// Created by root on 4/26/18.
//

#ifndef FEATHER_UPLOAD_CONTROLLER_HPP
#define FEATHER_UPLOAD_CONTROLLER_HPP
namespace feather{
    class upload_controller{
    public:
        void upload(const request& req,response &res)
        {
            assert(req.get_content_type() == content_type::multipart);
            auto state = req.get_state();
            switch (state)
            {
                case cinatra::data_proc_state::data_begin:
                {
                    auto file_name_s = req.get_multipart_file_name();
                    init_file_path(req.get_conn()->static_dir(), file_name_s);
                    auto save_path = std::string(".")+save_file_name_;
                    auto file = std::make_shared<std::ofstream>(save_path, std::ios::binary);
                    if (!file->is_open()) {
                        std::cout<<"file error"<<std::endl;
                        res.set_continue(false);
                        return;
                    }else{
                        res.set_continue(true);
                    }

                    req.get_conn()->set_tag(file);
                }
                    break;
                case cinatra::data_proc_state::data_continue:
                {
                    if (!res.need_continue()) {
                        //std::cout<<"do not need"<<std::endl;
                        return;
                    }

                    auto file = std::any_cast<std::shared_ptr<std::ofstream>>(req.get_conn()->get_tag());
                    auto part_data = req.get_part_data();
                    file->write(part_data.data(), part_data.length());
                }
                    break;
                case cinatra::data_proc_state::data_end:
                {
                    //std::cout << "one file finished" << std::endl;
                }
                    break;
                case cinatra::data_proc_state::data_all_end:
                {
                    //all the upstream end
                    //std::cout << "all files finished" << std::endl;
                    nlohmann::json  json;
                    json["error"] = 0;
                    json["url"] = "http://120.78.135.113:8080"+save_file_name_;
                    res.add_header("Access-Control-Allow-origin","*");
                    res.set_status_and_content(status_type::ok,json.dump());
                }
                    break;
                case cinatra::data_proc_state::data_error:
                {
                    //network error
                }
                    break;
            }
        }
    private:
        void init_file_path(const std::string& dir, std::string_view file_name){
            save_file_name_ = dir+std::to_string(std::time(nullptr))+std::string(file_name.data(), file_name.length());
        }

        std::string save_file_name_;
    };
}
#endif //FEATHER_UPLOAD_CONTROLLER_HPP
