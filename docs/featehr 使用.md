# feather使用说明

1. 安装依赖库

    ```text
    mysql
    ```
	
2. 编译

    ```shell
    git clone --recurse-submodules https://github.com/qicosmos/feather.git

    cmake -B build

    cmake --build build
    ```

3. 运行

建立数据库feather

导入数据库表 source feather.sql

成功导入后，应该可以看到如下的表结构：

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

  **注意**：在数据库无数据的时候无法看到页面，可以通过访问：`http://localhost:8080/sign_up_page`注册用户并发表一篇文章。这时其他页面就可以访问了。