<?php
/**
 * Created by YANBO.
 * User: yanbo
 * Date: 2016/4/1
 * Time: 14:15
 * Description: 原始基础框架设计
 */

require_once 'CoreDefine.php';
require_once ORM_DIR.'DBProvider.orm.php';
require_once ORM_DIR.'SqlConstructor.orm.php';
require_once ORM_DIR.'../util/LogUtil.php';

define('MINI_ORM_NAME', "MiniORM");

// ORM映射工具
class MiniORM
{
    private $mProvider;
    private $mConstructor;
    private $mReflection;
    private $mObject;

    private static $sInstance;

    // 模拟无参数构造函数
    private function MiniORM()
    {
        $this->mProvider = new DBprovider(CONFIG);
        $this->mConstructor = new SqlConstructor();
        $this->mObject = null;
        $this->mReflection = null;
    }

    public static function getInstance()
    {
        if (self::$sInstance == null) {
            self::$sInstance = new MiniORM();
        }

        return self::$sInstance;
    }

    public function setReflectObject($obj)
    {
        $this->mObject = $obj;
        $this->mReflection = new ReflectionClass($obj);
    }

    public function getObjects()
    {
        $property_values = $this->getClassProperties();
        LogUtil::d($property_values);
        $class_name = $this->mReflection->getName();
        $sql_query = $this->mConstructor->sqlquery_cons(null, $class_name, $property_values);
        LogUtil::d($sql_query);
        $dataset = $this->mProvider->sqldataset($sql_query);
        LogUtil::d($dataset);
        $object_array = array();
        foreach ($dataset as $row)
        {
            $obj0 = new $class_name();
            foreach ($row as $key => $value)
            {
                $set_method_name = ENTITY_SET_PREFIX.$key;
                $obj0->$set_method_name($value);
            }
            $object_array[] = $obj0;
        }
        return $object_array;
    }

    public function addObject($obj)
    {
        $this->setReflectObject($obj);
        $conditions = $this->getClassProperties();
        $class_name = $this->mReflection->getName();
        $sql_insert = $this->mConstructor->sqlinsert_cons($class_name, $conditions);
        $this->mProvider->sqlexec($sql_insert);
    }

    public function updateObject($objsrc, $objdest)
    {
        $this->setReflectObject($objsrc);
        $src_property_values = $this->getClassProperties();

        $this->setReflectObject($objdest);
        $dest_property_values = $this->getClassProperties();

        $class_name = $this->mReflection->getName();
        $sql_update = $this->mConstructor->sqlupdate_cons($dest_property_values, $class_name, $src_property_values);
        $this->mProvider->sqlexec($sql_update);
    }

    public function deleteObject($obj)
    {
        $this->setReflectObject($obj);
        $property_values = $this->getClassProperties();
        $class_name = $this->mReflection->getName();
        $sql_delete = $this->mConstructor->sqldelete_cons($class_name, $property_values);
        $this->mProvider->sqlexec($sql_delete);
    }

    public function getClassProperties()
    {
        $methods = $this->mReflection->getMethods();
        $conditions = array();
        $pattern = '/^'.ENTITY_GET_PREFIX.'/';
        $pos = strlen(ENTITY_GET_PREFIX);
        foreach($methods as $method)
        {
            $method_name = $method->getName();
            if (preg_match($pattern, $method_name))
            {
                $property_name = lcfirst(substr($method_name, $pos));
                $property_value = $this->mObject->$method_name();
                // $property_value不为空则赋值
                if ($property_value)
                {
                    $conditions[$property_name] = $property_value;
                }
            }
        }

        return $conditions;
    }
}
?>