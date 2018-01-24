<?php
class Product
{
    private $productId;
    
    private $productType;
    
    private $productName;
    
    private $productInfo;
    
    private $productPrice;
    
    public function setProductId($productId)
    {
        $this->productId = $productId;
    }
    
    public function getProductId()
    {
        return $this->productId;
    }
    public function setProductType($productType)
    {
        $this->productType = $productType;
    }
    
    public function getProductType()
    {
        return $this->productType;
    }
    public function setProductName($productName)
    {
        $this->productName = $productName;
    }
    
    public function getProductName()
    {
        return $this->productName;
    }
    public function setProductInfo($productInfo)
    {
        $this->productInfo = $productInfo;
    }
    
    public function getProductInfo()
    {
        return $this->productInfo;
    }
    public function setProductPrice($productPrice)
    {
        $this->productPrice = $productPrice;
    }
    
    public function getProductPrice()
    {
        return $this->productPrice;
    }
}
?>