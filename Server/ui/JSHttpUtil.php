<?php

/**
 * Created by PhpStorm.
 * User: yanbo
 * Date: 2017/9/13
 * Time: 11:15
 * 主要用来接收JS传送过来的url地址，并返回数据给JS,
 * 实现跨域功能
 */
require_once "../util/Request.php";

echo Request::httpRequest();
//echo "hello httpRequest";
?>