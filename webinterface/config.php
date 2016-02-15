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
$source = $_SERVER['SERVER_ADDR'];
$target = '127.0.0.1';
$port = 11337;

/*
 * specify configuration of sockets to use
 *   array("group", "plug", "description");
 * use empty string to create empty box
 *   ""
 *
 */
$config=array(
  array("1", "00100", "01", "Steckdose1"),
  array("1", "00100", "02", "Steckdose2"),
  array("1", "00100", "03", "Steckdose3"),
  array("1", "00100", "04", "Steckdose4"),
  array("1", "00100", "05", "Steckdose5"),
  array("1", "00100", "06", "Steckdose6"),
  array("2", "7"	, "02", "Kueche"),
)
?>
