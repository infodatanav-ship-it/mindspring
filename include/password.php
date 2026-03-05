<?php
// $Id: password.php.inc,v 1.2 2008/01/22 07:39:43 laurence Exp $

class password
{
	
/*	
	function mod_crypt_password ($password, $mode) 
	{
		$crypt_key = 'rdf{kk06EajR';
		$iv = mcrypt_create_iv (mcrypt_get_iv_size (MCRYPT_BLOWFISH, MCRYPT_MODE_ECB), MCRYPT_RAND);
		
		if ($mode == 'encrypt') {
			return base64_encode (mcrypt_ecb (MCRYPT_BLOWFISH, $crypt_key, $password, MCRYPT_ENCRYPT, $iv));
		} else if ($mode == 'decrypt') {
				return rtrim (mcrypt_ecb (MCRYPT_BLOWFISH, $crypt_key, base64_decode ($password), MCRYPT_DECRYPT, $iv));
			} else {
				return $password;
			}
	}
	*/
	
function mod_crypt_password ($password, $mode) 
	{
		$crypt_key = 'rdf{kk06EajR';
		//$iv = mcrypt_create_iv (mcrypt_get_iv_size (MCRYPT_BLOWFISH, MCRYPT_MODE_ECB), MCRYPT_RAND);
		//$iv = openssl_random_pseudo_bytes(strlen($crypt_key), $cstrong);
		
		
		//$simple_string = "Welcome to GeeksforGeeks\n"; 
  

  
      // Store the cipher method 
     $ciphering = "AES-128-CTR"; 
  
// Use OpenSSl Encryption method 
$iv_length = openssl_cipher_iv_length($ciphering); 
$options = 0; 
  
// Non-NULL Initialization Vector for encryption 
$encryption_iv = '1234567891011121'; 
  
// Store the encryption key 
$encryption_key = "rdf{kk06EajR"; 
  
// Use openssl_encrypt() function to encrypt the data 
$encryption = openssl_encrypt($simple_string, $ciphering, 
            $encryption_key, $options, $encryption_iv); 
		
		
	// Non-NULL Initialization Vector for decryption 
$decryption_iv = '1234567891011121'; 
  
// Store the decryption key 
$decryption_key = "rdf{kk06EajR";  
  
// Use openssl_decrypt() function to decrypt the data 
$decryption=openssl_decrypt ($encryption, $ciphering,  
$decryption_key, $options, $decryption_iv); 	
	
		
		if ($mode == 'encrypt') {
			//return base64_encode (mcrypt_ecb (MCRYPT_BLOWFISH, $crypt_key, $password, MCRYPT_ENCRYPT, $iv));
			//     $encrypted = openssl_encrypt($data, $method, $key, OPENSSL_RAW_DATA, $iv);
			
			return openssl_encrypt($password, $ciphering, $encryption_key, $options, $encryption_iv); 
		
			
		} else if ($mode == 'decrypt') {
				
				//return rtrim (mcrypt_ecb (MCRYPT_BLOWFISH, $crypt_key, base64_decode ($password), MCRYPT_DECRYPT, $iv));
				return openssl_decrypt($encryption, $ciphering, $decryption_key, $options, $decryption_iv); 
				
			} else {
				return $password;
			}
	}	
	
	
	
	
	
	
	function generate_pronouncable($length = 8) {
		# Initialize valid char lists
		$valid_consonant = 'bcdfghjkmnprstv';
		$valid_vowel = 'aeiouy';
		$valid_numbers = '0123456789';
		
		# Find the charset length
		$consonant_length = strlen($valid_consonant);
		$vowel_length = strlen($valid_vowel);
		$numbers_length = strlen($valid_numbers);
		
		# Initialize the password and loop till we have all
		$password = "";
		while(strlen($password) < $length) {
			# Pull out a random set of pronouncable chars
			if (mt_rand(0, 2) != 1) $password .= $valid_consonant[mt_rand(0, ($consonant_length-1))].$valid_vowel[mt_rand(0, ($vowel_length-1))].$valid_consonant[mt_rand(0, ($consonant_length-1))];
			else $password .= $valid_numbers[mt_rand(0, ($numbers_length-1))];
		}

		return substr($password, 0, $length);
	} 
	function search_form()
	{
        if(!acl::has_access($_SESSION['id'],"password","read")) return 1;

		session_unregister ('search_string');
		//header ('Location: '.$_SERVER['SCRIPT_NAME'].'?module=password&action=list');
		
		print password::commonheader().'
				<td class="pagemain">
				<tr>
				<table align="center" border="0" cellpadding="0" cellspacing="0">
				<td align="left">
				<p>
				You can enter regular expressions in the search field.<br>
				These expressions are NOT case sensitive.
				</p>
				<p>
				Examples:<br>
				^fudge <i>will select all fields starting with the string fudge</i>
				<br>
				fudge$ <i>will select all fields ending with the string fudge</i>
				<br>
				</p>
				</table>
				
				<form action="?module=password&action=search" method="POST" name="main_form">
				<input type="hidden" name="action" value="search">
				<table align="center" border="0" cellpadding="0" cellspacing="0">
				<tr>
				</tr>
				<tr>
				<td>
				<table border="0" cellpadding="0" cellspacing="0">
				<tr>
				<td class="form_label"></td>
				<td>
				<input name="search_string" type="text" value="">
				<i class="input_help"></i>
				</td>
				</tr>
				</table>
				<tr>
				<td class="form_submit_buttons">
				<input type="submit">
				</td>
				</tr>
				</table>
				</form>';
		
	}
	
