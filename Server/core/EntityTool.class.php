<?php
/**
 * Created by YANBO.
 * User: yanbo
 * Date: 2016/4/1
 * Time: 14:15
 * Description: 原始基础框架设计
 * 创建实体类自动化工具
 */

require_once 'CoreDefine.php';
require_once ORM_DIR.'DBProvider.orm.php';
require_once ORM_DIR.'../util/LogUtil.php';
require_once ORM_DIR.'../util/FileUtil.php';

// 映射数据库表创建实体类自动化工具
class EntityTool
{
    private $mProvider;
    private $mDBName;

    function EntityTool()
    {
        $this->mProvider = new DBProvider(CONFIG);
        $this->mDBName = DB_NAME_PREFIX.$this->mProvider->getDbName();
        // 第一个词首字母变大写
        echo ucfirst($this->mDBName);
    }

    public function createEntities()
    {
        $tableSet = $this->mProvider->sqldataset('show tables');

        LogUtil::d($tableSet);
        $sql_array = array();

        foreach ($tableSet as $table)
        {
            $my_table = $table[$this->mDBName];
            $sql0 = 'show columns from '.$table[$this->mDBName];
            $columnSet = $this->mProvider->sqldataset($sql0);
            LogUtil::d($columnSet);

            foreach ($columnSet as $value)
            {
                $sql_array[$my_table][$value['Field']] = $value['Field'];
            }
        }

        LogUtil::d($sql_array);

        FileUtil::dirDelete(ENTITY_DIR);

        mkdir(ENTITY_DIR);

        $this->createClassFile($sql_array);
    }

    private function createClassFile($tableInfo)
    {
        foreach ($tableInfo as $table0 => $values)
        {
            $file_name = ENTITY_DIR.ucfirst($table0).ENTITY_CLASS_SUFFIX;
            $f = fopen($file_name, "w");

            $file_str = "";
            $file_str .= "<?php\r\n";
            $file_str .= "class ".ucfirst($table0)."\r\n";
            $file_str .= "{\r\n";

            // 内部成员变量
            foreach ($values as $value)
            {
                $file_str .= "    private $".$value.";\r\n";
                $file_str .= "    \r\n";
            }

            // 内部方法
            foreach ($values as $value)
            {
                //set
                $file_str .= "    public function ".ENTITY_SET_PREFIX.ucfirst($value)."($$value)\r\n";
                $file_str .= "    {\r\n";
                $file_str .= '        $this->'.$value." = $$value;\r\n";
                $file_str .= "    }\r\n";
                $file_str .= "    \r\n";
                //get
                $file_str .= "    public function ".ENTITY_GET_PREFIX.ucfirst($value)."()\r\n";
                $file_str .= "    {\r\n";
                $file_str .= '        return $this->'.$value.";\r\n";
                $file_str .= "    }\r\n";
            }

            $file_str .= "}\r\n";
            $file_str .= "?>";
            fwrite($f, $file_str);
            fclose($f);
            $file_str = '';
        }
    }
}

$en = new EntityTool();
$en->createEntities();
?>