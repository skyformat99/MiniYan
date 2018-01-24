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

class SqlConstructor
{
    public function sqlquery_cons($results, $table, $conditions)
    {
        $sql_query = 'select ';

        if (null == $results)
        {
            $sql_query .= '*';
        }
        else
        {
            // 取出列
            $count = count($results);
            foreach ($results as $value)
            {
                $sql_query .= "`$value`";
                if (0 != --$count)
                {
                    $sql_query .= ',';
                }
            }
        }

        $sql_query .= " from `$table`";
        $count = count($conditions);
        if (0 != $count)
        {
            $sql_query .= ' where ';
            foreach ($conditions as $key => $value)
            {
                // 字符串必须加引号括起来
                $sql_query .= $this->getstatment($key, $value);

                if (0 != --$count)
                {
                    $sql_query .= ' and ';
                }
            }
        }

        LogUtil::echo($sql_query);

        return $sql_query;
    }

    public function sqlinsert_cons($table, $conditions)
    {
        $sqlinsert1 = "insert into `$table`(";
        $sqlinsert2 = "values(";

        $count = count($conditions);
        foreach ($conditions as $key => $value)
        {
            $sqlinsert1 .= "`$key`";
            if (is_string($value))
            {
                $sqlinsert2 .= "'$value'";
            }
            else
            {
                $sqlinsert2 .= "$value";
            }

            if (0 != --$count)
            {
                $sqlinsert1 .= ',';
                $sqlinsert2 .= ',';
            }
        }

        $sqlinsert1 .= ")";
        $sqlinsert2 .= ")";
        $sqlinsert1 .= $sqlinsert2;

        return $sqlinsert1;
    }

    public function sqlupdate_cons($results, $table, $conditions)
    {
        $sqlupdate = "update `$table` set ";
        $count = count($results);
        foreach ($results as $key => $value)
        {
            $sqlupdate .= $this->getstatment($key, $value);

            if (0 != --$count)
            {
                $sqlupdate .= ",";
            }
        }

        $count = count($conditions);
        if (0 != $count)
        {
            $sqlupdate .= " where ";
            foreach ($conditions as $key => $value)
            {
                $sqlupdate .= $this->getstatment($key, $value);
                if (0 != --$count)
                {
                    $sqlupdate .= " and ";
                }
            }
        }

        return $sqlupdate;
    }

    public function sqldelete_cons($table, $conditions)
    {
        $sqldelete = "delete from `$table` where ";
        $count = count($conditions);
        if (0 != $count)
        {
            $sqldelete .= " where ";
            foreach ($conditions as $key => $value)
            {
                $sqldelete .= $this->getstatment($key, $value);
                if (0 != --$count)
                {
                    $sqldelete .= " and ";
                }
            }
        }
    }

    private function getstatment($key, $value)
    {
        $statment = "";

        if (is_string($value))
        {
            $statment = "`$key`='$value'";
        }
        else
        {
            $statment = "`$key`=$value";
        }

        return $statment;
    }
}
?>