<?php 
if(!isset($_SESSION)){
	session_start();
}

class billing_reports {

	function navigation(){
		print '
		<div class="sec-links"><ul>
			<li><a href="?module=billing_reports&action=breakdown">Monthly Report</a></li>
			<li><a href="?module=billing_reports&action=weekly">Weekly Report </a></li>
			<li><a href="?module=billing_reports&action=remote_onsite_report">Onsite/Remote Report </a></li>
			<li><a href="?module=billing_reports&action=customer_summary">Client Report</a></li></ul>
			<li><a href="?module=billing_reports&action=maintenance_contract_report" >Maintenance Report</a></li>
			<li><a href="?module=billing_reports&action=bill_nonbill" >Billable/Non-Billable </a></li>
		  </div>';
		  
	}

	function main_report($year,$month){
 
		if(!acl::has_access($_SESSION['id'],"billing admin","read")) return 1;
		
		$next_month = $month +1;
 	
		if ($next_month > 12) {
			$next_month = 1;
			$next_year = $year +1;
		} else {
			$next_year = $year;
		}
		
		$prev_month = $month - 1;
		
		if ($prev_month == 0){
			$prev_month = 12;
			$prev_year= $year -1;
		} else {
			$prev_year= $year;
		}
		
		print '<table><tr><td colspan="3" align="center">';
		print "<a href=?module=billing_reports&action=breakdown&year=$prev_year&month=$prev_month style=\"color: #1A53FF\">". date(F, mktime(0, 0, 0, $prev_month, 1, $prev_year)) ." $prev_year << Previous</a> ";
		print "&nbsp&nbsp";
		print " <b><a href=?module=billing_reports&action=breakdown&year=$year&month=$month>". date(F, mktime(0, 0, 0, $month, 1, $year)) ." $year</a></b>";
		print "&nbsp&nbsp";
		print " <a href=?module=billing_reports&action=breakdown&year=$next_year&month=$next_month style=\"color: #1A53FF\">Next >> ". date(F, mktime(0, 0, 0, $next_month, 1, $next_year)) ." $next_year</a><p>";
		print '</td></tr><tr><td valign=top>';

		billing::report_client_summary($year,$month);
		print "</td><td valign=top>";
		billing::report_month_summary($year,$month);
		print '</td><td valign=top>';
		billing::report_techie_summary($year,$month);
		print '<p>';
		billing::report_six_month_summary($year,$month);
		print '</td></tr></table>';
	}

	function weekly_report($year,$week){
		if (!acl::has_access($_SESSION['id'],"billing admin","read")) return 1;

		$next_week = $week +1;

		if ($next_week > 52) {
			$next_week = 1;
			$next_year = $year +1;
		} else {
			$next_year = $year;
		}

		$prev_week = $week - 1;

		if ($prev_week == 0){
			$prev_week = 52;
			$prev_year= $year -1;
		} else {
			$prev_year= $year;
		}

		print '<table><tr><td colspan="3" align="center">';
		print "<a href=?module=billing_reports&action=weekly&week=$prev_week&year=$prev_year style=\"color: #1A53FF\"> Week $prev_week  $prev_year << Previous</a> ";
		print "&nbsp&nbsp";
		print " <b><a href=?module=billing_reports&action=weekly&year=$year&week=$week> Week  $week $year</a></b>";
		print "&nbsp&nbsp";
		print " <a href=?module=billing_reports&action=weekly&year=$next_year&week=$next_week style=\"color: #1A53FF\">Next >> Week $next_week $next_year</a><p>";
		print '</td></tr></table>';
		//print '<table><tr><td>'.$week.' => '.$year.'</td></tr></table>';

		// create a button to send the weekly report that will bring up a modal with a form 
		// where the user can enter a list of email addresses separated by commas

		print "<div class='btn-container'>";
		print "<button class='btn btn-secondary send-report'>Send Report</button>";
		print "</div>";


		print "<table><tr>";
		print "<td valign=\"top\">";
		$month = date('n', strtotime($year . 'W' . str_pad($week, 2, '0', STR_PAD_LEFT)));

		print self::ClientSummaryTable($week, $year, $month);
		print "</td>";
		print "<td valign=\"top\">";

		$sql="SELECT msi_users.id, msi_users.username, sum( msi_billing.bill_hrs ) hours
		FROM msi_billing , msi_users
		WHERE msi_users.id = msi_billing.techie AND
		WEEK ( msi_billing.date ) = $week - 1 AND year( msi_billing.date ) = $year
		GROUP BY msi_billing.techie
		ORDER BY hours DESC";

		$results = sql::sql_run($sql);

		$totalCHA = 0;
		$totalCB = 0;
		$totalAHB = 0;
		$totalNBH = 0;
		$totalHB = 0;

		print "<span class=\"head\">Techies</span>
		<table class=\"client\">
		<tr class=\"head\">
		<td style=\"font-size: 13px;\">Name</td>
		<td style=\"font-size: 13px;\">Total Hours Billed</td>
		<td style=\"font-size: 13px;\">Contract Hours Assigned</td>
		<td style=\"font-size: 13px;\">Contract Hours Billed</td>
		<td style=\"font-size: 13px;\">Ad-hoc Hours</td>
		<td style=\"font-size: 13px;\">Maintenance Hours</td>
		<td style=\"font-size: 13px;\">Proposal Hours</td>
		<td style=\"font-size: 13px;\">Non-billable Hours</td>
		</tr>";
		
		if(is_array($results)){

			foreach ($results as $result) {

				$techid = $result[0];
				$username = $result[1];

				// $sqlTotalContractHoursPerMonth = "SELECT sum(hours) as billedhours FROM msi_contract WHERE terminating = 'no' AND primarytech = '" . $result[1] . "';";

				$sqlTotalContractHoursPerMonth = "SELECT sum(hours) as billedhours FROM msi_contract WHERE terminating = 'no' AND primarytech = '".$username."';";
				$resultTotalContractHours = sql::sql_run( $sqlTotalContractHoursPerMonth );

				if ( $resultTotalContractHours[0]['billedhours'] ) {
					$totalCHA = $totalCHA + floatval($resultTotalContractHours[0]['billedhours']);
					$totalCHAHTML = "<td class=\"client\">" . floatval($resultTotalContractHours[0]['billedhours']) . "</td>";
				} else {
					$totalCHA = $totalCHA + 0;
					$totalCHAHTML = "<td class=\"client\">0</td>";
				}

				/* ----------------------------------------------------------------------------------- */
				
				$sqlTotalContractBilled = "SELECT SUM(msi_billing.bill_hrs) as billedhours FROM msi_billing , msi_users WHERE msi_users.id = msi_billing.techie AND WEEK ( msi_billing.date ) = $week - 1 AND year( msi_billing.date ) = $year AND msi_billing.type = 'Contract' AND msi_billing.techie = ".$techid.";";
				
				$resultTotalContractBilled = sql::sql_run( $sqlTotalContractBilled );
				
				$lineTCB = 0;
				
				if ( $resultTotalContractBilled[0]['billedhours'] ) {
					
					$lineTCB = floatval($resultTotalContractBilled[0]['billedhours']);
					$totalCB = $totalCB + floatval($resultTotalContractBilled[0]['billedhours']);
					$totalCBHTML = "<td class=\"client\">" . floatval($resultTotalContractBilled[0]['billedhours']) . "</td>";
				} else {
					$totalCB = $totalCB + 0;
					$totalCBHTML = "<td class=\"client\">0</td>";
				}

				/* ----------------------------------------------------------------------------------- */
				
				$sqlTotalAdHocBilled = "SELECT SUM(msi_billing.bill_hrs) as billedhours FROM msi_billing , msi_users WHERE msi_users.id = msi_billing.techie AND WEEK ( msi_billing.date ) = $week - 1 AND year( msi_billing.date ) = $year AND msi_billing.type = 'Ad hoc' AND msi_billing.techie = '".$techid."';";
				$resultTotalAdHocBilled = sql::sql_run( $sqlTotalAdHocBilled );
				
				$lineTAHB = 0;
				
				if ( $resultTotalAdHocBilled[0]['billedhours'] ) {
					$lineTAHB = floatval($resultTotalAdHocBilled[0]['billedhours']);
					$totalAHB = $totalAHB + floatval($resultTotalAdHocBilled[0]['billedhours']);
					$totalAHBHTML = "<td class=\"client\">" . floatval($resultTotalAdHocBilled[0]['billedhours']) . "</td>";
				} else {
					$totalAHB = $totalAHB + 0;
					$totalAHBHTML = "<td class=\"client\">0</td>";
				}

				/* ----------------------------------------------------------------------------------- */
				// Total Maintenance Billed
			
				$sqlTotalMaintenanceBilled = "SELECT SUM(msi_mbilling.bill_hrs) as billedhours FROM msi_mbilling, msi_users WHERE msi_users.id = msi_mbilling.techie AND WEEK ( msi_mbilling.date ) = $week AND year( msi_mbilling.date ) = $year AND msi_mbilling.techie = '".$techid."' AND msi_mbilling.type = 'Maintenance' GROUP BY msi_mbilling.techie;";
				//var_dump($sqlTotalMaintenanceBilled);
				$resultTotalMaintenanceBilled = sql::sql_run( $sqlTotalMaintenanceBilled );
				
				if ( $resultTotalMaintenanceBilled[0]['billedhours'] ) {
					$lineTMB = floatval($resultTotalMaintenanceBilled[0]['billedhours']);
					// var_dump("maintenance: " . $lineTMB . "<br />");
					$totalTMB = $totalTMB + floatval($resultTotalMaintenanceBilled[0]['billedhours']);
					$totalTMBHTML = "<td class=\"client\">" . floatval($resultTotalMaintenanceBilled[0]['billedhours']) . "</td>";
				} else {
					$totalTMB = $totalTMB + 0;
					$totalTMBHTML = "<td class=\"client\">0</td>";
				}

				/* ----------------------------------------------------------------------------------- */
				// Total Proposal Billed
			
				$sqlTotalProposalBilled = "SELECT SUM(`msi_billing`.`bill_hrs`) as billedhours FROM `msi_billing`, `msi_users` WHERE `msi_users`.`id` = `msi_billing`.`techie` AND WEEK ( `msi_billing`.`date` ) = $week - 1 AND year( `msi_billing`.`date` ) = $year AND `msi_billing`.`techie` = '".$techid."' AND `msi_billing`.`type` LIKE '%Proposal%' GROUP BY `msi_billing`.`techie`;";
				// var_dump($sqlTotalProposalBilled);
				$resultTotalProposalBilled = sql::sql_run( $sqlTotalProposalBilled );


				if ( $resultTotalProposalBilled[0]['billedhours'] ) {
					$lineTPB = floatval($resultTotalProposalBilled[0]['billedhours']);
					// var_dump("proposal: " . $lineTPB . "<br />");
					$totalTPB = $totalTPB + floatval($resultTotalProposalBilled[0]['billedhours']);
					$totalTPBHTML = "<td class=\"client\">" . floatval($resultTotalProposalBilled[0]['billedhours']) . "</td>";
				} else {
					$totalTPB = $totalTPB + 0;
					$totalTPBHTML = "<td class=\"client\">0</td>";
				}

				/* ----------------------------------------------------------------------------------- */

				$sqlTotalNonBillableHours = "SELECT SUM(nonbill_hrs) as billedhours FROM msi_billing WHERE WEEK ( msi_billing.date ) = $week - 1 AND year( msi_billing.date ) = $year AND msi_billing.techie = '".$techid."' GROUP BY techie;";
				$resultTotalNonBillableHours = sql::sql_run( $sqlTotalNonBillableHours );

				if ( $resultTotalAdHocBilled[0]['billedhours'] ) {
					$totalNBH = $totalNBH + floatval($resultTotalNonBillableHours[0]['billedhours']);
					$totalNBHHTML = "<td class=\"client\">" . floatval($resultTotalNonBillableHours[0]['billedhours']) . "</td>";
				} else {
					$totalNBH = $totalNBH + 0;
					$totalNBHHTML = "<td class=\"client\">0</td>";
				}

				//var_dump($totalNBH);
				
				$currentTotalHoursBilled = $totalCB + $totalAHB + $lineTMB + $lineTPB;
				//$totalHB = $totalHB + ( $currentTotalHoursBilled );

				/* ----------------------------------------------------------------------------------- */
				
				$lineTot = $lineTAHB + $lineTCB + $lineTMB + $lineTPB;

				print "<tr>" .
				"<td class=\"client\">" . ucfirst($result[1]) . "</td>" . 
				"<td class=\"client\">$lineTot</td>" .
				$totalCHAHTML . 
				$totalCBHTML .
				$totalAHBHTML .
				$totalTMBHTML .
				$totalTPBHTML .
				$totalNBHHTML .
				"</tr>";
			}
		}
	
		$totalHB = $totalCB + $totalAHB;

		print "<tr>
			<td class=\"client\"><b>Total :</b></td>
			<td class=\"client\"><b>$totalHB</b></td>
			<td class=\"client\"><b>$totalCHA</b></td>
			<td class=\"client\"><b>$totalCB</b></td>
			<td class=\"client\"><b>$totalAHB</b></td>
			<td class=\"client\"><b>$totalTMB</b></td>
			<td class=\"client\"><b>$totalTPB</b></td>
			<td class=\"client\"><b>$totalNBH</b></td></tr>";

		print "</table>";
		print "</td>";

		// print "<td>";
		// print self::ClientSummaryTable($week, $year);
		// print "</td>";

		// print "</tr>";
		// print "</table>";
		print "</tr></table>";
	}

