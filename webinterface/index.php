<?php
include("config.php");
// config

$nGroup=$_GET['group'];
$nSwitch=$_GET['switch'];
$nAction=$_GET['action'];
$nDelay=$_GET['delay'];


// actually send
$output = $nGroup.$nSwitch.$nAction.$nDelay;
if (strlen($output) >= 8) {
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
echo "<P>Delay: ";
echo "<A class=\"".($nDelay==0 ? bold:reg)."\" HREF=\"index.php?delay=0\">0</A> | ";
echo "<A class=\"".($nDelay==5 ? bold:reg)."\" HREF=\"index.php?delay=5\">5</A> | ";
echo "<A class=\"".($nDelay==15 ? bold:reg)."\" HREF=\"index.php?delay=15\">15</A> | ";
echo "<A class=\"".($nDelay==30 ? bold:reg)."\" HREF=\"index.php?delay=30\">30</A> | ";
echo "<A class=\"".($nDelay==60 ? bold:reg)."\" HREF=\"index.php?delay=60\">60</A>";
echo "</P>";

$index=0;
echo "<TABLE BORDER=\"0\">\n";
foreach($config as $current) {
  $ig = $current[0];
  $is = $current[1];
  $id = $current[2];

  $socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP) or die("Could not create socket\n");
  socket_bind($socket, $source) or die("Could not bind to socket\n");
  socket_connect($socket, $target, $port) or die("Could not connect to socket\n");

  $output = $ig.$is."2";
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
  if ($index%2 == 0) echo "<TR>\n";
    echo "<TD class=outer ".$color.">\n";
    echo "<TABLE><TR><TD class=inner BGCOLOR=\"#000000\">";
    echo "<A CLASS=\"".$direction."\" HREF=\"?group=".$ig;
    echo "&switch=".$is;
    echo "&action=".$ia;
    echo "&delay=".$nDelay."\">";
    echo "<H3>".$id."</H3><BR />";
    echo $ig.":".$is."<BR />";
    echo "switch ".$direction;
    echo "</A>";
    echo "</TD>";
    echo "</TR></TABLE>\n";
    echo "</TD>\n";
  if ($index%2 == 1) echo "</TR>\n";
  $index++;
  socket_close($socket);
}
echo "</TR></TABLE>";
?>
</body>
</html>
