<?php
/*
 * Raspberry Remote
 * http://xkonni.github.com/raspberry-remote/
 *
 * webinterface
 *
 * get configuration
 * don't forget to edit config.php
 */
include("config.php");

/*
 * get parameters
 */
if (isset($_GET['sys'])) $nSys=$_GET['sys'];
else $nSys="";
if (isset($_GET['group'])) $nGroup=$_GET['group'];
else $nGroup="";
if (isset($_GET['switch'])) $nSwitch=$_GET['switch'];
else $nSwitch="";
if (isset($_GET['action'])) $nAction=$_GET['action'];
else $nAction="";
if (isset($_GET['delay'])) $nDelay=$_GET['delay'];
else $nDelay=0;

/*
 * actually send to the daemon
 */
$output = $nSys.$nGroup.$nSwitch.$nAction.$nDelay;
if (strlen($output) >= 5) {
  $socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP) or die("Could not create socket\n");
  socket_bind($socket, $source) or die("Could not bind to socket\n");
  socket_connect($socket, $target, $port) or die("Could not connect to socket\n");
  socket_write($socket, $output, strlen ($output)) or die("Could not write output\n");
  socket_close($socket);

  echo "Success!";
}else {
  echo "Please set sys group switch action delay";
}

?>
