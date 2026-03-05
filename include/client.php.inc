<?php
// $Id: client.php.inc,v 1.5 2008/03/18 15:23:40 shane Exp $ 

class client {
	var $id;
	var $company;
	var $lastname;
	var $firstname;
	var $telephone;
	var $fax;
	var $cellphone;
	var $home;
	var $email;
	var $saddress1;
	var $saddress2;
	var $szip;
	var $paddress1;
	var $paddress2;
	var $pzip;
	var $customer;


	public function client($id, $company, $lastname, $firstname, $telephone, $fax, $cellphone,$home , $email, $saddress1, $saddress2, $szip, $paddress1, $paddress2, $pzip, $customer){
		
		if($customer == 'on'){ $customer = "1"; } else {$customer = "0";};

		$sql = "insert into msi_clients set company = \"$company\" , lastname = \"$lastname\", firstname = \"$firstname\",
			telephone = \"$telephone\", fax = \"$fax\", cellphone = \"$cellphone\", home = \"$home\", email = \"$email\",
			saddress1 = \"$saddress1\", saddress2 = \"$saddress2\", szip = \"$szip\", paddress1 = \"$paddress1\",
			paddress2 = \"$paddress2\", pzip = \"$pzip\", customer =\"$customer\"";

		// var_dump($sql);

		$result = sql::sql_run($sql);

		// $last_id = sql::getLastInsertId();
		// echo "Last inserted ID: " . $last_id;

		$result = sql::sql_run('SELECT MAX(`id`) FROM `msi_clients`;');

		// var_dump($result[0][0]);

		client::info($result[0][0]);

		// if(!isset($result)){
		// 	print "added client<br>";
		// 	$sql = "select id from msi_clients where company = \"$company\" and lastname = \"$lastname\" and firstname = \"$firstname\" and
		// 		telephone = \"$telephone\" and fax = \"$fax\" and cellphone = \"$cellphone\" and home = \"$home\" and email = \"$email\" and
		// 		saddress1 = \"$saddress1\" and saddress2 = \"$saddress2\" and szip = \"$szip\" and paddress1 = \"$paddress1\" and
		// 		paddress2 = \"$paddress2\" and pzip = \"$pzip\" and  customer =\"$customer\" limit 1";
		// 	$result = sql::sql_run($sql);
		// 	/* commented dubugging lines, 25/01/08 <psycode> 
		// 	print "$sql";
		// 	print_r($result); */
		// 	client::info($result[0][0]);
		// } else {
		// 	print "<b>Something went wrong.....</b><br>I tried to run $sql";
		// }

	}


	function update($id, $company, $lastname, $firstname, $telephone, $fax, $cellphone, $home,  $email, $saddress1, $saddress2, $szip, $paddress1, $paddress2, $pzip, $customer){
		if($customer == 'on'){ $customer = "1"; } else {$customer = "0";};

		$sql = "update msi_clients set company = \"$company\" , lastname = \"$lastname\", firstname = \"$firstname\",
			telephone = \"$telephone\", fax = \"$fax\", cellphone = \"$cellphone\", home = \"$home\", email = \"$email\",
			saddress1 = \"$saddress1\", saddress2 = \"$saddress2\", szip = \"$szip\", paddress1 = \"$paddress1\",
			paddress2 = \"$paddress2\", pzip = \"$pzip\",customer =\"$customer\" where id = \"$id\" "; 

		$result = sql::sql_run($sql);
		
		client::info($id);

		/*
		if(!isset($result)){
		} else {
			print "<b>Something went wrong.....</b><br>I tried to run $sql";
		}
		*/

	}

