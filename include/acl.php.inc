<?php
// $Id: acl.php.inc,v 1.3 2009/03/25 11:35:42 etienne Exp $
include './sql.php.inc';

class acl {
	public static function logged_in(){
		// Return true if user is logged in and a valid user
		if (isset($_SESSION['username'])){
			$username = $_SESSION['username'];
			// the session is set but double check the username is valid
			$results = sql::sql_run("SELECT * FROM `msi_users` WHERE  `username` = '$username'");
			if ( $results[0]['username'] == "$username" ){
				// Session is still valid and the user is still active
				return true; 
			} else {
				// Should never get here but if it does its best to not log them in
				return false;
			}
		} 
		else {
			// no session set this person is not logged in
			return false;
		}
	}

	// 760129ae0a6c90e9

	public static function login($username,$password){

		if ($username == "" || $password == ""){
			print '<b>Please supply a Username and Password.</b><p>';
		} else {
			// $results = sqlQueries::sql_run("SELECT * FROM `msi_users` WHERE `username` = '$username' AND OLD_PASSWORD('$password') = password;");
			$results = sql::sql_run("SELECT * FROM `msi_users` WHERE `username` = '$username';");
			// $results = sql::sql_run("SELECT * FROM `msi_users` WHERE `username` = '$username' AND OLD_PASSWORD('$password') = password;");
			// $results = sql::sql_run("SELECT * FROM `msi_users` WHERE `username` = '$username' AND OLD_PASSWORD('$password') = password;");
			// OLD_PASSWORD('$password')
			if ( $results[0]['username'] == $username ){
				//we can log them in
				$_SESSION['username'] = $username; 
				$_SESSION['id'] = $results[0]['id']; 
			} else {
				// Wrong username and password combo
				print '<b>The Username and Password you entered are incorrect. <br>Please try again or seek assitance.</b><p>';
				session_destroy(); // just to be safe delete any session that is active
			}
		}
	}


	public static function login_form(){
		// display form used for logins
		print '<div class="login">
			<form action="?action=login" method="POST">		
				<h2>Mindspring Intranet Login</h2>
				<table>
					<tr><td> Username :</td><td><input type="input" name="username" size="15"></td></tr>
					<tr><td>Password :</td><td><input type="password" name="password" size="15"></td>
					<tr><td colspan="2"><input type="submit" value="LOGIN"></td></tr>
				</table>
			</form>
		</div>
		';
	}

	public static function has_access($user,$module,$type){
		$sql = 'SELECT msi_acl.id, msi_module.name, msi_acl.user FROM msi_acl, msi_module 
			WHERE msi_acl.module = msi_module.id 
			AND msi_acl.user = '.$user.' and msi_acl.'.$type.' = 1 AND (msi_module.name = "all" or msi_module.name = "'.$module.'" )';

		# print "$sql <br>";
		$result = sql::sql_run($sql);
		if (count($result) > 0){
			return true;
		} else { 
			print "<div class=\"bolder\">User ".$_SESSION['username']." with id $user, does not have $type access to $module</div>
			If you require access to this part of the intranet please copy and paste the error message from above and email it to support@mindspring.co.za with a brief description.<br>";
			return false;
		}
	
	}
}


?>






