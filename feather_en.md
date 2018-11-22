# feather

* [introduction](#introduction)
* [how to build]()
* [quick example]()
* [note]()
* [roadmap](#roadmap)
* [contact us]()

# introduction

Feather is a rapid modern c++(c++17) web development framework.The goal of feather is to develop a web application efficiently, low learning cost.

**Features of feather**

1. easy to use
2. header only
3. cross-platform
4. high performance
5. support AOP

**components of feather**

Feather is made of three core components:

1. http component--cinatra

	cinatra is an easy to use, header only http server, cinatra support http1.1, web socket, ssl.

2. database component--ormpp

	the goal of ormpp is to simplify database programming in C++. ormpp provides easy to use unified interface, supports multiple databases, and reduces the learning cost of using the database.

	ormpp supports three databases currently: mysql, postgresql and sqlite, it can be extended to support more databases.

3. html render component--render

	render helps to render html pages, such as fill dynamic data into the html template pages, control the UI display, reuse the same html code.

All the components are independent, feather just make them work together.

Feather is a classical MVC framework, you just need focus on the business logic in controller, in a controller function you can get http request fields and data and visit the database, at last return the rendered html page to the client.

# how to build

Feather is based on asio, so you'd better install boost, howerver it is not necessary, you can also use AsioStandalone.

The http component cinatra rely on the uuid library, so you uuid library is necessary.
It also rely on zlib, but zlib is  necessary otherwise you want to use gzip.

The database component ormpp rely on some database connector.

If you want to use ssl, you also need install ssl library, it is not necessary otherwise you want to use ssl.

**Debian linux**

1.install mysql client: sudo apt-get install libmysqlclient-dev

2.install postgresql client: sudo apt-get install libpq-dev

3.install sqlite3: sudo apt-get install libsqlite3-dev

4.install uuid: sudo apt-get install uuid

5.install zlib: sudo apt-get install zlib1g-dev

6.install openssl: sudo apt-get install libssl-dev


**Centos（you'd better prepare centos7.0+）**

1.install mysql client: sudo yum install mysql++-devel.x86_64

2.install postgresql: sudo yum install postgresql-devel.x86_64

3.install sqlite3: sudo yum install sqlite-devel.x86_64

4.install uuid: sudo yum install uuid-devel.x86_64

5.install zlib: sudo yum install zlib-devel.x86_64

6.install openssl: sudo yum install openssl-devel.x86_64

**build**

git clone https://github.com/qicosmos/feather.git

git submodule update --init

mkdir build

cd build

make -j

# quick example

## hello world

	#include "feather.h"
	
	using namespace feather;
	using namespace ormpp;
	using namespace cinatra;

	int main() {
		int max_thread_num = std::thread::hardware_concurrency();
		http_server server(max_thread_num);
		server.listen("0.0.0.0", "8080");
		server.set_http_handler<GET, POST>("/", [](request& req, response& res) {
			res.set_status_and_content(status_type::ok, "hello world");
		});

		server.run();
		return 0;
	}

request url: http://127.0.0.1:8080/

response: hello world

**note**

you can handle both post and get request from the client in above example, because you have assigned the verbs with POST and GET. 

Feather provides GET, POST, DEL, HEAD, PUT, CONNECT, TRACE, OPTIONS for you. You could choose 0-n arbitrary verbs for the request handle function as your wish.

So you could reuse the same request handle function very easily.

## request

**note**

All the fields and data types from parsed http request are std::string_view, no memory copy, no memory move, enough convenient algorithms. I really love this feature from c++17. I think it is should be used in all networking programmes.  

you can get header fields from request object.

	server.set_http_handler<GET, POST>("/test", [](request& req, response& res) {
		auto name = req.get_header_value("name");
		if (name.empty()) {
			res.render_string("no name");
			return;
		}

		res.render_string("hello world");
	});
you can get a header value by a case-insensitive key from http headers.

	auto name = req.get_header_value("name");

If the header is not exist the returned value will be empty.

There is a similar method get_query_value to get query values.

	auto id = req.get_query_value("id");
	if (id.empty()) {
		res.render_404();
		return;
	}

You can also use get_query_value to get x-www-form-urlencoded value.

## response

In above example you can see 

	res.render_string("hello world");

Indeed it's a wrapper of 

	res.set_status_and_content(status_type::ok, "hello world", res_content_type::string);

It lets you write fewer code.

This is res_content_type:

	enum class res_content_type{
		html,
		json,
		string,
        none
	};

The default res_content_type in set_status_and_content is none. You can set the res_content_type as json.

	server.set_http_handler<GET, POST,OPTIONS>("/json", [](request& req, response& res) {
		nlohmann::json json;
        res.add_header("Access-Control-Allow-Origin","*");
		if(req.get_method()=="OPTIONS"){
            res.add_header("Access-Control-Allow-Headers","Authorization");
            res.render_string("");
		}else{
            json["abc"] = "abc";
            json["success"] = true;
            json["number"] = 100.005;
            json["time_stamp"] = std::time(nullptr);
            res.render_json(json);
		}
	});

The above exmpale show how to response a json string or a normal string, further more you could add headers into the response.

	res.render_json(json);
	it is same with
	res.set_status_and_content(status_type::ok, json, res_content_type::json);

req.get_method() will return http verbs such as POST, GET, OPTIONS etc.

How to deal with gzip?

Don't worry，we have supported it already.

If you want to use gzip to compress the response string, just need set the content_encoding type as gzip,

	res.set_status_and_content(status_type::ok, "hello world", res_content_type::none, content_encoding::gzip);

the framework will use gzip compress the response string for you, comfortable and sweet right? :)

I think you have know enough about request and response(If you want to more, please tell me).

## file upload/download

## session/cookie

## AOP

## ORM

## render

## real example

# contact us




