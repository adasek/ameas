-- Adminer 4.2.4 MySQL dump

SET NAMES utf8;
SET time_zone = '+00:00';
SET foreign_key_checks = 0;
SET sql_mode = 'NO_AUTO_VALUE_ON_ZERO';

DROP TABLE IF EXISTS `Device`;
CREATE TABLE `Device` (
  `id` int(11) NOT NULL,
  `ip4Addr` varchar(15) COLLATE utf8_czech_ci DEFAULT NULL,
  `ip6Addr` varchar(45) COLLATE utf8_czech_ci DEFAULT NULL,
  `hostname` varchar(255) COLLATE utf8_czech_ci DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_czech_ci;


DROP TABLE IF EXISTS `has_rights_to`;
CREATE TABLE `has_rights_to` (
  `User_id` int(11) NOT NULL,
  `Sensor_id` int(11) NOT NULL,
  `type` int(11) DEFAULT NULL,
  PRIMARY KEY (`User_id`,`Sensor_id`),
  KEY `FK_ASS_5` (`Sensor_id`),
  CONSTRAINT `FK_ASS_4` FOREIGN KEY (`User_id`) REFERENCES `User` (`id`),
  CONSTRAINT `FK_ASS_5` FOREIGN KEY (`Sensor_id`) REFERENCES `Sensor` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_czech_ci;


DROP TABLE IF EXISTS `Measurement`;
CREATE TABLE `Measurement` (
  `pos` int(11) NOT NULL DEFAULT '0',
  `val` float DEFAULT NULL,
  `TIME` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `Sensor_id` int(11) NOT NULL,
  PRIMARY KEY (`TIME`,`Sensor_id`,`pos`),
  KEY `Sensor_id_pos` (`Sensor_id`,`pos`),
  CONSTRAINT `Measurement_Sensor_FK` FOREIGN KEY (`Sensor_id`) REFERENCES `Sensor` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_czech_ci;


DROP TABLE IF EXISTS `Sensor`;
CREATE TABLE `Sensor` (
  `id` int(11) NOT NULL,
  `name` varchar(255) COLLATE utf8_czech_ci DEFAULT NULL,
  `Device_id` int(11) DEFAULT NULL,
  `SensorType_id` int(11) NOT NULL,
  `script_param` varchar(255) COLLATE utf8_czech_ci NOT NULL,
  PRIMARY KEY (`id`),
  KEY `Sensor_Device_FK` (`Device_id`),
  KEY `Sensor_SensorType_FK` (`SensorType_id`),
  CONSTRAINT `Sensor_Device_FK` FOREIGN KEY (`Device_id`) REFERENCES `Device` (`id`),
  CONSTRAINT `Sensor_SensorType_FK` FOREIGN KEY (`SensorType_id`) REFERENCES `SensorType` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_czech_ci;


DROP TABLE IF EXISTS `SensorType`;
CREATE TABLE `SensorType` (
  `id` int(11) NOT NULL,
  `name` varchar(255) COLLATE utf8_czech_ci DEFAULT NULL,
  `script` varchar(255) COLLATE utf8_czech_ci DEFAULT NULL,
  `retries` int(11) DEFAULT NULL,
  `numOfRemeasurements` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_czech_ci;


DROP TABLE IF EXISTS `sensorType-valueType`;
CREATE TABLE `sensorType-valueType` (
  `SensorType_id` int(11) NOT NULL,
  `ValueType_id` int(11) NOT NULL,
  `pos` int(11) NOT NULL,
  PRIMARY KEY (`SensorType_id`,`ValueType_id`),
  KEY `FK_ASS_9` (`ValueType_id`),
  CONSTRAINT `FK_ASS_8` FOREIGN KEY (`SensorType_id`) REFERENCES `SensorType` (`id`),
  CONSTRAINT `FK_ASS_9` FOREIGN KEY (`ValueType_id`) REFERENCES `ValueType` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_czech_ci;


DROP TABLE IF EXISTS `User`;
CREATE TABLE `User` (
  `id` int(11) NOT NULL,
  `email` varchar(255) COLLATE utf8_czech_ci NOT NULL,
  `password` varchar(255) COLLATE utf8_czech_ci NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_czech_ci;


DROP TABLE IF EXISTS `ValueType`;
CREATE TABLE `ValueType` (
  `id` int(11) NOT NULL,
  `shortName` varchar(255) COLLATE utf8_czech_ci DEFAULT NULL,
  `name` varchar(255) COLLATE utf8_czech_ci DEFAULT NULL,
  `allowedDeviation` float DEFAULT NULL,
  `color` varchar(7) COLLATE utf8_czech_ci DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_czech_ci;


-- 2016-02-14 13:59:43
