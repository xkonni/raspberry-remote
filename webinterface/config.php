<?php
/*
 * Raspberry Remote
 * http://xkonni.github.com/raspberry-remote/
 *
 * configuration for the webinterface
 *
 */

/*
 * define ip address and port here
 */
$target = '192.168.11.124';
$port = 11337;

/*
 * specify configuration of sockets to use
 *   array("group", "plug", "description");
 * use empty string to create empty box
 *   ""
 *
 */
$config=array(
  array("00010", "01", "Schreibtisch"),
  array("00010", "02", "Laptop"),
  "",
  "",
  array("00001", "01", "Sofa Lampe"),
  array("00001", "02", "Sofa Steckdose"),
  array("00001", "03", "TV"),
  array("00001", "04", "Verstaerker"),
)
?>
