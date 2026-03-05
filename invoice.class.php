<?php
require_once('security.class.php');
if((!security::instance()->check_ssl())&&
   (isset($_SERVER['HTTP_HOST']))) {
    header("Location: https://".$_SERVER['HTTP_HOST']);
}
if(security::instance()->check_client()) {
 if($_GET[action]=='print')
                {
            // include('../config.php.inc');
             require_once('billing.class.php');
             require_once('mysql.class.php');
            

            $id=$_GET[id];
                        
            billing::instance()->ginvoice($id);
                }
}


?>


