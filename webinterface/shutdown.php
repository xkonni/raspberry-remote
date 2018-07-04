<?php
        function runShutdown(){
                exec("/sbin/shutdown -h now");
        }

        if (isset($_GET['shutdown'])) {
                runShutdown();
        }
?>
