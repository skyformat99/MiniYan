<?php
/**
 * Created by PhpStorm.
 * User: yanbo
 * Date: 2017/1/20
 * Time: 12:11
 */
require_once "../core/MiniORM.orm.php";
require_once ENTITY_DIR . 'Userinfo.class.php';
require_once ORM_DIR.'../util/Common.php';
require_once ORM_DIR.'../util/Request.php';

class LoginModule
{
    public static function index()
    {
        return "{ retCode : '1', retMsg : 'Login Success', startUrl : '" . LOGIN_INFO. "'}";
    }

    public static function error()
    {
        return "{ retCode : '0', retMsg : 'Login Failure'}";
    }

    public static function login($name, $pwd)
    {
        if (!empty(Request::session("token")))
        {
            return self::index();
        }
        else
        {
            if (empty($name) || empty($pwd))
            {
                return self::error();
            }
        }

        if (empty($name) || empty($pwd)) {
            echo self::error();
            return false;
        }

        $user = new Userinfo();
        $user->setUserName($name);
        $user->setUserPw($pwd);

        MiniORM::getInstance()->setReflectObject($user);
        $result = MiniORM::getInstance()->getObjects();

        if (count($result) > 0)
        {
            Request::setSession(md5(microtime(true)));
            return self::index();
        }

        return self::error();
    }

    public static function getUserInfo($name, $pwd)
    {
        if (empty($name) || empty($pwd))
        {
            return null;
        }
        
        $user = new Userinfo();
        $user->setUserName($name);
        $user->setUserPw($pwd);

        MiniORM::getInstance()->setReflectObject($user);
        $result = MiniORM::getInstance()->getObjects();

        //LogUtil::output(LogUtil::object2array($result));
        return Common::objectArray2Array($result);
    }

}
?>
