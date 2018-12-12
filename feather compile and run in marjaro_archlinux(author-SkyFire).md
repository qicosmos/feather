# feather 使用（majaro/archlinux）

1. 查看系统版本

   ```shell
   uname -srvmpio
   ```

   本机输出：

   ```text
   Linux 4.14.85-1-MANJARO #1 SMP PREEMPT Sat Dec 1 12:18:09 UTC 2018 x86_64 unknown unknown GNU/Linux
   ```

2. 安装依赖库

   ```shell
   sudo pacman -S boost libutil-linux mysql libpqxx sqlite zlib openssl libcurl-compat
   ```

3. 检查依赖

4. 拉取代码

      ```shell
      git clone http://github.com/qicosmos/feather
      ```

4. 安装依赖项（以下均相对于`feather`目录）

   进入`feather`源码目录，我尝试使用

   ```shell
   git submodule update --init
   ```

   如果拉取成功就可以忽略下面的这些内容，如果失败了就往下看。

   命令拉取依赖模块，但是报出了`fatal: unable to access 'https://github.com/qicosmos/cinatra.git/': OpenSSL SSL_connect: SSL_ERROR_SYSCALL in connection to ...`的错误，于是我修改了`.gitmodules`文件。将其中的`https`全部修改为`http`，以下为修改后的内容：

   ```text
   [submodule "cinatra"]
   	path = cinatra
   	url = http://github.com/qicosmos/cinatra.git
   [submodule "ormpp"]
   	path = ormpp
   	url = http://github.com/qicosmos/ormpp.git
   [submodule "iguana"]
   	path = iguana
   	url = http://github.com/qicosmos/iguana.git
   [submodule "render"]
   	path = render
   	url = http://github.com/qicosmos/render.git
   ```

   然后执行：

   ```
   git submodule update --init
   ```

   可成功拉取子模块。

5. 编译

   执行命令：

   ```shell
   mkdir build
   cd build
   cmake ..
   make
   ```

   如果发现在`make`过程中报错：

   ```text
   In file included from /usr/include/mysql/mysql.h:81,
                    from /home/skyfire/code/feather_demo/feather/ormpp/type_mapping.hpp:10,
                    from /home/skyfire/code/feather_demo/feather/ormpp/utility.hpp:7,
                    from /home/skyfire/code/feather_demo/feather/ormpp/dbng.hpp:13,
                    from /home/skyfire/code/feather_demo/feather/feather.h:9,
                    from /home/skyfire/code/feather_demo/feather/main.cpp:5:
   /usr/include/mysql/mysql_com.h:1011:10: 致命错误：mysql/udf_registration_types.h：没有那个文件或目录
    #include <mysql/udf_registration_types.h>
             ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   编译中断。
   make[2]: *** [CMakeFiles/feather.dir/build.make:63：CMakeFiles/feather.dir/main.cpp.o] 错误 1
   make[1]: *** [CMakeFiles/Makefile2:73：CMakeFiles/feather.dir/all] 错误 2
   make: *** [Makefile:130：all] 错误 2
   
   ```

   使用`find`搜索文件`udf_registration_types.h`

   ```shell
   sudo find / -name udf_registration_types.h
   ```

   找到文件：

   ```text
   /usr/include/mysql/mysql/udf_registration_types.h
   ```

   于是将`/usr/include/mysql`添加至搜索路径重新编译：

   ```shell
   make CXX_FLAGS+="-I/usr/include/mysql"
   ```

   继续报错，这次错误太多，就不贴完整信息了，翻到最上面的报错信息，发现是关于`string_view`的：

   ```text
   feather/cinatra/define.h:20:20: 错误：‘string_view’不是命名空间‘std’中的一个类型名
     inline const std::string_view STAIC_RES = "cinatra_staic_resource";
                       ^~~~~~~~~~~
   ```

   推测与`c++`标准未指定有关，继续添加编译命令：

   ```shell
   make CXX_FLAGS+="-I/usr/include/mysql -std=c++17"
   ```

   编译通过。

