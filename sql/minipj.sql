create database if not exists minipj;

CREATE TABLE `temp` 
(
  `id` int(3) NOT NULL primary key AUTO_INCREMENT,
  `temp` varchar(5) DEFAULT NULL,
  `date` datetime DEFAULT current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

CREATE TABLE `humid` 
(
  `id` int(3) NOT NULL primary key AUTO_INCREMENT,
  `humid` varchar(3) DEFAULT NULL,
  `date` datetime DEFAULT current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

CREATE TABLE `moisture` 
(
  `id` int(3) NOT NULL primary key AUTO_INCREMENT,
  `mois` varchar(3) NOT NULL,
  `date` datetime NOT NULL DEFAULT current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

CREATE TABLE `autostate` 
(
  `id` int(3) NOT NULL primary key AUTO_INCREMENT,
  `state` varchar(3) DEFAULT NULL,
  `date` datetime DEFAULT current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

CREATE TABLE `fanstate` 
(
  `id` int(3) NOT NULL primary key AUTO_INCREMENT,
  `state` varchar(3) DEFAULT NULL,
  `date` datetime DEFAULT current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

CREATE TABLE `pumpstate` 
(
  `id` int(3) NOT NULL primary key AUTO_INCREMENT,
  `state` varchar(3) DEFAULT NULL,
  `date` datetime DEFAULT current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;
