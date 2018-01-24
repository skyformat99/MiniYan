<?php
class Userinfo
{
    private $userId;
    
    private $userType;
    
    private $userName;
    
    private $userPw;
    
    public function setUserId($userId)
    {
        $this->userId = $userId;
    }
    
    public function getUserId()
    {
        return $this->userId;
    }
    public function setUserType($userType)
    {
        $this->userType = $userType;
    }
    
    public function getUserType()
    {
        return $this->userType;
    }
    public function setUserName($userName)
    {
        $this->userName = $userName;
    }
    
    public function getUserName()
    {
        return $this->userName;
    }
    public function setUserPw($userPw)
    {
        $this->userPw = $userPw;
    }
    
    public function getUserPw()
    {
        return $this->userPw;
    }
}
?>