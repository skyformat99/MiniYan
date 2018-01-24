DROP DATABASE IF EXISTS BoGame;

CREATE DATABASE IF NOT EXISTS BoGame;

USE BoGame;

CREATE TABLE IF NOT EXISTS UserInfo (
  userId int(10) NOT NULL AUTO_INCREMENT,
  userType int(10) DEFAULT NULL,
  userName varchar(25) DEFAULT NULL,
  userPw varchar(25) DEFAULT NULL,
  PRIMARY KEY (userId ASC)
);

CREATE TABLE IF NOT EXISTS product (
  productId int(10) NOT NULL AUTO_INCREMENT,
  productType int(10) DEFAULT NULL,
  productName varchar(25) DEFAULT NULL,
  productInfo varchar(255) DEFAULT NULL,
  productPrice varchar(25) DEFAULT NULL,
  productImg varchar(25) DEFAULT NULL,
  PRIMARY KEY (productId ASC)
);

CREATE TABLE IF NOT EXISTS admin (
  adminId int(10) NOT NULL AUTO_INCREMENT,
  adminName varchar(25) DEFAULT NULL,
  adminPw varchar(25) DEFAULT NULL,
  PRIMARY KEY (adminId ASC)
);