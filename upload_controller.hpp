//
// Created by root on 4/26/18.
//

#ifndef FEATHER_UPLOAD_CONTROLLER_HPP
#define FEATHER_UPLOAD_CONTROLLER_HPP
namespace feather{
    class upload_controller{
    public:
        void upload(request& req,response &res)
        {
			assert(req.get_content_type() == content_type::multipart);
			std::string filenames = "";
			auto& files = req.get_upload_files();
			for (auto& file : files) {
				filenames+= file.get_file_path();
			}
			res.render_string("multipart finished "+ std::move(filenames));
        }
    };
}
#endif //FEATHER_UPLOAD_CONTROLLER_HPP
