<?php
/*
 * Raspberry Remote
 * http://xkonni.github.com/raspberry-remote/
 *
 * webinterface
 *
*/

/*
 * get configuration
 * don't forget to edit config.php
 */
include("config.php");
include("shutdown.php");

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
 * then reload the webpage without parameters
 * except for delay
 */
$output = $nSys.$nGroup.$nSwitch.$nAction.$nDelay;
if (strlen($output) >= 5) {
  $socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP) or die("Could not create socket\n");
  socket_bind($socket, $source) or die("Could not bind to socket\n");
  socket_connect($socket, $target, $port) or die("Could not connect to socket\n");
  socket_write($socket, $output, strlen ($output)) or die("Could not write output\n");
  socket_close($socket);
  header("Location: index.php?delay=$nDelay");
}
?>
<html>
  <head>
    <title>raspberry</title>
    <link rel="stylesheet" href="style.css">
    <link rel="icon"
          type="image/png"
          href="favicon.png">
    <meta name="viewport"
          content="
              height = device-height,
              width = device-width,
              initial-scale = 1.0,
              user-scalable = no ,
              target-densitydpi = device-dpi
              " />
  </head>
<body>
<?php
/*
 * links to change the delay
 */
echo "<P>Delay: ";
echo "<A";
if ($nDelay == 0) echo " class=\"bold\"";
echo " HREF=\"index.php?delay=0\">0</A> | ";
echo "<A";
if ($nDelay == 5) echo " class=\"bold\"";
echo " HREF=\"index.php?delay=5\">5</A> | ";
echo "<A";
if ($nDelay == 15) echo " class=\"bold\"";
echo " HREF=\"index.php?delay=15\">15</A> | ";
echo "<A";
if ($nDelay == 30) echo " class=\"bold\"";
echo " HREF=\"index.php?delay=30\">30</A> | ";
echo "<A";
if ($nDelay == 60) echo " class=\"bold\"";
echo " HREF=\"index.php?delay=60\">60</A> ";
echo "</P>";

/*
 * table containing all configured sockets
 */
$index=0;
echo "<TABLE BORDER=\"0\">\n";
foreach($config as $current) {
  if ($current != "") {
    $iSys = $current[0];
    $ig = $current[1];
    $is = $current[2];
    $id = $current[3];

    if ($index%2 == 0) echo "<TR>\n";

    $socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP) or die("Could not create socket\n");
    socket_bind($socket, $source) or die("Could not bind to socket\n");
    socket_connect($socket, $target, $port) or die("Could not connect to socket\n");

    $output = $iSys.$ig.$is."2";
    socket_write($socket, $output, strlen ($output)) or die("Could not write output\n");
    $state = socket_read($socket, 2048);
    if ($state == 0) {
      $color=" BGCOLOR=\"#C00000\"";
      $ia = 1;
      $direction="on";
    }
    if ($state == 1) {
      $color=" BGCOLOR=\"#00C000\"";
      $ia = 0;
      $direction="off";
    }
    echo "<TD class=outer ".$color.">\n";
    echo "<TABLE><TR><TD class=inner BGCOLOR=\"#000000\">";
    echo "<A CLASS=\"".$direction."\" HREF=\"?group=".$ig;
    echo "&sys=".$iSys;
    echo "&switch=".$is;
    echo "&action=".$ia;
    echo "&delay=".$nDelay."\">";
    echo "<H3>".$id."</H3><BR />";
    echo $iSys.":".$ig.":".$is."<BR />";
    echo "switch ".$direction;
    echo "</A>";
    echo "</TD>";
    echo "</TR></TABLE>\n";
    echo "</TD>\n";
    socket_close($socket);
  }
  else {
    echo "<TD></TD>\n";
  }
  if ($index%2 == 1) echo "</TR>\n";
  $index++;
}
echo "</TR></TABLE>";
?>
<a id="shutdown" href='shutdown.php?shutdown=true'>Shutdown System</a>
</body>
</html>
