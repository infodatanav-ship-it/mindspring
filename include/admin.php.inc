<?php 
// $Id: admin.php.inc,v 1.4 2009/03/25 11:35:42 etienne Exp $

class admin {

	function navigation(){
		print '<div class="sec-links"><ul>
					<li> <a href="?module=admin">Permissions</a></li>
					<li> <a href="?module=admin&action=users">Users</a></li>
					<li> <a href="?module=admin&action=add_user_form">Add User</a></li></ul>
				</div>';
	}


	//	function add_user($username,$email, $password,$rate,$hrate){


	function add_user($username,$password, $rate,$hrate,$email){

		if(!acl::has_access($_SESSION['id'],"admin","add")) return 1;

		$sql = 'insert into msi_users(username,password,rate,hrate,mail) values("'.$username.'", OLD_PASSWORD("'.$password.'"),'.$rate.', '.$hrate.',"'.$email.'")';
		sql::sql_run($sql);
		print "Added user $username";
	}

	function add_user_form(){
		if(!acl::has_access($_SESSION['id'],"admin","add")) return 1;
		
		print '<span class="head"> Add User '.$result[0]['username'].'</span>
			<table class="box"><form action="?module=admin&action=add_user" method="POST"
				<tr><td><b>Username :</b></td><td><input name="username" value=""></td></tr>
				<tr><td><b>Email :</b></td><td><input name="email" value=""></td></tr>
				<tr><td><b>Password:</b></td><td><input name="password" value="PASSW0RD"></td></tr>
				<tr><td><b>Rate :</b></td><td><input name="rate" value=""></td></tr>
				<tr><td><b>Hour Rate :</b></td><td><input name="hrate" value=""></td></tr>
				<tr><td colspan="2" align="center"><input type="submit" value="submit"></td></tr>
			</table>
		</form>';
	}

	function list_users(){
		if(!acl::has_access($_SESSION['id'],"admin","read")) return 1;

		$sql = 'select * from msi_users order by username';
		$results = sql::sql_run($sql);
		print '<table class="client">
			<tr class="heading"><td>Username</td><td>Actions</td>';
		foreach ($results as $result){
		print '<tr>
			<td class="client">'.$result['username'].'</td>
			<td class="client"><a href="?module=admin&action=edit_user_form&id='.$result['id'].'">Edit</a></td>
		</tr>';
		}
		print '</table>';

	}

	function edit_user_form($id){
		if(!acl::has_access($_SESSION['id'],"admin","edit")) return 1;

		$sql= 'select * from msi_users where id = '.$id.' limit 1';

		$result = sql::sql_run($sql);

		print '<span class="head"> Edit User ('.$result[0]['username'].')</span>
			<table class="box"><form action="?module=admin&action=edit_user&id='.$id.'" method="POST"
				<tr>
					<td>
						<b>Username :</b>
					</td>
					<td>
						<input name="username" value="'.$result[0]['username'].'">
					</td>
				</tr>
				<tr>
					<td>
						<b>Email :</b>
					</td>
					<td>
						<input name="email" value="'.$result[0]['mail'].'">
					</td>
				</tr>

				<tr>
					<td>
						<b>Password :</b>
					</td>
					<td>
						<input name="password" value="">
					</td>
				</tr>
				<tr>
					<td>
						<b>Rate :</b>
					</td>
					<td>
						<input name="rate" value="'.$result[0]['rate'].'">
					</td>
				</tr>

				<tr>
					<td>
						<b>Hour Rate :</b>
					</td>
					<td>
						<input name="hrate" value="'.$result[0]['hrate'].'">
					</td>
				</tr>
				<tr>
					<td>
						<b>Color :</b>
					</td>

					<td>
						<select name="color">
							<option value="'.$result[0]['color'].'">'.$result[0]['color'].'</option> >
							<option value="black">black</option>
							<option value="teal">teal</option>
							<option value="navy">navy</option>
							<option value="purple">purple</option>
							<option value="green">green</option>
							<option value="olive">olive</option>
							<option value="maroon">maroon</option>
							<option value="aqua">aqua</option>
							<option value="blue">blue</option>
							<option value="fuchsia">fuchsia</option>
							<option value="silver">silver</option>
							<option value="lime">lime</option>
							<option value="yellow">yellow</option>
							<option value="red">red</option>
						</select>
					</td>
				</tr>

				<tr>
					<td>
						<b>Active :</b>
					</td>
					<td>
						<select name="activetechie">
							<option value="'.$result[0]['activetechie'].'">'.$result[0]['activetechie'].'</option>
							<option value="yes">yes</option>
							<option value="no">no</option>
						</select>
					</td>
				</tr>

				<tr>
					<td colspan="2" align="center"><input type="subContract Hours Assignedmit" value="submit"></td>
				</tr>
			</table>
		</form>';

	}

  function edit_user($id,$username,$email,$password,$rate,$hrate,$color,$activetechie){
  if(!acl::has_access($_SESSION['id'],"admin","edit")) return 1;
  if ($password == "") { $password_sql ="";}  else {$password_sql = ', password = OLD_PASSWORD("'.$password.'")' ; }
  $sql  = 'update msi_users set activetechie = "'.$activetechie.'", color = "'.$color.'", username = "'.$username.'", rate = "'.$rate.'",hrate = "'.$hrate.'", mail = "'.$email.'" '.$password_sql.' where id = "'.$id.'"';
  print "Updating user $username";
  sql::sql_run($sql);
  }

