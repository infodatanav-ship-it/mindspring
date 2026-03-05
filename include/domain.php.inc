<?php

class domain {

  function view(){
  $results = sql_run("select domain from msi_domain  order by domain");
  $result_count = count($results);
		print "<span class=\"head\">Domain Names Hosted</span>
	  				<table class=\"client\">
	  				<tr class=\"head\"><td>Domain</td></td><td>Client</td></td><td></td></td></tr>";
	   	foreach ($results as $result) {
                    print "<tr>
                             <td class=\"client\">$result[0]</td>
                             <td class=\"client\">$result[0]</td>
                             <td class=\"client\">$result[0]</td>
                           </tr>";
	          }
		  print "</table><hr>";
  
  }
}

  


?>