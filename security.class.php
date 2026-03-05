<?php

require_once('config.class.php');
require_once('user.class.php');

class security {
    
    public function check_access() {
        
       if((isset($_GET['page']))&&
          ($_GET['page']=='admin')&&
          ($_SESSION['user']['group']!='admin')) {
            ?>
            <fieldset style="margin-top: 20px;">
                <table width="100%">
                    <tr>
                        <td colspan="3" align="center">
                            <font color="red">
                                <b>Your are not allowed to directly access this page - Access denied!</b>
                            </font>
                        </td>
                    </tr>
                </table>
            </fieldset>
            <?php
            /*
             Clear out everything, except for the session data.
            */
            $_GET=array();
            $_POST=array();
            $_REQUEST=array();
            die;
       }
       
    }
    
    public function check_ssl() {
        
        if((!isset($_SERVER['HTTPS']))||
           ($_SERVER['HTTPS']!="on")) {
            
            return FALSE;
        }
        return TRUE;
    }
    
    public function check_client() {
        
        if((!isset($_SESSION['user']['username']))||
           (!isset($_SESSION['user']['password']))){
            
            if((!isset($_POST['username']))||
               (!isset($_POST['password']))) {
                
                /*Fail Safe, ask the user to login again!*/
                return user::instance()->login();
                
            }else if(user::instance()->login(
                $_POST['username'],$_POST['password'])){
                
                $_SESSION['user']['username']=$_POST['username'];
                $_SESSION['user']['password']=$_POST['password'];
                
                unset($_POST['username'],$_POST['password']);
                
            }else{
                
                return FALSE;
            
            }
            
        }else if(!user::instance()->login()) {
            
            return FALSE;
            
        }
        
        if((!isset($_SESSION['security']['expiry']))||
           (!isset($_SESSION['security']['ip']))) {
            
            $_SESSION['security']['expiry']=time()+config::instance()->getTimeOut();
            
            $_SESSION['security']['ip']=$_SERVER['REMOTE_ADDR'];
                
            
        }else if((time()>$_SESSION['security']['expiry'])||
            ($_SERVER['REMOTE_ADDR']!=$_SESSION['security']['ip'])){
            
            //remove all security info and ask user to re-authenticate...
            unset($_SESSION['security']['expiry'],
                $_SESSION['security']['ip'],
                $_SESSION['user']['username'],
                $_SESSION['user']['password']);
            
            return user::instance()->login();
            
        }else if(time()<=$_SESSION['security']['expiry']){
            
            $_SESSION['security']['expiry']=time()+config::instance()->getTimeOut();
            
        }
        return TRUE;

    }
       
    public function check_input(
        $format = null,
        $input = null
    ) {
       
        if(($format==null)||
           ($input==null)) {
           return FALSE;
        }
         
        if(!preg_match('/'.$format.'/i',$input)) {
           
            return FALSE;
        }
                
        return TRUE;
    }
    
    public static function instance(){
        
        static $instance;/*persistant instance varible*/
        
        if(!isset($instance)) {/*create class if 'instance' was not previously set*/
            
            $instance = new security();
            
        }
        
        return $instance;/*return the class instance to the caller*/
        
    }//instance
    
    private function __construct(){
        
        user::instance();
        
    }//constructor
    
    public function __destruct(){
    
    }//deconstructor
    
}

?>
