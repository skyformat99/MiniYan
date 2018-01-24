<?php

/**
 * Created by PhpStorm.
 * User: yanbo
 * Date: 2016/4/5
 * Time: 14:50
 */
class LogUtil
{
    public static function d($info)
    {
        if (DEBUG) {
            echo '<pre>';
            print_r($info);
            echo '</pre>';
        }
    }

    public static function output($info)
    {
        echo '<pre>';
        print_r($info);
        echo '</pre>';
    }

    public static function echo ($info)
    {
        if (DEBUG) {
            echo $info;
        }
    }

}

?>