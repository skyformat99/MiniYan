<?php
/**
 * Created by PhpStorm.
 * User: yanbo
 * Date: 2016/3/30
 * Time: 11:23
 */
//define('ORM_DIR', str_replace('\\','/', dirname(__FILE__)).'/');
//require_once "../core/DBProvider.orm.php";
//require_once "../db/DBOperator.php";
require_once "../core/MiniORM.orm.php";
require_once ENTITY_DIR.'userinfo.class.php';
require_once ORM_DIR.'../util/LogUtil.php';

//phpinfo();

//$dbp = new DBProvider("../xml/DBConfig.xml");
//$dbp->dbconnection();

//$dsn = "mysql:host=localhost;";
//$db = new PDO($dsn, 'root', '');
//$db->query('create database BoGame');



echo ORM_DIR;
//$dbp = new DBProvider();
//$dbp->dbconnection();

//echo json_decode(array());

$user = new Userinfo();
$user->setUserName('ahh');
$user->setUserPw('ahhahhahh');

MiniORM::getInstance()->addObject($user);
//$user->set_UserName(null);
//$user->set_PassWord(null);
LogUtil::d(MiniORM::getInstance()->getObjects());
?>