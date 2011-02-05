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

--
-- Database: `skyproco_phskyprom`
--

-- --------------------------------------------------------

--
-- Table structure for table `Bla000`
--

CREATE TABLE IF NOT EXISTS `Bla000` (
  `uid` varchar(64) COLLATE utf8_unicode_ci NOT NULL,
  `comment` text COLLATE utf8_unicode_ci NOT NULL,
  `en` text COLLATE utf8_unicode_ci,
  `de` text COLLATE utf8_unicode_ci,
  `fr` text COLLATE utf8_unicode_ci,
  `it` text COLLATE utf8_unicode_ci,
  `rm` text COLLATE utf8_unicode_ci,
  PRIMARY KEY (`uid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

-- --------------------------------------------------------

--
-- Table structure for table `blog`
--

CREATE TABLE IF NOT EXISTS `blog` (
  `blog_id` int(11) NOT NULL AUTO_INCREMENT,
  `post_dts` datetime NOT NULL,
  `title` varchar(100) COLLATE utf8_unicode_ci NOT NULL,
  `post` text COLLATE utf8_unicode_ci NOT NULL,
  PRIMARY KEY (`blog_id`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci AUTO_INCREMENT=6 ;

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

-- --------------------------------------------------------

--
-- Table structure for table `permissions`
--

CREATE TABLE IF NOT EXISTS `permissions` (
  `uid` int(11) NOT NULL AUTO_INCREMENT,
  `domain` varchar(16) COLLATE utf8_unicode_ci NOT NULL DEFAULT 'GLOBAL',
  `hash` int(128) NOT NULL,
  PRIMARY KEY (`uid`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci AUTO_INCREMENT=15 ;

-- --------------------------------------------------------

--
-- Table structure for table `permpresets`
--

CREATE TABLE IF NOT EXISTS `permpresets` (
  `uid` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(32) COLLATE utf8_unicode_ci NOT NULL DEFAULT '',
  `permissions` varchar(256) COLLATE utf8_unicode_ci NOT NULL DEFAULT '',
  PRIMARY KEY (`uid`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci AUTO_INCREMENT=6 ;

-- --------------------------------------------------------

--
-- Table structure for table `Users`
--

CREATE TABLE IF NOT EXISTS `Users` (
  `handle` varchar(64) CHARACTER SET utf8 COLLATE utf8_turkish_ci NOT NULL,
  `permissions` varchar(256) COLLATE utf8_unicode_ci NOT NULL DEFAULT '',
  `passhash` varchar(255) COLLATE utf8_unicode_ci NOT NULL DEFAULT '',
  `oAuth` varchar(256) COLLATE utf8_unicode_ci DEFAULT NULL,
  `openID` varchar(256) COLLATE utf8_unicode_ci DEFAULT NULL,
  `email` varchar(128) COLLATE utf8_unicode_ci NOT NULL DEFAULT '',
  `realname` varchar(64) COLLATE utf8_unicode_ci NOT NULL DEFAULT '',
  `uid` int(11) NOT NULL,
  `url` varchar(256) COLLATE utf8_unicode_ci DEFAULT NULL,
  `avatarMediaUID` int(11) NOT NULL,
  `languageOrder` varchar(16) COLLATE utf8_unicode_ci NOT NULL DEFAULT 'de,fr,it,rm,en',
  `country` varchar(32) COLLATE utf8_unicode_ci DEFAULT NULL,
  `region` varchar(32) COLLATE utf8_unicode_ci DEFAULT NULL,
  `karma` int(11) NOT NULL DEFAULT '100',
  PRIMARY KEY (`handle`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

INSERT INTO `permissions` (`uid`, `domain`, `hash`) VALUES
(1, 'SYSTEM', 0),
(2, 'GLOBAL', 0),
(3, 'SYSTEM', 32768),
(4, 'GLOBAL', 32768),
(5, 'GLOBAL', 1),
(6, 'SYSTEM', 1),
(7, 'NOTES', 1),
(8, 'NOTES', 5),
(9, 'NOTES', 21),
(10, 'NOTES', 32789),
(11, 'MEDIA', 1),
(12, 'MEDIA', 5),
(13, 'MEDIA', 21),
(14, 'MEDIA', 32789);

INSERT INTO `permpresets` (`uid`, `name`, `permissions`) VALUES
(1, 'nobody', '1,2'),
(2, 'www', '1,5,7,11'),
(3, 'regUser', '1,5,7,8,11,12'),
(4, 'contentAdmin', '5,6,7,8,9,11,12,13'),
(5, 'SuperUser', '3,4,5,6,7,8,9,10,11,12,13,14');

INSERT INTO `Users` (`handle`, `permissions`, `passhash`, `oAuth`, `openID`, `email`, `realname`, `uid`, `url`, `avatarMediaUID`, `languageOrder`, `country`, `region`, `karma`) VALUES
('SwissalpS', '5', 'a9e290accb79d94bfe2fc24e7e20b4469885864c', NULL, NULL, 'Luke@SwissalpS.ws', 'Luke Zimmermann', 0, 'lukezimmermann.com', 1, 'de,fr,it,rm,en', 'CH', 'BE', 200),
('admin', '4', '2532a2bd437d825ca612be49f7f0768f8f80d935', NULL, NULL, 'admin@foo.bar', 'admin', 0, 'foo.bar', 0, 'de,fr,it,rm,en', 'CH', 'BE', 200);
