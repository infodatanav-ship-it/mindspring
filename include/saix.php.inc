<?php
// $Id: saix.php.inc,v 1.2 2008/01/22 07:39:43 laurence Exp $

class saix
{
	function commonheader()
	{
		return '<table align="center">
				<tr>
				<td class="mod-nav">
				<a href="?module=dialup&action=listall">Local Dialups</a> :: 
				<a href="?module=saix&amp;saix_type=analog">SAIX Analog Dialups</a> :: 
				<a href="?module=saix&amp;saix_type=isdn">SAIX ISDN Dialups</a> :: 
				<a href="?module=adsl">ADSL Accounts</a>::
				<a href="?module=saix&action=add_form">Add Dialup</a>
				</td>
				</table>
				<br>';
	}
	function listall($type = 'analog')
	{
		/*	c
		   if (isset ($_REQUEST['saix_type'])) {
		     $_SESSION['msi_dialups_saix_type'] = $_REQUEST['saix_type'];
		   } else {
		     if (!isset ($_SESSION['msi_dialups_saix_type']))
		       $_SESSION['msi_dialups_saix_type'] = 'analog';
		   }*/
		
		
		$printout = saix::commonheader();
		$table = '
				<table class="client">
				<tr class="head">
				<td>CLIENT</td>
				<td>USER_ID</td>
				<td>PASSWORD</td>
				<td>NOTES</td>
				<td>ACTIONS</td>
				</tr>
				
				<!--ROWS-->
				
				</table>';
				
		$res = sql_run (sprintf ("SELECT s.id AS id,
						c.company AS company,
						s.userid AS userid,
						s.password AS password,
						s.notes AS notes
						FROM msi_saix s
						LEFT JOIN msi_clients c
						ON s.client = c.id
						WHERE s.type = '%s'
						ORDER BY c.company",$type));
		# changed by laurence so sorted by company
		#ORDER BY s.userid", $_SESSION['msi_dialups_saix_type']), $msi_conn);	
		
		
		#------> Buiding rows of the list					
		$rows;

		if(count($res) > 0)  //to avoid some error ('foreach - fed wrong arg') which shouldn't be happening	
		foreach ($res as $rec) {
			if (isset ($actions)) unset ($actions);
			
				$actions = '';
				if(login::can_edit('saix')){
					$actions = '<a href="/mindspring/intranet/beta/index.php?module=saix&action=edit_form&id=<!--ID-->">Edit</a>';
					$actions .= '&nbsp;&nbsp;&nbsp;&nbsp;<a href="/mindspring/intranet/beta/index.php?module=saix&action=delete&id=<!--ID-->" onclick="submit_form (\'delete\',\'<!--ID-->\')">Delete</a>';
				}
			
			$cells  = sprintf ('<td class="client" nowrap="yes">%s</td>', htmlspecialchars ($rec['company']));
			$cells .= sprintf ('<td class="client" nowrap="yes">%s</td>', htmlspecialchars ($rec['userid']));
			$cells .= sprintf ('<td class="client" nowrap="yes">%s</td>', htmlspecialchars ($rec['password']));
			$cells .= sprintf ('<td class="client" nowrap="yes">%s</td>', $rec['notes']);
			$cells .= sprintf ('<td class="client" nowrap="yes" width="10%%">%s</td>', isset ($actions) ? $actions : '');
			
			$row = sprintf ('<tr><cells>%s</cells></tr>', $cells);
			$rows .= str_replace ('<!--ID-->', $rec['id'], $row);
		}
		
		$table = str_replace('<!--ROWS-->',$rows,$table);
		
		$printout .= $table;
		
		#-->inserting javascript
		$printout .= '<script type="text/javascript"><!--
				function submit_form (action, id) {
				if (action == \'delete\') {
				if (!confirm (\'You are about to delete this record!\'))
				return false;
				}
				
				}
				//--></script>';
		print $printout;
		
		return;
	}
	
	function add_form()
	{
		
		$printout = '<form action="?module=saix" method="post" name="main_form">
				<input name="action" type="hidden" value="add">
				<input name="module" type="hidden" value="saix">
				
				<table align="center" border="0" cellpadding="0" cellspacing="0">
				<tr>
				<td class="form_nav_buttons">
				
				<input class="button" type="button" onclick="location.replace (\'?module=saix&action=listall\'); return false;" value="<< Back">
				</td>
				</tr>
				<tr>
				<td>
				<table border="0" cellpadding="0" cellspacing="0">
				<tr>
				<td class="form_label">Client:</td>
				
				<td>
				<select name="client">
				<option value="0">--unknown--</option>
				
				
				<!--OPTIONS-->
				
				
				</select>
				<i class="input_help"></i>
				</td>
				<tr>
				<td class="form_label">Dialup Type:</td>
				<td>
				<select name="saix_type">
				<option value="isdn">ISDN</option>
				<option value="analog">ANALOG</option>
				</td>
				</tr>
				</tr>
				<tr>
				<td class="form_label">User ID:</td>
				
				<td>
				<input name="userid" type="text" value="">
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
				<tr>
				<td class="form_label">Notes:</td>
				<td>
				<input class="long_input" name="notes" type="text" value="">
				
				<i class="input_help"></i>
				</td>
				</tr>
				</table>
				</td>
				</tr>
				<tr>
				<td class="form_submit_buttons">
				<input class="button" type="submit" value="Add">
				
				</td>
				</tr>
				</table>
				</form>';
		
		
		#------> Adding OPTIONS to SELECT list
		$res = sql_run("SELECT DISTINCT id, company  FROM msi.msi_clients");
		$options = '';
		
		if(count($res) != 0)
			foreach($res as $rec)
				$options .= sprintf('<option value="%s">%s</option>',$rec['id'],$rec['company']);;
		
		$printout = str_replace('<!--OPTIONS-->',$options,$printout);
		
		print $printout;
		return;
	}
	
	function add($type,$client,$userid,$password,$notes,$created,$userid)
	{
		/*
		if (!msi_acl_has_access ($_SESSION['msi_module_name'], 'ADD')) {
		    header ('Location: '.$_SERVER['SCRIPT_NAME']."?module_name=$_SESSION[msi_module_name]&pk=".$mod_conf['default_key']."&msg="
		      .urlencode ("You are not authorized to add records to the &quot;$mod_conf[module_title]&quot; module."));
		    exit;
		  }*/
		
		$res = sql_run(sprintf ("INSERT INTO msi_saix (type, client, userid, password, notes, created, created_by)
						VALUES ('%s', %d, '%s', '%s', '%s', '%s', '%s')",
					$type,					 // $_SESSION['msi_dialups_saix_type'],
					$client,
					$userid,
					$password,
					$notes,
					date ('Y-m-d H:i:s'),
					$userid));//$_SESSION['msi_userid']));
		
		return;
	}
	function edit_form($id)
	{
		
		$printout = saix::commonheader().'<form action="?module=saix" method="post" name="main_form">
				<input name="action" type="hidden" value="edit">
				<input name="module" type="hidden" value="saix">
				<input name="id" type="hidden" value="<!--ID-->">
				
				<table align="center" border="0" cellpadding="0" cellspacing="0">
				<tr>
				<td class="form_nav_buttons">
				
				<input class="button" type="button" onclick="location.replace (\'?module=saix&action=listall\'); return false;" value="<< Back">
				</td>
				</tr>
				<tr>
				<td>
				<table border="0" cellpadding="0" cellspacing="0">
				<tr>
				<td class="form_label">Client:</td>
				
				<td>
				<select name="client">
				<option value="0">--unknown--</option>
				<!--DEFAULTOPTION-->
				
				<!--OPTIONS-->
				
				
				</select>
				<i class="input_help"></i>
				</td>
				<tr>
				<td class="form_label">Dialup Type:</td>
				<td>
				<select name="saix_type">
				
				<option value="analog">ANALOG</option>
				<option value="isdn">ISDN</option>
				
				</select>
				</td>
				</tr>
				</tr>
				<tr>
				<td class="form_label">User ID:</td>
				
				<td>
				<input name="userid" type="text" value="<!--USER_ID-->">
				<i class="input_help"></i>
				</td>
				</tr>
				<tr>
				<td class="form_label">Password:</td>
				<td>
				
				<input name="password" type="text" value="<!--PASSWORD-->">
				<i class="input_help"></i>
				</td>
				</tr>
				<tr>
				<td class="form_label">Notes:</td>
				<td>
				<input class="long_input" name="notes" type="text" value="<!--NOTES-->">
				
				<i class="input_help"></i>
				</td>
				</tr>
				</table>
				</td>
				</tr>
				<tr>
				<td class="form_submit_buttons">
				<input class="button" type="submit" value="Edit">
				
				</td>
				</tr>
				</table>
				</form>';
		
		
		$res_dialup = sql_run("SELECT * FROM msi.msi_saix WHERE id=".$id);
		$res_clients = sql_run("SELECT DISTINCT id, company  FROM msi.msi_clients");
		
		# Adding OPTIONS to Client list*/
		$options = '';
		
		if(count($res_clients) != 0){
			foreach($res_clients as $rec){
				if($rec['id'] == $id){
					$printout = str_replace('<!--DEFAULTCLIENT-->',sprintf('<option value="%s">%s</option>',$rec['id'],$rec['company']),$printout);
					}
				else{
					$options .= sprintf('<option value="%s">%s</option>',$rec['id'],$rec['company']);
					}
			}
		}
		$printout = str_replace('<!--OPTIONS-->',$options,$printout);
		
		#Setting original dialup type as default
		if($res_dialup[0]['type'] == 'analog'){
			$printout = str_replace('<!--ANALOG_DEFAULT_OPTION-->','selected',$printout);
		}
		else { $printout = str_replace('<!--ISDN_DEFAULT_OPTION-->','selected',$printout);
		}
		
		#Setting default values
		$printout = str_replace('<!--USER_ID-->',$res_dialup[0]['userid'],$printout);
		$printout = str_replace('<!--PASSWORD-->',$res_dialup[0]['password'],$printout);
		$printout = str_replace('<!--NOTES-->',$res_dialup[0]['notes'],$printout);
		$printout = str_replace('<!--ID-->',$res_dialup[0]['id'],$printout);
		
		
		print $printout;
	
	}
	function edit($id,$type,$client,$userid,$password,$notes,$created,$userid)
	{
		/*
		 if (!msi_acl_has_access ($_SESSION['msi_module_name'], 'EDIT')) {
		    header ('Location: '.$_SERVER['SCRIPT_NAME']."?module_name=$_SESSION[msi_module_name]&pk=".$mod_conf['default_key']."&msg="
		      .urlencode ("You are not authorized to edit records in the &quot;$mod_conf[module_title]&quot; module."));
		    exit;
		  }*/
				
		  $sql = sprintf ("UPDATE msi_saix
									SET type = '%s',
									client = %d,
									userid = '%s',
									password = '%s',
									notes = '%s',
									modified = '%s',
									modified_by = '%s'
		                    WHERE id = %d",
				$type,
				$client,
				$userid,
				$password,
				$notes,
				date ('Y-m-d H:i:s'),
				$_SESSION['msi_userid'],
				$id);
				
		  $res = sql_run($sql);
		return;
		
	}
	function delete($id)
	{
		/*
		if (!msi_acl_has_access ($_SESSION['msi_module_name'], 'DELETE')) {
		    header ('Location: '.$_SERVER['SCRIPT_NAME']."?module_name=$_SESSION[msi_module_name]&pk=".$mod_conf['default_key']."&msg="
		      .urlencode ("You are not authorized to delete records from the &quot;$mod_conf[module_title]&quot; module."));
		    exit;
		  }*/
				
		  $res = sql_run(sprintf ("DELETE FROM msi_saix
		                                  WHERE id = %d",
		                                  $id));
	}
	/*
	function apply()
	{
		
		if (!msi_acl_has_access ($_SESSION['msi_module_name'], 'EDIT')) {
		    header ('Location: '.$_SERVER['SCRIPT_NAME']."?module_name=$_SESSION[msi_module_name]&pk=".$mod_conf['default_key']."&msg="
		      .urlencode ("You are not authorized to apply changes in the &quot;$mod_conf[module_title]&quot; module."));
		    exit;
		  }
				
		  $res = mysql_query ('SELECT * FROM msi_radius WHERE active = 1 ORDER BY userid', $msi_conn);
				
		  $fp = fopen ('/tmp/radius-users', 'w');
				
		  //while ($rec = mysql_fetch_object ($res)) {
		foreach($res as $rec)
		    if($rec['adsl']){
		      if($rec['capped']){
		        $account = "$rec->userid	Auth-Type = Local, Password = \"$rec->password\"
		                  Cisco-AVPair = \"ip:ip-unnumbered=Loopback51\",
		           Cisco-AVPair = \"ip:addr-pool=ipnetpool2\",
		           Service-Type = Framed-User,
		           Framed-Protocol = PPP\n\n";
		} else {
		        $account = "$rec->userid	Auth-Type = Local, Password = \"$rec->password\"
		                  Cisco-AVPair = \"ip:ip-unnumbered=Loopback50\",
		           Cisco-AVPair = \"ip:addr-pool=ipnetpool1\",
		           Service-Type = Framed-User,
		           Framed-Protocol = PPP\n\n";
		      }
		    } else {
		    $account = sprintf ("%s\tAuth-Type = Local, Password = \"%s\"\n"
		                        ."\tService-Type = %s,\n"
		                        ."\tFramed-Protocol = %s,\n"
		                        ."\tFramed-IP-Address = %s,\n"
		                        ."\tFramed-IP-Netmask = %s,\n"
		                        ."\tFramed-Routing = %s,\n"
		                        ."\tFramed-Compression = %s,\n"
		                        ."\tFramed-MTU = %s\n\n",
		                        $rec['userid'],
		                        $rec['password'],
		                        $rec['service_type'],
		                        $rec['framed_protocol'],
		                        $rec['framed_ip_address'],
		                        $rec['framed_ip_netmask'],
		                        $rec['framed_routing'],
		                        $rec['framed_compression'],
		                        $rec['framed_mtu']);
		    }
				
		    fwrite ($fp, $account);
		  }
				
		  fclose ($fp);
				
		  $fp = popen ('sudo /usr/local/sbin/radconf-apply 2>&1', 'r');
		  $msg = fread ($fp, 128);
		  fclose ($fp);
				
		  mysql_close ($msi_conn);
				
		  header ('Location: '.$_SERVER['SCRIPT_NAME'].'?pk=apply-confirm&msg='.$msg);
				
		  exit;
		
	}
	function apply_saix()
	{
		
	}
	function apply_confirm()
	{
		/*
		if (!msi_acl_has_access ($_SESSION['msi_module_name'], 'EDIT')) {
		    header ('Location: '.$_SERVER['SCRIPT_NAME']."?module_name=$_SESSION[msi_module_name]&pk=".$mod_conf['default_key']."&msg="
		      .urlencode ("You are not authorized to apply changes in the &quot;$mod_conf[module_title]&quot; module."));
		    exit;
		  }
				
		  mysql_close ($msi_conn);
				
		  $page = str_replace ('<!--TITLE-->', $mod_conf['module_title'].' - '.$mod_conf['module_title'].' - Confirm Changes', $msi_templates['PAGE']);
		  $page = str_replace ('<!--CONTENT-->', $mod_templates['APPLY-CONFIRM'], $page);
				
		  msi_output_xml_page ($page);
				
		  exit;
		
	}
	function mod_build_client_list ($default) {
		global $msi_conn;
		
		$res = mysql_query ('SELECT id, custnum, company FROM msi_clients ORDER BY company', $msi_conn);
		
		$items = '<input-item label="--unknown--" value="0"/>';
		
		while ($rec = mysql_fetch_object ($res)) {
			$items .= sprintf ('<input-item label="%s: %s" value="%s" selected="%s"/>',
					htmlspecialchars ($rec->custnum),
					htmlspecialchars ($rec->company),
					$rec->id,
					$default == 0 ? 'no' : ($default == $rec->id ? 'yes' : 'no'));
		}
		
		return $items;
	}
	function mod_send_notification ($key, $userid) {
		switch ($key) {
			case 'add':
				$subject = 'MSI-RadConf: Dial-up account added - '.$userid.' - by '.$_SESSION['msi_userid']; break;
			case 'save':
				$subject = 'MSI-RadConf: Dial-up account modified - '.$userid.' - by '.$_SESSION['msi_userid']; break;
			case 'delete':
				$subject = 'MSI-RadConf: Dial-up account deleted - '.$userid.' - by '.$_SESSION['msi_userid']; break;
		}
		//mail('nchseb002@mail.uct.ac.za','Mindspring mail','testing','FROM: ntimotys@yahoo.com');
		
		
		/***** original code:Tim**************
		 mail ('inetstatus@mindspring.co.za',
		       $subject, '',
		       "From: noreply@$SERVER_NAME\r\n"
		      ."X-Mailer: PHP/".phpversion ());
			
		/*********** End of original code ********
	}
	
	*/
	
}

?>
