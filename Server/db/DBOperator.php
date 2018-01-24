<?php
/**
 * Created by PhpStorm.
 * User: yanbo
 * Date: 2016/4/1
 * Time: 14:15
 */
require_once "../core/DBProvider.orm.php";

class DBOperator
{
    private static $sInstance;
    private $mProvider;

    public static function getInstance()
    {
        if (self::$sInstance == null) {
            self::$sInstance = new DBOperator();
        }

        return self::$sInstance;
    }

    private function DBOperator()
    {
        $this->mProvider = new DBProvider('../xml/DBConfig.xml');
        // 初始化数据库
        if ($this->mProvider->getDbName() == null
            || $this->mProvider->getDbName() == "") {
            $sqlstr = file_get_contents('../sql/BoGame.sql');
            $this->mProvider->sqlexec($sqlstr);
        }
    }

    public function restoreBackup()
    {
        //echo "restoreBackup!";
        $sqlstr = file_get_contents('../sql/Backup.sql');
        $this->mProvider->sqlexec($sqlstr);
    }

    // 备份数据
    public function backUpData()
    {
        $tableSet = $this->mProvider->sqlexec('show tables');
    }
}

// 数据库创建以及恢复
DBOperator::getInstance()->restoreBackup();
?>