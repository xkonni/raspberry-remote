<?php
// config
$source = $_SERVER['SERVER_ADDR'];
$target = '192.168.11.124';
$port = 12345;

$nGroup=$_GET['group'];
$nSwitch=$_GET['switch'];
$nAction=$_GET['action'];

// actually send
$output = $nGroup.$nSwitch.$nAction;
if (strlen($output) == 8) {
  $socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP) or die("Could not create socket\n");
  socket_bind($socket, $source) or die("Could not bind to socket\n");
  socket_connect($socket, $target, $port) or die("Could not connect to socket\n");
  socket_write($socket, $output, strlen ($output)) or die("Could not write output\n");
  socket_close($socket);
  header("Location: index.php");
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


$names=array("Schreibtisch","Sofa", "TV", "Verstaerker", "e", "f", "g", "h", "i", "j", "k");
$groups=array("00001", "00010");
$index=0;
echo "<TABLE BORDER=\"0\"><TR>";
foreach($groups as $ig) {
  for ($is=1; $is<=5; $is++) {
    $socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP) or die("Could not create socket\n");
    socket_bind($socket, $source) or die("Could not bind to socket\n");
    socket_connect($socket, $target, $port) or die("Could not connect to socket\n");

    $output = $ig."0".$is."2";
    socket_write($socket, $output, strlen ($output)) or die("Could not write output\n");
    $state = socket_read($socket, 2048);
    if ($state == 0) {
      $color=" BGCOLOR=\"#C00000\"";
      $action = 1;
      $done="switch on";
    }
    if ($state == 1) {
      $color=" BGCOLOR=\"#00C000\"";
      $action = 0;
      $done="switch off";
    }
    if (($ig == $nGroup) && ($is == $nSwitch)) {
      if ($state == 1) {
        $done="switched on";
        $color=" BGCOLOR=\"#00FF00\"";
      }
      else if ($state == 0) {
        $done="switched off";
        $color=" BGCOLOR=\"#FF0000\"";
      }
    }
    echo "<TD class=outer ".$color.">";
      echo "<TABLE><TR><TD class=inner BGCOLOR=\"#000000\">";
      echo "<A HREF=\"?group=".$ig;
        echo "&switch=0".$is;
        echo "&action=".$action."\">";
        echo "<H3>".$names[$index]."</H3><BR />";
        echo $ig.":".$is."<BR />";
        echo $done;
      echo "</A>";
      echo "</TD></TR></TABLE>";
    echo "</TD>";
    // socket_close($socket);
    if ($index%2 == 1) {
      echo "</TR><TR>";
    }
    $index++;
    socket_close($socket);
  }
}
echo "</TR></TABLE>";
?>
</body>
</html>