	#used either for search or listall purposes
	#lists all when argument is empty:Tim
	function getlist($search_string = '')
	{
        if(!acl::has_access($_SESSION['id'],"password","read")) return 1;
		
		$rows;
		
		if($search_string == '') unset($search_string);
	
			if (isset ($search_string)) 
			{
			  $sql = str_replace ('--TEXT--', $search_string,
								  "SELECT * FROM msi_passwords
									WHERE ((client REGEXP '--TEXT--')
									   OR (server REGEXP '--TEXT--')
									   OR (os REGEXP '--TEXT--')
									   OR (login REGEXP '--TEXT--')
									   OR (password REGEXP '--TEXT--'))
									ORDER BY client, server, login");
			} else {
			  $sql = 'SELECT * FROM msi_passwords ORDER BY client, server, login';
			}

			$res = sql_run($sql);
			
		if(count($res)!= 0)  //to avoid some error ('foreach - fed wrong arg') which shouldn't be happening	
			foreach($res as $rec)
			{
			
				$actions = '';
					$actions = '<a href="?module=password&action=edit_form&id=<!--ID-->">Edit</a>';
					$actions .= '&nbsp;&nbsp;&nbsp;&nbsp;<a href="?module=password&action=delete" onclick="submit_form (\'delete\',\'<!--ID-->\')">Delete</a>';
				$cells  = sprintf ('<td class="client" nowrap="yes">%s</td>', $rec['client']);
				$cells .= sprintf ('<td class="client" nowrap="yes">%s</td>', $rec['server']);
				$cells .= sprintf ('<td class="client" nowrap="yes">%s</td>', $rec['os']);
				$cells .= sprintf ('<td class="client" nowrap="yes">%s</td>', $rec['login']);
				$cells .= sprintf ('<td class="client" nowrap="yes">%s</td>',password::mod_crypt_password ($rec['password'], 'decrypt'));
				$cells .= sprintf ('<td class="client" nowrap="yes" width="10%%">%s</td>', isset ($actions) ? $actions : '');

				$row = sprintf ('<trvalign="top">%s</tr>',$cells);
				$rows .= str_replace ('<!--ID-->',$rec['id'], $row);
			}

		return $rows;
		
	}
	
	#returns the top form (containing links) that all pages of this class has have in common
	function commonheader()
	{
		return '<form action="?module=password" method="post" name="main_form">
				<input name="id" type="hidden" value="">
				<input name="action" type="hidden" value="">
				
				
				<table align="center">
				<tr>
				<td class="mod-nav">
				<a href="?module=password&action=listall">List All</a> :: 
				<a href="?module=password&action=search_form">New Search</a>::
				<a href="?module=password&action=add_form">Add New Record</a>
				</td>
				</tr>
				</table>
				
				</form><br>';
	}
	
	function listall($search_string = '')
	{	
        if(!acl::has_access($_SESSION['id'],"password","read")) return 1;
		$printout = password::commonheader();
		$printout .= '
				<b>Search phrase: <!--search_phrase--></b>

				<table class="client">
				
				<tr class = "head">
				<td>CLIENT</td>
				<td>SERVER</td>
				<td>DESCRIPTION</td>
				<td>LOGIN</td>
				<td>PASSWORD</td>
				<td>ACTIONS</td>
				</tr>
			
				<!--getlist-->
			
                </table>
                
				<script type="text/javascript"><!--
				function submit_form (action, id) {
				if (action == \'delete\') {
				if (!confirm (\'You are about to delete this record!\'))
				return false;
				}
				document.main_form.action.value = action;
				document.main_form.id.value = id;
				document.main_form.submit ();
				}
				//--></script>';
		
		
		//getlist() returns a group of cells to insert in to the table	
		$printout = str_replace('<!--search_phrase-->',$search_string,$printout);	
		$printout =  str_replace('<!--getlist-->',password::getlist($search_string), $printout);
		print $printout;
		
	}
	function add_form()
	{
        if(!acl::has_access($_SESSION['id'],"password","add")) return 1;
		
		$printout = password::commonheader().'
				<form action="?module=password&action=add" method="post" name="main_form">
				<input name="id" type="hidden" value="">
				<input name="action" type="hidden" value="">
				</tr>
				
				<tr>
		<table align="center" border="0" cellpadding="0" cellspacing="0">
        <td class="pagemain">
          <form action="index.php" method="post" name="main_form">
            <input name="action" type="hidden" value="add">
            <table align="center" border="0" cellpadding="0" cellspacing="0">
              <tr>
                <td class="form_nav_buttons">
				<input type="button" onclick="location.replace (\'?module=password&action=list\'); return false;" value="<< Back">
                </td>
              </tr>

              <tr>
                <td>
                  <table border="0" cellpadding="0" cellspacing="0">
                    <tr>
                      <td class="form_label">Client Name:</td>
                      <td>
                        <input name="client" type="text" value="">
                        <i class="input_help"></i>

                      </td>
                    </tr>
                    <tr>
                      <td class="form_label">Server:</td>
                      <td>
                        <input name="server" type="text" value="">
                        <i class="input_help"></i>
                      </td>

                    </tr>
                    <tr>
                      <td class="form_label">Description:</td>
                      <td>
                        <input name="os" type="text" value="">
                        <i class="input_help"></i>
                      </td>
                    </tr>

                    <tr>
                      <td class="form_label">Login:</td>
                      <td>
                        <input name="login" type="text" value="">
                        <i class="input_help"></i>
                      </td>
                    </tr>
                    <tr>

                      <td class="form_label">Password:</td>
                      <td>
                        <input name="password" type="text" value="">
                        <i class="input_help"></i>
                      </td>
                    </tr>
                  </table>
                </td>

              </tr>
              <tr>
                <td class="form_submit_buttons">
                  <input type="submit" value="Add">
                </td>
              </tr>
            </table>
          </form>
          <script type="text/javascript"><!--
    document.main_form.client.focus ();
  //--></script>

</td>
      </tr>
				
				</tbody>
				</table>
				</td>
				</tr>
				
				</table>
				<script type="text/javascript"><!--
				function submit_form (action, id) {
				if (action == \'delete\') {
				if (!confirm (\'You are about to delete this record!\'))
				return false;
				}
				document.main_form.action.value = action;
				document.main_form.id.value = id;
				document.main_form.submit ();
				}
				//--></script>
				</td>
				</table>';
				
	print $printout;
	}
	
	function add($client,$server,$os,$login,$password)
	{
        if(!acl::has_access($_SESSION['id'],"password","add")) return 1;
		/* From original code
		
		if (!msi_acl_has_access ($_SESSION['msi_module_name'], 'ADD')) {
				header ('Location: '.$_SERVER['SCRIPT_NAME']."?module=password&module_name=$_SESSION[msi_module_name]&action=".$mod_conf['default_key']."&msg="
					.urlencode ("You are not authorized to add records to the &quot;$mod_conf[module_title]&quot; module."));
			exit;
		}*/
		
		$sql = sprintf ("INSERT INTO msi_passwords (client, server, os, login, password)
						VALUES ('%s', '%s', '%s', '%s', '%s')",
					$client,
					$server,
					$os,
					$login,
					password::mod_crypt_password ($password, 'encrypt'));
		
		$res = sql_run($sql);
		print "added $login for client $client<br>";
	}
	function edit_form($id)
	{
        if(!acl::has_access($_SESSION['id'],"password","edit")) return 1;
		/* From original code
		
		if (!msi_acl_has_access ($_SESSION['msi_module_name'], 'EDIT')) {
			header ('Location: '.$_SERVER['SCRIPT_NAME']."?module=password&module_name=$_SESSION[msi_module_name]&action=".$mod_conf['default_key']."&msg="
					.urlencode ("You are not authorized to modify records in the &quot;$mod_conf[module_title]&quot; module."));
			exit;
		}*/
		
		$printout = password::commonheader().'
				<td class="pagemain">
				<form action="?module=password&action=edit" method="post" name="main_form">
				<input name="id" type="hidden" value="<!--id-->">
				<input name="action" type="hidden" value="edit">
				</tr>
				
				<tr>
				<table align="center" border="0" cellpadding="0" cellspacing="0">
				<td class="pagemain">
	
				<table align="center" border="0" cellpadding="0" cellspacing="0">
				<tr>
				<td class="form_nav_buttons">
				<input type="button" onclick="location.replace (\'?module=password&action=list\'); return false;" value="<< Back">
				</td>
				</tr>
				
				<tr>
				<td>
				<table border="0" cellpadding="0" cellspacing="0">
				<tr>
				<td class="form_label">Client Name:</td>
				<td>
				<input name="client" type="text" value="<!--client-->">
				<i class="input_help"></i>
				
				</td>
				</tr>
				<tr>
				<td class="form_label">Server:</td>
				<td>
				<input name="server" type="text" value="<!--server-->">
				<i class="input_help"></i>
				</td>
				
				</tr>
				<tr>
				<td class="form_label">Description:</td>
				<td>
				<input name="os" type="text" value="<!--os-->">
				<i class="input_help"></i>
				</td>
				</tr>
				
				<tr>
				<td class="form_label">Login:</td>
				<td>
				<input name="login" type="text" value="<!--login-->">
				<i class="input_help"></i>
				</td>
				</tr>
				<tr>
				
				<td class="form_label">Password:</td>
				<td>
				<input name="password" type="text" value="<!--password-->">
				<i class="input_help"></i>
				</td>
				</tr>
				</table>
				</td>
				
				</tr>
				<tr>
				<td class="form_submit_buttons">
				<input type="submit" value="Save">
				</td>
				</tr>
				</table>
				</form>
				<script type="text/javascript"><!--
				document.main_form.client.focus ();
				//--></script>
				
				</td>
				</tr>
				
				</tbody>
				</table>
				</td>
				</tr>
				
				</table>
			
				</td>
				</table>';
		
				
		$res = sql_run("SELECT * FROM msi_passwords WHERE id = '$id'");
		
		//filing the form field from recorded values
		$printout = str_replace('<!--id-->',$res[0]['id'],$printout);
		$printout = str_replace('<!--client-->',$res[0]['client'],$printout);
		$printout = str_replace('<!--server-->',$res[0]['server'],$printout);
		$printout = str_replace('<!--os-->',$res[0]['os'],$printout);
		$printout = str_replace('<!--login-->',$res[0]['login'],$printout);
		$printout = str_replace('<!--password-->',password::mod_crypt_password ($res[0]['password'], 'decrypt'),$printout);
	
		print $printout;
				
	}
	
	function edit($id,$client,$server,$os,$login,$password)
	{
        if(!acl::has_access($_SESSION['id'],"password","edit")) return 1;

		
		/* From original code
		
		if (!msi_acl_has_access ($_SESSION['msi_module_name'], 'EDIT')) {
				header ('Location: '.$_SERVER['SCRIPT_NAME']."?module=password&module_name=$_SESSION[msi_module_name]&action=".$mod_conf['default_key']."&msg="
					.urlencode ("You are not authorized to modify records in the &quot;$mod_conf[module_title]&quot; module."));
			exit;
		}
		*/
		
		$sql = sprintf ("UPDATE msi_passwords set client = '%s',
						server = '%s',
						os = '%s',
						login = '%s',
						password = '%s'
						WHERE id = ".$id,
					$client,
					$server,
					$os,
					$login,
					password::mod_crypt_password ($password, 'encrypt'));
					
		$res = sql_run($sql);
		return;
	}
	
	function search($search_string)
	{
		//no need to implement, listall searches by string when given a set argument
		password::listall($search_string);
	}
	function delete($id)
	{
        if(!acl::has_access($_SESSION['id'],"password","add")) return 1;
		/*
		if (!msi_acl_has_access ($_SESSION['msi_module_name'], 'DELETE')) {
			header ('Location: '.$_SERVER['SCRIPT_NAME']."?module=password&module_name=$_SESSION[msi_module_name]&action=".$mod_conf['default_key']."&msg="
					.urlencode ("You are not authorized to delete records from the &quot;$mod_conf[module_title]&quot; module."));
			exit;
		}*/
		
		$res = sql_run ('DELETE FROM msi_passwords WHERE id = '.$id,'n');
		return;
	}
}

?>
