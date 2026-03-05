
<?php
error_reporting (E_ALL ^ E_NOTICE); 
function sql_run4($query)
{
	
//	$link = mysqli_connect("intranetvm.mindspring.local", "supportnew", "466c8YdaFCwpAWmd","msi");//
 //$link = mysqli_connect("localhost", "admin", "qjXq9sQmRKez98jB","msi");
//$link = mysqli_connect("intranetvm.mindspring.local", "msi_user", "hag!hd3@1zfa","msi");

//        $link = mysqli_connect("devintranet.mindspring.local", "msi_user", "hag!hd3@1zfa","msi");
// $link = mysqli_connect("intranetvm.mindspring.local", "devpc_user", "yougetwhatyougetanyoudontgetupset","msi");		 
 $link = mysqli_connect("intranetvm.mindspring.local", "msi_user", "hag!hd3@1zfa","msi");
 
 
	//or die("Could not connect : " . mysql_error());
	//mysql_select_db("msi") or die("Could not select database");
	
		
if (!$link) {
    echo "Error: Unable to connect to MySQL." . PHP_EOL;
    echo "Debugging errno: " . mysqli_connect_errno() . PHP_EOL;
    echo "Debugging error: " . mysqli_connect_error() . PHP_EOL;
    exit;
}

	$result = mysqli_query($link,$query) or die("Query failed : " . mysqli_error($link));
	$values ;

	if(stristr($query,'INSERT')== false && stristr($query,'DELETE')==false && stristr($query,'UPDATE')== false)
	{
       while ($fields = mysqli_fetch_array($result)){
			$values[] = $fields;
		}
		mysqli_free_result($result);
	}
	mysqli_close($link);
	return $values;
}

