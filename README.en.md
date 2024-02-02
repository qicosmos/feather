# Feather

*Read this in other languages: [English](https://github.com/qicosmos/feather/blob/master/README.en.md), [简体中文](https://github.com/qicosmos/feather/blob/master/README.md)*

* [Introduction](#Introduction)
* [How to build](#Build)
* [Quick example](#Example)
* [Contact us](#Contact)

# Introduction

Feather is a rapid modern C++ web development framework, written in C++17. The goal of Feather is to develop web applications quickly, with a low barrier to entry.

Here is a presentation by Yu Qi on Feather from CppCon 2018: https://www.youtube.com/watch?v=DoXXCZJVNeo.

**Features of Feather**

1. Easy to use
2. Header only
3. Cross-platform
4. High performance
5. Supports [AOP](https://en.wikipedia.org/wiki/Aspect-oriented_programming)

**Components of Feather**

Feather is made of three core components:

1. HTTP component: **cinatra**

	**cinatra** is an easy to use, header only HTTP server. **cinatra** supports HTTP/1.1, web sockets, and SSL.

2. Database component: **ormpp**

	The goal of **ormpp** is to simplify database programming in C++. **ormpp** provides an easy to use unified interface, supports multiple databases, and reduces the learning cost of using a database.

	**ormpp** currently supports three databases: **mysql**, **postgresql**, and **sqlite**. It can be extended to support more databases.

3. HTML render component: **render**

	**render** helps to render HTML pages, such as filling dynamic data into HTML template pages, controlling the UI display, and reusing HTML code.

All the components are independent. Feather just make them work together.

Feather is a classical MVC framework, you just need to focus on your business logic in the controller. In a controller function, you can get HTTP request fields, data, and visit the database. The controller then returns the rendered HTML page to the client.

# Build

Feather is based on **asio**, so it's recommended to install Boost. However this is not strictly necessary, as you can also use **AsioStandalone**.

The HTTP component **cinatra** relies on the **uuid** library, so please install it.
It also relies on **zlib**, but **zlib** is unnecessary if you want to use **gzip**. If you want to use **gzip** please `SET(ENABLE_GZIP ON)` in **CMakeLists.txt**. The default value is `OFF`.

The database component **ormpp** relies on a database connector.

If you want to use SSL, you also need to install a SSL library. This is not necessary if you don't want to use SSL.

**Supported compilers**
* **gcc7.1+**
* **msvc2017 15.7.1+**

**Debian Linux**

1. Install **mysql** client: `sudo apt-get install libmysqlclient-dev`

2. Install **postgresql** client: `sudo apt-get install libpq-dev`

3. Install **sqlite3**: `sudo apt-get install libsqlite3-dev`

4. Install **uuid**: `sudo apt-get install uuid`

5. Install **zlib**: `sudo apt-get install zlib1g-dev`

6. Install **openssl**: `sudo apt-get install libssl-dev`


**CentOS（version 7+ required）**

1. Install **mysql** client: `sudo yum install mysql++-devel.x86_64`

2. Install **postgresql**: `sudo yum install postgresql-devel.x86_64`

3. Install **sqlite3**: `sudo yum install sqlite-devel.x86_64`

4. Install **uuid**: `sudo yum install uuid-devel.x86_64`

5. Install **zlib**: `sudo yum install zlib-devel.x86_64`

6. Install **openssl**: `sudo yum install openssl-devel.x86_64`

**Windows**

1. Install the **mysql** connector if you use MySQL
2. Install the **postgresql** connector if you use Postgres
3. Install **sqlite3** if you use it

**Build**

```
git clone https://github.com/qicosmos/feather.git

git submodule update --init

cmake -B build

cmake --build build
```

# Example

## Hello world

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

The request URL is: http://127.0.0.1:8080/. The response will be: `hello world`.

You can handle both `POST` and `GET` requests from the client in the above example, because you have assigned the verbs `POST` and `GET`. 

Feather provides `GET`, `POST`, `DEL`, `HEAD`, `PUT`, `CONNECT`, `TRACE`, `OPTIONS` for you. You could choose 0-n arbitrary verbs for the request handler function as you wish.

So you could reuse the same request handler function very easily.

## HTTP Request

All the fields and data types from a parsed HTTP request are `std::string_view`. There are no memory copies, no memory moves, and there are enough convenient algorithms. I really love this feature from C++17. I think it should be used in all networking programs.  

You can get header fields from the request object:

	server.set_http_handler<GET, POST>("/test", [](request& req, response& res) {
		auto name = req.get_header_value("name");
		if (name.empty()) {
			res.render_string("no name");
			return;
		}

		res.render_string("hello world");
	});

You can get a header value by a case-insensitive key from the HTTP headers:

	auto name = req.get_header_value("name");

If the header does not exist, the returned value will be empty.

There is a similar method `get_query_value` to get query values:

	auto id = req.get_query_value("id");
	if (id.empty()) {
		res.render_404();
		return;
	}

You can also use `get_query_value` to get `x-www-form-urlencoded` value.

## HTTP Response

In the above example, you can see:

	res.render_string("hello world");

It's a wrapper for:

	res.set_status_and_content(status_type::ok, "hello world", res_content_type::string);

It lets you write less code.

Here is `res_content_type`:

	enum class res_content_type{
		html,
		json,
		string,
        none
	};

The default `res_content_type` in `set_status_and_content` is `none`. You can set the `res_content_type` as `json`:

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

The above example shows how to respond with a JSON string or a normal string. Furthermore you could add headers to the response:

	res.render_json(json);
	// It is the same as:
	res.set_status_and_content(status_type::ok, json, res_content_type::json);

`req.get_method()` will return HTTP verbs such as `POST`, `GET`, `OPTIONS`, etc.

How do we deal with **gzip**? Don't worry，we support it already. If you want to use **gzip** to compress the response string, just set the `content_encoding` type to `gzip`:

	res.set_status_and_content(status_type::ok, "hello world", res_content_type::none, content_encoding::gzip);

The framework will compress the response string for you. Comfortable and sweet, right? :)

I think you now know enough about requests and responses. If you want to know more, please ask me.

## File upload/download

TODO

## Sessions and cookies

TODO

## AOP

TODO

## Cache 

TODO

## Configuration

TODO

## ORM

TODO

## Render

TODO

## Real example

TODO

# Contact

TODO