7. 运行

   到这里了说明编译完成了，恭喜你！请继续加油，我们还需要把feather运行起来，要把例子跑起来还需要以下步骤：

   你还可以直接导入feather.sql，里面已经准备了一些初始数据，导入之后就可以运行了。

   如果你希望一个个去创建表就往下面看。

   * 建立数据库`feather`
   * 导入数据库表（以下为feather数据库的表结构）

   ```sql
   -- MySQL dump 10.13  Distrib 5.7.24, for Linux (x86_64)
   --
   -- Host: localhost    Database: feather
   -- ------------------------------------------------------
   -- Server version	5.7.24-0ubuntu0.18.04.1
   
   /*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
   /*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
   /*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
   /*!40101 SET NAMES utf8 */;
   /*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
   /*!40103 SET TIME_ZONE='+00:00' */;
   /*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
   /*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
   /*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
   /*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;
   
   --
   -- Table structure for table `article`
   --
   
   DROP TABLE IF EXISTS `article`;
   /*!40101 SET @saved_cs_client     = @@character_set_client */;
   /*!40101 SET character_set_client = utf8 */;
   CREATE TABLE `article` (
     `id` int(11) NOT NULL AUTO_INCREMENT,
     `title` text COLLATE utf8mb4_unicode_ci,
     `introduce` text COLLATE utf8mb4_unicode_ci,
     `user_id` int(11) DEFAULT NULL,
     `visible` int(11) DEFAULT NULL,
     `create_time` text COLLATE utf8mb4_unicode_ci,
     PRIMARY KEY (`id`)
   ) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
   /*!40101 SET character_set_client = @saved_cs_client */;
   
   --
   -- Table structure for table `article_detail`
   --
   
   DROP TABLE IF EXISTS `article_detail`;
   /*!40101 SET @saved_cs_client     = @@character_set_client */;
   /*!40101 SET character_set_client = utf8 */;
   CREATE TABLE `article_detail` (
     `id` int(11) NOT NULL AUTO_INCREMENT,
     `parant_id` int(11) DEFAULT NULL,
     `title` text COLLATE utf8mb4_unicode_ci,
     `content` text COLLATE utf8mb4_unicode_ci,
     `update_time` text COLLATE utf8mb4_unicode_ci,
     PRIMARY KEY (`id`)
   ) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
   /*!40101 SET character_set_client = @saved_cs_client */;
   
   --
   -- Table structure for table `cncppcon2018_user`
   --
   
   DROP TABLE IF EXISTS `cncppcon2018_user`;
   /*!40101 SET @saved_cs_client     = @@character_set_client */;
   /*!40101 SET character_set_client = utf8 */;
   CREATE TABLE `cncppcon2018_user` (
     `id` int(11) NOT NULL AUTO_INCREMENT,
     `user_name` text,
     `phone` bigint(20) DEFAULT NULL,
     `email` text,
     `user_group` text,
     `join_time` text,
     PRIMARY KEY (`id`),
     UNIQUE KEY `phone` (`phone`)
   ) ENGINE=InnoDB AUTO_INCREMENT=341 DEFAULT CHARSET=utf8;
   /*!40101 SET character_set_client = @saved_cs_client */;
   
   --
   -- Table structure for table `pp_comment`
   --
   
   DROP TABLE IF EXISTS `pp_comment`;
   /*!40101 SET @saved_cs_client     = @@character_set_client */;
   /*!40101 SET character_set_client = utf8 */;
   CREATE TABLE `pp_comment` (
     `ID` bigint(20) NOT NULL AUTO_INCREMENT,
     `user_id` bigint(20) NOT NULL,
     `post_id` bigint(20) NOT NULL,
     `comment_parant` bigint(20) NOT NULL,
     `comment_content` text NOT NULL,
     `comment_date` varchar(255) NOT NULL,
     `comment_status` varchar(45) DEFAULT NULL,
     PRIMARY KEY (`ID`)
   ) ENGINE=MyISAM AUTO_INCREMENT=704 DEFAULT CHARSET=utf8;
   /*!40101 SET character_set_client = @saved_cs_client */;
   
   --
   -- Table structure for table `pp_post_views`
   --
   
   DROP TABLE IF EXISTS `pp_post_views`;
   /*!40101 SET @saved_cs_client     = @@character_set_client */;
   /*!40101 SET character_set_client = utf8 */;
   CREATE TABLE `pp_post_views` (
     `ID` bigint(20) NOT NULL AUTO_INCREMENT,
     `type` tinyint(4) NOT NULL,
     `count` int(11) NOT NULL,
     `period` varchar(255) NOT NULL,
     PRIMARY KEY (`type`,`period`,`ID`)
   ) ENGINE=MyISAM AUTO_INCREMENT=13284 DEFAULT CHARSET=utf8;
   /*!40101 SET character_set_client = @saved_cs_client */;
   
   --
   -- Table structure for table `pp_posts`
   --
   
   DROP TABLE IF EXISTS `pp_posts`;
   /*!40101 SET @saved_cs_client     = @@character_set_client */;
   /*!40101 SET character_set_client = utf8 */;
   CREATE TABLE `pp_posts` (
     `ID` bigint(20) NOT NULL AUTO_INCREMENT,
     `post_author` bigint(20) NOT NULL,
     `post_date` varchar(255) NOT NULL,
     `post_title` text NOT NULL,
     `post_content` text NOT NULL,
     `post_status` varchar(255) NOT NULL,
     `post_modified` varchar(255) NOT NULL,
     `content_abstract` text NOT NULL,
     `url` varchar(255) NOT NULL,
     `comment_count` bigint(20) NOT NULL,
     `category` varchar(255) NOT NULL,
     `raw_content` text NOT NULL,
     PRIMARY KEY (`ID`)
   ) ENGINE=MyISAM AUTO_INCREMENT=2058 DEFAULT CHARSET=utf8;
   /*!40101 SET character_set_client = @saved_cs_client */;
   
   --
   -- Table structure for table `pp_sign_out_answer`
   --
   
   DROP TABLE IF EXISTS `pp_sign_out_answer`;
   /*!40101 SET @saved_cs_client     = @@character_set_client */;
   /*!40101 SET character_set_client = utf8 */;
   CREATE TABLE `pp_sign_out_answer` (
     `ID` int(11) NOT NULL AUTO_INCREMENT,
     `question` varchar(255) NOT NULL,
     `answer` text NOT NULL,
     PRIMARY KEY (`ID`)
   ) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8mb4;
   /*!40101 SET character_set_client = @saved_cs_client */;
   
   --
   -- Table structure for table `pp_terms`
   --
   
   DROP TABLE IF EXISTS `pp_terms`;
   /*!40101 SET @saved_cs_client     = @@character_set_client */;
   /*!40101 SET character_set_client = utf8 */;
   CREATE TABLE `pp_terms` (
     `term_id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
     `name` varchar(200) NOT NULL DEFAULT '',
     `slug` varchar(200) NOT NULL DEFAULT '',
     `term_group` bigint(10) NOT NULL DEFAULT '0',
     PRIMARY KEY (`term_id`),
     KEY `slug` (`slug`) USING BTREE,
     KEY `name` (`name`) USING BTREE
   ) ENGINE=MyISAM AUTO_INCREMENT=61 DEFAULT CHARSET=utf8;
   /*!40101 SET character_set_client = @saved_cs_client */;
   
   --
   -- Table structure for table `pp_user`
   --
   
   DROP TABLE IF EXISTS `pp_user`;
   /*!40101 SET @saved_cs_client     = @@character_set_client */;
   /*!40101 SET character_set_client = utf8 */;
   CREATE TABLE `pp_user` (
     `ID` bigint(20) NOT NULL AUTO_INCREMENT,
     `user_login` varchar(255) NOT NULL,
     `user_nickname` varchar(255) NOT NULL,
     `user_email` varchar(255) NOT NULL,
     `user_registered` varchar(255) DEFAULT NULL,
     `user_icon` varchar(255) DEFAULT NULL,
     `user_pass` varchar(255) NOT NULL,
     `user_role` tinyint(4) NOT NULL,
     PRIMARY KEY (`ID`)
   ) ENGINE=MyISAM AUTO_INCREMENT=2535 DEFAULT CHARSET=utf8;
   /*!40101 SET character_set_client = @saved_cs_client */;
   
   --
   -- Table structure for table `user`
   --
   
   DROP TABLE IF EXISTS `user`;
   /*!40101 SET @saved_cs_client     = @@character_set_client */;
   /*!40101 SET character_set_client = utf8 */;
   CREATE TABLE `user` (
     `id` int(11) NOT NULL AUTO_INCREMENT,
     `user_name` text COLLATE utf8mb4_unicode_ci,
     `nick_name` text COLLATE utf8mb4_unicode_ci,
     `token` text COLLATE utf8mb4_unicode_ci,
     `gender` int(11) DEFAULT NULL,
     `role` int(11) DEFAULT NULL,
     `avatar` text COLLATE utf8mb4_unicode_ci,
     `phone` bigint(20) DEFAULT NULL,
     `email` text COLLATE utf8mb4_unicode_ci,
     `qq` text COLLATE utf8mb4_unicode_ci,
     `location` text COLLATE utf8mb4_unicode_ci,
     PRIMARY KEY (`id`)
   ) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
   /*!40101 SET character_set_client = @saved_cs_client */;
   
   --
   -- Table structure for table `visit_counter`
   --
   
   DROP TABLE IF EXISTS `visit_counter`;
   /*!40101 SET @saved_cs_client     = @@character_set_client */;
   /*!40101 SET character_set_client = utf8 */;
   CREATE TABLE `visit_counter` (
     `id` int(11) NOT NULL AUTO_INCREMENT,
     `save_hour` text,
     `counter` int(11) DEFAULT NULL,
     PRIMARY KEY (`id`)
   ) ENGINE=InnoDB AUTO_INCREMENT=114 DEFAULT CHARSET=utf8;
   /*!40101 SET character_set_client = @saved_cs_client */;
   
   --
   -- Table structure for table `wp_term_relationships`
   --
   
   DROP TABLE IF EXISTS `wp_term_relationships`;
   /*!40101 SET @saved_cs_client     = @@character_set_client */;
   /*!40101 SET character_set_client = utf8 */;
   CREATE TABLE `wp_term_relationships` (
     `object_id` bigint(20) unsigned NOT NULL DEFAULT '0',
     `term_taxonomy_id` bigint(20) unsigned NOT NULL DEFAULT '0',
     `term_order` int(11) NOT NULL DEFAULT '0',
     PRIMARY KEY (`object_id`,`term_taxonomy_id`),
     KEY `term_taxonomy_id` (`term_taxonomy_id`) USING BTREE
   ) ENGINE=MyISAM DEFAULT CHARSET=utf8;
   /*!40101 SET character_set_client = @saved_cs_client */;
   /*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;
   
   /*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
   /*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
   /*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
   /*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
   /*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
   /*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
   /*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;
   
   -- Dump completed on 2018-12-11 15:01:53
   ```

   成功导入后，应该可以看到如下的表结构：

   ```text
   +-----------------------+
   | Tables_in_feather     |
   +-----------------------+
   | article               |
   | article_detail        |
   | cncppcon2018_user     |
   | pp_comment            |
   | pp_post_views         |
   | pp_posts              |
   | pp_sign_out_answer    |
   | pp_terms              |
   | pp_user               |
   | user                  |
   | visit_counter         |
   | wp_term_relationships |
   +-----------------------+
   ```



   * 插入验证数据。

   导入数据库表后，还需要插入两条数据（可参见代码`purecpp_controller.hpp`的`423行`及`522`行的验证逻辑）：

   ```sql
   insert into pp_sign_out_answer values(1,"args +  ...是C++17什么特性？","fold expression,right fold,折叠表达式"),(2,"[](){}是C++11的什么特性？","lambda,Lambda,lambda表达式,Lambda表达式");
   ```

   **注意这里的`id`分别为`1`和`2`。**

   * 创建运行时需要的目录结构

     在`feather`目录下执行(此处假设构建目录为`feather/build`)：

     ```shell
     mkdir runtime
     cd runtime
     cp -rvt . ../build/feather ../purecpp ../cfg
     ```

     此时`runtime`目录下应该有文件（目录）：

     ```text
     cfg/  feather*  purecpp/
     ```

   * 修改配置文件（以下均相对于`runtime`目录）

     配置文件为`cfg/feather.cfg`,修改其中的数据库用户名和密码为真实数据。

   * 运行

     在`runtime`目录下执行：

     ```shell
     ./feather
     ```

   * 访问

     **注意**：在数据库无数据的时候无法看到页面，可以通过访问：`http://localhost:8080/sign_out_page`注册用户并发表一篇文章。这时其他页面就可以访问了。