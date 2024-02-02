# Feather--一个快速开发的web框架

*Read this in other languages: [English](https://github.com/qicosmos/feather/blob/master/README.en.md), [简体中文](https://github.com/qicosmos/feather/blob/master/README.md)

[feather使用](https://github.com/qicosmos/feather/blob/master/docs/featehr%20%E4%BD%BF%E7%94%A8.md)

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
3. html 模版： render

Feather内部的三个核心组件是松耦合的，Feather只是把它们像搭积木一样搭在一起。

## Feather的使用

以一个简单的例子展示如何使用Feather，比如显示文章列表，几乎每个网站都有这个功能。

获取文章列表的功能很简单，它底层数据部分是一个文章表，业务逻辑层是获取这些列表，然后做html渲染。对外接口部分是一个http服务，前端后端交互的数据格式为json。为了简单起见就不考虑缓存之类的。

那么要实现这个文章列表功能用Feather怎么做呢？可以按照这些步骤去开发：

1. 提供获取文章列表的http接口；
2. 通过ORM提供数据库访问功能；
3. 编写业务逻辑，根据用户请求获取文章列表并通过html模版做渲染；

## Demo示例

TODO

我们用Feather开发了一个社区网站，地址在这里：http://purecpp.org/

Feather社区：
![Feather社区](https://github.com/qicosmos/feather/blob/master/demo.png)

## benchmark
ab -c50 -n3000 xxxurl

![feather benchmark](https://github.com/qicosmos/feather/blob/master/qps.png)

## 致谢

Feather社区网站由我和网友[XMH](https://github.com/xmh0511/)共同开发完成（花了两天的业余时间），XMH也是cinatra的忠实用户，不但贡献了很多代码，还提供了很多宝贵的建议，在此表示衷心的感谢！

希望有更多人能加入进来把Feather完善得更好。

## 联系我们

purecpp@163.com

[http://purecpp.org/](http://purecpp.org/ "purecpp")

[https://github.com/qicosmos/feather](https://github.com/qicosmos/feather "feather")
