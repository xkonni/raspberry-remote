<html>
  <head>
    <title>raspberry</title>
  </head>
<body>
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
if (strlen($output) >= 8) {
  $socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP) or die("Could not create socket\n");
  socket_bind($socket, $source) or die("Could not bind to socket\n");
  socket_connect($socket, $target, $port) or die("Could not connect to socket\n");
  socket_write($socket, $output, strlen ($output)) or die("Could not write output\n");
  echo "ACTION: ";
  echo "group: ".$nGroup;
  echo " switch: ".$nSwitch;
  echo " action: ".$nAction;
  echo "=> ".socket_read($socket, 2048)."<BR /><BR />";
  socket_close($socket);
}
else if (strlen($OUTPUT) > 0) {
  echo "ACTION: ";
  echo $output." too short";
}

$groups=array("00001", "00010");
echo "<TABLE BORDER=\"0\">";
foreach($groups as $ig) {
  echo "<TR>";
  for ($is=1; $is<=5; $is++) {
    $socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP) or die("Could not create socket\n");
    socket_bind($socket, $source) or die("Could not bind to socket\n");
    socket_connect($socket, $target, $port) or die("Could not connect to socket\n");

    $output = $ig."0".$is."2";
    socket_write($socket, $output, strlen ($output)) or die("Could not write output\n");
    $state = socket_read($socket, 2048);
    if ($state == 0) {
      $color=" BGCOLOR=\"#FF0000\"";  
      $action = 1;
    }
    if ($state == 1) {
      $color=" BGCOLOR=\"#00FF00\"";  
      $action = 0;
    }
    echo "<TD ".$color.">";
    echo "<A HREF=\"?group=".$ig;
    echo "&switch=0".$is;
    echo "&action=".$action;
    echo "\">".$ig.":".$is."<BR /> => ".$action."</A>";
    socket_close($socket);
  }
  echo "</TR>";
}
echo "</TABLE>";
?>
</body>
</html>
