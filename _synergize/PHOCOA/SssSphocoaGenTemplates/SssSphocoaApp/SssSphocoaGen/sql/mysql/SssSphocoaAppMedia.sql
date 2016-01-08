-- phpMyAdmin SQL Dump
-- version 3.3.8.1
-- http://www.phpmyadmin.net
--
-- Host: localhost
-- Generation Time: Jan 08, 2011 at 03:41 AM
-- Server version: 5.1.54
-- PHP Version: 5.3.3

SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

-- --------------------------------------------------------

--
-- Table structure for table `mediaMap`
--

CREATE TABLE IF NOT EXISTS `mediaMap` (
  `mediaUID` int(11) NOT NULL AUTO_INCREMENT,
  `url` varchar(256) COLLATE utf8_unicode_ci NOT NULL DEFAULT '',
  `type` varchar(32) COLLATE utf8_unicode_ci NOT NULL DEFAULT '',
  `width` int(7) NOT NULL DEFAULT '240',
  `height` int(7) NOT NULL DEFAULT '80',
  `cssClass` varchar(128) COLLATE utf8_unicode_ci NOT NULL DEFAULT '',
  `mime` varchar(128) COLLATE utf8_unicode_ci NOT NULL DEFAULT '',
  `karma` int(11) NOT NULL DEFAULT '100',
  `bigWidth` int(7) NOT NULL DEFAULT '800',
  `bigHeight` int(7) NOT NULL DEFAULT '600',
  `bigURL` varchar(256) COLLATE utf8_unicode_ci NOT NULL DEFAULT '',
  `thumbWidth` int(7) NOT NULL,
  `thumbHeight` int(7) NOT NULL,
  `thumbUrl` varchar(256) COLLATE utf8_unicode_ci NOT NULL DEFAULT '',
  PRIMARY KEY (`mediaUID`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci AUTO_INCREMENT=108 ;

-- --------------------------------------------------------

--
-- Table structure for table `Notes`
--

CREATE TABLE IF NOT EXISTS `Notes` (
  `uid` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(64) COLLATE utf8_unicode_ci NOT NULL DEFAULT '_name_',
  `email` varchar(128) COLLATE utf8_unicode_ci DEFAULT NULL,
  `url0` varchar(128) COLLATE utf8_unicode_ci DEFAULT NULL,
  `url1` varchar(128) COLLATE utf8_unicode_ci DEFAULT NULL,
  `children` varchar(256) COLLATE utf8_unicode_ci DEFAULT NULL,
  `note` text COLLATE utf8_unicode_ci NOT NULL,
  `mediaUID` int(11) DEFAULT NULL,
  `lang` varchar(2) COLLATE utf8_unicode_ci NOT NULL DEFAULT 'de',
  `country` varchar(32) COLLATE utf8_unicode_ci NOT NULL DEFAULT '',
  `region` varchar(32) COLLATE utf8_unicode_ci NOT NULL DEFAULT '',
  `bridgeUID` int(11) NOT NULL DEFAULT '0',
  `date` int(11) NOT NULL DEFAULT '0',
  `publish` int(1) DEFAULT NULL,
  `karma` int(11) NOT NULL DEFAULT '100',
  `handle` varchar(64) COLLATE utf8_unicode_ci DEFAULT NULL,
  PRIMARY KEY (`uid`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci AUTO_INCREMENT=197 ;
