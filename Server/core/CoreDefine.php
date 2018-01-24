<?php
/**
 * Created by PhpStorm.
 * User: yanbo
 * Date: 2016/4/1
 * Time: 17:45
 */

// 常量配置

// ORM当前目录
define('ORM_DIR', str_replace('\\','/', dirname(__FILE__)).'/');
// 实体类文件夹
define('ENTITY_DIR', '../entity/');
// 配置文件
define('CONFIG', '../xml/DBConfig.xml');
// 数据库数组索引名前缀
define('DB_NAME_PREFIX', 'Tables_in_');
// 实体类文件后缀
define('ENTITY_CLASS_SUFFIX', '.class.php');
// 实体类set方法前缀
define('ENTITY_SET_PREFIX', 'set');
// 实体类get方法前缀
define('ENTITY_GET_PREFIX', 'get');
// DEBUG控制开关
define('DEBUG', false);
// 服务器HOST地址
define('SERVER_HOST', 'http://' . $_SERVER['HTTP_HOST']);
// 首次进入地址
define('LOGIN_INFO', SERVER_HOST . '/api/StartInfo.php');
?>