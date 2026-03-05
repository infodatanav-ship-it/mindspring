<?php
 
function sql_run($query)
{
 //       $link = mysql_connect("localhost", "msi", "144455caregfdrrra")
 //       or die("Could not connect : " . mysql_error());
 //       mysql_select_db("msi_dev2") or die("Could not select database");
 
//The above is for database connection on a dev environment
 
//The comment below is for database connection on a live environment
 
        $link = mysql_connect("localhost", "admin", "qjXq9sQmRKez98jB")
         or die("Could not connect : " . mysql_error());
         mysql_select_db("msi") or die("Could not select database");
 
        $result = mysql_query($query) or die("Query failed : " . mysql_error());
        $values;
        if(stristr($query,'INSERT')== false && stristr($query,'DELETE')==false && stristr($query,'UPDATE')== false)
        {
               while ($fields = mysql_fetch_array ($result)){
                       $values[] = $fields;
               }
               mysql_free_result($result);
        }
        mysql_close($link);
        return $values;
}
function percent_notify(){
        $FirstDayOfMonth = date('Y-m-01');
        $today = date("Y-m-d");
        $fulldate2 = date('Y-m-01');
        $fulldate = date('Y-m');
        $LastDayOfMonth = date("Y-m-t", strtotime($FirstDayOfMonth));
		
		echo "FirstDayOfMonth ", $FirstDayOfMonth, "\n" ;
		echo "today ", $today, "\n" ;
		echo "fulldate ", $fulldate, "\n" ;
		echo "fulldate2 ", $fulldate2, "\n" ;
		echo "LastDayOfMonth ", $LastDayOfMonth, "\n" ;

        
//uncomment the above date variables to run on current date(or on live environment)
 
//Below the dates are hardcoded for test purposes (to ensure there is data available on dev)
 
//       $FirstDayOfMonth = date('2019-07-01');
//        $today = date("2019-07-25");
//        $LastDayOfMonth = date("2018-11-31", strtotime($FirstDayOfMonth));
//        $fulldate2 = date('2018-11-01');
//        $fulldate = date('2018-11');
 
        $customer = "
        select msi_clients.company,
        msi_clients.firstname,
        msi_clients.email,
        msi_contract.hours,
        msi_contract.server_hours,
        msi_contract.primarytech,
        msi_contract.server_techie,
        msi_contract.acc_manager,
        msi_contract.hours_notify,
        msi_contract.client,
        msi_clients.id,
        msi_clients.lastname
 
        from msi_contract,msi_clients
        where msi_contract.client = msi_clients.id
        and msi_clients.customer = 1
	    and msi_clients.email='jacquesd@cjpchemcpt.co.za'
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
        ";
 
        $results = sql_run($customer);
 
        foreach ($results as $results){
        $tot= $results[3] + $results[4];
        $company = $results[0];
        $clientname = $results[1];
        $clientemail = $results[2];
        $primarytech = $results[5];
        $sitemanager = $results[6];
        $acc_manager = $results[7];
        $hours_notify = $results[8];
        $clientid = $results[9];
        $clientid2 = $results[10];
        $lastname = $results[11];
        $htmlContent = $company."<br /><br />";
 
        $bill = "select
        ifnull(sum(msi_billing.bill_hrs),0),
        (select msi_users.mail
        from msi_users
        where msi_users.username ='$primarytech')as ptechie,
 
        (select msi_users.mail
        from msi_users
        where msi_users.username ='$sitemanager')as site,
 
        (select msi_users.mail
        from msi_users
        where msi_users.username ='$acc_manager')as acc
 
        from msi_billing,msi_clients
        where
        msi_billing.client = msi_clients.id
        and msi_billing.client ='$clientid'
        and msi_clients.company = \"$company\"
        and msi_billing.date between '$FirstDayOfMonth' and '$today'
        and msi_billing.type ='contract'
        
        ";
 
        $results2 = sql_run($bill);
 
        foreach ($results2 as $results2){
        $used =$results2[0];
        $primtechie =$results2[1];
        $sitman=$results2[2];
        $accman=$results2[3];
        
        $tonotify = round((($used/$tot)*100),2);      
        if(($tonotify >= $hours_notify) && ($used > 0)){
        
        $htmlContent = 'Dear '. ucwords($clientname)."<br /><br />
        Please note that you have a <b> $tot hour</b> support contract with us, and have used a total of <b>$used</b> from ".
        date('d F Y', strtotime($FirstDayOfMonth))." until ".date('d F Y', strtotime($today)). ". <br /><br />
                       
        The following options are available once the monthly contracted hours is reached:<br /><br />
                       
        Option 1 - <b>Suspend </b>support for the remainder of the month <br />
    Option 2 - <b>Continue </b>with support for the month <br /><br />
                       
    Please reply with one of the options above.  If we do not hear back from you, we will continue based on Option 2.<br /><br />
 
    If you have any queries, please do not hesitate to contact us.<br /><br />              
 
Kind Regards<br />
Anthea Engelbrecht<br /><br />";
 
 
$to = 'pramod@mindspring.co.za';
//$to = $clientemail; //(client email address commented out while testing)
//when live uncomment $to = $clientemail; 
//and comment $to = sandiswa@mindspring.co.za'  
 
$from = 'anthea@mindspring.co.za';
$fromName = 'Anthea';
$returnpath = "-f" . $from;
$subject = 'Support Contract For '.$company;
$headers = "From: $fromName"." <".$from.">";
$headers  .= 'MIME-Version: 1.0' . "\r\n";
$headers .= 'Content-type: text/html; charset=iso-8859-1' . "\r\n";
 
//$headers .= "CC:$primtechie,$sitman,$accman";
// (primary techie, site manager and account manager email addresses) 
 
//$headers .= "CC:sigasandi@gmail.com,sandiswa@mindspring.co.za";
 
//echo $to."<br />"; //this is printing on screen the email address on screen
$mail = mail($to, $subject, $htmlContent, $headers, $returnpath);
 
//echo $mail?"<h1>Mail sent.</h1>":"<h1>Mail sending failed for . $company .</h1>"; // also printing on screen for testing whether an email was sent or not and for which client
 
 
 
}
}
}
}
percent_notify();

