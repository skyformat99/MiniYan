<?php
class Admin
{
    private $adminId;
    
    private $adminName;
    
    private $adminPw;
    
    public function setAdminId($adminId)
    {
        $this->adminId = $adminId;
    }
    
    public function getAdminId()
    {
        return $this->adminId;
    }
    public function setAdminName($adminName)
    {
        $this->adminName = $adminName;
    }
    
    public function getAdminName()
    {
        return $this->adminName;
    }
    public function setAdminPw($adminPw)
    {
        $this->adminPw = $adminPw;
    }
    
    public function getAdminPw()
    {
        return $this->adminPw;
    }
}
?>