	function ClientSummaryTable($week, $year, $month){
		$output = "";
		$sql="SELECT msi_clients.company, sum( msi_billing.bill_hrs ) hours, msi_clients.id
		FROM `msi_clients` , msi_billing
		WHERE msi_billing.client = msi_clients.id AND
		week ( msi_billing.date ) = $week -1 AND year( msi_billing.date ) = $year
		GROUP BY msi_billing.client
		ORDER BY `hours` DESC";
		$results = sql::sql_run($sql);
		$output .= "<span class=\"head\">Client Summary</span>
			<table class=\"client\" id=\"data-table\">
			<tr class=\"head\"><td>Client</td><td>Hours</td></tr>";
 
		if(is_array($results)){
			foreach ($results as $result) {
				$output .= "<tr>
				<td class=\"client\"> <a href = \"?module=billing&action=contract_client&id=$result[2]&year=$year&month=$month\">$result[0]</td>
				<td class=\"client\">$result[1]</td>
				</tr>";
			}
		}

		$output .= "<tr><td colspan='2'>";
		$output .= "<div class='button-container'>";
		$output .= "<button id='downloadBtn' class='download-btn'>Download Table as CSV</button>";
		$output .= "</div>";
		$output .= "</td></tr>";
		$output .= "</table>";

		return $output;

	}
 
 function remote_onsite_report($year,$month,$sort_column){
 
 	if(!acl::has_access($_SESSION['id'],"billing admin","read")) return 1;
 	$next_month = $month +1;
 	if ($next_month > 12) {
 		$next_month = 1;
 		$next_year = $year +1;
 	}
 	else {
 		$next_year = $year;
 	}
 
 	$prev_month = $month - 1;
 	if ($prev_month == 0){
 		$prev_month = 12;
 		$prev_year= $year -1;
 	}
 	else {
 		$prev_year= $year;
 	}
 
 	print '<br />
	<table><tr><td colspan="3" align="center">';
 	print "<a href=\"?module=billing_reports&action=remote_onsite_report&year=$prev_year&month=$prev_month\" style=\"color: #1A53FF\">". date(F, mktime(0, 0, 0, $prev_month, 1, $prev_year)) ." $prev_year << Previous</a> ";
 	print "&nbsp&nbsp";
 	print " <b><a href=?module=billing_reports&action=remote_onsite_report&year=$year&month=$month>".date(F, mktime(0, 0, 0, $month, 1, $year))." $year</a></b>";
 	print "&nbsp&nbsp";
 	print " <a href=\"?module=billing_reports&action=remote_onsite_report&year=$next_year&month=$next_month\" style=\"color: #1A53FF\">Next >> ". date(F, mktime(0, 0, 0, $next_month, 1, $next_year)) ." $next_year</a><p>";
 	print '</td>
 		</tr>
 		</table><br />';
 	
 
 	billing_reports::total($year,$month);
 
 	print '<br />';
    print '<table class=\"client\"  width = 600>
         <tr><td valign = "top">';
    billing_reports::hours_rem_type($year,$month);
    print '</td><td>&nbsp;</td>
    
    		<td valign = "top">';
    billing_reports::hours_site_type($year,$month);
    print '</td>
    		<td width = 100>&nbsp;</td>
    		
    		
    		
     <td valign = "top">';
    billing_reports::hours_per_type($year,$month);
    print '</td><td>&nbsp;</td>		
    		
    		
    		
   </tr></table>';
    
    
    
	print '<p>&nbsp;</p>';
	print' <p><b><i> Click on a techie to view his/her billing</i></b></p>';
 
 	print '<table class=\"client\">
         <tr><td valign = "top">';
 	billing_reports::report_onsite($year,$month,$sort_column);
 	print '</td><td>&nbsp;</td>
   
   		 <td valign = "top">';
 	billing_reports::report_remote($year,$month,$sort_column);
 	print '</td><td >&nbsp;</td>
   
   		 <td valign = "top">';
 	billing_reports::report_unknown($year,$month,$sort_column);
 	print '</td>';
 		 
	print '<td >&nbsp;</td>';
 	
 print'	<td valign = "top">';
 	
 	billing_reports::obligated_hours($year,$month,$sort_column);
 	print'</td> <td >&nbsp;</td>';
 
 print'	<td valign = "top">';
 	
 	print "<table width = '400' class=\"client\"padding=\"none\" >";
 	print "<tr><span class=\"head\" align=\"right\">Fields Description</span></tr>";
 	
 	print
 	"
	<tr><td class=\"client\"><font color=\"blue\">Remote Totals</font></td><td class=\"client\">A total of all hours used remotely</td></tr>
	<tr><td class=\"client\"><font color=\"blue\">On-Site Totals </font></td><td class=\"client\"> A total of all hours used onsite </td></tr>
	<tr><td class=\"client\"><font color=\"blue\">Totals Per Type</font></td><td class=\"client\">A total of all hours used remotely and onsite for each type</td></tr>
 	<tr><td class=\"client\"><font color=\"blue\">Obligated Hours</font></td><td class=\"client\">Total of contract hours to be covered for all customers per techie</td></tr>

	</tr>";
 	print "</table>
 	</td>
 	</tr></table>";

 }
 
