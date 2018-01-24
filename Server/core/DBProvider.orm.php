<?php
/**
 * Created by YANBO.
 * User: yanbo
 * Date: 2016/4/1
 * Time: 14:15
 * Description: 原始基础框架设计
 */
require_once 'CoreDefine.php';
require_once ORM_DIR.'../util/LogUtil.php';

define('PROVIDER_ORM_NAME', "DBProvider");

class DBProvider
{
    private $dbhost = null;
    private $dbuser = null;
    private $dbpwd = null;
    private $dbname = null;
    private $dbpdo = null;

    function DBProvider()
    {
        $class_name = PROVIDER_ORM_NAME.func_num_args();
        $this->$class_name(func_get_args(func_num_args()));
    }

    // 构造函数重载，无参数
    function DBProvider0() {}

    // 构造函数重载，带一个参数
    function DBProvider1($xml_path)
    {
        $dom = new DOMDocument();
        $dom->load($xml_path[0]);
        $this->dbhost = $dom->getElementsByTagName('dbhost')->item(0)->nodeValue;
        $this->dbuser = $dom->getElementsByTagName('dbuser')->item(0)->nodeValue;
        $this->dbpwd = $dom->getElementsByTagName('dbpwd')->item(0)->nodeValue;
        $this->dbname = $dom->getElementsByTagName('dbname')->item(0)->nodeValue;

        $this->dbConnection();
    }

    public function getDbName() {
        return $this->dbname;
    }

    public function dbConnection()
    {
        if ($this->dbhost != null)
        {
            try
            {
                $opt = array(PDO::ATTR_PERSISTENT=>true);
                $this->dbpdo = new PDO('mysql:dbname='.$this->dbname.';host='.$this->dbhost, $this->dbuser, $this->dbpwd, $opt);
            }
            catch (PDOException $e)
            {
                $this->connect_error($e);
            }
        }
    }

    public function sqldataset($sql)
    {
        $dataset = array(); // 一个二维数组
        try
        {
            $pdostmt = $this->dbpdo->query($sql);
            $i = 0;
            while ($row = $pdostmt->fetch(PDO::FETCH_ASSOC))
            {
                foreach ($row as $key => $value)
                {
                    $dataset[$i][$key] = $value;
                }
                $i++;
            }
        }
        catch (PDOException $e)
        {
            $this->connect_error($e);
        }

        return $dataset;
    }

    public function sqlexec($sql)
    {
        $affect = null;
        try
        {
            $affect = $this->dbpdo->exec($sql);
        }
        catch (PDOException $e)
        {
            $this->connect_error($e);
        }

        if ($affect)
        {
            echo 'the number of affected lines in table is '.$affect;
        }
        else
        {
            print_r($this->dbpdo->errorinfo());
        }

    }

    private function connect_error(PDOException $e)
    {
        echo 'database connected failure!'.$e->getMessage();
    }

    // 关闭PDO连接
    public function close()
    {
        $this->dbpdo = null;
    }
}



?>