<?php
class incident {
  var $open_date;
  var $close_date;
  var $uniq_num;
  var $description;
  var $client_id;
  var $techie;

  function incident($techie, $client_id, $description, $open_date, $close_date) {
    // insert into incident table
    $return = sql_run("insert into msi_incident (techie, client_id, description, open, close)
   			values (\"$techie\", \"$client_id\", \"$description\", \"$open_date\", \"$close_date\")");
  }
	

  function reopen($uniq_num){
    // remove close date
    $return = sql_run("update msi_incident set close = \"0000-00-00 00:00:00\" where incident_id = $uniq_num");
  }

  function close($uniq_num){
    $now = date("Y-m-d H:i:s");
    $return = sql_run("update msi_incident set close = \"$now\" where incident_id = $uniq_num");
  }  

  function form($client_id){
    $sqltime=date("Y-m-d H:i:s");
    print "<form action=\"index.php?module=incident&action=new&client_id=$client_id\" method=\"POST\">
          <textarea name=\"incident\" rows=\"5\" cols=\"60\"></textarea><br>
	  <input type=\"hidden\" name=\"sqltime\" value=\"$sqltime\" />
          <input type=\"submit\" name=\"submit\" value=\"close\" />
          <input type=\"submit\" name=\"submit\" value=\"open\" />
        </form>";
 }

  function table($client_id){
     $results = sql_run("select techie, open, close, description, incident_id from msi_incident where client_id = \"$client_id\" order by close desc limit 10 ");
     print '<br>
      		<span class="head"> Last 10 Incidents </span>
            <table class="client">
              <tr class=head><td>Techie</td><td>Open</td><td>Close</td><td>Description</td><td>Action</td></tr>';
     foreach ($results as $result) {
	          if($result[2] == "0000-00-00 00:00:00"){ 
		    $result[2] = "still open";
		    $modify = "close";
		  } else { $modify = "reopen";}
                  print "<tr><td class=\"client\">$result[0]</td><td class=\"client\">$result[1]</td><td class=\"client\">$result[2]</td>
		  <td  class=\"client\">$result[3]</td>
		    <td class=\"client\">
		      <form action=\"index.php?module=incident&action=modify&id=$result[4]&client_id=$client_id\" method=\"POST\">
		       <input type=\"submit\" name=\"submit\" value=\"$modify\">
		      </form>
		    </td>
		  </tr>";
	        }
     print '</table>';
  }
  

}



?>
