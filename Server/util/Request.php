<?php

/**
 * Created by PhpStorm.
 * User: yanbo
 * Date: 2017/1/22
 * Time: 13:25
 */
class Request
{
    public static function startSession()
    {
        session_start();
    }

    public static function stopSession()
    {
        session_destroy();
    }

    public static function get($parem) 
    {
        if (isset($_GET[$parem])) {
            return $_GET[$parem];
        }

        return null;
    }

    public static function post($parem)
    {
        if (isset($_POST[$parem])) {
            return $_POST[$parem];
        }

        return null;
    }

    public static function session($parem)
    {
        if (isset($_SESSION[$parem])) {
            return $_SESSION[$parem];
        }

        return null;
    }

    public static function setSession($parem)
    {
        $_SESSION[$parem] = $parem;
    }

    public static function httpRequest()
    {
        $url = Request::post("url");
        $method = Request::post("method");
        $curl = curl_init();
        curl_setopt($curl, CURLOPT_URL, $url);
        // 不输出http header的内容
        //curl_setopt($curl, CURLOPT_HEADER, 1);

        if (strcasecmp($method, "post") == 0)
        {
            curl_setopt($curl, CURLOPT_POSTFIELDS, 0);
        }
        curl_setopt($curl, CURLOPT_RETURNTRANSFER, 0);
        $data = curl_exec($curl);
        curl_close($curl);
        return $data;
    }
}