	function edit_form($client_id) {

		$result = sql::sql_run("select company, firstname, lastname, telephone, fax, cellphone, email, saddress1, saddress2, szip, paddress1, paddress2, pzip, home , customer
		from msi_clients where id = \"$client_id\" limit 1");
		
		if($result[0][14] == '1'){ $checked = "checked"; } else {$checked = "unchecked"; ;};
		
		print "<span class=\"head\">Editing Client Info</span>
			<table class=\"box\">
			<tr>
				<td align=\"right\">
				<form action=\"index.php?module=client&client_id=$client_id\" method=\"POST\">
				<b>Company Name: </b>
				</td>
				<td>
				<input type=\"input\" name=\"company\" size=\"40\" value=\"".$result[0][0]."\">
				</td>
			</tr>
			<tr>
				<td align=\"right\">    
				<b>Firstname: </b>
				</td>
				<td>
				<input type=\"input\" name=\"firstname\" size=\"40\" value=\"".$result[0][1]."\">
				</td>
			</tr>
			<tr>
				<td align=\"right\">
				<b>Lastname: </b>
				</td>
				<td>
				<input type=\"input\" name=\"lastname\" size=\"40\" value=\"".$result[0][2]."\">
				</td>
			</tr>
			<tr>
				<td align=\"right\">
				<b>Work Telephone: </b>
				</td>
				<td>
				<input type=\"input\" name=\"telephone\" size=\"40\" value=\"".$result[0][3]."\">
				</td>
			</tr>
			<tr>
				<td align=\"right\">
				<b>Fax: </b>
				</td>
				<td>
				<input type=\"input\" name=\"fax\" size=\"40\" value=\"".$result[0][4]."\">
				</td>
			</tr>
					<tr>
				<td align=\"right\">
				<b>Cell: </b>
				</td>
				<td>
				<input type=\"input\" name=\"cellphone\" size=\"40\" value=\"".$result[0][5]."\">
				</td>
			</tr>
			<tr>
				<td align=\"right\">
				<b>Home Telephone: </b>
				</td>
				<td>
				<input type=\"input\" name=\"home\" size=\"40\" value=\"".$result[0][13]."\">
				</td>
			</tr>
			<tr>
				<td align=\"right\">
				<b>Email: </b>
				</td>
				<td>
				<input type=\"input\" name=\"email\" size=\"40\" value=\"".$result[0][6]."\">
				</td>
			</tr>
			<tr>
				<td align=\"right\" valign=\"top\">
				<b>Street Address: </b>
				</td>
				<td>
				<input type=\"input\" name=\"saddress1\" size=\"40\" value=\"".$result[0][7]."\"><br>
				<input type=\"input\" name=\"saddress2\" size=\"40\" value=\"".$result[0][8]."\"><br>
				<input type=\"input\" name=\"szip\" size=\"5\" value=\"".$result[0][9]."\">
				</td>
			</tr>
			<tr>
				<td align=\"right\" valign=\"top\">       			
				<b>Postal Address: </b>
				</td>
				<td>
				<input type=\"input\" name=\"paddress1\" size=\"40\" value=\"".$result[0][10]."\"><br>
				<input type=\"input\" name=\"paddress2\" size=\"40\" value=\"".$result[0][11]."\"><br>
				<input type=\"input\" name=\"pzip\" size=\"5\" value=\"".$result[0][12]."\"><br>
				</td>
			</tr>

			<tr>
				<td colspan=\"2\">
			<b>Are they a Mindspring customer?</b> <input type=\"checkbox\" $checked name=\"customer\">
			</td>
			</tr>
			
				<tr>
					<td align=\"right\">       			
					<input type=\"submit\" value=\"update\" name=\"action\">
					</td>
					<td>
					<input type=\"submit\" value=\"delete\" name=\"action\"></form>                
					</td>
				</tr>
			</table>
		"; 
	
	}
	