function weekly_format_mail4($client){

    $firstday = date('Y-m-01');
	$today = date("Y-m-d");
	$lastday = date("Y-m-t", strtotime($firstday));
	$fulldate2 = date('Y-m-01');
	$fulldate = date('Y-m');

	$nextweek = date('Y-m-d', strtotime($today. ' + 7 days'));

	if($nextweek >= $lastday ){
		$nextweek = $lastday;
	}

	$startTimestamp = strtotime($today);
	$endTimestamp = strtotime($lastday);
	$daysleft = 	($endTimestamp - $startTimestamp)/24/3600;
	$weeksleft = ceil($daysleft/7);

	$get_techies = "Select DISTINCT(msi_contract.primarytech)
			        FROM msi_contract,msi_clients
			        WHERE msi_contract.client = msi_clients.id
			        AND
			        (('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
			        AND
			        date_format( msi_contract.enddate,'%Y-%m'))
			        OR
			        (msi_contract.terminating='no' and '$fulldate2'>=date_format(msi_contract.startdate,'%Y-%m-00'))
			        OR
			        (('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
			        AND
			        date_format( msi_contract.terminatingdate,'%Y-%m'))
			        AND
			        msi_contract.terminating='yes')
					)
			        ";

	$techies = sql_run4($get_techies);
	$headers  = 'MIME-Version: 1.0' . "\r\n";
	$headers .= 'Content-type: text/html; charset=iso-8859-1' . "\r\n";
	$headers .= 'From: Intranet' . "\r\n";
//         $headers .= "CC:renata@mindspring.co.za;zaheerah@mindspring.co.za;anthea@mindspring.co.za;michyle@mindspring.co.za;";
        $headers .= "CC:managers@mindspring.co.za;anthea@mindspring.co.za;eric@mindspring.co.za";

	foreach ($techies as $techies){
		
		$pritechie = $techies["primarytech"];
		
		$message = ' Dear '. ucwords($pritechie)."<br /><br />
		Weekly summary, from <b>".date('01 F')." until ". date('d F Y')."</b><br /><br />
		<table>		

            <tr style ='padding: 3px;background:#3333CC;font-weight: bold;color: white;'>
			<td style='border-left: 1px solid #FFFFFF'> <b> Client </b></td>
			<td style='border-left: 1px solid #FFFFFF'> <b> Contract hours </b></td>
			<td style='border-left: 1px solid #FFFFFF'> <b> Total hours used</b> </td>
			<td style='border-left: 1px solid #FFFFFF'> <b> Effective hours </b></td>
			<td style='border-left: 1px solid #FFFFFF'> <b> Hours to cover weekly </b></td>
			</tr>";

	$statement = "SELECT
			msi_clients.company,
			msi_contract.client,
			ifnull(msi_contract.hours,0),
			msi_contract.startdate,
			msi_contract.enddate,
			ifnull(msi_contract.server_hours,0),
			msi_contract.server_techie,
			ifnull(msi_close_off.borrowed,0),
			ifnull(msi_contract.effective,0)
		
			FROM
			msi_contract,msi_clients,msi_close_off
			WHERE
			msi_clients.id = msi_contract.client
			AND
			(('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
			AND
			date_format( msi_contract.enddate,'%Y-%m'))
			OR
			(msi_contract.terminating='no' and '$fulldate2'>=date_format(msi_contract.startdate,'%Y-%m-00'))
			OR
			(('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
			AND
			date_format( msi_contract.terminatingdate,'%Y-%m'))
			AND
			msi_contract.terminating='yes'))
		
			AND msi_close_off.client = msi_contract.client
			AND msi_close_off.closingmonth <= '$lastday'
			AND msi_close_off.closingmonth >= date_format(msi_contract.startdate,'%Y-%m-00')
			AND msi_contract.hours <= 20
			
			AND msi_contract.primarytech = '$pritechie'
			Group by msi_clients.company
			ORDER BY
			msi_clients.company ASC " ;

	$results = sql_run4($statement);
if(is_array($results)){

		foreach($results as $results){
   
			$time = date('Y-m-d', strtotime('+1 day', strtotime($results[3])));
			$sdate = strtotime($time);
			$edate = strtotime($lastday);
			$servtechie = $results[6];
			$client = $results[0];
			
					
	$t = 1;
			while (($sdate = strtotime("+1 MONTH", $sdate)) <=  $edate) {
				$t++;
					
			}

			$used = sql_run4("SELECT ifnull(sum(msi_billing.bill_hrs),0)
					FROM msi_billing
					WHERE  msi_billing.date >= '$results[3]'
					AND msi_billing.date <= '$lastday'
					AND msi_billing.type = 'Contract'
					and msi_billing.client = \"$results[1]\"
					group by msi_billing.client");
			
			$borrowedhours = "select ifnull(msi_close_off.borrowed,0)as borrowed
             from msi_close_off
             where msi_close_off.client = '".$results[1]."'
             and msi_close_off.closingmonth >= '".$fulldate."'
              ";
			
			$bhours = sql_run4($borrowedhours);
			if(is_array($bhours )){
				foreach($bhours as $bhours){
					$nowborrowed = $bhours[0];
				}
			}
			else {$nowborrowed = 0;}

if(!isset($results[5] )){
	$results5 = 0;
	}
	elseif($results[5]  = ''){
	$results5 = 0;
	}
elseif(empty($results[5])) {
   $results5 = 0;
}	
	
else {$results5 = $results[5];}

			$total_hours = (($results[2] + $results5)* $t)+ $nowborrowed + $results[8] ;
			
			if(is_array($used )){
			foreach($used as $used){
				
				$used = $used[0];
				
			}
		}else {$used =0 ;}
			
			$inv_borr = sql_run4("SELECT
					ifnull(sum(msi_close_off.invoiced),0),
					ifnull(sum(msi_close_off.writeoff),0)
					from msi_close_off
					where msi_close_off.client = \"$results[1]\"
					and msi_close_off.closingmonth >= '$results[3]'
					AND msi_close_off.closingmonth <= '$lastday'
					group by msi_close_off.client");

if(is_array($inv_borr)){
			foreach($inv_borr as $inv_borr){
				$invoiced = $inv_borr[0];
				$writeoff = $inv_borr[1];
			}
			}else {$invoiced = 0;
				$writeoff = 0;}
	if($pritechie == ''){
				$subject = " No Primary Techie assigned for ".$results[0];
			}
			elseif(empty($pritechie)){
		$subject = " No Primary Techie assigned for ".$results[0];
	}
			else {
			$subject = "Weekly Hours Summary for " .ucwords($pritechie). "'s customers";
			}
			$techname = "Select msi_users.mail from msi_users where username = '".$pritechie."'";
		
			$techie = sql_run4($techname);
			if(is_array($techie)){
				foreach ($techie as $techie)	{
					$ptechie = $techie["mail"];
                
				}
			}
			$servname = "Select msi_users.mail from msi_users where username = '".$servtechie."'";
			$stechie = sql_run4($servname);
			if(is_array($stechie)){
				foreach ($stechie as $stechie)	{
					$servertechie = $stechie["mail"];
		
				}
			}

			$total_used = $used - $invoiced;
			
			$effective = ($total_hours - $total_used)- $writeoff;
			
			$hours = "select ifnull(sum(msi_billing.bill_hrs),0)as hours
            from msi_billing
             where msi_billing.client = '".$results[1]."'
             and msi_billing.date >= '".$fulldate2."'
             and msi_billing.type = 'Contract'
              ";
			
			$tothours = sql_run4($hours);
			
			if(is_array($tothours)){
			foreach ($tothours as $thours){
				$tot = $thours[0];
				$hrs = $total_hours;
				$unused = $hrs - $tot;
			
			}
			}
			else {$tot = 0;
						$hrs = 0;
				$unused = 0;
			}
			

			$tocover = $effective/$weeksleft;
			$tocover = sprintf('%0.2f', $tocover);

			$message .= '<tr style="background:lavender;padding:3px;">
					     <td >'.$client.' </td>
					     <td class=\'client\'>'.$results[2].' </td>
					     <td class=\'client\'>'.$tot.' </td>
					     <td class=\'client\'>'.$effective.' </td>
					     <td class=\'client\'>'.$tocover.' </td>
					     </tr>';
		}
		$message .='</table><br /><br />';
		
			if((date('F') == "March")||(date('F') == "April") ){
			$message .='<b>*Please note that this calculation does factor in the 2 public holidays for Easter, which applies to April/March </b>.  <br /><br />';
		    }

		   $message .='<hr style="border-width: 1px 1px 0;
           border-style: solid;
           border-color: #0000ff;
           width: 100%;
           margin-left: auto;
           margin-right: auto;">
		   <table width = "60%" >
           <tr><td><font size = "3" color = "blue">Mindspring Computing<br>
           Unit 5, Melomed Office Park<br>
           Punters Way, Kenilworth.<br>
           Tel: (021) 657 1780<br>
           Fax: (021) 671 7599
           </font></td>
 		   </tr></table>
           ';
		   $to = $ptechie;
//		   $to .= ",dandre@mindspring.co.za";
//		   $to .= ",pramod@mindspring.co.za";
//		   $to .= ",anthea@mindspring.co.za";
//		   $to .= ",celento@mindspring.co.za";
$send  = mail($to, $subject, $message, $headers);
//$send  = mail("dandre@mindspring.co.za", $subject, $message, $headers);
//print $message;		  
	}

	}

}

function daily_format_mail4($client){

	//get public holidays
	$holidays = array(
			strtotime(date("Y-01-01")),
			strtotime(date("Y-01-02")),
			strtotime(date("Y-03-21")),
                        strtotime(date("Y-04-07")),
                        strtotime(date("Y-04-10")),

			strtotime(date("Y-04-27")),
			strtotime(date("Y-05-01")),
			strtotime(date("Y-06-16")),
			strtotime(date("Y-08-09")),
			strtotime(date("Y-09-24")),
			strtotime(date("Y-12-16")),
			strtotime(date("Y-12-26")),
			strtotime(date("Y-12-25"))
				
	);
	 
$firstday = date('Y-m-01');
	$today = date("Y-m-d");
	$lastday = date("Y-m-t", strtotime($firstday));
	$fulldate2 = date('Y-m-01');
	$fulldate = date('Y-m');
	$diff = (strtotime($lastday)- strtotime($firstday))/24/3600;	
$workingDays = 0;
	
	$startTimestamp = strtotime($today);
	$endTimestamp = strtotime($lastday);
	
	for($i=$startTimestamp; $i<=$endTimestamp; $i = $i+(60*60*24) ){
             
             	
     		if(date("N",$i) <= 5){
			if( !in_array($i,$holidays)){
				
				$workingDays = $workingDays + 1;
			}
	
		}
		elseif(date("N",$i) == 7){
			if( in_array($i,$holidays)){

				$workingDays = $workingDays - 1;
			}
		    }
			}
			
			$get_techies = "Select DISTINCT(msi_contract.primarytech)
			FROM msi_contract,msi_clients
			WHERE msi_contract.client = msi_clients.id
			AND
			(('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
			AND
			date_format( msi_contract.enddate,'%Y-%m'))
			OR
			(msi_contract.terminating='no' and '$fulldate2'>=date_format(msi_contract.startdate,'%Y-%m-00'))
			OR
			(('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
			AND
			date_format( msi_contract.terminatingdate,'%Y-%m'))
			AND
			msi_contract.terminating='yes'))
				
			And msi_clients.customer = 1";
			
			
			$techies = sql_run4($get_techies);
			
			$headers  = 'MIME-Version: 1.0' . "\r\n";
			$headers .= 'Content-type: text/html; charset=iso-8859-1' . "\r\n";
			$headers .= 'From: Intranet' . "\r\n";
//			$headers .= "CC:renata@mindspring.co.za;anthea@mindspring.co.za;michyle@mindspring.co.za,imaan@mindspring.co.za";
			$headers .= "CC:managers@mindspring.co.za;anthea@mindspring.co.za;imaan@mindspring.co.za;eric@mindspring.co.za";

			$message = ' Dear '. ucwords($techies[0][0]);
			foreach($techies as $techies){
				$pritechie = $techies[0];
				
				$message = ' Dear '. ucwords($pritechie)."<br /><br />
				Daily summary, from <b>".date('01 F')." until ". date('d F Y')."</b><br /><br />
						
		    <table>		
            <tr style ='padding: 3px;background:#3333CC;font-weight: bold;color: white;'>
			<td style='border-left: 1px solid #FFFFFF'> <b> Client </b></td>
			<td style='border-left: 1px solid #FFFFFF'> <b> Contract hours </b></td>
			<td style='border-left: 1px solid #FFFFFF'> <b> Total hours used</b> </td>
			<td style='border-left: 1px solid #FFFFFF'> <b> Effective hours </b></td>
			<td style='border-left: 1px solid #FFFFFF'> <b> Hours to cover daily </b></td>
			</tr>";
			
$results = sql_run4("SELECT
		msi_clients.company,
		msi_contract.client,
		msi_contract.hours,
		msi_contract.startdate,
		msi_contract.enddate,
		msi_contract.primarytech,
		msi_contract.backuptech,
		msi_contract.server_hours,
		msi_contract.server_techie,
		ifnull(msi_close_off.borrowed,0),
		ifnull(msi_contract.effective,0)
		
		FROM
		msi_contract,msi_clients,msi_close_off
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
		
		AND msi_close_off.client = msi_contract.client
		AND msi_close_off.closingmonth <= '$lastday'
		AND msi_close_off.closingmonth >= date_format(msi_contract.startdate,'%Y-%m-00')
		AND msi_contract.hours > 20
		AND msi_contract.primarytech = '$pritechie'
		And msi_clients.customer = 1
        Group by msi_clients.company
		ORDER BY
		msi_clients.company ASC " );

if(is_array($results)){
foreach($results as $results){
	//calculate total hours
	
	$time = date('Y-m-d', strtotime('+1 day', strtotime($results[3])));
	$sdate = strtotime($time);
	$edate = strtotime($lastday);
	$pritechie = $results[5];
	$servtechie = $results[8];
	$client = $results[0];
	$t = 1;
	while (($sdate = strtotime("+1 MONTH", $sdate)) <=  $edate) {
		$t++;
	}
	$borrowedhours = "select ifnull(msi_close_off.borrowed,0)as borrowed
             from msi_close_off
             where msi_close_off.client = '".$results[1]."'
             and msi_close_off.closingmonth >= '".$fulldate."'
              ";
	
	$bhours = sql_run4("select ifnull(msi_close_off.borrowed,0)as borrowed
             from msi_close_off
             where msi_close_off.client = '".$results[1]."'
             and msi_close_off.closingmonth >= '".$fulldate."'
              ");
	if(is_array($bhours )){
	foreach($bhours as $bhours){
		$nowborrowed = $bhours[0];
		
	}
	}
	else {	$nowborrowed = 0;}

	$used = sql_run4("SELECT ifnull(sum(msi_billing.bill_hrs),0)
	                 FROM msi_billing
	                 WHERE  msi_billing.date >= '$results[3]'
			         AND msi_billing.date <= '$lastday' 
			         AND msi_billing.type = 'Contract'
	                 and msi_billing.client = \"$results[1]\"
	                 group by msi_billing.client");
					 
	if(!isset($results[7] )){
	$results7 = 0;
	}
	elseif($results[7]  = ''){
	$results7 = 0;
	}
elseif(empty($results[7])) {
   $results7 = 0;
}	
	
else {$results7 = $results[7];}
				 
					 

	$total_hours = (($results[2] + $results7)* $t)+ $nowborrowed + $results[10]  ;
	
	foreach($used as $used){
	$used = $used[0];
	}

	$inv_borr = sql_run4("SELECT 
                          ifnull(sum(msi_close_off.invoiced),0),
	                      ifnull(sum(msi_close_off.writeoff),0)
			              from msi_close_off
			              where msi_close_off.client = \"$results[1]\"
			              and msi_close_off.closingmonth >= '$results[3]'
			              AND msi_close_off.closingmonth <= '$lastday'
			group by msi_close_off.client");

	   foreach($inv_borr as $inv_borr){
		$invoiced = $inv_borr[0];
		$writeoff = $inv_borr[1];
	}
	
	if($pritechie == ''){
		$subject = " No Primary Techie assigned for ".$results[0];
	}
	
	elseif(empty($pritechie)){
		$subject = " No Primary Techie assigned for ".$results[0];
	}
	else {
		$subject = "Daily Hours Summary for ".$results[0];
	}
	$techname = "Select msi_users.mail from msi_users where username = '".$pritechie."'";

	$techie = sql_run4($techname);
	if(is_array($techie)){
		foreach ($techie as $techie)	{
		$ptechie = $techie["mail"];

		}
	}
	$servname = "Select msi_users.mail from msi_users where username = '".$servtechie."'";
	$stechie = sql_run4($servname);
	if(is_array($stechie)){
		foreach ($stechie as $stechie)	{
			$servertechie = $stechie["mail"];
		}
	}

   $to = $ptechie;
 //  $to .= ",celento@mindspring.co.za";
  // $to .= ",pramod@mindspring.co.za";
  // $to .= ",dandre@mindspring.co.za";
  // $to .= ",anthea@mindspring.co.za";
	
	$total_used = $used - $invoiced;
	$effective = ($total_hours - $total_used)- $writeoff;
	$contract = 'Contract';
	$hours = "select ifnull(sum(msi_billing.bill_hrs),0)as hours
             from msi_billing
             where msi_billing.client = '".$results[1]."'
             and msi_billing.date >= '".$fulldate2."'
             and msi_billing.type = '".$contract."'
              ";
		
	$tothours = sql_run4($hours);
	foreach ($tothours as $thours){
		$tot = $thours[0];
		$hrs = $total_hours;
		$unused = $hrs - $thours["hours"];	
	}
	
	$tocover = $effective/$workingDays;
	$tocover = sprintf('%0.2f', $tocover);

			$message .= '<tr style="background:lavender;padding:3px;">
					     <td >'.$client.' </td>
					     <td class=\'client\'>'.$results[2].' </td>
					     <td class=\'client\'>'.$tot.' </td>
					     <td class=\'client\'>'.$effective.' </td>
					     <td class=\'client\'>'.$tocover.' </td>
					     </tr>';
			
			
			}
			$message .='</table><br /><br />';

	
	if((date('F') == "March")||(date('F') == "April") ){
		$message .='<b>*Please note that this calculation does factor in the 2 public holidays for Easter, which applies to April/March </b>. <br/><br />';
	}
	$message .='<hr style="border-width: 1px 1px 0;
           border-style: solid;
           border-color: #0000ff;
           width: 100%;
           margin-left: auto;
           margin-right: auto;">
		   <table width = "60%" >
          <tr><td><font size = "3" color = "blue">Mindspring Computing<br>
          Unit 5, Melomed Office Park<br>
          Punters Way, Kenilworth.<br>
          Tel: (021) 657 1780<br>
          Fax: (021) 671 7599
          </font></td>
  		</tr></table>
          ';
//
      $send  = mail($to, $subject, $message, $headers);

//	$send  = mail("dandre@mindspring.co.za", $subject, $message, $headers);

//print $message;	
	
}	
}
}
		  
daily_format_mail4($name["company"]);
weekly_format_mail4($name["company"]);


?>
