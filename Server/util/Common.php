<?php
/**
 * Created by PhpStorm.
 * User: yanbo
 * Date: 2017/1/23
 * Time: 10:10
 */
class Common
{
    // 对象数组转2维数组
    public static function objectArray2Array($objectArray)
    {
        $array = array();
        $array[] = self::object2KeyArray($objectArray[0]);

        foreach ($objectArray as $obj)
        {
            if (is_object($obj))
            {
                $array[] = self::object2ValueArray($obj);
            }
        }

        return $array;
    }

    // 将对象转成属性值的数组
    private static function object2ValueArray($obj)
    {
        if (is_object($obj))
        {
            $subArray = array();

            $reflect = new ReflectionObject($obj);
            $properties = $reflect->getProperties();

            foreach ($properties as $key => $value)
            {
                $method_name = ENTITY_GET_PREFIX . ucfirst($value->getName());
                $subArray[] = $obj->$method_name();
            }

            return $subArray;
        }
        else
        {
            return null;
        }
    }

    // 将对象转成属性名的数组
    private static function object2KeyArray($obj)
    {
        if (is_object($obj))
        {
            $subArray = array();

            $reflect = new ReflectionObject($obj);
            $properties = $reflect->getProperties();

            foreach ($properties as $key => $value)
            {
                $subArray[] = $value->getName();
            }

            return $subArray;
        }
        else
        {
            return null;
        }
    }
}
?>