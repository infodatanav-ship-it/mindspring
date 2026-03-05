<?php

function helpdesk_welcome(){
	print 'Welcome to the Mindspring Helpdesk.<br>
	       Please type in the clients name below and click search.<br>
	       If only one customer is found you will be redirected to there client page, else you will be given a list of possible clients<br>';
	helpdesk_psearch();
       }

function helpdesk_psearch(){
	print '<form action="index.php?module=helpdesk&action=search" method="POST">
		 <input type="text" name="search_string" size="10" />
		 <input type="submit" value="Search" />
	       </form>';
}

function helpdesk_dsearch($search_string){
	$results = sql_run("select id, company from msi_clients where company rlike \"$search_string\"");
	$result_count = count($results);
	
	if($result_count == 1) {
		helpdesk_client($results[0][0]);
	} else if($result_count == 0){
		print "No results";
		helpdesk_psearch();
	} else {
	 	foreach ($results as $result) {
                  print "<a href=index.php?module=helpdesk&client_id=$result[0]>$result[1]</a><br>";
	        }
		print "<hr>";
		helpdesk_psearch();
	}
}

function helpdesk_client($client_id){
	print "<table width=\"100%\">
	        <tr>
		  <td>";
	            client::info($client_id);
	print	 "</td>
		  <td>";
		    client::accounts($client_id);
	print	 "</td>
		 </tr>
		</table>";

	incident::form($client_id); 	
	incident::table($client_id);

	print "<hr>";
	helpdesk_psearch();
}

function helpdesk_summary(){
  $results = sql_run("select msi_incident.incident_id, msi_incident.techie, msi_incident.open, msi_clients.company, msi_incident.description, msi_clients.id
  								from msi_incident, msi_clients where msi_incident.client_id = msi_clients.id and !close");
	$result_count = count($results);
	
	print "<span class=\"head\"> Open Incidents </span>
	       <table class=\"client\">
	       <tr class=\"head\"><td>Company</td><td>Techie</td><td>open</td><td>Description</td><td>Action</td></tr>";
   
   if($result_count == 0){
		print "<tr><td class=\"client\" colspan=\"5\">No open incidents</td></td>";
	} else {
	 	foreach ($results as $result) {
                  print "<tr><td class=\"client\"><a href=\"index.php?module=helpdesk&client_id=$result[5]\">$result[3]</a></td><td class=\"client\">$result[1]</td>
                           <td class=\"client\">$result[2]</td><td class=\"client\">$result[4]</td>
                           <td class=\"client\">
                             <form action=\"index.php?module=incident&action=modify&id=$result[0]\" method=\"POST\">
		                         <input type=\"submit\" name=\"submit\" value=\"close\">
		                       </form>
		                     </td>
		                   </tr>";
	        }
	}
	
	print "</table> 
	       <br>
	       <span class=\"head\"> Top Techie </span>
	       <table class=\"client\">
	       <tr class=\"head\"><td>Incidents</td><td>Techie</td></tr>";
	
	// top techie!
	print '';
	$results = sql_run("select count(*) As  repetitions, techie from msi_incident group by techie having repetitions >1 order by repetitions desc");
	$result_count = count($results);	
	foreach ($results as $result) {
                  print "<tr>
                           <td class=\"client\">$result[0]</td>
                           <td class=\"client\">$result[1]</td>
		                   </tr>";
	        }
	print "</table>";
	
	print "<br>
	       <span class=\"head\"> Top Customer </span>
	       <table class=\"client\">
	       <tr class=\"head\"><td>Incidents</td><td>Company</td></tr>";
	
	// top techie!
	print '';
	$results = sql_run("select count(*) As  rep, msi_clients.company, msi_clients.id from msi_incident, msi_clients where msi_clients.id = msi_incident.client_id  group by msi_clients.company having rep >1 order by rep desc  limit 20");
	$result_count = count($results);	
	foreach ($results as $result) {
                  print "<tr>
                           <td class=\"client\">$result[0]</td>
                           <td class=\"client\"><a href=\"index.php?module=helpdesk&client_id=$result[2]\">$result[1]</a></td>
                           
		                   </tr>";
	        }
	print "</table>";
}

?>