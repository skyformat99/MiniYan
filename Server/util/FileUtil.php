<?php

/**
 * Created by PhpStorm.
 * User: yanbo
 * Date: 2016/4/8
 * Time: 12:34
 */
class FileUtil
{
    public static function dirDelete($dir)
    {
        // 先删除目录下的文件：
        if (!file_exists($dir) || !is_dir($dir))
        {
            return false;
        }
        
        $dh = opendir($dir);
        while ($file = readdir($dh))
        {
            if ($file != "." && $file != "..")
            {
                $fullpath = $dir . "/" . $file;
                if (!is_dir($fullpath))
                {
                    unlink($fullpath);
                }
                else
                {
                    deldir($fullpath);
                }
            }
        }

        closedir($dh);
        //删除当前文件夹：
        if (rmdir($dir)) {
            return true;
        } else {
            return false;
        }
    }
}

?>