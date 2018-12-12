# feather Mac编译文档 by zacklocx(zacklocx@gmail.com)

#编译环境

    macOS Mojave 10.14.1
    Xcode 10.1

#安装依赖库

    安装mysql，这里比较坑
    首先，不用能brew安装最新版mysql(8.0.3)，否则编译不过
    其次，不能使用 "brew install mysql@5.7" 安装5.7旧版，虽然可以编译过，但是在我这里mysql服务启动异常
    最终，从这里(https://cdn.mysql.com//archives/mysql-5.6/mysql-5.6.31-osx10.8-x86_64.dmg)下载5.6的安装包，可用
    
    安装好mysql之后，记得在你的.bashrc或者.zshrc中设置路径
    export PATH="/usr/local/mysql/bin:$PATH"

    使用brew安装其它依赖
    brew install gcc
    brew install boost
    brew install ossp-uuid
    brew install mysql-connector-c

    安装好以上依赖之后，打开feather项目的CMakeLists.txt，有几处需要修改
    
    1. 打开这里的注释
    #for mac
    SET(CMAKE_C_COMPILER "/usr/local/bin/gcc-8")
    SET(CMAKE_CXX_COMPILER "/usr/local/bin/g++-8")
    set(CMAKE_BUILD_TYPE Release)

    2. 添加mysql的目录
    
    include_directories(/usr/local/mysql/include)
    link_directories(/usr/local/mysql/lib)

    3. 某个库好像用到了Mac的CoreFoundation，因此需要添加该framework
    
    function(add_framework target framework)
	   if(APPLE)
		  find_library(found ${framework})

		  if(${found} STREQUAL "${framework}-NOTFOUND")
			 message(FATAL_ERROR "ERROR: ${framework} not found (${CMAKE_OSX_SYSROOT})")
		  endif()

		  #message("Found framework ${found}")

		  target_link_libraries(${target} ${found})
	   endif()
    endfunction()
    
    add_framework(feather CoreFoundation)

#编译

    首先，有一处修改
    feather/ormpp/type_mapping.hpp中，
    将 #include <mysql/mysql.h> 改为 #include <mysql.h>

    然后
    cd feather
    md build
    cd build
    cmake ..
    make

#组装
   只是编译完成，还不能直接运行，要把例子跑起来还需要以下步骤：

   * 建立数据库`feather`
   * 导入数据库表（以下为`feather_tables.sql`内容）

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

   注意这里的`id`分别为`1`和`2`。

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