-- phpMyAdmin SQL Dump
-- version 5.2.1
-- https://www.phpmyadmin.net/
--
-- Host: localhost
-- Generation Time: Aug 04, 2024 at 11:07 PM
-- Server version: 5.7.29-0ubuntu0.18.04.1
-- PHP Version: 7.2.24-0ubuntu0.18.04.2

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `client894_samp`
--

-- --------------------------------------------------------

--
-- Table structure for table `accs`
--

CREATE TABLE `accs` (
  `ID` int(11) NOT NULL,
  `Userid` int(11) NOT NULL DEFAULT '0',
  `Model` int(5) NOT NULL DEFAULT '0',
  `Status` int(2) NOT NULL DEFAULT '0',
  `Bone` int(2) NOT NULL DEFAULT '0',
  `OffsetX` float NOT NULL,
  `OffsetY` float NOT NULL,
  `OffsetZ` float NOT NULL,
  `ScaleX` float NOT NULL,
  `ScaleY` float NOT NULL,
  `ScaleZ` float NOT NULL,
  `RotX` float NOT NULL,
  `RotY` float NOT NULL,
  `RotZ` float NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Dumping data for table `accs`
--

INSERT INTO `accs` (`ID`, `Userid`, `Model`, `Status`, `Bone`, `OffsetX`, `OffsetY`, `OffsetZ`, `ScaleX`, `ScaleY`, `ScaleZ`, `RotX`, `RotY`, `RotZ`) VALUES
(0, 1, 19631, 1, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0);

-- --------------------------------------------------------

--
-- Table structure for table `accs_models`
--

CREATE TABLE `accs_models` (
  `ID` int(11) NOT NULL,
  `Model` int(6) NOT NULL,
  `Name` varchar(24) NOT NULL,
  `Money` int(11) NOT NULL DEFAULT '1000000',
  `PP` int(3) NOT NULL DEFAULT '0',
  `Stock` int(3) NOT NULL DEFAULT '20'
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Dumping data for table `accs_models`
--

INSERT INTO `accs_models` (`ID`, `Model`, `Name`, `Money`, `PP`, `Stock`) VALUES
(1, 18947, 'Hat', 0, 50, 99),
(2, 18948, 'Hat', 15000, 0, 99),
(3, 18945, 'Hat', 15000, 0, 100),
(4, 18949, 'Hat', 15000, 0, 100),
(5, 18950, 'Hat', 15000, 0, 99),
(6, 18951, 'Hat', 15000, 0, 100),
(7, 18941, 'Hat', 15000, 0, 100),
(8, 18942, 'Hat', 15000, 0, 100),
(9, 18943, 'Hat', 15000, 0, 99),
(10, 19095, 'Hat', 15000, 0, 100),
(11, 18962, 'Hat', 15000, 0, 100),
(12, 19097, 'Hat', 15000, 0, 100),
(13, 19096, 'Hat', 15000, 0, 100),
(14, 18934, 'Hat', 15000, 0, 100),
(15, 18969, 'Hat', 15000, 0, 99),
(16, 18945, 'Hat', 15000, 0, 100),
(17, 18962, 'Hat', 15000, 0, 100),
(18, 19098, 'Hat', 15000, 0, 100),
(19, 19096, 'Hat', 15000, 0, 100),
(20, 18935, 'Hat', 15000, 0, 100),
(21, 18934, 'Hat', 15000, 0, 100),
(22, 19488, 'Hat', 15000, 0, 100),
(23, 18931, 'Hat', 15000, 0, 100),
(24, 19487, 'Hat', 15000, 0, 100),
(25, 19558, 'Hat', 15000, 0, 100),
(26, 19160, 'Hat', 15000, 0, 100),
(27, 19098, 'Hat', 15000, 0, 100),
(28, 19528, 'Hat', 15000, 0, 99),
(29, 19006, 'Eye Glasses', 15000, 0, 99),
(30, 19008, 'Eye Glasses', 15000, 0, 100),
(31, 19007, 'Eye Glasses', 15000, 0, 100),
(32, 19009, 'Eye Glasses', 15000, 0, 100),
(33, 19010, 'Eye Glasses', 15000, 0, 100),
(34, 19011, 'Eye Glasses', 15000, 0, 99),
(35, 19012, 'Eye Glasses', 15000, 0, 100),
(36, 19018, 'Eye Glasses', 15000, 0, 100),
(37, 19013, 'Eye Glasses', 15000, 0, 100),
(38, 19014, 'Eye Glasses', 15000, 0, 100),
(39, 19015, 'Eye Glasses', 15000, 0, 99),
(40, 19016, 'Eye Glasses', 15000, 0, 100),
(41, 19017, 'Eye Glasses', 15000, 0, 100),
(42, 19019, 'Eye Glasses', 15000, 0, 100),
(43, 19020, 'Eye Glasses', 15000, 0, 100),
(44, 19021, 'Eye Glasses', 15000, 0, 100),
(45, 19022, 'Eye Glasses', 15000, 0, 100),
(46, 19023, 'Eye Glasses', 15000, 0, 100),
(47, 19024, 'Eye Glasses', 15000, 0, 100),
(48, 19025, 'Eye Glasses', 15000, 0, 100),
(49, 19026, 'Eye Glasses', 15000, 0, 100),
(50, 19027, 'Eye Glasses', 15000, 0, 100),
(51, 19028, 'Eye Glasses', 15000, 0, 100),
(52, 19029, 'Eye Glasses', 15000, 0, 100),
(53, 19030, 'Eye Glasses', 15000, 0, 100),
(54, 19031, 'Eye Glasses', 15000, 0, 100),
(55, 19032, 'Eye Glasses', 15000, 0, 100),
(56, 19033, 'Eye Glasses', 15000, 0, 100),
(57, 19034, 'Eye Glasses', 15000, 0, 100),
(58, 19035, 'Eye Glasses', 15000, 0, 100),
(59, 19039, 'Watch', 15000, 0, 100),
(60, 19042, 'Watch', 15000, 0, 100),
(61, 19053, 'Watch', 15000, 0, 100),
(62, 19052, 'Watch', 15000, 0, 100),
(63, 19051, 'Watch', 15000, 0, 100),
(64, 19050, 'Watch', 15000, 0, 100),
(65, 19049, 'Watch', 15000, 0, 100),
(66, 19048, 'Watch', 15000, 0, 100),
(67, 19047, 'Watch', 15000, 0, 100),
(68, 19046, 'Watch', 15000, 0, 100),
(69, 19045, 'Watch', 15000, 0, 100),
(70, 19044, 'Watch', 15000, 0, 100),
(71, 19043, 'Watch', 15000, 0, 100),
(72, 19041, 'Watch', 15000, 0, 100),
(73, 19040, 'Watch', 15000, 0, 100),
(74, 19081, 'Laser', 15000, 0, 100),
(75, 19084, 'Laser', 15000, 0, 100),
(76, 18643, 'Laser', 15000, 0, 100),
(77, 19080, 'Laser', 15000, 0, 100),
(78, 19082, 'Laser', 15000, 0, 100),
(79, 19083, 'Laser', 15000, 0, 99),
(80, 2404, 'Special', 0, 30, 30),
(81, 18637, 'Special', 0, 30, 28),
(82, 18632, 'Special', 0, 30, 29),
(83, 1210, 'Special', 0, 30, 29),
(84, 19878, 'Special', 0, 30, 30),
(85, 2704, 'Special', 0, 30, 30),
(86, 19038, 'Bandana & Masks', 15000, 0, 50),
(87, 19036, 'Bandana & Masks', 15000, 0, 50),
(88, 19037, 'Bandana & Masks', 15000, 0, 50),
(89, 18919, 'Bandana & Masks', 15000, 0, 50),
(90, 18912, 'Bandana & Masks', 15000, 0, 50),
(91, 18913, 'Bandana & Masks', 15000, 0, 50),
(92, 18914, 'Bandana & Masks', 15000, 0, 50),
(93, 18915, 'Bandana & Masks', 15000, 0, 50),
(94, 18916, 'Bandana & Masks', 15000, 0, 50),
(95, 18917, 'Bandana & Masks', 15000, 0, 50),
(96, 18918, 'Bandana & Masks', 15000, 0, 48),
(97, 18920, 'Bandana & Masks', 15000, 0, 50),
(98, 19317, 'Special', 0, 30, 29),
(99, 19318, 'Special', 0, 30, 30),
(100, 19631, 'Sledgehammer', 200000, 0, 98);

-- --------------------------------------------------------

--
-- Table structure for table `allowedcomputers`
--

CREATE TABLE `allowedcomputers` (
  `ID` int(11) NOT NULL,
  `computer_hwid` varchar(128) DEFAULT NULL,
  `playerSQL` int(11) DEFAULT NULL,
  `connectTime` int(11) DEFAULT NULL,
  `IPAddress` varchar(16) DEFAULT NULL,
  `Blocked` int(11) NOT NULL DEFAULT '0'
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `allowedcomputers`
--

INSERT INTO `allowedcomputers` (`ID`, `computer_hwid`, `playerSQL`, `connectTime`, `IPAddress`, `Blocked`) VALUES
(1, 'CDCE9A88989C8EF09DECDADCDDC5CCDDC4A9EEC9', 4, 1722467510, '127.0.0.1', 0);

-- --------------------------------------------------------

--
-- Table structure for table `apartaments`
--

CREATE TABLE `apartaments` (
  `ID` int(11) NOT NULL,
  `OwnerSQL` int(11) DEFAULT NULL,
  `OwnerName` varchar(24) NOT NULL DEFAULT 'AdmBot',
  `Locked` int(11) DEFAULT NULL,
  `ExteriorX` float DEFAULT NULL,
  `ExteriorY` float DEFAULT NULL,
  `ExteriorZ` float DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Dumping data for table `apartaments`
--

INSERT INTO `apartaments` (`ID`, `OwnerSQL`, `OwnerName`, `Locked`, `ExteriorX`, `ExteriorY`, `ExteriorZ`) VALUES
(1, 0, 'AdmBot', 0, 276.376, -1620.33, 96.5),
(2, 0, 'AdmBot', 0, 294.761, -1623.59, 96.5),
(3, 0, 'AdmBot', 0, 276.656, -1620.32, 92),
(4, 0, 'AdmBot', 0, 294.671, -1623.63, 92),
(5, 0, 'AdmBot', 0, 276.629, -1620.46, 87.5),
(6, 0, 'AdmBot', 0, 294.589, -1623.51, 87.5),
(7, 0, 'AdmBot', 0, 276.616, -1620.44, 83),
(8, 0, 'AdmBot', 0, 294.57, -1623.67, 83),
(9, 0, 'AdmBot', 0, 276.467, -1620.32, 78.5),
(10, 0, 'AdmBot', 0, 294.618, -1623.57, 78.5),
(11, 0, 'AdmBot', 0, 276.753, -1620.41, 74),
(12, 0, 'AdmBot', 0, 294.478, -1623.64, 74),
(13, 0, 'AdmBot', 0, 276.653, -1620.36, 69.5),
(14, 0, 'AdmBot', 0, 294.657, -1623.68, 69.5),
(15, 0, 'AdmBot', 0, 276.435, -1620.37, 65),
(16, 0, 'AdmBot', 0, 294.604, -1623.63, 65),
(17, 0, 'AdmBot', 0, 276.445, -1620.42, 60.5),
(18, 0, 'AdmBot', 0, 294.646, -1623.64, 60.5),
(19, 0, 'AdmBot', 0, 276.525, -1620.39, 56),
(20, 0, 'AdmBot', 0, 294.54, -1623.71, 56),
(21, 0, 'AdmBot', 0, 276.559, -1620.39, 51.5),
(22, 0, 'AdmBot', 0, 294.639, -1623.71, 51.5),
(23, 0, 'AdmBot', 0, 276.635, -1620.46, 47),
(24, 0, 'AdmBot', 0, 294.537, -1623.67, 47);

-- --------------------------------------------------------

--
-- Table structure for table `atms`
--

CREATE TABLE `atms` (
  `atmId` int(11) NOT NULL,
  `atmPosX` float DEFAULT NULL,
  `atmPosY` float DEFAULT NULL,
  `atmPosZ` float DEFAULT NULL,
  `atmPosRotX` float DEFAULT NULL,
  `atmPosRotY` float DEFAULT NULL,
  `atmPosRotZ` float DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `atms`
--

INSERT INTO `atms` (`atmId`, `atmPosX`, `atmPosY`, `atmPosZ`, `atmPosRotX`, `atmPosRotY`, `atmPosRotZ`) VALUES
(1, 1497.75, -1749.87, 15.0882, 0, 0, 177.381),
(2, 2139.45, -1164.08, 23.6351, 0, 0, 91.3095),
(3, 2093.51, -1359.55, 23.6273, 0, 0, 0),
(4, 1795.71, -1164.84, 23.3281, 0, 0, 346.134),
(5, 1482.78, -1010.34, 26.4866, 0, 0, 0),
(6, 1460.07, -1135.88, 23.4967, 0, 0, 40.2302),
(7, 999.713, -914.554, 41.8281, 0, 0, 268.942),
(8, 691.082, -618.562, 15.9788, 0, 0, 268.691),
(9, 651.193, -520.488, 15.9788, 0, 0, 0),
(10, 565.689, -1293.93, 16.7482, 0, 0, 189.384),
(11, 387.166, -1816.05, 7.48341, 0, 0, 272.48),
(12, 1155.62, -1464.91, 15.4432, 0, 0, 290.21),
(13, 2065.44, -1897.55, 13.1967, 0, 0, 0),
(14, 2105.45, -1802.95, 13.0547, 0, 0, 275.634),
(15, 1832.76, -1835.06, 13.0781, 0, 0, 268.293),
(16, 2228.39, -1707.78, 13.25, 0, 0, 270),
(17, 1928.61, -1779.85, 13.1866, 0, 0, 88),
(18, 2324.4, -1644.94, 14.4699, 0, 0, 0),
(19, 1366.65, -1284.45, 13.0469, 0, 0, 283.959),
(20, 2316.1, -88.5226, 26.1273, 0, 0, 0),
(21, 2303.46, -13.5396, 26.1273, 0, 0, 272.435),
(22, 1260.88, 209.302, 19.1976, 0, 0, 65.5046),
(23, 1275.8, 368.315, 19.1976, 0, 0, 73.7599),
(24, 173.235, -155.076, 1.22102, 0, 0, 89.73),
(25, 45.7804, -291.809, 1.5024, 0, 0, 182.934),
(26, 2019.56, 1002.4, 10.3203, 0, 0, 99.9361),
(27, 2173.7, 1395.76, 10.5625, 0, 0, 270.491),
(28, 2631.46, 1129.53, 10.6797, 0, 0, 2.4883),
(29, 2102.31, 2261.01, 10.5234, 0, 0, 91.8659),
(30, 2001.57, 1547.05, 13.0859, 0, 0, 87.7158);

-- --------------------------------------------------------

--
-- Table structure for table `auctions`
--

CREATE TABLE `auctions` (
  `ID` int(11) NOT NULL,
  `ProprietyID` int(11) DEFAULT NULL,
  `ProprietyType` int(11) DEFAULT NULL,
  `Expected` int(11) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- --------------------------------------------------------

--
-- Table structure for table `banned_words`
--

CREATE TABLE `banned_words` (
  `id` int(11) NOT NULL,
  `word` varchar(30) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `banned_words`
--

INSERT INTO `banned_words` (`id`, `word`) VALUES
(1, 'bzone'),
(2, 'bugged'),
(3, 'lupmax'),
(4, 'b-zone'),
(5, 'og-times'),
(6, 'ogtimes'),
(7, 'b-game'),
(8, 'gfzone'),
(9, 'famzone'),
(10, 't4p'),
(11, 'time4play'),
(12, 'pro-gaming'),
(13, 'wrestling-arena'),
(14, 'just2play'),
(15, 'j2p'),
(16, 'playnion'),
(17, 'egaming'),
(18, 'projectx'),
(19, 'expertgame'),
(20, 'nephrite'),
(21, 'ruby'),
(22, 'jade'),
(23, 'cozo'),
(24, 'b-hood'),
(25, 'bhood'),
(27, 'crowland'),
(28, 'redtimes'),
(29, 'rpg.'),
(30, 'beta.'),
(31, 'easy.'),
(32, '.ro'),
(33, '.tk'),
(34, 'red-times');

-- --------------------------------------------------------

--
-- Table structure for table `bans`
--

CREATE TABLE `bans` (
  `name` varchar(24) DEFAULT NULL,
  `bannedPlayer` int(11) DEFAULT NULL,
  `bannedBy` varchar(24) NOT NULL DEFAULT 'AdmBot',
  `bannedReason` varchar(32) DEFAULT NULL,
  `bannedDays` int(11) DEFAULT NULL,
  `iTimestamp` int(11) DEFAULT NULL,
  `banHardware` varchar(150) DEFAULT NULL,
  `banIP` varchar(16) DEFAULT NULL,
  `ID` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- --------------------------------------------------------

--
-- Table structure for table `businesses`
--

CREATE TABLE `businesses` (
  `ID` int(11) NOT NULL,
  `OwnerSQL` int(11) DEFAULT NULL,
  `Type` int(11) DEFAULT NULL,
  `Static` int(11) DEFAULT NULL,
  `PosX` float DEFAULT NULL,
  `PosY` float DEFAULT NULL,
  `PosZ` float DEFAULT NULL,
  `Message` varchar(20) NOT NULL DEFAULT 't4p.ro',
  `secPosX` float DEFAULT NULL,
  `secPosY` float DEFAULT NULL,
  `secPosZ` float DEFAULT NULL,
  `Interior` int(11) DEFAULT NULL,
  `bizMoney` int(11) DEFAULT NULL,
  `bizFee` int(11) NOT NULL DEFAULT '500',
  `bizLocked` int(11) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Dumping data for table `businesses`
--

INSERT INTO `businesses` (`ID`, `OwnerSQL`, `Type`, `Static`, `PosX`, `PosY`, `PosZ`, `Message`, `secPosX`, `secPosY`, `secPosZ`, `Interior`, `bizMoney`, `bizFee`, `bizLocked`) VALUES
(1, 1, 1, 0, 1460.23, -1010.7, 26.8438, 'Time4Play Business', 4538.43, -14927.2, 254.642, 0, 25000, 500, 0),
(2, 0, 2, 0, 1367.99, -1279.75, 13.5469, 'Time4Play Business', 285.791, -86.23, 1001.52, 4, 87000, 2500, 0),
(3, 0, 3, 0, 1836.09, -1682.5, 13.3624, 'Time4Play Business', 493.46, -24.0736, 1000.68, 17, 2000, 500, 0),
(4, 0, 7, 0, 2102.38, 2228.72, 11.0234, 'Time4Play Business', 364.942, -11.0787, 1001.85, 9, 500, 500, 0),
(5, 0, 3, 0, 2421.41, -1220.3, 25.4937, 'Time4Play Business', 1204.83, -13.4011, 1000.92, 2, 2500, 500, 0),
(6, 0, 3, 0, 2309.85, -1643.54, 14.827, 'Time4Play Business', -794.942, 490.782, 1376.2, 1, 2500, 500, 0),
(7, 0, 16, 0, 1087.63, -923.003, 43.3906, 'Time4Play Business', -100.403, -24.3921, 1000.72, 3, 0, 500, 0),
(8, 0, 9, 0, 2019.6, 1007.68, 10.8203, 'Time4Play Business', 2015.45, 1017.09, 996.875, 10, 15500, 500, 0),
(9, 0, 2, 0, 1791.71, -1163.6, 23.8281, 'Time4Play Business', 285.791, -86.23, 1001.52, 4, 31500, 500, 0),
(10, 0, 4, 0, 1352.16, -1759.17, 13.5078, 'Time4Play Business', -27.2393, -58.1129, 1003.55, 6, 117000, 500, 0),
(11, 0, 4, 0, 999.932, -919.889, 42.3281, 'Time4Play Business', -27.2393, -58.1129, 1003.55, 6, 7000, 500, 0),
(12, 0, 4, 0, 1833.14, -1842.5, 13.5781, 'Time4Play Business', -27.2393, -58.1129, 1003.55, 6, 271000, 500, 0),
(13, 0, 4, 0, 1929.37, -1776.14, 13.5469, 'Time4Play Business', -27.2393, -58.1129, 1003.55, 6, 11000, 500, 0),
(14, 0, 4, 0, 1315.7, -898.097, 39.5781, 'Time4Play Business', -27.2393, -58.1129, 1003.55, 6, 8500, 500, 0),
(15, 0, 5, 0, 1199.38, -919.098, 43.1152, 'Time4Play Business', 363.134, -74.8469, 1001.51, 10, 1000, 500, 0),
(16, 0, 5, 0, 810.737, -1616.19, 13.5469, 'Time4Play Business', 363.134, -74.8469, 1001.51, 10, 1500, 500, 0),
(17, 0, 5, 0, 2472.79, 2034.26, 11.0625, 'Time4Play Business', 363.134, -74.8469, 1001.51, 10, 0, 500, 0),
(18, 0, 5, 0, 1158.14, 2072.27, 11.0625, 'Time4Play Business', 363.134, -74.8469, 1001.51, 10, 0, 500, 0),
(19, 0, 5, 0, 2170.06, 2795.64, 10.8203, 'Time4Play Business', 363.134, -74.8469, 1001.51, 10, 0, 500, 0),
(20, 0, 5, 0, 1872.71, 2071.67, 11.0625, 'Time4Play Business', 363.134, -74.8469, 1001.51, 10, 0, 500, 0),
(21, 0, 1, 0, 2194.99, 1676.89, 12.3672, 'Time4Play Business', 4538.43, -14927.2, 254.642, 0, 0, 500, 0),
(22, 0, 2, 0, 2556.62, 2063.98, 11.0995, 'Time4Play Business', 285.791, -86.23, 1001.52, 4, 6500, 500, 0),
(23, 0, 16, 0, 2515.95, 2297.69, 10.8203, 'Time4Play Business', -100.403, -24.3921, 1000.72, 3, 0, 500, 0),
(24, 0, 4, 0, 2637.61, 1129.28, 11.1797, 'Time4Play Business', -27.2393, -58.1129, 1003.55, 6, 1000, 500, 0),
(25, 0, 6, 0, 2229.22, -1721.89, 13.5671, 'Time4Play Business', 772.112, -3.89865, 1000.73, 5, 8500, 500, 0),
(26, 0, 15, 0, 2104.57, -1806.67, 13.5547, 'Time4Play Business', 372.54, -133.009, 1001.49, 5, 0, 500, 0),
(27, 0, 1, 0, 1102.91, -1440.13, 15.7969, 'Time4Play Business', 4538.43, -14927.2, 254.642, 0, 3500, 500, 0),
(28, 0, 13, 1, 1168.43, -1489.36, 22.757, 'Time4Play Business', 0, 0, 0, 0, 21110, 500, 0),
(29, 0, 8, 0, 2102.56, 2257.37, 11.0234, 'Time4Play Business', 207.738, -109.02, 1005.13, 15, 36500, 500, 0),
(30, 0, 8, 0, 499.936, -1359.74, 16.2911, 'Time4Play Business', 161.391, -93.1592, 1001.8, 18, 171000, 500, 0),
(31, 0, 14, 1, 562.411, -1288.06, 17.248, 'Time4Play Business', 557.152, -1288.91, 17.248, 0, 80000, 500, 0),
(32, 0, 8, 0, 1456.98, -1138.04, 23.9778, 'Time4Play Business', 161.391, -93.1592, 1001.8, 18, 190000, 500, 0),
(33, 0, 12, 1, 1941.6, -1764.74, 13.6406, 'Time4Play Business', 0, 0, 0, 0, 0, 500, 0),
(34, 0, 10, 1, 723.409, -463.627, 16.3359, 'Time4Play Business', 720.032, -457.613, 16.336, 0, 2000, 500, 0),
(35, 0, 10, 1, 2072.57, -1828.03, 13.5469, 'Time4Play Business', 2065.13, -1831.39, 13.547, 0, 0, 500, 0),
(36, 0, 10, 1, 1034.7, -1028.1, 32.1016, 'Time4Play Business', 1024.99, -1024.5, 32.102, 0, 0, 500, 0),
(37, 0, 10, 1, 485.37, -1733.32, 11.094, 'Time4Play Business', 487.653, -1739.55, 11.141, 0, 0, 500, 0),
(38, 0, 11, 1, 1037.11, -1025.04, 32.1016, 'Time4Play Business', 1041.58, -1019.56, 32.108, 0, 0, 500, 0),
(39, 0, 11, 1, 2650.33, -2021.76, 14.1766, 'Time4Play Business', 2644.82, -2044.2, 13.633, 0, 0, 500, 0),
(40, 0, 10, 1, 1966.45, 2156.36, 10.8203, 'Time4Play Business', 1972.54, 2162.45, 11.07, 0, 0, 500, 0),
(41, 0, 10, 1, -94.1097, 1109.86, 19.7422, 'Time4Play Business', -99.93, 1117.75, 19.742, 0, 0, 500, 0),
(42, 0, 12, 1, 1012.9, -935.545, 42.1797, 'Time4Play Business', 0, 0, 0, 0, 0, 500, 0),
(43, 0, 12, 1, 655.566, -572.436, 16.5015, 'Time4Play Business', 0, 0, 0, 0, 0, 500, 0),
(44, 0, 12, 1, -78.6301, -1169.16, 2.1491, 'Time4Play Business', 0, 0, 0, 0, 0, 500, 0),
(45, 0, 12, 1, 2646.33, 1101.31, 10.9609, 'Time4Play Business', 0, 0, 0, 0, 0, 500, 0),
(46, 0, 12, 1, 2139.81, 2753.5, 10.8203, 'Time4Play Business', 0, 0, 0, 0, 0, 500, 0),
(47, 0, 12, 1, 2207.51, 2482.56, 10.8203, 'Time4Play Business', 0, 0, 0, 0, 0, 500, 0),
(48, 0, 12, 1, 1604.09, 2204.47, 10.8203, 'Time4Play Business', 0, 0, 0, 0, 0, 500, 0),
(49, 0, 12, 1, 2122.56, 914.539, 10.8203, 'Time4Play Business', 0, 0, 0, 0, 0, 500, 0),
(50, 0, 12, 1, 638.368, 1683.78, 7.1875, 'Time4Play Business', 0, 0, 0, 0, 0, 500, 0),
(51, 0, 11, 1, 2392.17, 1042.01, 10.8203, 'Time4Play Business', 0, 0, 0, 0, 0, 500, 0),
(52, 0, 13, 1, 2079.2, 2045.07, 11.0579, 'Time4Play Business', 0, 0, 0, 0, 10555, 500, 0),
(53, 0, 8, 0, 2244.38, -1665.16, 15.4766, 'Time4Play Business', 207.738, -109.02, 1005.13, 15, 22500, 500, 0),
(54, 0, 7, 0, 172.527, 1176.09, 14.7645, 'Time4Play Business', 364.942, -11.0787, 1001.85, 9, 0, 500, 0),
(55, 0, 14, 1, 2229.39, 1397.45, 11.063, 'Time4Play Business', 2233.43, 1389.94, 10.813, 0, 0, 500, 0),
(56, NULL, 4, 0, 507.753, -1608.13, 16.384, 'Time4Play Business', -27.239, -58.113, 1003.55, 6, 12000, 500, NULL);

-- --------------------------------------------------------

--
-- Table structure for table `clans`
--

CREATE TABLE `clans` (
  `ID` int(11) NOT NULL,
  `clanName` varchar(32) DEFAULT NULL,
  `clanTag` varchar(10) DEFAULT NULL,
  `clanColor` varchar(10) NOT NULL DEFAULT 'FFCC99',
  `clanMOTD` varchar(128) DEFAULT NULL,
  `clanRank1` varchar(32) NOT NULL DEFAULT 'Newbie',
  `clanRank2` varchar(32) NOT NULL DEFAULT 'Member',
  `clanRank3` varchar(32) NOT NULL DEFAULT 'Advanced',
  `clanRank4` varchar(32) NOT NULL DEFAULT 'Expert',
  `clanRank5` varchar(32) NOT NULL DEFAULT 'Legend',
  `clanRank6` varchar(32) NOT NULL DEFAULT 'Manager',
  `clanRank7` varchar(32) NOT NULL DEFAULT 'Owner',
  `safeMoney` int(11) DEFAULT NULL,
  `Active` int(11) NOT NULL DEFAULT '1',
  `clanSlots` int(11) NOT NULL DEFAULT '25'
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Dumping data for table `clans`
--

INSERT INTO `clans` (`ID`, `clanName`, `clanTag`, `clanColor`, `clanMOTD`, `clanRank1`, `clanRank2`, `clanRank3`, `clanRank4`, `clanRank5`, `clanRank6`, `clanRank7`, `safeMoney`, `Active`, `clanSlots`) VALUES
(1, 'Golden Angels', 'GA', 'FFCC99', NULL, 'Newbie', 'Member', 'Advanced', 'Expert', 'Legend', 'Manager', '{ff0000}Owner', NULL, 1, 25);

-- --------------------------------------------------------

--
-- Table structure for table `clanvehicles`
--

CREATE TABLE `clanvehicles` (
  `ID` int(11) NOT NULL,
  `modelID` int(11) DEFAULT NULL,
  `clanID` int(11) DEFAULT NULL,
  `clanRank` int(11) NOT NULL DEFAULT '1',
  `posX` float DEFAULT NULL,
  `posY` float DEFAULT NULL,
  `posZ` float DEFAULT NULL,
  `posA` float DEFAULT NULL,
  `vehColor1` int(11) DEFAULT NULL,
  `vehColor2` int(11) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- --------------------------------------------------------

--
-- Table structure for table `dealership`
--

CREATE TABLE `dealership` (
  `ds_Id` int(11) NOT NULL,
  `ds_vStock` int(11) DEFAULT NULL,
  `ds_vPrice` int(20) DEFAULT NULL,
  `ds_vModel` int(11) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `dealership`
--

INSERT INTO `dealership` (`ds_Id`, `ds_vStock`, `ds_vPrice`, `ds_vModel`) VALUES
(1, 50, 500000, 509),
(2, 50, 700000, 462),
(3, 50, 800000, 499),
(4, 50, 850000, 404),
(5, 50, 900000, 600),
(6, 50, 900000, 413),
(7, 50, 950000, 440),
(8, 50, 1000000, 510),
(9, 50, 1000000, 418),
(10, 50, 1000000, 483),
(11, 50, 1100000, 543),
(12, 50, 1100000, 478),
(13, 50, 1200000, 479),
(14, 50, 1300000, 401),
(15, 50, 1400000, 467),
(16, 50, 1400000, 466),
(17, 50, 1450000, 529),
(18, 50, 1500000, 474),
(19, 50, 1700000, 410),
(20, 50, 1700000, 471),
(21, 50, 1800000, 546),
(22, 50, 1900000, 459),
(23, 50, 2000000, 436),
(24, 50, 2000000, 547),
(25, 50, 2000000, 526),
(26, 50, 2100000, 491),
(27, 50, 2100000, 517),
(28, 50, 2200000, 422),
(29, 50, 2200000, 527),
(30, 50, 2300000, 550),
(31, 50, 2500000, 482),
(32, 50, 2500000, 458),
(33, 50, 2600000, 500),
(34, 50, 2700000, 508),
(35, 50, 2800000, 551),
(36, 50, 2800000, 585),
(37, 50, 3000000, 400),
(38, 50, 3100000, 516),
(39, 50, 3300000, 419),
(40, 50, 3400000, 439),
(41, 50, 3400000, 580),
(42, 50, 3400000, 542),
(43, 50, 3500000, 549),
(44, 50, 4000000, 496),
(45, 50, 4000000, 492),
(46, 50, 4000000, 434),
(47, 50, 4000000, 545),
(48, 50, 4100000, 540),
(49, 50, 4200000, 561),
(50, 50, 4250000, 507),
(51, 50, 4300000, 421),
(52, 50, 4500000, 575),
(53, 50, 4700000, 566),
(54, 50, 4700000, 567),
(55, 50, 5000000, 576),
(56, 50, 5000000, 535),
(57, 50, 5000000, 518),
(58, 50, 5200000, 554),
(59, 50, 5300000, 555),
(60, 50, 5700000, 534),
(61, 50, 5900000, 586),
(62, 50, 6000000, 504),
(63, 50, 6200000, 426),
(64, 50, 6500000, 589),
(65, 50, 6700000, 603),
(66, 50, 6700000, 489),
(67, 50, 6700000, 463),
(68, 50, 6700000, 475),
(69, 50, 6900000, 536),
(70, 50, 7000000, 405),
(71, 50, 7000000, 533),
(72, 50, 7000000, 587),
(73, 50, 7000000, 445),
(74, 50, 7500000, 412),
(75, 50, 7700000, 468),
(76, 50, 8000000, 602),
(77, 50, 9000000, 565),
(78, 50, 9000000, 558),
(79, 50, 9000000, 579),
(80, 50, 12000000, 480),
(81, 50, 13000000, 581),
(82, 50, 14000000, 506),
(83, 50, 16000000, 461),
(84, 50, 18000000, 521),
(85, 50, 19000000, 477),
(86, 50, 20000000, 559),
(87, 50, 21000000, 402),
(88, 50, 22000000, 495),
(89, 50, 23000000, 562),
(90, 50, 25000000, 415),
(91, 50, 27000000, 429),
(92, 50, 40000000, 451),
(93, 50, 40000000, 560),
(94, 50, 42000000, 541),
(95, 50, 45000000, 522),
(96, 50, 95000000, 411),
(97, 50, 100000000, 469),
(98, 50, 100000000, 503),
(99, 50, 100000000, 502),
(100, 50, 100000000, 539),
(101, 50, 100000000, 494),
(102, 50, 100000000, 487);

-- --------------------------------------------------------

--
-- Table structure for table `emails`
--

CREATE TABLE `emails` (
  `ID` int(11) NOT NULL,
  `iPlayer` int(11) DEFAULT NULL,
  `sMessage` varchar(256) DEFAULT NULL,
  `iTimestamp` int(11) DEFAULT NULL,
  `iReadStatus` int(11) DEFAULT NULL,
  `DeliverStatus` int(11) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `emails`
--

INSERT INTO `emails` (`ID`, `iPlayer`, `sMessage`, `iTimestamp`, `iReadStatus`, `DeliverStatus`) VALUES
(1, 1, 'Ai fost demis de Admin ionut din factiunea din care faceai parte Los Santos Police Department (rank 2) dupa 0 zile, fara FP. Motiv: 0 0.', 1721387615, 1, NULL),
(2, 1, 'Zile insuficiente, nu ai primit rank-up, foloseste /myraport.', 1721387628, 1, NULL),
(3, 1, 'Ai fost demis de Admin ionut2 din factiunea din care faceai parte Los Santos Police Department (rank 7) dupa 3 zile, fara FP. Motiv: 0 0 0.', 1721685661, 1, NULL),
(4, 3, 'Ai fost demis de Admin ioneltalent din factiunea din care faceai parte Los Santos Police Department (rank 7) dupa 0 zile, fara FP. Motiv: 0 0 .', 1722119128, 1, NULL),
(5, 3, 'Ai fost demis de Admin ioneltalent din factiunea din care faceai parte Los Santos Police Department (rank 7) dupa 0 zile, fara FP. Motiv: 0 0.', 1722201152, 1, NULL),
(6, 3, 'Ai fost demis de Admin ioneltalent din factiunea din care faceai parte Los Santos Police Department (rank 7) dupa 0 zile, fara FP. Motiv: 0 0.', 1722270724, 1, NULL),
(7, 3, 'Ai fost demis de Admin ioneltalent din factiunea din care faceai parte Los Santos Police Department (rank 7) dupa 0 zile, fara FP. Motiv: 0 0 .', 1722270742, 0, NULL),
(8, 5, 'Ai fost demis de Admin Ramos din factiunea din care faceai parte Los Santos Police Department (rank 7) dupa 0 zile, fara FP. Motiv: 0.', 1722800674, 1, NULL);

-- --------------------------------------------------------

--
-- Table structure for table `factions`
--

CREATE TABLE `factions` (
  `ID` int(11) NOT NULL,
  `Name` varchar(32) DEFAULT NULL,
  `Motd` varchar(128) NOT NULL DEFAULT 'None',
  `Locked` int(11) NOT NULL DEFAULT '1',
  `Slots` int(11) NOT NULL DEFAULT '20',
  `ExteriorX` float DEFAULT NULL,
  `ExteriorY` float DEFAULT NULL,
  `ExteriorZ` float DEFAULT NULL,
  `Money` int(11) DEFAULT NULL,
  `Materials` int(11) DEFAULT NULL,
  `Drugs` int(11) DEFAULT NULL,
  `Rank1` varchar(32) NOT NULL DEFAULT 'Rank 1',
  `Rank2` varchar(32) NOT NULL DEFAULT 'Rank 2',
  `Rank3` varchar(32) NOT NULL DEFAULT 'Rank 3',
  `Rank4` varchar(32) NOT NULL DEFAULT 'Rank 4',
  `Rank5` varchar(32) NOT NULL DEFAULT 'Rank 5',
  `Rank6` varchar(32) NOT NULL DEFAULT 'Rank 6',
  `Rank7` varchar(32) NOT NULL DEFAULT 'Leader',
  `Raport1` varchar(64) NOT NULL DEFAULT '10 10 10 10 10',
  `Raport2` varchar(64) NOT NULL DEFAULT '5 5 5 5 5',
  `Raport3` varchar(64) NOT NULL DEFAULT '0 0 0 0 0',
  `Raport4` varchar(64) NOT NULL DEFAULT '0 0 0 0 0',
  `Raport5` varchar(64) NOT NULL DEFAULT '0 0 0 0 0',
  `Application` int(11) NOT NULL DEFAULT '0',
  `MinLevel` int(11) DEFAULT NULL,
  `LeaderApp` tinyint(1) DEFAULT '0',
  `factionColor` int(20) NOT NULL DEFAULT '-1'
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Dumping data for table `factions`
--

INSERT INTO `factions` (`ID`, `Name`, `Motd`, `Locked`, `Slots`, `ExteriorX`, `ExteriorY`, `ExteriorZ`, `Money`, `Materials`, `Drugs`, `Rank1`, `Rank2`, `Rank3`, `Rank4`, `Rank5`, `Rank6`, `Rank7`, `Raport1`, `Raport2`, `Raport3`, `Raport4`, `Raport5`, `Application`, `MinLevel`, `LeaderApp`, `factionColor`) VALUES
(1, 'Los Santos Police Department', 'Welcome to Los Santos Police Department!', 0, 20, 1554.9, -1675.57, 16.1953, 1, 0, 0, 'Rank 1', 'Rank 2', 'Rank 3', 'Rank 4', 'Rank 5', 'Rank 6', 'sef', '1 1 1 1 1', '2 2 2 2 2', '3 3 3 3 3', '4 4 4 4 4', '5 5 5 5 5', 1, 3, 0, 4837376),
(2, 'Federal Bureau Of Investigations', 'Welcome to Federal Bureau Of Investigations!', 1, 20, 627.616, -571.792, 17.6242, 0, 0, 0, 'Rank 1', 'Rank 2', 'Rank 3', 'Rank 4', 'Rank 5', 'Rank 6', 'Leader', '1 1 1 1 1', '2 2 2 2 2', '3 3 3 3 3', '4 4 4 4 4', '5 5 5 5 5', 0, 3, 0, 5963520),
(3, 'National Guard', 'Welcome to National Guard!', 1, 20, 156.843, 1903.28, 18.6444, 0, 0, 0, 'Rank 1', 'Rank 2', 'Rank 3', 'Rank 4', 'Rank 5', 'Rank 6', 'Leader', '1 1 1 1 1', '2 2 2 2 2', '3 3 3 3 3', '4 4 4 4 4', '5 5 5 5 5', 0, 5, 0, 27136),
(4, 'Los Aztecas', 'None', 1, 20, 1456.74, 2773.34, 10.8203, 1000, 0, 0, 'Rank 1', 'Rank 2', 'Rank 3', 'Rank 4', 'Rank 5', 'Rank 6', 'Leader', '1 1 1 1 1', '2 2 2 2 2', '3 3 3 3 3', '4 4 4 4 4', '5 5 5 5 5', 0, 999, 0, -1),
(5, 'Grove Street', 'salut suge mt', 1, 20, 2495.33, -1690.67, 14.7656, 0, 0, 0, 'ancepator', '', 'Rank 3', 'Rank 4', 'Rank 5', 'Rank 6', 'Leader', '1 1 1 1 1', '2 2 2 2 2', '3 3 3 3 3', '4 4 4 4 4', '5 5 5 5 5', 0, 5, 0, 204933632),
(6, 'Los Vagos', 'Welcome to Los Vagos!', 1, 20, 725.677, -1440.45, 13.5391, 0, 101, 0, 'Rank 1', 'Rank 2', 'Rank 3', 'Rank 4', 'Rank 5', 'Rank 6', 'Leader', '1 1 1 1 1', '2 2 2 2 2', '3 3 3 3 3', '4 4 4 4 4', '5 5 5 5 5', 0, 5, 0, -1786635264),
(7, 'Taxi Las Venturas', 'None', 1, 20, 938.196, 1733.21, 8.8516, 0, 0, 0, 'Rank 1', 'Rank 2', 'Rank 3', 'Rank 4', 'Rank 5', 'Rank 6', 'Leader', '1 1 1 1 1', '2 2 2 2 2', '3 3 3 3 3', '4 4 4 4 4', '5 5 5 5 5', 0, 999, 0, -65536),
(8, 'Las Venturas Police Department', 'None', 1, 20, 2293.28, 2432.26, 10.8203, 0, 0, 0, '*** LV Brigada Rutiera', '*** LV Agent', '*** LV Agent Principal', '*** LV Sub Inspector', '*** LV Inspector', '*** LV Chestor Adjunct', 'LV Chestor', '1 1 1 1 1', '2 2 2 2 2', '3 3 3 3 3', '4 4 4 4 4', '5 5 5 5 5', 0, 1, 0, 4837376),
(9, 'News Reporters', 'Welcome to News Reporters!', 1, 20, -329.69, 1537.04, 76.6117, 0, 0, 0, 'Rank 1', 'Rank 2', 'Rank 3', 'Rank 4', 'Rank 5', 'Rank 6', 'Leader', '1 1 1 1 1', '2 2 2 2 2', '3 3 3 3 3', '4 4 4 4 4', '5 5 5 5 5', 0, 3, 0, -272171264),
(10, 'Ballas', 'None', 1, 20, 1455.27, 750.868, 11.0234, 0, 0, 0, 'Rank 1', 'Rank 2', 'Rank 3', 'Rank 4', 'Rank 5', 'Rank 6', 'Leader', '1 1 1 1 1', '2 2 2 2 2', '3 3 3 3 3', '4 4 4 4 4', '5 5 5 5 5', 0, 999, 0, 2098883584),
(11, 'Hitman Agency', 'Welcome to Hitman Agency!', 1, 20, 1081.18, -345.398, 73.9825, 0, 0, 0, 'Rank 1', 'Rank 2', 'Rank 3', 'Rank 4', 'Rank 5', 'Rank 6', 'Leader', '1 1 1 1 1', '2 2 2 2 2', '3 3 3 3 3', '4 4 4 4 4', '5 5 5 5 5', 0, 4, 0, 2117678848),
(12, 'School Instructors', 'Welcome to School Instructors!', 1, 20, 2435.31, 1671.01, 10.8203, 0, 0, 0, 'Rank 1', 'Rank 2', 'Rank 3', 'Rank 4', 'Rank 5', 'Rank 6', 'Leader', '1 1 1 1 1', '2 2 2 2 2', '3 3 3 3 3', '4 4 4 4 4', '5 5 5 5 5', 0, 1, 0, 13728000),
(13, 'Taxi Los Santos', 'Welcome to Taxi Los Santos!', 1, 20, 1753.2, -1903.28, 13.5633, 0, 0, 0, 'Rank 1', 'Rank 2', 'Rank 3', 'Rank 4', 'Rank 5', 'Rank 6', 'Leader', '1 1 1 1 1', '2 2 2 2 2', '3 3 3 3 3', '4 4 4 4 4', '5 5 5 5 5', 0, 3, 0, -65536),
(14, 'Paramedic Department', 'Welcome to Paramedic Department!', 1, 20, 1615.06, 1815.72, 10.8203, 0, 0, 0, 'Rank 1', 'Rank 2', 'Rank 3', 'Rank 4', 'Rank 5', 'Rank 6', 'Leader', '1 1 1 1 1', '2 2 2 2 2', '3 3 3 3 3', '4 4 4 4 4', '5 5 5 5 5', 0, 3, 0, -127645696),
(15, 'Red Dragon Triads', 'None', 1, 20, 2597.68, 1900.39, 11.0312, 0, 0, 0, 'Rank 1', 'Rank 2', 'Rank 3', 'Rank 4', 'Rank 5', 'Rank 6', 'Leader', '1 1 1 1 1', '2 2 2 2 2', '3 3 3 3 3', '4 4 4 4 4', '5 5 5 5 5', 0, 999, 0, -1),
(16, 'The Corleone Family', 'None', 1, 20, 1546.84, -1269.89, 17.4063, 0, 0, 0, 'Rank 1', 'Rank 2', 'Rank 3', 'Rank 4', 'Rank 5', 'Rank 6', 'Leader', '1 1 1 1 1', '2 2 2 2 2', '3 3 3 3 3', '4 4 4 4 4', '5 5 5 5 5', 0, 999, 0, -1);

-- --------------------------------------------------------

--
-- Table structure for table `faction_history`
--

CREATE TABLE `faction_history` (
  `id` int(11) NOT NULL,
  `player` int(11) DEFAULT NULL,
  `actionby` int(11) DEFAULT NULL,
  `message` varchar(400) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `faction_history`
--

INSERT INTO `faction_history` (`id`, `player`, `actionby`, `message`) VALUES
(1, 1, 1, 'ionut was uninvited by Admin ionut from faction Los Santos Police Department (rank 2) after 0 days, without FP. Reason: 0 0.'),
(2, 1, 1, 'ionut is now the leader of faction Los Santos Police Department (promoted by ionut)'),
(3, 1, 2, 'ionut was uninvited by Admin ionut2 from faction Los Santos Police Department (rank 7) after 3 days, without FP. Reason: 0 0 0.'),
(4, 3, 3, 'ioneltalent is now the leader of faction Los Santos Police Department (promoted by ioneltalent)'),
(5, 3, 3, 'ioneltalent was uninvited by Admin ioneltalent from faction Los Santos Police Department (rank 7) after 0 days, without FP. Reason: 0 0 .'),
(6, 3, 3, 'ioneltalent is now the leader of faction Los Santos Police Department (promoted by ioneltalent)'),
(7, 3, 3, 'ioneltalent was uninvited by Admin ioneltalent from faction Los Santos Police Department (rank 7) after 0 days, without FP. Reason: 0 0.'),
(8, 3, 3, 'ioneltalent is now the leader of faction Los Santos Police Department (promoted by ioneltalent)'),
(9, 3, 3, 'ioneltalent was uninvited by Admin ioneltalent from faction Los Santos Police Department (rank 7) after 0 days, without FP. Reason: 0 0.'),
(10, 3, 3, 'ioneltalent is now the leader of faction Los Santos Police Department (promoted by ioneltalent)'),
(11, 3, 3, 'ioneltalent was uninvited by Admin ioneltalent from faction Los Santos Police Department (rank 7) after 0 days, without FP. Reason: 0 0 .'),
(12, 5, 5, 'Ramos is now the leader of faction Los Santos Police Department (promoted by Ramos)'),
(13, 5, 5, 'Ramos was uninvited by Admin Ramos from faction Los Santos Police Department (rank 7) after 0 days, without FP. Reason: 0.');

-- --------------------------------------------------------

--
-- Table structure for table `gangzones`
--

CREATE TABLE `gangzones` (
  `ID` int(11) NOT NULL,
  `Owned` int(11) DEFAULT NULL,
  `minX` float DEFAULT NULL,
  `minY` float DEFAULT NULL,
  `maxX` float DEFAULT NULL,
  `maxY` float DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Dumping data for table `gangzones`
--

INSERT INTO `gangzones` (`ID`, `Owned`, `minX`, `minY`, `maxX`, `maxY`) VALUES
(1, 6, 574, -1547.5, 869, -1345.5),
(2, 6, 263, -1558.5, 574, -1345.5),
(3, 6, 263, -1757.5, 575, -1558.5),
(4, 6, 574, -1762.5, 864, -1547.5),
(5, 6, 188.667, -1348.83, 506.667, -1125.83),
(6, 6, 506.667, -1347.83, 869.667, -1125.83),
(7, 5, 2150, -1817.17, 2626, -1545.17),
(8, 5, 2626, -1647.17, 2839, -1333.17),
(9, 5, 1846, -1287.17, 2090, -1082.17),
(10, 5, 2069, -1545.17, 2626, -1369.17),
(11, 5, 2148, -1369.17, 2469, -1059.17),
(12, 5, 1672.25, -1955.23, 1981.25, -1790.23);

-- --------------------------------------------------------

--
-- Table structure for table `gang_turfs`
--

CREATE TABLE `gang_turfs` (
  `ID` int(11) NOT NULL,
  `minX` float NOT NULL DEFAULT '0',
  `minY` float NOT NULL DEFAULT '0',
  `maxX` float NOT NULL DEFAULT '0',
  `maxY` float NOT NULL DEFAULT '0',
  `FactionID` int(11) NOT NULL DEFAULT '0'
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `gang_turfs`
--

INSERT INTO `gang_turfs` (`ID`, `minX`, `minY`, `maxX`, `maxY`, `FactionID`) VALUES
(1, 2162, -1784.5, 2665, -1568.5, 5),
(2, 516, -1540.5, 1066, -1359.5, 5);

-- --------------------------------------------------------

--
-- Table structure for table `garages`
--

CREATE TABLE `garages` (
  `ID` int(11) NOT NULL,
  `House` int(11) DEFAULT NULL,
  `Description` varchar(30) NOT NULL DEFAULT 'Time2Play Garage',
  `Locked` int(11) DEFAULT NULL,
  `garageX` float DEFAULT NULL,
  `garageY` float DEFAULT NULL,
  `garageZ` float DEFAULT NULL,
  `garageA` float DEFAULT NULL,
  `Mechanic` int(11) NOT NULL DEFAULT '0',
  `GasStation` int(11) NOT NULL DEFAULT '0'
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- --------------------------------------------------------

--
-- Table structure for table `housemapping`
--

CREATE TABLE `housemapping` (
  `ID` int(11) NOT NULL,
  `Model` int(11) DEFAULT NULL,
  `House` int(11) DEFAULT NULL,
  `Type` int(11) DEFAULT NULL,
  `X` float DEFAULT NULL,
  `Y` float DEFAULT NULL,
  `Z` float DEFAULT NULL,
  `RotX` float DEFAULT NULL,
  `RotY` float DEFAULT NULL,
  `RotZ` float DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `houses`
--

CREATE TABLE `houses` (
  `ID` int(11) NOT NULL,
  `Type` int(11) DEFAULT NULL,
  `OwnerSQL` int(11) DEFAULT NULL,
  `Description` varchar(24) NOT NULL DEFAULT 'Time2Play House',
  `Interior` int(11) DEFAULT NULL,
  `Locked` int(11) DEFAULT NULL,
  `ExteriorX` float DEFAULT NULL,
  `ExteriorY` float DEFAULT NULL,
  `ExteriorZ` float DEFAULT NULL,
  `InteriorX` float DEFAULT NULL,
  `InteriorY` float DEFAULT NULL,
  `InteriorZ` float DEFAULT NULL,
  `VIP` int(11) DEFAULT NULL,
  `Garage` int(11) DEFAULT NULL,
  `Rentable` int(11) NOT NULL DEFAULT '1',
  `Rent` int(11) NOT NULL DEFAULT '500',
  `Safe` int(11) DEFAULT NULL,
  `Health` int(11) DEFAULT NULL,
  `Armour` int(11) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Dumping data for table `houses`
--

INSERT INTO `houses` (`ID`, `Type`, `OwnerSQL`, `Description`, `Interior`, `Locked`, `ExteriorX`, `ExteriorY`, `ExteriorZ`, `InteriorX`, `InteriorY`, `InteriorZ`, `VIP`, `Garage`, `Rentable`, `Rent`, `Safe`, `Health`, `Armour`) VALUES
(1, 2, 1, 'Time4Play House', 15, 0, 1854.04, -1914.83, 15.257, 387.22, 1471.7, 1080.19, 1, NULL, 1, 500, NULL, NULL, NULL),
(2, 2, 0, 'Time4Play House', 6, 0, 1872.29, -1912.27, 15.257, -68.81, 1351.21, 1080.21, NULL, NULL, 1, 500, NULL, NULL, NULL),
(3, 2, 0, 'Time4Play House', 2, NULL, 1913.51, -1912.58, 15.257, 2237.59, -1081.64, 1049.02, NULL, NULL, 1, 500, NULL, NULL, NULL),
(4, 2, 0, 'Time4Play House', 2, NULL, 1938, -1911.35, 15.257, 2237.59, -1081.64, 1049.02, NULL, NULL, 1, 500, NULL, NULL, NULL),
(5, 2, 0, 'Time4Play House', 6, NULL, 1895.54, -2068.43, 15.669, -68.81, 1351.21, 1080.21, NULL, NULL, 1, 500, NULL, NULL, NULL),
(6, 2, 0, 'Time4Play House', 2, NULL, 1873.49, -2070.2, 15.497, 2237.59, -1081.64, 1049.02, NULL, NULL, 1, 500, NULL, NULL, NULL),
(7, 2, 0, 'Time4Play House', 6, NULL, 1851.75, -2069.91, 15.481, -68.81, 1351.21, 1080.21, NULL, NULL, 1, 500, NULL, NULL, NULL),
(8, 1, 0, 'Time4Play House', 8, NULL, 1804.11, -2123.96, 13.942, 2807.48, -1174.76, 1025.57, NULL, NULL, 1, 500, NULL, NULL, NULL),
(9, 2, 0, 'Time4Play House', 2, NULL, 1782.1, -2125.54, 14.068, 2237.59, -1081.64, 1049.02, NULL, NULL, 1, 500, NULL, NULL, NULL),
(10, 2, 0, 'Time4Play House', 12, NULL, 1761.17, -2124.65, 14.057, 446.772, 506.603, 1001.42, NULL, NULL, 1, 500, NULL, NULL, NULL),
(11, 2, 0, 'Time4Play House', 2, NULL, 1734.66, -2129.8, 14.021, 2237.59, -1081.64, 1049.02, NULL, NULL, 1, 500, NULL, NULL, NULL),
(12, 2, 0, 'Time4Play House', 12, NULL, 1715.1, -2124.7, 14.057, 446.772, 506.603, 1001.42, NULL, NULL, 1, 500, NULL, NULL, NULL),
(13, 2, 0, 'Time4Play House', 2, NULL, 1695.44, -2125.28, 13.81, 2237.59, -1081.64, 1049.02, NULL, NULL, 1, 500, NULL, NULL, NULL),
(14, 1, 0, 'Time4Play House', 2, NULL, 1673.91, -2122.12, 14.146, 446.99, 1397.07, 1084.3, NULL, NULL, 1, 500, 5000, NULL, NULL),
(15, 2, 0, 'Time4Play House', 2, NULL, 2437.71, -2020.4, 13.903, 2237.59, -1081.64, 1049.02, NULL, NULL, 1, 500, NULL, NULL, NULL),
(16, 2, 0, 'Time4Play House', 2, NULL, 2465.21, -2020.16, 14.124, 2237.59, -1081.64, 1049.02, NULL, NULL, 1, 500, NULL, NULL, NULL),
(17, 2, 0, 'Time4Play House', 2, NULL, 2486.32, -2021.47, 13.999, 2237.59, -1081.64, 1049.02, NULL, NULL, 1, 500, NULL, NULL, NULL),
(18, 2, 0, 'Time4Play House', 6, NULL, 2507.78, -2020.7, 14.21, -68.81, 1351.21, 1080.21, NULL, NULL, 1, 500, NULL, NULL, NULL),
(19, 2, 0, 'Time4Play House', 15, NULL, 2522.16, -2018.59, 14.074, 387.22, 1471.7, 1080.19, NULL, NULL, 1, 500, NULL, NULL, NULL),
(20, 2, 0, 'Time4Play House', 2, NULL, 2524.38, -1998.34, 14.113, 2237.59, -1081.64, 1049.02, NULL, NULL, 1, 500, NULL, NULL, NULL),
(21, 1, 0, 'Time4Play House', 2, NULL, 2508.17, -1999.02, 13.903, 446.99, 1397.07, 1084.3, NULL, NULL, 1, 500, NULL, NULL, NULL),
(22, 2, 0, 'Time4Play House', 6, NULL, 2483.43, -1995.7, 13.834, -68.81, 1351.21, 1080.21, NULL, NULL, 1, 500, NULL, NULL, NULL),
(23, 2, 0, 'Time4Play House', 6, NULL, 2465.13, -1996.08, 13.689, -68.81, 1351.21, 1080.21, NULL, NULL, 1, 500, NULL, NULL, NULL),
(24, 2, 0, 'Time4Play House', 2, NULL, 2695.36, -2020.48, 14.022, 2237.59, -1081.64, 1049.02, NULL, NULL, 1, 500, NULL, NULL, NULL),
(25, 2, 0, 'Time4Play House', 12, NULL, 2673.12, -2020.1, 14.168, 446.772, 506.603, 1001.42, NULL, NULL, 1, 500, NULL, NULL, NULL),
(26, 2, 0, 'Time4Play House', 12, NULL, 2635.57, -2012.87, 14.144, 446.772, 506.603, 1001.42, NULL, NULL, 1, 500, NULL, NULL, NULL),
(27, 2, 0, 'Time4Play House', 12, NULL, 2637.13, -1991.65, 14.324, 446.772, 506.603, 1001.42, NULL, NULL, 1, 500, NULL, NULL, NULL),
(28, 2, 0, 'Time4Play House', 2, NULL, 2652.78, -1989.7, 13.999, 2237.59, -1081.64, 1049.02, NULL, NULL, 1, 500, NULL, NULL, NULL),
(29, 2, 0, 'Time4Play House', 15, NULL, 2672.87, -1989.72, 13.994, 387.22, 1471.7, 1080.19, NULL, NULL, 1, 500, NULL, NULL, NULL),
(30, 2, 0, 'Time4Play House', 6, NULL, 2696.35, -1990.36, 14.223, -68.81, 1351.21, 1080.21, NULL, NULL, 1, 500, NULL, NULL, NULL),
(31, 1, 0, 'Time4Play House', 2, NULL, 2327.15, -1681.69, 14.93, 446.99, 1397.07, 1084.3, NULL, NULL, 1, 500, NULL, NULL, NULL),
(32, 2, 0, 'Time4Play House', 12, NULL, 2326.94, -1716.7, 14.238, 446.772, 506.603, 1001.42, NULL, NULL, 1, 500, NULL, NULL, NULL),
(33, 0, 0, 'Time4Play House', 5, NULL, 797.899, -1729.08, 13.547, 140.17, 1366.07, 1083.65, NULL, NULL, 1, 500, NULL, NULL, NULL),
(34, 1, 0, 'Time4Play House', 8, NULL, 791.661, -1753.73, 13.448, 2807.48, -1174.76, 1025.57, NULL, NULL, 1, 500, NULL, NULL, NULL),
(35, 2, 0, 'Time4Play House', 15, NULL, 769.228, -1745.88, 13.077, 387.22, 1471.7, 1080.19, NULL, NULL, 1, 500, 1000, NULL, NULL),
(36, 2, 0, 'Time4Play House', 6, NULL, 769.228, -1696.57, 5.155, -68.81, 1351.21, 1080.21, NULL, NULL, 1, 500, NULL, NULL, NULL),
(37, 2, 0, 'Time4Play House', 2, NULL, 768.08, -1655.91, 5.609, 2237.59, -1081.64, 1049.02, NULL, NULL, 1, 500, NULL, NULL, NULL),
(38, 0, 0, 'Time4Play House', 3, NULL, 761.779, -1564.1, 13.573, -2636.68, 1402.55, 906.46, NULL, NULL, 1, 500, NULL, NULL, NULL),
(39, 1, 0, 'Time4Play House', 15, NULL, 849.972, -1520.06, 14.352, 377.15, 1417.41, 1081.33, NULL, NULL, 1, 500, NULL, NULL, NULL),
(40, 2, 0, 'Time4Play House', 12, NULL, 828.298, -858.497, 70.331, 446.772, 506.603, 1001.42, NULL, NULL, 1, 500, NULL, NULL, NULL),
(41, 0, 0, 'Time4Play House', 3, NULL, 836.485, -893.673, 68.769, -2636.68, 1402.55, 906.46, NULL, NULL, 1, 500, NULL, NULL, NULL),
(42, 2, 0, 'Time4Play House', 6, NULL, 924.439, -852.936, 93.457, -68.81, 1351.21, 1080.21, NULL, NULL, 1, 500, NULL, NULL, NULL),
(43, 1, 0, 'Time4Play House', 8, NULL, 937.139, -848.025, 93.711, 2807.48, -1174.76, 1025.57, NULL, NULL, 1, 500, NULL, NULL, NULL),
(44, 0, 0, 'Time4Play House', 3, NULL, 989.689, -828.716, 95.469, -2636.68, 1402.55, 906.46, NULL, NULL, 1, 500, NULL, NULL, NULL),
(45, 1, 0, 'Time4Play House', 2, NULL, 1034.58, -812.666, 101.852, 446.99, 1397.07, 1084.3, NULL, NULL, 1, 500, 1000, NULL, NULL),
(46, 0, 0, 'Time4Play House', 5, NULL, 1093.83, -807.093, 107.42, 140.17, 1366.07, 1083.65, NULL, NULL, 1, 500, NULL, NULL, NULL),
(47, 0, 1, 'Time4Play House', 7, NULL, 1298.61, -798.718, 84.141, 225.68, 1021.45, 1084.02, 1, 1, 1, 500, NULL, NULL, NULL),
(48, 0, 0, 'Time4Play House', 3, NULL, 1496.97, -687.892, 95.563, -2636.68, 1402.55, 906.46, NULL, NULL, 1, 500, NULL, NULL, NULL),
(49, 0, 0, 'Time4Play House', 7, NULL, 1331.57, -632.501, 109.135, 225.68, 1021.45, 1084.02, NULL, NULL, 1, 500, 5000, NULL, NULL),
(50, 1, 0, 'Time4Play House', 8, NULL, 1094.92, -647.302, 113.648, 2807.48, -1174.76, 1025.57, NULL, NULL, 1, 500, NULL, NULL, NULL),
(51, 1, 0, 'Time4Play House', 8, NULL, 1045.31, -642.513, 120.117, 2807.48, -1174.76, 1025.57, NULL, NULL, 1, 500, NULL, NULL, NULL),
(52, 0, 0, 'Time4Play House', 3, NULL, 980.287, -677.355, 121.976, -2636.68, 1402.55, 906.46, NULL, NULL, 1, 500, NULL, NULL, NULL),
(53, 1, 0, 'Time4Play House', 2, NULL, 946.108, -710.356, 122.62, 446.99, 1397.07, 1084.3, NULL, NULL, 1, 500, NULL, NULL, NULL),
(54, 1, 0, 'Time4Play House', 8, NULL, 898.306, -677.401, 116.89, 2807.48, -1174.76, 1025.57, NULL, NULL, 1, 500, NULL, NULL, NULL),
(55, 1, 0, 'Time4Play House', 15, NULL, 867.831, -716.981, 105.68, 377.15, 1417.41, 1081.33, NULL, NULL, 1, 500, NULL, NULL, NULL),
(56, 1, 0, 'Time4Play House', 15, NULL, 891.011, -783.208, 101.313, 377.15, 1417.41, 1081.33, NULL, NULL, 1, 500, NULL, NULL, NULL),
(57, 1, 0, 'Time4Play House', 15, NULL, 848.054, -745.58, 94.969, 377.15, 1417.41, 1081.33, NULL, NULL, 1, 500, NULL, NULL, NULL),
(58, 1, 0, 'Time4Play House', 8, NULL, 808.805, -758.974, 76.531, 2807.48, -1174.76, 1025.57, NULL, NULL, 1, 500, NULL, NULL, NULL),
(59, 1, 0, 'Time4Play House', 2, NULL, 785.813, -828.022, 70.29, 446.99, 1397.07, 1084.3, NULL, NULL, 1, 500, NULL, NULL, NULL),
(60, 1, 0, 'Time4Play House', 8, NULL, 645.9, -1117.51, 44.207, 2807.48, -1174.76, 1025.57, NULL, NULL, 1, 500, NULL, NULL, NULL);

-- --------------------------------------------------------

--
-- Table structure for table `kenny_applications`
--

CREATE TABLE `kenny_applications` (
  `ID` int(11) NOT NULL,
  `UserID` int(11) NOT NULL,
  `UserName` varchar(90) NOT NULL,
  `Type` int(11) NOT NULL,
  `Answers` text NOT NULL,
  `Questions` text NOT NULL,
  `Status` int(11) NOT NULL,
  `Date` varchar(90) NOT NULL,
  `ActionBy` varchar(90) NOT NULL,
  `Reason` text NOT NULL,
  `FactionID` int(11) NOT NULL DEFAULT '0',
  `AnswerDate` varchar(90) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- --------------------------------------------------------

--
-- Table structure for table `kenny_questions`
--

CREATE TABLE `kenny_questions` (
  `ID` int(11) NOT NULL,
  `Question` text NOT NULL,
  `Type` int(11) NOT NULL,
  `Date` varchar(90) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Dumping data for table `kenny_questions`
--

INSERT INTO `kenny_questions` (`ID`, `Question`, `Type`, `Date`) VALUES
(1, 'Nick pe server: ', 1, ''),
(2, 'Zile adunate (aprox.) in factiuni:', 1, ''),
(3, 'Ce iti place sa faci pe SAMP?', 1, ''),
(4, 'Cate ore acorzi pe zi acestui joc? (fii sincer, se verifica):', 1, ''),
(5, 'De ce crezi ca meriti functia de helper?:', 1, ''),
(6, 'Ai fost banat vreodata pe Time2Play? Daca da, specifica toate banurile:', 1, ''),
(7, 'Ce alte jocuri joci?:', 1, ''),
(8, 'Ce-ti place cel mai mult la Time2Play, de ce ai ales sa joci aici?:', 1, ''),
(9, 'Cu cine iti petreci timpul de obicei pe server? (enumera doar cativa jucatori):', 1, ''),
(10, 'Daca un player te intreaba cum poate avea alt skin, ce ii raspunzi?:', 1, ''),
(11, 'Daca un player intreaba cum poate intra in factiunea L.S.P.D., ce ii raspunzi:', 1, ''),
(12, 'Nickname pe server:', 2, ''),
(13, 'Ore jucate:', 2, ''),
(14, 'De cat timp joci pe server:', 2, ''),
(15, 'Varsta reala:', 2, ''),
(16, 'Ce alte jocuri mai joc?:', 2, ''),
(17, 'Ce vei face dupa terminarea liceului?:', 2, ''),
(18, 'Cea mai mare realizare pe t2p:', 2, ''),
(19, 'Ce ai de gand sa faci diferit fata de alti lideri:', 2, ''),
(20, 'Ai citit regulamentul liderilor?:', 2, ''),
(21, 'Cat ai stat cel mai mult intr-o factiune? (specifica factiunea, rank-ul si timpul petrecut):', 2, ''),
(22, 'Cat ai stat cel mai putin intr-o factiune? De ce ai parasit factiunea?:', 2, ''),
(23, 'Ce te deranjeaza cel mai mult la t2p?:', 2, ''),
(24, 'De ce crezi ca te vei descurca ca lider?:', 2, ''),
(25, 'Cat ai stat cel mai mult intr-o mafie? Ce parere ai despre mafii?:', 2, ''),
(26, 'Cat ai stat cel mai mult intr-un departament? Ce parere ai despre departamente?:', 2, ''),
(27, 'Cat ai stat cel mai mult intr-o factiune pasnica? Ce parere ai despre factiunile pasnice?:', 2, '');

-- --------------------------------------------------------

--
-- Table structure for table `kenny_recovery`
--

CREATE TABLE `kenny_recovery` (
  `ID` int(11) NOT NULL,
  `UserID` int(11) NOT NULL,
  `UserName` varchar(25) NOT NULL,
  `Email` varchar(60) NOT NULL,
  `SecretCode` varchar(60) NOT NULL,
  `RecoveryTime` int(11) NOT NULL,
  `Type` int(2) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- --------------------------------------------------------

--
-- Table structure for table `kenny_suspend`
--

CREATE TABLE `kenny_suspend` (
  `ID` int(11) NOT NULL,
  `UserName` varchar(25) NOT NULL,
  `UserID` int(11) NOT NULL,
  `AdminName` varchar(25) NOT NULL,
  `AdminID` int(11) NOT NULL,
  `Days` int(4) NOT NULL,
  `Reason` varchar(90) NOT NULL,
  `SuspendDate` varchar(11) NOT NULL,
  `ExpireDate` varchar(11) NOT NULL,
  `Permanent` tinyint(1) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Table structure for table `personalvehicles`
--

CREATE TABLE `personalvehicles` (
  `ID` int(11) NOT NULL,
  `Model` int(11) DEFAULT NULL,
  `Owner` varchar(24) DEFAULT NULL,
  `posX` float DEFAULT NULL,
  `posY` float DEFAULT NULL,
  `posZ` float DEFAULT NULL,
  `posA` float DEFAULT NULL,
  `Days` int(11) DEFAULT NULL,
  `KM` float DEFAULT NULL,
  `Fuel` int(11) NOT NULL DEFAULT '100',
  `Plate` varchar(16) NOT NULL DEFAULT 'NewCar',
  `Insurance` int(11) NOT NULL DEFAULT '10',
  `InsurancePrice` int(11) NOT NULL DEFAULT '5000',
  `Color1` int(11) DEFAULT '1',
  `Color2` int(11) DEFAULT '1',
  `Special` int(11) DEFAULT NULL,
  `VIP` int(11) DEFAULT NULL,
  `VIPText` varchar(16) NOT NULL DEFAULT 'vip veh',
  `VIPColor` varchar(6) NOT NULL DEFAULT 'ffffff',
  `VIPPos` varchar(64) NOT NULL DEFAULT '0 0 0 0 0 0',
  `paintJob` int(11) NOT NULL DEFAULT '-1',
  `Tunning` varchar(100) NOT NULL DEFAULT '0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0',
  `VirtualWorld` int(11) DEFAULT NULL,
  `Money` int(11) DEFAULT NULL,
  `Materials` int(11) DEFAULT NULL,
  `Drugs` int(11) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Dumping data for table `personalvehicles`
--

INSERT INTO `personalvehicles` (`ID`, `Model`, `Owner`, `posX`, `posY`, `posZ`, `posA`, `Days`, `KM`, `Fuel`, `Plate`, `Insurance`, `InsurancePrice`, `Color1`, `Color2`, `Special`, `VIP`, `VIPText`, `VIPColor`, `VIPPos`, `paintJob`, `Tunning`, `VirtualWorld`, `Money`, `Materials`, `Drugs`) VALUES
(1, 415, '237ALKSNDiskhd', -1633.98, 1298.53, 6.75, 133.3, 1720370555, 4, 96, 'NewCar', 10, 5000, 128, 1, NULL, 0, 'vip veh', 'ffffff', '0 0 0 0 0 0', -1, '0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0', NULL, NULL, NULL, NULL),
(2, 509, 'vividsec2', -1632.78, 1295.83, 6.75, 133.3, 1720697639, 0, 100, 'NewCar', 10, 5000, 1, 1, NULL, 0, 'vip veh', 'ffffff', '0 0 0 0 0 0', -1, '0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0', NULL, NULL, NULL, NULL),
(3, 509, '237ALKSNDiskhd', -1630.38, 1290.43, 6.75, 133.3, 1720698919, 0, 100, 'NewCar', 10, 5000, 1, 1, NULL, 0, 'vip veh', 'ffffff', '0 0 0 0 0 0', -1, '0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0', NULL, NULL, NULL, NULL),
(4, 509, '237ALKSNDiskhd', -1636.38, 1303.93, 6.75, 133.3, 1720701536, 0, 100, 'NewCar', 10, 5000, 1, 1, NULL, 0, 'vip veh', 'ffffff', '0 0 0 0 0 0', -1, '0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0', NULL, NULL, NULL, NULL),
(5, 509, '', -1630.38, 1290.43, 6.75, 133.3, 1720701636, 0, 100, 'NewCar', 10, 5000, 1, 1, NULL, 0, 'vip veh', 'ffffff', '0 0 0 0 0 0', -1, '0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0', NULL, NULL, NULL, NULL),
(6, 509, '', -1633.98, 1298.53, 6.75, 133.3, 1721387505, 0, 100, 'NewCar', 10, 5000, 1, 1, NULL, 0, 'vip veh', 'ffffff', '0 0 0 0 0 0', -1, '0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0', NULL, NULL, NULL, NULL),
(7, 509, 'ionut', -1631.58, 1293.13, 6.75, 133.3, 1721419528, 0, 100, 'NewCar', 10, 5000, 1, 1, NULL, 0, 'vip veh', 'ffffff', '0 0 0 0 0 0', -1, '0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0', NULL, NULL, NULL, NULL),
(8, 424, 'ionut', -1638.78, 1309.33, 6.75, 133.3, 1721682762, 0, 100, 'NewCar', 10, 5000, 1, 1, NULL, 0, 'vip veh', 'ffffff', '0 0 0 0 0 0', -1, '0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0', NULL, NULL, NULL, NULL),
(9, 494, '237ALKSNDiskhd', -1631.58, 1293.13, 6.75, 133.3, 1721856174, 1, 99, 'NewCar', 10, 5000, 1, 1, NULL, 0, 'vip veh', 'ffffff', '0 0 0 0 0 0', -1, '0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0', NULL, NULL, NULL, NULL),
(10, 424, 'Ramos', -1632.78, 1295.83, 6.75, 133.3, 1722799265, NULL, 100, 'NewCar', 10, 5000, 1, 1, NULL, 0, 'vip veh', 'ffffff', '0 0 0 0 0 0', -1, '0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0', NULL, NULL, NULL, NULL);

-- --------------------------------------------------------

--
-- Table structure for table `player_logs`
--

CREATE TABLE `player_logs` (
  `id` int(11) NOT NULL,
  `userID` int(11) DEFAULT NULL,
  `userName` varchar(24) NOT NULL DEFAULT 'unknown',
  `giverName` varchar(24) NOT NULL DEFAULT '-',
  `logText` varchar(1024) DEFAULT NULL,
  `logTime` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `player_logs`
--

INSERT INTO `player_logs` (`id`, `userID`, `userName`, `giverName`, `logText`, `logTime`) VALUES
(1, 1, '', '-', 'ionut logged in, money $0 / $30,000, 0 pp [0/2 vehicles] ', '2024-07-19 00:33:40'),
(2, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-19 00:34:04'),
(3, 1, '', '-', 'ionut logged in, money $20.000 / $30,000, 0 pp [0/2 vehicles] ', '2024-07-19 00:34:18'),
(4, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-19 00:46:23'),
(5, 1, '4A09440E55DAF0DCEC599449', '-', 'ionut logged in, money $20.000 / $30,000, 0 pp [0/2 vehicles] ', '2024-07-19 00:46:51'),
(6, 1, '4A09440E55DAF0DCEC599449', '-', 'ionut logged in, money $20.000 / $30,000, 0 pp [0/2 vehicles] ', '2024-07-19 00:47:24'),
(7, 1, '4A09440E55DAF0DCEC599449', '-', 'ionut logged in, money $20.000 / $30,000, 0 pp [0/2 vehicles] ', '2024-07-19 00:49:21'),
(8, 1, '4A09440E55DAF0DCEC599449', '-', 'ionut logged in, money $20.000 / $30,000, 0 pp [0/2 vehicles] ', '2024-07-19 00:49:39'),
(9, 1, '4A09440E55DAF0DCEC599449', '-', 'ionut logged in, money $20.000 / $30,000, 0 pp [0/2 vehicles] ', '2024-07-19 00:50:09'),
(10, 1, '', '-', 'ionut logged in, money $20.000 / $30,000, 0 pp [0/2 vehicles] ', '2024-07-19 00:51:42'),
(11, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-19 00:52:43'),
(12, 1, '', '-', 'ionut logged in, money $20.000 / $30,000, 0 pp [0/2 vehicles] ', '2024-07-19 00:54:29'),
(13, 1, '', '-', 'ionut logged in, money $20.000 / $30,000, 0 pp [0/2 vehicles] ', '2024-07-19 00:56:00'),
(14, 1, '', '-', 'ionut logged in, money $20.000 / $30,000, 0 pp [0/2 vehicles] ', '2024-07-19 00:57:57'),
(15, 1, '', '-', 'ionut logged in, money $20.000 / $30,000, 0 pp [0/2 vehicles] ', '2024-07-19 01:00:23'),
(16, 1, '', '-', 'ionut logged in, money $20.000 / $30,000, 0 pp [0/2 vehicles] ', '2024-07-19 01:03:28'),
(17, 1, '', '-', 'ionut logged in, money $20.000 / $30,000, 0 pp [0/2 vehicles] ', '2024-07-19 01:05:52'),
(18, 1, '', '-', 'ionut logged in, money $20.000 / $30,000, 0 pp [0/2 vehicles] ', '2024-07-19 01:07:52'),
(19, 1, '', '-', 'ionut logged in, money $20.000 / $30,000, 0 pp [0/2 vehicles] ', '2024-07-19 01:09:48'),
(20, 1, '', '-', 'ionut logged in, money $20.000 / $30,000, 0 pp [0/2 vehicles] ', '2024-07-19 01:11:30'),
(21, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-19 01:11:34'),
(22, 1, '', '-', 'ionut logged in, money $20.000 / $30,000, 0 pp [0/2 vehicles] ', '2024-07-19 01:13:14'),
(23, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-19 01:13:18'),
(24, 1, '', '-', 'ionut logged in, money $20.000 / $30,000, 0 pp [0/2 vehicles] ', '2024-07-19 01:13:38'),
(25, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-19 01:14:32'),
(26, 1, '', '-', 'ionut logged in, money $20.000 / $30,000, 0 pp [0/2 vehicles] ', '2024-07-19 11:11:08'),
(27, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-19 11:16:46'),
(28, 1, '', '-', 'ionut logged in, money $191.000 / $999,499,999, 0 pp [0/2 vehicles] ', '2024-07-19 20:04:58'),
(29, 0, '', '-', ' logged out, money $0 / $0, 0 pp [1/0 vehicles] [car: 7] ', '2024-07-19 20:19:26'),
(30, 1, '', '-', 'ionut logged in, money $1.999.999 / $998,999,999, 0 pp [1/2 vehicles] [car: 7] ', '2024-07-20 11:11:12'),
(31, 0, '', '-', ' logged out, money $0 / $0, 0 pp [1/0 vehicles] [car: 7] ', '2024-07-20 11:11:31'),
(32, 1, '', '-', 'ionut logged in, money $1.999.999 / $998,999,999, 0 pp [1/2 vehicles] [car: 7] ', '2024-07-20 11:11:46'),
(33, 0, '', '-', ' logged out, money $0 / $0, 0 pp [1/0 vehicles] [car: 7] ', '2024-07-20 11:14:21'),
(34, 1, '', '-', 'ionut logged in, money $1.999.999 / $998,999,999, 0 pp [1/2 vehicles] [car: 7] ', '2024-07-20 18:14:49'),
(35, 2, '', '-', 'ionut2 logged in, money $0 / $30,000, 0 pp [0/2 vehicles] ', '2024-07-20 18:15:02'),
(36, 0, 'ionut', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-20 18:17:04'),
(37, 2, '', '-', 'ionut2 logged in, money $23.000 / $30,000, 0 pp [0/2 vehicles] ', '2024-07-20 18:17:14'),
(38, 0, 'ionut', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-20 18:17:16'),
(39, 2, '', '-', 'ionut2 logged in, money $23.000 / $30,000, 0 pp [0/2 vehicles] ', '2024-07-20 18:17:33'),
(40, 0, 'ionut', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-20 18:17:35'),
(41, 0, '', '-', ' logged out, money $0 / $0, 0 pp [1/0 vehicles] [car: 7] ', '2024-07-20 18:17:40'),
(42, 2, '', '-', 'ionut2 logged in, money $23.000 / $30,000, 0 pp [0/2 vehicles] ', '2024-07-20 18:24:52'),
(43, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-20 18:25:01'),
(44, 2, '', '-', 'ionut2 logged in, money $23.000 / $30,000, 0 pp [0/2 vehicles] ', '2024-07-20 18:27:02'),
(45, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-20 18:28:02'),
(46, 2, '', '-', 'ionut2 logged in, money $23.000 / $30,000, 0 pp [0/2 vehicles] ', '2024-07-20 18:28:37'),
(47, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-20 18:28:52'),
(48, 2, '', '-', 'ionut2 logged in, money $23.000 / $30,000, 0 pp [0/2 vehicles] ', '2024-07-20 18:30:03'),
(49, 2, '', '-', 'ionut2 logged in, money $23.000 / $30,000, 0 pp [0/2 vehicles] ', '2024-07-20 18:35:50'),
(50, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-20 18:35:54'),
(51, 2, '', '-', 'ionut2 logged in, money $23.000 / $30,000, 0 pp [0/2 vehicles] ', '2024-07-20 18:36:25'),
(52, 2, '', '-', 'ionut2 logged in, money $23.000 / $30,000, 0 pp [0/2 vehicles] ', '2024-07-20 18:37:15'),
(53, 1, 'ionut', '-', 'ionut logged in, money $1.999.999 / $998,999,999, 0 pp [1/2 vehicles] [car: 7] ', '2024-07-20 18:43:22'),
(54, 1, 'ionut', '-', 'ionut logged in, money $1.999.999 / $998,999,999, 0 pp [1/2 vehicles] [car: 7] ', '2024-07-20 18:43:42'),
(55, 2, '', '-', 'ionut2 logged in, money $23.000 / $30,000, 0 pp [0/2 vehicles] ', '2024-07-20 18:43:44'),
(56, 2, '', '-', 'ionut2 logged in, money $26.000 / $30,000, 0 pp [0/2 vehicles] ', '2024-07-20 18:56:18'),
(57, 1, 'ionut', '-', 'ionut logged in, money $1.999.999 / $998,999,999, 0 pp [1/2 vehicles] [car: 7] ', '2024-07-20 18:56:21'),
(58, 2, '', '-', 'ionut2 logged in, money $29.000 / $30,000, 0 pp [0/2 vehicles] ', '2024-07-20 18:57:43'),
(59, 1, 'ionut', '-', 'ionut logged in, money $1.999.999 / $998,999,999, 0 pp [1/2 vehicles] [car: 7] ', '2024-07-20 18:57:46'),
(60, 2, '', '-', 'ionut2 logged in, money $29.000 / $30,000, 0 pp [0/2 vehicles] ', '2024-07-20 20:04:38'),
(61, 1, 'ionut', '-', 'ionut logged in, money $3.399.999 / $998,999,999, 0 pp [1/2 vehicles] [car: 7] ', '2024-07-20 20:04:41'),
(62, 2, '', '-', 'ionut2 logged in, money $29.000 / $30,000, 0 pp [0/2 vehicles] ', '2024-07-20 20:07:51'),
(63, 1, 'ionut', '-', 'ionut logged in, money $3.399.999 / $998,999,999, 0 pp [1/2 vehicles] [car: 7] ', '2024-07-20 20:08:10'),
(64, 1, 'ionut', '-', 'ionut logged in, money $3.399.999 / $998,999,999, 0 pp [1/2 vehicles] [car: 7] ', '2024-07-20 20:08:55'),
(65, 2, '', '-', 'ionut2 logged in, money $29.000 / $30,000, 0 pp [0/2 vehicles] ', '2024-07-20 20:08:57'),
(66, 0, 'ionut2', '-', ' logged out, money $0 / $0, 0 pp [1/0 vehicles] [car: 7] ', '2024-07-20 20:10:06'),
(67, 2, '', '-', 'ionut2 logged in, money $29.000 / $30,000, 0 pp [0/2 vehicles] ', '2024-07-20 20:10:36'),
(68, 1, 'ionut', '-', 'ionut logged in, money $3.399.999 / $998,999,999, 0 pp [1/2 vehicles] [car: 7] ', '2024-07-20 20:10:52'),
(69, 2, '', '-', 'ionut2 logged in, money $32.000 / $30,000, 0 pp [0/2 vehicles] ', '2024-07-20 20:16:29'),
(70, 1, 'ionut', '-', 'ionut logged in, money $3.399.999 / $998,999,999, 0 pp [1/2 vehicles] [car: 7] ', '2024-07-20 20:16:32'),
(71, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-20 20:17:00'),
(72, 0, '', '-', ' logged out, money $0 / $0, 0 pp [1/0 vehicles] [car: 7] ', '2024-07-20 20:17:01'),
(73, 2, '', '-', 'ionut2 logged in, money $32.000 / $30,000, 0 pp [0/2 vehicles] ', '2024-07-20 20:28:28'),
(74, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-20 20:42:14'),
(75, 1, '', '-', 'ionut logged in, money $3.399.999 / $998,999,999, 0 pp [1/2 vehicles] [car: 7] ', '2024-07-20 21:40:38'),
(76, 0, '', '-', ' logged out, money $0 / $0, 0 pp [1/0 vehicles] [car: 7] ', '2024-07-20 21:41:00'),
(77, 1, '', '-', 'ionut logged in, money $3.399.999 / $998,999,999, 0 pp [1/2 vehicles] [car: 7] ', '2024-07-21 09:33:18'),
(78, 1, '', '-', 'ionut logged in, money $3.399.999 / $998,999,999, 0 pp [1/2 vehicles] [car: 7] ', '2024-07-21 09:46:54'),
(79, 0, '', '-', ' logged out, money $0 / $0, 0 pp [1/0 vehicles] [car: 7] ', '2024-07-21 09:46:55'),
(80, 1, '', '-', 'ionut logged in, money $3.399.999 / $998,999,999, 0 pp [1/2 vehicles] [car: 7] ', '2024-07-21 09:47:15'),
(81, 1, '', '-', 'ionut logged in, money $3.399.999 / $998,999,999, 0 pp [1/2 vehicles] [car: 7] ', '2024-07-21 09:48:13'),
(82, 0, '', '-', ' logged out, money $0 / $0, 0 pp [1/0 vehicles] [car: 7] ', '2024-07-21 09:49:46'),
(83, 1, '', '-', 'ionut logged in, money $3.399.999 / $998,999,999, 0 pp [1/2 vehicles] [car: 7] ', '2024-07-21 10:02:56'),
(84, 1, '', '-', 'ionut logged in, money $3.399.999 / $998,999,999, 0 pp [1/2 vehicles] [car: 7] ', '2024-07-21 10:03:58'),
(85, 0, '', '-', ' logged out, money $0 / $0, 0 pp [1/0 vehicles] [car: 7] ', '2024-07-21 10:10:39'),
(86, 1, '', '-', 'ionut logged in, money $3.399.999 / $998,999,999, 0 pp [1/2 vehicles] [car: 7] ', '2024-07-21 10:33:40'),
(87, 1, '', '-', 'ionut logged in, money $3.399.999 / $998,999,999, 0 pp [1/2 vehicles] [car: 7] ', '2024-07-21 10:35:32'),
(88, 1, '', '-', 'ionut logged in, money $3.399.999 / $998,999,999, 0 pp [1/2 vehicles] [car: 7] ', '2024-07-21 10:38:03'),
(89, 1, '', '-', 'ionut logged in, money $3.399.999 / $998,999,999, 0 pp [1/2 vehicles] [car: 7] ', '2024-07-21 10:39:34'),
(90, 0, '', '-', ' logged out, money $0 / $0, 0 pp [1/0 vehicles] [car: 7] ', '2024-07-21 10:39:52'),
(91, 1, '', '-', 'ionut logged in, money $3.399.999 / $998,999,999, 0 pp [1/2 vehicles] [car: 7] ', '2024-07-21 11:07:00'),
(92, 0, '', '-', ' logged out, money $0 / $0, 0 pp [1/0 vehicles] [car: 7] ', '2024-07-21 11:08:00'),
(93, 1, '', '-', 'ionut logged in, money $3.399.999 / $998,999,999, 0 pp [1/2 vehicles] [car: 7] ', '2024-07-21 11:08:50'),
(94, 1, '', '-', 'ionut logged in, money $3.399.999 / $998,999,999, 0 pp [1/2 vehicles] [car: 7] ', '2024-07-21 11:15:37'),
(95, 1, '', '-', 'ionut logged in, money $3.399.999 / $998,999,999, 0 pp [1/2 vehicles] [car: 7] ', '2024-07-21 11:17:23'),
(96, 1, '', '-', 'ionut logged in, money $3.399.999 / $998,999,999, 0 pp [1/2 vehicles] [car: 7] ', '2024-07-21 11:17:49'),
(97, 1, '', '-', 'ionut logged in, money $3.399.999 / $998,999,999, 0 pp [1/2 vehicles] [car: 7] ', '2024-07-21 11:19:34'),
(98, 1, '', '-', 'ionut logged in, money $3.399.999 / $998,999,999, 0 pp [1/2 vehicles] [car: 7] ', '2024-07-21 11:21:59'),
(99, 1, '', '-', 'ionut logged in, money $3.399.999 / $998,999,999, 0 pp [1/2 vehicles] [car: 7] ', '2024-07-21 11:23:41'),
(100, 1, '', '-', 'ionut logged in, money $3.399.999 / $998,999,999, 0 pp [1/2 vehicles] [car: 7] ', '2024-07-21 11:25:48'),
(101, 1, '', '-', 'ionut logged in, money $3.399.999 / $998,999,999, 0 pp [1/2 vehicles] [car: 7] ', '2024-07-21 11:28:11'),
(102, 1, '', '-', 'ionut logged in, money $3.399.999 / $998,999,999, 0 pp [1/2 vehicles] [car: 7] ', '2024-07-21 11:30:21'),
(103, 1, '', '-', 'ionut logged in, money $3.399.999 / $998,999,999, 0 pp [1/2 vehicles] [car: 7] ', '2024-07-21 11:30:56'),
(104, 1, '', '-', 'ionut logged in, money $3.399.999 / $998,999,999, 0 pp [1/2 vehicles] [car: 7] ', '2024-07-21 11:33:03'),
(105, 1, '', '-', 'ionut logged in, money $3.399.999 / $998,999,999, 0 pp [1/2 vehicles] [car: 7] ', '2024-07-21 11:41:30'),
(106, 1, '', '-', 'ionut logged in, money $3.399.999 / $998,999,999, 0 pp [1/2 vehicles] [car: 7] ', '2024-07-21 11:42:46'),
(107, 0, '', '-', ' logged out, money $0 / $0, 0 pp [1/0 vehicles] [car: 7] ', '2024-07-21 11:45:01'),
(108, 1, '', '-', 'ionut logged in, money $3.399.999 / $998,999,999, 0 pp [1/2 vehicles] [car: 7] ', '2024-07-21 11:45:38'),
(109, 1, '', '-', 'ionut logged in, money $3.399.999 / $998,999,999, 0 pp [1/2 vehicles] [car: 7] ', '2024-07-21 11:46:33'),
(110, 0, '', '-', ' logged out, money $0 / $0, 0 pp [1/0 vehicles] [car: 7] ', '2024-07-21 11:46:35'),
(111, 1, '', '-', 'ionut logged in, money $3.399.999 / $998,999,999, 0 pp [1/2 vehicles] [car: 7] ', '2024-07-21 11:46:44'),
(112, 0, '', '-', ' logged out, money $0 / $0, 0 pp [1/0 vehicles] [car: 7] ', '2024-07-21 11:46:56'),
(113, 1, '', '-', 'ionut logged in, money $3.399.999 / $998,999,999, 0 pp [1/2 vehicles] [car: 7] ', '2024-07-21 11:53:43'),
(114, 1, '', '-', 'ionut logged in, money $3.399.999 / $998,999,999, 0 pp [1/2 vehicles] [car: 7] ', '2024-07-21 11:59:53'),
(115, 1, '', '-', 'ionut logged in, money $4.099.999 / $998,999,999, 970 pp [1/2 vehicles] [car: 7] ', '2024-07-21 12:09:06'),
(116, 1, '', '-', 'ionut logged in, money $4.099.999 / $998,999,999, 970 pp [1/2 vehicles] [car: 7] ', '2024-07-21 12:17:51'),
(117, 1, '', '-', 'ionut logged in, money $4.099.977 / $998,999,999, 970 pp [1/2 vehicles] [car: 7] ', '2024-07-21 12:21:55'),
(118, 1, '', '-', 'ionut logged in, money $4.099.977 / $998,999,999, 970 pp [1/2 vehicles] [car: 7] ', '2024-07-21 12:23:16'),
(119, 0, '', '-', ' logged out, money $0 / $0, 0 pp [1/0 vehicles] [car: 7] ', '2024-07-21 12:27:27'),
(120, 1, '', '-', 'ionut logged in, money $4.099.977 / $998,999,999, 970 pp [1/2 vehicles] [car: 7] ', '2024-07-21 12:50:59'),
(121, 0, '', '-', ' logged out, money $0 / $0, 0 pp [1/0 vehicles] [car: 7] ', '2024-07-21 12:51:24'),
(122, 1, '', '-', 'ionut logged in, money $4.099.977 / $998,999,999, 970 pp [1/2 vehicles] [car: 7] ', '2024-07-21 12:51:43'),
(123, 1, '', '-', 'ionut logged in, money $4.099.977 / $998,999,999, 970 pp [1/2 vehicles] [car: 7] ', '2024-07-21 12:52:51'),
(124, 0, '', '-', ' logged out, money $0 / $0, 0 pp [1/0 vehicles] [car: 7] ', '2024-07-21 12:52:52'),
(125, 1, '', '-', 'ionut logged in, money $4.099.977 / $998,999,999, 970 pp [1/2 vehicles] [car: 7] ', '2024-07-21 12:53:23'),
(126, 1, '', '-', 'ionut logged in, money $4.099.977 / $998,999,999, 970 pp [1/2 vehicles] [car: 7] ', '2024-07-21 12:54:32'),
(127, 0, '', '-', ' logged out, money $0 / $0, 0 pp [1/0 vehicles] [car: 7] ', '2024-07-21 12:54:44'),
(128, 1, '', '-', 'ionut logged in, money $4.099.977 / $998,999,999, 970 pp [1/2 vehicles] [car: 7] ', '2024-07-21 12:57:29'),
(129, 1, '', '-', 'ionut logged in, money $4.099.977 / $998,999,999, 970 pp [1/2 vehicles] [car: 7] ', '2024-07-21 12:58:27'),
(130, 1, '', '-', 'ionut logged in, money $4.099.977 / $998,999,999, 970 pp [1/2 vehicles] [car: 7] ', '2024-07-21 12:59:41'),
(131, 1, '', '-', 'ionut logged in, money $4.799.977 / $998,999,999, 970 pp [1/2 vehicles] [car: 7] ', '2024-07-21 13:04:52'),
(132, 0, '', '-', ' logged out, money $0 / $0, 0 pp [1/0 vehicles] [car: 7] ', '2024-07-21 13:04:57'),
(133, 1, '', '-', 'ionut logged in, money $4.799.977 / $998,999,999, 970 pp [1/2 vehicles] [car: 7] ', '2024-07-21 13:05:17'),
(134, 1, '', '-', 'ionut logged in, money $4.799.977 / $998,999,999, 970 pp [1/2 vehicles] [car: 7] ', '2024-07-21 13:05:52'),
(135, 0, '', '-', ' logged out, money $0 / $0, 0 pp [1/0 vehicles] [car: 7] ', '2024-07-21 13:05:53'),
(136, 1, '', '-', 'ionut logged in, money $4.799.977 / $998,999,999, 970 pp [1/2 vehicles] [car: 7] ', '2024-07-21 13:06:14'),
(137, 0, '', '-', ' logged out, money $0 / $0, 0 pp [1/0 vehicles] [car: 7] ', '2024-07-21 13:06:23'),
(138, 1, '', '-', 'ionut logged in, money $4.799.977 / $998,999,999, 970 pp [1/2 vehicles] [car: 7] ', '2024-07-21 17:54:10'),
(139, 0, '', '-', ' logged out, money $0 / $0, 0 pp [1/0 vehicles] [car: 7] ', '2024-07-21 17:54:53'),
(140, 1, '', '-', 'ionut logged in, money $4.799.977 / $998,999,999, 970 pp [1/2 vehicles] [car: 7] ', '2024-07-21 18:27:34'),
(141, 1, '', '-', 'ionut logged in, money $4.799.977 / $998,999,999, 970 pp [1/2 vehicles] [car: 7] ', '2024-07-21 18:33:06'),
(142, 1, '', '-', 'ionut logged in, money $4.799.977 / $998,999,999, 970 pp [1/2 vehicles] [car: 7] ', '2024-07-21 18:39:04'),
(143, 0, '', '-', ' logged out, money $0 / $0, 0 pp [1/0 vehicles] [car: 7] ', '2024-07-21 18:47:14'),
(144, 1, '', '-', 'ionut logged in, money $4.799.977 / $998,999,999, 970 pp [1/2 vehicles] [car: 7] ', '2024-07-21 18:51:54'),
(145, 1, '', '-', 'ionut logged in, money $4.799.977 / $998,999,999, 970 pp [1/2 vehicles] [car: 7] ', '2024-07-21 18:55:11'),
(146, 0, '', '-', ' logged out, money $0 / $0, 0 pp [1/0 vehicles] [car: 7] ', '2024-07-21 19:00:03'),
(147, 1, '', '-', 'ionut logged in, money $4.799.977 / $998,999,999, 970 pp [1/2 vehicles] [car: 7] ', '2024-07-21 19:04:24'),
(148, 1, '', '-', 'ionut logged in, money $4.799.977 / $998,999,999, 970 pp [1/2 vehicles] [car: 7] ', '2024-07-21 19:10:08'),
(149, 1, '', '-', 'ionut logged in, money $4.799.977 / $998,999,999, 970 pp [1/2 vehicles] [car: 7] ', '2024-07-21 19:13:15'),
(150, 0, '', '-', ' logged out, money $0 / $0, 0 pp [1/0 vehicles] [car: 7] ', '2024-07-21 19:16:35'),
(151, 1, '', '-', 'ionut logged in, money $4.799.977 / $998,999,999, 970 pp [1/2 vehicles] [car: 7] ', '2024-07-21 19:18:47'),
(152, 1, '', '-', 'ionut logged in, money $4.799.977 / $998,999,999, 970 pp [1/2 vehicles] [car: 7] ', '2024-07-21 19:21:41'),
(153, 0, '', '-', ' logged out, money $0 / $0, 0 pp [1/0 vehicles] [car: 7] ', '2024-07-21 19:58:40'),
(154, 1, '', '-', 'ionut logged in, money $4.799.977 / $998,999,999, 970 pp [1/2 vehicles] [car: 7] ', '2024-07-21 20:54:35'),
(155, 0, '', '-', ' logged out, money $0 / $0, 0 pp [1/0 vehicles] [car: 7] ', '2024-07-21 20:58:15'),
(156, 1, '', '-', 'ionut logged in, money $4.799.977 / $998,999,999, 970 pp [1/2 vehicles] [car: 7] ', '2024-07-21 21:09:48'),
(157, 1, '', '-', 'ionut logged in, money $4.799.977 / $998,999,999, 970 pp [1/2 vehicles] [car: 7] ', '2024-07-21 21:11:17'),
(158, 1, '', '-', 'ionut logged in, money $4.799.977 / $998,999,999, 970 pp [1/2 vehicles] [car: 7] ', '2024-07-21 21:12:41'),
(159, 1, '', '-', 'ionut logged in, money $4.799.977 / $998,999,999, 970 pp [1/2 vehicles] [car: 7] ', '2024-07-21 21:21:59'),
(160, 0, '', '-', ' logged out, money $0 / $0, 0 pp [1/0 vehicles] [car: 7] ', '2024-07-21 21:22:01'),
(161, 1, '', '-', 'ionut logged in, money $4.799.977 / $998,999,999, 970 pp [1/2 vehicles] [car: 7] ', '2024-07-21 21:22:36'),
(162, 0, '', '-', ' logged out, money $0 / $0, 0 pp [1/0 vehicles] [car: 7] ', '2024-07-21 21:22:56'),
(163, 1, '', '-', 'ionut logged in, money $4.799.977 / $998,999,999, 970 pp [1/2 vehicles] [car: 7] ', '2024-07-21 21:24:59'),
(164, 0, '', '-', ' logged out, money $0 / $0, 0 pp [1/0 vehicles] [car: 7] ', '2024-07-21 21:26:02'),
(165, 1, '', '-', 'ionut logged in, money $4.799.977 / $998,999,999, 970 pp [1/2 vehicles] [car: 7] ', '2024-07-22 20:37:51'),
(166, 1, '', '-', 'ionut logged in, money $4.799.977 / $998,999,999, 970 pp [1/2 vehicles] [car: 7] ', '2024-07-22 20:47:57'),
(167, 1, '', '-', 'ionut logged in, money $5.499.977 / $998,999,999, 970 pp [1/2 vehicles] [car: 7] ', '2024-07-22 21:03:27'),
(168, 1, '', '-', 'ionut logged in, money $5.499.977 / $998,999,999, 970 pp [1/2 vehicles] [car: 7] ', '2024-07-22 21:06:52'),
(169, 1, '', '-', 'ionut logged in, money $5.499.977 / $998,999,999, 970 pp [1/2 vehicles] [car: 7] ', '2024-07-22 21:08:30'),
(170, 0, '', '-', ' logged out, money $0 / $0, 0 pp [1/0 vehicles] [car: 7] ', '2024-07-22 21:08:32'),
(171, 1, '', '-', 'ionut logged in, money $5.499.977 / $998,999,999, 970 pp [1/2 vehicles] [car: 7] ', '2024-07-22 21:08:51'),
(172, 1, '', '-', 'ionut logged in, money $5.499.977 / $998,999,999, 970 pp [1/2 vehicles] [car: 7] ', '2024-07-22 21:11:25'),
(173, 1, '', '-', 'ionut logged in, money $8.077.275 / $998,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-22 21:20:20'),
(174, 1, '', '-', 'ionut logged in, money $8.077.275 / $998,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-22 21:24:27'),
(175, 1, '', '-', 'ionut logged in, money $7.877.275 / $998,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-22 21:26:59'),
(176, 1, '', '-', 'ionut logged in, money $7.877.275 / $998,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-22 21:29:37'),
(177, 1, '', '-', 'ionut logged in, money $7.877.275 / $998,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-22 21:30:27'),
(178, 1, '', '-', 'ionut logged in, money $7.877.275 / $998,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-22 21:33:49'),
(179, 1, '', '-', 'ionut logged in, money $7.877.275 / $998,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-22 21:35:25'),
(180, 1, '', '-', 'ionut logged in, money $7.877.275 / $998,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-22 21:36:51'),
(181, 1, '', '-', 'ionut logged in, money $7.877.275 / $998,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-22 21:41:03'),
(182, 0, '', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-22 21:42:00'),
(183, 1, '', '-', 'ionut logged in, money $7.870.775 / $998,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-22 21:43:15'),
(184, 2, '', '-', 'ionut2 logged in, money $32.000 / $30,000, 0 pp [0/2 vehicles] ', '2024-07-22 21:43:35'),
(185, 2, '', '-', 'ionut2 logged in, money $26.000 / $30,000, 0 pp [0/2 vehicles] ', '2024-07-22 21:46:23'),
(186, 1, 'ionut2', '-', 'ionut logged in, money $7.870.775 / $998,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-22 21:46:25'),
(187, 1, 'ionut2', '', '[trade] ionut [user:1] a oferit 1 droguri lui ionut2 ', '2024-07-22 21:47:50'),
(188, 2, '', 'ionut2', '[trade] ionut2 [user:2] a primit 1 droguri de la ionut ', '2024-07-22 21:47:50'),
(189, 2, '', 'ionut2', '[trade] ionut2 [user:2] a oferit 1 droguri lui ionut ', '2024-07-22 21:49:18'),
(190, 1, 'ionut2', '', '[trade] ionut [user:1] a primit 1 droguri de la ionut2 ', '2024-07-22 21:49:18'),
(191, 2, '', '-', 'ionut2 logged in, money $26.000 / $999,999,998, 0 pp [0/2 vehicles] ', '2024-07-22 21:51:36'),
(192, 1, 'ionut2', '-', 'ionut logged in, money $7.870.775 / $0, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-22 21:51:38'),
(193, 1, 'ionut2', '', '[trade] ionut [user:1] a oferit 1 droguri lui ionut2 ', '2024-07-22 21:52:32'),
(194, 2, '', 'ionut2', '[trade] ionut2 [user:2] a primit 1 droguri de la ionut ', '2024-07-22 21:52:32'),
(195, 1, '', '-', 'ionut logged in, money $7.870.775 / $1, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-22 21:57:01'),
(196, 2, '', '-', 'ionut2 logged in, money $26.000 / $2, 0 pp [0/2 vehicles] ', '2024-07-22 21:57:04'),
(197, 2, '', '-', 'ionut2 logged out, money $26.000 / $2, 0 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-22 21:57:13'),
(198, 1, 'ionut2', '-', 'ionut logged in, money $7.870.775 / $1, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-22 21:57:29'),
(199, 1, 'ionut2', '', '[trade] ionut [user:1] a oferit 1 droguri lui ionut2 ', '2024-07-22 21:58:17'),
(200, 2, '', 'ionut2', '[trade] ionut2 [user:2] a primit 1 droguri de la ionut ', '2024-07-22 21:58:17'),
(201, 2, '', '-', 'ionut2 logged in, money $26.000 / $0, 0 pp [0/2 vehicles] ', '2024-07-22 22:03:30'),
(202, 1, 'ionut2', '-', 'ionut logged in, money $8.570.775 / $1, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-22 22:03:32'),
(203, 1, '', '-', 'ionut logged in, money $8.570.775 / $1, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-22 22:05:14'),
(204, 2, '', '-', 'ionut2 logged in, money $26.000 / $0, 0 pp [0/2 vehicles] ', '2024-07-22 22:05:16'),
(205, 2, '', '-', 'ionut2 logged in, money $26.000 / $0, 0 pp [0/2 vehicles] ', '2024-07-22 22:07:05'),
(206, 1, 'ionut2', '-', 'ionut logged in, money $8.570.775 / $1, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-22 22:07:07'),
(207, 2, '', '-', 'ionut2 logged in, money $26.000 / $0, 0 pp [0/2 vehicles] ', '2024-07-22 22:07:59'),
(208, 1, 'ionut2', '-', 'ionut logged in, money $8.570.775 / $1, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-22 22:08:02'),
(209, 1, '', '-', 'ionut logged in, money $8.570.775 / $1, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-22 22:11:58'),
(210, 2, '', '-', 'ionut2 logged in, money $26.000 / $0, 0 pp [0/2 vehicles] ', '2024-07-22 22:12:01'),
(211, 2, '', '-', 'ionut2 logged in, money $26.000 / $0, 0 pp [0/2 vehicles] ', '2024-07-22 22:18:48'),
(212, 1, 'ionut2', '-', 'ionut logged in, money $8.570.775 / $1, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-22 22:18:51'),
(213, 2, '', '-', 'ionut2 logged in, money $26.000 / $0, 0 pp [0/2 vehicles] ', '2024-07-22 22:19:24'),
(214, 1, 'ionut2', '-', 'ionut logged in, money $8.570.775 / $1, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-22 22:19:36'),
(215, 0, 'ionut', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-22 22:24:39'),
(216, 1, '', '-', 'ionut logged in, money $8.570.775 / $1, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-22 22:35:01'),
(217, 0, 'ionut', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-22 22:35:05'),
(218, 1, '', '-', 'ionut logged in, money $8.570.775 / $1, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-22 22:36:21'),
(219, 1, '', '-', 'ionut logged in, money $8.570.775 / $1, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-22 22:37:26'),
(220, 0, 'ionut', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-22 22:37:28'),
(221, 0, '', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-22 22:37:36'),
(222, 1, '', '-', 'ionut logged in, money $8.570.775 / $1, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-23 21:35:54'),
(223, 1, '', '-', 'ionut logged in, money $8.570.775 / $1, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-23 21:38:45'),
(224, 1, '', '-', 'ionut logged in, money $8.583.506 / $1, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-23 21:44:07'),
(225, 0, '', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-23 21:45:10'),
(226, 1, '', '-', 'ionut logged in, money $8.583.506 / $1, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-23 21:45:23'),
(227, 1, '', '-', 'ionut logged in, money $8.583.506 / $1, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-23 21:46:18'),
(228, 1, '', '-', 'ionut logged in, money $8.583.506 / $1, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-23 21:46:56'),
(229, 1, '', '-', 'ionut logged in, money $8.583.506 / $1, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-23 21:53:59'),
(230, 1, '', '-', 'ionut logged in, money $8.583.506 / $1, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-23 21:59:52'),
(231, 1, '', '-', 'ionut logged in, money $8.583.506 / $1, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-23 22:01:05'),
(232, 0, '', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-23 22:01:12'),
(233, 1, '', '-', 'ionut logged in, money $8.583.506 / $1, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-23 22:02:54'),
(234, 1, '', '-', 'ionut logged in, money $8.583.506 / $1, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-23 22:08:09'),
(235, 1, '', '-', 'ionut logged in, money $8.583.506 / $1, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-23 22:09:03'),
(236, 1, '', '-', 'ionut logged in, money $8.583.506 / $1, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-23 22:09:46'),
(237, 0, '', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-23 22:09:57'),
(238, 1, '', '-', 'ionut logged in, money $8.583.506 / $1, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-23 22:10:09'),
(239, 0, '', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-23 22:10:31'),
(240, 1, '', '-', 'ionut logged in, money $8.583.506 / $1, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-23 22:10:45'),
(241, 1, '', '-', 'ionut logged in, money $8.583.506 / $1, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-23 22:14:59'),
(242, 1, '', '-', 'ionut logged in, money $8.583.506 / $1, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-23 22:16:10'),
(243, 1, '', '-', 'ionut logged in, money $8.583.506 / $1, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-23 22:17:37'),
(244, 0, '', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-23 22:18:29'),
(245, 1, '', '-', 'ionut logged in, money $8.583.506 / $1, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-23 22:19:44'),
(246, 0, '', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-23 22:20:38'),
(247, 1, '', '-', 'ionut logged in, money $8.583.506 / $1, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-23 22:21:10'),
(248, 1, '', '-', 'ionut logged in, money $8.583.506 / $1, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-23 22:21:51'),
(249, 1, '', '-', 'ionut logged in, money $8.583.506 / $1, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-23 22:23:14'),
(250, 0, '', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-23 22:23:47'),
(251, 1, '', '-', 'ionut logged in, money $8.583.506 / $1, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-23 22:24:01'),
(252, 1, '', '-', 'ionut logged in, money $8.583.506 / $1, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-23 22:25:11'),
(253, 0, '', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-23 22:25:20'),
(254, 1, '', '-', 'ionut logged in, money $8.583.506 / $1, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-23 22:25:31'),
(255, 1, '', '-', 'ionut logged in, money $8.583.506 / $1, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-23 22:27:42'),
(256, 1, '', '-', 'ionut logged in, money $8.583.506 / $1, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-23 22:31:21'),
(257, 1, '', '-', 'ionut logged in, money $8.583.506 / $1, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-23 22:33:17'),
(258, 0, '', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-23 22:33:46'),
(259, 1, '', '-', 'ionut logged in, money $8.583.506 / $1, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-23 22:34:09'),
(260, 0, '', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-23 22:35:01'),
(261, 1, '', '-', 'ionut logged in, money $8.583.506 / $1, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-23 22:35:14'),
(262, 0, '', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-23 22:36:00'),
(263, 1, '', '-', 'ionut logged in, money $8.583.506 / $1, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-24 12:36:00'),
(264, 1, '', '-', 'ionut logged in, money $8.583.506 / $1, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-24 12:39:09'),
(265, 0, '', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-24 12:41:33'),
(266, 1, '', '-', 'ionut logged in, money $8.583.506 / $1, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-24 12:42:11'),
(267, 1, '', '-', 'ionut logged in, money $8.583.506 / $1, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-24 12:45:53'),
(268, 0, '', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-24 12:47:44'),
(269, 1, '', '-', 'ionut logged in, money $8.583.506 / $1, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-24 12:49:54'),
(270, 0, '', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-24 12:51:28'),
(271, 1, '', '-', 'ionut logged in, money $8.583.506 / $1, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-24 12:53:56'),
(272, 0, '', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-24 12:56:48'),
(273, 1, '', '-', 'ionut logged in, money $8.583.506 / $1, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-24 12:57:58'),
(274, 0, '', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-24 12:59:29'),
(275, 1, '', '-', 'ionut logged in, money $8.583.506 / $1, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-24 13:01:48'),
(276, 0, '', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-24 13:06:53'),
(277, 1, '', '-', 'ionut logged in, money $8.583.506 / $1, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-24 15:58:53'),
(278, 0, '', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-24 16:00:26'),
(279, 1, '', '-', 'ionut logged in, money $8.583.506 / $1, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-24 16:27:00'),
(280, 1, '', '-', 'ionut logged in, money $8.583.506 / $1, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-24 16:33:34'),
(281, 0, '', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-24 16:34:52'),
(282, 1, '', '-', 'ionut logged in, money $8.583.506 / $1, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-24 16:36:42'),
(283, 0, '', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-24 16:39:13'),
(284, 1, '', '-', 'ionut logged in, money $8.583.506 / $1, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-24 16:51:47'),
(285, 0, '', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-24 16:54:13'),
(286, 1, '', '-', 'ionut logged in, money $8.583.506 / $1, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-24 17:57:54'),
(287, 2, '', '-', 'ionut2 logged in, money $26.000 / $0, 0 pp [0/2 vehicles] ', '2024-07-24 17:58:14'),
(288, 1, 'ionut2', '', '[trade] ionut [user:1] a oferit 1 weed lui ionut2 ', '2024-07-24 18:00:31'),
(289, 2, '', 'ionut2', '[trade] ionut2 [user:2] a primit 1 weed de la ionut ', '2024-07-24 18:00:31'),
(290, 1, 'ionut2', '', '[trade] ionut [user:1] a oferit 1 methamphetamine lui ionut2 ', '2024-07-24 18:00:31'),
(291, 2, '', 'ionut2', '[trade] ionut2 [user:2] a primit 1 methamphetamine de la ionut ', '2024-07-24 18:00:31'),
(292, 1, 'ionut2', '', '[trade] ionut [user:1] a oferit 1 cocaine lui ionut2 ', '2024-07-24 18:00:31'),
(293, 2, '', 'ionut2', '[trade] ionut2 [user:2] a primit 1 cocaine de la ionut ', '2024-07-24 18:00:31'),
(294, 0, 'ionut', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-24 18:01:29'),
(295, 0, '', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-24 18:01:35'),
(296, 1, '', '-', 'ionut logged in, money $8.583.506 / $2, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-24 18:27:45'),
(297, 1, '', '-', 'ionut logged in, money $8.583.506 / $2, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-24 18:33:09'),
(298, 0, '', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-24 18:33:11'),
(299, 1, '', '-', 'ionut logged in, money $8.583.506 / $2, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-24 18:33:22'),
(300, 1, '', '-', 'ionut logged in, money $8.583.506 / $2, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-24 18:46:40'),
(301, 1, '', '-', 'ionut logged in, money $8.583.506 / $2, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-24 18:50:00'),
(302, 0, '', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-24 18:50:43'),
(303, 1, '', '-', 'ionut logged in, money $8.583.506 / $2, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-24 18:51:04'),
(304, 1, '', '-', 'ionut logged in, money $8.583.506 / $2, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-24 18:53:07'),
(305, 1, '', '-', 'ionut logged in, money $8.583.506 / $2, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-24 18:54:07'),
(306, 1, '', '-', 'ionut logged in, money $8.583.506 / $2, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-24 18:54:48'),
(307, 0, '', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-24 19:09:02'),
(308, 1, '', '-', 'ionut logged in, money $8.583.506 / $2, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-24 19:13:18'),
(309, 0, '', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-24 19:20:57'),
(310, 1, '', '-', 'ionut logged in, money $8.583.506 / $2, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-24 20:28:12'),
(311, 1, '', '-', 'ionut logged in, money $8.583.506 / $2, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-24 20:30:04'),
(312, 0, '', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-24 20:30:20'),
(313, 1, '', '-', 'ionut logged in, money $8.583.506 / $2, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-24 20:30:32'),
(314, 1, '', '-', 'ionut logged in, money $8.583.506 / $2, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-24 20:31:52'),
(315, 0, '', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-24 20:32:07'),
(316, 1, '', '-', 'ionut logged in, money $8.583.506 / $2, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-24 21:20:56'),
(317, 0, '', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-24 21:30:31'),
(318, 1, '', '-', 'ionut logged in, money $8.578.006 / $899,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-25 12:34:16'),
(319, 1, '', '-', 'ionut logged in, money $8.378.006 / $899,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-25 12:36:20'),
(320, 1, '', '-', 'ionut logged in, money $8.378.006 / $899,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-25 12:37:30'),
(321, 1, '', '-', 'ionut logged in, money $8.378.006 / $899,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-25 12:38:56'),
(322, 1, '', '-', 'ionut logged in, money $8.378.006 / $899,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-25 12:41:17'),
(323, 0, '', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-25 12:41:18'),
(324, 1, '', '-', 'ionut logged in, money $8.378.006 / $899,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-25 12:41:28'),
(325, 1, '', '-', 'ionut logged in, money $8.378.006 / $899,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-25 12:46:38'),
(326, 1, '', '-', 'ionut logged in, money $8.378.006 / $899,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-25 12:49:58'),
(327, 0, '', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-25 12:49:59'),
(328, 1, '', '-', 'ionut logged in, money $8.378.006 / $899,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-25 12:50:08'),
(329, 1, '', '-', 'ionut logged in, money $8.378.006 / $899,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-25 13:02:58'),
(330, 1, '', '-', 'ionut logged in, money $8.378.006 / $899,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-25 13:05:29'),
(331, 1, '', '-', 'ionut logged in, money $8.378.006 / $899,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-25 13:06:52'),
(332, 0, '', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-25 13:10:52'),
(333, 1, '', '-', 'ionut logged in, money $8.378.006 / $899,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-25 13:11:58'),
(334, 1, '', '-', 'ionut logged in, money $8.378.006 / $899,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-25 13:15:01'),
(335, 1, '', '-', 'ionut logged in, money $8.378.006 / $899,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-25 13:21:40'),
(336, 1, '', '-', 'ionut logged in, money $8.378.006 / $899,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-25 13:25:07'),
(337, 1, '', '-', 'ionut logged in, money $8.378.006 / $899,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-25 13:26:08'),
(338, 1, '', '-', 'ionut logged in, money $8.372.856 / $899,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-25 13:28:36'),
(339, 2, '', '-', 'ionut2 logged in, money $26.000 / $0, 0 pp [0/2 vehicles] ', '2024-07-25 13:28:50'),
(340, 2, '', '-', 'ionut2 logged in, money $25.999 / $0, 0 pp [0/2 vehicles] ', '2024-07-25 13:32:48'),
(341, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-25 13:32:49'),
(342, 1, '', '-', 'ionut logged in, money $8.372.857 / $899,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-25 13:32:51'),
(343, 2, '', '-', 'ionut2 logged in, money $25.999 / $0, 0 pp [0/2 vehicles] ', '2024-07-25 13:33:04'),
(344, 1, '', '-', 'ionut logged in, money $8.372.867 / $899,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-25 13:35:25'),
(345, 2, '', '-', 'ionut2 logged in, money $25.989 / $0, 0 pp [0/2 vehicles] ', '2024-07-25 13:35:28'),
(346, 0, 'ionut', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-25 13:36:18'),
(347, 0, '', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-25 13:36:20'),
(348, 1, '', '-', 'ionut logged in, money $8.372.868 / $899,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-25 23:01:55'),
(349, 1, '', '-', 'ionut logged in, money $8.372.868 / $899,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-25 23:11:39'),
(350, 1, '', '-', 'ionut logged in, money $8.372.868 / $899,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-25 23:19:00'),
(351, 0, '', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-25 23:21:01'),
(352, 1, '', '-', 'ionut logged in, money $8.372.868 / $899,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-25 23:36:33'),
(353, 1, '', '-', 'ionut logged in, money $8.372.868 / $899,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-25 23:45:09'),
(354, 1, '', '-', 'ionut logged in, money $8.372.868 / $899,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-25 23:48:58'),
(355, 0, 'ionut', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-25 23:50:23'),
(356, 1, '', '-', 'ionut logged in, money $8.372.868 / $899,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-25 23:51:38'),
(357, 1, '', '-', 'ionut logged in, money $8.372.868 / $899,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-25 23:56:30'),
(358, 0, '', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-26 00:05:15'),
(359, 1, '', '-', 'ionut logged in, money $8.423.705 / $899,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 00:06:18'),
(360, 1, '', '-', 'ionut logged in, money $8.423.705 / $899,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 00:12:08'),
(361, 1, '', '-', 'ionut logged in, money $8.423.705 / $899,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 00:16:21'),
(362, 1, '', '-', 'ionut logged in, money $8.423.705 / $899,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 00:18:31'),
(363, 0, '', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-26 00:20:54'),
(364, 1, '', '-', 'ionut logged in, money $8.423.705 / $899,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 00:22:53'),
(365, 1, '', '-', 'ionut logged in, money $8.423.705 / $899,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 00:27:56'),
(366, 0, '', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-26 00:28:16'),
(367, 1, '', '-', 'ionut logged in, money $8.423.705 / $899,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 00:28:33'),
(368, 0, '', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-26 00:28:56'),
(369, 1, '', '-', 'ionut logged in, money $8.423.705 / $899,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 00:29:07'),
(370, 1, '', '-', 'ionut logged in, money $8.423.705 / $899,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 00:31:40'),
(371, 0, '', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-26 00:31:41'),
(372, 1, '', '-', 'ionut logged in, money $8.423.705 / $899,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 00:31:52'),
(373, 1, '', '-', 'ionut logged in, money $8.423.705 / $899,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 00:35:00'),
(374, 1, '', '-', 'ionut logged in, money $8.423.705 / $899,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 00:38:46'),
(375, 0, '', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-26 00:38:47'),
(376, 1, '', '-', 'ionut logged in, money $8.423.705 / $899,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 00:39:08'),
(377, 0, '', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-26 00:40:44'),
(378, 1, '', '-', 'ionut logged in, money $8.423.705 / $899,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 00:41:06'),
(379, 1, '', '-', 'ionut logged in, money $8.423.705 / $899,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 00:43:24'),
(380, 0, '', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-26 00:46:25'),
(381, 1, '', '-', 'ionut logged in, money $8.423.705 / $899,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 00:47:13'),
(382, 0, '', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-26 01:01:32'),
(383, 1, '', '-', 'ionut logged in, money $8.423.705 / $899,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 09:19:30'),
(384, 0, 'ionut', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-26 09:36:43'),
(385, 1, '', '-', 'ionut logged in, money $8.428.063 / $899,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 09:46:46'),
(386, 1, '', '-', 'ionut logged in, money $8.428.063 / $899,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 09:48:43'),
(387, 1, '', '-', 'ionut logged in, money $8.428.063 / $899,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 09:50:55'),
(388, 0, '', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-26 09:50:56'),
(389, 1, '', '-', 'ionut logged in, money $8.428.063 / $899,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 09:51:11'),
(390, 1, '', '-', 'ionut logged in, money $8.428.063 / $899,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 09:53:24'),
(391, 1, '', '-', 'ionut logged in, money $8.428.063 / $899,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 09:56:09'),
(392, 1, '', '-', 'ionut logged in, money $8.428.063 / $899,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 09:57:37'),
(393, 0, '', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-26 09:57:38'),
(394, 1, '', '-', 'ionut logged in, money $8.428.063 / $899,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 09:57:50'),
(395, 1, '', '-', 'ionut logged in, money $8.428.063 / $899,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 10:05:14'),
(396, 1, '', '-', 'ionut logged in, money $8.434.725 / $899,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 10:17:40'),
(397, 0, 'ionut', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-26 10:35:07'),
(398, 0, '', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-26 10:43:34'),
(399, 1, '', '-', 'ionut logged in, money $8.439.959 / $899,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 11:55:23'),
(400, 2, '', '-', 'ionut2 logged in, money $25.988 / $0, 0 pp [0/2 vehicles] ', '2024-07-26 11:55:56'),
(401, 2, '', '-', 'ionut2 logged in, money $25.988 / $0, 0 pp [0/2 vehicles] ', '2024-07-26 11:57:35'),
(402, 1, 'ionut2', '-', 'ionut logged in, money $8.439.959 / $899,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 11:57:37'),
(403, 0, 'ionut', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-26 11:57:39'),
(404, 2, '', '-', 'ionut2 logged in, money $25.988 / $0, 0 pp [0/2 vehicles] ', '2024-07-26 11:57:50'),
(405, 0, 'ionut', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-26 11:59:15'),
(406, 0, '', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-26 11:59:16'),
(407, 1, '', '-', 'ionut logged in, money $8.439.959 / $899,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 12:07:34'),
(408, 1, '', '-', 'ionut logged in, money $8.439.959 / $899,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 12:10:23'),
(409, 0, '', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-26 12:14:45'),
(410, 1, '', '-', 'ionut logged in, money $8.442.635 / $899,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 12:15:33'),
(411, 0, '', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-26 12:15:46'),
(412, 1, '', '-', 'ionut logged in, money $8.442.635 / $899,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 12:20:04'),
(413, 0, '', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-26 12:23:33'),
(414, 1, '', '-', 'ionut logged in, money $8.445.138 / $899,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 12:24:13'),
(415, 1, '', '-', 'ionut logged in, money $8.445.138 / $899,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 12:26:56'),
(416, 1, '', '-', 'ionut logged in, money $8.445.138 / $899,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 12:29:59'),
(417, 0, '', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-26 12:31:15'),
(418, 1, '', '-', 'ionut logged in, money $8.445.138 / $899,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 12:37:16'),
(419, 1, '', '-', 'ionut logged in, money $8.445.138 / $899,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 12:41:30'),
(420, 0, '', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-26 12:41:31'),
(421, 1, '', '-', 'ionut logged in, money $8.445.138 / $899,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 12:41:41'),
(422, 1, '', '-', 'ionut logged in, money $8.445.138 / $899,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 12:43:39'),
(423, 0, '', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-26 12:43:41'),
(424, 1, '', '-', 'ionut logged in, money $8.445.138 / $899,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 12:43:53');
INSERT INTO `player_logs` (`id`, `userID`, `userName`, `giverName`, `logText`, `logTime`) VALUES
(425, 2, '', '-', 'ionut2 logged in, money $25.988 / $0, 0 pp [0/2 vehicles] ', '2024-07-26 12:45:24'),
(426, 0, 'ionut', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-26 12:48:41'),
(427, 1, '', '-', 'ionut logged in, money $8.439.638 / $899,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 12:48:59'),
(428, 1, '', '-', 'ionut logged in, money $8.439.638 / $899,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 12:50:28'),
(429, 0, '', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-26 12:50:50'),
(430, 2, '', '-', 'ionut2 logged in, money $25.988 / $0, 0 pp [0/2 vehicles] ', '2024-07-26 12:51:01'),
(431, 2, '', '-', 'ionut2 logged in, money $25.988 / $0, 0 pp [0/2 vehicles] ', '2024-07-26 12:52:07'),
(432, 2, '', '-', 'ionut2 logged in, money $25.988 / $0, 0 pp [0/2 vehicles] ', '2024-07-26 12:54:10'),
(433, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-26 12:54:14'),
(434, 2, '', '-', 'ionut2 logged in, money $25.988 / $0, 0 pp [0/2 vehicles] ', '2024-07-26 12:56:24'),
(435, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-26 12:56:36'),
(436, 2, '', '-', 'ionut2 logged in, money $25.988 / $0, 0 pp [0/2 vehicles] ', '2024-07-26 12:56:54'),
(437, 2, '', '-', 'ionut2 logged in, money $25.988 / $0, 0 pp [0/2 vehicles] ', '2024-07-26 12:58:01'),
(438, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-26 12:58:21'),
(439, 1, '', '-', 'ionut logged in, money $8.439.638 / $899,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 12:58:33'),
(440, 0, '', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-26 12:58:54'),
(441, 1, '', '-', 'ionut logged in, money $8.439.638 / $899,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 13:00:18'),
(442, 0, '', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-26 13:00:44'),
(443, 1, '', '-', 'ionut logged in, money $8.439.638 / $899,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 13:01:05'),
(444, 1, '', '-', 'ionut logged in, money $8.439.638 / $899,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 13:03:35'),
(445, 1, '', '-', 'ionut logged in, money $8.439.638 / $899,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 13:10:09'),
(446, 0, '', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-26 13:14:24'),
(447, 1, '', '-', 'ionut logged in, money $8.448.582 / $899,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 13:14:45'),
(448, 1, '', '-', 'ionut logged in, money $8.452.189 / $899,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 13:17:05'),
(449, 0, '', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-26 13:17:06'),
(450, 1, '', '-', 'ionut logged in, money $8.452.189 / $899,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 13:17:17'),
(451, 1, '', '-', 'ionut logged in, money $8.456.186 / $899,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 13:18:51'),
(452, 0, '', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-26 13:21:13'),
(453, 1, '', '-', 'ionut logged in, money $8.426.686 / $899,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 13:21:50'),
(454, 2, '', '-', 'ionut2 logged in, money $25.988 / $0, 0 pp [0/2 vehicles] ', '2024-07-26 13:22:03'),
(455, 0, 'ionut', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-26 13:31:14'),
(456, 1, '', '-', 'ionut logged in, money $8.426.686 / $899,999,999, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 19:38:42'),
(457, 0, '', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-26 19:43:06'),
(458, 1, '', '-', 'ionut logged in, money $999.999.999 / $899,999,893, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 19:43:30'),
(459, 1, '', '-', 'ionut logged in, money $999.719.999 / $899,999,893, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 19:45:11'),
(460, 0, '', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-26 19:45:13'),
(461, 1, '', '-', 'ionut logged in, money $999.719.999 / $899,999,893, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 19:45:27'),
(462, 1, '', '-', 'ionut logged in, money $999.075.999 / $899,999,893, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 19:49:57'),
(463, 1, '', '-', 'ionut logged in, money $998.711.999 / $899,999,893, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 19:52:35'),
(464, 0, '', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-26 19:52:40'),
(465, 1, '', '-', 'ionut logged in, money $998.711.999 / $899,999,893, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 19:54:15'),
(466, 1, '', '-', 'ionut logged in, money $998.711.999 / $899,999,893, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 19:56:17'),
(467, 1, '', '-', 'ionut logged in, money $998.711.999 / $899,999,893, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 19:57:57'),
(468, 1, '', '-', 'ionut logged in, money $998.711.999 / $899,999,893, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 19:58:47'),
(469, 0, '', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-26 19:58:49'),
(470, 1, '', '-', 'ionut logged in, money $998.711.999 / $899,999,893, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 20:02:06'),
(471, 1, '', '-', 'ionut logged in, money $998.711.999 / $899,999,893, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 20:07:06'),
(472, 0, '', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-26 20:09:45'),
(473, 1, '', '-', 'ionut logged in, money $996.471.999 / $899,999,893, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 20:10:18'),
(474, 0, '', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-26 20:11:06'),
(475, 1, '', '-', 'ionut logged in, money $989.079.999 / $899,999,893, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 20:13:00'),
(476, 0, '', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-26 20:13:38'),
(477, 1, '', '-', 'ionut logged in, money $985.551.999 / $899,999,893, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 20:18:49'),
(478, 0, '', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-26 20:19:30'),
(479, 1, '', '-', 'ionut logged in, money $982.611.999 / $899,999,893, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 20:20:54'),
(480, 0, '', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-26 20:21:17'),
(481, 1, '', '-', 'ionut logged in, money $982.611.999 / $899,999,893, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 20:57:08'),
(482, 0, '', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-26 20:58:33'),
(483, 1, '', '-', 'ionut logged in, money $980.511.999 / $899,999,893, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 21:00:02'),
(484, 0, 'ionut', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-26 21:17:19'),
(485, 1, '', '-', 'ionut logged in, money $977.134.499 / $899,999,893, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 21:21:36'),
(486, 0, '', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-26 21:21:38'),
(487, 1, '', '-', 'ionut logged in, money $977.134.499 / $899,999,893, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 21:21:50'),
(488, 1, '', '-', 'ionut logged in, money $977.134.499 / $899,999,893, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 21:24:52'),
(489, 0, '', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-26 21:27:15'),
(490, 1, '', '-', 'ionut logged in, money $977.134.499 / $899,999,893, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 22:17:35'),
(491, 2, '', '-', 'ionut2 logged in, money $25.988 / $0, 0 pp [0/2 vehicles] ', '2024-07-26 22:17:50'),
(492, 1, '', '-', 'ionut logged in, money $977.134.499 / $899,999,893, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 22:18:59'),
(493, 2, '', '-', 'ionut2 logged in, money $25.988 / $0, 0 pp [0/2 vehicles] ', '2024-07-26 22:19:03'),
(494, 1, '', '-', 'ionut logged in, money $977.134.499 / $899,999,893, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 22:20:04'),
(495, 2, '', '-', 'ionut2 logged in, money $25.988 / $0, 0 pp [0/2 vehicles] ', '2024-07-26 22:20:06'),
(496, 0, 'ionut', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-26 22:20:07'),
(497, 2, '', '-', 'ionut2 logged in, money $25.988 / $0, 0 pp [0/2 vehicles] ', '2024-07-26 22:20:23'),
(498, 1, '', '-', 'ionut logged in, money $977.134.499 / $899,999,893, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 22:21:40'),
(499, 0, '', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-26 22:21:42'),
(500, 2, '', '-', 'ionut2 logged in, money $25.988 / $0, 0 pp [0/2 vehicles] ', '2024-07-26 22:21:45'),
(501, 1, 'ionut2', '-', 'ionut logged in, money $977.134.499 / $899,999,893, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 22:22:04'),
(502, 2, '', '-', 'ionut2 logged in, money $25.988 / $0, 0 pp [0/2 vehicles] ', '2024-07-26 22:24:08'),
(503, 1, 'ionut2', '-', 'ionut logged in, money $977.134.499 / $899,999,893, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 22:24:15'),
(504, 2, '', '-', 'ionut2 logged in, money $25.988 / $0, 0 pp [0/2 vehicles] ', '2024-07-26 22:30:23'),
(505, 1, 'ionut2', '-', 'ionut logged in, money $977.134.499 / $899,999,893, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 22:30:25'),
(506, 1, '', '-', 'ionut logged in, money $977.134.499 / $899,999,893, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 22:34:19'),
(507, 2, '', '-', 'ionut2 logged in, money $25.988 / $0, 0 pp [0/2 vehicles] ', '2024-07-26 22:34:22'),
(508, 0, 'ionut', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-26 22:34:24'),
(509, 2, '', '-', 'ionut2 logged in, money $25.988 / $0, 0 pp [0/2 vehicles] ', '2024-07-26 22:34:46'),
(510, 0, 'ionut', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-26 22:51:34'),
(511, 2, '', '-', 'ionut2 logged in, money $25.988 / $0, 0 pp [0/2 vehicles] ', '2024-07-26 22:56:03'),
(512, 1, 'ionut', '-', 'ionut logged in, money $977.134.499 / $899,999,893, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 22:56:16'),
(513, 2, '', '-', 'ionut2 logged in, money $25.988 / $0, 0 pp [0/2 vehicles] ', '2024-07-26 22:57:39'),
(514, 1, 'ionut', '-', 'ionut logged in, money $977.134.499 / $899,999,893, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 22:57:43'),
(515, 2, '', '-', 'ionut2 logged in, money $25.988 / $0, 0 pp [0/2 vehicles] ', '2024-07-26 22:58:37'),
(516, 1, 'ionut', '-', 'ionut logged in, money $977.134.499 / $899,999,893, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 22:58:39'),
(517, 2, '', '-', 'ionut2 logged in, money $25.988 / $0, 0 pp [0/2 vehicles] ', '2024-07-26 23:00:56'),
(518, 1, 'ionut', '-', 'ionut logged in, money $977.134.499 / $899,999,893, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 23:00:58'),
(519, 1, 'ionut', '-', 'ionut logged in, money $977.134.499 / $899,999,893, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 23:01:50'),
(520, 2, '', '-', 'ionut2 logged in, money $25.988 / $0, 0 pp [0/2 vehicles] ', '2024-07-26 23:01:52'),
(521, 2, '', '-', 'ionut2 logged in, money $25.988 / $0, 0 pp [0/2 vehicles] ', '2024-07-26 23:05:35'),
(522, 1, 'ionut', '-', 'ionut logged in, money $977.134.499 / $899,999,893, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 23:05:38'),
(523, 0, 'ionut2', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-26 23:05:41'),
(524, 1, 'ionut', '-', 'ionut logged in, money $977.134.499 / $899,999,893, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 23:06:07'),
(525, 1, 'ionut', '-', 'ionut logged in, money $977.134.499 / $899,999,893, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 23:08:47'),
(526, 2, '', '-', 'ionut2 logged in, money $25.988 / $0, 0 pp [0/2 vehicles] ', '2024-07-26 23:08:53'),
(527, 2, '', '-', 'ionut2 logged in, money $25.988 / $0, 0 pp [0/2 vehicles] ', '2024-07-26 23:14:08'),
(528, 1, 'ionut', '-', 'ionut logged in, money $977.134.499 / $899,999,893, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 23:14:10'),
(529, 1, 'ionut', '-', 'ionut logged in, money $977.134.499 / $899,999,893, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 23:16:27'),
(530, 2, '', '-', 'ionut2 logged in, money $25.988 / $0, 0 pp [0/2 vehicles] ', '2024-07-26 23:16:29'),
(531, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-26 23:16:30'),
(532, 2, '', '-', 'ionut2 logged in, money $25.988 / $0, 0 pp [0/2 vehicles] ', '2024-07-26 23:16:51'),
(533, 1, 'ionut', '-', 'ionut logged in, money $977.134.499 / $899,999,893, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 23:21:41'),
(534, 2, '', '-', 'ionut2 logged in, money $25.988 / $0, 0 pp [0/2 vehicles] ', '2024-07-26 23:21:43'),
(535, 1, 'ionut', '-', 'ionut logged in, money $977.134.499 / $899,999,893, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 23:25:46'),
(536, 2, '', '-', 'ionut2 logged in, money $25.988 / $0, 0 pp [0/2 vehicles] ', '2024-07-26 23:25:52'),
(537, 1, 'ionut', '-', 'ionut logged in, money $977.134.499 / $899,999,893, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 23:29:44'),
(538, 2, '', '-', 'ionut2 logged in, money $25.988 / $0, 0 pp [0/2 vehicles] ', '2024-07-26 23:29:47'),
(539, 1, 'ionut', '-', 'ionut logged in, money $977.134.499 / $899,999,893, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 23:32:38'),
(540, 2, '', '-', 'ionut2 logged in, money $25.988 / $0, 0 pp [0/2 vehicles] ', '2024-07-26 23:32:41'),
(541, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-26 23:34:50'),
(542, 0, '', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-26 23:36:37'),
(543, 2, '', '-', 'ionut2 logged in, money $25.988 / $0, 0 pp [0/2 vehicles] ', '2024-07-26 23:42:57'),
(544, 1, 'ionut', '-', 'ionut logged in, money $977.134.499 / $899,999,893, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 23:43:17'),
(545, 1, 'ionut', '-', 'ionut logged in, money $977.134.499 / $899,999,893, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 23:50:29'),
(546, 0, '', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-26 23:50:30'),
(547, 2, '', '-', 'ionut2 logged in, money $25.988 / $0, 0 pp [0/2 vehicles] ', '2024-07-26 23:50:34'),
(548, 1, 'ionut', '-', 'ionut logged in, money $977.134.499 / $899,999,893, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-26 23:51:00'),
(549, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-26 23:51:56'),
(550, 0, '', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-26 23:51:57'),
(551, 1, '', '-', 'ionut logged in, money $977.134.499 / $899,999,893, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-27 00:02:02'),
(552, 2, '', '-', 'ionut2 logged in, money $25.988 / $0, 0 pp [0/2 vehicles] ', '2024-07-27 00:02:15'),
(553, 1, '', '-', 'ionut logged in, money $977.134.499 / $899,999,893, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-27 00:22:23'),
(554, 0, '', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-27 00:22:40'),
(555, 1, '', '-', 'ionut logged in, money $977.134.499 / $899,999,893, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-27 00:39:27'),
(556, 2, '', '-', 'ionut2 logged in, money $25.988 / $0, 0 pp [0/2 vehicles] ', '2024-07-27 00:40:32'),
(557, 2, '', '-', 'ionut2 logged in, money $25.988 / $0, 0 pp [0/2 vehicles] ', '2024-07-27 00:43:37'),
(558, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-27 00:43:39'),
(559, 2, '', '-', 'ionut2 logged in, money $25.988 / $0, 0 pp [0/2 vehicles] ', '2024-07-27 00:44:09'),
(560, 1, 'ionut', '-', 'ionut logged in, money $977.134.499 / $899,999,893, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-27 00:44:22'),
(561, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-27 00:45:03'),
(562, 1, '', '-', 'ionut logged in, money $977.134.499 / $899,999,893, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-27 00:50:32'),
(563, 1, '', '-', 'ionut logged in, money $977.134.499 / $899,999,893, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-27 00:53:25'),
(564, 2, '', '-', 'ionut2 logged in, money $25.988 / $0, 0 pp [0/2 vehicles] ', '2024-07-27 00:53:43'),
(565, 2, '', '-', 'ionut2 logged in, money $25.988 / $0, 0 pp [0/2 vehicles] ', '2024-07-27 00:56:30'),
(566, 1, 'ionut', '-', 'ionut logged in, money $977.134.499 / $899,999,893, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-27 00:56:52'),
(567, 1, 'ionut', '-', 'ionut logged in, money $977.134.499 / $899,999,893, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-27 00:59:32'),
(568, 2, '', '-', 'ionut2 logged in, money $25.988 / $0, 0 pp [0/2 vehicles] ', '2024-07-27 00:59:37'),
(569, 2, '', '-', 'ionut2 logged in, money $25.988 / $0, 0 pp [0/2 vehicles] ', '2024-07-27 01:03:46'),
(570, 1, 'ionut', '-', 'ionut logged in, money $977.134.499 / $899,999,893, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-27 01:03:50'),
(571, 0, 'ionut2', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-27 01:03:52'),
(572, 1, 'ionut', '-', 'ionut logged in, money $977.134.499 / $899,999,893, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-27 01:04:08'),
(573, 2, '', '-', 'ionut2 logged in, money $25.988 / $0, 0 pp [0/2 vehicles] ', '2024-07-27 01:11:15'),
(574, 1, 'ionut', '-', 'ionut logged in, money $977.134.499 / $899,999,893, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-27 01:11:53'),
(575, 0, 'ionut2', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-27 01:11:54'),
(576, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-27 01:14:55'),
(577, 1, '', '-', 'ionut logged in, money $977.134.499 / $899,999,893, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-27 09:33:07'),
(578, 2, '', '-', 'ionut2 logged in, money $25.988 / $0, 0 pp [0/2 vehicles] ', '2024-07-27 09:33:30'),
(579, 1, '', '-', 'ionut logged in, money $977.134.499 / $899,999,893, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-27 09:38:17'),
(580, 2, '', '-', 'ionut2 logged in, money $25.988 / $0, 0 pp [0/2 vehicles] ', '2024-07-27 09:38:19'),
(581, 1, '', '-', 'ionut logged in, money $977.134.499 / $899,999,893, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-27 09:46:45'),
(582, 2, '', '-', 'ionut2 logged in, money $25.988 / $0, 0 pp [0/2 vehicles] ', '2024-07-27 09:46:48'),
(583, 1, '', '-', 'ionut logged in, money $977.134.499 / $899,999,893, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-27 09:48:46'),
(584, 0, '', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-27 09:48:48'),
(585, 2, '', '-', 'ionut2 logged in, money $25.988 / $0, 0 pp [0/2 vehicles] ', '2024-07-27 09:48:51'),
(586, 2, '', '-', 'ionut2 logged in, money $25.988 / $0, 0 pp [0/2 vehicles] ', '2024-07-27 09:50:47'),
(587, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-27 09:52:55'),
(588, 2, '', '-', 'ionut2 logged in, money $25.988 / $0, 0 pp [0/2 vehicles] ', '2024-07-27 10:06:35'),
(589, 1, 'ionut', '-', 'ionut logged in, money $977.134.499 / $899,999,893, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-27 10:06:52'),
(590, 0, 'ionut2', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-27 10:15:36'),
(591, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-27 10:16:36'),
(592, 1, '', '-', 'ionut logged in, money $977.134.499 / $899,999,893, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-27 10:16:47'),
(593, 2, '', '-', 'ionut2 logged in, money $25.988 / $0, 0 pp [0/2 vehicles] ', '2024-07-27 10:17:02'),
(594, 1, '', '-', 'ionut logged in, money $977.134.499 / $899,999,893, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-27 10:19:44'),
(595, 2, '', '-', 'ionut2 logged in, money $25.988 / $0, 0 pp [0/2 vehicles] ', '2024-07-27 10:19:47'),
(596, 2, '', '-', 'ionut2 logged in, money $25.988 / $0, 0 pp [0/2 vehicles] ', '2024-07-27 10:21:58'),
(597, 1, 'ionut2', '-', 'ionut logged in, money $977.134.499 / $899,999,893, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-27 10:22:02'),
(598, 2, '', '-', 'ionut2 logged out, money $25.988 / $0, 0 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-27 10:26:36'),
(599, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-27 10:26:39'),
(600, 2, '', '-', 'ionut2 logged in, money $25.988 / $0, 0 pp [0/2 vehicles] ', '2024-07-27 10:45:54'),
(601, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-27 10:46:14'),
(602, 2, '', '-', 'ionut2 logged in, money $25.988 / $0, 0 pp [0/2 vehicles] ', '2024-07-27 10:46:38'),
(603, 1, 'ionut', '-', 'ionut logged in, money $977.134.499 / $899,999,893, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-27 10:47:10'),
(604, 2, '', '-', 'ionut2 logged in, money $25.988 / $0, 0 pp [0/2 vehicles] ', '2024-07-27 10:57:23'),
(605, 1, 'ionut', '-', 'ionut logged in, money $977.134.499 / $899,999,893, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-27 10:57:26'),
(606, 1, 'ionut', '-', 'ionut logged in, money $977.134.499 / $899,999,893, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-27 10:58:52'),
(607, 2, '', '-', 'ionut2 logged in, money $25.988 / $0, 0 pp [0/2 vehicles] ', '2024-07-27 10:58:55'),
(608, 2, '', '-', 'ionut2 logged in, money $25.988 / $0, 0 pp [0/2 vehicles] ', '2024-07-27 11:02:02'),
(609, 1, 'ionut', '-', 'ionut logged in, money $977.134.499 / $899,999,893, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-27 11:02:22'),
(610, 2, '', '-', 'ionut2 logged in, money $25.988 / $0, 0 pp [0/2 vehicles] ', '2024-07-27 11:07:22'),
(611, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-27 11:07:23'),
(612, 1, 'ionut', '-', 'ionut logged in, money $977.134.499 / $899,999,893, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-27 11:07:24'),
(613, 2, '', '-', 'ionut2 logged in, money $25.988 / $0, 0 pp [0/2 vehicles] ', '2024-07-27 11:07:39'),
(614, 2, '', '-', 'ionut2 logged in, money $25.988 / $0, 0 pp [0/2 vehicles] ', '2024-07-27 11:08:00'),
(615, 1, 'ionut2', '-', 'ionut logged in, money $977.134.499 / $899,999,893, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-27 11:08:02'),
(616, 2, '', '-', 'ionut2 logged in, money $25.988 / $0, 0 pp [0/2 vehicles] ', '2024-07-27 11:09:32'),
(617, 2, '', '-', 'ionut2 logged in, money $25.988 / $0, 0 pp [0/2 vehicles] ', '2024-07-27 11:11:18'),
(618, 1, 'ionut', '-', 'ionut logged in, money $977.134.499 / $899,999,893, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-27 11:11:59'),
(619, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-27 11:13:08'),
(620, 0, '', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-27 11:13:10'),
(621, 1, '', '-', 'ionut logged in, money $977.134.499 / $899,999,893, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-27 11:13:51'),
(622, 1, '', '-', 'ionut logged in, money $977.134.499 / $899,999,893, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-27 11:15:38'),
(623, 1, '', '-', 'ionut logged in, money $977.134.499 / $899,999,893, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-27 11:17:33'),
(624, 0, 'ionut', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-27 11:35:00'),
(625, 1, '', '-', 'ionut logged in, money $977.134.499 / $899,999,893, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-27 11:40:38'),
(626, 0, '', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-27 11:40:39'),
(627, 1, '', '-', 'ionut logged in, money $977.134.499 / $899,999,893, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-27 11:40:51'),
(628, 2, '', '-', 'ionut2 logged in, money $25.988 / $0, 0 pp [0/2 vehicles] ', '2024-07-27 11:41:16'),
(629, 1, '', '-', 'ionut logged in, money $977.134.499 / $899,999,893, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-27 11:45:19'),
(630, 2, '', '-', 'ionut2 logged in, money $25.988 / $0, 0 pp [0/2 vehicles] ', '2024-07-27 11:45:32'),
(631, 1, '', '-', 'ionut logged in, money $977.134.499 / $899,999,893, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-27 11:52:09'),
(632, 2, '', '-', 'ionut2 logged in, money $25.988 / $0, 0 pp [0/2 vehicles] ', '2024-07-27 11:52:12'),
(633, 0, 'ionut', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-27 11:52:13'),
(634, 2, '', '-', 'ionut2 logged in, money $25.988 / $0, 0 pp [0/2 vehicles] ', '2024-07-27 11:52:25'),
(635, 0, 'ionut', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-27 11:53:14'),
(636, 0, '', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-27 11:53:15'),
(637, 1, '', '-', 'ionut logged in, money $977.134.499 / $899,999,893, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-27 12:09:19'),
(638, 2, '', '-', 'ionut2 logged in, money $25.988 / $0, 0 pp [0/2 vehicles] ', '2024-07-27 12:09:49'),
(639, 2, '', '-', 'ionut2 logged in, money $25.988 / $0, 0 pp [0/2 vehicles] ', '2024-07-27 12:14:41'),
(640, 1, 'ionut2', '-', 'ionut logged in, money $977.134.499 / $899,999,893, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-27 12:14:46'),
(641, 0, 'ionut', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-27 12:19:13'),
(642, 0, '', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-27 12:23:23'),
(643, 2, '', '-', 'ionut2 logged in, money $25.988 / $0, 0 pp [0/2 vehicles] ', '2024-07-27 12:24:00'),
(644, 1, 'ionut', '-', 'ionut logged in, money $977.134.499 / $899,999,893, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-27 12:24:13'),
(645, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-27 12:25:10'),
(646, 1, '', '-', 'ionut logged in, money $977.134.499 / $899,999,893, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-27 12:48:50'),
(647, 1, '', '-', 'ionut logged in, money $977.134.499 / $899,999,893, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-27 12:52:02'),
(648, 1, '', '-', 'ionut logged in, money $977.134.499 / $899,999,893, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-27 12:52:40'),
(649, 1, '', '-', 'ionut logged in, money $977.134.499 / $899,999,893, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-27 12:57:11'),
(650, 0, '', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-27 12:57:12'),
(651, 1, '', '-', 'ionut logged in, money $977.134.499 / $899,999,893, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-27 12:57:22'),
(652, 0, '', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-27 13:00:48'),
(653, 1, '', '-', 'ionut logged in, money $977.134.499 / $899,999,893, 970 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-27 13:07:22'),
(654, 0, '', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-27 13:10:52'),
(655, 1, '', '-', 'ionut logged in, money $977.132.499 / $899,999,893, 900 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-27 14:48:30'),
(656, 0, '', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-27 14:48:53'),
(657, 1, '', '-', 'ionut logged in, money $977.132.499 / $899,999,893, 650 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-27 14:49:27'),
(658, 1, '', '-', 'ionut logged in, money $977.132.499 / $899,999,893, 400 pp [2/3 vehicles] [car: 7] [car: 8] ', '2024-07-27 14:53:38'),
(659, 0, 'ionut', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-27 15:11:06'),
(660, 0, 'ionut', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-27 15:21:06'),
(661, 0, 'ionut', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-27 15:31:06'),
(662, 1, '', '-', 'ionut logged in, money $977.132.499 / $899,999,893, 400 pp [2/3 vehicles] [car: 7] [car: 8] ', '2024-07-27 15:32:17'),
(663, 0, '', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-27 15:32:19'),
(664, 1, '', '-', 'ionut logged in, money $977.132.499 / $899,999,893, 400 pp [2/3 vehicles] [car: 7] [car: 8] ', '2024-07-27 15:32:30'),
(665, 2, '', '-', 'ionut2 logged in, money $25.988 / $0, 0 pp [0/2 vehicles] ', '2024-07-27 15:33:04'),
(666, 0, 'ionut', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-27 15:33:33'),
(667, 2, '', '-', 'ionut2 logged in, money $25.988 / $0, 0 pp [0/2 vehicles] ', '2024-07-27 15:33:52'),
(668, 0, 'ionut', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-27 15:34:18'),
(669, 2, '', '-', 'ionut2 logged in, money $25.988 / $0, 0 pp [0/2 vehicles] ', '2024-07-27 15:34:33'),
(670, 0, 'ionut', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-27 15:35:24'),
(671, 2, '', '-', 'ionut2 logged in, money $25.988 / $0, 0 pp [0/2 vehicles] ', '2024-07-27 15:35:34'),
(672, 2, '', '-', 'ionut2 logged out, money $25.988 / $0, 0 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-27 15:35:44'),
(673, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-27 15:35:46'),
(674, 2, '', '-', 'ionut2 logged in, money $25.988 / $0, 0 pp [0/2 vehicles] ', '2024-07-27 15:35:56'),
(675, 1, 'ionut', '-', 'ionut logged in, money $977.132.499 / $899,999,893, 400 pp [2/3 vehicles] [car: 7] [car: 8] ', '2024-07-27 15:36:10'),
(676, 1, 'ionut', '-', 'ionut logged in, money $977.132.499 / $899,999,893, 400 pp [2/3 vehicles] [car: 7] [car: 8] ', '2024-07-27 15:39:15'),
(677, 2, '', '-', 'ionut2 logged in, money $25.988 / $0, 0 pp [0/2 vehicles] ', '2024-07-27 15:39:17'),
(678, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-27 15:40:22'),
(679, 0, '', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-27 15:40:40'),
(680, 1, '', '-', 'ionut logged in, money $977.132.499 / $899,999,893, 400 pp [2/3 vehicles] [car: 7] [car: 8] ', '2024-07-27 15:48:53'),
(681, 0, '', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-27 15:53:56'),
(682, 3, '', '-', 'ionutadv logged in, money $0 / $30,000, 0 pp [0/2 vehicles] ', '2024-07-27 18:49:33'),
(683, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-27 18:50:01'),
(684, 3, '', '-', 'ioneltalent logged in, money $20.000 / $30,000, 0 pp [0/2 vehicles] ', '2024-07-27 18:52:03'),
(685, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-27 18:52:19'),
(686, 3, '', '-', 'ioneltalent logged in, money $20.000 / $30,000, 0 pp [0/2 vehicles] ', '2024-07-27 22:22:15'),
(687, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-27 22:24:14'),
(688, 3, '', '-', 'ioneltalent logged in, money $20.000 / $30,000, 0 pp [0/2 vehicles] ', '2024-07-27 22:24:46'),
(689, 3, '', '-', 'ioneltalent logged in, money $20.000 / $30,000, 0 pp [0/2 vehicles] ', '2024-07-27 22:44:43'),
(690, 0, 'ioneltalent', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-27 22:46:43'),
(691, 3, '', '-', 'ioneltalent logged in, money $20.000 / $30,000, 0 pp [0/2 vehicles] ', '2024-07-27 22:48:11'),
(692, 3, '', '-', 'ioneltalent logged in, money $20.000 / $30,000, 0 pp [0/2 vehicles] ', '2024-07-27 22:50:11'),
(693, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-27 22:50:40'),
(694, 3, '', '-', 'ioneltalent logged in, money $20.000 / $30,000, 0 pp [0/2 vehicles] ', '2024-07-27 22:51:28'),
(695, 3, '', '-', 'ioneltalent logged in, money $20.000 / $30,000, 0 pp [0/2 vehicles] ', '2024-07-27 22:55:15'),
(696, 3, '', '-', 'ioneltalent logged in, money $20.000 / $30,000, 0 pp [0/2 vehicles] ', '2024-07-27 22:58:09'),
(697, 0, 'ioneltalent', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-27 23:15:37'),
(698, 0, 'ioneltalent', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-27 23:25:37'),
(699, 0, 'ioneltalent', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-27 23:35:37'),
(700, 0, 'ioneltalent', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-27 23:45:37'),
(701, 0, 'ioneltalent', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-27 23:55:37'),
(702, 3, '', '-', 'ioneltalent logged in, money $720.000 / $30,000, 0 pp [0/2 vehicles] ', '2024-07-28 00:52:05'),
(703, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-28 00:55:37'),
(704, 3, '', '-', 'ioneltalent logged in, money $720.000 / $30,000, 0 pp [0/2 vehicles] ', '2024-07-28 01:09:26'),
(705, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-28 01:11:54'),
(706, 3, '', '-', 'ioneltalent logged in, money $708.000 / $30,000, 0 pp [0/2 vehicles] ', '2024-07-28 01:16:37'),
(707, 3, '', '-', 'ioneltalent logged in, money $708.000 / $30,000, 0 pp [0/2 vehicles] ', '2024-07-28 17:51:07'),
(708, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-28 17:55:26'),
(709, 3, '', '-', 'ioneltalent logged in, money $708.000 / $30,000, 0 pp [0/2 vehicles] ', '2024-07-28 18:37:29'),
(710, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-28 18:38:52'),
(711, 3, '', '-', 'ioneltalent logged in, money $708.000 / $30,000, 0 pp [0/2 vehicles] ', '2024-07-28 18:48:53'),
(712, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-28 18:50:56'),
(713, 3, '', '-', 'ioneltalent logged in, money $708.000 / $30,000, 0 pp [0/2 vehicles] ', '2024-07-28 18:51:35'),
(714, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-28 18:52:34'),
(715, 3, '', '-', 'ioneltalent logged in, money $708.000 / $30,000, 0 pp [0/2 vehicles] ', '2024-07-28 19:03:34'),
(716, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-28 19:05:22'),
(717, 3, '', '-', 'ioneltalent logged in, money $708.000 / $30,000, 0 pp [0/2 vehicles] ', '2024-07-28 19:17:08'),
(718, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-28 19:18:14'),
(719, 3, '', '-', 'ioneltalent logged in, money $708.000 / $30,000, 0 pp [0/2 vehicles] ', '2024-07-28 19:36:37'),
(720, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-28 19:38:43'),
(721, 3, '', '-', 'ioneltalent logged in, money $769.019 / $30,000, 0 pp [0/2 vehicles] ', '2024-07-28 19:54:13'),
(722, 3, '', '-', 'ioneltalent logged in, money $768.519 / $30,000, 0 pp [0/2 vehicles] ', '2024-07-28 19:56:51'),
(723, 3, '', '-', 'ioneltalent logged in, money $768.019 / $30,000, 0 pp [0/2 vehicles] ', '2024-07-28 19:58:12'),
(724, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-28 19:58:57'),
(725, 3, '', '-', 'ioneltalent logged in, money $767.519 / $30,000, 0 pp [0/2 vehicles] ', '2024-07-28 20:00:47'),
(726, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-28 20:02:25'),
(727, 3, '', '-', 'ioneltalent logged in, money $840.387 / $30,000, 0 pp [0/2 vehicles] ', '2024-07-28 20:03:26'),
(728, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-28 20:04:12'),
(729, 3, '', '-', 'ioneltalent logged in, money $840.387 / $30,000, 0 pp [0/2 vehicles] ', '2024-07-28 20:13:57'),
(730, 3, '', '-', 'ioneltalent logged in, money $839.887 / $30,000, 0 pp [0/2 vehicles] ', '2024-07-28 20:15:55'),
(731, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-28 20:15:56'),
(732, 3, '', '-', 'ioneltalent logged in, money $839.887 / $30,000, 0 pp [0/2 vehicles] ', '2024-07-28 20:16:41'),
(733, 3, '', '-', 'ioneltalent logged in, money $839.387 / $30,000, 0 pp [0/2 vehicles] ', '2024-07-28 20:23:49'),
(734, 3, '', '-', 'ioneltalent logged in, money $838.887 / $30,000, 0 pp [0/2 vehicles] ', '2024-07-28 20:25:49'),
(735, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-28 20:28:17'),
(736, 3, '', '-', 'ioneltalent logged in, money $898.690 / $30,000, 0 pp [0/2 vehicles] ', '2024-07-28 20:48:44'),
(737, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-28 20:52:40'),
(738, 3, '', '-', 'ioneltalent logged in, money $966.002 / $30,000, 0 pp [0/2 vehicles] ', '2024-07-28 20:53:17'),
(739, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-28 20:54:14'),
(740, 3, '', '-', 'ioneltalent logged in, money $966.002 / $30,000, 0 pp [0/2 vehicles] ', '2024-07-28 21:08:59'),
(741, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-28 21:10:25'),
(742, 3, '', '-', 'ioneltalent logged in, money $1.027.181 / $30,000, 0 pp [0/2 vehicles] ', '2024-07-28 21:11:41'),
(743, 3, '', '-', 'ioneltalent logged in, money $1.079.192 / $30,000, 0 pp [0/2 vehicles] ', '2024-07-28 21:15:03'),
(744, 3, '', '-', 'ioneltalent logged in, money $1.156.206 / $30,000, 0 pp [0/2 vehicles] ', '2024-07-28 21:21:58'),
(745, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-28 21:23:13'),
(746, 3, '', '-', 'ioneltalent logged in, money $1.155.706 / $30,000, 0 pp [0/2 vehicles] ', '2024-07-28 21:23:36'),
(747, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-28 21:24:41'),
(748, 3, '', '-', 'ioneltalent logged in, money $1.155.706 / $30,000, 0 pp [0/2 vehicles] ', '2024-07-28 21:50:10'),
(749, 1, 'ionut', '-', 'ionut logged in, money $977.132.499 / $899,999,893, 400 pp [2/3 vehicles] [car: 7] [car: 8] ', '2024-07-28 21:50:34'),
(750, 3, '', '-', 'ioneltalent logged in, money $1.155.706 / $30,000, 0 pp [0/2 vehicles] ', '2024-07-28 21:52:25'),
(751, 1, 'ionut', '-', 'ionut logged in, money $977.132.499 / $899,999,893, 400 pp [2/3 vehicles] [car: 7] [car: 8] ', '2024-07-28 21:52:28'),
(752, 0, 'ioneltalent', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-28 21:53:32'),
(753, 3, '', '-', 'ioneltalent logged in, money $1.155.706 / $30,000, 0 pp [0/2 vehicles] ', '2024-07-28 21:55:10'),
(754, 1, 'ionut', '-', 'ionut logged in, money $977.132.499 / $899,999,893, 400 pp [2/3 vehicles] [car: 7] [car: 8] ', '2024-07-28 21:55:24'),
(755, 0, 'ioneltalent', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-28 21:56:48'),
(756, 1, 'ionut', '-', 'ionut logged in, money $977.132.499 / $899,999,893, 400 pp [2/3 vehicles] [car: 7] [car: 8] ', '2024-07-28 21:57:09'),
(757, 3, '', '-', 'ioneltalent logged in, money $1.155.706 / $30,000, 0 pp [0/2 vehicles] ', '2024-07-28 21:58:48'),
(758, 1, 'ionut', '-', 'ionut logged in, money $977.132.499 / $899,999,893, 400 pp [2/3 vehicles] [car: 7] [car: 8] ', '2024-07-28 21:58:54'),
(759, 0, 'ioneltalent', '-', ' logged out, money $0 / $0, 0 pp [2/0 vehicles] [car: 7] [car: 8] ', '2024-07-28 21:59:05'),
(760, 1, '', '-', 'ionut logged in, money $977.132.499 / $899,999,893, 400 pp [2/3 vehicles] [car: 7] [car: 8] ', '2024-07-28 22:14:48'),
(761, 3, '', '-', 'ioneltalent logged in, money $1.155.706 / $30,000, 0 pp [0/2 vehicles] ', '2024-07-28 22:15:07'),
(762, 1, '', '-', 'ionut logged in, money $977.135.499 / $899,999,893, 400 pp [2/3 vehicles] [car: 7] [car: 8] ', '2024-07-28 22:18:27'),
(763, 3, '', '-', 'ioneltalent logged in, money $1.155.706 / $30,000, 0 pp [0/2 vehicles] ', '2024-07-28 22:18:33'),
(764, 3, '', '-', 'ioneltalent logged out, money $1.155.706 / $30,000, 0 pp [2/2 vehicles] [car: 7] [car: 8] ', '2024-07-28 22:21:11'),
(765, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-28 22:21:43'),
(766, 3, '', '-', 'ioneltalent logged in, money $1.155.706 / $30,000, 0 pp [0/2 vehicles] ', '2024-07-28 22:24:51'),
(767, 3, '', '-', 'ioneltalent logged in, money $1.155.206 / $30,000, 0 pp [0/2 vehicles] ', '2024-07-28 22:25:53'),
(768, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-28 22:28:03'),
(769, 3, '', '-', 'ioneltalent logged in, money $1.154.706 / $30,000, 0 pp [0/2 vehicles] ', '2024-07-28 22:30:51'),
(770, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-28 22:38:30'),
(771, 3, '', '-', 'ioneltalent logged in, money $1.153.710 / $29,996, 0 pp [0/2 vehicles] ', '2024-07-29 16:31:42'),
(772, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-29 16:34:38'),
(773, 3, '', '-', 'ioneltalent logged in, money $1.233.205 / $29,996, 0 pp [0/2 vehicles] ', '2024-07-29 16:44:42'),
(774, 3, '', '-', 'ioneltalent logged in, money $1.232.705 / $29,996, 0 pp [0/2 vehicles] ', '2024-07-29 16:56:01'),
(775, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-29 16:57:14'),
(776, 3, '', '-', 'ioneltalent logged in, money $1.232.205 / $29,996, 0 pp [0/2 vehicles] ', '2024-07-29 17:38:25'),
(777, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-29 17:40:43'),
(778, 3, '', '-', 'ioneltalent logged in, money $1.231.705 / $29,996, 0 pp [0/2 vehicles] ', '2024-07-29 17:43:15'),
(779, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-29 17:47:08'),
(780, 3, '', '-', 'asser logged in, money $1.308.457 / $29,996, 0 pp [0/2 vehicles] ', '2024-07-29 17:47:23'),
(781, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-29 17:48:11'),
(782, 3, '', '-', 'asser logged in, money $1.308.457 / $29,996, 0 pp [0/2 vehicles] ', '2024-07-29 19:46:54'),
(783, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-29 19:49:43'),
(784, 3, '', '-', 'asser logged in, money $1.307.957 / $29,996, 0 pp [0/2 vehicles] ', '2024-07-29 20:22:26'),
(785, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-29 20:30:06'),
(786, 3, '', '-', 'asser logged in, money $1.382.121 / $29,996, 0 pp [0/2 vehicles] ', '2024-07-29 21:00:31'),
(787, 3, '', '-', 'asser logged in, money $1.381.621 / $29,996, 0 pp [0/2 vehicles] ', '2024-07-29 21:04:50'),
(788, 3, '', '-', 'asser logged in, money $1.457.915 / $29,996, 0 pp [0/2 vehicles] ', '2024-07-29 21:19:00'),
(789, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-29 21:21:57'),
(790, 3, '', '-', 'asser logged in, money $1.457.415 / $29,996, 0 pp [0/2 vehicles] ', '2024-07-29 21:22:22'),
(791, 3, '', '-', 'asser logged in, money $1.456.915 / $29,996, 0 pp [0/2 vehicles] ', '2024-07-29 21:25:20'),
(792, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-29 21:27:24'),
(793, 3, '', '-', 'asser logged in, money $1.456.415 / $29,996, 0 pp [0/2 vehicles] ', '2024-07-29 21:27:48'),
(794, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-29 21:30:32'),
(795, 3, '', '-', 'asser logged in, money $1.529.421 / $29,996, 0 pp [0/2 vehicles] ', '2024-07-29 21:31:02'),
(796, 0, 'asser', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-29 21:48:16'),
(797, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-29 21:50:07'),
(798, 3, '', '-', 'asser logged in, money $1.528.921 / $29,996, 0 pp [0/2 vehicles] ', '2024-07-29 21:53:30'),
(799, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-29 21:56:01'),
(800, 3, '', '-', 'asser logged in, money $1.528.421 / $29,996, 0 pp [0/2 vehicles] ', '2024-07-29 21:56:38'),
(801, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-29 21:59:00'),
(802, 3, '', '-', 'asser logged in, money $1.593.031 / $29,996, 0 pp [0/2 vehicles] ', '2024-07-29 22:00:31'),
(803, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-29 22:02:51'),
(804, 3, '', '-', 'asser logged in, money $1.654.661 / $29,996, 0 pp [0/2 vehicles] ', '2024-07-29 22:11:35'),
(805, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-29 22:12:28'),
(806, 3, '', '-', 'asser logged in, money $1.654.661 / $29,996, 9750 pp [0/3 vehicles] ', '2024-07-29 22:12:41'),
(807, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-29 22:13:12'),
(808, 3, '', '-', 'ionutadv logged in, money $1.654.661 / $29,996, 9600 pp [0/3 vehicles] ', '2024-07-29 22:13:38'),
(809, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-29 22:15:40'),
(810, 3, '', '-', 'ionutadv logged in, money $1.654.161 / $29,996, 9600 pp [0/3 vehicles] ', '2024-07-29 22:17:58'),
(811, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-29 22:18:53'),
(812, 3, '', '-', 'ionutadv logged in, money $1.653.661 / $29,996, 9600 pp [0/3 vehicles] ', '2024-07-29 22:20:06'),
(813, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-29 22:23:00'),
(814, 3, '', '-', 'ionutadv logged in, money $1.653.161 / $29,996, 9600 pp [0/3 vehicles] ', '2024-07-29 22:23:28'),
(815, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-29 22:25:18'),
(816, 3, '', '-', 'ionutadv logged in, money $1.738.075 / $29,996, 9600 pp [0/3 vehicles] ', '2024-07-29 22:26:46'),
(817, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-29 22:28:16'),
(818, 3, '', '-', 'ionutadv logged in, money $1.813.669 / $29,996, 9600 pp [0/3 vehicles] ', '2024-07-29 22:29:21'),
(819, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-29 22:31:08'),
(820, 3, '', '-', 'ionutadv logged in, money $1.883.667 / $29,996, 9600 pp [0/3 vehicles] ', '2024-07-29 22:33:38'),
(821, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-29 22:35:42'),
(822, 3, '', '-', 'ionutadv logged in, money $1.959.411 / $29,996, 9600 pp [0/3 vehicles] ', '2024-07-29 22:38:35'),
(823, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-29 22:41:24'),
(824, 3, '', '-', 'ionutadv logged in, money $2.035.907 / $29,996, 9600 pp [0/3 vehicles] ', '2024-07-29 22:44:03'),
(825, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-29 22:46:13'),
(826, 3, '', '-', 'ionutadv logged in, money $2.112.786 / $29,996, 9600 pp [0/3 vehicles] ', '2024-07-29 22:48:39'),
(827, 3, '', '-', 'ionutadv logged in, money $2.193.309 / $29,996, 9600 pp [0/3 vehicles] ', '2024-07-29 22:53:07'),
(828, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-29 22:54:31'),
(829, 3, '', '-', 'ionutadv logged in, money $2.192.809 / $29,996, 9600 pp [0/3 vehicles] ', '2024-07-29 22:55:00'),
(830, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-29 22:55:54'),
(831, 3, '', '-', 'ionutadv logged in, money $2.192.309 / $29,996, 9600 pp [0/3 vehicles] ', '2024-07-29 23:00:59'),
(832, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-29 23:03:07'),
(833, 3, '', '-', 'ionutadv logged in, money $2.191.809 / $29,996, 9600 pp [0/3 vehicles] ', '2024-07-29 23:03:57'),
(834, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-29 23:05:01'),
(835, 3, '', '-', 'ionutadv logged in, money $2.191.309 / $29,996, 9600 pp [0/3 vehicles] ', '2024-07-29 23:20:14'),
(836, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-29 23:20:41'),
(837, 3, '', '-', 'ionutadv logged in, money $2.191.309 / $29,996, 9600 pp [0/3 vehicles] ', '2024-07-29 23:21:07'),
(838, 2, '', '-', 'ionut2 logged in, money $25.988 / $0, 0 pp [0/2 vehicles] ', '2024-07-29 23:21:24'),
(839, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-29 23:22:00'),
(840, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-29 23:22:09'),
(841, 4, '', '-', 'asser logged in, money $0 / $30,000, 0 pp [0/2 vehicles] ', '2024-07-31 23:08:10'),
(842, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-31 23:09:31'),
(843, 4, '', '-', 'asser logged in, money $20.000 / $30,000, 0 pp [0/2 vehicles] ', '2024-07-31 23:11:50'),
(844, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-07-31 23:13:50'),
(845, 3, '', '-', 'ionutadv logged in, money $2.191.309 / $29,996, 9600 pp [0/3 vehicles] ', '2024-08-04 19:16:44'),
(846, 5, '', '-', 'Ramos logged in, money $0 / $30,000, 0 pp [0/2 vehicles] ', '2024-08-04 19:17:04'),
(847, 5, '', '', '[trade] Ramos [user:5] a oferit 1 cocaine lui ionutadv ', '2024-08-04 19:18:32'),
(848, 3, '', '', '[trade] ionutadv [user:3] a primit 1 cocaine de la Ramos ', '2024-08-04 19:18:32'),
(849, 3, '', '', '[trade] ionutadv [user:3] a oferit 1 mushrooms lui Ramos ', '2024-08-04 19:18:33'),
(850, 5, '', '', '[trade] Ramos [user:5] a primit 1 mushrooms de la ionutadv ', '2024-08-04 19:18:33'),
(851, 0, 'ionutadv', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-08-04 19:33:43'),
(852, 0, 'ionutadv', '-', ' logged out, money $0 / $0, 0 pp [1/0 vehicles] [car: 10] ', '2024-08-04 19:33:43'),
(853, 6, '', '-', 'FRANCOKL888 logged in, money $0 / $30,000, 0 pp [0/2 vehicles] ', '2024-08-04 19:34:13'),
(854, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-08-04 19:34:39'),
(855, 0, '', '-', ' logged out, money $0 / $0, 0 pp [0/0 vehicles] ', '2024-08-04 19:35:22'),
(856, 0, '', '-', ' logged out, money $0 / $0, 0 pp [1/0 vehicles] [car: 10] ', '2024-08-04 19:43:43'),
(857, 0, '', '-', ' logged out, money $0 / $0, 0 pp [1/0 vehicles] [car: 10] ', '2024-08-04 19:53:43'),
(858, 0, '', '-', ' logged out, money $0 / $0, 0 pp [1/0 vehicles] [car: 10] ', '2024-08-04 19:59:58');

-- --------------------------------------------------------

--
-- Table structure for table `server_anticheats`
--

CREATE TABLE `server_anticheats` (
  `ID` int(11) NOT NULL,
  `Name` varchar(24) NOT NULL,
  `Active` int(11) NOT NULL,
  `Sanctions` int(11) NOT NULL,
  `DaysBan` int(11) NOT NULL DEFAULT '0'
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `server_job_goal`
--

CREATE TABLE `server_job_goal` (
  `ID` int(11) NOT NULL,
  `money` int(11) NOT NULL,
  `totalmoney` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- --------------------------------------------------------

--
-- Table structure for table `server_market`
--

CREATE TABLE `server_market` (
  `marketID` int(11) NOT NULL,
  `marketVehicleID` int(11) NOT NULL,
  `marketVehicleOwner` int(11) NOT NULL,
  `marketPrice` int(11) NOT NULL,
  `marketListDate` int(11) NOT NULL,
  `marketVehicleOwnerName` varchar(250) NOT NULL DEFAULT '-1'
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `server_market`
--

INSERT INTO `server_market` (`marketID`, `marketVehicleID`, `marketVehicleOwner`, `marketPrice`, `marketListDate`, `marketVehicleOwnerName`) VALUES
(3, 9, 1, 2000000, 1721856216, 'ionut');

-- --------------------------------------------------------

--
-- Table structure for table `server_safezones`
--

CREATE TABLE `server_safezones` (
  `safezoneKey` int(11) NOT NULL,
  `safezoneminX` float NOT NULL,
  `safezoneminY` float NOT NULL,
  `safezonemaxX` float NOT NULL,
  `safezonemaxY` float NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Dumping data for table `server_safezones`
--

INSERT INTO `server_safezones` (`safezoneKey`, `safezoneminX`, `safezoneminY`, `safezonemaxX`, `safezonemaxY`) VALUES
(1, 433, -1592.5, 561, -1403.5),
(2, 1050, -1577.5, 1217, -1362.5);

-- --------------------------------------------------------

--
-- Table structure for table `server_users_wars`
--

CREATE TABLE `server_users_wars` (
  `ID` int(11) NOT NULL,
  `warID` int(11) NOT NULL,
  `warPlayerID` int(11) NOT NULL,
  `warFactionID` int(11) NOT NULL,
  `warPlayerKills` int(11) NOT NULL DEFAULT '0',
  `warPlayerDeaths` int(11) NOT NULL DEFAULT '0',
  `warPlayerKD` float NOT NULL DEFAULT '0',
  `warPlayerTurfTime` int(11) NOT NULL DEFAULT '0',
  `warPlayerAssists` int(11) NOT NULL DEFAULT '0'
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `server_wars`
--

CREATE TABLE `server_wars` (
  `ID` int(11) NOT NULL,
  `warAttackerFaction` int(11) NOT NULL,
  `warDefenderFaction` int(11) NOT NULL,
  `warAttackerScore` int(11) NOT NULL DEFAULT '0',
  `warDefenderScore` int(11) NOT NULL DEFAULT '0',
  `warStartTime` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `warEndTime` timestamp NULL DEFAULT NULL,
  `warTurfID` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `special_codes`
--

CREATE TABLE `special_codes` (
  `ID` int(11) NOT NULL,
  `AdminName` varchar(250) NOT NULL DEFAULT 'NONE',
  `Code` varchar(250) NOT NULL DEFAULT '0',
  `Online` int(11) NOT NULL,
  `Cash` int(11) NOT NULL,
  `Respect` int(11) NOT NULL,
  `Diamonds` int(11) NOT NULL,
  `UsedBy` varchar(245) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `users`
--

CREATE TABLE `users` (
  `ID` int(11) NOT NULL,
  `Name` varchar(32) DEFAULT NULL,
  `Password` varchar(129) DEFAULT NULL,
  `Email` varchar(32) NOT NULL DEFAULT 'email@yahoo.com',
  `Gender` int(11) DEFAULT NULL,
  `Language` int(11) DEFAULT NULL,
  `Hours` float DEFAULT '0',
  `Money` int(11) NOT NULL DEFAULT '0',
  `BankLY` int(11) DEFAULT NULL,
  `Bank` int(32) NOT NULL DEFAULT '30000',
  `PremiumPoints` int(11) NOT NULL DEFAULT '0',
  `Admin` int(11) NOT NULL DEFAULT '0',
  `Helper` int(11) NOT NULL DEFAULT '0',
  `SpawnLocation` int(11) NOT NULL DEFAULT '0',
  `Experience` int(11) NOT NULL DEFAULT '0',
  `Level` int(11) NOT NULL DEFAULT '1',
  `Tutorial` int(11) NOT NULL DEFAULT '0',
  `ChatColor` int(11) NOT NULL DEFAULT '0',
  `Clan` int(11) NOT NULL DEFAULT '0',
  `ClanRank` int(11) NOT NULL DEFAULT '0',
  `House` int(11) NOT NULL DEFAULT '0',
  `Apartament` int(11) NOT NULL DEFAULT '0',
  `Skin` int(11) NOT NULL DEFAULT '36',
  `acc_skins` varchar(1024) NOT NULL DEFAULT '2|12|27|36|37|56|78|79|88|151|157|',
  `acc_skins_type` varchar(1024) NOT NULL DEFAULT '0|0|0|0|0|0|0|0|0|0|0|',
  `ClanTagPos` int(11) NOT NULL DEFAULT '0',
  `ClanDays` int(11) NOT NULL DEFAULT '0',
  `ClanMoney` int(11) NOT NULL DEFAULT '0',
  `ClanWarns` int(11) NOT NULL DEFAULT '0',
  `Faction` int(11) NOT NULL DEFAULT '0',
  `FactionRank` int(11) NOT NULL DEFAULT '0',
  `FactionDays` int(11) NOT NULL DEFAULT '0',
  `FactionWarns` int(11) DEFAULT '0',
  `FactionPunish` int(11) NOT NULL DEFAULT '0',
  `RaportTime` int(11) NOT NULL DEFAULT '0',
  `RaportScore` varchar(64) NOT NULL DEFAULT '0 0 0 0 0 0',
  `RaportIncompleted` int(11) NOT NULL DEFAULT '0',
  `Business` int(11) NOT NULL DEFAULT '0',
  `wantedLevel` int(11) NOT NULL DEFAULT '0',
  `wantedTime` int(11) NOT NULL DEFAULT '-1',
  `wantedCrime1` varchar(32) DEFAULT 'Fara Crima',
  `wantedCrime2` varchar(32) DEFAULT 'Fara Crima',
  `wantedCrime3` varchar(32) DEFAULT 'Fara Crima',
  `wantedCrime4` varchar(32) DEFAULT 'Fara Crima',
  `wantedCrime5` varchar(32) DEFAULT 'Fara Crima',
  `wantedCrime6` varchar(32) DEFAULT 'Fara Crima',
  `fightStyle` int(11) NOT NULL DEFAULT '4',
  `MuscleReps` int(11) NOT NULL DEFAULT '0',
  `MuscleLevel` int(11) NOT NULL DEFAULT '1',
  `TotalReps` int(11) NOT NULL DEFAULT '0',
  `gasCan` int(11) NOT NULL DEFAULT '0',
  `Phone` int(11) NOT NULL DEFAULT '0',
  `PhoneBook` int(11) NOT NULL DEFAULT '0',
  `PhoneCredit` int(11) NOT NULL DEFAULT '0',
  `WalkieTalkie` int(11) NOT NULL DEFAULT '0',
  `WTFreq` int(11) NOT NULL DEFAULT '0',
  `carSlots` int(11) NOT NULL DEFAULT '2',
  `Quest` varchar(16) NOT NULL DEFAULT '0 0 0',
  `QuestProgress` varchar(16) NOT NULL DEFAULT '0 0 0',
  `QuestNeed` varchar(16) NOT NULL DEFAULT '0 0 0',
  `QuestTime` int(11) NOT NULL DEFAULT '0',
  `Jail` int(11) NOT NULL DEFAULT '0',
  `JailTime` int(11) NOT NULL DEFAULT '0',
  `DrivingLicence` int(11) NOT NULL DEFAULT '0',
  `WeaponLicence` int(11) NOT NULL DEFAULT '0',
  `FlyLicence` int(11) NOT NULL DEFAULT '0',
  `SailingLicence` int(11) NOT NULL DEFAULT '0',
  `lastOnlineTime` int(11) NOT NULL DEFAULT '0',
  `HudOptions` varchar(64) NOT NULL DEFAULT '0 0 0 0 0 1 1 0 1 1 0 0 1',
  `Job` int(11) NOT NULL DEFAULT '0',
  `JobSkill` varchar(128) NOT NULL DEFAULT '1 1 1 1 1 1 1 1 1 1 1 1 1',
  `JobTimes` varchar(128) NOT NULL DEFAULT '0 0 0 0 0 0 0 0 0 0 0 0 0',
  `JobRem` varchar(128) NOT NULL DEFAULT '0 0 0 0 0 0 0 0 0 0 0 0 0',
  `OrderWeapons` varchar(16) NOT NULL DEFAULT '0 0 0',
  `Drugs` int(11) NOT NULL DEFAULT '0',
  `PaintKills` int(11) NOT NULL DEFAULT '0',
  `PaintDeaths` int(11) NOT NULL DEFAULT '0',
  `Cases` int(11) NOT NULL DEFAULT '0',
  `CaseKeys` int(11) NOT NULL DEFAULT '0',
  `Materials` int(11) NOT NULL DEFAULT '0',
  `PremiumAccount` int(11) NOT NULL DEFAULT '0',
  `HiddenColor` int(11) NOT NULL DEFAULT '0',
  `iPhone` int(11) NOT NULL DEFAULT '0',
  `PetLevel` int(11) NOT NULL DEFAULT '1',
  `PetSkin` int(11) NOT NULL DEFAULT '19079',
  `Pet` int(11) NOT NULL DEFAULT '0',
  `PetPoints` int(11) NOT NULL DEFAULT '0',
  `PetName` varchar(32) NOT NULL DEFAULT 'pet',
  `PetStatus` int(11) NOT NULL DEFAULT '0',
  `ReportMuted` int(5) NOT NULL DEFAULT '0',
  `NMuted` int(11) NOT NULL DEFAULT '0',
  `BlockedAcc` int(11) NOT NULL DEFAULT '0',
  `PetHealth` int(11) NOT NULL DEFAULT '100',
  `Status` int(11) NOT NULL DEFAULT '0',
  `DailyLogin` int(11) NOT NULL DEFAULT '0',
  `LastLogin` varchar(32) NOT NULL DEFAULT 'None',
  `private` int(11) DEFAULT '0',
  `GrowWaitTime` int(4) NOT NULL DEFAULT '-1',
  `Warn` int(11) NOT NULL DEFAULT '0',
  `Mute` int(11) DEFAULT '0',
  `QuestionMute` int(11) NOT NULL DEFAULT '0',
  `ReportMute` int(11) NOT NULL DEFAULT '0',
  `UnblockToken` varchar(245) DEFAULT NULL,
  `FishingRod` int(11) NOT NULL DEFAULT '0',
  `FishHealth` int(3) NOT NULL DEFAULT '0',
  `NormalBait` int(11) NOT NULL DEFAULT '0',
  `LiveBait` int(11) NOT NULL DEFAULT '0',
  `SturdyReel` int(11) NOT NULL DEFAULT '0',
  `QuickHook` int(11) NOT NULL DEFAULT '0',
  `BountyBag` int(11) NOT NULL DEFAULT '0',
  `PlayersHelped` int(11) NOT NULL DEFAULT '0',
  `AdminWarns` int(11) NOT NULL DEFAULT '0',
  `HelperWarns` int(11) NOT NULL DEFAULT '0',
  `robPoints` int(11) NOT NULL DEFAULT '0',
  `PaydayTime` int(4) NOT NULL DEFAULT '3600',
  `PayDay` int(20) NOT NULL DEFAULT '0',
  `questSpecial` varchar(52) NOT NULL DEFAULT '0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0',
  `completedToday` varchar(128) NOT NULL DEFAULT '0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0',
  `questTimes` int(3) NOT NULL DEFAULT '0',
  `Pin` int(11) NOT NULL DEFAULT '-1',
  `warPlayerID` int(11) NOT NULL DEFAULT '0',
  `Theme` int(11) NOT NULL DEFAULT '0',
  `JailTime2` int(11) NOT NULL,
  `JailType` int(11) NOT NULL DEFAULT '0',
  `playerTutorial` int(11) NOT NULL DEFAULT '0',
  `MoneyStore` int(11) NOT NULL DEFAULT '0',
  `QuestsUnlocked` int(11) NOT NULL DEFAULT '0',
  `gpci` varchar(200) NOT NULL DEFAULT '-1',
  `hwid` varchar(200) NOT NULL DEFAULT '-1',
  `MiningPoints` int(11) NOT NULL DEFAULT '0',
  `Cocaine` int(11) NOT NULL,
  `Mushrooms` int(11) NOT NULL,
  `Addiction` int(11) NOT NULL,
  `PremiumExpire` int(11) NOT NULL DEFAULT '-1',
  `PremiumCount` int(11) NOT NULL DEFAULT '0',
  `AdminColor` int(11) NOT NULL DEFAULT '0',
  `PremiumColor` int(11) NOT NULL DEFAULT '0',
  `BankDeelay` int(11) NOT NULL DEFAULT '0'
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Dumping data for table `users`
--

INSERT INTO `users` (`ID`, `Name`, `Password`, `Email`, `Gender`, `Language`, `Hours`, `Money`, `BankLY`, `Bank`, `PremiumPoints`, `Admin`, `Helper`, `SpawnLocation`, `Experience`, `Level`, `Tutorial`, `ChatColor`, `Clan`, `ClanRank`, `House`, `Apartament`, `Skin`, `acc_skins`, `acc_skins_type`, `ClanTagPos`, `ClanDays`, `ClanMoney`, `ClanWarns`, `Faction`, `FactionRank`, `FactionDays`, `FactionWarns`, `FactionPunish`, `RaportTime`, `RaportScore`, `RaportIncompleted`, `Business`, `wantedLevel`, `wantedTime`, `wantedCrime1`, `wantedCrime2`, `wantedCrime3`, `wantedCrime4`, `wantedCrime5`, `wantedCrime6`, `fightStyle`, `MuscleReps`, `MuscleLevel`, `TotalReps`, `gasCan`, `Phone`, `PhoneBook`, `PhoneCredit`, `WalkieTalkie`, `WTFreq`, `carSlots`, `Quest`, `QuestProgress`, `QuestNeed`, `QuestTime`, `Jail`, `JailTime`, `DrivingLicence`, `WeaponLicence`, `FlyLicence`, `SailingLicence`, `lastOnlineTime`, `HudOptions`, `Job`, `JobSkill`, `JobTimes`, `JobRem`, `OrderWeapons`, `Drugs`, `PaintKills`, `PaintDeaths`, `Cases`, `CaseKeys`, `Materials`, `PremiumAccount`, `HiddenColor`, `iPhone`, `PetLevel`, `PetSkin`, `Pet`, `PetPoints`, `PetName`, `PetStatus`, `ReportMuted`, `NMuted`, `BlockedAcc`, `PetHealth`, `Status`, `DailyLogin`, `LastLogin`, `private`, `GrowWaitTime`, `Warn`, `Mute`, `QuestionMute`, `ReportMute`, `UnblockToken`, `FishingRod`, `FishHealth`, `NormalBait`, `LiveBait`, `SturdyReel`, `QuickHook`, `BountyBag`, `PlayersHelped`, `AdminWarns`, `HelperWarns`, `robPoints`, `PaydayTime`, `PayDay`, `questSpecial`, `completedToday`, `questTimes`, `Pin`, `warPlayerID`, `Theme`, `JailTime2`, `JailType`, `playerTutorial`, `MoneyStore`, `QuestsUnlocked`, `gpci`, `hwid`, `MiningPoints`, `Cocaine`, `Mushrooms`, `Addiction`, `PremiumExpire`, `PremiumCount`, `AdminColor`, `PremiumColor`, `BankDeelay`) VALUES
(1, 'ionut', 'DA68AF38218C1DF7897ADC449423866343B6EA75E4BF0D6B143D862EEA2BECAA2E607C5243D947F9705AAB589A3A1E6DFD39992BF575B200D237120B9A81CF7C', 'email@yahoo.com', 0, 1, 10001.6, 977135499, 0, 899999893, 400, 8, 0, 1, 998023, 42, 1, 0, 0, 0, 0, 0, 84, '2|12|27|36|37|56|78|79|88|151|157|84|', '0|0|0|0|0|0|0|0|0|0|0|4|', 0, 0, 0, 0, 0, 0, 0, 0, 0, 1721992428, '4 0 0 0 0 0', 0, 0, 1, 886, 'copattack', 'Fara Crima', 'Fara Crima', 'Fara Crima', 'Fara Crima', 'Fara Crima', 4, 0, 1, 34, 1, 1337, 1, 250, 1, 0, 3, '0 0 0', '0 0 0', '0 0 0', 0, 0, -1, 96, 0, 86, 86, 1722205271, '0 0 1 0 0 1 1 1 1 1 1', 2, '1 1 2 1 1 1 1 1 1 1 1 1 2', '0 0 27 0 0 0 0 0 6 0 5 0 40', '0 0 100 0 0 0 0 0 20 0 21 0 25', '0 0 0', 14, 0, 0, 10, 0, 0, 1, 0, 1, 1, 19079, 0, 0, 'pet', 0, 0, 0, 0, 100, 0, 0, '01:21:11 29/07/2024', 0, -1, 0, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 100, 3600, 826, '0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0', '0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0', 0, -1, 0, 0, 0, 0, 0, 0, 0, 'CDCE9A88989C8EF09DECDADCDDC5CCDDC4A9EEC9', 'CA95E8AEDDED4A09440E55DAF0DCEC599449D5FC', 10, 7, 5, 41, 1724683771, 1, 1, 5, 0),
(2, 'ionut2', 'DA68AF38218C1DF7897ADC449423866343B6EA75E4BF0D6B143D862EEA2BECAA2E607C5243D947F9705AAB589A3A1E6DFD39992BF575B200D237120B9A81CF7C', 'email@yahoo.com', 0, 1, 1.29, 25988, 0, 0, 0, 0, 0, 1, 59, 4, 1, 0, 0, 0, 0, 0, 36, '2|12|27|36|37|56|78|79|88|151|157|', '0|0|0|0|0|0|0|0|0|0|0|', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '0 0 0 0 0 0', 0, 0, 0, 0, 'Fara Crima', 'Fara Crima', 'Fara Crima', 'Fara Crima', 'Fara Crima', 'Fara Crima', 4, 0, 1, 0, 0, 450506, 1, 194, 0, 0, 2, '0 0 0', '0 0 0', '0 0 0', 0, 0, 0, 3, 0, 0, 0, 1722295329, '0 0 1 0 0 1 1 0 1 1 1', 2, '1 1 1 1 1 1 1 1 1 1 1 1 1', '0 0 3 0 0 0 0 0 0 0 0 0 0', '0 0 47 0 0 0 0 0 0 0 0 0 0', '0 0 0', 5, 0, 0, 0, 0, 0, 0, 0, 0, 1, 19079, 0, 0, 'pet', 0, 0, 0, 0, 100, 0, 0, '02:22:09 30/07/2024', 0, -1, 0, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3600, 806, '0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0', '0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0', 0, -1, 0, 0, 0, 0, 0, 0, 0, 'CDCE9A88989C8EF09DECDADCDDC5CCDDC4A9EEC9', 'CA95E8AEDDED4A09440E55DAF0DCEC599449D5FC', 0, 12, 3, 0, 0, 0, 0, 0, 0),
(3, 'ionutadv', 'DA68AF38218C1DF7897ADC449423866343B6EA75E4BF0D6B143D862EEA2BECAA2E607C5243D947F9705AAB589A3A1E6DFD39992BF575B200D237120B9A81CF7C', 'email@yahoo.com', 0, 1, 3.04, 2191309, 0, 29996, 9600, 8, 0, 1, 1157, 10, 1, 0, 0, 0, 0, 0, 296, '2|12|27|36|37|56|78|79|88|151|157|296|', '0|0|0|0|0|0|0|0|0|0|0|1|', 0, 0, 0, 0, 0, 0, 0, 0, 0, 1722875535, '1 0 0 0 0 0', 0, 0, 0, -1, 'Fara Crima', 'Fara Crima', 'Fara Crima', 'Fara Crima', 'Fara Crima', 'Fara Crima', 4, 0, 1, 0, 0, 0, 0, 0, 0, 0, 3, '0 0 0', '0 0 0', '0 0 0', 0, 0, 0, 10, 99, 99, 99, 1722800038, '0 0 1 1 0 1 1 1 1 1 1', 2, '1 1 1 1 1 1 1 1 1 1 1 1 1', '0 0 1 0 0 0 0 0 0 0 0 0 0', '0 0 49 0 0 0 0 0 0 0 0 0 0', '0 0 0', 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 19079, 0, 0, 'pet', 0, 0, 0, 0, 100, 0, 0, '22:33:58 04/08/2024', 0, -1, 0, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 100, 3600, 2462, '0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0', '0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0', 0, -1, 0, 0, 0, 0, 0, 0, 0, 'CDCE9A88989C8EF09DECDADCDDC5CCDDC4A9EEC9', 'CA95E8AEDDED4A09440E55DAF0DCEC599449D5FC', 0, 99, 0, 8, 1724883123, 1, 0, 0, 0),
(4, 'asser', 'DA68AF38218C1DF7897ADC449423866343B6EA75E4BF0D6B143D862EEA2BECAA2E607C5243D947F9705AAB589A3A1E6DFD39992BF575B200D237120B9A81CF7C', 'loaded1338@gmail.com', 0, 1, 0, 20000, 0, 30000, 0, 8, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 36, '2|12|27|36|37|56|78|79|88|151|157|', '0|0|0|0|0|0|0|0|0|0|0|', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '0 0 0 0 0 0', 0, 0, 0, -1, 'Fara Crima', 'Fara Crima', 'Fara Crima', 'Fara Crima', 'Fara Crima', 'Fara Crima', 4, 0, 1, 0, 0, 0, 0, 0, 0, 0, 2, '0 0 0', '0 0 0', '0 0 0', 0, 0, 0, 10, 100, 0, 0, 1722467630, '0 0 0 0 0 1 1 0 1 1 0 0 1', 0, '1 1 1 1 1 1 1 1 1 1 1 1 1', '0 0 0 0 0 0 0 0 0 0 0 0 0', '0 0 0 0 0 0 0 0 0 0 0 0 0', '0 0 0', 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 19079, 0, 0, 'pet', 0, 0, 0, 0, 100, 0, 0, '02:13:50 01/08/2024', 0, -1, 0, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3600, 198, '0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0', '0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0', 0, -1, 0, 0, 0, 0, 0, 0, 0, 'CDCE9A88989C8EF09DECDADCDDC5CCDDC4A9EEC9', 'CDCE9A88989C8EF09DECDADCDDC5CCDDC4A9EEC9', 0, 0, 0, 0, -1, 0, 0, 0, 0),
(5, 'Ramos', '84204A7FA37801C5812C4A1DEEAFF634EA1EDFB7E624BE2311CD0ED03996F8ECDEA438FE9952AE3A07C271E3605F8851F1DF63936D87DB853D069241281F87BA', 'mhormx@gmail.com', 0, 1, 0, 3560743, 0, 30000, 500, 8, 0, 1, 0, 10, 1, 0, 1, 7, 0, 0, 37, '2|12|27|36|37|56|78|79|88|151|157|', '0|0|0|0|0|0|0|0|0|0|0|', 0, 1722799597, 0, 0, 0, 0, 0, 0, 0, 1723404188, '0 0 0 0 0 0', 0, 0, 0, -1, 'Fara Crima', 'Fara Crima', 'Fara Crima', 'Fara Crima', 'Fara Crima', 'Fara Crima', 4, 0, 1, 0, 1, 792741, 1, 200, 1, 0, 2, '0 0 0', '0 0 0', '0 0 0', 0, 0, 0, 10, 100, 0, 0, 1722801557, '0 0 1 1 0 1 1 0 1 0 1', 8, '1 1 1 1 1 1 1 1 1 1 1 1', '0 0 0 0 0 0 0 0 1 0 0 0', '0 0 0 0 0 0 0 0 25 0 0 0', '0 0 0', 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 19079, 0, 0, 'pet', 0, 0, 0, 0, 100, 0, 0, '22:59:17 04/08/2024', 0, -1, 0, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3600, 1047, '0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0', '0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0', 0, -1, 0, 0, 0, 0, 0, 0, 0, 'E8CEC085E80E5849AF4CD9E88949EAE95889DCAF', 'E8CEC085E80E5849AF4CD9E88949EAE95889DCAF', 0, 0, 1, 0, -1, 0, 0, 0, 0),
(6, 'FRANCOKL888', '14EFE467B090B35CA4BAC5A0A952E671DF8AC7FBE1DD468D99F57787D394979CC52F4C518973EE00320297DF7A6A5DFE7D829F0C780752DC263882584A9DBF54', 'francoju123xd@gmail.com', 0, 0, 0, 20000, 0, 30000, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 36, '2|12|27|36|37|56|78|79|88|151|157|', '0|0|0|0|0|0|0|0|0|0|0|', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '0 0 0 0 0 0', 0, 0, 0, -1, 'Fara Crima', 'Fara Crima', 'Fara Crima', 'Fara Crima', 'Fara Crima', 'Fara Crima', 4, 0, 1, 0, 0, 0, 0, 0, 0, 0, 2, '0 0 0', '0 0 0', '0 0 0', 0, 0, 0, 10, 0, 0, 0, 1722800081, '0 0 0 0 0 1 1 0 1 1 0 0 1', 0, '1 1 1 1 1 1 1 1 1 1 1 1 1', '0 0 0 0 0 0 0 0 0 0 0 0 0', '0 0 0 0 0 0 0 0 0 0 0 0 0', '0 0 0', 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 19079, 0, 0, 'pet', 0, 0, 0, 0, 100, 0, 0, '22:34:41 04/08/2024', 0, -1, 0, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3600, 50, '0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0', '0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|0', 0, -1, 0, 0, 0, 0, 0, 0, 0, 'EEACC9DA0D4E9E5EEFEF55C890489095090DD8AF', '-1', 0, 0, 0, 0, -1, 0, 0, 0, 0);

-- --------------------------------------------------------

--
-- Table structure for table `vehicles`
--

CREATE TABLE `vehicles` (
  `ID` int(11) NOT NULL,
  `modelId` int(11) DEFAULT NULL,
  `groupId` int(11) DEFAULT NULL,
  `vehRank` int(11) NOT NULL DEFAULT '1',
  `color1` int(11) DEFAULT NULL,
  `color2` int(11) DEFAULT NULL,
  `posX` float DEFAULT NULL,
  `posY` float DEFAULT NULL,
  `posZ` float DEFAULT NULL,
  `posA` float DEFAULT NULL,
  `job` int(11) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Dumping data for table `vehicles`
--

INSERT INTO `vehicles` (`ID`, `modelId`, `groupId`, `vehRank`, `color1`, `color2`, `posX`, `posY`, `posZ`, `posA`, `job`) VALUES
(1, 411, 1, 3, 1, 1, 1600.51, -1608.28, 13.4886, 90, 0),
(2, 596, 1, 1, 1, 0, 1600.51, -1608.28, 13.4886, 90, 0),
(3, 523, 1, 1, 1, 1, 1600.51, -1608.28, 13.4886, 90, 0),
(4, 411, 2, 2, 53, 53, 612.114, -589.481, 16.9552, 270, 0),
(5, 596, 2, 1, 53, 53, 612.114, -589.481, 16.9552, 270, 0),
(6, 470, 3, 1, 245, 245, 202.916, 1879.47, 17.3733, 3.5, 0),
(7, 411, 3, 2, 245, 245, 202.916, 1879.47, 17.3733, 3.5, 0),
(8, 520, 3, 1, 245, 245, 202.916, 1879.47, 17.3733, 3.5, 0),
(9, 425, 3, 1, 245, 1, 202.916, 1879.47, 17.3733, 3.5, 0),
(10, 411, 8, 3, 1, 1, 2307.22, 2453, 10.5474, 180, 0),
(11, 598, 8, 1, 1, 0, 2307.22, 2453, 10.5474, 180, 0),
(12, 523, 8, 1, 1, 1, 2307.22, 2453, 10.5474, 180, 0),
(13, 582, 9, 1, 136, 136, -295.506, 1554.61, 75.0864, 225, 0),
(14, 560, 9, 2, 136, 136, -295.506, 1554.61, 75.0864, 225, 0),
(15, 488, 9, 2, 136, 136, -295.506, 1554.61, 75.0864, 225, 0),
(19, 487, 11, 1, 175, 175, 1071.97, -310.398, 73.6975, 180, 0),
(20, 469, 11, 1, 175, 175, 1071.97, -310.398, 73.6975, 180, 0),
(21, 560, 11, 1, 175, 175, 1071.97, -310.398, 73.6975, 180, 0),
(22, 461, 11, 1, 175, 175, 1071.97, -310.398, 73.6975, 180, 0),
(23, 445, 12, 1, 154, 154, 2394.14, 1677.22, 10.5474, 180, 0),
(24, 560, 12, 2, 154, 154, 2394.14, 1677.22, 10.5474, 180, 0),
(25, 522, 12, 2, 154, 154, 2394.14, 1677.22, 10.5474, 180, 0),
(26, 438, 13, 1, 1, 1, 1777.04, -1911.44, 13.092, 270, 0),
(27, 420, 13, 1, 1, 1, 1777.04, -1911.44, 13.092, 270, 0),
(28, 560, 13, 2, 1, 1, 1777.04, -1911.44, 13.092, 270, 0),
(29, 416, 14, 1, 158, 1, 1592.77, 1850.72, 10.5474, 180, 0),
(30, 599, 14, 2, 158, 1, 1592.77, 1850.72, 10.5474, 180, 0),
(31, 411, 14, 4, 158, 1, 1592.77, 1850.72, 10.5474, 180, 0),
(86, 490, 2, 1, 53, 53, 612.114, -589.481, 16.9552, 270, 0),
(87, 541, 2, 1, 53, 53, 612.114, -589.481, 16.9552, 270, 0),
(88, 497, 2, 1, 53, 53, 615.302, -575.59, 26.3201, 270, 0),
(256, 445, 4, 1, 155, 155, 1466.83, 2752.14, 10.6202, 28.1635, 0),
(257, 560, 4, 1, 155, 155, 1472.76, 2763.47, 10.4478, 1.1737, 0),
(258, 567, 4, 1, 155, 155, 1473.01, 2773.22, 10.6111, 0.5568, 0),
(259, 560, 4, 1, 155, 155, 1472.67, 2782.06, 10.4493, 359.73, 0),
(260, 567, 4, 1, 155, 155, 1471.88, 2799.83, 10.6154, 312.66, 0),
(261, 521, 4, 1, 155, 155, 1459.73, 2786.92, 10.3888, 274.353, 0),
(262, 521, 4, 1, 155, 155, 1459.79, 2783.84, 10.381, 272.745, 0),
(263, 521, 4, 1, 155, 155, 1459.81, 2781.05, 10.3888, 268.326, 0),
(264, 521, 4, 1, 155, 155, 1459.7, 2778.48, 10.3888, 266.84, 0),
(265, 487, 4, 1, 155, 155, 1484, 2814.2, 10.967, 237.186, 0),
(266, 579, 4, 1, 155, 155, 1487.81, 2782.01, 10.7339, 272.076, 0),
(267, 579, 4, 1, 155, 155, 1488.34, 2764.92, 10.7707, 271.283, 0),
(268, 579, 5, 1, 245, 245, 2473.35, -1691.94, 13.4406, 0.1959, 0),
(269, 560, 5, 1, 245, 245, 2468.93, -1671.24, 13.103, 8.9487, 0),
(270, 461, 5, 1, 234, 234, 2480.84, -1689.69, 13.0986, 353.983, 0),
(271, 461, 5, 1, 234, 234, 2483.8, -1689.85, 13.0959, 355.075, 0),
(272, 521, 5, 1, 245, 245, 2506.92, -1686.61, 13.1228, 49.1575, 0),
(273, 521, 5, 1, 245, 245, 2509.33, -1684.39, 13.1284, 49.1438, 0),
(274, 567, 5, 1, 187, 187, 2510.91, -1677.77, 13.4198, 56.6809, 0),
(275, 492, 5, 1, 245, 245, 2512.5, -1672.83, 13.2828, 76.263, 0),
(276, 567, 5, 1, 245, 245, 2513.33, -1666.85, 13.456, 100.498, 0),
(277, 409, 5, 1, 245, 245, 2498.21, -1656.17, 13.2082, 78.5592, 0),
(278, 579, 5, 1, 245, 245, 2481.58, -1654.29, 13.3074, 89.6694, 0),
(279, 487, 5, 1, 234, 234, 2529.58, -1677.65, 20.107, 89.802, 0),
(280, 567, 6, 1, 6, 6, 703.886, -1422.26, 13.4081, 0.1326, 0),
(281, 560, 6, 1, 6, 6, 713.625, -1423.11, 13.2375, 356.434, 0),
(282, 567, 6, 1, 6, 6, 703.882, -1437.76, 13.4052, 359.732, 0),
(283, 461, 6, 1, 6, 6, 711.285, -1439.26, 13.1204, 4.5633, 0),
(284, 521, 6, 1, 6, 6, 712.757, -1439.34, 13.1076, 6.2141, 0),
(285, 521, 6, 1, 6, 6, 714.634, -1439.45, 13.1076, 1.2864, 0),
(286, 521, 6, 1, 6, 6, 716.422, -1439.4, 13.1076, 4.8335, 0),
(287, 579, 6, 1, 6, 6, 722.215, -1431.55, 13.4844, 88.9456, 0),
(288, 409, 6, 1, 6, 6, 731.121, -1431.5, 13.3157, 89.3363, 0),
(289, 492, 6, 1, 6, 6, 740.421, -1431.58, 13.309, 89.2923, 0),
(290, 579, 6, 1, 6, 6, 745.264, -1442.08, 13.4485, 0.1308, 0),
(291, 487, 6, 1, 6, 6, 704.686, -1452.22, 17.8458, 0.6501, 0),
(292, 521, 10, 1, 233, 233, 1458.56, 734.428, 10.592, 179.927, 0),
(293, 521, 10, 1, 233, 233, 1456.68, 734.469, 10.592, 180.32, 0),
(294, 461, 10, 1, 233, 233, 1453.84, 734.387, 10.6047, 183.146, 0),
(295, 461, 10, 1, 233, 233, 1451.93, 734.588, 10.6047, 194.612, 0),
(296, 409, 10, 1, 233, 233, 1446.79, 738.13, 10.6386, 181.096, 0),
(297, 487, 10, 1, 233, 233, 1439.73, 784.875, 11.1732, 175.803, 0),
(298, 492, 10, 1, 233, 233, 1413.32, 774.961, 10.5967, 270.655, 0),
(299, 560, 10, 1, 233, 233, 1413.45, 768.711, 10.5251, 270.244, 0),
(300, 579, 10, 1, 233, 233, 1413.93, 762.341, 10.7464, 270.236, 0),
(301, 567, 10, 1, 233, 233, 1413.88, 755.852, 10.6925, 269.612, 0),
(302, 579, 10, 1, 233, 233, 1413.96, 749.49, 10.7488, 269.889, 0),
(303, 567, 10, 1, 233, 233, 1413.88, 743.065, 10.6907, 269.678, 0),
(304, 492, 15, 1, 3, 3, 2562.96, 1887.33, 10.6037, 270.693, 0),
(305, 579, 15, 1, 3, 3, 2576.66, 1897.19, 10.7495, 181.443, 0),
(306, 560, 15, 1, 3, 3, 2583.34, 1897.6, 10.527, 180.462, 0),
(307, 521, 15, 1, 3, 3, 2585.37, 1885.56, 10.3907, 179.88, 0),
(308, 461, 15, 1, 3, 3, 2589.4, 1885.23, 10.4034, 179.831, 0),
(309, 461, 15, 1, 3, 3, 2606.27, 1885.04, 10.4015, 180.9, 0),
(310, 521, 15, 1, 3, 3, 2610.11, 1885.49, 10.3888, 179.183, 0),
(311, 560, 15, 1, 3, 3, 2611.92, 1897.6, 10.5255, 179.231, 0),
(312, 579, 15, 1, 3, 3, 2618.36, 1897.06, 10.7507, 179.774, 0),
(313, 409, 15, 1, 3, 3, 2630.14, 1887.34, 10.6387, 88.3941, 0),
(314, 567, 15, 1, 3, 3, 2602.93, 1862.64, 10.6884, 270.045, 0),
(315, 567, 15, 1, 3, 3, 2591.67, 1862.53, 10.6928, 89.1819, 0),
(316, 487, 15, 1, 3, 3, 2596.46, 1839.83, 11.1537, 266.784, 0),
(317, 409, 16, 1, 236, 236, 1480.8, -1282.37, 14.371, 270.7, 0),
(318, 567, 16, 1, 236, 236, 1502.06, -1281.65, 14.414, 179.822, 0),
(319, 579, 16, 1, 236, 236, 1506.7, -1282.35, 14.465, 177.695, 0),
(320, 579, 16, 1, 236, 236, 1513.9, -1271.77, 14.496, 178.844, 0),
(321, 567, 16, 1, 236, 236, 1513.93, -1263.53, 14.431, 180.255, 0),
(322, 492, 16, 1, 236, 236, 1519.07, -1263.76, 14.315, 179.52, 0),
(323, 560, 16, 1, 236, 236, 1518.87, -1271.39, 14.228, 179.377, 0),
(324, 461, 16, 1, 236, 236, 1528.87, -1277.04, 15.098, 126.478, 0),
(325, 521, 16, 1, 236, 236, 1529.64, -1278.24, 15.096, 125.343, 0),
(326, 521, 16, 1, 236, 236, 1534.91, -1286.1, 15.093, 144.558, 0),
(327, 461, 16, 1, 236, 236, 1536.14, -1286.63, 15.107, 148.695, 0),
(328, 487, 16, 1, 236, 236, 1508.04, -1320.95, 14.359, 0.017, 0),
(329, 438, 7, 1, 6, 6, 984.779, 1733.01, 8.36, 90.3, 0),
(330, 560, 7, 1, 6, 6, 984.779, 1733.01, 8.36, 90.3, 0),
(331, 420, 7, 1, 6, 6, 984.779, 1733.01, 8.36, 90.3, 0),
(332, 497, 1, 3, 1, 0, 1560.81, -1651.27, 28.5739, 90, 0),
(333, 497, 8, 3, 1, 0, 2319.03, 2474.85, 38.8591, 180, 0),
(334, 598, 3, 1, 245, 1, 202.916, 1879.47, 17.3733, 3.5, 0),
(335, 468, 3, 1, 245, 1, 202.916, 1879.47, 17.3733, 3.5, 0),
(336, 510, 0, 1, 1, 1, 491.052, -1514.49, 20.044, 91.911, NULL),
(337, 510, 0, 1, 1, 1, 491.073, -1511.24, 20.063, 89.987, NULL),
(338, 510, 0, 1, 1, 1, 490.981, -1508.44, 20.128, 89.987, NULL),
(339, 510, 0, 1, 1, 1, 491.031, -1504.81, 20.141, 90.012, NULL),
(340, 510, 0, 1, 1, 1, 490.89, -1506.59, 20.225, 89.954, NULL),
(341, 510, 0, 1, 1, 1, 491.578, -1500.81, 20.027, 90.032, NULL),
(342, 510, 0, 1, 1, 1, 492.008, -1497.25, 19.951, 90.011, NULL),
(343, 510, 0, 1, 1, 1, 490.932, -1502.64, 20.122, 90.229, NULL);

-- --------------------------------------------------------

--
-- Table structure for table `wars`
--

CREATE TABLE `wars` (
  `ID` int(11) NOT NULL,
  `playerName` varchar(24) DEFAULT NULL,
  `gangId` int(11) DEFAULT NULL,
  `warTurf` int(11) DEFAULT NULL,
  `warKills` int(11) DEFAULT NULL,
  `warDeaths` int(11) DEFAULT NULL,
  `warAssists` int(11) DEFAULT NULL,
  `warSeconds` int(11) DEFAULT NULL,
  `insertTime` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `wcode_applications`
--

CREATE TABLE `wcode_applications` (
  `id` int(11) NOT NULL,
  `UserID` int(11) NOT NULL,
  `UserName` varchar(32) NOT NULL,
  `FactionID` int(11) NOT NULL,
  `Answers` text NOT NULL,
  `Questions` int(11) NOT NULL,
  `Status` int(11) NOT NULL,
  `Date` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `ActionBy` varchar(24) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `wcode_commentaries`
--

CREATE TABLE `wcode_commentaries` (
  `ID` int(11) NOT NULL,
  `UserID` int(11) NOT NULL,
  `UserName` varchar(32) NOT NULL,
  `Skin` int(11) NOT NULL,
  `Text` text NOT NULL,
  `Date` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `Section` varchar(32) NOT NULL,
  `TopicID` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `wcode_complaints`
--

CREATE TABLE `wcode_complaints` (
  `ID` int(11) NOT NULL,
  `UserID` int(11) NOT NULL,
  `UserName` varchar(32) NOT NULL,
  `AccusedID` int(11) NOT NULL,
  `AccusedName` varchar(32) NOT NULL,
  `Text` text NOT NULL,
  `Category` int(11) NOT NULL,
  `Faction` int(11) NOT NULL,
  `Status` int(11) NOT NULL,
  `Date` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `ActionBy` varchar(32) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `wcode_complaints`
--

INSERT INTO `wcode_complaints` (`ID`, `UserID`, `UserName`, `AccusedID`, `AccusedName`, `Text`, `Category`, `Faction`, `Status`, `Date`, `ActionBy`) VALUES
(1, 1, 'vividsec', 3, 'vividsec2', 'asdasdasdasdasdasdasdasdasdasdasdasdasdasdasdasdasdasdasdasdasdasdasdasdasdasdasdasdasdasdasdasdasdasdasdasdasdasdasdasdasdasdasdasdasdasdasdasdasdasd', 0, 0, 0, '2024-06-05 10:44:39', '');

-- --------------------------------------------------------

--
-- Table structure for table `wcode_editables`
--

CREATE TABLE `wcode_editables` (
  `ID` int(11) NOT NULL,
  `Text` text NOT NULL,
  `By` varchar(32) NOT NULL,
  `Form` varchar(32) NOT NULL,
  `Updated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `wcode_functions`
--

CREATE TABLE `wcode_functions` (
  `ID` int(11) NOT NULL,
  `UserID` int(11) NOT NULL,
  `UserName` varchar(32) NOT NULL,
  `Tag` varchar(64) NOT NULL,
  `Color` varchar(32) NOT NULL,
  `Icon` varchar(64) NOT NULL,
  `Date` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `wcode_logs`
--

CREATE TABLE `wcode_logs` (
  `ID` int(11) NOT NULL,
  `UserID` int(11) NOT NULL,
  `UserName` varchar(32) NOT NULL,
  `Log` text NOT NULL,
  `VictimID` int(11) NOT NULL,
  `VictimName` varchar(32) NOT NULL,
  `Date` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `wcode_notifications`
--

CREATE TABLE `wcode_notifications` (
  `ID` int(11) NOT NULL,
  `UserID` int(11) NOT NULL,
  `UserName` varchar(32) NOT NULL,
  `Notification` text NOT NULL,
  `FromID` int(11) NOT NULL,
  `FromName` varchar(32) NOT NULL,
  `Seen` int(11) NOT NULL,
  `Link` text NOT NULL,
  `Date` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `wcode_notifications`
--

INSERT INTO `wcode_notifications` (`ID`, `UserID`, `UserName`, `Notification`, `FromID`, `FromName`, `Seen`, `Link`, `Date`) VALUES
(1, 3, 'vividsec2', 'vividsec made an Faction complaint against you! Check it out.', 1, 'vividsec', 0, 'http://localhost/t2p/complaints', '2024-06-05 10:44:39');

-- --------------------------------------------------------

--
-- Table structure for table `wcode_questions`
--

CREATE TABLE `wcode_questions` (
  `id` int(11) NOT NULL,
  `question` text NOT NULL,
  `factionid` int(11) NOT NULL,
  `date` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `wcode_recover`
--

CREATE TABLE `wcode_recover` (
  `ID` int(11) NOT NULL,
  `user` int(11) NOT NULL,
  `username` varchar(90) NOT NULL,
  `email` text NOT NULL,
  `token` varchar(90) NOT NULL,
  `time` varchar(90) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- --------------------------------------------------------

--
-- Table structure for table `wcode_tickets`
--

CREATE TABLE `wcode_tickets` (
  `ID` int(11) NOT NULL,
  `UserID` int(11) NOT NULL,
  `UserName` varchar(32) NOT NULL,
  `Category` varchar(64) NOT NULL,
  `Text` text NOT NULL,
  `Status` int(11) NOT NULL,
  `Date` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `ActionBy` varchar(32) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `wcode_tickets`
--

INSERT INTO `wcode_tickets` (`ID`, `UserID`, `UserName`, `Category`, `Text`, `Status`, `Date`, `ActionBy`) VALUES
(1, 1, 'vividsec', 'Account problems', 'asdasdasd', 0, '2024-06-05 10:44:20', '');

-- --------------------------------------------------------

--
-- Table structure for table `wcode_unban`
--

CREATE TABLE `wcode_unban` (
  `ID` int(11) NOT NULL,
  `UserID` int(11) NOT NULL,
  `UserName` varchar(32) NOT NULL,
  `BanDetails` text NOT NULL,
  `Title` varchar(32) NOT NULL,
  `Details` text NOT NULL,
  `Status` int(11) NOT NULL,
  `Date` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `ActionBy` varchar(32) NOT NULL DEFAULT 'Banned'
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Indexes for dumped tables
--

--
-- Indexes for table `accs`
--
ALTER TABLE `accs`
  ADD PRIMARY KEY (`ID`);

--
-- Indexes for table `accs_models`
--
ALTER TABLE `accs_models`
  ADD UNIQUE KEY `ID` (`ID`);

--
-- Indexes for table `allowedcomputers`
--
ALTER TABLE `allowedcomputers`
  ADD PRIMARY KEY (`ID`);

--
-- Indexes for table `apartaments`
--
ALTER TABLE `apartaments`
  ADD PRIMARY KEY (`ID`);

--
-- Indexes for table `atms`
--
ALTER TABLE `atms`
  ADD PRIMARY KEY (`atmId`);

--
-- Indexes for table `auctions`
--
ALTER TABLE `auctions`
  ADD PRIMARY KEY (`ID`);

--
-- Indexes for table `bans`
--
ALTER TABLE `bans`
  ADD PRIMARY KEY (`ID`),
  ADD UNIQUE KEY `ID` (`ID`);

--
-- Indexes for table `businesses`
--
ALTER TABLE `businesses`
  ADD PRIMARY KEY (`ID`);

--
-- Indexes for table `clans`
--
ALTER TABLE `clans`
  ADD PRIMARY KEY (`ID`);

--
-- Indexes for table `clanvehicles`
--
ALTER TABLE `clanvehicles`
  ADD PRIMARY KEY (`ID`);

--
-- Indexes for table `dealership`
--
ALTER TABLE `dealership`
  ADD PRIMARY KEY (`ds_Id`);

--
-- Indexes for table `emails`
--
ALTER TABLE `emails`
  ADD PRIMARY KEY (`ID`);

--
-- Indexes for table `factions`
--
ALTER TABLE `factions`
  ADD PRIMARY KEY (`ID`);

--
-- Indexes for table `faction_history`
--
ALTER TABLE `faction_history`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `gangzones`
--
ALTER TABLE `gangzones`
  ADD PRIMARY KEY (`ID`);

--
-- Indexes for table `garages`
--
ALTER TABLE `garages`
  ADD PRIMARY KEY (`ID`);

--
-- Indexes for table `housemapping`
--
ALTER TABLE `housemapping`
  ADD PRIMARY KEY (`ID`);

--
-- Indexes for table `houses`
--
ALTER TABLE `houses`
  ADD PRIMARY KEY (`ID`);

--
-- Indexes for table `kenny_applications`
--
ALTER TABLE `kenny_applications`
  ADD PRIMARY KEY (`ID`);

--
-- Indexes for table `kenny_questions`
--
ALTER TABLE `kenny_questions`
  ADD PRIMARY KEY (`ID`);

--
-- Indexes for table `kenny_recovery`
--
ALTER TABLE `kenny_recovery`
  ADD PRIMARY KEY (`ID`);

--
-- Indexes for table `kenny_suspend`
--
ALTER TABLE `kenny_suspend`
  ADD PRIMARY KEY (`ID`);

--
-- Indexes for table `personalvehicles`
--
ALTER TABLE `personalvehicles`
  ADD PRIMARY KEY (`ID`);

--
-- Indexes for table `player_logs`
--
ALTER TABLE `player_logs`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `server_job_goal`
--
ALTER TABLE `server_job_goal`
  ADD PRIMARY KEY (`ID`);

--
-- Indexes for table `server_market`
--
ALTER TABLE `server_market`
  ADD PRIMARY KEY (`marketID`);

--
-- Indexes for table `server_safezones`
--
ALTER TABLE `server_safezones`
  ADD PRIMARY KEY (`safezoneKey`);

--
-- Indexes for table `server_wars`
--
ALTER TABLE `server_wars`
  ADD PRIMARY KEY (`ID`);

--
-- Indexes for table `special_codes`
--
ALTER TABLE `special_codes`
  ADD PRIMARY KEY (`ID`);

--
-- Indexes for table `users`
--
ALTER TABLE `users`
  ADD PRIMARY KEY (`ID`);

--
-- Indexes for table `vehicles`
--
ALTER TABLE `vehicles`
  ADD PRIMARY KEY (`ID`);

--
-- Indexes for table `wars`
--
ALTER TABLE `wars`
  ADD PRIMARY KEY (`ID`);

--
-- Indexes for table `wcode_applications`
--
ALTER TABLE `wcode_applications`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `wcode_commentaries`
--
ALTER TABLE `wcode_commentaries`
  ADD PRIMARY KEY (`ID`);

--
-- Indexes for table `wcode_complaints`
--
ALTER TABLE `wcode_complaints`
  ADD PRIMARY KEY (`ID`);

--
-- Indexes for table `wcode_editables`
--
ALTER TABLE `wcode_editables`
  ADD PRIMARY KEY (`ID`);

--
-- Indexes for table `wcode_functions`
--
ALTER TABLE `wcode_functions`
  ADD PRIMARY KEY (`ID`);

--
-- Indexes for table `wcode_logs`
--
ALTER TABLE `wcode_logs`
  ADD PRIMARY KEY (`ID`);

--
-- Indexes for table `wcode_notifications`
--
ALTER TABLE `wcode_notifications`
  ADD PRIMARY KEY (`ID`);

--
-- Indexes for table `wcode_questions`
--
ALTER TABLE `wcode_questions`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `wcode_recover`
--
ALTER TABLE `wcode_recover`
  ADD PRIMARY KEY (`ID`);

--
-- Indexes for table `wcode_tickets`
--
ALTER TABLE `wcode_tickets`
  ADD PRIMARY KEY (`ID`);

--
-- Indexes for table `wcode_unban`
--
ALTER TABLE `wcode_unban`
  ADD PRIMARY KEY (`ID`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `allowedcomputers`
--
ALTER TABLE `allowedcomputers`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=2;

--
-- AUTO_INCREMENT for table `apartaments`
--
ALTER TABLE `apartaments`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=25;

--
-- AUTO_INCREMENT for table `atms`
--
ALTER TABLE `atms`
  MODIFY `atmId` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=31;

--
-- AUTO_INCREMENT for table `auctions`
--
ALTER TABLE `auctions`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `bans`
--
ALTER TABLE `bans`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `businesses`
--
ALTER TABLE `businesses`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=57;

--
-- AUTO_INCREMENT for table `clans`
--
ALTER TABLE `clans`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=2;

--
-- AUTO_INCREMENT for table `clanvehicles`
--
ALTER TABLE `clanvehicles`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `dealership`
--
ALTER TABLE `dealership`
  MODIFY `ds_Id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=106;

--
-- AUTO_INCREMENT for table `emails`
--
ALTER TABLE `emails`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=9;

--
-- AUTO_INCREMENT for table `factions`
--
ALTER TABLE `factions`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=17;

--
-- AUTO_INCREMENT for table `faction_history`
--
ALTER TABLE `faction_history`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=14;

--
-- AUTO_INCREMENT for table `gangzones`
--
ALTER TABLE `gangzones`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=13;

--
-- AUTO_INCREMENT for table `garages`
--
ALTER TABLE `garages`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `housemapping`
--
ALTER TABLE `housemapping`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `houses`
--
ALTER TABLE `houses`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=61;

--
-- AUTO_INCREMENT for table `kenny_applications`
--
ALTER TABLE `kenny_applications`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `kenny_questions`
--
ALTER TABLE `kenny_questions`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=28;

--
-- AUTO_INCREMENT for table `kenny_recovery`
--
ALTER TABLE `kenny_recovery`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=11;

--
-- AUTO_INCREMENT for table `kenny_suspend`
--
ALTER TABLE `kenny_suspend`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `personalvehicles`
--
ALTER TABLE `personalvehicles`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=11;

--
-- AUTO_INCREMENT for table `player_logs`
--
ALTER TABLE `player_logs`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=859;

--
-- AUTO_INCREMENT for table `server_market`
--
ALTER TABLE `server_market`
  MODIFY `marketID` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=4;

--
-- AUTO_INCREMENT for table `server_safezones`
--
ALTER TABLE `server_safezones`
  MODIFY `safezoneKey` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=3;

--
-- AUTO_INCREMENT for table `special_codes`
--
ALTER TABLE `special_codes`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `users`
--
ALTER TABLE `users`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=7;

--
-- AUTO_INCREMENT for table `vehicles`
--
ALTER TABLE `vehicles`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=344;

--
-- AUTO_INCREMENT for table `wars`
--
ALTER TABLE `wars`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `wcode_applications`
--
ALTER TABLE `wcode_applications`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `wcode_commentaries`
--
ALTER TABLE `wcode_commentaries`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `wcode_complaints`
--
ALTER TABLE `wcode_complaints`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=2;

--
-- AUTO_INCREMENT for table `wcode_editables`
--
ALTER TABLE `wcode_editables`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `wcode_functions`
--
ALTER TABLE `wcode_functions`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `wcode_logs`
--
ALTER TABLE `wcode_logs`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `wcode_notifications`
--
ALTER TABLE `wcode_notifications`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=2;

--
-- AUTO_INCREMENT for table `wcode_questions`
--
ALTER TABLE `wcode_questions`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `wcode_recover`
--
ALTER TABLE `wcode_recover`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `wcode_tickets`
--
ALTER TABLE `wcode_tickets`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=2;

--
-- AUTO_INCREMENT for table `wcode_unban`
--
ALTER TABLE `wcode_unban`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
