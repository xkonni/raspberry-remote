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
	array("1","00100","1","SD1"),
	array("1","00100","2","SD2"),
	array("1","00100","3","SD3"),
	array("1","00100","4","SD4"),
	array("1","00100","5","SD5"),
	array("1","00100","6","SD6"),
	array("2","7","2","LichtKueche"),	
 
?>