	function add_form(){
		print "<span class=\"head\">New Client Info</span>
				<table class=\"box\">
				<tr>
					<td align=\"right\">
					<form action=\"index.php?module=client&action=add\" method=\"POST\">
					<b>Company Name: </b>
					</td>
					<td>
					<input type=\"input\" name=\"company\" size=\"40\" value=\"\">
					</td>"./* added this 18/03/08 Required field warning for staff */
					"<td> <b>*Required Field </b></td>
				</tr>
				<tr>
					<td align=\"right\">    
					<b>Firstname: </b>
					</td>
					<td>
					<input type=\"input\" name=\"firstname\" size=\"40\" value=\"\">
					</td>"./* added this 18/03/08 Required field warning for staff */
						
					"<td><b>*Required Field</b></td>
				</tr>
				<tr>
					<td align=\"right\">
					<b>Lastname: </b>
					</td>
					<td>
					<input type=\"input\" name=\"lastname\" size=\"40\" value=\"\">
					</td>"./* added this 18/03/08 Required field warning for staff */
				"<td> <b>*Required Field </b></td>
				</tr>
				<tr>
					<td align=\"right\">
					<b>Work Telephone: </b>
					</td>
					<td>
					<input type=\"input\" name=\"telephone\" size=\"40\" value=\"\">
					</td>
				</tr>
				<tr>
					<td align=\"right\">
					<b>Fax: </b>
					</td>
					<td>
					<input type=\"input\" name=\"fax\" size=\"40\" value=\"\">
					</td>
				</tr>
				<tr>
					<td align=\"right\">
					<b>Cell: </b>
					</td>
					<td>
					<input type=\"input\" name=\"cellphone\" size=\"40\" value=\"\">
					</td>
				</tr>
				<tr>
					<td align=\"right\">
					<b>Home Telephone:</b>
					</td>
					<td>
					<input type=\"input\" name=\"home\" size=\"40\" value=\"\">
					</td>
				</tr>
				<tr>
					<td align=\"right\">
					<b>Email: </b>
					</td>
					<td>
					<input type=\"input\" name=\"email\" size=\"40\" value=\"\">
					</td>
				</tr>
				<tr>
					<td align=\"right\" valign=\"top\">
					<b>Street Address: </b>
					</td>
					<td>
					<input type=\"input\" name=\"saddress1\" size=\"40\" value=\"\"><br>
					<input type=\"input\" name=\"saddress2\" size=\"40\" value=\"\"><br>
					<input type=\"input\" name=\"szip\" size=\"5\" value=\"\">
					</td>
				</tr>
				<tr>
					<td align=\"right\" valign=\"top\">       			
					<b>Postal Address: </b>
					</td>
					<td>
					<input type=\"input\" name=\"paddress1\" size=\"40\" value=\"\"><br>
					<input type=\"input\" name=\"paddress2\" size=\"40\" value=\"\"><br>
					<input type=\"input\" name=\"pzip\" size=\"5\" value=\"\"><br>
					</td>
				</tr>
			<tr>
				<td align=\"right\" colspan=\"2\">
			<b>Are they a Mindspring customer?</b> <input type=\"checkbox\" name=\"customer\">
			</td>
			</tr>
				<tr>
					<td> &nbsp&nbsp&nbsp</td>
					<td align=\"right\">
					<input type=\"submit\" value=\"add\" name=\"action\">
					</td>
					<td>
					</form>                
					</td>
				</tr>
		</table>"; 
	
	}
	
	

	function accounts($client_id){
		#print "acounts for $client_id"; 
		$saix = "-1";
		$servers = "-1";
		$ms_dialups = sql::sql_run("select userid, password from msi_radius where client = \"$client_id\"");
		if (count($saix) > 0){ 
			print "
			<span class=\"head\">Saix Accounts 08600 7249</span>
			<table class=\"client\">
			<tr class=\"head\"><td>Type</td><td>Username</td><td>Password</td><td>Note</td></tr>";
			foreach($saix as $account){
				print "<tr><td  class=\"client\">$account[0]</td><td  class=\"client\">$account[1]</td><td  class=\"client\">$account[2]</td>
			<td  class=\"client\">$account[3]</td></tr>";
			}
				print '</table><br>';	    
			}
			
		if (count($servers) > 0){
			print "<span class=\"head\">Servers</span>
			<table class=\"client\">
			<tr class=\"head\"><td>Name</td><td>Connection</td><td>OS</td></tr>";
			foreach($servers as $server){
				print "<tr><td class=\"client\">$server[0]</td><td class=\"client\">$server[1]</td><td class=\"client\">$server[2]</td></tr>";
			}
			print '</table><br>';
			}

		if (count($ms_dialups) > 0){ 
			print "
			<span class=\"head\">Mindspring Dialup 021 657 9999</span>
			<table class=\"client\">
			<tr class=\"head\"><td>Username</td><td>Password</td></tr>";
			foreach($ms_dialups as $ms_dialup){
				print "<tr><td class=\"client\">$ms_dialup[0]</td><td class=\"client\">$ms_dialup[1]</td></tr>";
			}
			print '</table><br>';
			}
		
	}

