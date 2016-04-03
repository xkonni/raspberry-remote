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
 *   array("systemcode", "group" , "plug", "description");
 * use empty string to create empty box
 *   ""
 *
 */
$config=array(
  array("1", "00100", "01", "PowerPlug1"),
  )
?>
