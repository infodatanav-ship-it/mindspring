<?php session_start(); // Get the cookies set as soon as possible 
header("Content-Type: application/x-msexcel");
header("Pragma: no-cache");
header("Expires: 0");
// $Id: export.php,v 1.2 2008/01/22 07:39:43 laurence Exp $
ini_set('error_reporting', E_ALL & ~E_NOTICE & ~E_STRICT & ~E_DEPRECATED);
error_reporting(1); 
?>

<?php
include("include/sql.php.inc");
include("include/acl.php.inc");
function export_client($id, $month, $year){
	 
	if(!acl::has_access($_SESSION['id'],"billing admin","read")) return 1;
	
	$year = date("Y");
	$month = date("n");

	if(isset($_REQUEST['id'])){ $id = $_REQUEST['id'];}
	if(isset($_REQUEST['year'])){ $year = $_REQUEST['year'];}
	if(isset($_REQUEST['username'])){ $username = $_REQUEST['username'];}
	if(isset($_POST['username'])){ $username = $_POST['username'];}
	if(isset($_REQUEST['month'])){ $month = $_REQUEST['month'];}
	if(isset($_REQUEST['module'])){ $module = $_REQUEST['module'];}
	if(isset($_REQUEST['action'])){ $action = $_REQUEST['action'];}	
	
	$prev_month = $month - 1;
	if ($prev_month == 0){
		$prev_month = 12;
		$prev_year= $year -1;
	}
	else {
		$prev_year= $year;
	}
	
	//add leading 0 for months less than 10
	if($month < 10){
		$strmonth = '0'.$month;
	}else{
		$strmonth = $month;
	}
	
	if($prev_month < 10){
		$strprevmonth = '0'.$prev_month;
	}else{
		$strprevmonth = $prev_month;
	}
	
	$fulldate=$year.'-'.$strmonth;
	$fulldate2=$year.'-'.$strmonth.'-'.'00';
	
	$fulldateprev=$prev_year.'-'.$strprevmonth;
	$fulldate2prev=$prev_year.'-'.$strprevmonth.'-'.'00';

/* $strprevmonth and $prev_year => To use for pulling billing for the prevous month
 * $month and $year => To use for pulling billing for current month
 */	

		
$sql = 'SELECT msi_clients.company,
       msi_billing.date,
       ifnull(msi_billing.bill_hrs,0)as hours,
       msi_billing.nonbill_hrs,
       msi_billing.description,
       msi_billing.invoice,
       msi_users.username,
       msi_billing.id,
       msi_billing.type,
       msi_billing.invoiced_by,
       msi_billing.site,
       msi_billing.after_hour,
	   msi_contract.server_hours,
	   msi_contract.hours
	  

      FROM msi_users, msi_billing,msi_clients,msi_contract
      WHERE msi_clients.id = msi_billing.client AND msi_billing.techie = msi_users.id 
		AND msi_clients.id = msi_contract.client		
		AND msi_billing.client = '.$id.'
      AND month(msi_billing.date) = '.$month.' and year(msi_billing.date)= '.$year.'
	 
      AND msi_billing.type = \'Contract\'
       		       		   AND 
	(("'.$fulldate.'" between date_format( msi_contract.startdate,\'%Y-%m\')
AND 
	date_format( msi_contract.enddate,\'%Y-%m\')) 
OR
 	(msi_contract.terminating=\'no\' and "'.$fulldate2.'">=date_format(msi_contract.startdate,\'%Y-%m-00\'))
OR 
	(("'.$fulldate.'" between date_format( msi_contract.startdate,\'%Y-%m\')
AND 
	date_format( msi_contract.terminatingdate,\'%Y-%m\'))
AND 
	msi_contract.terminating=\'yes\'))	
      group by msi_billing.date,msi_users.username,msi_billing.description
      ORDER BY msi_billing.date  ASC';


$avprev =  'SELECT msi_clients.company,
       msi_billing.date,
       ifnull(msi_billing.bill_hrs,0)as hours,
       msi_billing.nonbill_hrs,
       msi_billing.description,
       msi_billing.invoice,
       msi_users.username,
       msi_billing.id,
       msi_billing.type,
       msi_billing.invoiced_by,
       msi_billing.site,
       msi_billing.after_hour,
	   msi_contract.server_hours,
	   msi_contract.hours,
	   msi_contract.effective

      FROM msi_users, msi_billing,msi_clients,msi_contract
      WHERE msi_clients.id = msi_billing.client AND msi_billing.techie = msi_users.id 
	  AND msi_clients.id = msi_contract.client		
	  AND msi_billing.client = '.$id.'
      AND month(msi_billing.date) = '.$strprevmonth.' and year(msi_billing.date)= '.$year.'
	 
      AND msi_billing.type = \'Contract\'
       		       		   AND 
	(("'.$fulldateprev.'" between date_format( msi_contract.startdate,\'%Y-%m\')
AND 
	date_format( msi_contract.enddate,\'%Y-%m\')) 
OR
 	(msi_contract.terminating=\'no\' and "'.$fulldate2prev.'">=date_format(msi_contract.startdate,\'%Y-%m-00\'))
OR 
	(("'.$fulldateprev.'" between date_format( msi_contract.startdate,\'%Y-%m\')
AND 
	date_format( msi_contract.terminatingdate,\'%Y-%m\'))
AND 
	msi_contract.terminating=\'yes\'))	
      group by msi_billing.date,msi_users.username,msi_billing.description
      ORDER BY msi_billing.date  ASC ';


$c_name = 'SELECT msi_clients.company
from msi_clients
WHERE msi_clients.id ='.$id.'';

$name = sql::sql_run($c_name);


	$results = sql::sql_run($sql);
	$results2 = sql::sql_run($avprev);
	foreach($results2 as $results2){
	$available = $results2[14] ;
	}
    header('Content-Disposition: attachment; filename="'.$name[0][0].'-'.$year.'-'.$month.'.xls"');
  
	print"<table width = '60%' >
          <tr><td><font size = '2' color = 'blue'>Mindspring Computing<br>
          Unit 5, Doncaster Office Park<br>
          Punters Way, Kenilworth.<br>
          Tel: (021) 657 1780<br>
          Fax: (021) 671 7599
          </font></td><td></td><td></td><td><img src = 'http://intranet.mindspring.co.za/Mindspring.jpeg.jpg'  height ='69' width = '263' valign = 'top'/></td></tr></table><hr/>
          ";
	print "<h2><CENTER>SUPPORT REPORT FOR &nbsp : &nbsp ".date("F Y", mktime(0, 0, 0, $month,1,$year))."</CENTER></h2>";
	print "<h3><u><center>".$name[0][0]."</center></u></h3>";
	print "<h4><i><center>The shaded blocks fall over the contract</center></i></h4>";
	
	if($results){
		$b = 0;
		foreach($results as $results2){
			if($results2[10] == 'On-Site'){
				$b++;
			}
		}
		if($b == 0){
			print "<h2>No Support Records Available....</h2><hr />";
		}
		else {
	
	      print "<div>";
	      print "<table border = '1' width= '60%' >
          <th style ='background-color:silver' >Date </th>
          <th style ='background-color:silver' >Billable Hours</th>";
	      print "  <th style ='background-color:silver' > Time </th>";
	      print "  <th style ='background-color:silver' >&nbsp;&nbsp;&nbsp;Engineer&nbsp;&nbsp;&nbsp; </th>
          <th style ='background-color:silver' >Description  </th>";

	if(is_array($results)){
		foreach($results as $results){
			$total += $results[2];
			$hours = $results[12] + $results[13];
			
			if($results[10] == 'On-Site'){
			if($total > $hours ){
				
			$bg = "background-color:#ff6666";
			}
			
			$t += $results[2];
			print"<tr style ='$bg' >

    	            <td align= center width = '8%'>".$results[1]."</td>
    	            <td align = center width = 10%>".$results[2]."</td>";
			print " <td align = center width = 10%>".$results[11]."</td>";
			print " <td align = center width = 10%>".$results[6]."</td>";
			print " <td align = center width = 10%>".$results[4]."</td>";
			print " <tr>";
			}
		}
	}
	print "<tr style ='background-color:silver'><td align = center><b> Total </b></td>
    <td style ='background-color:silver' align = center><b>".$t."</b></td></tr>";
	print "</table>";
	print "</div>";
	print "<p>&nbsp;</p>";
		}
	}
		else {
			print "<h2>No records found ... </h2>
					<hr />" ;
	}

	$sql_over = 'SELECT msi_clients.company,
       msi_billing.date,
       ifnull(msi_billing.bill_hrs,0)as hours,
       msi_billing.nonbill_hrs,
       msi_billing.description,
       msi_billing.invoice,
       msi_users.username,
       msi_billing.id,
       msi_billing.type,
       msi_billing.invoiced_by,
       msi_billing.site,
       msi_billing.after_hour,
	   msi_contract.server_hours,
	   msi_contract.hours	
	
      FROM msi_users, msi_billing,msi_clients,msi_contract
      WHERE msi_clients.id = msi_billing.client AND msi_billing.techie = msi_users.id 
	  AND msi_clients.id = msi_contract.client
	  AND msi_billing.client = '.$id.'
      AND month(msi_billing.date) = '.$month.' and year(msi_billing.date)= '.$year.'
	  AND msi_billing.type = \'Contract\'
	  AND msi_contract.terminating=\'no\' 
      group by  msi_billing.date,msi_users.username, msi_billing.description
      ORDER BY msi_billing.date  ASC';
	//echo $sql_over;
$results3 = sql::sql_run($sql_over);
if($results3){
	
	$i = 0; 
	foreach($results3 as $results4){
		if($results4[10] == 'Remote/Telephonic'){
			$i++;
		}
	}
	if($i == 0){
		print "<h2>No Remote Records Available....</h2><hr />";
	}
	else {
	
print "<h2><CENTER>REMOTE REPORT FOR &nbsp : &nbsp ".date("F Y", mktime(0, 0, 0, $month,1,$year))."</CENTER></h2>";
print "<h4><i><center>The shaded blocks fall over the contract</center></i></h4>";

	print "<div>";
	print "<table border = '1' width= '60%' >
            <th style ='background-color:silver'>Date </th>
            <th style ='background-color:silver'>Billable Hours</th>";
	print " <th style ='background-color:silver'> Time </th>";
	print " <th style ='background-color:silver'>&nbsp;&nbsp;&nbsp;Engineer&nbsp;&nbsp;&nbsp; </th>
            <th style ='background-color:silver'>Description  </th>
		  ";

	if(is_array($results3)){
	
		foreach($results3 as $results3){
			$total3 += $results3[2];
			$hours = $results3[12] + $results3[13];
			if($results3[10] == 'Remote/Telephonic'){
            if($total3 > $hours ){
              $bg = "background-color:#ff6666";
            }
            else {$bg = "background-color:white";}
            		$t2 += $results3[2];
	
			print"<tr  style = '$bg'>
    	            <td align= center width = '8%'>".$results3[1]."</td>
    	            <td align = center width = 10%>".$results3[2]."</td>";
			print " <td align = center width = 10%>".$results3[11]."</td>";
			print " <td align = center width = 10%>".$results3[6]."</td>";
			print " <td align = center width = 10%>".$results3[4]."</td>";
			print " <tr>";
			print " </tr>";
            }
            }
		}
	
	print "<tr style ='background-color:silver'><td align = center><b> Total </b></td>
     <td style ='background-color:silver' align = center><b>".$t2."</b></td></tr>";
	print "</table>";
	print "</div>";
	print "<p>&nbsp;</p>";
}

}
	
    //////////////////////////////////////////////////////////////////////////////////////	
	//////////////PREVIOUS MONTH//////////////////////////////////////////////////////////

	$contract ="SELECT ifnull( msi_contract.hours, 0 ) + ifnull( msi_contract.server_hours, 0 )
	from msi_contract
	where msi_contract.client = \"$id\"
	
	and (('$fulldateprev' between date_format( msi_contract.startdate,'%Y-%m')
	and date_format( msi_contract.enddate,'%Y-%m')) or
	(msi_contract.terminating='no'and '$fulldate2prev'>=date_format(msi_contract.startdate,'%Y-%m-00'))
	or ('$fulldateprev' between date_format( msi_contract.startdate,'%Y-%m')
	and date_format( msi_contract.terminatingdate,'%Y-%m') and  msi_contract.terminating='yes') )
	";
	
	$totalhours = sql::sql_run($contract);
	$thours = $totalhours [0][0];
	
	$totprev ="Select sum(ifnull(msi_billing.bill_hrs,0))as hours
	from msi_billing
	where msi_billing.client= \"$id\"
	AND month(msi_billing.date) = $strprevmonth and year(msi_billing.date)= $prev_year
	AND msi_billing.type = \"Contract\"";

	$totalbillingprev = sql::sql_run($totprev);
	$tbillingprev = $totalbillingprev [0][0];
	
	$diffprev = $thours - $tbillingprev;
	if($diffprev >= 0)
	{
		$unusedprev = $diffprev;
		$exceedprev = 0;
		$tatementprev = $unusedprev." Support hours unused for ".date("F Y", mktime(0, 0, 0, $month,1,$year));
	
	}
	else if($diffprev < 0)
	{
		$unusedprev = 0;
		$exceedprev = $diffprev *(-1);
		$tatementprev =" Support hours exceeded by ".$exceed." for ".date("F Y", mktime(0, 0, 0, $month,1,$year));
	}
	
	//get invoiced and written off hours.
	$closeoffprev = "Select msi_close_off.invoiced, msi_close_off.writeoff,msi_close_off.closingmonth,msi_close_off.borrowed
	From msi_close_off
	Where msi_close_off.client = \"$id\"
	And month(msi_close_off.closingmonth) = \"$strprevmonth\"
	And year(msi_close_off.closingmonth) = \"$prev_year\"
	";

	$inv_borr = "SELECT
			ifnull(sum(msi_close_off.invoiced),0),
			ifnull(sum(msi_close_off.writeoff),0)
			from msi_close_off
			where msi_close_off.client = \"$id\"
			and msi_close_off.closingmonth >= '$strprevmonth'
			AND msi_close_off.closingmonth <= '$prev_year'";
		
	$in_borr = sql::sql_run($inv_borr);
	foreach($in_borr as $in_borr){
		$invoiced = $in_borr[0][0];
		$writeoff = $in_borr[0][1];
	}

	$closeresultprev = sql::sql_run($closeoffprev);
	if(is_array($closeresultprev)){
	$witeoffprev = $closeresultprev[0][1];
	$invoicedprev =$closeresultprev[0][0];
	$borrowedprev =$closeresultprev[0][3];
	}
	else {
	$unusedprev = "xxx";
	$exceedprev = "xxx";
	$witeoffprev = "xxx";
	$invoicedprev = "xxx";
	}

	$total = "SELECT date_format(msi_contract.startdate,'%Y-%m-%00'),(ifnull(sum(msi_contract.hours),0) + ifnull(sum(msi_contract.server_hours),0))
	from msi_contract, msi_clients
	where msi_clients.id = msi_contract.client
	and msi_contract.client = \"$id\"
	and (('$fulldateprev' between date_format( msi_contract.startdate,'%Y-%m')
	and date_format( msi_contract.enddate,'%Y-%m')) or
	(msi_contract.terminating='no'and '$fulldate2prev'>=date_format(msi_contract.startdate,'%Y-%m-00'))
	or ('$fulldateprev' between date_format( msi_contract.startdate,'%Y-%m')
	and date_format( msi_contract.terminatingdate,'%Y-%m') and  msi_contract.terminating='yes') )";
	
	$contract = sql::sql_run($total);
	$contract_date = $contract[0][0];
	$contract_hours = $contract[0][1];
	$fulldateprev=$prev_year.'-'.$strprevmonth;
	$fulldate2prev=$prev_year.'-'.$strprevmonth.'-'.'01';
	$num = cal_days_in_month(CAL_GREGORIAN, $strprevmonth, $prev_year); // 31
	$ddate=$prev_year.'-'.$strprevmonth.'-'.$num;
	$date1 = date(strtotime($contract_date));
	$date2 = date(strtotime($ddate));
	$difference = $date2 - $date1;
	$mon = floor($difference / 86400 / 30 )+ 1;
	$months = round($difference / 86400 / 30 );
	$contracttime  = $contract_date ;
	$currentdate = $prev_year.'-'.$strprevmonth.'-'.$num;
	$d1 = strtotime($contracttime );
	$d2 = strtotime($currentdate);
	$min_date = min($d1, $d2);
	$max_date = max($d1, $d2);

	$t = 1;
	while (($min_date = strtotime("+1 MONTH", $min_date)) <=  $max_date) {
	
		$t++;
	}

	$used = "SELECT ifnull(sum(msi_billing.bill_hrs),0)
			FROM msi_billing
			WHERE  msi_billing.date >= '$contract_date'
			AND msi_billing.date <= '$ddate'
			AND msi_billing.type = 'Contract'
			and msi_billing.client = \"$id\"
			group by msi_billing.client";	

	$inv = "SELECT
	sum(msi_close_off.writeoff),sum(msi_close_off.invoiced)
	from msi_close_off
	where msi_close_off.client = \"$id\"
	and msi_close_off.closingmonth  >= '$contract_date'
	AND msi_close_off.closingmonth  <= '$ddate'
	group by msi_close_off.client";

	$invcd= sql::sql_run($inv);
	foreach($invcd as $invcd){
		$off = $invcd[0];
		$invoiced = $invcd[1];
	}

	$usedhrs = sql::sql_run($used);

//$totalhrs = $t*$contract_hours;
$totalhrs = $months*$contract_hours;
$totused = $usedhrs[0][0] - $invoiced;


$effective = $available+$totalhrs-$totused-$off+$borrowedprev  ;

?>
		<div style = "color:red;">
		
		<h3>Previous Month</h3>
		<table>
		<tr><td style = "color:red;text-align: left;"><strong>Hours Unused : </strong></td><td style = "color:red;text-align: left;"><strong><?php echo $unusedprev; ?></strong></td></tr>
		<tr><td style = "color:red;text-align: left;"><strong>Hours Exeeded : </td><td style = "color:red; text-align: left;"><strong><?php echo $exceedprev; ?></td></tr>
		<tr><td style = "color:red;text-align: left;"><strong>Hours Written Off : </td><td style = "color:red;text-align: left;"><strong><?php echo $witeoffprev; ?></td></tr>
		<tr><td style = "color:red;text-align: left;"><strong>Invoiced : </td><td style = "color:red;text-align: left;"><strong><?php echo $invoicedprev; ?></td></tr>
		</table>
		</div>
		
	<?php 	

	//////////////END PREVIOUS MONTH/////////////////////////////////////////////////////

	//////////////CURRENT MONTH//////////////////////////////////////////////////////////
	
	$contract ="SELECT ifnull( msi_contract.hours, 0 ) + ifnull( msi_contract.server_hours, 0 )
	from msi_contract
	where msi_contract.client = \"$id\"
	
	and (('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
	and date_format( msi_contract.enddate,'%Y-%m')) or
	(msi_contract.terminating='no'and '$fulldate2'>=date_format(msi_contract.startdate,'%Y-%m-00'))
	or ('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
	and date_format( msi_contract.terminatingdate,'%Y-%m') and  msi_contract.terminating='yes'))
	";
	$totalhours = sql::sql_run($contract);
	$thours = $totalhours [0][0];
	
	$tot ="Select sum(ifnull(msi_billing.bill_hrs,0))as hours
	from msi_billing
	where msi_billing.client= \"$id\"
	AND month(msi_billing.date) = $month and year(msi_billing.date)= $year
	AND msi_billing.type = \"Contract\"";

    $totalbilling = sql::sql_run($tot);
	$tbilling = $totalbilling [0][0];
    $diff = $thours - $tbilling;
    if($diff >= 0)
	 {
	 	$unused = $diff;
	 	$exceed = 0;
	 	$tatement = $unused." Support hours unused for ".date("F Y", mktime(0, 0, 0, $month,1,$year));
	 }
	 else if($diff < 0)
	 	{
	 		$unused = 0;
	 		$exceed = $diff *(-1);
	 		$tatement =" Support hours exceeded by ".$exceed." for ".date("F Y", mktime(0, 0, 0, $month,1,$year));
	 	}
	 		 	
//get invoiced and written off hours.	
 $closeoff = "Select msi_close_off.invoiced, msi_close_off.writeoff,msi_close_off.closingmonth
		      From msi_close_off
		      Where msi_close_off.client = \"$id\"
              And month(msi_close_off.closingmonth) = \"$month\"
              And year(msi_close_off.closingmonth) = \"$year\"";
 
 $closeresult = sql::sql_run($closeoff);
 if(is_array($closeresult)){
 	$witeoff  =  $closeresult[0][1];
 	$invoiced =  $closeresult[0][0];
 }
 else {
 	$unused =   "xxx";
 	$exceed =   "xxx";
 	$witeoff =  "xxx";
 	$invoiced = "xxx";
 }

	?>
	<div style = "color:red;">
	<h3>Current Month</h3>
	<table>
	<tr><td style = "color:blue;text-align: left;"><strong>Opening Balance :  </strong></td><td style = "color:blue;text-align:left;"><strong><?php echo $effective;?></strong></td></tr>
	<tr><td style = "color:red; text-align: left;"><strong>Hours Unused :     </td><td style = "color:red; text-align:left;"><strong><?php echo $unused;   ?></td></tr>
	<tr><td style = "color:red; text-align: left;"><strong>Hours Exeeded :    </td><td style = "color:red; text-align:left;"><strong><?php echo $exceed;   ?></td></tr>
	<tr><td style = "color:red; text-align: left;"><strong>Hours Written Off :</td><td style = "color:red; text-align:left;"><strong><?php echo $witeoff;  ?></td></tr>
	<tr><td style = "color:red; text-align: left;"><strong>Invoiced :         </td><td style = "color:red; text-align:left;"><strong><?php echo $invoiced; ?></td></tr>
	</table>
	</div>
	
<?php 	
/////////////////////////////////////////END CURRENT MONTH////////////////////////////////////////////////////////////////////////////////	
	
}
 
// Check that the user is logged in, if not send them to the login page
if( !acl::logged_in() || $action == "logout" ) {
  session_destroy(); // just to be sure
  print "you are not logged in or do not have access";
}

//switch ($module) {
 // case "billing":
      if ( $action = "export_client" ){
     // billing::export_client($id, $month, $year);
      export_client($id, $month, $year);
      } 
  //  break;

?>