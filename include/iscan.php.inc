<?php
class iscan {
  var $id;
  var $server_id;
  var $company;
  var $server_name;
  var $version;


  function iscan($server_id, $version){
     $return = sql_run("insert into msi_isvw (server_id, version)
   			values (\"$server_id\", \"$version\")");
  }

  function new_form(){
    $results = sql_run("select id, name from msi_server ");
    print "<b>New InterScan Virus Wall</b><br><form action=\"index.php?module=iscan&action=new\" method=\"post\">
	    Server:
	    <select name=\"server\">
	      <option> </option>";
	   
    foreach ($results as $result) {
                  print "<option value=\" $result[0]\">$result[1]</option>";
	        }
	    
    print "</select>	
	    Version :
	    <select name=\"version\">
	      <option></option>
	      <option>3.5</option>
	      <option>3.6</option>
	      <option>3.8</option>
	      <option>3.81</option>
	    </select>
	    <input type=\"submit\" value=\"submit\">
           </form>";
	  }

  function table(){
	       $results = sql_run("SELECT msi_clients.company , msi_isvw.isvw_id , msi_server.name, msi_isvw.version
                           FROM msi_clients, msi_server, msi_isvw 
                          WHERE msi_clients.id = msi_server.client_id AND 
                                msi_server.id = msi_isvw.server_id");
     print '<table class="client">
              <tr class=head><td>Company</td><td>Server</td><td>Version</td><td></td></tr>';
     foreach ($results as $result) {
                  print '<tr><td class="client">'.$result[0].'</td><td class="client"><a href="http://'.$result[2].':1812/interscan/">'.$result[2].'</a></td><td  class="client">'.$result[3].'</td>
                  		<td><form action="index.php?module=iscan&action=edit&isid='.$result[0].'"><input type="submit" value="edit"></form></td></tr>';
	        }
     print '</table>';
     print "<hr>";
     iscan::new_form();
     	  }
	  
  function edit_form($isw_id){
	   $result = sql_run("SELECT msi_clients.company , msi_isvw.isvw_id , msi_server.name, msi_isvw.version
                           FROM msi_clients, msi_server, msi_isvw 
                          WHERE msi_clients.id = msi_server.client_id AND 
                                msi_server.id = msi_isvw.server_id and msi_isvw.isvw_id = \"$isw_id\" limit 1");
	  print "
          
	  ";
	  }
  function edit_submit(){
	  }
} 

?>