 ////total hours remote and onsite////////
 function total($year,$month){
 	if(!acl::has_access($_SESSION['id'],"billing admin","read")) return 1;
 	if($month < 10){
 		$month = '0'.$month;
 	}
 	$moncalc=$year.'-'.$smonth.'-'.'01';
 	$fulldate=$year.'-'.$month;
 	$fulldate2=$year.'-'.$month.'-'.'00';
 
 	$totalhours1 = "SELECT
 	ifnull(SUM(msi_billing.bill_hrs),0)
 	FROM
 	msi_billing
 	WHERE
 	date_format(msi_billing.date,'%Y-%m') = '$fulldate'" ;
 
 
 	$totalhours = sql::sql_run($totalhours1);
 	print '<span class="head"> Total Billed Hours : ' .$totalhours[0][0].' </span> <br />';

 	
 }
 
 function report_onsite($year,$month,$sort_column=NULL){

 	if(!isset($sort_column))
 		$sort_column = "username";
 
 	$sql_onsite="SELECT msi_users.username,
 	ifnull(bill.hours,0)
 	FROM msi_users
 	LEFT OUTER JOIN (select  msi_billing.techie,sum(ifnull(msi_billing.bill_hrs,0))as hours
 	from msi_billing
 	where
 	MONTH ( msi_billing.date ) = $month AND year( msi_billing.date ) = $year
 	AND ((msi_billing.site = 'On-Site')OR (msi_billing.after_hour = 'On-Site'))
 	GROUP BY msi_billing.techie) bill
 
 	ON msi_users.id = bill.techie
 	Where msi_users.activetechie = 'yes'
 	ORDER BY $sort_column";
 	 
 	$results_onsite = sql::sql_run($sql_onsite);
 	print "<span class=\"head\"> Onsite Totals</span>
 	<table class=\"client\">
 	<tr class=\"head\">
 
 
 	<td class=\"client-link\"><a href=\"?module=billing_reports&action=remote_onsite_report&year=$year&month=$month&sort_column=username\">Name</td>
 	<td class=\"client-link\"><a href=\"?module=billing_reports&sort_column=hours&year=$year&month=$month&action=remote_onsite_report\">Hours</td></tr>";
 
 	if(is_array($results_onsite)){
 		foreach ($results_onsite as $results_onsite) {
 			$total = $total + $results_onsite[1];
 			print "<tr>
 			<td class=\"client\"><a href =\"?module=billing_reports&action=monthly_gap&year=$year&month=$month&techie=$results_onsite[0]&sort_column=company\">".ucfirst($results_onsite[0])."</td>
 			<td class=\"client\">$results_onsite[1]</td>
 			</tr>";
 		}
 	}
 	print "<tr><td class=\"client\"><b>Total :</b></td><td class=\"client\">$total</td></tr>";
 	print "</table>";
 	 
 }
 
