1.解压安装MySQL，字符集选utf8。其他步骤基本默认。root用户密码为“111111”
2.安装MySQL连接驱动
3.创建Visitor_db数据库：开始->程序->MySQL—>MySQL Command Line Client;输入上面的密码：111111。
输入：“create database visitor_db;”创建空数据库，关闭客户端。
4.还原数据库：在运行中，打开dos；进入MySQL安装路径。默认在“C:\Program Files\MySQL\MySQL Server 5.1\bin\”下，并切换到此目录。
输入：“mysql -u root -p visitor_db < *:\***\visitor_sb.sql”(*号表示要还原的数据文件路径)；回车；输入密码：111111。无提示表示
成功。
5.安装IIS Internet服务器，安装成功后，修改网站默认路径为visitorApp.exe所在文件夹即可。
6.配置串口端口，详细配置见程序后台配置管理。
7.访客单打印设置：IE浏览器->文件->页面设置->纸张大小58*297mm,纵向，页边距：左4，右4，上5，下170，页眉页脚全选“空”。