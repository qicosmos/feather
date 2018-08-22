# Feather--一个快速开发的web服务器框架

Feather是一个适合快速开发的modern c++ web框架，Feather的目标是让使用者以最小的精力和成本来开发一个web网站。

现在很多web框架都非常庞大，学习成本高，而Feather正是为了解决这些问题的，它就像它的名字feather那样轻盈，所有的一切都是为了让用户非常方便又快速地开发而不是陷入到框架的细节当中。

一个好的框架应该是能让用户轻松实现自己的想法而不是成为框架的奴隶。如果你希望轻松快速地开发一个web框架，而无需花费大量的精力去学习框架细节的话，那么Feather非常适合你！

## Feather是什么？

Feather作为一个快速开发框架，它的目标是让web服务器开发变得简单。它主要有下面几个特点：

1. 简洁易用
2. 高性能，modern c++（c++17）开发
3. header only
4. 跨平台
5. 支持编译期反射
6. 支持AOP

Feather框架的核心包括：
1. 一个易用的http库，支持http1.x, https, websocket
2. 一个功能强大的html模版引擎
3. 一个可扩展的ORM库，支持多种数据库（mysql,postgresql，sqlite）
4. 一个可扩展序列化库，支持多种格式(json, xml)

## Feather的架构

下面是Feather的架构图:

![Feather的架构图](https://github.com/qicosmos/feather/blob/master/framework.png)：

1. http组件：  cinatra
2. ORM组件：   ormpp
3. 序列化组件： iguana
4. html 模版： inja

Feather内部的四个核心组件是松耦合的，Feather只是把它们像搭积木一样搭在一起。

## Feather的使用

以一个简单的例子展示如何使用Feather，比如显示文章列表，几乎每个网站都有这个功能。

获取文章列表的功能很简单，它底层数据部分是一个文章表，业务逻辑层是获取这些列表，然后做html渲染。对外接口部分是一个http服务，前端后端交互的数据格式为json。为了简单起见就不考虑缓存之类的。

那么要实现这个文章列表功能用Feather怎么做呢？可以按照这些步骤去开发：

1. 提供获取文章列表的http接口；
2. 通过ORM提供数据库访问功能；
3. 编写业务逻辑，根据用户请求获取文章列表并通过html模版做渲染；

接下来看看这些步骤具体是怎么做的。

### 获取文章列表的http接口

	const int max_thread_num = 4;
	http_server server(max_thread_num);
	server.listen("0.0.0.0", "http");

	article_controller article_ctl;
	server.set_http_handler<GET, POST>("/get_article_list", &article_controller::get_article_list, &article_ctl);

其中login接口是这样的：

	class article_controller{
	public:
		void get_article_list(const cinatra::request& req, cinatra::response& res){
            //todo
            res.set_status_and_content(status_type::ok);
        }
	};
接下来就可以测试这个http服务接口了，客户端只要发送一个http请求即可。比如发送一个这样的请求
http://127.0.0.1/get_article_list
服务器会自动路由到article_controller::get_article_list函数中，如果请求不对则会返回http错误给客户端。当服务器收到这样的请求之后就表明服务器提供的这个http服务是可用的。

接下来需要编写数据库部分的代码，由于有了ORM，所以你可以很方便地编写数据库部分的代码了，同样很简单。


### 通过ORM提供数据库访问功能
登录业务涉及到一个用户表，因此我们需要创建这个表，不过在创建数据库之前先确定你选用什么数据库，Feather的ORM支持mysql, postgresql和sqlite三种数据库，假设我们的数据库是mysql。我们可以通过下面的代码来创建一个用户表。

1.创建文章表

	struct article{
        int id;
        std::string title;
        std::string introduce;
        int user_id;
        int visible;
        std::string create_time;
    };
    REFLECTION(article, id, title, introduce, user_id, visible, create_time);

	dbng<mysql> mysql;
	mysql.connect("127.0.0.1", "root", "12345", "testdb")
	mysql.create_table<article>(ormpp_auto_key{ ID(article::id) });

dao.create_table< article >将会在testdb数据库中自动创建一个article表，其中id字段是自增长的。

2.编写获取文章列表的逻辑（包含访问数据库）

	void get_article_list(const cinatra::request& req, cinatra::response& res){
		auto page_s = req.get_query_value("page");
        int current_page = atoi(page_s.data());

        dao_t<dbng<mysql>> dao;
        std::vector<article> v;
        dao.get_object(v);
            
        article_list_res list{std::move(v), current_page, total_page(v.size())};

        iguana::string_stream ss;
        iguana::json::to_json(ss, list);

        res.add_header("Access-Control-Allow-origin", "*");
		res.set_status_and_content(status_type::ok, ss.str());
    }

访问数据库，序列化为json返回给客户端。

详细的例子你可以看[github上的代码](https://github.com/qicosmos/feather)。

## Demo示例

我们用Feather开发了一个社区网站，地址在这里：http://119.61.16.32:8080/

Feather社区：
![Feather社区](https://github.com/qicosmos/feather/blob/master/demo.png)

## benchmark
ab -c50 -n3000 xxxurl

![feather benchmark](https://github.com/qicosmos/feather/blob/master/qps.png)

## 致谢

Feather社区网站由我和网友[XMH](https://github.com/xmh0511/)共同开发完成（花了两天的业余时间），XMH也是cinatra的忠实用户，不但贡献了很多代码，还提供了很多宝贵的建议，在此表示衷心的感谢！

XMH是一名热爱编程的程序猿，平时从事c++，web，移动端等开发。从事过游戏后台和APP开发。也是个忠实的mordern c++粉丝，追随着cpp的发展，喜欢通过元编程做一些小工具。热爱开源社区，也是开源项目feather的使用者，希望feather能为更多的开发者所使用，社区发展越来越好。

希望有更多人能加入进来把Feather完善得更好。

## 联系我们

purecpp@163.com

[http://purecpp.org/](http://purecpp.org/ "purecpp")

[https://github.com/qicosmos/feather](https://github.com/qicosmos/feather "feather")
