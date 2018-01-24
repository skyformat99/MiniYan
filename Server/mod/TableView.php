<?php
/**
 * Created by PhpStorm.
 * User: yanbo
 * Date: 2017/1/23
 * Time: 9:52
 * Description: 组件开发之Table
 */

class TableView
{
    // TR row style
    private $tabStyle;
    // TD cell style
    private $contentStyle;

    function TableView($tabStyle, $contentStyle)
    {
        $this->tabStyle = $tabStyle;
        $this->contentStyle = $contentStyle;
    }

    public static function createTableView($tabStyle, $contentStyle, $tableInfo)
    {
        $tableView = new TableView($tabStyle, $contentStyle);
        $tableView->getTable($tableInfo);
    }

    private function getTable($tableInfo)
    {
        if ($tableInfo == null || !is_array($tableInfo) || count($tableInfo) == 0) {
            return "";
        }

        $output = "<div class='$this->tabStyle'>";

        $output .= "<div class='$this->contentStyle'>";
        foreach ($tableInfo[0] as $val)
        {
            $output .= "<div class='cell'>";
            $output .= "$val";
            $output .= "</div>";
        }
        //$output .= "<div class='r_close'/>";
        $output .= "</div>";

        $length = count($tableInfo);
        if ($length < 1)
        {
            return $output;
        }

        for ($i = 1; $i < $length; $i++)
        {
            $output .= "<div class='$this->contentStyle'>";
            foreach ($tableInfo[$i] as $val)
            {
                $output .= "<div class='cell'>";
				if (empty($val))
				{
					$output .= "empty";
				}
				else
				{
					$output .= "$val";
				}
                
                $output .= "</div>";
            }
            //$output .= "<div class='r_close'/>";
            $output .= "</div>";
        }

        $array_length = count($tableInfo[0]);
        $output .= "<div class='$this->contentStyle'>";
        for ($i = 0; $i < $array_length; $i++)
        {
            $output .= "<div class='b_close' />";
        }
        $output .= "</div>";

        $output .= "</div>";

        echo $output;
    }
}

?>