  function add_perm($id, $module_id,$read,$add,$edit){
  if(!acl::has_access($_SESSION['id'],"admin","add")) return 1;

  $myresult=sql::sql_run('SELECT count(*) FROM msi_acl WHERE user = "'.$id.'" AND `module` = "'.$module_id.'"');
	if ($myresult[0][0] != '0') 
	{
		echo 'This user already has permissions for this module, updating those instead';
		$result = sql::sql_run('UPDATE msi_acl SET `read` = "'.$read.'" , `add` = "'.$add.'" , `edit` = "'.$edit.'" WHERE user = "'.	$id.'" AND `module` = "'.$module_id.'"'); 
	} else {
		//if ($add=='1') {$read='1';}
		//if ($edit=='1') {$read='1'; $add='1';}
  	print "Added permsions for user id ". $id;
  	$sql ='insert into msi_acl set user = "'.$id.'", module = "'.$module_id.'", read = "'.$read.'" , add = "'.$add.'" , edit = "'.$edit.'"'; 
  	$result = sql::sql_run('insert into msi_acl set user = "'.$id.'", `module` = "'.$module_id.'", `read` = "'.$read.'" , `add` = "'.$add.'" , `edit` = "'.$edit.'"');
	} 
  admin::list_perm(); 
  admin::add_perm_form(); 
  
  }
 
  function del_perm($id){
  if(!acl::has_access($_SESSION['id'],"admin","add")) return 1;
    $result = sql::sql_run('delete from msi_acl where id = "'.$id.'"');
    print "Deleted ACL with id $id"; 
  admin::add_perm_form(); 
  admin::list_perm(); 

  } 
  
  function list_perm(){
  if(!acl::has_access($_SESSION['id'],"admin","read")) return 1;

 //  $link = mysql_connect("intranetvm.mindspring.local", "admin", "qjXq9sQmRKez98jB")
//		or die("Could not connect : " . mysql_error());
//	mysql_select_db("msi") or die("Could not select database");
	
	
	
// $link = mysqli_connect("intranetvm.mindspring.local", "msi_user", "hag!hd3@1zfa","msi");
		
// if (!$link) {
// 	echo "Error: Unable to connect to MySQL." . PHP_EOL;
// 	echo "Debugging errno: " . mysqli_connect_errno() . PHP_EOL;
// 	echo "Debugging error: " . mysqli_connect_error() . PHP_EOL;
// 	exit;
// }

	//	or die("could not connect : " . mysqli_error());
	// mysql_select_db("msi") or die("could not select database" . mysql_error());
	//$result = mysqli_query($link,$query) or die("query failed : " . mysqli_error($link));
//	$values;
	
// $myresults = mysqli_query($link,'
$myresults = sql::sql_run("SELECT `username` , `name` , CONCAT(`read` , `edit` , `add`)
FROM (

SELECT `msi_users`.`username` , `msi_users`.`id` AS user_id, `msi_module`.`id` AS module_id, `msi_module`.`name`
FROM `msi_users` , `msi_module` 
) AS BOB
LEFT JOIN (
`msi_acl`
) ON ( BOB.`user_id` = `msi_acl`.`user`
AND BOB.`module_id` = `msi_acl`.`module` )
ORDER BY `username` , `name`
LIMIT 0 , 1000 ");

// var_dump($myresults);


	echo '<table style="border:1px solid black">
	  <tr class="heading">
           	<td>User</td>
		<td>address book</td>
		<td>admin</td>
		<td>adsl</td>
		<td>adsl_password</td>
		<td>ALL</td>
		<td>billing</td>
		<td>billing admin</td>
		<td>billing_edit</td>    
		<td>contractadmin</td>   
		<td>contracts</td>
		<td>dialups</td>
		<td>documents admin</td> 
		<td>invoice</td>
		<td>password</td>
	     <td>tier</td>
		<td>ics</td>';  

	$currentname='';

	// while ($row=mysqli_fetch_row($myresults)) {

	foreach ($myresults as $row) {
		if ($currentname!=$row[0]) {
			$currentname=$row[0];
			echo '</tr><tr><td>'.$row[0].'</td>';
		}
		
		if ($row[2]=='100') {
			echo '<td>read</td>';
		} elseif ($row[2]=='110') {
			echo '<td>edit</td>';
		} elseif ($row[2]=='111') {
			echo '<td>add</td>';
		} else {
			echo '<td>---</td>';
		}
		$currentname=$row[0];
			

	}
	echo '</tr></table>';
}

  function add_perm_form(){
 if(!acl::has_access($_SESSION['id'],"admin","add"))  return 1;
    $results = sql::sql_run('select id, name from msi_module order by name');
    $results2 = sql::sql_run('select id, username from msi_users order by username');
    print '<h2>Add a Permission</h2>
    <form action="?action=add_perm&module=admin" method="POST">
      <b>Module</b> <select name="module_id">';
    foreach ($results as $result){
      print '<option value="'.$result[0].'">'.$result[1].'</option>';
    }
	
    print '               </select>
      <b>Username</b> <select name="id">';
    foreach ($results2 as $result2){
      print '<option value="'.$result2[0].'">'.$result2[1].'</option>';
    }
    print '               </select>
      <b>Read</b> <input type="checkbox" name="read" value="1">
      <b>Edit</b> <input type="checkbox" name="edit" value="1">
      <b>Add</b> <input type="checkbox" name="add" value="1">
      <input type="submit" value="add">
    </form> ';
	

  } 
  }

?>
