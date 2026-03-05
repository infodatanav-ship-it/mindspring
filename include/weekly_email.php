<?php
error_reporting (E_ALL ^ E_NOTICE);
function sql_run4($query)
{

	// $link = mysqli_connect("intranetvm.mindspring.local", "supportnew", "466c8YdaFCwpAWmd","msi");//
	// $link = mysqli_connect("localhost", "admin", "qjXq9sQmRKez98jB","msi");
	// $link = mysqli_connect("intranetvm.mindspring.local", "msi_user", "hag!hd3@1zfa","msi");

	// $link = mysqli_connect("devintranet.mindspring.local", "msi_user", "hag!hd3@1zfa","msi");
	// $link = mysqli_connect("intranetvm.mindspring.local", "devpc_user", "yougetwhatyougetanyoudontgetupset","msi");

	$link = mysqli_connect("intranetvm.mindspring.local", "msi_user", "hag!hd3@1zfa","msi");


	if (!$link) {
		echo "Error: Unable to connect to MySQL." . PHP_EOL;
		echo "Debugging errno: " . mysqli_connect_errno() . PHP_EOL;
		echo "Debugging error: " . mysqli_connect_error() . PHP_EOL;
		exit;
	}

	$result = mysqli_query($link,$query) or die("Query failed : " . mysqli_error($link));
	$values = [] ;

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

function additional_emails ($testing) {

	$localTesting = false;

	if ( $testing ) {
		$emails = "pramod@mindspring.co.za;celento@mindspring.co.za";
		var_dump("CC: Emails: " . $emails);
		return $emails;
	} else {
		if ( $localTesting ){
			return "pramod@mindspring.co.za;celento@mindspring.co.za";
		} else {
			$cc_emails_sql = "SELECT * FROM msi_settings where `settingname` = 'weekly_cc_emails';";
			$cc_emails = sql_run4($cc_emails_sql);
			// var_dump($cc_emails[0]['settingvalue']);
			return $cc_emails[0]['settingvalue'];
		}
	}

}

function bccEmailHeader () {
	$bcc_emails_sql = "SELECT * FROM msi_settings where `settingname` = 'weekly_bcc_emails';";
	$bcc_emails = sql_run4($bcc_emails_sql);
	// var_dump($cc_emails[0]['settingvalue']);
	return "Bcc: " . $bcc_emails[0]['settingvalue'] . "\r\n";;
	// return "Bcc: bevan@datanav.co.za\r\n";
}

function weekly_format_mail4($client){

	$testing = false;

	$firstday			= date('Y-m-01');
	$today				= date("Y-m-d");
	$lastday			= date("Y-m-t", strtotime($firstday));

	$fulldate2			= date('Y-m-01');
	$fulldate			= date('Y-m');

	$nextweek	= date('Y-m-d', strtotime($today. ' + 7 days'));

	if($nextweek >= $lastday ){
		$nextweek = $lastday;
	}

	$startTimestamp		= strtotime($today);
	$endTimestamp		= strtotime($lastday);

	$daysleft			= ($endTimestamp - $startTimestamp)/24/3600;
	$weeksleft			= ceil($daysleft/7);

	$get_techies = "SELECT DISTINCT(msi_contract.primarytech)
		FROM msi_contract,msi_clients
		WHERE msi_contract.client = msi_clients.id
		AND
		(('$fulldate' between date_format( msi_contract.startdate,'%Y-%m') AND date_format( msi_contract.enddate,'%Y-%m'))
		OR
		(msi_contract.terminating='no' and '$fulldate2'>=date_format(msi_contract.startdate,'%Y-%m-00'))
		OR
		(('$fulldate' between date_format( msi_contract.startdate,'%Y-%m') AND date_format( msi_contract.terminatingdate,'%Y-%m'))
		AND
		msi_contract.terminating='yes')
		)
	";

	var_dump("Fulldate: " . $fulldate);
	var_dump("Lastday: " . $lastday);
	var_dump("Nextweek: " . $nextweek);

	var_dump($get_techies);
	exit();

	doLogging("CC-String: " . additional_emails($testing));

	$techies = sql_run4($get_techies);
	$headers  = 'MIME-Version: 1.0' . "\r\n";
	$headers .= 'Content-type: text/html; charset=iso-8859-1' . "\r\n";
	$headers .= 'From: Intranet' . "\r\n";
	$headers .= "CC:" . additional_emails($testing) . "\r\n";

	if ( !$testing ) {
		$headers .= bccEmailHeader();
	}

	foreach ($techies as $techie){

		$continueScript = true;

		if ( $techie["primarytech"] === null || strlen($techie["primarytech"]) == 0 ){

			$content = "primarytech is null or empty to send.\n";
			doLogging($content);
			$continueScript = false;

		}

		if ( $continueScript ) {

			var_dump('begin-tech: ' . $techie["primarytech"]);

			$pritechie = $techie["primarytech"];

			if ( $pritechie === null || strlen($pritechie) == 0 ){

				$content = "primarytech is null or empty to send.\n";
				doLogging($content);

			} else {

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
					ORDER BY msi_clients.company ASC " ;

				$resultsX = sql_run4($statement);

				if(is_array($resultsX)){


					foreach($resultsX as $results){

						$time = date('Y-m-d', strtotime('+1 day', strtotime($results[3])));
						$sdate = strtotime($time);
						$edate = strtotime($lastday);
						$servtechie = $results[6];
						$client = $results[0];

						$t = 1;
						while (($sdate = strtotime("+1 MONTH", $sdate)) <=  $edate) {
							$t++;
						}

						$uses = sql_run4("SELECT ifnull(sum(msi_billing.bill_hrs),0)
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
						} else {
							$nowborrowed = 0;
						}

						if ( !isset($results[5]) ){
							$results5 = 0;
						} elseif($results[5] == ''){
							$results5 = 0;
						} elseif(empty($results[5])) {
							$results5 = 0;
						} else {
							$results5 = $results[5];
						}

						$total_hours = (($results[2] + $results5)* $t)+ $nowborrowed + $results[8] ;

						if(is_array($uses )){
							foreach($uses as $used){
								$used = $used[0];
							}
						} else {
							$used = 0;
						}

						$inv_borrs = sql_run4("SELECT
							ifnull(sum(msi_close_off.invoiced),0),
							ifnull(sum(msi_close_off.writeoff),0)
							from msi_close_off
							where msi_close_off.client = \"$results[1]\"
							and msi_close_off.closingmonth >= '$results[3]'
							AND msi_close_off.closingmonth <= '$lastday'
							group by msi_close_off.client");

						if(is_array($inv_borrs)){
							foreach($inv_borrs as $inv_borr){
								$invoiced = $inv_borr[0];
								$writeoff = $inv_borr[1];
							}
						} else {
							$invoiced = 0;
							$writeoff = 0;
						}

						if ( $pritechie == '' ){
							$subject = " No Primary Techie assigned for ".$results[0];
						} elseif ( empty($pritechie) ){
							$subject = " No Primary Techie assigned for ".$results[0];
						} else {
							$subject = " Weekly Hours Summary for " .ucwords($pritechie). "'s customers";
						}

						$techname = "SELECT msi_users.mail from msi_users where username = '".$pritechie."'";

						$techies = sql_run4($techname);

						if(is_array($techies)){
							foreach ($techies as $techie){
								$ptechie = $techie["mail"];
							}
						}

						$servname = "SELECT msi_users.mail from msi_users where username = '".$servtechie."'";
						$stechies = sql_run4($servname);

						if(is_array($stechies)){
							foreach ($stechies as $stechie)	{
								$servertechie = $stechie["mail"];
							}
						}

						$total_used = $used - $invoiced;

						$effective = ($total_hours - $total_used)- $writeoff;

						$hours = "select ifnull(sum(msi_billing.bill_hrs),0) as hours
							from msi_billing
							where msi_billing.client = '".$results[1]."'
							and msi_billing.date >= '".$fulldate2."'
							and msi_billing.type = 'Contract'";

						$tothours = sql_run4($hours);

						if(is_array($tothours)){
							foreach ($tothours as $thours){
								$tot = $thours[0];
								$hrs = $total_hours;
								$unused = $hrs - $tot;
							}
						} else {
							$tot = 0;
							$hrs = 0;
							$unused = 0;
						}

						// hours can't be less than zero
						if ( $effective < 0 ) {
							$effective = 0;
						}

						if ( $weeksleft == 0 ) {
							$tocover = 0.00;
						} else {
							$tocover = $effective/$weeksleft;
							$tocover = sprintf('%0.2f', $tocover);
						}

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
						$message .='<b>*Please note that this calculation does factor in the public holidays for Easter, which applies to April/March </b>.  <br /><br />';
					}

					$message .='<hr style="border-width: 1px 1px 0;
					border-style: solid;
					border-color: #0000ff;
					width: 100%;
					margin-left: auto;
					margin-right: auto;">
					<table width = "60%" >
					<tr><td>
					<font size = "3" color = "blue">
					Mindspring Computing<br>
					Unit 5, Melomed Office Park<br>
					Punters Way, Kenilworth.<br>
					Tel: (021) 657 1780<br>
					Fax: (021) 671 7599
					</font>
					</td>
					</tr></table>';

					$to = '';

					if ( $ptechie === null || strlen($ptechie) == 0 ){

						$content = "Nothing to send. (".$pritechie.")\n";
						doLogging($content);

					} else {

						if ( $testing ) {

							$to = "bevan@datanav.co.za";

							var_dump('toemail: ' . $to);
							var_dump('pritechie: ' . $pritechie);

							echo "[Testing] Email OK to sent (".$to.")...\n";
							doLogging($content);
							mail($to, $subject, $message, $headers);

						} else {

							$to = $ptechie;

							var_dump('toemail: ' . $to);
							var_dump('pritechie: ' . $pritechie);
	
							if ( checkNameAndEmail($pritechie, $to) ) { 
								echo "Email OK to sent...\n";
								$content = "Email OK to sent (".$to.")...\n";
								doLogging($content);
								// $to = 'bevan@datanav.co.za';
								mail($to, $subject, $message, $headers);
							} else {
								echo "Email will not be sent...\n";
								$content = "Email will not be sent (".$to.")...\n";
								doLogging($content);
							}

						}

						var_dump('--------------------------------------------------------------');

					}

				}

			}

		}

	}

}

function checkNameAndEmail($techName, $techEmail) {

	// split a string
	$parts = explode("@", $techEmail);

	$emailName = $parts[0];
	// $emailDomain = $parts[1];

	if ( checkStrings( $emailName , $techName )) {
		// echo "1. Strings match...\n";
		return true;
	} else {
		// echo "2. Strings do not match...\n";
		if ( checkStrings( $techName, $emailName )) {
			// echo "3. Strings match...\n";
			return true;
		} else {
			// echo "4. Strings do not match...\n";
			return false;
		}
	}

}

function checkStrings ( $strA, $strB ) {

	if (strstr($strA, $strB)) {
		// echo "Substring found!\n";
		return true;
	} else {
		// echo "Substring NOT found!\n";
		return false;
	}

}

function doLogging($content) {

	$file = 'logging.txt';
	// $content = "pritechie is null or empty to send.\n";

	// Open the file in write mode ('w' = overwrite, 'a' = append)
	$handle = fopen($file, 'a');

	if ($handle) {
		fwrite($handle, date("Y-m-d H:i:s") ." - " . $content . "\n"); // Write the string
		fclose($handle); // Close the file
	} else {
		echo "Failed to open file!";
	}
}

function daily_format_mail4($client){

	$testing =false;

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

		} elseif ( date("N",$i) == 7){

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
	$headers .= "CC:".additional_emails($testing) . "\r\n";

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

				if ( is_array($bhours )) {
					foreach($bhours as $bhours){
						$nowborrowed = $bhours[0];
					}
				} else {
					$nowborrowed = 0;
				}

				$used = sql_run4("SELECT ifnull(sum(msi_billing.bill_hrs),0)
					FROM msi_billing
					WHERE  msi_billing.date >= '$results[3]'
					AND msi_billing.date <= '$lastday'
					AND msi_billing.type = 'Contract'
					and msi_billing.client = \"$results[1]\"
					group by msi_billing.client");

				if(!isset($results[7] )){
					$results7 = 0;
				} elseif ( $results[7] == '' ) {
					$results7 = 0;
				} elseif ( empty($results[7]) ) {
					$results7 = 0;
				} else {
					$results7 = $results[7];
				}

				$total_hours = (($results[2] + $results7)* $t)+ $nowborrowed + $results[10];

				foreach( $used as $used ){
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

				foreach($inv_borr as $inv_borrx){
					$invoiced = $inv_borrx[0];
					$writeoff = $inv_borrx[1];
				}

				if ( $pritechie == '') {
					$subject = " No Primary Techie assigned for ".$results[0];
				} elseif ( empty($pritechie) ) {
					$subject = " No Primary Techie assigned for ".$results[0];
				} else {
					$subject = "Daily Hours Summary for ".$results[0];
				}

				$techname = "Select msi_users.mail from msi_users where username = '".$pritechie."'";

				$techie = sql_run4($techname);

				if(is_array($techie)){

					foreach ($techie as $techi) {
						$ptechie = $techi["mail"];
					}
				}

				$servname = "Select msi_users.mail from msi_users where username = '".$servtechie."'";

				$stechie = sql_run4($servname);

				var_dump($stechie);

				if(is_array($stechie)){

					foreach ($stechie as $stechi)	{

						$servertechie = $stechi["mail"];
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

				if ( $effective < 0 ) {
					$effective = 0;
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
				$message .='<b>*Please note that this calculation does factor in the public holidays for Easter, which applies to April/March </b>. <br/><br />';
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
			//$send  = mail($to, $subject, $message, $headers);

			//	$send  = mail("dandre@mindspring.co.za", $subject, $message, $headers);
			//print $message;
		}
	}
}

echo 'Script Started\n';

// daily_format_mail4($name["company"]);
weekly_format_mail4($name["company"]);


?>
