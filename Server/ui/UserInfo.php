<?php
/**
 * Created by PhpStorm.
 * User: yanbo
 * Date: 2017/7/14
 * Time: 17:02
 */
require_once "../mod/LoginModule.php";
require_once "../mod/TableView.php";

function output($type)
{
    if ($type == 0)
    {
        echo SERVER_HOST;
    }
    else if ($type == 1)
    {
        TableView::createTableView("tab", "content", LoginModule::getUserInfo(Request::post("name"), Request::post("pwd")));
    }
}

output(Request::get("type"));
?>