	function info($client_id){
		$result = sql::sql_run("select company, firstname, lastname, telephone, fax, cellphone, email, saddress1, saddress2, szip, paddress1, paddress2, pzip, home 
			from msi_clients where id = \"$client_id\" limit 1");
		print "<p></p><br>
		
		<span class=\"head\">Client Info</span>
		<p></p>
				<table class=\"client\"><tr><td class=\"client\"><h3><b>".$result[0][0]."</b></h3><br>
				<table border = 1>
		<tr><td><b>Contact Person : </b></td>  
			<td> "  .$result[0][1]." ".$result[0][2]." </tr></td> 
		<tr><td><b>Telephone : </b></td> 
		<td>".$result[0][3]."</tr></td> 
		<tr><td> <b>Fax : </b></td> 
		<td> " .$result[0][4]." </tr></td> 
			<tr><td><b>Cell : </b></td> 
			<td>".$result[0][5]."</tr></td>  
		<tr><td><b>Home : </b></td> 
		<td>".$result[0][13]."</tr></td> 
			<tr><td><b>Email :</b></td> 
			<td>".$result[0][6]."</tr></td> 
			<tr><td><b>Street Address : </b></td> 
		<td>".$result[0][7].", ".$result[0][8].", ".$result[0][9]."</tr></td> 
			<tr><td><b>Postal Address : </b></td> 
			<td>".$result[0][10].", ".$result[0][11].", ".$result[0][12]."</tr></td> 
		</table><br>
		<form action=\"index.php?module=client&client_id=$client_id\" method=\"POST\"><input type=\"submit\" value=\"edit\" name=\"action\">&nbsp;&nbsp;&nbsp;&nbsp;<input type=\"submit\" value=\"delete\" name=\"action\"></form>
		
		</td></tr></table>";
		
		
		
		
		
		/*    print "<span class=\"head\">Client Info</span>
			<table class=\"client\">
			<tr><td class=\"client\"><b>".$result[0][0]."</b>
				
			
			<tr><td><b>Contact Person: </b>".$result[0][1]." ".$result[0][2]."<br>
		<b>Contact Person: </b>".$result[0][1]." ".$result[0][2]."<br>
		<b>Telephone: </b>".$result[0][3]."<br> 
		<b>Fax: </b>" .$result[0][4]." <br>
		<b>Cell:</b>".$result[0][5]."<br> 
		<b>Home: </b>".$result[0][13]."<br>
		<b>Email: </b>".$result[0][6]."<br>
		<b>Street Address: </b>".$result[0][7].", ".$result[0][8].", ".$result[0][9]."<br>
		<b>Postal Address: </b>".$result[0][10].", ".$result[0][11].", ".$result[0][12]."<br><br>
		<form action=\"index.php?module=client&client_id=$client_id\" method=\"POST\"><input type=\"submit\" value=\"edit\" name=\"action\"><input type=\"submit\" value=\"delete\" name=\"action\"></form>
		</td></tr></table>";*/
			

		
	}

	function delete($client_id){
		// check there are no servers, dialup accounts or siax accounts else dont delete
		$saix = 0;
		$servers = 0;
		$ms_dialups = sql::sql_run("select userid, password from msi_radius where client = \"$client_id\"");
		
		if($saix == 0 && $servers == 0 && count($ms_dialups) == 0){
			print "This Company has no accounts with us, its safe to delete<br>
				<form action=\"index.php?module=client&client_id=$client_id\" method=\"POST\"><input type=\"submit\" name=\"action\" value=\"remove\"></form>";
		} else {
			print "<b>Stop!</b><br>
						I wont let you delete this client because they have accounts with us still.<p>";
		}
	}
	
	function remove($client_id){

		print "Deleted client with client ID: $client_id";
		$result = sql::sql_run("DELETE FROM `msi`.`msi_clients` where id = \"$client_id\";");
		var_dump($result);

		if ( $result['status'] === true ) {
			print "<h4>Client successfully deleted.</h4>";
		} else {
			print "<b>Something went wrong.....</b>";
		}

		// $result = sql::sql_run("INSERT INTO `msi`.`msi_clients-backup`SELECT * FROM `msi`.`msi_clients` where id = \"$client_id\";");
		// if(!isset($result)){
		// 	$result = sql::sql_run("delete FROM `msi`.`msi_clients` where id = \"$client_id\";");
		// } else {
		// 	print "<b>Something went wrong.....</b><br>I tried to run $sql";
		// }

	}
	
} 

?>
