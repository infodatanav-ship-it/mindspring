<?php

class website {

	function view(){
		$results = sql_run("select website from msi_website  order by website");
		$result_count = count($results);

		print "<span class=\"head\">Websites Names Hosted</span>
		<table class=\"client\">
		<tr class=\"head\"><td>Website</td></td><td>Client</td></td><td></td></td></tr>";

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