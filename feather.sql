-- MySQL dump 10.13  Distrib 5.7.24, for Win64 (x86_64)
--
-- Host: localhost    Database: feather
-- ------------------------------------------------------
-- Server version	5.7.24-log

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
-- Dumping data for table `article`
--

LOCK TABLES `article` WRITE;
/*!40000 ALTER TABLE `article` DISABLE KEYS */;
/*!40000 ALTER TABLE `article` ENABLE KEYS */;
UNLOCK TABLES;

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
-- Dumping data for table `article_detail`
--

LOCK TABLES `article_detail` WRITE;
/*!40000 ALTER TABLE `article_detail` DISABLE KEYS */;
/*!40000 ALTER TABLE `article_detail` ENABLE KEYS */;
UNLOCK TABLES;

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
-- Dumping data for table `cncppcon2018_user`
--

LOCK TABLES `cncppcon2018_user` WRITE;
/*!40000 ALTER TABLE `cncppcon2018_user` DISABLE KEYS */;
/*!40000 ALTER TABLE `cncppcon2018_user` ENABLE KEYS */;
UNLOCK TABLES;

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
-- Dumping data for table `pp_comment`
--

LOCK TABLES `pp_comment` WRITE;
/*!40000 ALTER TABLE `pp_comment` DISABLE KEYS */;
/*!40000 ALTER TABLE `pp_comment` ENABLE KEYS */;
UNLOCK TABLES;

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
-- Dumping data for table `pp_post_views`
--

LOCK TABLES `pp_post_views` WRITE;
/*!40000 ALTER TABLE `pp_post_views` DISABLE KEYS */;
/*!40000 ALTER TABLE `pp_post_views` ENABLE KEYS */;
UNLOCK TABLES;

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
) ENGINE=MyISAM AUTO_INCREMENT=2059 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `pp_posts`
--

LOCK TABLES `pp_posts` WRITE;
/*!40000 ALTER TABLE `pp_posts` DISABLE KEYS */;
INSERT INTO `pp_posts` VALUES (2058,2535,'2018-12-12 10:16:44','1','# 初始数据放这里','publish','2018-12-12 10:16:44','# 初始数据放这里...','',0,'3','');
/*!40000 ALTER TABLE `pp_posts` ENABLE KEYS */;
UNLOCK TABLES;

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
-- Dumping data for table `pp_sign_out_answer`
--

LOCK TABLES `pp_sign_out_answer` WRITE;
/*!40000 ALTER TABLE `pp_sign_out_answer` DISABLE KEYS */;
INSERT INTO `pp_sign_out_answer` VALUES (1,'args +  ...是C++17什么特性？','fold expression,right fold,折叠表达式'),(2,'[](){}是C++11的什么特性？','lambda,Lambda,lambda表达式,Lambda表达式');
/*!40000 ALTER TABLE `pp_sign_out_answer` ENABLE KEYS */;
UNLOCK TABLES;

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
-- Dumping data for table `pp_terms`
--

LOCK TABLES `pp_terms` WRITE;
/*!40000 ALTER TABLE `pp_terms` DISABLE KEYS */;
/*!40000 ALTER TABLE `pp_terms` ENABLE KEYS */;
UNLOCK TABLES;

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
) ENGINE=MyISAM AUTO_INCREMENT=2536 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `pp_user`
--

LOCK TABLES `pp_user` WRITE;
/*!40000 ALTER TABLE `pp_user` DISABLE KEYS */;
INSERT INTO `pp_user` VALUES (2535,'tc','tc','tc163mail@163.com','','','e10adc3949ba59abbe56e057f20f883e',0);
/*!40000 ALTER TABLE `pp_user` ENABLE KEYS */;
UNLOCK TABLES;

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
-- Dumping data for table `user`
--

LOCK TABLES `user` WRITE;
/*!40000 ALTER TABLE `user` DISABLE KEYS */;
/*!40000 ALTER TABLE `user` ENABLE KEYS */;
UNLOCK TABLES;

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
-- Dumping data for table `visit_counter`
--

LOCK TABLES `visit_counter` WRITE;
/*!40000 ALTER TABLE `visit_counter` DISABLE KEYS */;
/*!40000 ALTER TABLE `visit_counter` ENABLE KEYS */;
UNLOCK TABLES;

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

--
-- Dumping data for table `wp_term_relationships`
--

LOCK TABLES `wp_term_relationships` WRITE;
/*!40000 ALTER TABLE `wp_term_relationships` DISABLE KEYS */;
/*!40000 ALTER TABLE `wp_term_relationships` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2018-12-12 10:20:13