 function obligated_hours ($year,$month,$sort_column=NULL) {
 	if(!isset($sort_column))
 		$sort_column = "username";
 	
 	if(!isset($month)&&!isset($year))
 	{
 	
 		$year = date(Y);
 		$month = date(n);
 	
 	}
 	$next_month = $month +1;
 	if ($next_month > 12) {
 		$next_month = 1;
 		$next_year = $year +1;
 	}
 	else {
 		$next_year = $year;
 	}
 	$prev_month = $month - 1;
 	if ($prev_month == 0){
 		$prev_month = 12;
 		$prev_year= $year -1;
 	}
 	// elseif ($prev_month < 10){
 	//  $prev_month = '0'.$prev_month;
 	//  }
 	else {
 		$prev_year= $year;
 	}
 	//add leading 0 for months less than 10
 	if($month < 10){
 		$strmonth = '0'.$month;
 	}else{
 		$strmonth = $month;
 	}
 	//end add leading
 
 	$fulldate=$year.'-'.$strmonth;
 	$fulldate2=$year.'-'.$strmonth.'-'.'00';
 	$fulldateprev= $prev_year.'-'.$prev_month;
 	$fulldateprev2=$prev_year.'-'.$prev_month.'-'.'00';
 	
 	$obligation = sql::sql_run("SELECT
 			msi_contract.primarytech as username,
 			ifnull(sum(msi_contract.hours),0),
 			ifnull(sum(msi_contract.server_hours),0)

 			FROM
 			msi_contract,msi_clients
 			WHERE
 			msi_clients.id = msi_contract.client
 			AND
 			(('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
 			AND
 			date_format( msi_contract.enddate,'%Y-%m'))
 			OR
 			(msi_contract.terminating='no'and '$fulldate2'>=date_format(msi_contract.startdate,'%Y-%m-00'))
 			OR
 			(('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
 			AND
 			date_format( msi_contract.terminatingdate,'%Y-%m'))
 			AND
 			msi_contract.terminating='yes'))
 			Group By username
 			ORDER BY $sort_column " );
 
 	print "<span class=\"head\"> Obligated Hours</span> 
 	<table class=\"client\">
 	<tr class=\"head\">
 
 	<td class=\"client-link\"><a href=\"?module=billing_reports&action=remote_onsite_report&year=$year&month=$month&sort_column=username\">Techie</td>
 	<td class=\"client-link\"><a href=\"?module=billing_reports&sort_column=hours&year=$year&month=$month&action=remote_onsite_report\">Hours</td></tr>";
 	
 	if(is_array($obligation)){
 		foreach ($obligation as $obligation) {
 			$total = $obligation[1] + $obligation[2];
 			$techie = $obligation[0]; 
 			$tot = $tot+ $total;
 			print "<tr>
 			<td class=\"client\">".ucfirst($techie)."</td>
 			<td class=\"client\">$total</td>
 			</tr>";
 		}
 	}
	print "<tr><td class=\"client\"><b>Total :</b></td><td class=\"client\">$tot</td></tr>";
 	print "</table>";
 		
 } 

 function hours_rem_type($year,$month){
 
 	$totremote = "select sum(ifnull(msi_billing.bill_hrs,0))as hours,msi_billing.type
 	
	from msi_billing where
	MONTH ( msi_billing.date ) = $month AND year( msi_billing.date ) = $year
	AND (msi_billing.after_hour = 'Remote/Telephonic' OR msi_billing.site = 'Remote/Telephonic')
    Group by msi_billing.type ";
  
 
 	print "
 	<span class=\"head\" > Remote Totals</span>
 	<table class=\"client\"  width = 200 >
 	<tr class=\"head\">
 	
 	
 	<td>Type</td>
 			<td>Hours</td></tr>
 			";
 	
 	$results_rem = sql::sql_run($totremote);
 	if(is_array($results_rem)){
 		foreach ($results_rem as $results_rem) {
 			$totalr = $totalr + $results_rem[0];
 
 			
 			print "<tr>
 			<td class=\"client\">".ucfirst($results_rem[1])."</td>
 			<td class=\"client\">$results_rem[0]</td>
 			</tr>";
 		}
 	}
 	print "<tr><td class=\"client\"><b>Total :</b></td><td class=\"client\">$totalr</td></tr>";
 	print "</table>";
 	
 }
 

 function hours_per_type($year,$month){
 
 	$tottype = "select sum(ifnull(msi_billing.bill_hrs,0))as hours,msi_billing.type
 
 	from msi_billing where
 	MONTH ( msi_billing.date ) = $month AND year( msi_billing.date ) = $year
 	Group by msi_billing.type ";
 
 	print "
 	<span class=\"head\"> Totals Per Type(Remote + On-Site)</span>
 	<table class=\"client\" width = 250>
 	<tr class=\"head\">
    <td>Type</td>
 	<td>Hours</td></tr>";
 
 	$results_type = sql::sql_run($tottype);
 	 if(is_array($results_type)){
 		foreach ($results_type as $results_type) {
 			$totaltype = $totaltype + $results_type[0];
 
 
 			print "<tr>
 			<td class=\"client\">$results_type[1]</td>
 			<td class=\"client\">$results_type[0]</td>
 			</tr>";
 		}
 	}
 	print "<tr><td class=\"client\"><b>Total :</b></td><td class=\"client\">$totaltype</td></tr>";
 	print "</table>";
 
 } 

 function hours_site_type($year,$month){
 
 	$totsite = "select sum(ifnull(msi_billing.bill_hrs,0))as hours,msi_billing.type
 
 	from msi_billing where
 	MONTH ( msi_billing.date ) = $month AND year( msi_billing.date ) = $year
 	AND (msi_billing.after_hour = 'On-Site' OR msi_billing.site = 'On-Site')
 	Group by msi_billing.type ";
 	 
 	print "
 	<span class=\"head\"> On-Site Totals</span>
 	
 	<table class=\"client\" width = 200>
 	<tr class=\"head\">
    <td>Type</td>
 	<td>Hours</td></tr>";
 
 	$results_site = sql::sql_run($totsite);
 	if(is_array($results_site)){
 		foreach ($results_site as $results_site) {
 			$totalsite = $totalsite + $results_site[0];
 
           print "<tr>
 			<td class=\"client\">$results_site[1]</td>
 			<td class=\"client\">$results_site[0]</td>
 			</tr>";
 		}
 	}
 	print "<tr><td class=\"client\"><b>Total :</b></td><td class=\"client\">$totalsite</td></tr>";
 	print "</table>";
 }
 
 function report_unknown($year,$month,$sort_column=NULL){
 	if(!isset($sort_column))
 		$sort_column = "username";
 
 	$sql_unknown="SELECT msi_users.username,
 	ifnull(bill.hours,0)
 	FROM msi_users
 	LEFT OUTER JOIN (select  msi_billing.techie,sum(ifnull(msi_billing.bill_hrs,0))as hours
 	from msi_billing
 	where
 	MONTH ( msi_billing.date ) = $month AND year( msi_billing.date ) = $year
 	AND ((msi_billing.site != 'On-Site') and (msi_billing.after_hour != 'On-Site'))
 	AND ((msi_billing.site != 'Remote/Telephonic') and (msi_billing.after_hour != 'Remote/Telephonic'))
 	GROUP BY msi_billing.techie) bill
 
 	ON msi_users.id = bill.techie
 	Where msi_users.activetechie = 'yes'
 	ORDER BY $sort_column";

 
 	$results_unknown = sql::sql_run($sql_unknown);
 	if(is_array($results_unknown)){
 		print "<span class=\"head\"> Unknown Totals</span>
 		<table class=\"client\">
 		<tr class=\"head\">
 		<td class=\"client-link\"><a href=\"?module=billing_reports&action=remote_onsite_report&year=$year&month=$month&sort_column=username\">Name</td>
 		<td class=\"client-link\"><a href=\"?module=billing_reports&sort_column=hours&year=$year&month=$month&action=remote_onsite_report\">Hours</td></tr>";
 		 
 		foreach ($results_unknown as $results_unknown) {
 		$total = $total + $results_unknown[1];
 		print "<tr>
 		<td class=\"client\"><a href =\"?module=monthly_gap&action=techie_billing&year=$year&month=$month&techie=$results_unknown[0]\">".ucfirst($results_unknown[0])."</td>
 		<td class=\"client\">$results_unknown[1]</td>
 		</tr>";
 		}
 		print "<tr><td class=\"client\"><b>Total :</b></td><td class=\"client\">$total</td></tr>";
 		print "</table>";
	}
}
 
 function report_remote($year,$month,$sort_column=NULL){
 if(!isset($sort_column))
 	$sort_column = "username";
 
 $sql_remote="SELECT msi_users.username,
 ifnull(bill.hours,0)
 FROM msi_users
 LEFT OUTER JOIN (select  msi_billing.techie,sum(ifnull(msi_billing.bill_hrs,0))as hours
 from msi_billing
 where
 MONTH ( msi_billing.date ) = $month AND year( msi_billing.date ) = $year
 AND ((msi_billing.site = 'Remote/Telephonic')
 OR (msi_billing.after_hour = 'Remote/Telephonic'))
 GROUP BY msi_billing.techie) bill
 
 ON msi_users.id = bill.techie
 Where msi_users.activetechie = 'yes'
 ORDER BY $sort_column";
 
 $results_remote = sql::sql_run($sql_remote);
 print "<span class=\"head\"> Remote Totals</span>
 <table class=\"client\">
 <td class=\"client-link\"><a href=\"?module=billing_reports&action=remote_onsite_report&year=$year&month=$month&sort_column=username\">Name</td>
 <td class=\"client-link\"><a href=\"?module=billing_reports&sort_column=hours&year=$year&month=$month&action=remote_onsite_report\">Hours</td></tr>";
 
 if(is_array($results_remote)){
 		foreach ($results_remote as $results_remote) {
 		$total = $total + $results_remote[1];
 		print "<tr>
 		<td class=\"client\"><a href =\"?module=billing_reports&action=monthly_gap&year=$year&month=$month&techie=$results_remote[0]\">".ucfirst($results_remote[0])."</td>
 		<td class=\"client\">$results_remote[1]</td>
 				</tr>";
 		}
 		}
 				print "<tr><td class=\"client\"><b>Total :</b></td><td class=\"client\">$total</td></tr>";
 				print "</table>";
 
 }
 function techie_billing($techie,$year,$month,$sort_column=NULL){
 
 	if(!acl::has_access($_SESSION['id'],"billing admin","read")) return 1;
 
 	if(!isset($techie)){
 		$techie = $_SESSION['username'];
 	}
 
 	if(!isset($sort_column))
 		$sort_column = "date";
 
 	if($sort_column == "date"){
 		$sort_column = "date,arrive_time" ;}
 
 		$techies_billing = "SELECT
 		msi_billing.date,
 		msi_clients.company,
 		msi_billing.bill_hrs,
 		msi_billing.nonbill_hrs,
 		msi_billing.type,
 		msi_billing.description,
 		msi_billing.arrive_time,
 		msi_billing.depart_time
 
 		FROM msi_billing, msi_clients, msi_users
 		WHERE msi_clients.id = msi_billing.client AND msi_billing.techie = msi_users.id AND msi_users.username = \"$techie\"
 		AND month(msi_billing.date) = \"$month\" and year(msi_billing.date) = \"$year\" order by $sort_column";
 
 		$next_month = $month +1;
 		if ($next_month > 12) {
 			$next_month = 1;
 			$next_year = $year +1;
 		}
 		else {
 			$next_year = $year;
 		}
 
 		$prev_month = $month - 1;
 		if ($prev_month == 0){
 			$prev_month = 12;
 			$prev_year = $year -1;
 		}
 		else {
 			$prev_year= $year;
 		}
 
 
 		$t_billing = sql::sql_run($techies_billing);
 
 		print '<br />
	<table><tr><td colspan="3" align="center">';
 		print "<a href=\"?module=billing_reports&action=techie_billing&year=$prev_year&month=$prev_month&techie=$techie\" style=\"color: #1A53FF\">". date(F, mktime(0, 0, 0, $prev_month, 1, $prev_year)) ." $prev_year << Previous</a> ";
 		print "&nbsp&nbsp";
 		print " <b><a href=?module=billing_reports&action=techie_billing&year=$year&month=$month&techie=$techie>". date(F, mktime(0, 0, 0, $month, 1, $year)) ." $year</a></b>";
 		print "&nbsp&nbsp";
 		print " <a href=\"?module=billing_reports&action=techie_billing&year=$next_year&month=$next_month&techie=$techie\" style=\"color: #1A53FF\">Next >> ". date(F, mktime(0, 0, 0, $next_month, 1, $next_year)) ." $next_year</a><p>";
 		print '</td>
 		</tr>
</table>';
 		print "<span class=\"head\"> Billing Report for ".ucfirst($techie)."</span>";
 
 		print" <table class=\"client\">
 		<tr class=\"head\">
 
 		<td class=\"client-link\"><a href=\"?module=billing_reports&sort_column=date&year=$year&month=$month&action=techie_billing&techie=$techie\">Date</a></td>
 		<td class=\"client-link\"><a href=\"?module=billing_reports&sort_column=company&year=$year&month=$month&action=techie_billing&techie=$techie\">Client</a></td>
 		<td>Billable hours</td>
 		<td>Non Billable hours</td>
 		<td>Arrival Time</td>
 		<td>Departure Time</td>
 		<td class=\"client-link\"><a href=\"?module=billing_reports&sort_column=type&year=$year&month=$month&action=techie_billing&techie=$techie\">Type</a></td>
 		<td>Description</td>
 		</tr>";
   $sum = 0;
   $sum2 = 0;
 		if(is_array($t_billing)){
 			foreach ($t_billing as $t_billing) {
 				
 				$sum += $t_billing[2];
 				$sum2 += $t_billing[3];
 
 				print "<tr>
 				<td class=\"client\">$t_billing[0]</td>
 				<td class=\"client\">$t_billing[1]</td>
 				<td class=\"client\">$t_billing[2]</td>
 				<td class=\"client\">$t_billing[3]</td>
 				<td class=\"client\">$t_billing[6]</td>
 				<td class=\"client\">$t_billing[7]</td>
 				<td class=\"client\">$t_billing[4]</td>
 				<td class=\"client\">$t_billing[5]</td>
 				</tr>";
 			}
 			print "<tr>
 					
 					<td colspan =\"2\" class=\"client\" align =\"right\" ><b>Totals :</b><t/d>
 					<td colspan =\"1\" class=\"client\" ><b>".$sum."</b><t/d>
 					<td colspan =\"5\" class=\"client\"><b>".$sum2."</b><t/d>
 					</tr>";
 		}
 
 		print "</table>";
 
 }  
 
 
 function techie_billing_monthlygap($techie,$year,$month,$sort_column=NULL){
 
 	if(!acl::has_access($_SESSION['id'],"billing admin","read")) return 1;
 
 	if(!isset($techie)){
 		$techie = $_SESSION['username'];
 	}
 
 	if(!isset($sort_column))
 		$sort_column = "date";
 
 	if($sort_column == "date"){
 		$sort_column = "date,arrive_time" ;}
 
 		$next_month = $month +1;
 		if ($next_month > 12) {
 			$next_month = 1;
 			$next_year = $year +1;
 		}
 		else {
 			$next_year = $year;
 		}
 
 		$prev_month = $month - 1;
 		if ($prev_month == 0){
 			$prev_month = 12;
 			$prev_year = $year -1;
 		}
 		else {
 			$prev_year= $year;
 		}
 
 		print '<br />
	<table><tr><td colspan="3" align="center">';
 		print "<a href=\"?module=billing_reports&action=techie_billing_monthlygap&year=$prev_year&month=$prev_month&techie=$techie\" style=\"color: #1A53FF\">". date(F, mktime(0, 0, 0, $prev_month, 1, $prev_year)) ." $prev_year << Previous</a> ";
 		print "&nbsp&nbsp";
 		print " <b><a href=?module=billing_reports&action=techie_billing_monthlygap&year=$year&month=$month&techie=$techie>". date(F, mktime(0, 0, 0, $month, 1, $year)) ." $year</a></b>";
 		print "&nbsp&nbsp";
 		print " <a href=\"?module=billing_reports&action=techie_billing_monthlygap&year=$next_year&month=$next_month&techie=$techie\" style=\"color: #1A53FF\">Next >> ". date(F, mktime(0, 0, 0, $next_month, 1, $next_year)) ." $next_year</a><p>";
 		print '</td>
 		</tr>
</table>';
 		print "<span class=\"head\"> Billing Report for ".ucfirst($techie)."</span>";
 
 		print" <table class=\"client\">
 		<tr class=\"head\">
 
 		<td class=\"client-link\"><a href=\"?module=billing_reports&sort_column=date&year=$year&month=$month&action=techie_billing_monthlygap&techie=$techie\">Date</a></td>
 		<td class=\"client-link\"><a href=\"?module=billing_reports&sort_column=company&year=$year&month=$month&action=techie_billing_monthlygap&techie=$techie\">Client</a></td>";
 		print "	<td>Billable hours</td>
 		<td>Non Billable hours</td>
 		<td>Start Gap</td>
 		<td>Arrival Time</td>
 		<td>Departure Time</td>
 		<td>End Gap</td>
 		<td class=\"client-link\"><a href=\"?module=billing_reports&sort_column=type&year=$year&month=$month&action=techie_billing_monthlygap&techie=$techie\">Type</a></td>
 		<td>Description</td>
 		</tr>";
 
 		$totrows =  sql::sql_run("SELECT
 
 				count(*)
 				FROM msi_billing, msi_clients, msi_users
 				WHERE msi_clients.id = msi_billing.client AND msi_billing.techie = msi_users.id AND msi_users.username = \"$techie\"
 				AND month(msi_billing.date) = \"$month\" and year(msi_billing.date) = \"$year\" order by msi_billing.date,msi_billing.id");
 
 		$sum = 0;
 		$sum2 = 0;
 		$numrows = $totrows[0][0];
 
 		$techies_billing = "SELECT
 		msi_billing.date,
 		msi_clients.company,
 		msi_billing.bill_hrs,
 		msi_billing.nonbill_hrs,
 		msi_billing.type,
 		msi_billing.description,
 		msi_billing.arrive_time,
 		msi_billing.depart_time,
 		msi_billing.id
 		 
 		FROM msi_billing, msi_clients, msi_users
 		WHERE msi_clients.id = msi_billing.client AND msi_billing.techie = msi_users.id AND msi_users.username = \"$techie\"
 		AND month(msi_billing.date) = \"$month\" and year(msi_billing.date) = \"$year\" order by msi_billing.date,msi_billing.arrive_time";
 		 
 		$t_billing = sql::sql_run($techies_billing);
 
 		foreach ($t_billing as $t_billing) {
 
 			$time = strtotime($t_billing[6]);
 			 
 			$depart = "SELECT
 			 
 			msi_billing.depart_time,
 			msi_billing.id,
 			msi_billing.date
 			 
 			FROM msi_billing, msi_clients, msi_users
 			 
 			WHERE msi_clients.id = msi_billing.client AND msi_billing.techie = msi_users.id AND msi_users.username = \"$techie\"
 			AND month(msi_billing.date) = \"$month\" and year(msi_billing.date) = \"$year\"
 			And TIME(msi_billing.arrive_time) <  TIME('$t_billing[6]') and msi_billing.date = \"$t_billing[0]\" order by msi_billing.date,msi_billing.arrive_time";
 
 			 
 			$arr = "SELECT
 
 			msi_billing.arrive_time,
 			msi_billing.id,
 			msi_billing.date
 			
 
 			FROM msi_billing, msi_clients, msi_users
 
 			WHERE msi_clients.id = msi_billing.client AND msi_billing.techie = msi_users.id AND msi_users.username = \"$techie\"
 			AND month(msi_billing.date) = \"$month\" and year(msi_billing.date) = \"$year\"
 			And TIME(msi_billing.arrive_time) > TIME('$t_billing[6]') and msi_billing.date = \"$t_billing[0]\" order by msi_billing.date,msi_billing.arrive_time ";
 			 
 			 
 			//echo $depart."<br />";
 
 			$endtime = "17:00";
 			$t_depart = sql::sql_run($depart);
 			if(!is_array($t_depart)){
 				$departure = "8:00";
 
 			}
 			else {
 				foreach ($t_depart as $t_depart){
 					$departure = $t_depart[0];
 					//echo $t_depart[2];
 				}
 			}
 
 			$t_arrive = sql::sql_run($arr);
 			if(!is_array($t_arrive)){
 				$hours2 = (strtotime($endtime) - strtotime($t_billing[7]))/3600 ;
 			}
 			else{
 				//	foreach ($t_arrive as $t_arrive){
 				//$arive= $t_arrive[0];
 				//	}
 
 				if(strtotime($t_billing[7]) > strtotime($arive) ){
 					$hours2 = 0;
 					 
 				}
 				else{
 					$hours2 = (strtotime($endtime) - strtotime($t_billing[7]))/3600 ;
 					 
 				}
 					
 			}
 			 
 			$sum +=  $t_billing[2];
 			$sum2 += $t_billing[3];
 
 			$arrival = $t_billing[6];
 			
 			if(strtotime($arrival) < strtotime('8:00')){
 				$arrival = '8:00';
 			}
 			
 			$hours = (strtotime($arrival) - strtotime($departure))/3600;
 			$hours = round($hours, 2);
 			$hours2 = round($hours2, 2);
 
 			print "<tr>
 			<td class=\"client\">$t_billing[0]</td>
 			<td class=\"client\">$t_billing[1]</td>";
 			print " <td class=\"client\">$t_billing[2]</td>
 			<td class=\"client\">$t_billing[3]</td>
 			<td class=\"client\">$hours</td>
 			<td class=\"client\">$t_billing[6]</td>
 			<td class=\"client\">$t_billing[7]</td>
 			<td class=\"client\">$hours2</td>
 			<td class=\"client\">$t_billing[4]</td>
 			<td class=\"client\">$t_billing[5]</td>
 			</tr>";
 			$hourst += $hours;
 			$hours2t += $hours2;
 			
 			
 			
 		//echo	
 		//if($t_depart[2] = $t_billing[0]){
 			//echo $t_billing[0]." : ".$t_depart[2]."<br />";
 		//	echo $t_depart[2]." : ".$hours."<br />";
 		//}
 
 		}
 		
 
 		print "<tr>
 					<td colspan =\"2\" class=\"client\" align =\"right\" ><b>Totals :</b><t/d>
 					<td colspan =\"1\" class=\"client\" ><b>".$sum."</b><t/d>
 					<td colspan =\"1\" class=\"client\"><b>".$sum2."</b><t/d>
 					<td colspan =\"3\" class=\"client\" ><b>".$hourst."</b><t/d>
 					<td colspan =\"5\" class=\"client\"><b>".$hours2t."</b><t/d>
 					</tr>";
 
 		print "</table>";
 
 } 


 function monthly_gap($techie,$year,$month,$sort_column=NULL){
 
 	if(!acl::has_access($_SESSION['id'],"billing admin","read")) return 1;
 
 	if(!isset($techie)){
 		$techie = $_SESSION['username'];
 	}
 
 	if(!isset($sort_column))
 		$sort_column = "date";
 
 	if($sort_column == "date"){
 		$sort_column = "date,arrive_time" ;}
 
 		$next_month = $month +1;
 		if ($next_month > 12) {
 			$next_month = 1;
 			$next_year = $year +1;
 		}
 		else {
 			$next_year = $year;
 		}
 
 		$prev_month = $month - 1;
 		if ($prev_month == 0){
 			$prev_month = 12;
 			$prev_year = $year -1;
 		}
 		else {
 			$prev_year= $year;
 		}
 		
 		print '<br />';
 		print "<span> <b> You are viewing morning & afternoon gaps</b> </span>";
 	
 	print '<br />
	<table><tr><td colspan="3" align="center">';
 		print "<a href=\"?module=billing_reports&action=monthly_gap&year=$prev_year&month=$prev_month&techie=$techie\" style=\"color: #1A53FF\">". date(F, mktime(0, 0, 0, $prev_month, 1, $prev_year)) ." $prev_year << Previous</a> ";
 		print "&nbsp&nbsp";
 		print " <b><a href=?module=billing_reports&action=monthly_gap&year=$year&month=$month&techie=$techie>". date(F, mktime(0, 0, 0, $month, 1, $year)) ." $year</a></b>";
 		print "&nbsp&nbsp";
 		print " <a href=\"?module=billing_reports&action=monthly_gap&year=$next_year&month=$next_month&techie=$techie\" style=\"color: #1A53FF\">Next >> ". date(F, mktime(0, 0, 0, $next_month, 1, $next_year)) ." $next_year</a>";
 		print '</td>';
 		print "&nbsp&nbsp";
 		print '<td>&nbsp;&nbsp;&nbsp;&nbsp;</td>';
 	//	print '<td>';
 			//	print "<b><a href=\"?module=billing_reports&action=techie_billing_monthlygap&year=$year&month=$month&techie=$techie\" style=\"color: #FF0000\"> Total Gaps</a></b></td>";
 				
 				print "&nbsp&nbsp";
 				print '<td>&nbsp;&nbsp;&nbsp;&nbsp;</td>';
 		//		print '<td>';
 			//	print " <b><a href=\"?module=billing_reports&action=between_gaps&year=$year&month=$month&techie=$techie\" style=\"color: #FF0000\"> Gaps in Between</a></b></td>";

 		//print '</td>	
 		//</tr>
//</table><br />';
 		
 		print "<br /><span class=\"head\"> Morning & Afternoon Gaps for ".ucfirst($techie)."</span><br />";
print "<br /><table>
		<tr>
		<td>";

 		
 		print" <table class=\"client\">
 		<tr class=\"head\">

 		<td>Date</a></td>
 		<td>Arrival Time</a></td>
 		<td>Morning Gap</td>
 		<td>Departure time</a></td>
 		<td>Afternoon Gap</td>
 		</tr>";

 		$endtime = "17:00";
 		$starttime = "8:00";
 
 $arrival = "SELECT
             msi_billing.date,
             msi_billing.arrive_time
             FROM msi_billing, msi_clients, msi_users
             WHERE msi_clients.id = msi_billing.client AND msi_billing.techie = msi_users.id AND msi_users.username = \"$techie\"
             AND month(msi_billing.date) = \"$month\" and year(msi_billing.date) = \"$year\"  group by msi_billing.date order by msi_billing.date,msi_billing.arrive_time ";		
 $t_arrival = sql::sql_run($arrival);
 				
 foreach ($t_arrival as $t_arrival) {
 	
 $departure = "SELECT
 msi_billing.date,
 msi_billing.depart_time,
 msi_billing.arrive_time
 FROM msi_billing, msi_clients, msi_users
 WHERE msi_clients.id = msi_billing.client AND msi_billing.techie = msi_users.id AND msi_users.username = \"$techie\"
 AND month(msi_billing.date) = \"$month\" and year(msi_billing.date) = \"$year\" AND
 msi_billing.date = \"$t_arrival[0]\" order by msi_billing.date,msi_billing.depart_time DESC limit 1";

 $t_departure = sql::sql_run($departure);

  if(!is_array($t_departure)) {
  	echo "no results"."<br />";
  }	
foreach($t_departure as $t_departure){
	$d_time = $t_departure[1];
	if(strtotime($d_time) > strtotime($endtime) || $t_departure[1] == "24:00"){
		$d_time = $endtime;
	}

	$a_time = $t_arrival[1];
	if(strtotime($t_departure[2])< strtotime($starttime)){
		$a_time = $starttime;
	}
	
	
	$morning =  (strtotime($a_time)- strtotime($starttime))/3600 ;
    $afternoon = (strtotime($endtime) - strtotime($d_time))/3600;
  }
 			$morning = round( $morning, 2);
 			$afternoon = round($afternoon, 2);
 
 			print "<tr>
 			<td class=\"client\">$t_departure[0]</td>
            <td class=\"client\">$t_arrival[1]</td>
 			<td class=\"client\">$morning</td>
 			<td class=\"client\">$t_departure[1]</td>
 			<td class=\"client\">$afternoon</td>
 			
 			</tr>";
 			$morning_tot += $morning;
 			$afternoon_tot += $afternoon;
 		}
 
 
 		print "<tr>
 				<td colspan =\"2\" class=\"client\" align =\"right\" ><b> Sub Totals :</b><t/d>
 				<td colspan =\"2\" class=\"client\"><b>".$morning_tot." </b> <i>(morning gaps)<t/d>
 				<td colspan =\"2\" class=\"client\"><b>".$afternoon_tot."</b> <i>(afternoon gaps)<t/d>
 				</tr>";
 		$grand = $morning_tot + $afternoon_tot;
 		
 		print "<tr>
 				<td colspan =\"2\" class=\"client\" align =\"right\" ><b> Grand Total :</b><t/d>
 				<td colspan =\"3\" class=\"client\"><b>".$grand."</b><t/d>
 				
 				</tr>";

 		print "</table>
 				</td>
 				<td colspan = '50'>&nbsp;&nbsp;&nbsp;&nbsp;</td>
 				<td valign ='top'>";

 		
 		print" <table class=\"client\">";
 				print "<span class=\"head\"> Descriptions </span>
 		<tr>
 	    <td class=\"client\">Morning Gap</td>
 		<td class=\"client\">Gap between 8:00 and the first visit of the day.</td>
 		</tr>

 				<tr>
 				<td class=\"client\">Afternoon Gap</td>
 	    <td class=\"client\">Gap between the last visit of the day and 17:00. </td>
 		</tr>
        </table>
 				</td>
 				</tr>
 				</table>";

 }


 function customer_summary($year,$month){
 
 	if(!acl::has_access($_SESSION['id'],"billing admin","read")) return 1;
 	$next_month = $month +1;
 	if ($next_month > 12) {
 		$next_month = 1;
 		$next_year = $year +1;
 	}
 	else {
 		$next_year = $year;
 	}
 
 	$prev_month = $month - 1;
 	if ($prev_month == 0){
 		$prev_month = 12;
 		$prev_year= $year -1;
 	}
 	else {
 		$prev_year= $year;
 	}
 
 	print '<br />
	<table><tr><td colspan="3" align="center">';
 	print "<a href=\"?module=billing_reports&action=customer_summary&year=$prev_year&month=$prev_month\" style=\"color: #1A53FF\">". date(F, mktime(0, 0, 0, $prev_month, 1, $prev_year)) ." $prev_year << Previous</a> ";
 	print "&nbsp&nbsp";
 	print " <b><a href=?module=billing_reports&action=customer_summary&year=$year&month=$month>". date(F, mktime(0, 0, 0, $month, 1, $year)) ." $year</a></b>";
 	print "&nbsp&nbsp";
 	print " <a href=\"?module=billing_reports&action=customer_summary&year=$next_year&month=$next_month\" style=\"color: #1A53FF\">Next >> ". date(F, mktime(0, 0, 0, $next_month, 1, $next_year)) ." $next_year</a><p>";
 	print '</td>
 		</tr>
 		</table>';
 
 	print '<table class=\"client\">
    <tr><td valign = "top">';
 	billing::report_client_summary2($year,$month);
 	print '</td><td>&nbsp;</td>
  
   	<td valign = "top">';
 	billing::unused_client_summary($year,$month);
 	print '</td><td>&nbsp;</td>
 
    <td valign = "top"> ';
 	billing::used_client_afterhours($year,$month);
 	print "</td><td>&nbsp;</td>
  
   	<td valign = 'top'>";
 	billing::used_client_pholiday($year,$month);
 
 
 	print "	<br />";
 
 	print "<table width = '400' class=\"client\"padding=\"none\" >";
 	print "<tr><span class=\"head\" align=\"right\">Fields Description</span></tr>";
 
 	print
 	"
	<tr><td class=\"client\"><font color=\"blue\">Total Hours Used</font></td><td class=\"client\">Total hours that have been used in the selected month only</td></tr>
	<tr><td class=\"client\"><font color=\"blue\">Contract Hours </font></td><td class=\"client\"> Contracted hours </td></tr>
	<tr><td class=\"client\"><font color=\"blue\">Unused Hours</font></td><td class=\"client\">Hours left over in the selected month</td></tr>
	</tr>";
 	print "</table><br>
 
</td></tr>	</table>";
 }
 
 function maintenance_contract_report($year,$month){
 	if(!acl::has_access($_SESSION['id'],"billing admin","read")) return 1;
 
 	$strmonth = '';
 	if($month < 10){
 		$strmonth = '0'.$month;
 	}else{
 		$strmonth = $month;
 	}
 
 	$fulldate=$year.'-'.$strmonth;
 	$fulldate2=$year.'-'.$strmonth.'-'.'00';
 	$prevdate=$year.'-'.($strmonth2-1);
 	$prevdate2=$year.'-'.($strmonth2-1).'-'.'00';
 
 	$sql = "
 	SELECT
 	msi_clients.company,
 	msi_mcontract.hours,
 	msi_mcontract.server_hours,
 	msi_mcontract.client,
 	msi_mcontract.type_maintenance,
 	msi_clients.id,
 	msi_mcontract.contractid
 
 	FROM
 	msi_mcontract,
 	msi_clients
 	WHERE
 	msi_clients.id = msi_mcontract.client
 	AND
 	(('$fulldate' between date_format( msi_mcontract.startdate,'%Y-%m')
 	AND
 	date_format( msi_mcontract.enddate,'%Y-%m'))
 	OR
 	(msi_mcontract.terminating='no'and '$fulldate2'>=date_format(msi_mcontract.startdate,'%Y-%m-00') )
 	OR
 	(('$fulldate' between date_format( msi_mcontract.startdate,'%Y-%m')
 	AND
 	date_format( msi_mcontract.terminatingdate,'%Y-%m'))
 	AND
 	msi_mcontract.terminating='yes'))
 
 	ORDER BY
 	msi_clients.company";
 
 	$results = sql::sql_run($sql);
 	$next_month = $month +1;
 	if ($next_month > 12) {
 		$next_month = 1;
 		$next_year = $year +1;
 	}
 	else {
 		$next_year = $year;
 	}
 
 	$prev_month = $month - 1;
 	if ($prev_month == 0){
 		$prev_month = 12;
 		$prev_year= $year -1;
 	}
 	else {
 		$prev_year= $year;
 	}
 	print "<br />";
 	print '<table><tr><td colspan="3" align="center">';
 	print "<a href=?module=billing_reports&action=maintenance_contract_report&year=$prev_year&month=$prev_month style=\"color: #1A53FF\">". date(F, mktime(0, 0, 0, $prev_month, 1, $prev_year)) ." $prev_year << Previous</a> ";
 	print "&nbsp&nbsp";
 	print " <b><a href=?module=billing_reports&action=maintenance_contract_report&year=$year&month=$month>". date(F, mktime(0, 0, 0, $month, 1, $year)) ." $year</a></b>";
 	print "&nbsp&nbsp";
 	print " <a href=?module=billing_reports&action=maintenance_contract_report&year=$next_year&month=$next_month style=\"color: #1A53FF\">Next >> ". date(F, mktime(0, 0, 0, $next_month, 1, $next_year)) ." $next_year</a><p>";
 	print '</td></tr>
           </table>';
 
 	print "<i>For a monthly report including all the maintenance billing for a client please click on the client name.</i><p>";
 	print "<br />";
 
    print '<table><tr><td>';
 	print "<span class=\"head\">Maintenance Contract Client Hours</span>";
 
 	print " <table class=\"client\">
                <tr class=\"head\">
  		<td>Company</td>
  		<td>Type of Maintenance Contract</td></tr>";
 
 	foreach ($results as $result) {
 		print "<tr>
 		<td class=\"client\"><a href=\"?module=billing_reports&action=maintenance_contract_client&id=$result[3]&year=$year&month=$month&cid=$result[6]\" style = \"text-decoration:none\">$result[0]</a>
 		</td>
 		<td class=\"client\">$result[4]</td>
 		</tr>";
 
 	}
 	print "</table>";

 }
 
 function maintenance_contract_client($id,$year,$month,$cid){
 	if(!acl::has_access($_SESSION['id'],"contracts","read")) return 1;
 	
 	if(isset($_GET['cid'])){
  $cid = $_GET['cid'];
 	}
 	$sql = 'SELECT msi_clients.company, msi_mbilling.date, msi_mbilling.bill_hrs, msi_mbilling.nonbill_hrs, msi_mbilling.description, msi_mbilling.invoice
    , msi_users.username, msi_mbilling.id, msi_mcontract.type_maintenance, msi_mbilling.invoiced_by,msi_mcontract.contractid
    FROM msi_users, msi_mbilling, msi_clients,msi_mcontract
 
 		WHERE msi_clients.id = msi_mbilling.client
 		AND msi_clients.id = msi_mcontract.client
 		AND msi_mcontract.client = msi_mbilling.client
 		AND msi_mcontract.client = msi_clients.id
 	
 		AND msi_mbilling.techie = msi_users.id
 			
 		AND msi_mcontract.type_maintenance = msi_mbilling.type_maintenance
		AND msi_mcontract.contractid =	"'.$cid.'"	
 		AND msi_mbilling.client = '.$id.'
    AND month(msi_mbilling.date) = '.$month.' and year(msi_mbilling.date)= '.$year.'
    ORDER BY `invoice` ASC';

 	$results = sql::sql_run($sql);
 	$next_month = $month +1;
 	if ($next_month > 12) {
 		$next_month = 1;
 		$next_year = $year +1;
 	}
 	else {
 		$next_year = $year;
 	}
 	$prev_month = $month - 1;
 	if ($prev_month == 0){
 		$prev_month = 12;
 		$prev_year= $year -1;
 	}
 	else {
 		$prev_year= $year;
 	}
 	print '<table><tr><td colspan="3" align="center">';
 	print "<a href=?module=billing_reports&action=maintenance_contract_client&id=$id&year=$prev_year&month=$prev_month&cid=$cid style=\"color: #1A53FF\">". date(F, mktime(0, 0, 0, $prev_month, 1, $prev_year)) ." $prev_year << Previous</a> ";
 	print "&nbsp&nbsp";
 	print " <b><a href=?module=billing_reports&action=maintenance_contract_client&id=$id&year=$year&month=$month&cid=$cid>". date(F, mktime(0, 0, 0, $month, 1, $year)) ." $year</a></b>";
 	print "&nbsp&nbsp";
 	print " <a href=?module=billing_reports&action=maintenance_contract_client&id=$id&year=$next_year&month=$next_month&cid=$cid style=\"color: #1A53FF\">Next >> ". date(F, mktime(0, 0, 0, $next_month, 1, $next_year)) ." $next_year</a><p>";
 	print '</td></tr></table>';
 	print "<span class=\"head\">Client Maintenance Contract Report for ".$results[0][0]."</span>
           <table class=\"client\">
           <tr class=\"head\"><td>Date</td><td>Billable Hours</td><td>Non Billable Hours</td><td>Type</td><td>Description</td><td>Techie</td></tr>";
 	$bill = 0;
 	$nonbill = 0;
 	if(is_array($results )){
 		foreach ($results as $result) {
 			$bill += $result[2];
 			$nonbill += $result[3];
 			print "<tr>
 			<td class=\"client\">$result[1]</td>
 			<td class=\"client\">$result[2]</td>
 			<td class=\"client\">$result[3]</td>
 			<td class=\"client\">$result[8]</td>
 			<td class=\"client\">$result[4]</td>
 			<td class=\"client\">".ucfirst($result[6])."</td></tr>";
 		}
 	}
 	print "<tr><td class=\"client\">Total:</td><td class=\"client\">$bill</td><td class=\"client\">$nonbill</td><td class=\"client\"></td><td class=\"client\"></td>
 	<td class=\"client\"></td></tr>
 	</table>";
 }
 
 
 function bill_nonbill($year,$month){
 	
 	
 	$next_month = $month +1;
 	if ($next_month > 12) {
 		$next_month = 1;
 		$next_year = $year +1;
 	}
 	else {
 		$next_year = $year;
 	}
 	$prev_month = $month - 1;
 	if ($prev_month == 0){
 		$prev_month = 12;
 		$prev_year= $year -1;
 	}
 	else {
 		$prev_year= $year;
 	}
 	print '<table><tr><td colspan="3" align="center">';
 	print "<a href=?module=billing_reports&action=bill_nonbill&year=$prev_year&month=$prev_month style=\"color: #1A53FF\">". date(F, mktime(0, 0, 0, $prev_month, 1, $prev_year)) ." $prev_year << Previous</a> ";
 	print "&nbsp&nbsp";
 	print " <b><a href=?module=billing_reports&action=bill_nonbill&year=$year&month=$month>". date(F, mktime(0, 0, 0, $month, 1, $year)) ." $year</a></b>";
 	print "&nbsp&nbsp";
 	print " <a href=?module=billing_reports&action=bill_nonbill&year=$next_year&month=$next_month style=\"color: #1A53FF\">Next >> ". date(F, mktime(0, 0, 0, $next_month, 1, $next_year)) ." $next_year</a><p>";
 	print '</td></tr></table>';
 	print "<span class=\"head\">Billable & Non Billable Hours </span>
           <table class=\"client\">
           <tr class=\"head\"><td>Client</td><td>Billable Hours</td><td>Non Billable Hours</td></tr>";
 	
 	
 	$results = sql::sql_run("SELECT DISTINCT company, id
    FROM `msi_clients`
    WHERE customer = \"1\"
    GROUP BY `company`");
 	
 	
 	
 	
  foreach($results as $results){
  	
//  echo "SELECT IFNULL(SUM(msi_billing.bill_hrs),0), IFNULL(SUM(msi_billing.nonbill_hrs),0)
  			// from msi_billing
  		//	 where msi_billing.client = '$results[1]'
  		//	AND month(msi_billing.date) = '$month'
  		//	AND year(msi_billing.date) = '$year' ";
  	$hours = sql::sql_run("SELECT IFNULL(SUM(msi_billing.bill_hrs),0), IFNULL(SUM(msi_billing.nonbill_hrs),0)
  			 from msi_billing
  			 where msi_billing.client = '$results[1]'
  			 AND month(msi_billing.date) = '$month'
  			 AND year(msi_billing.date) = '$year' 
  			 ");
  	
  	Print "<tr>
  	<td  class=\"client\">$results[0]</td>";
  	    foreach($hours as $hours){
  	    	$bill += $hours[0];
  	    	$nonbill += $hours[1];
  		print" <td  class=\"client\">$hours[0]</td>
       	 <td  class=\"client\">$hours[1]</td>
  		</tr>";
  	}
  	}
  	print "<tr>
  			<td  class=\"client\"><b>Total</b></td>
  			<td class=\"client\">$bill</td>
  			<td class=\"client\">$nonbill</td>
  			</tr>
  			</table>";
  			
  }
  
  
  function daily_gaps($techie,$year,$month){

  	if(!acl::has_access($_SESSION['id'],"billing admin","read")) return 1;
  	
  	if(!isset($techie)){
  		$techie = $_SESSION['username'];
  	}
  	
  	if(!isset($sort_column))
  		$sort_column = "date";
  	
  	if($sort_column == "date"){
  		$sort_column = "date,arrive_time" ;}
  	
  		$next_month = $month +1;
  		if ($next_month > 12) {
  			$next_month = 1;
  			$next_year = $year +1;
  		}
  		else {
  			$next_year = $year;
  		}
  	
  		$prev_month = $month - 1;
  		if ($prev_month == 0){
  			$prev_month = 12;
  			$prev_year = $year -1;
  		}
  		else {
  			$prev_year= $year;
  		}

  		print '<br />
 		
	<table><tr><td colspan="3" align="center">';
  		print "<a href=\"?module=billing_reports&action=daily_gaps&year=$prev_year&month=$prev_month&techie=$techie\" style=\"color: #1A53FF\">". date(F, mktime(0, 0, 0, $prev_month, 1, $prev_year)) ." $prev_year << Previous</a> ";
  		print "&nbsp&nbsp";
  		print " <b><a href=?module=billing_reports&action=daily_gaps&year=$year&month=$month&techie=$techie>". date(F, mktime(0, 0, 0, $month, 1, $year)) ." $year</a></b>";
  		print "&nbsp&nbsp";
  		print " <a href=\"?module=billing_reports&action=daily_gaps&year=$next_year&month=$next_month&techie=$techie\" style=\"color: #1A53FF\">Next >> ". date(F, mktime(0, 0, 0, $next_month, 1, $next_year)) ." $next_year</a><p>";
  		print '</td>
 		</tr>
</table>';
  		
  		
  		print "<span class=\"head\"> Billing Report for ".ucfirst($techie)."</span>";
  	
  		print" <table class=\"client\">
  		<tr class=\"head\">
  	
  		<td >Date</td>
  		<td Client</td>";
  		print "	<td>Billable hours</td>
  		<td>Non Billable hours</td>
  		<td>Start Gap</td>
  		<td>Arrival Time</td>
  		<td>Departure Time</td>
  		<td>End Gap</td>
  		<td class=\"client-link\"><a href=\"?module=billing_reports&sort_column=type&year=$year&month=$month&action=techie_billing_monthlygap&techie=$techie\">Type</a></td>
  		<td>Description</td>
  		</tr>";
  }
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
 	


}



?>