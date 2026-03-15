<?php
//Testing git.....2015-11-18 at 01:09
// $Id: contract.php.inc,v 1.4 2008/06/10 09:48:47 shane Exp $

if(!isset($_SESSION)){
session_start();
}
class contract{


 function navigation(){

 print '<div class="sec-links"><ul>
       <li><a href="?module=contract&action=uninvoiced">To Be Invoiced </a></li>
       <li><a href="?module=contract&action=invoice_report">Invoice Admin</a></li>
        <li><a href="?module=contract&action=add_form">Add</a></li>
       <li><a href="?module=contract">Contract List / Monthend</a></li>';
       if(acl::has_access($_SESSION['id'],"contractadmin","read")){
       print' <li><a href="?module=contractadmin&action=terminate">Contract Admin</a></li>';
       print' <li><a href="?module=contract&action=audit">Contract Audit</a></li>';
    }
                print' <li><a href="?module=contract&action=maintenance">Maintenance Contract</a></li>';
print '</ul>
	       </div>';
   
 print'<p>';
}

function maintenance($contract_month,$contract_year){
   
    if(!acl::has_access($_SESSION['id'],"contracts","read")) return 1;
    
    //beginning of new code, 2008/06/06 making contracts month aware.
    if(!isset($contract_month)&&!isset($contract_year))
    {
    
    	$contract_year = date(Y);
    	$contract_month = date(n);
    }
    
    $next_month = $contract_month +1;
    if ($next_month > 12) {
    	$next_month = 1;
    	$next_year = $contract_year +1;
    }
    else {
    	$next_year = $contract_year;
    }
    
    $prev_month = $contract_month - 1;
    if ($prev_month == 0){
    	$prev_month = 12;
    	$prev_year= $contract_year -1;
    }
    else {
    	$prev_year= $contract_year;
    }
    
    //add leading 0 for months less than 10
    if($contract_month < 10){
    	$strmonth = '0'.$contract_month;
    }else{
    	$strmonth = $contract_month;
    }
    //end add leading
    $moncalc=$contract_year.'-'.$strmonth.'-'.'01';
    $fulldate=$contract_year.'-'.$strmonth;
    $fulldate2=$contract_year.'-'.$strmonth.'-'.'00';
    
    $totalhours = sql::sql_run("
    		SELECT
    		SUM(msi_mcontract.hours)+ SUM(msi_mcontract.server_hours), count(*) AS counter
    		FROM
    		msi_mcontract,
    		msi_clients
    		WHERE
    		msi_clients.id = msi_mcontract.client
    		
    		AND
    		(
    		(
    		'$fulldate' between date_format( msi_mcontract.startdate,'%Y-%m')
    		AND date_format( msi_mcontract.enddate,'%Y-%m')
    ) OR (
    		msi_mcontract.terminating='no'
    		AND '$fulldate2'>=date_format(msi_mcontract.startdate,'%Y-%m-00')
    ) OR (
    		(
    		'$fulldate' between date_format( msi_mcontract.startdate,'%Y-%m')
    		AND date_format( msi_mcontract.terminatingdate,'%Y-%m')
    ) AND msi_mcontract.terminating='yes'
    )
    )
    		"
    );
 
    print '<table><tr><td colspan="3" align="left">';
    print "<a href=?module=contract&contract_year=$prev_year&contract_month=$prev_month&action=maintenance style=\"color: #1A53FF\">". date(F, mktime(0, 0, 0, $prev_month, 1, $prev_year)) ." $prev_year << Previous</a> ";
    print "&nbsp&nbsp";
    print " <b><a href=?module=contract&year=$contract_year&month=$contract_month&action=maintenance>". date(F, mktime(0, 0, 0, $contract_month, 1, $contract_year)) ." $contract_year</a></b>";
    print "&nbsp&nbsp";
    print " <a href=?module=contract&contract_year=$next_year&contract_month=$next_month&action=maintenance style=\"color: #1A53FF\">Next >> ". date(F, mktime(0, 0, 0, $next_month, 1, $next_year)) ." $next_year</a><p>";
    print '</td></tr></table>';
  //<tr><td valign=top>';
    
    //ending of new code
    print "<span class=\"head\">Maintenance Contract List </span>";
    print "Total contracted hours: ".$totalhours[0][0]." in ".$totalhours[0][1]." contracts<br>";
    
    $results = sql::sql_run("SELECT
    		msi_clients.company,
    		msi_mcontract.client,
    		msi_mcontract.hours,
    		date_format(msi_mcontract.startdate,'%b-%Y'),
    		date_format(msi_mcontract.enddate,'%b-%Y'),
    		msi_mcontract.contractid,
    		datediff(msi_mcontract.enddate,now()),
    		msi_mcontract.enddate,
    		month(msi_mcontract.enddate),
    		month(now()),
    		msi_mcontract.primarytech,
    		msi_mcontract.backuptech,
    		msi_mcontract.server_hours,
    		msi_mcontract.server_techie,
    		msi_mcontract.terminating,
    		msi_mcontract.type_maintenance,
			msi_mcontract.acc_manager
			FROM
			msi_mcontract,msi_clients
			WHERE
			msi_clients.id = msi_mcontract.client
			AND
			(('$fulldate' between date_format( msi_mcontract.startdate,'%Y-%m')
    		AND
    		date_format( msi_mcontract.enddate,'%Y-%m'))
    		OR
    		(msi_mcontract.terminating='no'and '$fulldate2'>=date_format(msi_mcontract.startdate,'%Y-%m-00'))
    		OR
    		(('$fulldate' between date_format( msi_mcontract.startdate,'%Y-%m')
    		AND
    		date_format( msi_mcontract.terminatingdate,'%Y-%m'))
    		AND
    		msi_mcontract.terminating='yes'))
    		ORDER BY
    		msi_clients.company ASC " );

	print" <p><b>Listing all active maintenance contracts. To edit the contract click on the customer name</b></p>";
	if(!$results){
		print "<font style ='color: red'><b>No contracts found for the selected month...</b></font>" ;
  		}
  		
  		else {
    print "<table><tr><td>";
    print"<table class=\"client\">
            <tr class=\"head\">
  <td>Company</td>
  <td>Type of Maintenance</td>
  <td title =\"Contracted hours to be covered per month\" style=\"border-left: 1px solid #FFFFFF\" align=\"center\">Total Hours</td>";
   print"<td title =\"\" style=\"border-left: 1px solid #FFFFFF\">Server Hours</td>
   <td title =\"\" style=\"border-left: 1px solid #FFFFFF\"> Used Hours </td>
    		<td title =\"\" style=\"border-left: 1px solid #FFFFFF\">Start Date </td>
    		<td title =\"\" style=\"border-left: 1px solid #FFFFFF\">End Date </td>
   		    <td title =\"\" style=\"border-left: 1px solid #FFFFFF\">Status </td>
   	        <td title =\"\" style=\"border-left: 1px solid #FFFFFF\">Primary Techie </td>
    		<td title =\"\" style=\"border-left: 1px solid #FFFFFF\">Backup Techie </td>
    		<td title =\"\" style=\"border-left: 1px solid #FFFFFF\">Site Manager </td>
                <td title =\"\" style=\"border-left: 1px solid #FFFFFF\">Account Manager </td>
    		
  </tr>";
   
  
    
	foreach ($results as  $result){
		$totalServerHours = $result[12];
		$totalSupportHours = $result[2];
		$totalHours = $totalServerHours + $totalSupportHours;
		$state = $result[14];
		if($state == "no"){
			
			$state = "<font style ='color: blue'>Active</font>";
		}
			else {$state = "<font style ='color: red'>Inactive</font>";
			$color = "red";}
		
		print "<tr >
		<td class=\"client\" ><a href=\"?module=contract&action=editmaintenance_form&id=$result[1]&identity=$result[5]&year=$contract_year&month=$contract_month\" style = \"text-decoration : none\">$result[0]</a></td>
		<td class=\"client\" align=\"center\">$result[15]</td>
		<td class=\"client\" align=\"center\">$totalHours</td>
		<td class=\"client\" align=\"center\">$result[12]</td>";
		
		$used = "Select sum(msi_mbilling.bill_hrs), sum(msi_mbilling.nonbill_hrs)
		from msi_mbilling
		where msi_mbilling.client =  \"$result[1]\"
		and month(msi_mbilling.date) = \"$contract_month\"
		and year(msi_mbilling.date) = \"$contract_year\" ";
	
	$uhours = sql::sql_run($used);
	foreach ( $uhours as $uhours){
	$hours = $uhours[0] + $uhours[1];	
	}
		
	print "	
	    <td class=\"client\" align=\"center\">$hours</td>
		<td class=\"client\" align=\"center\">$result[3]</td>
		<td class=\"client\" align=\"center\">$result[4]</td>
		<td class=\"client\" align=\"center\">$state</td>
		<td class=\"client\" align=\"center\">$result[10]</td>
		<td class=\"client\" align=\"center\">$result[11]</td>
		<td class=\"client\" align=\"center\">$result[13]</td>
                <td class=\"client\" align=\"center\">$result[16]</td>
	   </tr >";
	
	}
		print '</table>';	
  		}
}  

            


function editmaintenance_form($id,$identity,$contract_month,$contract_year){

	if(!isset($contract_month)&&!isset($contract_year))
	{
		$contract_year =$_GET['year'];
		$contract_month =$_GET['month'];
	}
	else {
		//$contract_year =$_GET['year'];
		//$contract_month =$_GET['month'];
    }
	$next_month = $contract_month +1;
	if ($next_month > 12) {
		$next_month = 1;
		$next_year = $contract_year +1;
	}
	else {
		$next_year = $contract_year;
	}
	$prev_month = $contract_month - 1;
	if ($prev_month == 0){
		$prev_month = 12;
		$prev_year= $contract_year -1;
	}
	else {
		$prev_year= $contract_year;
	}

	//add leading 0 for months less than 10
	if($contract_month < 10){
		$strmonth = '0'.$contract_month;
	}else{
		$strmonth = $contract_month;
	}

	//end add leading
	//$numdays=date("t", strtotime($contract_year . "-" .$strmonth . "-01"));
	$moncalc=$contract_year.'-'.$strmonth.'-'.'01';
	$fulldate=$contract_year.'-'.$strmonth;
	$fulldate2=$contract_year.'-'.$strmonth.'-'.'00';

	$months = array(1 => 'January', 2 => 'February', 3 => 'March', 4 => 'April', 5 => 'May', 6 => 'June', 7 => 'July', 8 => 'August', 9 => 'September', 10 => 'October', 11 => 'November', 12 => 'December');
	$thisyear 		= date('Y');
	$thismonth 		= date('F');
	$thisday 		= date('j');
	$today 			= $thisday . " " . $thismonth . " " . $thisyear;
	$expirydate 	= strtotime("+ 10 days");
	$expiryyear 	= strftime('%Y',$expirydate);
	$expirymonth 	= strftime('%B',$expirydate);
	$expiryday 		= strftime('%d',$expirydate);

	if(!acl::has_access($_SESSION['id'],"contracts","edit")) return 1;
	
	$results = sql::sql_run("SELECT DISTINCT company, id
    FROM `msi_clients`
    WHERE customer = \"1\"
    GROUP BY `company`");
	
	$results1 = 'SELECT
	msi_clients.company,
	msi_mcontract.client,
	msi_mcontract.hours,
	day(msi_mcontract.startdate),
	monthname(msi_mcontract.startdate),
	month(msi_mcontract.startdate),
	year(msi_mcontract.startdate),
	day(msi_mcontract.enddate),
	month(msi_mcontract.enddate),
	monthname(msi_mcontract.enddate),
	year(msi_mcontract.enddate),
	msi_mcontract.terminating,
	msi_mcontract.terminatingdate,
	month(msi_mcontract.terminatingdate),
	year(msi_mcontract.terminatingdate),
	msi_mcontract.primarytech,
	msi_mcontract.backuptech,
	msi_mcontract.server_hours,
	msi_mcontract.server_techie,
         msi_mcontract.contractid,
        msi_mcontract.type_maintenance,
        msi_mcontract.acc_manager
              			
 FROM
	msi_mcontract,
	msi_clients
 WHERE
   	msi_clients.id = msi_mcontract.client
 AND
	msi_mcontract.client = "'.$id.'"
				AND
	msi_mcontract.contractid = "'.$identity.'"

 and (("'.$fulldate.'" between date_format( msi_mcontract.startdate,"%Y-%m")
 and date_format( msi_mcontract.enddate,"%Y-%m")) or
 (msi_mcontract.terminating="no" and "'.$fulldate2.'">=date_format(msi_mcontract.startdate,"%Y-%m-00"))
 or ("'.$fulldate.'" between date_format( msi_mcontract.startdate,"%Y-%m")
 and date_format( msi_mcontract.terminatingdate,"%Y-%m") and terminating="yes") )
';
	
$results2 = sql::sql_run($results1);
$identity = $results2[0][19];
$results3=sql::sql_run("SELECT username FROM msi_users WHERE activetechie = 'yes' ORDER BY `username`");

if(isset($_POST['update']))	{
contract::maintenance_termination($client,$hours,$identity,$year2,$month2,$year3,$month3,$termmonth,$termyear,$term,$primary,$backup,$server_hours,$server_techie,$m_type);
contract::maintenance(NULL,NULL);

}
else {

print '<table><tr><td colspan="3" align="left">';
print "<a href=?module=contract&action=editmaintenance_form&id=$id&identity=$identity&contract_year=$prev_year&contract_month=$prev_month style=\"color: #1A53FF\">". date(F, mktime(0, 0, 0, $prev_month, 1, $prev_year)) ." $prev_year << Previous</a> ";
print "&nbsp&nbsp";
print " <b><a href=?module=contract&action=editmaintenance_form&id=$id&identity=$identity&contract_year=$contract_year&contract_month=$contract_month >". date(F, mktime(0, 0, 0, $contract_month, 1, $contract_year)) ." $contract_year</a></b>";
print "&nbsp&nbsp";
print " <a href=?module=contract&action=editmaintenance_form&id=$id&identity=$identity&contract_year=$next_year&contract_month=$next_month style=\"color: #1A53FF\">Next >> ". date(F, mktime(0, 0, 0, $next_month, 1, $next_year)) ." $next_year</a><p>";
print '</td></tr>
<tr><td valign=top>';

print "<span class=\"head\">Cancel/Update Maintenance Contract Hours</span></td></tr></table>


<table class=\"box\">
<tr>
<td align=\"right\"><b>Company Name:</b>
</td>
<td>
<form action=\"index.php?module=contract&action=maintenance_termination&id=$id&identity=$identity&contract_year=$contract_year&contract_month=$contract_month\" name=\"editform\" method=\"POST\">
<select name=\"company\" readonly>
<option value=\"".$id."\">".$results2[0][0]."</option>";

foreach ($results as $result) {
	print "<option value=\"$result[1]\">$result[0]</option>";
}
print "
     </select>
     </td>
	</tr>
           		
       <tr>
        <td align=\"right\"><b>Type of Maintenance:</b></td> 		
       <td><input type=\"text\" name=\"m_type\" value=\"".$results2[0][20]."\">  </td>
		</tr>		
		
        <tr>
        <td><b>Contract Hours Per Month:</b></td>
        <td><input name=\"hours\" value=\"".$results2[0][2]."\" size='9px'></td>
 
 </tr>

	<tr>
		<td align=\"right\"><b>Server Hours:</b></td>
		<td><input name=\"server_hours\" value=\"".$results2[0][17]."\" size='9px'></td>
		</tr>
      <tr>
<td align=\"right\"><b>StartDate:</b></td>
        <td>";
    
	print "</select> \n
<select name=\"month2\">";

 foreach ($months as $num => $month_name) {

		print '<option value="' . $num . '"';
		if ($month_name==$results2[0][4]) { print " selected"; }
		print ">" .$month_name. "</option>\n";

}
	print "</select>\n";

    print '<select name="year2">';
	for ($year = date('Y') -3, $max_year = date('Y') + 2; $year < $max_year; $year++) {
		print '<option value="' . $year . '"';
		if ($year==$results2[0][6]) { print " selected"; }
		print ">" . $year. "</option>\n";
	}
	print "</select>\n";

print "</td>";
	print"</tr>
 <tr>
  <td align=\"right\"><b>EndDate:</b></td>
        <td>";
	print "</select> \n";
	print '<select name="month3">';
		if( is_array( $months ) && count( $months ) > 0 ) {
foreach ($months as $num => $month_name) {
		print '<option value="' . $num . '"';
		if ($num==$results2[0][8]) { print " selected"; }
		print ">" . $month_name . "</option>\n";
	}
		}
	print "</select>\n";
	print '<select name="year3">';
	for ($year = date('Y')-3, $max_year = date('Y') + 2; $year < $max_year; $year++) {
		print '<option value="' . $year . '"';
		if ($year==$results2[0][10]) { print " selected"; }
		print ">" . $year . "</option>\n";
}
	print "</select>\n";

   if ($results2[0][11] == "yes") { $read = 'checked'; } else { $read ="";}
   
      print" </td>
      <tr>
        <td align=\"right\"><b>Cancelled:</b></td>
        <td><input type=\"checkbox\" name=\"term\"  value=\"1\" \"$read\" ></td>
	    </tr>
      <tr>
           <td align=\"right\"><b>Date of cancellation:</b></td>";

    print "<td>";
	print "</select> \n";
	print '<select name="termmonth">';
if( is_array( $months ) && count( $months ) > 0 ) {
foreach ($months as $num => $month_name) {
		print '<option value="' . $num . '"';
		if ($num==$results2[0][13]) { print " selected"; }
		print ">" . $month_name . "</option>\n";
	}
}
	print "</select>\n";
	print '<select name="termyear">';
	for ($year = date('Y')-1, $max_year = date('Y') + 2; $year < $max_year; $year++) {
		print '<option value="' . $year . '"';
		if ($year==$results2[0][14]) { print " selected"; }
       	print ">" . $year . "</option>\n";
	}
	print "</select>\n";

      print" </td>
      
	</tr>
      <tr>
	<td align=\"right\"><b>Primary Technician:</b> </td>
	<td colspan=\"2\" align=\"left\"><select name=\"primary\">";
		   print "<option value=''></option>";
foreach ($results3 as $result3) {
                   print "<option value=\"$result3[0]\""; 
			if ($result3[0]==$results2[0][15]) { print " selected"; } 
		   print ">$result3[0] </option>";
                 };

print "	</select></td>
      </tr>
      <tr>
	<td align=\"right\"><b>Backup Technician:</b> </td>
	<td colspan=\"2\" align=\"left\"><select name=\"backup\">";
	 print "<option value=''></option>";
foreach ($results3 as $result3) {
                   print "<option value=\"$result3[0]\""; 
			if ($result3[0]==$results2[0][16]) { print " selected"; } 
		   print ">$result3[0] </option>";
                 };

print "	</select></td>
      </tr>
		<tr><td align=\"right\"><b>Site Manager:</b></td>
                 	<td><select name=\"server_techie\"><option value=''></option>";
                    foreach ($results3 as $result) {
                   	echo "<option value=\"$result[0]\"";
						if ($result[0] == $results2[0][18])
							echo ' selected="yes"';
						print ">$result[0]</option>";
                 			};
      					print "</select></td></tr> 

 <tr><td align=\"right\"><b>Account Manager:</b></td>
                 	<td><select name=\"acc_manager\"><option value=''></option>";
                    foreach ($results3 as $result) {
                    	echo $result[0]." ";
                   	echo "<option value=\"$result[0]\"";
						if ($result[0] == $results2[0][21])
							echo ' selected="yes"';
						print ">$result[0]</option>";
                 			};
      					print "</select></td></tr>

";
		
print " <tr><td colspan=\"2\" align=\"center\">
              <input type=\"submit\" value=\"Update\" name=\"update\" class=\"buttons\">
              <input type=\"submit\" value=\"Delete\" name=\"Delete\" onclick=\"return confirm('Are you sure you want to delete this item?')\" class=\"delete\">
              <input type=\"button\" onClick=\"javascript:history.go(-1)\"  value=\"Cancel\" class=\"buttons\">
             </td></tr>";
print '</form></td></tr></table>';

   }
 }


///////////////////////////////////////////////////////////////////////////////////////////////////

function add($client,$hours,$year2,$month2,$year3,$month3){
  if(!acl::has_access($_SESSION['id'],"contracts","add")) return 1;

  $primary=$_POST['primary'];
  $backup=$_POST['backup'];
  $server_hours=$_POST['server_hours'];
  
  if ($server_hours==''){
	  $server_hours=0;
  }
   if ($server_hours==null){
	  $server_hours=0;
  }
  $server_techie=$_POST['server_techie'];
  $acc_manager=$_POST['acc_manager'];
  $m_type = trim($_POST['m_type']);
  /*
   * Sandiswa disabled email function for next version    
  $email1=$_POST['email1'];
  $email2=$_POST['email2'];
  $email3=$_POST['email3'];
  */
 // $notify = $_POST['notify'];
  //if ($notify ==''){
 // $notify = '0000-00-00';}
 // echo  'Notify'.$notify;
  $notification = $_POST['notification'];
  
  $subtadd = $_POST['subtadd'];
  $add = $_POST['adddeduct'];
  if(isset($_POST['m_hours']))
  {
  	 $maintenance ='1';
  }
  else
  {
  	 $maintenance ='0';
  }

  $numdays=date("t", strtotime($year3 . "-" . $month3 . "-01"));
  if($month3<10)
  {
  $endcdate=$year3.'-'.'0'.$month3.'-'.'00';
  }
  else
  {
   $endcdate=$year3.'-'.$month3.'-'.'00';
  }
  if($month2<10)
  {

$startcdate=$year2.'-'.'0'.$month2.'-'.'00';
  }
  else
  {
   $startcdate=$year2.'-'.$month2.'-'.'00';
  }

  $months=explode("-",$startcdate);
  $months2=explode("-",$endcdate);
  $year=date("Y");

  if($startcdate!='')
{
  $months=explode("-",$startcdate);
  $months2=explode("-",$endcdate);
  $year=date("Y");
 // if($months[0]<$year|| $months2[0]<$year) {
 //   echo '<font color="red"><b>ERROR: You are not allowed to input previous years contracts</b></font>';
 //     return false;

 //   }
}

 /*
   * Sandiswa disabled email function for next version    
  
  $regex = '/^[_a-z0-9-]+(\.[_a-z0-9-]+)*@[a-z0-9-]+(\.[a-z0-9-]+)*(\.[a-z]{2,3})$/'; 
// Run the preg_match() function on regex against the email address
if (preg_match($regex, $email1)) {
     echo $email1 . ' is a valid email. We can accept it.';
} else { 
     echo $email1 . 'is an invalid email. Please try again.';
} 
if (preg_match($regex, $email2)) {
     echo $email2 . ' is a valid email. We can accept it.';
} else { 
     echo $email2 . 'is an invalid email. Please try again.';
} 
 if (preg_match($regex, $email3)) {
    // echo $email3 . ' is a valid email. We can accept it.';
} else { 
     echo $email3 . 'is an invalid email. Please try again.';
} 
*/  //  if(!preg_match("/^[0-9]+(.[0-9]{2})?$/", $carried)) {
  //if(!preg_match('/^[0-9]+$/',$carried)) {
      // echo '<font color="red"><b>ERROR add: Carried over hours are invalid</b></font>';
    //  return false;
  //  }
    
//if(!preg_match("/^[0-9]+(.[0-9]{2})?$/", $borrowed)) {
  //if(!preg_match('/^[0-9]+$/',$carried)) {
    // echo '<font color="red"><b>ERROR add: Borrowed hours are invalid</b></font>';
     // return false;
   // }
    
     /*if($hours<1) {
       // echo '<font color="red"><b>ERROR:The number of hours per month are Invalid</b></font>';
      return false;
}*/

if($startcdate=='' || $endcdate=='') {
      echo '<font color="red"><b>ERROR: Start date or end date cant be null</b></font>';
      return false;
    }
    if($hours>720) {
      echo '<font color="red"><b>ERROR:The number of hours per month are Invalid</b></font>';
      return false;
      }
    if($client=='') {
      echo '<font color="red"><b>ERROR: Please select a company</b></font>';
      return false;
      }
      if($client=='') {
      	echo '<font color="red"><b>ERROR: Please select a company</b></font>';
            return false;
            }
 // if(!preg_match('/^[0-9]+$/',$hours)) {
  	if(!preg_match('/[0-9.]+$/',$hours)) {
     echo '<font color="red"><b>ERROR: Contract hours are invalid</b></font>';
     // echo $hours."<br />";
    //  echo preg_match('/^[0-9]+$/',$hours)."<br />";
    return false;
     
  }
 
 
    if($startcdate>$endcdate) {
      echo '<font color="red"><b>ERROR: Start date cant be greater than end date</b></font>';
      return false;
     }
    $currentdate=date("Y-m-d");
    if($startcdate==''||$endcdate=='') {
     echo '<font color="red"><b>ERROR:Neither Start date nor End date can be null</b></font>';
      return false;
      }

if($maintenance == 0)
{      
   //Add customer to normal contract   

      $result_check = sql::sql_run("select * from msi_contract where client = ".$client." and ('$startcdate' between startdate and enddate or '$endcdate' between startdate and enddate or ('$startcdate'<startdate and '$endcdate.'>enddate))");

if(!$result_check){
	

 $add = $_POST['adddeduct'];
   $subtadd = $_POST['subtadd'];

	if($subtadd == 'Deduct'){
		$toadd = 0 -  $add;  
          }

   else{ $toadd = $add;}
 //  echo $_POST['subtadd']. " : ".$toadd."<br />";
	//  echo  $toadd."<br />";
//
//echo "INSERT INTO msi_contract(client,hours,month,year,startdate,enddate,terminating, primarytech, backuptech, server_hours, server_techie) VALUES($client, $hours, substr('$startcdate' from 6 for 2), substr('$startcdate' from 1 for 4),'$startcdate','$endcdate','no', '$primary', '$backup', '$server_hours', '$server_techie')";


$ret_result = sql::sql_run("
INSERT INTO msi_contract(
client,hours,month,year,startdate,enddate,terminating, primarytech, backuptech, server_hours, server_techie,weekly_notify,email1,email2,email3,effective,notes,acc_manager,hours_notify,notified)
VALUES($client, $hours, substr('$startcdate' from 6 for 2), substr('$startcdate' from 1 for 4),'$startcdate','$endcdate','no', '$primary', '$backup', '$server_hours', '$server_techie',null,'$email1','$email2','$email3',$toadd,null,'$acc_manager',$notification,'0000-00-00')");

$terminating = explode("-",$endcdate);
$term = $terminating[0]."-".$terminating[1]."-01";
$terminate = date('t',strtotime($term));  
$terminatingdate = $terminating[0]."-".$terminating[1]."-".$terminate;

$statement = "INSERT INTO msi_auditcontract(id,client,hours,startdate,enddate,terminating,terminatingdate,primarytech,backuptech,server_hours,server_techie,effective,acc_manager)
VALUES  (null,$client,$hours,'$startcdate','$endcdate','no','$terminatingdate', '$primary', '$backup', '$server_hours', '$server_techie',$toadd,'$acc_manager')";

//echo $statement."<br />";
$cancell =  sql::sql_run($statement);

}else{
print("This is already a contract client, please use the edit function next to the client name...");
exit;
}
$getclientname=sql::sql_run("select company from msi_clients where id='$client'");
$clientname=$getclientname[0][0];

if(!isset($ret_result)){
    print "<b>added contract hours for $clientname</b><p>";

    contract::listall(NULL,NULL);
    } else {
      print "<b>Something went wrong !</b>";
    }   
}
else {
		//Add customer to maintenance contract
	
$result_check = sql::sql_run("select * from msi_mcontract where client = ".$client." and type_maintenance = '".$m_type."' and ('$startcdate' between startdate and enddate or '$endcdate' between startdate and enddate or ('$startcdate' < startdate and '$endcdate' > enddate))");
	
		if(!$result_check){
	   //  $add = $_POST['adddeduct'];
		//	$subtadd = $_POST['subtadd'];
	
		//	if($subtadd == 'Deduct'){
		//		$toadd = 0 -  $add;
		//	}
	
		//	else{ $toadd = $add;}
			
			$terminating = explode("-",$endcdate);
			$term = $terminating[0]."-".$terminating[1]."-01";
			$terminate = date('t',strtotime($term));
			$terminatingdate = $terminating[0]."-".$terminating[1]."-".$terminate;

			//  echo $_POST['subtadd']. " : ".$toadd."<br />";
			//  echo  $toadd."<br />";
			//echo "INSERT INTO msi_contract(client,hours,month,year,startdate,enddate,terminating, primarytech, backuptech, server_hours, server_techie) VALUES($client, $hours, substr('$startcdate' from 6 for 2), substr('$startcdate' from 1 for 4),'$startcdate','$endcdate','no', '$primary', '$backup', '$server_hours', '$server_techie')";
			$ret_result = sql::sql_run("
					INSERT INTO msi_mcontract(
					contractid,
					client,
					hours,
					month,
					year,
					startdate,
					enddate,
					terminating,
					terminatingdate,
					primarytech,
					backuptech, 
					server_hours,
					server_techie,
					type_maintenance,
                                        acc_manager)
					VALUES(null,
					$client,
					$hours,
					substr('$startcdate' from 6 for 2),
					substr('$startcdate' from 1 for 4),
					'$startcdate',
					'$endcdate',
					'no',
					'$terminatingdate',
					'$primary',
					'$backup',
					'$server_hours',
					'$server_techie',
					'$m_type',
                                        '$acc_manager'
					)");
	
		//	$terminating = explode("-",$endcdate);
		//	$term = $terminating[0]."-".$terminating[1]."-01";
		//	$terminate = date('t',strtotime($term));
			//$terminatingdate = $terminating[0]."-".$terminating[1]."-".$terminate;
	
			//$statement = "INSERT INTO msi_auditcontract(id,client,hours,startdate,enddate,terminating,terminatingdate,primarytech,backuptech,server_hours,server_techie,effective)
			//VALUES  (null,$client,$hours,'$startcdate','$endcdate','no','$terminatingdate', '$primary', '$backup', '$server_hours', '$server_techie',$toadd)";
	
			///echo $statement."<br />";
			//$cancell =  sql::sql_run($statement);
	
		}else{
			print("<b>This is already a maintenance contract client, please use the edit function next to the client name...</b>");
			exit(contract::maintenance(NULL,NULL));
		}
		$getclientname=sql::sql_run("select company from msi_clients where id='$client'");
		$clientname=$getclientname[0][0];

		if(!isset($ret_result)){
			print "<b>added maintenance contract hours for $clientname</b><p>";
		
			contract::maintenance(NULL,NULL);
		} else {
			print "<b>Something went wrong !</b>";
		} 
	}
}	


function termination($client,$hours,$identity,$year2,$month2,$year3,$month3,$termmonth,$termyear,$term,$primary,$backup,$server_hours,$server_techie){
if(!acl::has_access($_SESSION['id'],"contracts","edit")) return 1;

$primary=$_POST['primary'];
$backup=$_POST['backup'];
$server_hours=$_POST['server_hours'];
 if($server_hours == ''){
 $server_hours = 0;
}
  if($server_hours == NULL) {
 
	  $server_hours = 0;
  }
$server_techie=$_POST['server_techie'];
$hours = $_POST['hours'];
$client = $_POST['company'];
$termyear = $_POST['termyear'];
$termmonth = $_POST['termmonth'];
$year2 = $_POST['year2'];
$year3 = $_POST['year3'];
$month2 = $_POST['month2'];
$month3 = $_POST['month3'];
$identity = $_POST['identity'];
$email1 = $_POST['email1'];
$email2 = $_POST['email2'];
$email3 = $_POST['email3'];
$term =$_POST['term'];
$effective = $_POST['ehours'];
$deduct = $_POST['deduct'];
$notes = $_POST['notes'];
$acc_manager = $_POST['acc_manager'];
$notification = $_POST['notification'];

if($term=='')
{
    $terminate='no';
}
if($term=='1')
{
  $terminate='yes';
}

$numdays=date("t", strtotime($year3 . "-" . $month3 . "-01"));
 // $endcdate=$year3.'-'.'0'.$month3.'-'.$numdays;
  //$startcdate=$year2.'-'.'0'.$month2.'-'.'01';
  $termday=date("t", strtotime($termyear . "-" . $termmonth . "-01"));
  $termdate=$termyear.'-'.'0'.$termmonth.'-'.$termday;

  if($month3<10)
  {
  $endcdate=$year3.'-'.'0'.$month3.'-'.'00';
  }
  else
  {
   $endcdate=$year3.'-'.$month3.'-'.'00';
  }
  if($month2<10)
  {
$startcdate=$year2.'-'.'0'.$month2.'-'.'00';
  }
  else
  {
   $startcdate=$year2.'-'.$month2.'-'.'00';
  }
  if($termmonth<10)
  {
$termdate=$termyear.'-'.'0'.$termmonth.'-'.'00';
  }
  else
  {
   $termdate=$termyear.'-'.$termmonth.'-'.'00';
  }
  if($term=='')
{
    $terminate='no';
    $termdate=null;
}
//if($term=='1')
//{
 // $terminate='yes';
//}
//$startcdate=$year2.'-'.$month2.'-'.'01';
  $months=explode("-",$startcdate);
  $months2=explode("-",$endcdate);
/*
  if($startcdate!='')
{
  $months=explode("-",$startcdate);
  $months2=explode("-",$endcdate);
  $year=date("Y");
  $currentdate=date("Y-m-d");
 
  if($months[0]<$year|| $months2[0]<$year) {
      echo '<font color="red"><b>ERROR: You are not allowed to input last years contracts</b></font>';
      return false;
    }
}
*/

if($startcdate=='' || $endcdate=='') {
     echo '<font color="red"><b>ERROR: Start date or end date cant be null</b></font>';
      return false;
    }
  
if(!preg_match("/^[0-9]+(.[0-9]{2})?$/", $hours)) {
echo '<font color="red"><b>ERROR: Contract hours is not valid</b></font>';
 return false;
   }
 if(preg_match("/^[0-9]+(.[0-9]{2})?$/", $carried)) {
//if(!preg_match('/^[0-9]+$/',$carried)) {
      echo '<font color="red"><b>ERROR: Carried over hours is not valid</b></font>';
      return false;
      }
 if(preg_match("/^[0-9]+(.[0-9]{2})?$/", $borrowed)) {
 //if(!preg_match('/^[0-9]+$/',$carried)) {
      echo '<font color="red"><b>ERROR: Borrowed hours is not valid</b></font>';
      return false;
    }
/* Sandiswa disabled email function for next version    
 * 
 *  $regex = '/^[_a-z0-9-]+(\.[_a-z0-9-]+)*@[a-z0-9-]+(\.[a-z0-9-]+)*(\.[a-z]{2,3})$/'; 
// Run the preg_match() function on regex against the email address
if (!preg_match($regex, $email1)) {
//echo $email1 . 'is an invalid email. Please try again.';
} 
  
if (!preg_match($regex, $email2)) {
    
 //    echo $email2 . 'is an invalid email. Please try again.';
} 
if (!preg_match($regex, $email3)) {
    
  //   echo $email3 . 'is an invalid email. Please try again.';
} */
     if($hours<1) {
      echo '<font color="red"><b>ERROR:The number of hours per month are Invalid</b></font>';
      return false;
       }
    if($hours>720) {
      echo '<font color="red"><b>ERROR:The number of hours per month are Invalid</b></font>';
      return false;

      }
      
     if($termdate<$startcdate and $term=='1') {
       echo '<font color="red"><b>ERROR:Termination date cant be less than the contract start date</b></font>';
         return false;
      }

   /*// if($startcdate>$endcdate) {
   //   echo 'font color="red"><b>ERROR: Start date cant be greater than end date</b></font>';
   //   return false;
       }
 //   if($termdate<$endcdate and $term=='1') {
 //     echo '<font color="red"><b>ERROR:Termination date cant be less than the contract end date</b></font>';
  //    return false;
  //    }
    $currentdate=date("Y-m-d");
    if($startcdate<$currentdate) {
      echo'<font color="red"><b>ERROR: Start date cant be less than todays date</b></font>';
      return false;
}
    */
 if($startcdate==''||$endcdate=='') {
      echo' <font color="red"><b>ERROR:Neither Start date nor End date can be null</b></font>';
      return false;
    }
        
//echo "<br>"."select $client,contid,startdate,enddate from msi_contract where client = $client and ('$startcdate' between startdate and enddate or '$endcdate' between startdate and enddate or ('$startcdate'<startdate and '$endcdate.'>enddate)) and contid !=".$identity."";
//$ret_existscnt = sql::sql_run("select $client,contid,startdate,enddate from msi_contract where client = $client and ('$startcdate' between startdate and enddate or '$endcdate' between startdate and enddate or ('$startcdate'<startdate and '$endcdate.'>enddate)) and contid !=".$identity."");
//echo $ret_existscnt ."<br >";
//if(!$ret_existscnt)
//{
 //print"There is already a contract for the same client on the following date from ".$ret_existscnt[0][2].'  '.'to'.' '.$ret_existscnt[0][3];
//exit;
//}
  $termdate=$termyear.'-'.'0'.$termmonth.'-'.$termday;
//echo "Termdate".$termdate."<br>";

$ret_exists = sql::sql_run("select $client from msi_contract where client = $client and contid='$identity'");
//echo $ret_exists."<br >";
if(!$ret_exists){
print("This is not a contract client please add the client first...");
exit;
}else{
//echo "update msi_contract set client = $client, hours = $hours,startdate='$startcdate',enddate='$endcdate',terminatingdate='$termdate',terminating='$terminate',primarytech='$primary', backuptech='$backup', server_hours = '$server_hours', server_techie = '$server_techie' where client = $client and contid='$identity'
// ";
$ret_update = sql::sql_run("update msi_contract set client = $client, hours = $hours,startdate='$startcdate',
enddate='$endcdate',terminatingdate='$termdate',terminating='$terminate',primarytech='$primary', 
backuptech='$backup', server_hours = '$server_hours', server_techie = '$server_techie',effective ='$deduct',
notes='$notes',acc_manager = '$acc_manager',hours_notify = '$notification' where client = $client and contid='$identity'
 ");

$terminatings = explode("-",$endcdate);
$terms = $terminatings[0]."-".$terminatings[1]."-01";
$terminates = date('t',strtotime($terms));
$terminatingdate = $terminatings[0]."-".$terminating[1]."-".$terminates;
$today = date("Y-m-d");

if($terminate == 'yes'){
$statement = "INSERT INTO msi_auditcontract(id,client,hours,startdate,enddate,terminating,terminatingdate,primarytech,backuptech,server_hours,server_techie,effective,acc_manager)
VALUES  (null,$client,$hours,'$startcdate','$endcdate','$terminate','$today', '$primary', '$backup', $server_hours, '$server_techie',$effective,'$acc_manager')";
}
else 

{
$statement = "update msi_auditcontract,msi_contract set
 msi_auditcontract.client = $client,
 msi_auditcontract.hours = $hours,
 msi_auditcontract.startdate='$startcdate',
 msi_auditcontract.enddate='$endcdate',
 msi_auditcontract.terminatingdate='$termdate',
 msi_auditcontract.terminating='$terminate',
 msi_auditcontract.primarytech='$primary',
 msi_auditcontract.backuptech='$backup', 
 msi_auditcontract.server_hours = $server_hours,
 msi_auditcontract.server_techie = '$server_techie',
 msi_contract.effective = $deduct,
 msi_auditcontract.acc_manager = '$acc_manager'
 where msi_auditcontract.client = $client
 and msi_contract.contid = $identity";

}

$run = sql::sql_run($statement);

}

$getclientname=sql::sql_run("select company from msi_clients where id='$client'");
$clientname=$getclientname[0][0];
if(!isset($ret_update)){
    print "<b>Updated details for $clientname</b><p>";
  //  contract::terminate(NULL,NULL);
    } else {
    
      print "<b>Something went wrong !</b>";
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////

	
function terminatingedit_form($id,$identity,$contract_month,$contract_year){

	var_dump("terminating edit form");

	if(!isset($contract_month)&&!isset($contract_year)) {
		$contract_year =$_GET['year'];
		$contract_month =$_GET['month'];
	}

	$next_month = $contract_month +1;
	if ($next_month > 12) {
		$next_month = 1;
		$next_year = $contract_year +1;
	} else {
		$next_year = $contract_year;
	}

	$prev_month = $contract_month - 1;

	if ($prev_month == 0){
		$prev_month = 12;
		$prev_year= $contract_year -1;
	} else {
		$prev_year= $contract_year;
	}

	//add leading 0 for months less than 10
	if($contract_month < 10){
		$strmonth = '0'.$contract_month;
	}else{
		$strmonth = $contract_month;
	}

	//end add leading
	//$numdays=date("t", strtotime($contract_year . "-" .$strmonth . "-01"));
	$moncalc=$contract_year.'-'.$strmonth.'-'.'01';
	$fulldate=$contract_year.'-'.$strmonth;
	$fulldate2=$contract_year.'-'.$strmonth.'-'.'00';

	$months = array(1 => 'January', 2 => 'February', 3 => 'March', 4 => 'April', 5 => 'May', 6 => 'June', 7 => 'July', 8 => 'August', 9 => 'September', 10 => 'October', 11 => 'November', 12 => 'December');
	$thisyear 		= date('Y');
	$thismonth 		= date('F');
	$thisday 		= date('j');
	$today 			= $thisday . " " . $thismonth . " " . $thisyear;
	$expirydate 	= strtotime("+ 10 days");
	$expiryyear 	= strftime('%Y',$expirydate);
	$expirymonth 	= strftime('%B',$expirydate);
	$expiryday 		= strftime('%d',$expirydate);

	if(!acl::has_access($_SESSION['id'],"contracts","edit")) return 1;

	$results = sql::sql_run("SELECT DISTINCT company, id
	FROM `msi_clients`
	WHERE customer = \"1\"
	GROUP BY `company`");

	var_dump($results);

	$results2 = sql::sql_run('SELECT
		msi_clients.company,
		msi_contract.client,
		msi_contract.hours,
		day(msi_contract.startdate),
		monthname(msi_contract.startdate),
		month(msi_contract.startdate),
		year(msi_contract.startdate),
		day(msi_contract.enddate),
		month(msi_contract.enddate),
		monthname(msi_contract.enddate),
		year(msi_contract.enddate),
		msi_contract.terminating,
		msi_contract.terminatingdate,
		month(msi_contract.terminatingdate),
		year(msi_contract.terminatingdate),
		msi_contract.primarytech,
		msi_contract.backuptech,
		msi_contract.server_hours,
		msi_contract.server_techie,
		ifnull(msi_contract.effective,0),
		msi_contract.contid,
		msi_contract.notes,
			msi_contract.acc_manager,
		msi_contract.hours_notify
		
	FROM 
		msi_contract, 
		msi_clients 
	WHERE
		msi_clients.id = msi_contract.client
	AND
		msi_contract.client = "'.$id.'" 

	and (("'.$fulldate.'" between date_format( msi_contract.startdate,"%Y-%m")
	and date_format( msi_contract.enddate,"%Y-%m")) or
	(msi_contract.terminating="no" and "'.$fulldate2.'">=date_format(msi_contract.startdate,"%Y-%m-00"))
	or ("'.$fulldate.'" between date_format( msi_contract.startdate,"%Y-%m")
	and date_format( msi_contract.terminatingdate,"%Y-%m") and terminating="yes") )
	');

	var_dump($results2);

	$av = "SELECT ifnull(msi_close_off.available,0.00),ifnull(msi_close_off.unused,0.00)
	from msi_close_off
	where msi_close_off.client = \"$id\"
	and  month(msi_close_off.closingmonth) = \"$prev_month\"
	and year(msi_close_off.closingmonth)= \"$prev_year\"";

	$avres = sql::sql_run($av);

	// and  msi_contract.contid = "'.$identity.'"
	$identity = $results2[0][20];
	//echo $identity;


	$mm = $results2[0][5];
	if($mm<10){
		$mm = '0'.$mm;
	} else {
		$mm =$mm;
	}

	$strtdate = $results2[0][6]."-".$mm;

	if ($strtdate == $fulldate){
		//echo "start of contract : ".$avail
		$avail =  $results2[0][19];
		$prevunused = 0;
	} else {

		if(is_array($avres)) {
			$avail = $avres[0][0];
			$prevunused = $avres[0][1];
		} else {
			$prevunused = 0;
			$avail = 0;
		}
	}

	if ($avail < 0){
		$avail = 0;
	}

	$results4 = sql::sql_run("select ifnull(msi_close_off.carried,0),
		ifnull(msi_close_off.borrowed,0),
		ifnull(msi_close_off.extra,0),
		ifnull(msi_close_off.invoiced,0),
		sum(msi_billing.bill_hrs),
		msi_contract.hours,
		msi_contract.server_hours
		from msi_billing,msi_contract,msi_close_off
		where msi_billing.client = msi_contract.client
		and msi_close_off.client = msi_billing.client
		and  month(msi_close_off.closingmonth) = $contract_month 
		and year(msi_close_off.closingmonth)= $contract_year
		AND msi_billing.client = \"$id\"

		and (('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
		and date_format( msi_contract.enddate,'%Y-%m')) or
		(msi_contract.terminating='no'and '$fulldate2'>=date_format(msi_contract.startdate,'%Y-%m-00'))
		or ('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
		and date_format( msi_contract.terminatingdate,'%Y-%m') and terminating='yes') )
		and upper(msi_billing.type) = 'CONTRACT' 
		");

$available = $results2[0][19];
//echo $available."<br />";

$borrowed = $results4[0][1];
$carriedover = $results4[0][5] + $results4[0][6] - $results4[0][4];
if($carriedover < 0){
$carriedover = 0;}
$m = $contract_month-1;
if($m<10){$m = '0'.$m;}
else {$m =$m;}
if($m == '0'){ $m = 12;}
if($m == '12'){ $cyear = $contract_year-1;}else{ $cyear = $contract_year;}

  $sql2="SELECT ifnull(sum(msi_billing.bill_hrs),0),ifnull(sum(msi_close_off.extra),0),ifnull(msi_close_off.borrowed,0),ifnull(msi_close_off.invoiced,0)
  FROM msi_billing,msi_close_off
  WHERE msi_billing.client = msi_close_off.client
  and month(msi_close_off.closingmonth)= $m  and year(msi_close_off.closingmonth)= $cyear
  AND msi_billing.client = \"$id\" and upper(msi_billing.type) = 'CONTRACT' "; 
  
     $usedprev = sql::sql_run($sql2);
     $prev =  $usedprev [0][0];
    // $extra = $usedprev [0][1];
     $borrowprev = $usedprev [0][2];
    //echo  $borrowprev."<br />";
     $leftprev = $totalHours - $prev;
   
       if ($leftprev < 0 ){
     //  $extra = $leftprev*-(1);
          $leftprev = 0;
     }

   $off="SELECT ifnull(sum(`msi_close_off`.`writeoff`),0)
     FROM `msi_close_off`
     WHERE month(`msi_close_off`.closingmonth)= $prev_month  and year(`msi_close_off`.closingmonth)= $prev_year
     AND `msi_close_off`.client = \"$id\"
     ";
     $offprev = sql::sql_run($off);
     $writeprev = $offprev [0][0];
          
//calculate extra hours previous month

$carry2 = sql::sql_run("select ifnull(msi_close_off.carried,0),
                         ifnull(msi_close_off.borrowed,0),
                         ifnull(msi_close_off.extra,0),
                         ifnull(msi_close_off.invoiced,0),
                         sum(msi_billing.bill_hrs),
                         msi_contract.hours,
                         msi_contract.server_hours,
                         msi_close_off.closed,
                         ifnull(msi_close_off.writeoff,0)
                         
 from msi_billing,msi_contract,msi_close_off
 where msi_billing.client = msi_contract.client
 and msi_close_off.client = msi_billing.client
 and month(msi_close_off.closingmonth) = $contract_month 
 and year(msi_close_off.closingmonth)= $contract_year
 AND msi_billing.client = \"$id\" and upper(msi_billing.type) = 'CONTRACT' 
 and (('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
 and date_format( msi_contract.enddate,'%Y-%m')) or
 (msi_contract.terminating='no'and '$fulldate2'>=date_format(msi_contract.startdate,'%Y-%m-00'))
 or ('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
 and date_format( msi_contract.terminatingdate,'%Y-%m') and terminating='yes') ) ");		
		
   // AND msi_contract.terminating = 'no' limit 1");
     
$inv2 = $carry2[0][3];
$writtenoff =$carry2[0][8];
$previous = sql::sql_run("select sum(msi_billing.bill_hrs),
                            msi_contract.hours,
                            msi_contract.server_hours
 from msi_billing,msi_contract
 where msi_billing.client = msi_contract.client
 and  month(msi_billing.date) = $contract_month
 and year(msi_billing.date)= $contract_year
 AND msi_billing.client = \"$id\" and upper(msi_billing.type) = 'CONTRACT'
 and (('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
 and date_format( msi_contract.enddate,'%Y-%m')) or
 (msi_contract.terminating='no'and '$fulldate2'>=date_format(msi_contract.startdate,'%Y-%m-00'))
 or ('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
 and date_format( msi_contract.terminatingdate,'%Y-%m') and terminating='yes') )");	
		
//AND msi_contract.terminating = 'no' limit 1");

 $unused2 = sql::sql_run("SELECT ifnull( msi_contract.hours, 0 ) + ifnull( msi_contract.server_hours, 0 )
 from msi_contract
 where msi_contract.client = \"$id\"
 and (('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
 and date_format( msi_contract.enddate,'%Y-%m')) or
 (msi_contract.terminating='no'and '$fulldate2'>=date_format(msi_contract.startdate,'%Y-%m-00'))
 or ('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
 and date_format( msi_contract.terminatingdate,'%Y-%m') and terminating='yes') )");	
	
// AND msi_contract.terminating = 'no'");


$extra2 = $previous[0][0] -($previous[0][2]+$previous[0][1] );
$unused = $previous[0][1] + $previous[0][2] - $previous[0][0];

if ($unused < 0 ){
$unused = 0;
}
else if($previous[0][0] == 0 ){
$unused =  $unused2[0][0];
}

if($extra2 < 0){
$extra2 = 0;
}
else {
$extra2 = $extra2 ;}

$prevchanges = sql::sql_run("select msi_audit.date, msi_audit.msi_user
                       From msi_audit
                       where msi_audit.company =\"$id\" order by msi_audit.date desc limit 1");
	
if(isset($_POST['update']))	{
$ca = $_POST['carried'];
$bo = $_POST['borrowed'];
$ex = $_POST['extra'];
$writeoff = $_POST['writeoff'];
$available = $_POST['deduct'];
$hours_left = $_POST['hours_left'];
$unused_updated = $_POST['previous'];
if(isset($_POST['invoiced'])){
$inv = $_POST['invoiced'];
 }
$effect = $previous[0][1] +$previous[0][2] - $previous[0][0] + $bo - $borrowprev ;
$fg =  $_SESSION['username'];
$today = date("Y-m-d h:i:s");
$closemonth = $contract_year."-".$strmonth."-00";
$closedate = date("Y-m-d");

$check_duplicate = sql::sql_run("select * 
                       From msi_close_off
                       where msi_close_off.closingmonth = '$closemonth'
                       and msi_close_off.client = '$id'");

if(!$check_duplicate){
$insert = "Insert into msi_close_off
           values(null,$id, $unused,$inv,$ex,'Yes',$effect,$bo,$writeoff,'$closemonth','$closedate','$fg','$hours_left',$unused_updated)";
sql::sql_run($insert);

if(!$insert){
echo mysqli_error($GLOBALS['db']);
}
}
 else {
   $update =  sql::sql_run(" update msi_close_off set 
    msi_close_off.carried = '$ca',
    msi_close_off.borrowed = '$bo', 
    msi_close_off.extra = '$ex',
    msi_close_off.invoiced = '$inv',
    msi_close_off.writeoff = '$writeoff',
   	msi_close_off.available = '$hours_left',
   	msi_close_off.unused = '$unused_updated'
	where month(msi_close_off.closingmonth) = $contract_month 
	and year(msi_close_off.closingmonth)= $contract_year
    AND msi_close_off.client = \"$id\" 
 "); 
   
// echo $update ."<br />";
 if(!$update){
mysqli_error();
}   
}
   
$comp = sql::sql_run("select msi_contract.client 
from msi_contract,msi_billing
where msi_billing.client = msi_contract.client
 AND msi_billing.client = \"$id\" and upper(msi_billing.type) = 'CONTRACT'
");
$company =$comp[0][0];

$ins = "insert into msi_audit (id, company,date,extra_prev,carried,borrowed,invoiced,writeoff,msi_user)
 values (null,
     $company,
     '$today',
     $ex,
     $ca,
     $bo,
     $inv,
     $writeoff,
     '$fg')
     ";

//$audit = sql::sql_run($ins);
//if(!$audit){
// mysql_error();
   
//}
//else { mysql_error();} 

contract::termination($client,$hours,$identity,$year2,$month2,$year3,$month3,$termmonth,$termyear,$term,$primary,$backup,$server_hours,$server_techie,$effective);
contract::terminate(NULL,NULL);

}
else {

//calculate effective hours
$total = "SELECT date_format(msi_contract.startdate,'%Y-%m-%01') 
          FROM msi_contract
          WHERE msi_contract.client = \"$id\"
          and (('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
 and date_format( msi_contract.enddate,'%Y-%m')) or
 (msi_contract.terminating='no'and '$fulldate2'>=date_format(msi_contract.startdate,'%Y-%m-00'))
 or ('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
 and date_format( msi_contract.terminatingdate,'%Y-%m') and terminating='yes') )";
// AND	msi_contract.terminating='no'
$contract = sql::sql_run($total);
$contract_date = $contract[0][0];
$num = cal_days_in_month(CAL_GREGORIAN, $contract_month, $contract_year); // 31
$ddate=$contract_year.'-'.$strmonth.'-'.$num;
$date1 = date(strtotime($contract_date));
$date2 = date(strtotime($ddate));
$difference = $date2 - $date1;
$month = floor($difference / 86400 / 30 )+ 1;
//$months = round($difference / 86400 / 30 );
$contracttime  = $contract_date ;
$currentdate = $contract_year."-".$contract_month."-".$num;
$d1 = strtotime($contracttime );
$d2 = strtotime($currentdate);
$min_date = min($d1, $d2);
$max_date = max($d1, $d2);
$t = 1;
while (($min_date = strtotime("+1 MONTH", $min_date)) <=  $max_date) {
$t++; 
} 

$tothrs ="SELECT (ifnull(msi_contract.hours,0) + ifnull(msi_contract.server_hours,0))
         FROM msi_contract
         WHERE msi_contract.client =\"$id\"
         and (('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
  and date_format( msi_contract.enddate,'%Y-%m')) or
  (msi_contract.terminating='no'and '$fulldate2'>=date_format(msi_contract.startdate,'%Y-%m-00'))
  or ('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
  and date_format( msi_contract.terminatingdate,'%Y-%m') and terminating='yes') ) ";
$hours =  sql::sql_run($tothrs);
$allhours = $hours[0][0] * $t;
$tothours = $hours[0][0];

//calculate hours from prev month
$m = $contract_month-1;
if($m<10){$m = '0'.$m;}
else {$m =$m;}
if($m == '0'){ $m = 12;}
if($m == '12'){ $cyear = $contract_year-1;}else{ $cyear = $contract_year;}

  $sql2="SELECT ifnull(sum(msi_billing.bill_hrs),0)
          FROM msi_billing
          WHERE month(msi_billing.date)= $prev_month  and year(msi_billing.date)= $prev_year
          AND msi_billing.client = \"$id\" and upper(msi_billing.type) = 'CONTRACT'";

   $sql3= "SELECT ifnull(sum(msi_close_off.extra),0),ifnull(msi_close_off.borrowed,0),ifnull(msi_close_off.writeoff,0)
          FROM msi_close_off
          WHERE month(msi_close_off.closingmonth) = $m
          AND year(msi_close_off.closingmonth)= $cyear
          AND msi_close_off.client = \"$id\" "; 

    $sql4= "SELECT sum(ifnull(msi_close_off.writeoff,0)),msi_close_off.closingmonth,msi_contract.startdate 
           FROM msi_close_off,msi_contract 
           WHERE msi_close_off.client = msi_contract.client
           AND msi_close_off.closingmonth >= msi_contract.startdate 
           AND msi_close_off.closingmonth <= \"$fulldate2\"
           and (('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
           and date_format( msi_contract.enddate,'%Y-%m')) or
           (msi_contract.terminating='no'and '$fulldate2'>=date_format(msi_contract.startdate,'%Y-%m-00'))
           or ('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
           and date_format( msi_contract.terminatingdate,'%Y-%m') and terminating='yes') ) 
           AND msi_close_off.client = \"$id\" "; 

      $usedprev = sql::sql_run($sql2);
      $previous = sql::sql_run($sql3);
      
      $prev =  $usedprev[0][0];
      $extra = $previous[0][0];
     
      $write = sql::sql_run($sql4);
      $writeoff = $write[0][0];
     
      $leftprev = $results2[0][2]- $prev;
     // $leftprev = $tothours - $prev;
      //echo $leftprev;
       if ($leftprev < 0 ){
          $extra = $leftprev*-(1);
          $leftprev = 0;
     }
     else {$extra = $extra;}
    
$sql5="SELECT ifnull(msi_close_off.invoiced,0)
  FROM msi_close_off
  WHERE msi_close_off.client = \"$id\"
  AND month(msi_close_off.closingmonth)= $prev_month 
   and year(msi_close_off.closingmonth)= $prev_year
   ";

$invoicedprev = sql::sql_run($sql5);
$invprev =  $invoicedprev[0][0];

$usedcurrent="SELECT sum(bill_hrs)
      FROM msi_billing
      WHERE month(date) = $contract_month and year(date)= $contract_year
      AND client = \"$id\" and upper(type) = 'CONTRACT' "; 	
      $usedhourscur = sql::sql_run($usedcurrent);    
     
     $used =  $usedhourscur[0][0];

  $effecborrow = "SELECT ifnull(sum(msi_close_off.borrowed),0)
  FROM msi_close_off,msi_contract 
  WHERE msi_close_off.client  = \"$id\"
  AND msi_close_off.client = msi_contract.client
  AND msi_close_off.closingmonth >= msi_contract.startdate 
  and month(msi_close_off.closingmonth) = $m
  AND year(msi_close_off.closingmonth)= $cyear
  and (('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
  and date_format( msi_contract.enddate,'%Y-%m')) or
  (msi_contract.terminating='no'and '$fulldate2'>=date_format(msi_contract.startdate,'%Y-%m-00'))
  or ('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
  and date_format( msi_contract.terminatingdate,'%Y-%m') and terminating='yes') )
  "; 

  $effecinvoice = "SELECT sum(msi_close_off.invoiced),sum(ifnull(msi_close_off.borrowed,0))
  FROM msi_close_off,msi_contract 
  
  WHERE msi_close_off.client  = \"$id\"
  AND msi_close_off.client = msi_contract.client
  AND msi_close_off.closingmonth >= msi_contract.startdate 
  AND msi_close_off.closingmonth <= '$ddate'

 and (('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
 and date_format( msi_contract.enddate,'%Y-%m')) or
 (msi_contract.terminating='no'and '$fulldate2'>=date_format(msi_contract.startdate,'%Y-%m-00'))
 or ('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
 and date_format( msi_contract.terminatingdate,'%Y-%m') and terminating='yes') )";
     
$effecthours ="SELECT ifnull(sum(msi_billing.bill_hrs),0),msi_billing.date, date_format( msi_contract.startdate,'%Y-%m'), msi_contract.enddate
FROM msi_contract,msi_clients,msi_billing
WHERE msi_clients.id = msi_contract.client 
AND msi_billing.client = msi_contract.client 
AND msi_billing.date >= msi_contract.startdate 
AND msi_billing.date <= '$ddate' 
AND msi_billing.client = \"$id\" and upper(msi_billing.type) = 'CONTRACT'
and (('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
and date_format( msi_contract.enddate,'%Y-%m')) or
(msi_contract.terminating='no'and '$fulldate2'>=date_format(msi_contract.startdate,'%Y-%m-00'))
or ('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
and date_format( msi_contract.terminatingdate,'%Y-%m') and terminating='yes') )
AND msi_clients.customer = 1 "; 

 $efehours = sql::sql_run($effecthours); 
 $peffective = $efehours[0][0];
 $pinvoiced = sql::sql_run($effecinvoice); 
 $inveffec= $pinvoiced[0][0];
 $borowing = sql::sql_run($effecborrow); 
 $borrowprev =  $borowing[0][0];

 $unusedall = $allhours -  $peffective;
 if($unusedall < 0){
 	$unusedall = 0;
 }

$invoicedcurrent = "SELECT sum(ifnull(msi_close_off.invoiced,0))
                    FROM msi_close_off, msi_contract
                    WHERE msi_close_off.closingmonth like \"%$fulldate%\"
                    AND msi_close_off.client = \"$id\" 
                    AND msi_contract.client = msi_close_off.client
                    AND msi_close_off.closingmonth >= msi_contract.startdate
and (('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
and date_format( msi_contract.enddate,'%Y-%m')) or
(msi_contract.terminating='no'and '$fulldate2'>=date_format(msi_contract.startdate,'%Y-%m-00'))
or ('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
and date_format( msi_contract.terminatingdate,'%Y-%m') and terminating='yes') )";

$invoiced = sql::sql_run($invoicedcurrent);   
$invcd = $invoiced[0][0];

$condate= "SELECT month(msi_contract.startdate), year(msi_contract.startdate)
                  FROM
                  msi_contract
                  WHERE 
                  msi_contract.client = \"$id\"
                  and (('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
 and date_format( msi_contract.enddate,'%Y-%m')) or
 (msi_contract.terminating='no'and '$fulldate2'>=date_format(msi_contract.startdate,'%Y-%m-00'))
 or ('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
 and date_format( msi_contract.terminatingdate,'%Y-%m') and terminating='yes') )";  

$ddate = sql::sql_run($condate);
$mdate = $ddate[0][0];
$ydate = $ddate[0][1];
$sdate = $ydate."-".$mdate;
$cdate = $contract_year."-".$contract_month;

$effectivenow = 0;
if ($sdate == $cdate){
$effectivenow  = $tothours - $used + $invcd + $borrowed - $writeoff + $available; 
//echo $effectivenow  ." = ".$tothours." - ". $used ." + ". $invcd   ." +". $borrowed." + ". $available." - ". $writeoff." + ". $available."<br />";

$leftprev = 0;
$extra = 0;
}

else{
$effectivenow  = $allhours - $peffective +  $inveffec + $borrowed  - $writeoff + $available;
//echo $effectivenow  ." = ".$allhours ." - ". $peffective." + ". $inveffec  ." +". $borrowed." - ". $writeoff." + ". $available."<br />";
$leftprev = $leftprev ;
}
$bdate =  date('m',strtotime($cdate));
if($bdate > 10){
$bdate = $contract_year."-".$contract_month;
 }
else {$bdate = $contract_year."-0".$contract_month;} 
if($bdate == date('Y-m')){
//$effectivenow  = $allhours - $peffective - $used + $inveffec  + $borrowed;

}
   if(($used + $borrowed +$borrowprev ) >= $totalHours) {
    $carried = 0;
    }
    else
    {
    $carried = $unused; 
    }
$gr = $prev + $borrowprev;
 if($gr >= $totalHours) {
 $leftprev = 0;
   
 }
 $effectivenow = number_format($effectivenow, 2);
//if($effectivenow < 0){$effectivenow = ($effectivenow * -1)."  ".$over_contract;}  
//if($effectivenow < 0){$effectivenow = 0;}

$t =$results2[0][2];
if($t < 0){$t = 0;}
if ($sdate == $cdate){
	$leftprev = 0;

}
else {$leftprev = $t- $prev;}

if($leftprev < 0){$leftprev = 0;}

///////////////////////////////////////////////////////////////////////////////

?>

<?php 

$results3=sql::sql_run("SELECT username FROM msi_users WHERE activetechie = 'yes' ORDER BY `username`");

	print '<table><tr><td colspan="3" align="left">';
	print "<a href=?module=contract&action=termedit&id=$id&identity=$identity&contract_year=$prev_year&contract_month=$prev_month style=\"color: #1A53FF\">". date(F, mktime(0, 0, 0, $prev_month, 1, $prev_year)) ." $prev_year << Previous</a> ";
	print "&nbsp&nbsp";
	print " <b><a href=?module=contract&action=termedit&id=$id&identity=$identity&contract_year=$contract_year&contract_month=$contract_month >". date(F, mktime(0, 0, 0, $contract_month, 1, $contract_year)) ." $contract_year</a></b>";
	print "&nbsp&nbsp";
	print " <a href=?module=contract&action=termedit&id=$id&identity=$identity&contract_year=$next_year&contract_month=$next_month style=\"color: #1A53FF\">Next >> ". date(F, mktime(0, 0, 0, $next_month, 1, $next_year)) ." $next_year</a><p>";
	print '</td></tr><tr><td valign=top>';

    print "<span class=\"head\">Cancell/Update Contract Hours</span><br />";       
    print" <table><tr><td>
            <table class=\"box\">
              <tr>
              <td align=\"right\"><b>Company Name:</b>
               </td>
                 <td>
                  <form action=\"index.php?module=contract&action=termedit&id=$id&identity=$identity&contract_year=$contract_year&contract_month=$contract_month\" name=\"editform\" method=\"POST\">
                 <select name=\"company\" readonly>
                  <option value=\"".$id."\">".$results2[0][0]."</option>";

                 foreach ($results as $result) {
                   print "<option value=\"$result[1]\">$result[0]</option>";
                 };
	print "
                 </select>
               </td>
	         <tr>

		<td align=\"right\"><b>	Notes:</b></td>
		<td><textarea rows='2' cols='36' maxLength ='60'  name='notes' placeholder='Write Here...' onkeydown='limitTextOnKeyUpDown(this.form.notes,this.form.countdown,60);'
   		 onkeyup='limitTextOnKeyUpDown(this.form.message,this.form.countdown,60);'>".$results2[0][21]."</textarea>
         <input readonly type='text' name='countdown' size='1' value='60'> chars are left</td>
         </tr>	
         <tr colspan = 3 rowspan = 3>
        <td><b>Contract Hours Per Month:</b></td> 
        <td><input style = \"background-color:#C8C8C8\" name=\"hours\" value=\"".$results2[0][2]."\" size='9px'></td>
        <td ><div class = 'bolder'>Effective Hours:</div></td><td><input style = \"background-color:#C8C8C8\" name=\"ehours\" id=\"ehours\" value=\"".$effectivenow."\" size='9px' onkeyup=\"getValues(16)\">";
		//echo " <br />Unused Hours:</div></td><td><input name=\"unus\" value=\"". $unusedall."\" size='9px'></td>";
	print "</tr>
<input type=\"hidden\" name=\"identity\" value=\"".$identity."\">
	<tr>
		<td align=\"right\"><b>Server Hours:</b></td>
		<td><input style = \"background-color:#C8C8C8\" name=\"server_hours\" value=\"".$results2[0][17]."\" size='9px'></td>
		</tr>
		<tr>
		<td align=\"right\"><b> Unused hours:</b></td>
			<td><input style = \"background-color:#C8C8C8\" name=\"carried\" id=\"carried\" value=\"".$unused."\" onkeyup=\"getValues(10)\" readonly=\"true\" size='9px'></td>
		</tr>
        <tr>
     <td align=\"right\"><b> Unused Hours(previous):</b></td>";
 print"  <td><input  style = \"background-color:#C8C8C8\" name=\"carried\"  id=\"unus\" value=\"". $leftprev."\" readonly=\"true\" size='9px' onkeyup=\"getValues(4)\"></td>";
//print " <td><input  style = \"background-color:#C8C8C8\" name=\"carried\"  id=\"unus\" value=\"".$prevunused."\" readonly=\"true\" size='9px' onkeyup=\"getValues(4)\"></td>";		
 print" <td><input name=\"carried\" type = \"hidden\"  id=\"invpre\" value=\"". $invprev."\" readonly=\"true\"  size='9px' onkeyup=\"getValues(5)\"></td>
      		
</tr>
   <tr>
      <td align=\"right\"><b>Hours Written off(previous):</b></td>
      <td><input style = \"background-color:#C8C8C8\" name=\"writeoff\"  id=\"off\" value=\"". $writeprev."\" readonly=\"true\" onkeyup=\"getValues(6)\" size='9px'></td>
      	
</tr>       		
";
		
?>
<script type="text/javascript">

function getValues(){
	


	var extra =parseFloat(document.getElementById("ex").value);
	var borrowed =parseFloat(document.getElementById("brwd").value);
	var borrowedpre=parseFloat(document.getElementById("brwdpre").value);
	//var unusedpre=parseFloat(document.getElementById("unus").value);
	var unusedpre=parseFloat(document.getElementById("previous").value);
	var invpre=parseFloat(document.getElementById("invpre").value);
	var offpre=parseFloat(document.getElementById("off").value);
	var deduct=parseFloat(document.getElementById("deduct2").value);
	var unused =parseFloat(document.getElementById("carried").value);
	var off = parseFloat(document.getElementById("writeoff").value);
	var effective = parseFloat(document.getElementById("ehours").value);
  

if(isNaN(invpre) == "true"){
	invpre = 0;
}
if(isNaN(extra) == "true"){
	extra = 0;
}
if(isNaN(borrowedpre) == "true"){
	borrowedpre = 0;
}
if(isNaN(borrowed) == "true"){
	borrowed = 0;
}
if(isNaN(invpre) == "true"){
	invpre = 0;
}
if(isNaN(offpre) == "true"){
	offpre = 0;
}
if(isNaN(off) == "true"){
	off = 0;
}

if(isNaN(deduct) == "true"){
	deduct = 0;
}
	if(isNaN(borrowed) == "true"){
		alert('enter a valid number');
	} 

	if (unusedpre < 0){
		unusedpre = 0;
	}

	var totalValue =(borrowed + unusedpre + deduct + unused)-(extra + borrowedpre );
	if(effective < 0){
	var totalValue3 = borrowed - (effective*-1);
	}
	else if (effective >= 0) {
		 totalValue3 = effective - borrowed;
	}
  else { totalValue3 = 0;}
	if(totalValue3 < 0){
		totalValue3 = totalValue3 *(-1);
	}
	else { totalValue3 = 0;}
		

	
    var totalValue2 = borrowed + unusedpre + unused  - extra - borrowedpre ;

	if (totalValue < 0 ) {
	 totalValue = totalValue*(-1);
	}
	else (totalValue = 0);

	if(isNaN(totalValue) == "true"){
		alert('enter a valid number');
	}
	//totalValue = parseFloat(totalValue).toFixed(2);
	//document.getElementById("invoiced").value = totalValue;
	totalValue3 = parseFloat(totalValue3).toFixed(2);
	document.getElementById("invoiced").value = totalValue3;

	

	 var hours_left = 0 ;

	  if (totalValue2 < 0 ) {
		 totalValue2 = totalValue2*(-1);
		 hours_left =  deduct - totalValue2 - off;
		 if (hours_left < 0)
		 {
		hours_left = 0;
		 }
		}

	else{
		hours_left =  deduct - off;
		}

hours_left = parseFloat(hours_left).toFixed(2);
document.getElementById("hours_left").value = hours_left;

var prev = 0;
if (unused > 0){
 prev = unused - (borrowedpre + off);
}
if(prev < 0){
	prev = 0;
}
//alert(prev);
prev = parseFloat(prev).toFixed(2);
document.getElementById("previous").value = prev;
}
</script>
<style>
input.numbox{
	width:30px;
	height:20px;
}
input.mainbox{
	width:9px;
	height:5px;
	font-size:30;
	font-weight:bold;
}
</style> 
<?php   
  print "<tr>
		<td align=\"right\"><b>Extra Hours:</b></td>
        <td><input style = \"background-color:#C8C8C8\"  name = \"extra\" type=\"text\" id=\"ex\" value=\"".$extra2."\" readonly=\"true\" size='9px' onkeyup=\"getValues(1)\" /></td>
			</tr>
				<tr>
	<td align=\"right\"><b> Borrowed prev month: </b></td>
	<td><input style = \"background-color:#C8C8C8\" name=\"brwdpre\"  id = \"brwdpre\" value=\"".$borrowprev."\"  readonly=\"true\" size='9px' ></td>
	</tr>
			<tr>
	<td align=\"right\"><b>	Borrow from next month:</b></td>
     <td> <input  size = \"9px\" name = \"borrowed\" type=\"text\" id=\"brwd\" value = \"".$borrowed."\" onkeyup=\"getValues(2)\"/></td>
		</tr>
		<tr>
		   <td align=\"center\">&nbsp;</td><td><strong> To undo the invoicing, modify the borrow from next month field</strong></td>
		   </tr>
	<tr>
	<td align=\"right\"><b>Invoiced:</b></td>
    <td><input style = \"background-color:#C8C8C8\" name = \"invoiced\" id=\"invoiced\" value=\"".$inv2." \" readonly=\"true\" size = \"9px\" onkeyup=\"getValues(3)\"/><i> Automatically calculated </i>
	
    <td><input type = \"hidden\" style = \"background-color:#C8C8C8\" name = \"previous\" id=\"previous\" value=\"$prevunused\" readonly=\"true\" size = \"9px\" onkeyup=\"getValues(3)\"/>
	</tr>
          			
	<tr>
	<td align=\"right\"><b>Write-off:</b></td>
    <td><input name = \"writeoff\" id=\"writeoff\" value=\"".$writtenoff." \" size = \"9px\" type=\"text\" onkeyup=\"getValues(3)\" / ></td>
	</tr>	
     		
     <tr>
	<td align=\"right\"><b>Hours added/deducted:</b></td>
    <td><input  style = \"background-color:#C8C8C8\" name = \"deduct\" id=\"deduct\" value=\"".$available." \" size = \"9px\" type=\"text\" onkeyup=\"getValues(2)\" / > <i> (hours added at the beginning of the contract)</i></td>
		</tr>
		<tr>
	<td align=\"right\"><b>Hours available(current)</b></td>
		<td><input style = \"background-color:#C8C8C8\"  name = \"deduct2\" id=\"deduct2\" value=\"".$avail." \" size = \"9px\" type=\"text\" onkeyup=\"getValues(5)\" / > <i> (available from the added hours to use in current month)</i> </td>
		<td><input style = \"background-color:#C8C8C8\" name = \"hours_left\" id=\"hours_left\" value=\"".$avail."\" size = \"9px\" type=\"hidden\" onkeyup=\"getValues(8)\" / ></td>
	</tr>"; 


print"
 <tr>
	<td align=\"right\"><b>Percentage :</b></td>
		<td><input style = \"background-color:#C8C8C8\"  name = \"notification\" id=\"notification\" value=\"".$results2[0][23]." \" size = \"9px\" type=\"text\" / > <i> (Customer notification when hours almost used up)</i> </td>
		
	</tr>  
	    

<tr>
<td align=\"right\"><b>StartDate:</b></td>
        <td>";
    
	print "</select> \n
<select name=\"month2\">";

 foreach ($months as $num => $month_name) {

		print '<option value="' . $num . '"';
		if ($month_name==$results2[0][4]) { print " selected"; }
		print ">" .$month_name. "</option>\n";

}
	print "</select>\n";

    print '<select name="year2">';
	for ($year = date('Y') -5, $max_year = date('Y') + 2; $year < $max_year; $year++) {
		print '<option value="' . $year . '"';
		if ($year==$results2[0][6]) { print " selected"; }
		print ">" . $year. "</option>\n";
	}
	print "</select>\n";

print "</td>";
	print"</tr>
 <tr>
  <td align=\"right\"><b>EndDate:</b></td>
        <td>";
	print "</select> \n";
	print '<select name="month3">';
		if( is_array( $months ) && count( $months ) > 0 ) {
foreach ($months as $num => $month_name) {
		print '<option value="' . $num . '"';
		if ($num==$results2[0][8]) { print " selected"; }
		print ">" . $month_name . "</option>\n";
	}
		}
	print "</select>\n";
	print '<select name="year3">';
	for ($year = date('Y')-5, $max_year = date('Y') + 2; $year < $max_year; $year++) {
		print '<option value="' . $year . '"';
		if ($year==$results2[0][10]) { print " selected"; }
		print ">" . $year . "</option>\n";
}
	print "</select>\n";

   if ($results2[0][11] == "yes") { $read = 'checked'; } else { $read ="";}
   
      print" </td>
      <tr>
        <td align=\"right\"><b>Cancelled:</b></td>
        <td><input type=\"checkbox\" name=\"term\"  value=\"1\" \"$read\" ></td>
	    </tr>
      <tr>
           <td align=\"right\"><b>Date of cancellation:</b></td>";

    print "<td>";
	print "</select> \n";
	print '<select name="termmonth">';
if( is_array( $months ) && count( $months ) > 0 ) {
foreach ($months as $num => $month_name) {
		print '<option value="' . $num . '"';
		if ($num==$results2[0][13]) { print " selected"; }
		print ">" . $month_name . "</option>\n";
	}
}
	print "</select>\n";
	print '<select name="termyear">';
	for ($year = date('Y')-3, $max_year = date('Y') + 2; $year < $max_year; $year++) {
		print '<option value="' . $year . '"';
		if ($year==$results2[0][14]) { print " selected"; }
       	print ">" . $year . "</option>\n";
	}
	print "</select>\n";

      print" </td>
      
	</tr>
      <tr>
	<td align=\"right\"><b>Primary Technician:</b> </td>
	<td colspan=\"2\" align=\"left\"><select name=\"primary\">";
		   print "<option value=''></option>";
foreach ($results3 as $result3) {
                   print "<option value=\"$result3[0]\""; 
			if ($result3[0]==$results2[0][15]) { print " selected"; } 
		   print ">$result3[0] </option>";
};

print "	</select></td>
      </tr>
      <tr>
	<td align=\"right\"><b>Backup Technician:</b> </td>
	<td colspan=\"2\" align=\"left\"><select name=\"backup\">";
	 print "<option value=''></option>";
foreach ($results3 as $result3) {
                   print "<option value=\"$result3[0]\""; 
			if ($result3[0]==$results2[0][16]) { print " selected"; } 
		   print ">$result3[0] </option>";
                 };

print "	</select></td>
      </tr>
		<tr><td align=\"right\"><b>Site Manager :</b></td>
                 	<td><select name=\"server_techie\"><option value=''></option>";
                    foreach ($results3 as $result) {
                   	echo "<option value=\"$result[0]\"";
						if ($result[0] == $results2[0][18])
							echo ' selected="yes"';
						print ">$result[0]</option>";
                 			};
      					print "</select></td></tr>



<tr><td align=\"right\"><b>Account Manager :</b></td>
                 	<td><select name=\"acc_manager\"><option value=''></option>";
                    foreach ($results3 as $result) {
                    	
                   	echo "<option value=\"$result[0]\"";
						if ($result[0] == $results2[0][22])
							echo ' selected="yes"';
						print ">$result[0]</option>";
                 			};
      					print "</select></td></tr>     		
                   		 ";
/*print " 	 
      //<tr>
        <td align=\"right\"><b>Weekly Email:</b></td>
        <td><input type=\"checkbox\" name=\"email\"  value=\"1\" ></td>
	</tr>
	
	<tr>
	<td align=\"right\"><b>Email Address:</b></td>
    <td><input  name = \"email1\" type=\"text\"  size='35px' /></td>
	</tr>
			
	<tr>
	<td align=\"right\"><b>	Email Address:</b></td>
    <td> <input size = \"35px\" name = \"email2\" type=\"text\" /></td>
	</tr>
		
	<tr>
	<td align=\"right\"><b>Email Address:</b></td>
    <td> <input size = \"35px\" name = \"email3\" type=\"text\" /></td>
	</tr> "; */
			
print " <tr><td colspan=\"2\" align=\"center\">
              <input type=\"submit\" value=\"Update\" name=\"update\" class=\"buttons\">
              <input type=\"submit\" value=\"Delete\" name=\"Delete\" onclick=\"return confirm('Are you sure you want to delete this item?')\" class=\"delete\">
              <input type=\"button\" onClick=\"javascript:history.go(-1)\"  value=\"Cancel\" class=\"buttons\">
              <b>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
              Last changes were made on <br />&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
              &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
              &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
              &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;". $prevchanges[0][0]." By ". ucfirst($prevchanges[0][1])."</b></td></tr>";

print '</form>

</table></td><td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td>';
print "<br/>";
	
print "<td valign = \"top\"><table style = \"width:400px\" class=\"client\"padding=\"none\">";
print "<tr><span class=\"head\" align=\"right\">Fields Description</span></tr>";
print "<tr><td class=\"client\"><font color=\"blue\">Contract hours per month</font></td><td class=\"client\">Contracted hours to be covered per month</td></tr>
<tr><td class=\"client\"><font color=\"blue\">Unused</font></td><td class=\"client\"> Hours that have not been covered from this month only</td></tr>
<tr><td class=\"client\"><font color=\"blue\">Extra hours</font></td><td class=\"client\">Additional hours for selected month</td></tr>
<tr><td class=\"client\"><font color=\"blue\">Borrowed prev month</font></td><td class=\"client\"> Hours borrowed in previous month</td></tr>
<tr><td class=\"client\"><font color=\"blue\">Borrow from next month</font></td><td class=\"client\"> Hours to be borrowed in the following month - (Settable)</td></tr>
<tr><td class=\"client\"><font color=\"blue\">Invoiced</font></td><td class=\"client\">Hours to be invoiced</td></tr>
<tr><td class=\"client\"><font color=\"blue\">Write Off</font></td><td class=\"client\"> Number of hours to be written off</td></tr>
<tr><td class=\"client\"><font color=\"blue\">Effective</font></td><td class=\"client\">Hours left over in current month, taking into consideration carried over hours and borrowed from hours from next month</td></tr>
</tr>";
print "</table></td></tr></table>"; 

}
}




function terminate($contract_month,$contract_year,$sort_column = null){
  if(!acl::has_access($_SESSION['id'],"contracts","read")) return 1;

print"</br>";
//beginning of new code, 2008/06/06 making contracts month aware.

if(!isset($contract_month)&&!isset($contract_year))
{
$contract_year = date(Y);
$contract_month = date(n);
}

  $next_month = $contract_month +1;
  if ($next_month > 12) {
          $next_month = 1;
          $next_year = $contract_year +1;
          }
  else {
          $next_year = $contract_year;
  }

  $prev_month = $contract_month - 1;
  if ($prev_month == 0){
          $prev_month = 12;
          $prev_year= $contract_year -1;
          }
  else {
    $prev_year= $contract_year;
  }

//add leading 0 for months less than 10
if($contract_month < 10){
$strmonth = '0'.$contract_month;
}else{
$strmonth = $contract_month;
}
//end add leading
//$numdays=date("t", strtotime($contract_year . "-" .$strmonth . "-01"));
$moncalc=$contract_year.'-'.$strmonth.'-'.'01';
$fulldate=$contract_year.'-'.$strmonth;
$fulldate2=$contract_year.'-'.$strmonth.'-'.'00';

if(!isset($sort_column)){
	$sort_column = 'company';
}

$results = sql::sql_run("select msi_clients.company,
msi_contract.client,
msi_contract.hours,
date_format(msi_contract.startdate,'%b-%Y'),
date_format(msi_contract.enddate,'%b-%Y'),
msi_contract.contid,
datediff(msi_contract.enddate,now()),
msi_contract.enddate,
msi_contract.contid,
msi_contract.terminating,
msi_contract.primarytech,
msi_contract.backuptech,
msi_contract.server_hours,
msi_contract.effective,
msi_contract.server_techie,
msi_contract.notes,
msi_contract.acc_manager
	
 from msi_contract,
 msi_clients where
 msi_clients.id = msi_contract.client
 and (('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
 and date_format( msi_contract.enddate,'%Y-%m')) or
 (msi_contract.terminating='no'and '$fulldate2'>=date_format(msi_contract.startdate,'%Y-%m-00'))
 or ('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
 and date_format( msi_contract.terminatingdate,'%Y-%m') and terminating='yes') )
 order by $sort_column  " );
 
  print "<span class=\"head\">Contract Admin (X)</span>
  <p><b>Listing all active contracts. To cancell or update a contract click on the customer name</b></p>";
  print '<table><tr><td colspan="3" align="left">';
  print "<a href=?module=contractadmin&action=terminate&contract_year=$prev_year&contract_month=$prev_month style=\"color: #1A53FF\">". date(F, mktime(0, 0, 0, $prev_month, 1, $prev_year)) ." $prev_year << Previous</a> ";
  print "&nbsp&nbsp";
  print " <b><a href=?module=contractadmin&action=terminate&contract_year=$contract_year&contract_month=$contract_month >". date(F, mktime(0, 0, 0, $contract_month, 1, $contract_year)) ." $contract_year</a></b>";
  print "&nbsp&nbsp";
  print " <a href=?module=contractadmin&action=terminate&contract_year=$next_year&contract_month=$next_month style=\"color: #1A53FF\">Next >> ". date(F, mktime(0, 0, 0, $next_month, 1, $next_year)) ." $next_year</a><p>";
  print '</td></tr>
  <tr><td valign=top>';

//ending of new code
 print "<span class=\"head\">Client Contracts</span>";
 print "<br>";
 //<td style=\"border-left: 1px solid #FFFFFF\">Used Total</td>
print"<form action=\"index.php?module=contract&action=term\" name=\"editform\" method=\"POST\">
<table><tr><td>
            <table class=\"client\">		
            <tr class=\"head\">
            
            <td class=\"client-link\"><a href=\"?module=contractadmin&action=terminate&contract_year=$contract_year&contract_month=$contract_month&sort_column=company\">Client</a></td>
  <td class=\"client-link\" title =\"Contracted hours to be covered per month\" style=\"border-left: 1px solid #FFFFFF\" >&nbsp;
  <a href=\"?module=contractadmin&action=terminate&contract_year=$contract_year&contract_month=$contract_month&sort_column=hours\">Hours</a>&nbsp;</td>
            <td title =\"Total hours that have been used in this month only\"style=\"border-left: 1px solid #FFFFFF\">&nbsp;Used&nbsp; </td> 
            <td title =\"Hours that have not been covered from this month only\"style=\"border-left: 1px solid #FFFFFF\">&nbsp;Unused&nbsp;</td> 
            <td title =\"Hours that should be borrowed from the following month\"style=\"border-left: 1px solid #FFFFFF\">&nbsp;Borrowed&nbsp;</td> 
            <td title =\"Hours that have not been covered from previous month only\"style=\"border-left: 1px solid #FFFFFF\">&nbsp;Under Prev</td> 
            <td title =\"Additional hours covered in the previous month only\"style=\"border-left: 1px solid #FFFFFF\">&nbsp;Over Prev</td> 
            <td title =\"Hours to be carried over to the next month\"style=\"border-left: 1px solid #FFFFFF\">&nbsp;Carry over</td> 
            <td title =\"Hours left over in current month, taking into consideration carried over hours from previous month and borrowed from hours from next month\"style=\"border-left: 1px solid #FFFFFF\">&nbsp;Effective&nbsp;</td>
            <td class=\"client-link\" style=\"border-left: 1px solid #FFFFFF\"> <a href=\"?module=contractadmin&action=terminate&contract_year=$contract_year&contract_month=$contract_month&sort_column=startdate\">StartDate</a></td>
            <td class=\"client-link\" style=\"border-left: 1px solid #FFFFFF\"> <a href=\"?module=contractadmin&action=terminate&contract_year=$contract_year&contract_month=$contract_month&sort_column=enddate\">EndDate</a></td>
            <td style=\"border-left: 1px solid #FFFFFF\">Status</td>
            <td class=\"client-link\"><a href=\"?module=contractadmin&action=terminate&contract_year=$contract_year&contract_month=$contract_month&sort_column=primarytech\">Primary</a></td>
            <td>Backup</td>
	    <td>Site Manager</td>
            <td>Account Manager</td>
            <td>Notes</td>
            </tr>";

 foreach ($results as $result) {
 $dateman=$result[7];
 $dateim=explode("-",$dateman);
 $numdays=date("t", strtotime($dateim[0]. "-" . $dateim[1]. "-01"));
 $newend=$dateim[0].'-'.$dateim[1].'-'.$numdays;
     //condition for current month

      if(date("m")==$strmonth and date("Y")==$contract_year)
      {
       $start_date=date("Y-m-d");
        $end_date=$newend;
      }
      if(date("m")>$strmonth and date("Y")==$contract_year)
      {
       $start_date=date("Y-m-d");
        $end_date=$newend;

      }
      if(date("Y")>$contract_year and $dateim[0]<date("Y"))
      {
        $start_date=date("Y-m-d");
        $end_date=$newend;

      }
      if(date("Y")>$contract_year and $dateim[0]==date("Y"))
      {
        $start_date=$moncalc;
        $end_date=$newend;

      }
     
      if(date("m")<$strmonth and date("Y")==$contract_year)
      {
        $start_date=$moncalc;
        $end_date=$newend;
      }
 
       $daydiff=contract::calculateDurationFromDates($start_date,$end_date);

       //$daydiff=$result[6];
        if($daydiff>30 and $daydiff<=60)
        {
            $color="#FF8C00";
        }
       else if($daydiff>=1 and $daydiff<=30)
        {
            $color="#FF0000";
        }
        else if($daydiff<1)
        {
            $color='#008B8B';
        }
        else
        {
            $color='';
        }
        
         if(!acl::has_access($_SESSION['id'],"contracts","read")) return 1;


$available = $result[13];

 //calculate effective hours
$total = "SELECT date_format(msi_contract.startdate,'%Y-%m-%01') 
            from msi_contract, msi_clients 
            where msi_clients.id = msi_contract.client
            and msi_contract.client = \"$result[1]\"
            and (('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
 and date_format( msi_contract.enddate,'%Y-%m')) or
 (msi_contract.terminating='no'and '$fulldate2'>=date_format(msi_contract.startdate,'%Y-%m-00'))
 or ('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
 and date_format( msi_contract.terminatingdate,'%Y-%m') and terminating='yes') )
 order by msi_clients.company asc ";
            
$contract = sql::sql_run($total);
$contract_date = $contract[0][0];
$num = cal_days_in_month(CAL_GREGORIAN, $contract_month, $contract_year); // 31
$ddate=$contract_year.'-'.$strmonth.'-'.$num;
$date1 = date(strtotime($contract_date));
$date2 = date(strtotime($ddate));
$difference = $date2 - $date1;

$month = floor($difference / 86400 / 30 )+ 1;
$months = round($difference / 86400 / 30 );

$contracttime  = $contract_date ;
$currentdate = $contract_year."-".$contract_month."-".$num;

$d1 = strtotime($contracttime );
$d2 = strtotime($currentdate);
$min_date = min($d1, $d2);
$max_date = max($d1, $d2);
$t = 1;

while (($min_date = strtotime("+1 MONTH", $min_date)) <=  $max_date) {
 
$t++; 
} 

$tothrs ="SELECT (ifnull(msi_contract.hours,0) + ifnull(msi_contract.server_hours,0))
 from msi_contract, msi_clients 
 where msi_clients.id = msi_contract.client
 and msi_contract.client = \"$result[1]\"
 and (('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
 and date_format( msi_contract.enddate,'%Y-%m')) or
 (msi_contract.terminating='no'and '$fulldate2'>=date_format(msi_contract.startdate,'%Y-%m-00'))
 or ('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
 and date_format( msi_contract.terminatingdate,'%Y-%m') and terminating='yes') )
 order by msi_clients.company asc ";

$hours =  sql::sql_run($tothrs);
$allhours = $hours[0][0] * $t;
$tothours = $hours[0][0] ;

 //get used hours from previous month.
            $m = $contract_month -1;
            if($m<10){$m = '0'.$m;}
             else {$m =$m;}
             if($m == '0'){ $m = 12;}
             if($m == '12'){ $cyear = $contract_year-1;}else{ $cyear = $contract_year;}
          
            
      $previous="SELECT sum(bill_hrs) 
      FROM msi_billing
      WHERE month(date) = $m and year(date)= $cyear
      AND client = \"$result[1]\" and upper(type) = 'CONTRACT' "; 
 
    $prevusedhours = sql::sql_run($previous);
    $usedprevoius = $prevusedhours[0][0];

    $used = $usedhours[0][0]; 
    
    $sql="SELECT sum(bill_hrs) 
      FROM msi_billing
      WHERE month(date) = $contract_month and year(date)= $contract_year
      AND client = \"$result[1]\" and upper(type) = 'CONTRACT' ";
     	
   $usedhours = sql::sql_run($sql);
   $used = $usedhours[0][0];   
   $serverHours = $result[12];
	$supportHours = $result[2];
	$totalHours = $serverHours + $supportHours;
  	$unused = $totalHours - $used;
  	if($unused < 0){
  	    $unused = 0;}
  	    else {$unused = $unused;
  	    }
  	    
$borowed = sql::sql_run("SELECT ifnull(msi_close_off.carried,0),ifnull(msi_close_off.borrowed,0)
	from msi_billing,msi_close_off

	where msi_close_off.client = msi_billing.client
	and  month(msi_billing.date) = $contract_month 
	and  year(msi_billing.date)= $contract_year
	and  month(msi_close_off.closingmonth) = $contract_month 
	and  year(msi_close_off.closingmonth)= $contract_year
    AND  msi_billing.client = \"$result[1]\" and upper(msi_billing.type) = 'CONTRACT'");
	

$carriedover = $borowed[0][0];
       $borrowed = $borowed[0][1];
       if ($borrowed == null) {
             $borrowed = 0;}

    if ($_GET['contract_month']== date(n)) {$myday=date(d);}
    else {$myday=30;}
	
	$deviance=($used*100/$totalHours)-($myday*100/30) ;
			if($deviance > 20) { 
				$colour='blue';
			}else if($deviance < (-20)) {
				$colour='red';
			}else{
				$colour='black';
			}
  	      $leftprev = $totalHours -$usedprevoius;

   $m = $contract_month-1;
if($m<10){$m = '0'.$m;}
else {$m =$m;}
if($m == '0'){ $m = 12;}
if($m == '12'){ $cyear = $contract_year-1;}else{ $cyear = $contract_year;}


 $sql2="SELECT sum(msi_billing.bill_hrs),ifnull(sum(msi_close_off.extra),0),ifnull(msi_close_off.borrowed,0)
      FROM msi_billing,msi_close_off
      WHERE month( msi_billing.date)= $m  and year( msi_billing.date)= $cyear
      AND month(msi_close_off.closingmonth)= $m  and year(msi_close_off.closingmonth)= $cyear
      AND msi_billing.client = msi_close_off.client
      AND msi_billing.client = \"$result[1]\" and upper(msi_billing.type) = 'CONTRACT' "; 
	
     $usedprev = sql::sql_run($sql2);
     $prev =  $usedprev [0][0];
     $extra = $usedprev [0][1];
     $borrowprev = $usedprev [0][2];
     //$leftprev = $totalHours - $prev;
       if ($leftprev < 0 ){
          $extra = $leftprev*-(1);
          $leftprev = 0;
     }
     else {$extra = $extra;}  

$effecthours ="SELECT ifnull(sum(msi_billing.bill_hrs),0),msi_billing.date, date_format( msi_contract.startdate,'%Y-%m'), msi_contract.enddate,msi_contract.startdate 

FROM msi_contract,msi_clients,msi_billing

WHERE msi_clients.id = msi_contract.client 
AND msi_billing.client = msi_contract.client 
AND msi_billing.client = \"$result[1]\"

 and (('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
 and date_format( msi_contract.enddate,'%Y-%m')) or
 (msi_contract.terminating='no'and '$fulldate2'>=date_format(msi_contract.startdate,'%Y-%m-00'))
 or ('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
 and date_format( msi_contract.terminatingdate,'%Y-%m') and terminating='yes') )
 
AND msi_billing.date >= msi_contract.startdate 
AND msi_billing.date <= '$ddate' 
AND upper(msi_billing.type) = 'CONTRACT'";

//AND msi_clients.customer = 1 "; 
//echo  $ddate."<br />"; 

 $efehours = sql::sql_run($effecthours); 
 $peffective = $efehours[0][0]; 
// $inveffec= $efehours[0][2];

$condate= "SELECT month(msi_contract.startdate), year(msi_contract.startdate)
               FROM
                msi_contract
                WHERE 
                msi_contract.client = \"$result[1]\"
and (('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
 and date_format( msi_contract.enddate,'%Y-%m')) or
 (msi_contract.terminating='no'and '$fulldate2'>=date_format(msi_contract.startdate,'%Y-%m-00'))
 or ('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
 and date_format( msi_contract.terminatingdate,'%Y-%m') and terminating='yes') )";  

 $effecborrow = "SELECT sum(ifnull(msi_close_off.borrowed,0)),msi_close_off.closingmonth,msi_contract.startdate,msi_contract.enddate,month(msi_close_off.closingmonth),year(msi_close_off.closingmonth)
 FROM msi_close_off,msi_contract 
  
 WHERE msi_close_off.client  = \"$result[1]\"
 AND msi_close_off.client = msi_contract.client

 and (('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
 and date_format( msi_contract.enddate,'%Y-%m')) or
 (msi_contract.terminating='no'and '$fulldate2'>=date_format(msi_contract.startdate,'%Y-%m-00'))
 or ('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
 and date_format( msi_contract.terminatingdate,'%Y-%m') and terminating='yes') )
 
 AND msi_close_off.closingmonth >= msi_contract.startdate 
 and month(msi_close_off.closingmonth) = $m
 AND year(msi_close_off.closingmonth)= $cyear"; 
 
 $borowing = sql::sql_run($effecborrow);
 $borrowprev =  $borowing[0][0];
 

/* invoicing for the whole contract duration */ 
 
 $effecinvoice = "SELECT sum(msi_close_off.invoiced),sum(ifnull(msi_close_off.borrowed,0)),msi_contract.startdate,msi_contract.enddate,month(msi_close_off.closingmonth),year(msi_close_off.closingmonth)
 FROM msi_close_off,msi_contract 
  
 WHERE msi_close_off.client  = \"$result[1]\"
 AND msi_close_off.client = msi_contract.client

 and (('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
 and date_format( msi_contract.enddate,'%Y-%m')) or
 (msi_contract.terminating='no'and '$fulldate2'>=date_format(msi_contract.startdate,'%Y-%m-00'))
 or ('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
 and date_format( msi_contract.terminatingdate,'%Y-%m') and terminating='yes') )
 AND msi_close_off.closingmonth >= msi_contract.startdate 
 AND msi_close_off.closingmonth <= '$ddate'"; 
     
 $pinvoiced = sql::sql_run($effecinvoice); 
 $inveffec= $pinvoiced[0][0];


 $invoicedcurrent = "SELECT sum(ifnull(msi_close_off.invoiced,0)),sum(ifnull(msi_close_off.writeoff,0))
                   FROM msi_close_off, msi_contract
                   WHERE msi_close_off.closingmonth like \"%$fulldate%\"
                   AND msi_close_off.client = \"$result[1]\" 
                   AND msi_contract.client = msi_close_off.client
                   AND msi_close_off.closingmonth >= msi_contract.startdate
 and (('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
 and date_format( msi_contract.enddate,'%Y-%m')) or
 (msi_contract.terminating='no'and '$fulldate2'>=date_format(msi_contract.startdate,'%Y-%m-00'))
 or ('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
 and date_format( msi_contract.terminatingdate,'%Y-%m') and terminating='yes') )";
                   

$invoiced = sql::sql_run($invoicedcurrent);   
$invcd = $invoiced[0][0];

 
$sql4= "SELECT sum(ifnull(msi_close_off.writeoff,0)),msi_close_off.closingmonth,msi_contract.startdate,msi_contract.enddate,month(msi_close_off.closingmonth),year(msi_close_off.closingmonth) 
           FROM msi_close_off,msi_contract 
           WHERE msi_close_off.client = msi_contract.client
  		   AND msi_close_off.client = \"$result[1]\"

           and (('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
           and date_format( msi_contract.enddate,'%Y-%m')) or
           (msi_contract.terminating='no'and '$fulldate2'>=date_format(msi_contract.startdate,'%Y-%m-00'))
           or ('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
           and date_format( msi_contract.terminatingdate,'%Y-%m') and terminating='yes') )

  		   AND msi_close_off.closingmonth >= msi_contract.startdate
  		   AND '$fulldate2'>=msi_close_off.closingmonth"; 

$write = sql::sql_run($sql4);
$writeoff = $write[0][0];
$ddate = sql::sql_run($condate);
$mdate = $ddate[0][0];
$ydate = $ddate[0][1];
$sdate = $ydate."-".$mdate;
$cdate = $contract_year."-".$contract_month;
$effectivenow = 0;

if ($sdate == $cdate){

$effectivenow  = $tothours - $used + $invcd + $borrowed + $available  - $writeoff;
//echo $effectivenow  ." = ".$tothours ." - ".$used ." + ". $invcd. " + ".$borrowed."<br />" ;
//echo $effectivenow  ." = ".$tothours ." - ". $used  ." + ". $invcd  ." + ".$borrowed."  +".$available ."- ". $writeoff."<br />";
$leftprev = 0;
$invcd = 0;
$extra = 0;
}

else{

$invcd = $invcd ;
$effectivenow  = $allhours - $peffective+  $inveffec + $borrowed  - $writeoff + $available;
//echo $effectivenow  ." = ".$allhours ." - ". $peffective ." + ". $inveffec  ." +". $borrowed."<br />";
//echo $effectivenow  ." = ".$allhours ." - ". $peffective ." + ". $inveffec  ." + ". $borrowed ."- ".$writeoff."<br />";
$leftprev = $leftprev ;
}
$bdate =  date('m',strtotime($cdate));
if($bdate > 10){
 $bdate = $contract_year."-".$contract_month;
 }
else {$bdate = $contract_year."-0".$contract_month;} 

if($bdate == date('Y-m')){
//$effectivenow  = $allhours - $peffective - $used + $inveffec + $borrowed;
//echo $effectivenow ." = ".$allhours ." - ". $peffective ." - ". $used ." + ". $inveffec ." + ". $borrowed."<br />";
}

   if(($used + $borrowprev) >= ($totalHours + $borrowed + $leftprev)) {
    $carried = 0;
    }
    else
    {
      $carried = $unused; 
    }

$gr = $prev + $borrowprev;
 if($gr >= $totalHours) {
 $leftprev = 0; 
 }     

$over = " (Over)";
$over_contract = "<font color='red'>$over</font>";
$effectivenow = number_format($effectivenow, 2);
if($effectivenow < 0){$effectivenow = ($effectivenow * -1)."  ".$over_contract;}  
 
  //end prev month hours   
     	$used = "<font color='".$colour."'>$used</font>";
     	$unused = "<font color='".$colour."'>$unused</font>";
     	//$borrowed = "<font color='".$colour."'>$borrowed</font>";
     	$leftprev = "<font color='".$colour."'>$leftprev</font>";
     	//$carried = "<font color='".$colour."'>$carried</font>";
     	$effective = "<font color='".$colour."'>$effectivenow</font>";
$state = $result[9];
if($state == "no"){
$state = "Active";}
else {$state = "Inactive";}

print "<tr >
              <td class=\"client\"><a href=\"?module=contract&action=termedit&id=$result[1]&identity=$result[5]&year=$contract_year&month=$contract_month\" style = \"text-decoration : none\">$result[0]</a></td>
               <td class=\"client\"  align=\"center\">$totalHours</td>
                <td class=\"client\"  align=\"center\">$used</td>
                 <td class=\"client\"  align=\"center\">$unused</td>
                  <td class=\"client\"  align=\"center\">$borrowed</td>
                   <td class=\"client\"  align=\"center\">$leftprev </td>
                    <td class=\"client\"  align=\"center\">$extra</td>";
             print " <td class=\"client\"  align=\"center\"> $carried </td>
                      <td class=\"client\"  align=\"center\">$effectivenow</td>
                       <td class=\"client\"  style=\"color:"; ?><?php print" $color\" align=\"center\">$result[3]</td>
                        <td class=\"client\"  style=\"color:"; ?><?php print" $color\" align=\"center\">$result[4]</td>
                         <td class=\"client\"  align=\"center\">$state</td>
		                  <td class=\"client\"  align=\"center\">$result[10]</td><td class=\"client\"  align=\"center\">$result[11]</td>
                           <td class=\"client\"  align=\"center\">$result[14]</td>
                          <td class=\"client\"  align=\"center\">$result[16]</td>
                            <td class=\"client\"  align=\"center\">$result[15]</td>
                  </tr>";
   }

  print '
 </form>

</table></td><td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td>';

  print "<br/>";

	print "<td valign = \"top\"><table style = \"width:400px\" class=\"client\"padding=\"none\">";
	print "<tr><span class=\"head\" align=\"right\">Fields Description</span></tr>";
	print "   <tr><td class=\"client\"><font color=\"blue\">Hours</font></td><td class=\"client\">Contracted hours to be covered per month</td></tr>
	           <tr><td class=\"client\"><font color=\"blue\">Used </font></td><td class=\"client\">Total hours that have been used in this month only</td></tr>
	            <tr><td class=\"client\"><font color=\"blue\">Unused</font></td><td class=\"client\"> Hours that have not been covered from this month only</td></tr>
                 <tr><td class=\"client\"><font color=\"blue\">Borrowed</font></td><td class=\"client\">  Hours that should be borrowed from the following month - (Settable)</td></tr>
	              <tr><td class=\"client\"><font color=\"blue\">Under Prev</font></td><td class=\"client\"> Hours that have not been covered from previous month only</td></tr>
	               <tr><td class=\"client\"><font color=\"blue\">Over Prev</font></td><td class=\"client\"> Additional hours covered in the previous month only</td></tr>
	                <tr><td class=\"client\"><font color=\"blue\">Carry over</font></td><td class=\"client\"> If there are left over hours, these hours can be carried over to the next month</td></tr>
	                 <tr><td class=\"client\"><font color=\"blue\">Effective</font></td><td class=\"client\">Hours left over in current month, taking into consideration carried over hours and borrowed from hours from next month</td></tr>
	</tr>";
    print "</table></td></tr></table>";   

}


function invoice_report ($year,$month,$sort_column=NULL){
  if(!acl::has_access($_SESSION['id'],"invoice","read")) return 1;

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
 
    /* give sort_column a default value if not set, otherwise sort according to param */
    if(!isset($sort_column))
       $sort_column = "invoice DESC";
    
    $results = sql::sql_run('SELECT
     msi_clients.company,
     msi_billing.date,
     msi_billing.bill_hrs,
     msi_billing.nonbill_hrs,
     msi_billing.description, 
     msi_billing.invoice, 
     msi_users.username, 
     msi_billing.id, 
     msi_billing.type, 
     msi_billing.invoiced_by, 
     msi_billing.online, 
     msi_billing.rate, 
     msi_billing.arrive_time, 
     msi_billing.depart_time, 
     msi_billing.after_hour, 
     msi_billing.site,
     msi_billing.quoteno 
    FROM msi_users, msi_billing, msi_clients
    WHERE msi_clients.id = msi_billing.client AND msi_billing.techie = msi_users.id 
    AND month(msi_billing.date) = '.$month.' and year(msi_billing.date)= '.$year.' 
    AND msi_billing.invoice != "0000-00-00"
    ORDER BY '.$sort_column);
/* removed exclusive contract reporting and made adhok entries viewable*/
/* this is the offending SQL code -> and upper(msi_billing.type) = \'CONTRACT\' */
    print '<table><tr><td colspan="3" align="center">';
    print "<a  href=?module=contract&action=invoice_report&year=$prev_year&month=$prev_month style=\"color: #1A53FF\">". date(F, mktime(0, 0, 0, $prev_month, 1, $prev_year)) ." $prev_year << Previous</a> ";
    print "&nbsp&nbsp";
    print " <b><a  href=?module=contract&action=invoice_report&year=$year&month=$month>". date(F, mktime(0, 0, 0, $month, 1, $year)) ." $year</a></b>";
    print "&nbsp&nbsp";   
    print " <a  href=?module=contract&action=invoice_report&year=$next_year&month=$next_month style=\"color: #1A53FF\">Next >> ". date(F, mktime(0, 0, 0, $next_month, 1, $next_year)) ." $next_year</a><p>";
    print '</td></tr></table>';

print "<span class=\"head\">Billing Invoice Table</span>
<p><b>Billing that is already invoiced</b></p>
<table class=\"client-link\">
<tr class=\"head\"><td class=\"client-link\"><a href=\"?module=contract&sort_column=company&year=$year&month=$month&action=invoice_report\">Company</a></td><td class=\"client-link\"><a href=\"?module=contract&sort_column=username&year=$year&month=$month&action=invoice_report\">Techie</a></td><td class=\"client-link\"><a href=\"?module=contract&sort_column=date&year=$year&month=$month&action=invoice_report\">Date (Work)</a></td><td class=\"client-link\">
<a href=\"?module=contract&sort_column=online&year=$year&month=$month&action=invoice_report\">Date (Online)</a></td><td class=\"client-link\"><a href=\"?module=contract&sort_column=bill_hrs&year=$year&month=$month&action=invoice_report\">Bill Hours</a></td><td class=\"client-link\"><a href=\"?module=contract&sort_column=nonbill_hrs&year=$year&month=$month&action=invoice_report\">Non Bill Hours</a></td>
<td class=\"client-link\"><a href=\"?module=contract&sort_column=arrive&year=$year&month=$month&action=invoice_report\">Arrival Time</a></td>
<td class=\"client-link\"><a href=\"?module=contract&sort_column=depart&year=$year&month=$month&action=invoice_report\">Departure Time</a></td>
<td class=\"client-link\"><a href=\"?module=contract&sort_column=type&year=$year&month=$month&action=invoice_report\">Type</a></td>
<td class=\"client-link\"><a href=\"?module=contract&sort_column=type&year=$year&month=$month&action=invoice_report\">Quote No</a></td>
<td class=\"client-link\"><a href=\"?module=contract&sort_column=rate&year=$year&month=$month&action=invoice_report\">Rate</a></td>
<td class=\"client-link\"><a href=\"?module=billing&sort_column=rate&year=$year&month=$month&action=invoice_report\">Location</a></td>
<td class=\"client-link\"><a href=\"?module=billing&sort_column=rate&year=$year&month=$month&action=invoice_report\">Time</a></td>
<td class=\"client-link\"><a href=\"?module=billing&sort_column=description&year=$year&month=$month&action=invoice_report\">Description</a></td>
<td class=\"client-link\"><a href=\"?module=contract&sort_column=invoice&year=$year&month=$month&action=invoice_report\">Invoice</a></td>
<td class=\"client-link\"><a href=\"?module=contract&sort_column=company&year=$year&month=$month&action=invoice_report\">edit</a></td>
</tr>";  
if(is_array($results)){
    foreach ($results as $result) {
                     print "<tr>
                              <td class=\"client\">$result[0]</td>
                              <td class=\"client\">$result[6]</td>
                              <td class=\"client\">$result[1]</td>
                              <td class=\"client\">$result[10]</td>
                              <td class=\"client\">$result[2]</td>
                              <td class=\"client\">$result[3]</td>
                              <td class=\"client\">$result[12]</td>
			                  <td class=\"client\">$result[13]</td>
                              <td class=\"client\">$result[8]</td>
                              <td class=\"client\">$result[16]</td>
			      			  <td class=\"client\">$result[11]</td>
			                  <td class=\"client\">$result[15]</td>
			                  <td class=\"client\">$result[14]</td>
                              <td class=\"client\">$result[4]</td>";
                   if ($result[5] == "0000-00-00"){
                        print "<td class=\"client\"><a href=\"?module=contract&action=invoice_confirm&id=$result[7]\">$result[5]</a></td>";
	               } else {
                        print "<td class=\"client\">$result[5] (".billing::username($result[9]).")</td>";
		               }
		                print "<td class=\"client\"><a href=\"?module=billing&action=billing_edit&id=$result[7]\">Edit</a></td>";
	               }
         	     }
        print "</table>";
              }





function uninvoiced($year,$month,$sort_column = NULL){
 if(!acl::has_access($_SESSION['id'],"invoice","read")) return 1;

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
 
if(!isset($sort_column)){
    $results = sql::sql_run('SELECT msi_clients.company,
    		 msi_billing.date,
    		 msi_billing.bill_hrs,
    		 msi_billing.nonbill_hrs,
    		 msi_billing.description,
    		 msi_billing.invoice,
    		 msi_users.username, 
    		 msi_billing.id, 
    	     msi_billing.type,
    		 msi_billing.invoiced_by ,
    		 msi_billing.online,
    		 msi_billing.rate,
    		 msi_billing.arrive_time,
    		 msi_billing.depart_time,
    	     msi_billing.site,
    		 msi_billing.after_hour,
    		 msi_billing.quoteno,
    		msi_billing.bill_ready
    FROM msi_users, msi_billing, msi_clients
    WHERE msi_clients.id = msi_billing.client AND msi_billing.techie = msi_users.id
    AND INVOICE = "0000-00-00"
   ORDER BY `date` DESC');
  //  ORDER BY `invoice` ASC');
}else{
 $results = sql::sql_run('SELECT msi_clients.company, msi_billing.date, msi_billing.bill_hrs, msi_billing.nonbill_hrs, msi_billing.description, msi_billing.invoice
    , msi_users.username, msi_billing.id, msi_billing.type, msi_billing.invoiced_by , msi_billing.online, msi_billing.rate, msi_billing.arrive_time, msi_billing.depart_time,msi_billing.site,msi_billing.after_hour,
 		msi_billing.quoteno,msi_billing.bill_ready
    FROM msi_users, msi_billing, msi_clients
    WHERE msi_clients.id = msi_billing.client AND msi_billing.techie = msi_users.id
    AND INVOICE = "0000-00-00"
    ORDER BY \''.$sort_column.'\'');
}

print "<span class=\"head\">Still to Be Invoiced</span>
    		<p><b>Billing that still needs to be invoiced</b></p>
             <table class=\"client-link\">
                 <tr class=\"head\">
		         <td class=\"client-link\"><a href=\"?module=contract&sort_column=company&year=$year&month=$month&action=uninvoiced\">Company</a></td>
		         <td class=\"client-link\"><a href=\"?module=contract&sort_column=techie&year=$year&month=$month&action=uninvoiced\">Techie</a></td>
		         <td class=\"client-link\"><a href=\"?module=contract&sort_column=date&year=$year&month=$month&action=uninvoiced\">Date (Work)</a></td>
		         <td class=\"client-link\"><a href=\"?module=contract&sort_column=online&year=$year&month=$month&action=uninvoiced\">Date (Online)</a></td>
		         <td class=\"client-link\"><a href=\"?module=contract&sort_column=bill_hrs&year=$year&month=$month&action=uninvoiced\">Bill Hours</a></td>
		         <td class=\"client-link\"><a href=\"?module=contract&sort_column=nonbill_hrs&year=$year&month=$month&action=uninvoiced\">Non Bill Hours</a></td>
		         <td class=\"client-link\"><a href=\"?module=contract&sort_column=arrive_time&year=$year&month=$month&action=uninvoiced\">Arrival Time</a></td>
		         <td class=\"client-link\"><a href=\"?module=contract&sort_column=depart_time&year=$year&month=$month&action=uninvoiced\">Departure Time</a></td>
		         <td class=\"client-link\"><a href=\"?module=contract&sort_column=type&year=$year&month=$month&action=uninvoiced\">Type</a></td>
		         <td class=\"client-link\"><a href=\"?module=contract&sort_column=quoteno&year=$year&month=$month&action=uninvoiced\">Quote No</a></td>
		         <td class=\"client-link\"><a href=\"?module=contract&sort_column=rate&year=$year&month=$month&action=uninvoiced\">Rate</a></td>
		         <td class=\"client-link\"><a href=\"?module=contract&sort_column=description&year=$year&month=$month&action=uninvoiced\">Location</a></td>
		         <td class=\"client-link\"><a href=\"?module=contract&sort_column=description&year=$year&month=$month&action=uninvoiced\">Time</a></td>
		         <td class=\"client-link\"><a href=\"?module=contract&sort_column=description&year=$year&month=$month&action=uninvoiced\">Description</a></td>
		         <td class=\"client-link\"><a href=\"?module=contract&sort_column=invoice&year=$year&month=$month&action=uninvoiced\">Ready</a></td>
			     <td class=\"client-link\"><a href=\"?module=contract&sort_column=invoice&year=$year&month=$month&action=uninvoiced\">Invoice</a></td>
        		 <td class=\"client-link\"><a href=\"?module=contract&sort_column=company&year=$year&month=$month&action=uninvoiced\">edit</a></td></tr>";
                 foreach ($results as $result) {
		         print "<tr>
  		          <td class=\"client\">$result[0]</td>
                  <td class=\"client\">$result[6]</td>
                  <td class=\"client\">$result[1]</td>
                  <td class=\"client\">$result[10]</td>
                  <td class=\"client\">$result[2]</td>
                  <td class=\"client\">$result[3]</td>
                  <td class=\"client\">$result[12]</td>
			      <td class=\"client\">$result[13]</td>
                  <td class=\"client\">$result[8]</td>
                  <td class=\"client\">$result[16]</td>
                  <td class=\"client\">$result[11]</td>
                  <td class=\"client\">$result[14]</td>
                  <td class=\"client\">$result[15]</td>
                  <td class=\"client\">$result[4]</td>
		          <td class=\"client\">".ucfirst($result[17])."</td>";
 
		        if ($result[5] == "0000-00-00" && $result[17] == "yes"){
		         	print "<td class=\"client\"><a href=\"?module=contract&action=invoice_confirm&id=$result[7]\">$result[5]</a></td>";
		          } else {
		          	print "<td class=\"client\">$result[5]</td>";
		        	
		       }
		         print "<td class=\"client\"><a href=\"?module=billing&action=billing_edit&id=$result[7]\">Edit</a></td>";
		         }		     
		         print "</table>";

}







function add_form(){
    $months = array(1 => 'January', 2 => 'February', 3 => 'March', 4 => 'April', 5 => 'May', 6 => 'June', 7 => 'July', 8 => 'August', 9 => 'September', 10 => 'October', 11 => 'November', 12 => 'December');
$thisyear 		= date('Y');
$thismonth 		= date('F');
$thisday 		= date('j');
$today 			= $thisday . " " . $thismonth . " " . $thisyear;
$expirydate 	= strtotime("+ 10 days");
$expiryyear 	= strftime('%Y',$expirydate);
$expirymonth 	= strftime('%B',$expirydate);
$expiryday 		= strftime('%d',$expirydate);
    
  if(!acl::has_access($_SESSION['id'],"contracts","add")) return 1;
 $results = sql::sql_run("SELECT DISTINCT company, id
    FROM `msi_clients`
    WHERE customer = \"1\"
    GROUP BY `company`");

	$results3=sql::sql_run("SELECT username FROM msi_users WHERE activetechie = 'yes' order by `username`");
   print "<span class=\"head\">New Contract Hours</span>
   		 <p><b>Adding new contract hours</b></p>
   
   
   <table><tr><td>
            <table class=\"box\">
<form action=\"index.php?module=contract&action=add\" name=\"contadd\"method=\"POST\">
   		
   		
   		<tr>
        <td align=\"right\"><b>Maintenance Contract:</b></td>
        <td><input type=\"checkbox\" name=\"m_hours\" id = \"m_hours\" onclick=\"showhide()\";></td><td><i>Tick if adding a new maintenance contract</i></td>
	</tr>
 <tr>
              <td align=\"right\"><b>Company Name:</b>
               </td>
                 <td>
<select name=\"company\">
                  <option></option>";
   ?>

<script type = "text/javascript">
function showhide()
{
  if (document.getElementById('m_hours').checked) 
  {
	  document.getElementById("deduct").style.visibility = "hidden";
	  document.getElementById("m_type").style.visibility = "visible";
  }

  else {
	document.getElementById("deduct").style.visibility = "visible";
	document.getElementById("m_type").style.visibility = "hidden";
  }
  
}
</script>


<?php 

//<input type="checkbox" id="xxx" name="xxx" onclick="calc();"/>
foreach ($results as $result) {
                   print "<option value=\"$result[1]\">$result[0]</option>";
                 };
      print "
                 </select>
	</tr>
 <tr>
        <td align=\"right\"><b>Contract Hours Per Month:</b></td>
        <td><input name=\"hours\" value=\"0\" size='9px'></td>
	</tr>
      		
	<tr>
			<td align=\"right\"><b>Server Hours:</b></td>
			<td><input name=\"server_hours\" value=\"".$results2[0][15]."\" size='9px'></td>
		</tr>
		<tr>
			<td align=\"right\"><b>Percentage:</b></td>
			<td><input name=\"notification\" value=\"0\" size='9px'> <i>To notify customer when this percentage is reached</i></td>
		</tr>
  	<tr id= \"m_type\" style = \" visibility:hidden \" >
			<td align=\"right\"><b>Maintenance Type:</b></td>
			<td>
		
        <input name=\"m_type\"  value=\"".$results2[0][15]."\" size='65px'></td>
		</tr>         		

		<tr id = \"deduct\">
              <td align=\"right\"><b>Add/Deduct hours from contract: </b>
               </td>
                 <td>
<select name=\"subtadd\" id = \"subtadd\">

					 <option>Add</option>
					 <option>Deduct</option>

                 </select> 
					<input name=\"adddeduct\" value=\"0\" size='9px' >
               </td>
	</tr>
        <tr>
        <td align=\"right\"><b>Start Date:</b></td>
        <td>";
	print "
<select name=\"month2\">";
	foreach ($months as $num => $month_name) {
		print '<option value="' . $num . '"';
		if ($month_name == $thismonth) { print " selected"; }
		print ">" . $month_name . "</option>\n";
	
}
	print "</select>\n";  
    print '<select name="year2">';
	for ($year = date('Y') - 4, $max_year = date('Y') + 4; $year < $max_year; $year++) {
	
		print '<option value="' . $year . '"';
		if ($year == $thisyear) { print " selected"; }
		print ">" . $year . "</option>\n";
	}
	print "</select>\n
</td>
	</tr>
<tr>
        <td align=\"right\"><b>End Date:</b></td>
        <td>";

	print '<select name="month3">';
	foreach ($months as $num => $month_name) {
		print '<option value="' . $num . '"';
		if ($month_name == $expirymonth) { print " selected"; }
		print ">" . $month_name . "</option>\n";
	}
	print "</select>\n";
    print '<select name="year3">';
	for ($year = date('Y') - 4, $max_year = date('Y') + 4; $year < $max_year; $year++) {
		print '<option value="' . $year . '"';
		if ($year == $thisyear) { print " selected"; }
		print ">" . $year . "</option>\n";
	}
	print "</select>\n
     </td></tr>
      </tr>
<tr>
	<td align=\"right\"><b>Primary Technician:</b> </td>
	<td colspan=\"2\" align=\"left\"><select name=\"primary\">";
		   print "<option value=''></option>";
foreach ($results3 as $result3) {
                   print "<option value=\"$result3[0]\">$result3[0]</option>";
                 };

print "	</select></td>
</tr>
<tr>
	<td align=\"right\"><b>Backup Technician:</b> </td>
	<td colspan=\"2\" align=\"left\"><select name=\"backup\">";
		   print "<option value=''></option>";
foreach ($results3 as $result3) {
                   print "<option value=\"$result3[0]\">$result3[0]</option>";
                 };

print "	</select></td>

</tr>
<tr>
	<td align=\"right\"><b>Site Manager :</b> </td>
	<td colspan=\"2\" align=\"left\"><select name=\"server_techie\">";
		   print "<option value=''></option>";
foreach ($results3 as $result3) {
                   print "<option value=\"$result3[0]\">$result3[0]</option>";
                 };

print "	</select></td></tr>";

print "	</select></td>

</tr>
<tr>
	<td align=\"right\"><b>Account Manager :</b> </td>
	<td colspan=\"2\" align=\"left\"><select name=\"acc_manager\">";
print "<option value=''></option>";
foreach ($results3 as $result3) {
	print "<option value=\"$result3[0]\">$result3[0]</option>";
};


print "	</select></td></tr>";
 /* Sandiswa disabled email function for next version    
  * 
  * 
  * 
  *  print "  <tr>
        <td align=\"right\"><b>Weekly Email:</b></td>
        <td><input type=\"checkbox\" name=\"email\"  value=\" \" checked = \"checked\"></td>
	</tr>
 <tr>
        <td align=\"right\"><b>Email Address:</b></td>
        <td><input name=\"email1\" value=\" \" size='30px'></td>
	</tr>
	 <tr>
        <td align=\"right\"><b>Email Address:</b></td>
        <td><input name=\"email2\" value=\" \" size='30px'></td>
	</tr>
	 <tr>
        <td align=\"right\"><b>Email Address:</b></td>
        <td><input name=\"email3\" value=\" \" size='30px'></td>
	</tr>";*/
   
   print "
<tr>
	<td colspan=\"2\" align=\"center\">
       	<input type=\"submit\" value=\"Submit\" class=\"buttons\">
       </td>
</tr>
</form>
</table>";
  
}

function listall($contract_month,$contract_year){

	if(!acl::has_access($_SESSION['id'],"contracts","read")) return 1;

	//beginning of new code, 2008/06/06 making contracts month aware.
	if(!isset($contract_month)&&!isset($contract_year)) {

		$contract_year = date(Y);
		$contract_month = date(n);

	}

	$next_month = $contract_month +1;

	if ($next_month > 12) {
		$next_month = 1;
		$next_year = $contract_year +1;
	} else {
		$next_year = $contract_year;
	}

	$prev_month = $contract_month - 1;
	if ($prev_month == 0){
		$prev_month = 12;
		$prev_year= $contract_year -1;
	}
	// elseif ($prev_month < 10){
	// $prev_month = '0'.$prev_month;
	// }
	else {
		$prev_year= $contract_year;
	}
	//add leading 0 for months less than 10
	if ($contract_month < 10){
		$strmonth = '0'.$contract_month;
	} else {
		$strmonth = $contract_month;
	}

	//end add leading
	$moncalc=$contract_year.'-'.$strmonth.'-'.'01';
	$fulldate=$contract_year.'-'.$strmonth;
	$fulldate2=$contract_year.'-'.$strmonth.'-'.'00';
	$fulldateprev= $prev_year.'-'.$prev_month;
	$fulldateprev2=$prev_year.'-'.$prev_month.'-'.'00';

	$totalhours = sql::sql_run("SELECT 
			SUM(msi_contract.hours), count(*) AS counter
		FROM 
			msi_contract,
			msi_clients 
		WHERE
			msi_clients.id = msi_contract.client
		AND
		(
			(
				'$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
				AND date_format( msi_contract.enddate,'%Y-%m')
			) OR (
				msi_contract.terminating='no' 
				AND '$fulldate2'>=date_format(msi_contract.startdate,'%Y-%m-00')
			) OR (
				(
					'$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
					AND date_format( msi_contract.terminatingdate,'%Y-%m')
				) AND msi_contract.terminating='yes'
			)
		)
	");

	$results = sql::sql_run("SELECT 
		msi_clients.company,
		msi_contract.client,
		msi_contract.hours,
		date_format(msi_contract.startdate,'%b-%Y'),
		date_format(msi_contract.enddate,'%b-%Y'),
		msi_contract.contid,
		datediff(msi_contract.enddate,now()),
		msi_contract.enddate,
		month(msi_contract.enddate),
		month(now()),
		msi_contract.primarytech,
		msi_contract.backuptech,
		msi_contract.server_hours,
		msi_contract.server_techie,
		msi_contract.effective,
		msi_contract.notes
		
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
	ORDER BY
		msi_clients.company ASC " );


	print   "<span class=\"head\">Contract List</span>
		<p><b>Listing all active contracts. To close off the the month click on the customer name</b></p>";
	print '<table><tr><td colspan="3" align="left">';
	print "<a href=?module=contract&contract_year=$prev_year&contract_month=$prev_month style=\"color: #1A53FF\">". date(F, mktime(0, 0, 0, $prev_month, 1, $prev_year)) ." $prev_year << Previous</a> ";
	print "&nbsp&nbsp";
	print " <b><a href=?module=contract&year=$contract_year&month=$contract_month>". date(F, mktime(0, 0, 0, $contract_month, 1, $contract_year)) ." $contract_year</a></b>";
	print "&nbsp&nbsp";  
	print " <a href=?module=contract&contract_year=$next_year&contract_month=$next_month style=\"color: #1A53FF\">Next >> ". date(F, mktime(0, 0, 0, $next_month, 1, $next_year)) ." $next_year</a><p>";
	print '</td></tr>
	<tr><td valign=top>';
	//beginning of new code
	//ending of new code
	print "<span class=\"head\">Client Contracts</span>";
	print "Total contracted hours: ".$totalhours[0][0]." in ".$totalhours[0][1]." contracts<br>";

	print "<table><tr><td>";
	print"<table class=\"client\">
				<tr class=\"head\">
	<td>Client</td>
	<td title =\"Contracted hours to be covered per month\" style=\"border-left: 1px solid #FFFFFF\" align=\"center\">Total Hours</td>";
	//  <td style=\"border-left: 1px solid #FFFFFF\" align=\"center\">Support Hours</td>
	// <td style=\"border-left: 1px solid #FFFFFF\" align=\"center\">Server Hours</td>
	print"<td title =\"Total hours that have been used in this month only\" style=\"border-left: 1px solid #FFFFFF\">&nbsp;Used&nbsp;</td> 
			<td title =\"Hours that have not been covered from this month only\" style=\"border-left: 1px solid #FFFFFF\">&nbsp;Unused&nbsp; </td> 
			<td title =\"Hours that should be borrowed from the following month\" style=\"border-left: 1px solid #FFFFFF\">Borrowed</td> 
			<td title =\"Hours that have not been covered from previous month only\" style=\"border-left: 1px solid #FFFFFF\">Under Prev</td> 
			<td title =\"Additional hours covered in the previous month only\" style=\"border-left: 1px solid #FFFFFF\">Over Prev</td> 
			<td title =\"Hours to be carried over to the next month\" style=\"border-left: 1px solid #FFFFFF\">Carry Over</td> 
			<td title =\"Hours left over in current month, taking into consideration carried over hours from previous month
	and borrowed from hours from next month\" style=\"border-left: 1px solid #FFFFFF\">Effective </td>
			<td>&nbsp;Closed&nbsp;</td>
			<td style=\"border-left: 1px solid #FFFFFF\">&nbsp;Export&nbsp;</td>
			<td style=\"border-left: 1px solid #FFFFFF\">&nbsp;Notes&nbsp;</td>
	</tr>";

	foreach($results as $result) {

		$dateman=$result[7];
		$dateim=explode("-",$dateman);
		$numdays=date("t", strtotime($dateim[0]. "-" . $dateim[1]. "-01"));
		$newend=$dateim[0].'-'.$dateim[1].'-'.$numdays;

		if(date("m")==$strmonth and date("Y")==$contract_year) {
			$start_date=date("Y-m-d");
			$end_date=$newend;
		}
		if(date("m")>$strmonth and date("Y")==$contract_year) {
			$start_date=date("Y-m-d");
			$end_date=$newend;
		}

		if(date("Y")>$contract_year and $dateim[0]<date("Y")) {
			$start_date=date("Y-m-d");
			$end_date=$newend;
		}

		if(date("Y")>$contract_year and $dateim[0]==date("Y")) {
			$start_date=$moncalc;
			$end_date=$newend;
		}

		if(date("m")<$strmonth and date("Y")==$contract_year) {
			$start_date=$moncalc;
			$end_date=$newend;
		}

		//   echo "<br />".$start_date."   ".  $end_date."<br />";

		$daydiff=contract::calculateDurationFromDates($start_date,$end_date);


		//$daydiff=$result[6];

		if($daydiff>30 and $daydiff<=60) {
			$color="#FF8C00";
		} else if( $daydiff >= 1 AND $daydiff <= 30 ) {
			$color="#FF0000";
		} else if( $daydiff < 1 ) {
			$color='#008B8B';
		} else {
			$color='';
		}

		if(!acl::has_access($_SESSION['id'],"contracts","read")) return 1;

		$usedcurrent="SELECT ifnull(sum(bill_hrs),0)
			FROM msi_billing
			WHERE month(date) = $contract_month and year(date)= $contract_year
			AND client = \"$result[1]\" and upper(type) = 'CONTRACT' ";
			$usedhourscur = sql::sql_run($usedcurrent);
		
		$sql = "SELECT ifnull(sum(msi_billing.bill_hrs),0), ifnull(msi_close_off.carried,0), ifnull(msi_close_off.borrowed,0),msi_close_off.closed 
			FROM msi_close_off
			LEFT OUTER JOIN msi_billing
			ON msi_close_off.client =  msi_billing.client
			where month(msi_close_off.closingmonth) = $contract_month 
			AND year(msi_close_off.closingmonth)= $contract_year
			AND month(msi_billing.date) = $contract_month  
			AND year(msi_billing.date)= $contract_year
			AND msi_billing.client = \"$result[1]\" ";

		// and upper(msi_billing.type) = 'CONTRACT'
		// echo $sql."<br />";
		$usedhours = sql::sql_run($sql);
		$used =  $usedhourscur[0][0]; 
		$carried = $usedhours[0][1]; 
		$borrowed = $usedhours[0][2]; 
		// $status = $usedhours[0][3];

		//  if($usedhours){
		//	$status = 'Yes';
		// }
		//  if($status == 'Yes') {
		// $status = 'Yes';}
		// else { $status = 'No';}
		//  }

		$state="SELECT msi_close_off.closed
			FROM msi_close_off
			where month(msi_close_off.closingmonth) = $contract_month
			and year(msi_close_off.closingmonth)= $contract_year
			AND msi_close_off.client = \"$result[1]\" ";

		$sts = sql::sql_run($state);

		$status = $sts[0][0];

		// if($usedhours){
			//$status = 'Yes';
		// }

		if($status == 'Yes') {
			$status = 'Yes';
		} else {
			$status = 'No';
		}

		$totalServerHours = $result[12];
		$totalSupportHours = $result[2];
		$totalHours = $totalServerHours + $totalSupportHours;
		$unused = $totalHours -  $used;

		if ($unused < 0){
			$unused = 0;
		}

		$available = $result[14];

		//calculate effective hours
		$total = "SELECT date_format(msi_contract.startdate,'%Y-%m-%01') 
			from msi_contract, msi_clients 
			where msi_clients.id = msi_contract.client
			and msi_contract.client = \"$result[1]\"
			and (('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
			and date_format( msi_contract.enddate,'%Y-%m')) or
			(msi_contract.terminating='no'and '$fulldate2'>=date_format(msi_contract.startdate,'%Y-%m-00'))
			or ('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
			and date_format( msi_contract.terminatingdate,'%Y-%m') and terminating='yes') )
			order by msi_clients.company asc ";

		$contract = sql::sql_run($total);
		$contract_date = $contract[0][0];
		$num = cal_days_in_month(CAL_GREGORIAN, $contract_month, $contract_year); // 31
		$ddate=$contract_year.'-'.$strmonth.'-'.$num;
		$date1 = date(strtotime($contract_date));
		$date2 = date(strtotime($ddate));
		$difference = $date2 - $date1;
		$month = floor($difference / 86400 / 30 )+ 1;
		$months = round($difference / 86400 / 30 );
		$contracttime  = $contract_date ;
		$currentdate = $contract_year."-".$contract_month."-".$num;
		$d1 = strtotime($contracttime );
		$d2 = strtotime($currentdate);
		$min_date = min($d1, $d2);
		$max_date = max($d1, $d2);
		$t = 1;
		while (($min_date = strtotime("+1 MONTH", $min_date)) <=  $max_date) {
			$t++; 
		} 

		/*

		$prevtothrs ="SELECT (ifnull(msi_contract.hours,0) + ifnull(msi_contract.server_hours,0))
		from msi_contract, msi_clients
		where msi_clients.id = msi_contract.client
		and msi_contract.client = \"$result[1]\"
		and (('$fulldateprev' between date_format( msi_contract.startdate,'%Y-%m')
		and date_format( msi_contract.enddate,'%Y-%m')) or
		(msi_contract.terminating='no'and '$fulldateprev2'>=date_format(msi_contract.startdate,'%Y-%m-00'))
		or ('$fulldateprev' between date_format( msi_contract.startdate,'%Y-%m')
		and date_format( msi_contract.terminatingdate,'%Y-%m') and terminating='yes') )
		order by msi_clients.company asc ";

		$hoursprev =  sql::sql_run($prevtothrs);

		*/

		$tothrs ="SELECT (ifnull(msi_contract.hours,0) + ifnull(msi_contract.server_hours,0))
			from msi_contract, msi_clients 
			where msi_clients.id = msi_contract.client
			and msi_contract.client = \"$result[1]\"
			and (('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
			and date_format( msi_contract.enddate,'%Y-%m')) or
			(msi_contract.terminating='no'and '$fulldate2'>=date_format(msi_contract.startdate,'%Y-%m-00'))
			or ('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
			and date_format( msi_contract.terminatingdate,'%Y-%m') and terminating='yes') )
			order by msi_clients.company asc ";

		$hours =  sql::sql_run($tothrs);
		$allhours = $hours[0][0] * $t;
		$tothours = $hours[0][0] ;

		//calculate hours from prev month
		$m = $contract_month-1;
		if($m<10){$m = '0'.$m;}
		else {$m =$m;}
		if($m == '0'){ $m = 12;}
		if($m == '12'){ $cyear = $contract_year-1;}else{ $cyear = $contract_year;}

		//$sql2="SELECT ifnull(sum(msi_billing.bill_hrs),0)
		//  FROM msi_billing
		//  WHERE month(msi_billing.date)= $m  and year(msi_billing.date)= $cyear
		// AND msi_billing.client = \"$result[1]\" and upper(msi_billing.type) = 'CONTRACT' ";

		$sql2="SELECT ifnull(sum(msi_billing.bill_hrs),0),ifnull(sum(msi_close_off.extra),0),ifnull(msi_close_off.borrowed,0)
			FROM msi_billing,msi_close_off
			WHERE month( msi_billing.date)= $m  and year( msi_billing.date)= $cyear
			AND month(msi_close_off.closingmonth)= $m  and year(msi_close_off.closingmonth)= $cyear
			AND msi_billing.client = msi_close_off.client
			AND msi_billing.client = \"$result[1]\" and upper(msi_billing.type) = 'CONTRACT' "; 

		$sql3= "SELECT ifnull(sum(msi_close_off.extra),0),ifnull(msi_close_off.borrowed,0),ifnull(msi_close_off.writeoff,0)
			FROM msi_close_off
			WHERE month(msi_close_off.closingmonth) = $m
			AND year(msi_close_off.closingmonth)= $cyear
			AND msi_close_off.client = \"$result[1]\" "; 


		$sql4= "SELECT sum(ifnull(msi_close_off.writeoff,0)),msi_close_off.closingmonth,msi_contract.startdate,msi_contract.enddate,month(msi_close_off.closingmonth),year(msi_close_off.closingmonth) 
			FROM msi_close_off,msi_contract 
			WHERE msi_close_off.client = msi_contract.client
			AND msi_close_off.client = \"$result[1]\"

			and (('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
			and date_format( msi_contract.enddate,'%Y-%m')) or
			(msi_contract.terminating='no'and '$fulldate2'>=date_format(msi_contract.startdate,'%Y-%m-00'))
			or ('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
			and date_format( msi_contract.terminatingdate,'%Y-%m') and terminating='yes') )
			AND msi_close_off.closingmonth >= msi_contract.startdate
			and '$fulldate2'>=msi_close_off.closingmonth ";
		//    AND msi_close_off.closingmonth = \"$fulldate2\""; 
		// echo $sql4."<br /><br />";

		$usedprev = sql::sql_run($sql2);
		$previous = sql::sql_run($sql3);
		$prev =  $usedprev[0][0];
		$extra = $previous[0][0];
		
		$write = sql::sql_run($sql4);
		$writeoff = $write[0][0];

		$leftprev = $totalHours - $prev;

		if ($leftprev < 0 ){
			$extra = $leftprev*-(1);
			$leftprev = 0;
		}
		else {
			$extra = $extra;
		}

		// echo $sql4."<br />";
		//echo $writeoff."<br />";

		$effecborrow = "SELECT sum(ifnull(msi_close_off.borrowed,0)),msi_close_off.closingmonth,msi_contract.startdate,msi_contract.enddate,month(msi_close_off.closingmonth),year(msi_close_off.closingmonth)
		FROM msi_close_off,msi_contract 
		
		WHERE msi_close_off.client  = \"$result[1]\"
		AND msi_close_off.client = msi_contract.client

		and (('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
		and date_format( msi_contract.enddate,'%Y-%m')) or
		(msi_contract.terminating='no'and '$fulldate2'>=date_format(msi_contract.startdate,'%Y-%m-00'))
		or ('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
		and date_format( msi_contract.terminatingdate,'%Y-%m') and terminating='yes') )
		
		AND msi_close_off.closingmonth >= msi_contract.startdate 
		and month(msi_close_off.closingmonth) = $m
		AND year(msi_close_off.closingmonth)= $cyear"; 

		/* invoicing for the whole contract duration */ 
 
		$effecinvoice = "SELECT sum(msi_close_off.invoiced),sum(ifnull(msi_close_off.borrowed,0)),msi_contract.startdate,msi_contract.enddate,month(msi_close_off.closingmonth),year(msi_close_off.closingmonth)
		FROM msi_close_off,msi_contract 

		WHERE msi_close_off.client  = \"$result[1]\"
		AND msi_close_off.client = msi_contract.client

		and (('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
		and date_format( msi_contract.enddate,'%Y-%m')) or
		(msi_contract.terminating='no'and '$fulldate2'>=date_format(msi_contract.startdate,'%Y-%m-00'))
		or ('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
		and date_format( msi_contract.terminatingdate,'%Y-%m') and terminating='yes') )

		AND msi_close_off.closingmonth >= msi_contract.startdate 
		AND msi_close_off.closingmonth <= '$ddate'";  

		//echo $effecinvoice."<br/>";

		$effecthours ="SELECT ifnull(sum(msi_billing.bill_hrs),0),msi_billing.date, date_format( msi_contract.startdate,'%Y-%m'), msi_contract.enddate

		FROM msi_contract,msi_clients,msi_billing

		WHERE msi_clients.id = msi_contract.client 
		AND msi_billing.client = msi_contract.client 
		AND msi_billing.date >= msi_contract.startdate 
		AND msi_billing.date <= '$ddate' 
		AND msi_billing.client = \"$result[1]\" and upper(msi_billing.type) = 'CONTRACT'
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
			msi_contract.terminating='yes'))";

		//AND msi_contract.terminating = 'no' 
		//AND msi_clients.customer = 1 "; 

		//echo $effecthours."<br />";
		$efehours = sql::sql_run($effecthours); 
		$peffective = $efehours[0][0];
		$pinvoiced = sql::sql_run($effecinvoice); 
		$inveffec= $pinvoiced[0][0];
		$borowing = sql::sql_run($effecborrow); 
		$borrowprev =  $borowing[0][0];

		$invoicedcurrent = "SELECT sum(ifnull(msi_close_off.invoiced,0))
		FROM msi_close_off, msi_contract
		WHERE msi_close_off.closingmonth like \"%$fulldate%\"
		AND msi_close_off.client = \"$result[1]\" 
		AND msi_contract.client = msi_close_off.client
		AND msi_close_off.closingmonth >= msi_contract.startdate
		and (('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
		and date_format( msi_contract.enddate,'%Y-%m')) or
		(msi_contract.terminating='no'and '$fulldate2'>=date_format(msi_contract.startdate,'%Y-%m-00'))
		or ('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
		and date_format( msi_contract.terminatingdate,'%Y-%m') and terminating='yes') )";


		//echo $invoicedcurrent."<br />".

		$invoiced = sql::sql_run($invoicedcurrent);   
		$invcd = $invoiced[0][0];

		//echo $result[1]."<br />".$invcd."<br />";
		$condate= "SELECT month(msi_contract.startdate), year(msi_contract.startdate)
			FROM
			msi_contract
			WHERE 
			msi_contract.client = \"$result[1]\"
			and terminating = 'no' 
		"; 

		$ddate = sql::sql_run($condate);
		$mdate = $ddate[0][0];
		$ydate = $ddate[0][1];
		$sdate = $ydate."-".$mdate;
		$cdate = $contract_year."-".$contract_month;

		$effectivenow = 0;

if ($sdate == $cdate){

$effectivenow  = $tothours - $used + $invcd + $borrowed - $writeoff + $available;
//$effectivenow  = $tothours - $used + $invcd + $borrowed + $available  - $writeoff;
//echo $tothours ." - ". $used  ." + ". $invcd  ." +".$available ."- ". $writeoff."<br />";
$leftprev = 0;
$extra = 0;
}

else{

$effectivenow  = $allhours - $peffective +  $inveffec + $borrowed - $writeoff + $available ;
//$effectivenow  = $allhours - $peffective +  $inveffec + $borrowed - $writeoff  + $available ;
//echo $allhours ." - ". $peffective ." + ". $inveffec  ." + ". $borrowed ."- ".$writeoff + $available ."<br />";
$leftprev = $leftprev ;
}
$bdate =  date('m',strtotime($cdate));
if($bdate > 10){
$bdate = $contract_year."-".$contract_month;
 }
else {$bdate = $contract_year."-0".$contract_month;} 
if($bdate == date('Y-m')){
//$effectivenow  = $allhours - $peffective - $used + $inveffec  + $borrowed - $writeoff  + $available ;

}
   if(($used + $borrowed +$borrowprev ) >= $totalHours) {
    $carried = 0;
    }
    else
    {
    $carried = $unused; 
    }
$gr = $prev + $borrowprev;
 if($gr >= $totalHours) {
 $leftprev = 0;
  }
  $effectivenow = number_format($effectivenow, 2);
$over = " (Over)";
$over_contract = "<font color='red'>$over</font>";
if($effectivenow < 0){$effectivenow = ($effectivenow * -1)."  ".$over_contract;}  
if($_GET['contract_month']== date(n)) {$myday=date(d);}
else {$myday=30;}
 
	$deviance=($used*100/$totalHours)-($myday*100/30) ;
			if($deviance > 20) { 
				$colour='blue';
			}else if($deviance < (-20)) {
				$colour='red';
			}else{
				$colour='black';
			}

//end prev month hours 

			if($status == 'Yes'){
				$colour='red';
			}else{
				$colour='black';
			}
			
     	$used = "<font color='".$colour."'>$used</font>";
     	$unused = "<font color='".$colour."'>$unused</font>";
     	$leftprev = "<font color='".$colour."'>$leftprev</font>";
     	$effectivenow = "<font color='".$colour."'>$effectivenow</font>";
     	$status = "<font color='".$colour."'>$status</font>";
     print "<tr >
              <td class=\"client\" ><a href=\"?module=contract&action=edit_form&id=$result[1]&identity=$result[5]&year=$contract_year&month=$contract_month\" style = \"text-decoration : none\">$result[0]</a></td>
               <td class=\"client\" align=\"center\">$totalHours</td>";
       	        print"  <td class=\"client\" align=\"center\"> $used </td>
       	           <td class=\"client\" align=\"center\">$unused </td>
                    <td class=\"client\" align=\"center\">$borrowed</td>
       		         <td class=\"client\" align=\"center\">$leftprev</td>
       		          <td class=\"client\" align=\"center\">$extra</td>
       		           <td class=\"client\" align=\"center\">$carried</td>
       		            <td class=\"client\" align=\"center\">$effectivenow</td>
       		             <td class=\"client\" align=\"center\">$status</td>";
       		              print"<td class=\"client\"><a href=\"export.php?action=export_client&id=$result[1]&year=$contract_year&month=$contract_month\">(export)</a></td>    		           
    		               <td style=\"width:300px\" class=\"client\" align=\"center\">$result[15]</td>
       		                </tr>";
       		                   		            
   }
  print '</table></td><td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td>
         <td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td>';
   print "<br/>";
	print "<td valign=\"top\">";
	print "<table style = \"width:450px\" class=\"client\"padding=\"none\">";
	print "<tr><span class=\"head\"align=\"right\">Fields Description</span></tr>";
	print "<tr><td class=\"client\"><font color=\"blue\">Hours                   </font></td><td class=\"client\">Contracted hours to be covered per month</td></tr>
	        <tr><td class=\"client\"><font color=\"blue\">Used                  </font></td>  <td class=\"client\">Total hours that have been used in this month only</td></tr>
	         <tr><td class=\"client\"><font color=\"blue\">Unused              </font></td>    <td class=\"client\"> Hours that have not been covered from this month only</td></tr>
              <tr><td class=\"client\"><font color=\"blue\">Borrowed          </font></td>      <td class=\"client\">  Hours that should be borrowed from the following month - (Settable)</td></tr>
	           <tr><td class=\"client\"><font color=\"blue\">Under Prev      </font></td>        <td class=\"client\"> Hours that have not been covered from previous month only</td></tr>
	            <tr><td class=\"client\"><font color=\"blue\">Over Prev     </font></td>          <td class=\"client\"> Additional hours covered in the previous month only</td></tr>
	             <tr><td class=\"client\"><font color=\"blue\">Carry over  </font></td>            <td class=\"client\"> If there are left over hours, these hours can be carried over to the next month</td></tr>
	              <tr><td class=\"client\"><font color=\"blue\">Effective </font></td>              <td class=\"client\">Hours left over in current month, taking into consideration carried over hours and borrowed from hours from next month</td></tr>
	</tr>";
    print "</table></td></tr></table>"; 
 
}


function calculateDurationFromDates($start_date, $end_date, $offset = 0) {

	define('SECONDS_IN_DAY', 86400);
	$start_days = strtotime($start_date);
	$end_days = strtotime($end_date);

	// calculate seconds between dates and then divide between seconds in day, to give you number of days
	$days = ($end_days - $start_days) / SECONDS_IN_DAY;
	// return days plus any offset thath has been set
	return $days + $offset;

}


function edit_form($id,$identity, $contract_month, $contract_year){

	print_r('updating<br />');

	if(!isset($contract_month)&&!isset($contract_year)) {
		$contract_year =$_GET['year'];
		$contract_month =$_GET['month'];
	}
	
	$next_month = $contract_month +1;

	if ($next_month > 12) { 
		$next_month = 1;
		$next_year = $contract_year +1;
	} else {
		$next_year = $contract_year;
	}

	$prev_month = $contract_month - 1;
	if ($prev_month == 0){
		$prev_month = 12;
		$prev_year= $contract_year -1;
	} else {
		$prev_year= $contract_year;
	}

	//add leading 0 for months less than 10
	if($contract_month < 10){
		$strmonth = '0'.$contract_month;
	} else {
		$strmonth = $contract_month;
	}

	//end add leading
	$moncalc=$contract_year.'-'.$strmonth.'-'.'01';
	$fulldate=$contract_year.'-'.$strmonth;
	$fulldate2=$contract_year.'-'.$strmonth.'-'.'00';

	// $results3 = username
	$results3=sql::sql_run("SELECT username FROM msi_users WHERE activetechie = 'yes' order by `username`");

	$months = array(1 => 'January', 2 => 'February', 3 => 'March', 4 => 'April', 5 => 'May', 6 => 'June', 7 => 'July', 8 => 'August', 9 => 'September', 10 => 'October', 11 => 'November', 12 => 'December');
	$thisyear 		= date('Y');
	$thismonth 		= date('F');
	$thisday 		= date('j');
	$today 		= $thisday . " " . $thismonth . " " . $thisyear;
	$expirydate 		= strtotime("+ 10 days");
	$expiryyear 		= strftime('%Y',$expirydate);
	$expirymonth 		= strftime('%B',$expirydate);
	$expiryday 		= strftime('%d',$expirydate);

	if(!acl::has_access($_SESSION['id'],"contracts","edit")) return 1;

	$results = sql::sql_run("SELECT DISTINCT company, id
		FROM `msi_clients`
		WHERE customer = \"1\"
		GROUP BY `company`");

	$results2 = sql::sql_run("select
		msi_clients.company,
		msi_contract.client,
		msi_contract.hours,
		day(msi_contract.startdate),
		monthname(msi_contract.startdate),
		month(msi_contract.startdate),
		year(msi_contract.startdate),
		day(msi_contract.enddate),
		month(msi_contract.enddate),
		monthname(msi_contract.enddate),
		year(msi_contract.enddate),
		msi_contract.terminating,
		msi_contract.terminatingdate,
		msi_contract.primarytech,
		msi_contract.backuptech,
		ifnull(msi_contract.server_hours,0),
		msi_contract.server_techie,
		ifnull(msi_contract.effective,0),
		msi_contract.contid

		from msi_contract, msi_clients where
		msi_clients.id = msi_contract.client and
		msi_contract.client = '".$id."'


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
		ORDER BY
			msi_clients.company ASC 
		");

	$av = "SELECT ifnull(msi_close_off.available,0.00),ifnull(msi_close_off.unused,0.00)
		FROM msi_close_off
		WHERE msi_close_off.client = \"$id\"
		AND  month(msi_close_off.closingmonth) = \"$prev_month\"
		AND year(msi_close_off.closingmonth) = \"$prev_year\"";

	$avres =sql::sql_run($av);

	$mm = $results2[0][5];
	if($mm<10){$mm = '0'.$mm;}
	// else {$mm =$mm;}

	$strtdate = $results2[0][6]."-".$mm;

	if ($strtdate == $fulldate){
		$avail =  $results2[0][17];
		$prevunused = 0;
	} else {
		if(is_array($avres)) {
			$avail = $avres[0][0];
			$prevunused = $avres[0][1];
		}else {
			$prevunused = 0;
			$avail = 0;
		}
	}

	$identity = $results2[0][18];

	$carry = sql::sql_run("SELECT ifnull(msi_close_off.carried,0),
		ifnull(msi_close_off.borrowed,0),
		ifnull(msi_close_off.extra,0),
		ifnull(msi_close_off.invoiced,0),
		SUM(msi_billing.bill_hrs),
		msi_contract.hours,
		msi_contract.server_hours,
		msi_close_off.closed,
		ifnull(msi_close_off.writeoff,0)

		from msi_billing,msi_contract,msi_close_off
		where msi_billing.client = msi_contract.client
		and msi_close_off.client = msi_billing.client
		and  month(msi_close_off.closingmonth) = $contract_month 
		and year(msi_close_off.closingmonth)= $contract_year
		AND msi_billing.client = \"$id\" and upper(msi_billing.type) = 'CONTRACT' 

		and (('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
		and date_format( msi_contract.enddate,'%Y-%m')) or
		(msi_contract.terminating='no'and '$fulldate2'>=date_format(msi_contract.startdate,'%Y-%m-00'))
		or ('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
		and date_format( msi_contract.terminatingdate,'%Y-%m') and terminating='yes') )
	");

	$carry2 = sql::sql_run("SELECT ifnull(msi_close_off.writeoff,0)
		FROM msi_close_off
		WHERE month(msi_close_off.closingmonth) = \".$contract_month.\"
		AND year(msi_close_off.closingmonth)= \".$contract_year.\"
		AND msi_close_off.client = \".$id.\";
	");

	$borrowed = $carry[0][1];
	$writtenoff = $carry2[0][0];
	$carried = $carry[0][5] + $carry[0][6] - $carry[0][4];

	?>

	<script type="text/javascript" src="./js/contract.js"></script>
	<link type="text/css" rel="stylesheet" href="./css/contract.css">

	<?php

	if($carried < 0){

		$carried = 0;}

		$closed = $carry[0][7];

		$inv2 = $carry[0][3];


		if($closed == 'Yes'){

			$readonly = 'readonly';
			$background = "background-color:#C8C8C8";

		} else {

			$readonly = ''; 

			$background = "background-color:#fff";
		}

		//calculate extra hours previous month

		$previous = sql::sql_run("SELECT ifnull(sum(msi_billing.bill_hrs),0),
				ifnull(msi_contract.hours,0),
				ifnull(msi_contract.server_hours,0)
			FROM msi_billing,msi_contract
			WHERE msi_billing.client = msi_contract.client
				AND  month(msi_billing.date) = $contract_month
				AND year(msi_billing.date)= $contract_year
				AND msi_billing.client = \"$id\" and upper(msi_billing.type) = 'CONTRACT'

				AND (('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
				AND date_format( msi_contract.enddate,'%Y-%m')) or
				(msi_contract.terminating='no'and '$fulldate2'>=date_format(msi_contract.startdate,'%Y-%m-00'))
				OR ('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
				AND date_format( msi_contract.terminatingdate,'%Y-%m') and terminating='yes') );
		");

		$unused2 = sql::sql_run("SELECT ifnull( msi_contract.hours, 0 ) + ifnull( msi_contract.server_hours, 0 )
			from msi_contract
			where msi_contract.client = \"$id\"

			and (('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
			and date_format( msi_contract.enddate,'%Y-%m')) or
			(msi_contract.terminating='no'and '$fulldate2'>=date_format(msi_contract.startdate,'%Y-%m-00'))
			or ('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
			and date_format( msi_contract.terminatingdate,'%Y-%m') and terminating='yes') )
		");

		$extra = $previous[0][0] -($previous[0][2]+$previous[0][1] );

		$unused = ($previous[0][1] + $previous[0][2]) - $previous[0][0];

		if ($unused < 0 ){

			$unused = 0;

		} else if($previous[0][0] == 0 ){

			$unused =  $unused2[0][0];

		}

		if ( $extra < 0 ){
			$extra = 0;
		} else {

			$extra = $extra;
		}

		//calculate borrowed from previous month
		$m = $contract_month-1;
		if($m<10){$m = '0'.$m;}
		else {$m =$m;}
		if($m == '0'){ $m = 12;}
		if($m == '12'){ $cyear = $contract_year-1;}else{ $cyear = $contract_year;}

		$sql2="SELECT ifnull(sum(msi_billing.bill_hrs),0),ifnull(sum(msi_close_off.extra),0),ifnull(msi_close_off.borrowed,0),ifnull(msi_close_off.invoiced,0)
			FROM msi_billing,msi_close_off
			WHERE msi_billing.client = msi_close_off.client
			AND month(msi_close_off.closingmonth)= $m  and year(msi_close_off.closingmonth)= $cyear
			AND msi_billing.client = \"$id\" and upper(msi_billing.type) = 'CONTRACT' "; 

		$usedprev = sql::sql_run($sql2);
		$prev =  $usedprev [0][0];
		$borrowprev = $usedprev [0][2];
		$leftprev = $totalHours - $prev;
		$invprev= $usedprev [0][3];

		if ($leftprev < 0 ){
			$leftprev = 0;
		}

		$prevchanges = sql::sql_run("SELECT msi_audit.date, msi_audit.msi_user
			From msi_audit
			where msi_audit.company =\"$id\" order by msi_audit.date desc limit 1;
		");

		if(isset($_POST['update'])){

			print_r('<br />update is set...<br />');

			$ca = $_POST['carried'];
			$bo = $_POST['borrowed'];
			$ex = $_POST['extra'];

			$hours_left = $_POST['hours_left'];
			$unused_updated = $_POST['previous'];

			if ( !isset($_POST['writeoff']) ) {
				$writeoff = 0;
			} else if( $writeoff == null ) {
				$writeoff = 0;
			} else {
				$writeoff = $_POST['writeoff'];
			}

			if(isset($_POST['invoiced'])){
				$inv = $_POST['invoiced'];
			}

			$effect = $previous[0][1] +$previous[0][2] - $previous[0][0] + $bo - $borrowprev ;

			if ($effect < 0){
				$effect = 0;
			}

			$fg =  $_SESSION['username'];
			$closemonth = $contract_year."-".$contract_month."-00";
			$closedate = date("Y-m-d");

			echo '----------------------------------<br />';
			print_r('id: ' . $id);
			print_r('<br />');
			print_r('closemonth: '.$closemonth);
			echo '----------------------------------<br />';

			$check_duplicate = sql::sql_run("SELECT * 
				FROM msi_close_off
				WHERE msi_close_off.closingmonth = '$closemonth'
				AND msi_close_off.client = '$id';
			");

			var_dump('check duplicate: ');
			print_r($check_duplicate);

			if ( !is_array($check_duplicate) ) {

				print_r('<br />not is_array check duplicate');

				$insert = "INSERT INTO msi_close_off values(null,$id,$ca,$inv,$ex,'Yes',$effect,$bo,$writeoff,'$closemonth','$closedate','$fg','$hours_left',$unused_updated);";

				//echo $insert."<br />";
				sql::sql_run($insert);

				if(!$insert){
					mysqli_error();
				}

				$today = date("Y-m-d");

				$comp = sql::sql_run("SELECT msi_contract.client 
				from msi_contract,msi_billing
				where msi_billing.client = msi_contract.client
				AND msi_billing.client = \"$id\" and upper(msi_billing.type) = 'CONTRACT'
				");

				$company =$comp[0][0];
				$audit = sql::sql_run("INSERT INTO msi_audit (`id`,`company`,`date`,`extra_prev`,`carried`,`borrowed`,`invoiced`,`writeoff`,`msi_user`)
				VALUES (
					null,
					$company,
					'$today',
					$ex,
					$ca,
					$bo,
					$inv,
					$writeoff,
					'$fg'
				);");

				if(!$audit){
					mysqli_error();
				}

				contract::edit($client,$hours,$identity,$year2,$month2,$year3,$month3,$primary,$backup,$server_hours,$server_techie,$contract_month,$contract_year);
				contract::listall(NULL,NULL);

			} else { 
				echo "<p style = 'color:red; size:18px;'><strong>RECORD ALREADY CLOSED</strong></p>";
				contract::listall(NULL,NULL);
			}
		} else {

			print_r('<br />not update is set...<br />');

			$tothrs ="SELECT (ifnull(msi_contract.hours,0) + ifnull(msi_contract.server_hours,0))
				FROM msi_contract
				WHERE msi_contract.client =\"$id\"
				AND (('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
				AND date_format( msi_contract.enddate,'%Y-%m')) 
				OR (msi_contract.terminating='no'and '$fulldate2'>=date_format(msi_contract.startdate,'%Y-%m-00'))
				OR ('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
				AND date_format( msi_contract.terminatingdate,'%Y-%m') and terminating='yes') )
			";

			$hours =  sql::sql_run($tothrs);
			$tothours = $hours[0][0] ;

			//calculate hours from prev month
			$m = $contract_month-1;

			if($m<10){$m = '0'.$m;}
			else {$m =$m;}
			if($m == '0'){ $m = 12;}
			if($m == '12'){ $cyear = $contract_year-1;}else{ $cyear = $contract_year;}

			$sql2="SELECT ifnull(sum(msi_billing.bill_hrs),0)
				FROM msi_billing
				WHERE month(msi_billing.date)= $m  and year(msi_billing.date)= $cyear
				AND msi_billing.client = \"$id\" and upper(msi_billing.type) = 'CONTRACT'
			";

			$off="SELECT ifnull(sum(`msi_close_off`.`writeoff`),0)
				FROM `msi_close_off`
				WHERE month(`msi_close_off`.closingmonth)= $m  and year(`msi_close_off`.closingmonth)= $cyear
				AND `msi_close_off`.client = \"$id\" 
			";

			$sql5="SELECT startdate
				FROM msi_contract
				WHERE msi_contract.client = \"$id\"
				and terminating = 'no'
			";

			$startd = sql::sql_run($sql5);
			$start =  $startd[0][0];
			$usedprev = sql::sql_run($sql2);
			$offprev = sql::sql_run($off);

			$prev =  $usedprev [0][0];
			$writeprev = $offprev [0][0];
			$leftprev = $tothours - $prev;

			if($contract_month<10){$contract_month = '0'.$contract_month;}
			//echo $contract_year."-".$contract_month."-00" ." : ".$start;
			// //  echo   $extra ."extra 3<br />";  // if(date("m")==$contract_month and date("Y")==$contract_year){

			if ( $contract_year."-".$contract_month."-00" == $start ) {
				$leftprev =0;
			}

			//   else {echo " ooooooooooooooops";}
			
			if($leftprev < 0){
				$leftprev = 0;
			}

			print '<table><tr><td colspan="3" align="left">';
			print "<a href=?module=contract&action=edit_form&id=$id&identity=$identity&contract_year=$prev_year&contract_month=$prev_month style=\"color: #1A53FF\">". date(F, mktime(0, 0, 0, $prev_month, 1, $prev_year)) ." $prev_year << Previous</a> ";
			print "&nbsp&nbsp";
			print " <b><a href=?module=contract&action=edit_form&id=$id&identity=$identity&contract_year=$contract_year&contract_month=$contract_month>". date(F, mktime(0, 0, 0, $contract_month, 1, $contract_year)) ." $contract_year</a></b> ";
			print " &nbsp&nbsp;<a href=?module=contract&action=edit_form&id=$id&identity=$identity&contract_year=$next_year&contract_month=$next_month style=\"color: #1A53FF\">Next >> ". date(F, mktime(0, 0, 0, $next_month, 1, $next_year)) ." $next_year</a><p>";
			print '</td></tr><tr><td valign=top>';
			print"<span class=\"head\" > Monthend closing off </span><p></p>";

			if($readonly == "readonly"){
				$sta= "<div class='bolder' style = 'margin-left:215px;'>This Account has been closed</div>";
			} else {
				$sta = "<div class='bolder' style = 'margin-left:150px;'>This Account still needs to be closed</div>";
			}

			print $sta;
			print "<table><tr><td>  

				<table class=\"box\">
					<td align=\"right\"><b>Company Name:</b>
						</td>
							<td>
								<form action=\"index.php?module=contract&action=edit_form&id=$id&identity=$identity&contract_year=$contract_year&contract_month=$contract_month\" name=\"editform\" method=\"POST\">
									<select name=\"company\" readonly>
										<option value=\"".$id."\">".$results2[0][0]."</option>";
											foreach ($results as $result) {
												print "<option value=\"$result[1]\">$result[0]</option>";
											};
									print "
									</select>
							</td>
						</tr>
						<tr>
							<td><b>Contract Hours Per Month:</b></td>
							<td><input style = \"background-color:#C8C8C8\" name=\"hours\" value=\"".$results2[0][2]."\" size='9px'></td>
						</tr>

						<tr>
							<td align=\"right\"><b>Server Hours:</b></td>  
							<td><input style = \"background-color:#C8C8C8\" name=\"server_hours\" value=\"".$results2[0][15]."\" size='9px'></td>
						</tr>

						<tr>
							<td align=\"right\"><b>Unused Hours(current):</b></td>
							<td><input style = \"background-color:#C8C8C8\" name=\"carried\" id=\"carried\" value=\"".$unused."\" onkeyup=\"getValues(20)\" readonly=\"true\"  size='9px' ></td>
						</tr>

						<tr>
							<td align=\"right\"><b> Unused Hours(previous):</b></td>";
							// print"  <td>pev unused1<input style = \"background-color:#C8C8C8\" name=\"carried\"  id=\"unus\" value=\"". $prevunused."\" readonly=\"true\" onkeyup=\"getValues(4)\" size='9px'></td>
			print "
							<td><input  style = \"background-color:#C8C8C8\" name=\"carried\"  id=\"unus\" value=\"".$leftprev."\" readonly=\"true\" size='9px' onkeyup=\"getValues(21)\"></td>
						</tr>
						<tr>
							<td align=\"right\"><b>Hours Written off(previous):</b></td>
							<td><input  style = \"background-color:#C8C8C8\" name=\"writeoff\"  id=\"off\" value=\"". $writeprev."\" readonly=\"true\" onkeyup=\"getValues(22)\" size='9px'></td>
						</tr>";

			print "<tr>
							<td align=\"right\"><b>Hours Over the Contract:</b></td>
							<td><input style = \"background-color:#C8C8C8\" name = \"extra\" type=\"text\" id=\"ex\" readonly=\"readonly\" value=\"".$extra."\" size='9px' onkeyup=\"getValues(23)\" / ></td>
								</tr>
								<tr>	
						<td align=\"right\"><b>	Borrowed prev month:</b></td>
						<td> <input style = \"background-color:#C8C8C8\" size = \"9px\" name = \"brwdpre\"  readonly=\"Readonly\"  type=\"text\" id=\"brwdpre\" value = \"".$borrowprev."\"  size='9px'/></td>
							</tr>		
								<tr>	
						<td align=\"right\"><b>	Borrow from next month:</b></td>
						<td> <input style = \"$background\" size = \"9px\" name = \"borrowed\" type=\"text\" id=\"brwd\" value = \"".$borrowed."\" onkeyup=\"getValues(23)\" $readonly /> (input number of hours)</td>

							</tr>
							<tr>
						<td align=\"right\"><b>Invoiced:</b></td>
						<td><input  style = \"background-color:#C8C8C8\" name = \"invoiced\" id=\"invoiced\" value=\"".$inv2." \"  size = \"9px\"  onkeyup=\"getValues(24)\" readonly=\"readonly\"/> <i>Automatically calculated </i></td>
					";

			print "<td><input style = \"background-color:#C8C8C8\" type = \"hidden\" name = \"previous\" id=\"previous\" value=\"$prevunused\" readonly=\"true\" size = \"9px\" onkeyup=\"getValues(3)\"/>";
			print "<td><input name=\"carried\" type = \"hidden\"  id=\"invpre\" value=\"". $invprev."\" readonly=\"true\" onkeyup=\"getValues(5)\" size='9px'></td>";
			print "<td><input style = \"background-color:#C8C8C8\" name = \"hours_left\" id=\"hours_left\" value=\"".$avail." \" size = \"9px\" type=\"hidden\" onkeyup=\"getValues(8)\" / ></td>";

			print" <td> <input  size = \"9px\" name = \"deduct2\" type=\"hidden\" id=\"deduct2\" value = \"".$avail."\" onkeyup=\"getValues(3)\"/  ></td>";


			$total = "SELECT date_format(msi_contract.startdate,'%Y-%m-%01'),ifnull(msi_contract.effective,0)
				FROM msi_contract
				WHERE msi_contract.client = \"$id\"
				and (('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
				and date_format( msi_contract.enddate,'%Y-%m')) or
				(msi_contract.terminating='no'and '$fulldate2'>=date_format(msi_contract.startdate,'%Y-%m-00'))
				or ('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
				and date_format( msi_contract.terminatingdate,'%Y-%m') and terminating='yes') )";

			// AND	msi_contract.terminating='no'
			$contract = sql::sql_run($total);
			$contract_date = $contract[0][0];
			$available = $contract[0][1];
			$num = cal_days_in_month(CAL_GREGORIAN, $contract_month, $contract_year); // 31
			$ddate=$contract_year.'-'.$strmonth.'-'.$num;
			$date1 = date(strtotime($contract_date));
			$date2 = date(strtotime($ddate));
			$difference = $date2 - $date1;
			$month = floor($difference / 86400 / 30 )+ 1;
			//$months = round($difference / 86400 / 30 );
			$contracttime  = $contract_date ;
			$currentdate = $contract_year."-".$contract_month."-".$num;
			$d1 = strtotime($contracttime );
			$d2 = strtotime($currentdate);
			$min_date = min($d1, $d2);
			$max_date = max($d1, $d2);
			$t = 1;

			while (($min_date = strtotime("+1 MONTH", $min_date)) <=  $max_date) {
				$t++;
			}

			$tothrs ="SELECT (ifnull(msi_contract.hours,0) + ifnull(msi_contract.server_hours,0))
				FROM msi_contract
				WHERE msi_contract.client =\"$id\"
				and (('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
				and date_format( msi_contract.enddate,'%Y-%m')) or
				(msi_contract.terminating='no'and '$fulldate2'>=date_format(msi_contract.startdate,'%Y-%m-00'))
				or ('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
				and date_format( msi_contract.terminatingdate,'%Y-%m') and terminating='yes') ) ";

			$hours =  sql::sql_run($tothrs);
			$allhours = $hours[0][0] * $t;
			$tothours = $hours[0][0];

			//calculate hours from prev month
			$m = $contract_month-1;
			if($m<10){$m = '0'.$m;}
			else {$m =$m;}
			if($m == '0'){ $m = 12;}
			if($m == '12'){ $cyear = $contract_year-1;}else{ $cyear = $contract_year;}
			
			$sql2="SELECT ifnull(sum(msi_billing.bill_hrs),0)
			FROM msi_billing
			WHERE month(msi_billing.date)= $prev_month  and year(msi_billing.date)= $prev_year
			AND msi_billing.client = \"$id\" and upper(msi_billing.type) = 'CONTRACT'";
			
			$sql3= "SELECT ifnull(sum(msi_close_off.extra),0),ifnull(msi_close_off.borrowed,0),ifnull(msi_close_off.writeoff,0)
			FROM msi_close_off
			WHERE month(msi_close_off.closingmonth) = $m
			AND year(msi_close_off.closingmonth)= $cyear
			AND msi_close_off.client = \"$id\" ";
			
			$sql4= "SELECT sum(ifnull(msi_close_off.writeoff,0)),msi_close_off.closingmonth,msi_contract.startdate
			FROM msi_close_off,msi_contract
			WHERE msi_close_off.client = msi_contract.client
			AND msi_close_off.closingmonth >= msi_contract.startdate
			AND msi_close_off.closingmonth <= \"$fulldate2\"
			and (('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
			and date_format( msi_contract.enddate,'%Y-%m')) or
			(msi_contract.terminating='no'and '$fulldate2'>=date_format(msi_contract.startdate,'%Y-%m-00'))
			or ('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
			and date_format( msi_contract.terminatingdate,'%Y-%m') and terminating='yes') )
			AND msi_close_off.client = \"$id\" ";

			$usedprev = sql::sql_run($sql2);
			$previous = sql::sql_run($sql3);
			
			$prev =  $usedprev[0][0];
			$extra = $previous[0][0];
			
			$write = sql::sql_run($sql4);
			$writeoff = $write[0][0];
			
			$leftprev = $results2[0][2]- $prev;
			// $leftprev = $tothours - $prev;
			//echo $leftprev;

			if ($leftprev < 0 ){
				$extra = $leftprev*-(1);
				$leftprev = 0;
			} else {
				$extra = $extra;
			}

			$sql5="SELECT ifnull(msi_close_off.invoiced,0)
				FROM msi_close_off
				WHERE msi_close_off.client = \"$id\"
				AND month(msi_close_off.closingmonth)= $prev_month
				and year(msi_close_off.closingmonth)= $prev_year
			";

			$invoicedprev = sql::sql_run($sql5);
			$invprev =  $invoicedprev[0][0];
			
			$usedcurrent="SELECT ifnull(sum(bill_hrs),0)
			FROM msi_billing
			WHERE month(date) = $contract_month and year(date)= $contract_year
			AND client = \"$id\" and upper(type) = 'CONTRACT' ";
			$usedhourscur = sql::sql_run($usedcurrent);

			$used =  $usedhourscur[0][0];

			$effecborrow = "SELECT ifnull(sum(msi_close_off.borrowed),0)
				FROM msi_close_off,msi_contract
				WHERE msi_close_off.client  = \"$id\"
				AND msi_close_off.client = msi_contract.client
				AND msi_close_off.closingmonth >= msi_contract.startdate
				and month(msi_close_off.closingmonth) = $m
				AND year(msi_close_off.closingmonth)= $cyear
				and (('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
				and date_format( msi_contract.enddate,'%Y-%m')) or
				(msi_contract.terminating='no'and '$fulldate2'>=date_format(msi_contract.startdate,'%Y-%m-00'))
				or ('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
				and date_format( msi_contract.terminatingdate,'%Y-%m') and terminating='yes') )
			";

			$effecinvoice = "SELECT sum(msi_close_off.invoiced),sum(ifnull(msi_close_off.borrowed,0))
				FROM msi_close_off,msi_contract
		
				WHERE msi_close_off.client  = \"$id\"
				AND msi_close_off.client = msi_contract.client
				AND msi_close_off.closingmonth >= msi_contract.startdate
				AND msi_close_off.closingmonth <= '$ddate'
		
				and (('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
				and date_format( msi_contract.enddate,'%Y-%m')) or
				(msi_contract.terminating='no'and '$fulldate2'>=date_format(msi_contract.startdate,'%Y-%m-00'))
				or ('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
				and date_format( msi_contract.terminatingdate,'%Y-%m') and terminating='yes') )";

				$effecthours ="SELECT ifnull(sum(msi_billing.bill_hrs),0),msi_billing.date, date_format( msi_contract.startdate,'%Y-%m'), msi_contract.enddate
				FROM msi_contract,msi_clients,msi_billing
				WHERE msi_clients.id = msi_contract.client
				AND msi_billing.client = msi_contract.client
				AND msi_billing.date >= msi_contract.startdate
				AND msi_billing.date <= '$ddate'
				AND msi_billing.client = \"$id\" and upper(msi_billing.type) = 'CONTRACT'
				and (('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
				and date_format( msi_contract.enddate,'%Y-%m')) or
				(msi_contract.terminating='no'and '$fulldate2'>=date_format(msi_contract.startdate,'%Y-%m-00'))
				or ('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
				and date_format( msi_contract.terminatingdate,'%Y-%m') and terminating='yes') )
				AND msi_clients.customer = 1 ";

			$efehours = sql::sql_run($effecthours);
			$peffective = $efehours[0][0];
			$pinvoiced = sql::sql_run($effecinvoice);
			$inveffec= $pinvoiced[0][0];
			$borowing = sql::sql_run($effecborrow);
			$borrowprev =  $borowing[0][0];

			$unusedall = $allhours -  $peffective;

			if($unusedall < 0){
				$unusedall = 0;
			}

			$invoicedcurrent = "SELECT sum(ifnull(msi_close_off.invoiced,0))
				FROM msi_close_off, msi_contract
				WHERE msi_close_off.closingmonth like \"%$fulldate%\"
				AND msi_close_off.client = \"$id\"
				AND msi_contract.client = msi_close_off.client
				AND msi_close_off.closingmonth >= msi_contract.startdate
				and (('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
				and date_format( msi_contract.enddate,'%Y-%m')) or
				(msi_contract.terminating='no'and '$fulldate2'>=date_format(msi_contract.startdate,'%Y-%m-00'))
				or ('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
				and date_format( msi_contract.terminatingdate,'%Y-%m') and terminating='yes') )";

			$invoiced = sql::sql_run($invoicedcurrent);
			$invcd = $invoiced[0][0];

			$condate= "SELECT month(msi_contract.startdate), year(msi_contract.startdate)
				FROM
				msi_contract
				WHERE
				msi_contract.client = \"$id\"
				and (('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
				and date_format( msi_contract.enddate,'%Y-%m')) or
				(msi_contract.terminating='no'and '$fulldate2'>=date_format(msi_contract.startdate,'%Y-%m-00'))
				or ('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
				and date_format( msi_contract.terminatingdate,'%Y-%m') and terminating='yes') )";

			$ddate = sql::sql_run($condate);
			$mdate = $ddate[0][0];
			$ydate = $ddate[0][1];
			$sdate = $ydate."-".$mdate;
			$cdate = $contract_year."-".$contract_month;

			$effectivenow = 0;
			if ($sdate == $cdate){
				$effectivenow  = $tothours - $used + $invcd + $borrowed - $writeoff + $available;
				//echo $effectivenow  ." = ".$tothours." - ". $used ." + ". $invcd   ." +". $borrowed." + ". $available." - ". $writeoff." + ". $available."<br />";
			} else {
				$effectivenow  = $allhours - $peffective +  $inveffec + $borrowed  - $writeoff + $available;
				//echo $effectivenow  ." = ".$allhours ." - ". $peffective." + ". $inveffec  ." +". $borrowed." - ". $writeoff." + ". $available."<br />";
			}

			$effectivenow = number_format($effectivenow, 2);
			print "	<td><div class = 'bolder' >Effective Hours:</div></td>
				<td><input style = \"background-color:#C8C8C8\" name=\"ehours\" id=\"ehours\" value=\"".$effectivenow."\" size='9px' onkeyup=\"getValues(25)\"></td></tr>";
			// code commented out -> see file temp_out_code.txt
			print "
				<input type=\"hidden\" name=\"identity\" value=\"".$identity."\">

				<tr>
					<td colspan=\"2\" align=\"center\" ></td>
				</tr>
				<tr>
					<td colspan=\"2\" align=\"center\" ></td>
				</tr>
				<tr>
					<td colspan=\"2\" align=\"center\" ></td>
				</tr>
				</tr>
				<tr><td colspan=\"2\" align=\"center\">
			
					<input type=\"submit\" value=\"Submit\" name =\"update\" onclick=\"return confirm('If you submit the changes you will not be able to go back. \\r\\n\\r\\nAre you sure you want to submit?')\"class=\"buttons\">
					<input type=\"button\" onClick=\"javascript:history.go(-1)\"  value=\"Cancel\" class=\"buttons\">
					<b>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
					Last changes were made on <br />&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;". $prevchanges[0][0]." By ". ucfirst($prevchanges[0][1])."
					
			</b></td></tr>
			</form>";

			print "</table></td><td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td><td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td>";
			print "<br/>";
			print "<td valign=\"top\">";
			print "<table style = \"width:400px\" class=\"client\"padding=\"none\">";
			print "<tr><span class=\"head\" align=\"right\">Fields Description</span></tr>";
			print "<tr><td class=\"client\"><font color=\"blue\">Contract Hours Per Month</font></td><td class=\"client\">Contracted hours to be covered per month</td></tr>";
			/* <tr><td class=\" client\"><font color=\"blue\">Used </font></td><td class=\"client\">Total hours that have been used in this month only</td></tr>
			<tr><td class=\"client\"><font color=\"blue\">Unused</font></td><td class=\"client\"> Hours that have not been covered from this month only</td></tr>
			*/
			print" <tr><td class=\"client\"><font color=\"blue\">Unused Hours</font></td><td class=\"client\">These hours are carried over to the next month</td></tr>
			<tr><td class=\"client\"><font color=\"blue\">Hours Over the Contract</font></td><td class=\"client\"> Additional hours for selected month</td></tr>
			<tr><td class=\"client\"><font color=\"blue\">Borrow from next month</font></td><td class=\"client\">  Hours that should be borrowed from the following month - (Settable)</td></tr>
			<tr><td class=\"client\"><font color=\"blue\">Invoiced </font></td><td class=\"client\">Hours to be invoiced</td></tr>
			</tr>";
			print "</table></td></tr></table>";
		}
	}


	function audit($contract_month,$contract_year){
			if(!acl::has_access($_SESSION['id'],"contracts","read")) return 1;

			//beginning of new code, 2008/06/06 making contracts month aware.

		if(!isset($contract_month)&&!isset($contract_year)) {
			$contract_year = date(Y);
			$contract_month = date(n);
		}

		$next_month = $contract_month +1;

		if ($next_month > 12) {
				$next_month = 1;
				$next_year = $contract_year +1;
				}
		else {
				$next_year = $contract_year;
		}

		$prev_month = $contract_month - 1;
		if ($prev_month == 0){
				$prev_month = 12;
				$prev_year= $contract_year -1;
				}
		else {
			$prev_year= $contract_year;
		}

		//add leading 0 for months less than 10
		if($contract_month < 10){
			$strmonth = '0'.$contract_month;
		}else{
			$strmonth = $contract_month;
		}

		//end add leading

		$moncalc=$contract_year.'-'.$strmonth.'-'.'01';

		$fulldate=$contract_year.'-'.$strmonth;

		$fulldate2=$contract_year.'-'.$strmonth.'-'.'00';


		$results = sql::sql_run("SELECT msi_clients.company,
			msi_contract.hours,
			msi_close_off.closingmonth,
			msi_close_off.closingdate,
			msi_close_off.extra,
			msi_close_off.carried,
			msi_close_off.borrowed,
			msi_close_off.invoiced,
			msi_close_off.user,
			msi_contract.server_hours,
			msi_close_off.writeoff
			
			from msi_contract,msi_close_off,msi_clients
			where msi_clients.id = msi_contract.client
			and msi_close_off.client = msi_clients.id
							
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
			AND month(msi_close_off.closingdate) = $contract_month and year(msi_close_off.closingdate)= $contract_year;
		");

		print '<table><tr><td colspan="3" align="left">';
		print "<a href=?module=contract&action=audit&contract_year=$prev_year&contract_month=$prev_month style=\"color: #1A53FF\">". date(F, mktime(0, 0, 0, $prev_month, 1, $prev_year)) ." $prev_year << Previous</a> ";
		print "&nbsp&nbsp";
		print " <b><a href=?module=contract&action=audit&contract_year=$contract_year&contract_month=$contract_month>". date(F, mktime(0, 0, 0, $contract_month, 1, $contract_year)) ." $contract_year</a></b>";
		print "&nbsp&nbsp";
		print " <a href=?module=contract&action=audit&contract_year=$next_year&contract_month=$next_month style=\"color: #1A53FF\">Next >> ". date(F, mktime(0, 0, 0, $next_month, 1, $next_year)) ." $next_year</a><p>";
		print '</td></tr>
		<tr><td valign=top>';

		//ending of new code
		// print "<span class=\"head\">Client Contracts</span>";
		if(is_array($results)){
			print"<form action=\"index.php?module=contract&action=term\" name=\"editform\" method=\"POST\">
			<span class=\"head\">Month End Closing</span><br />
			<table><tr><td valign =\"top\" >
						<table class=\"client\"  width =\"670px\">
						<tr class=\"head\"><td>Company Name</td>
						<td title =\"Contracted hours to be covered per month\"style=\"border-left: 1px solid #FFFFFF\">Contract Hours</td> 
						<td title =\"Close-off date\"style=\"border-left: 1px solid #FFFFFF\">Date Closed</td>
						<td title =\"Close-off month\"style=\"border-left: 1px solid #FFFFFF\">Closing Month</td>  
						<td title =\"Hours that are carried over to the next month\"style=\"border-left: 1px solid #FFFFFF\">Carried Over</td> 
						<td title =\"Extra hours from previous month only\"style=\"border-left: 1px solid #FFFFFF\">Extra</td> 
						<td title =\"Hours borrowed from the following month\"style=\"border-left: 1px solid #FFFFFF\">Borrowed</td> 
						<td title =\"Hours to be billed\"style=\"border-left: 1px solid #FFFFFF\">Invoiced</td> 
						<td title =\"Hours written off\"style=\"border-left: 1px solid #FFFFFF\">Written Off</td> 
						<td title =\"The user who updated the record\"style=\"border-left: 1px solid #FFFFFF\">User</td>
			</tr>";


			foreach ($results as $result) {
				$toth = $result[9]+$result[1];
				print "<tr >
					<td class=\"client\">$result[0]</td>
					<td class=\"client\" align=\"center\">$toth</td>
					<td class=\"client\" align=\"center\">$result[3]</td>
					<td class=\"client\" align=\"center\">$result[2]</td>
					<td class=\"client\" align=\"center\">$result[5] </td>
					<td class=\"client\" align=\"center\">$result[4] </td>
					<td class=\"client\" align=\"center\">$result[6] </td>
					<td class=\"client\" align=\"center\"> $result[7] </td>
					<td class=\"client\" align=\"center\"> $result[10] </td>
					<td class=\"client\" align=\"center\"> $result[8] </td>
				</tr>";
			}
		} else {
			echo "<p><font color ='red'><b>NO RESULTS FOUND</b></font></p>";
		}

		print "</form></table>";
		print' </td><td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td>
				<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td>';
		print "<br/>";
		print "<td valign=\"top\">";
		print "<table style = \"width:450px\" class=\"client\"padding=\"none\">";
		print "<tr><span class=\"head\" align=\"right\">Fields Description</span></tr>";
		print "<tr><td class=\"client\"><font color=\"blue\">Contract Hours</font></td><td class=\"client\">Contracted hours to be covered per month</td></tr>
			<tr><td class=\"client\"><font color=\"blue\">Date Closed</font></td><td class=\"client\">The actual date the month was closed </td></tr>
			<tr><td class=\"client\"><font color=\"blue\">Closing Month</font></td><td class=\"client\">The month being closed</td></tr>
				<tr><td class=\"client\"><font color=\"blue\">Carried over</font></td><td class=\"client\">Number of hours carried over to the following month</td></tr>
				<tr><td class=\"client\"><font color=\"blue\">Extra</font></td><td class=\"client\"> Additional hours covered in the month being closed</td></tr>
				<tr><td class=\"client\"><font color=\"blue\">Borrowed</font></td><td class=\"client\"> Hours  borrowed from the following month</td></tr>
				<tr><td class=\"client\"><font color=\"blue\">Invoiced</font></td><td class=\"client\"> Number of invoiced hours </td></tr>
				<tr><td class=\"client\"><font color=\"blue\">Written Off</font></td><td class=\"client\"> Number of hours written off</td></tr>
				<tr><td class=\"client\"><font color=\"blue\">User</font></td><td class=\"client\"> The user who closed the month</td></tr>
			</tr>";
		print "</table></td></tr></table>"; 
		print "<p>&nbsp;</p>" ;
		
		/************************CONTRACTS CANCELLATION/RENEWAL *****************************************/
		$contract_audit = "select msi_clients.company,
			msi_auditcontract.startdate,
			msi_auditcontract.enddate,
			msi_auditcontract.hours,
			ifnull(msi_auditcontract.server_hours,0),
			msi_auditcontract.terminating,
			msi_auditcontract.terminatingdate,
			msi_auditcontract.Effective,
			msi_auditcontract.primarytech,
			msi_auditcontract.backuptech,
			msi_auditcontract.server_techie,
			msi_auditcontract.acc_manager

			
			from msi_clients,msi_auditcontract
			where msi_auditcontract.client = msi_clients.id
			and( month(msi_auditcontract.startdate) = $contract_month and year(msi_auditcontract.startdate)= $contract_year
			or month(msi_auditcontract.enddate) = $contract_month and year(msi_auditcontract.enddate)= $contract_year
			or month(msi_auditcontract.terminatingdate) = $contract_month and year(msi_auditcontract.terminatingdate)= $contract_year)
			Order by msi_clients.company
		";

		//echo $contract_audit."<br />";

		$cancellation = sql::sql_run($contract_audit);

		if(is_array($cancellation)){
			print"<form action=\"index.php?module=contract&action=term\" name=\"editform\" method=\"POST\">
				<span class=\"head\">Contract Cancellation & Renewal</span><br /><br />
				<table><tr><td valign =\"top\" >
				<table class=\"client\" width =\"650px\">
				<tr class =\"head\"><td>Company Name</td>
				<td title =\"Contract start date\"style=\"border-left: 1px solid #FFFFFF\">Start Date</td> 
				<td title =\"Contract end date\"style=\"border-left: 1px solid #FFFFFF\">End Date</td>
				<td title =\"Number of contracted hours\"style=\"border-left: 1px solid #FFFFFF\">Contract Hours</td>
				<td title =\"Server Hours\"style=\"border-left: 1px solid #FFFFFF\">Server Hours</td>  
				<td title =\"Status of the contract\"style=\"border-left: 1px solid #FFFFFF\">Contract Status</td>  
				<td title =\"Contract cancellation date\"style=\"border-left: 1px solid #FFFFFF\">Cancellation Date</td> 
				<td title =\"Effective hours on cancellation date\"style=\"border-left: 1px solid #FFFFFF\">Effective</td> 
				<td title =\"Brimary Techie\"style=\"border-left: 1px solid #FFFFFF\">Brimary Techie</td>
				<td title =\"Backup Techie\"style=\"border-left: 1px solid #FFFFFF\">Backup Techie</td>  
				<td title =\"Server Techie\"style=\"border-left: 1px solid #FFFFFF\">Server Techie</td> 
				<td title =\"Account Manager\"style=\"border-left: 1px solid #FFFFFF\">Account Manager</td> 
				</tr>
			";

			foreach ($cancellation as $res) {

				if($res[5] == "yes"){
					$state = "Inactive";
					$colour='red';
				} else {
					$state = "Active";
					$colour='black';
				}

				$state = "<font color='".$colour."'>$state</font>";

				print "<tr >
					<td class=\"client\">$res[0]</td>
					<td class=\"client\" align=\"center\">$res[1]</td>
					<td class=\"client\" align=\"center\">$res[2]</td>
					<td class=\"client\" align=\"center\">$res[3]</td>
					<td class=\"client\" align=\"center\">$res[4]</td>
					<td class=\"client\" align=\"center\">$state</td>
					<td class=\"client\" align=\"center\">$res[6]</td>
					<td class=\"client\" align=\"center\">$res[7]</td>
					<td class=\"client\" align=\"center\">$res[8]</td>
					<td class=\"client\" align=\"center\">$res[9]</td>
					<td class=\"client\" align=\"center\">$res[10]</td>
					<td class=\"client\" align=\"center\">$res[11]</td>
					</tr>";
			}
		} else {
			echo "<p><font color ='red'><b>NO RESULTS FOUND</b></font></p>";
		}

		print "</form></table>";
		print' </td><td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td>
			<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td>';
		print "<br/>";
		print "<td valign=\"top\">";
		print "<table style = \"width:450px\" class=\"client\"padding=\"none\">";
		print "<tr><span class=\"head\" align=\"right\">Fields Description</span></tr>";
		print " <tr><td class=\"client\"><font color=\"blue\">Start Date</font></td><td class=\"client\">The start date of a new contract </td></tr>
				<tr><td class=\"client\"><font color=\"blue\">End Date</font></td><td class=\"client\">End date of the contract</td></tr>
				<tr><td class=\"client\"><font color=\"blue\">Contract Hours</font></td><td class=\"client\">Contracted hours to be covered per month</td></tr>
				<tr><td class=\"client\"><font color=\"blue\">Contract Status</font></td><td class=\"client\">Status of the contract(active/inactive) </td></tr>
				<tr><td class=\"client\"><font color=\"blue\">Cancellation Date</font></td><td class=\"client\">The contract canccellation date</td></tr>
				<tr><td class=\"client\"><font color=\"blue\">Effective</font></td><td class=\"client\">Effective hours on cancellation/renewal date</td></tr>
		</tr>";

		print "</table></td></tr></table>";
			// <tr><td class=\"client\"><font color=\"blue\">User</font></td><td class=\"client\"> The user who closed the month</td></tr>
			// <tr><td class=\"client\"><font color=\"blue\">Closing Month</font></td><td class=\"client\">The month being closed</td></tr>
			// <tr><td class=\"client\"><font color=\"blue\">Carried over</font></td><td class=\"client\">Number of hours carried over to the following month</td></tr>
			// <tr><td class=\"client\"><font color=\"blue\">Extra</font></td><td class=\"client\"> Additional hours covered in the month being closed</td></tr>
			// <tr><td class=\"client\"><font color=\"blue\">Borrowed</font></td><td class=\"client\"> Hours  borrowed from the following month</td></tr>
			// <tr><td class=\"client\"><font color=\"blue\">Invoiced</font></td><td class=\"client\"> Number of invoiced hours </td></tr>
			// <tr><td class=\"client\"><font color=\"blue\">Written Off</font></td><td class=\"client\"> Number of hours written off</td></tr>
			// <tr><td class=\"client\"><font color=\"blue\">User</font></td><td class=\"client\"> The user who closed the month</td></tr>

			//print "</table></td></tr></table>";    

}


function maintenance_termination($client,$hours,$identity,$year2,$month2,$year3,$month3,$termmonth,$termyear,$term,$primary,$backup,$server_hours,$server_techie,$m_type){

	var_dump('maintenance_termination');

	if(!acl::has_access($_SESSION['id'],"contracts","edit")) return 1;

	$primary=$_POST['primary'];
	$backup=$_POST['backup'];
	$server_hours=$_POST['server_hours'];
	$server_techie=$_POST['server_techie'];
	$hours = $_POST['hours'];
	$client = $_POST['company'];
	$termyear = $_POST['termyear'];
	$termmonth = $_POST['termmonth'];
	$year2 = $_POST['year2'];
	$year3 = $_POST['year3'];
	$month2 = $_POST['month2'];
	$month3 = $_POST['month3'];
	$identity = $_GET['identity'];
	$term =$_POST['term'];
	$m_type =trim($_POST['m_type']);
	$acc_manager =$_POST['acc_manager'];

	var_dump($_POST);

	if($term=='')
	{
		$terminate='no';
	}
	if($term=='1')
	{
		$terminate='yes';
	}

	$numdays=date("t", strtotime($year3 . "-" . $month3 . "-01"));

	$termday=date("t", strtotime($termyear . "-" . $termmonth . "-01"));
	$termdate=$termyear.'-'.'0'.$termmonth.'-'.$termday;

	if($month3<10)
	{
		$endcdate=$year3.'-'.'0'.$month3.'-'.'00';
	}
	else
	{
		$endcdate=$year3.'-'.$month3.'-'.'00';
	}
	if($month2<10)
	{
		$startcdate=$year2.'-'.'0'.$month2.'-'.'00';
	}
	else
	{
		$startcdate=$year2.'-'.$month2.'-'.'00';
	}
	if($termmonth<10)
	{
		$termdate=$termyear.'-'.'0'.$termmonth.'-'.'00';
	}
	else
	{
		$termdate=$termyear.'-'.$termmonth.'-'.'00';
	}
	if($term=='')
	{
		$terminate='no';
		$termdate=null;
	}

	$months=explode("-",$startcdate);
	$months2=explode("-",$endcdate);
	
	if($startcdate=='' || $endcdate=='') {
		echo '<font color="red"><b>ERROR: Start date or end date cant be null</b></font>';
		return false;
	}

	if(!preg_match("/^[0-9]+(.[0-9]{2})?$/", $hours)) {
		echo '<font color="red"><b>ERROR: Contract hours is not valid</b></font>';
		return false;
	}

	if(preg_match("/^[0-9]+(.[0-9]{2})?$/", $carried)) {
		//if(!preg_match('/^[0-9]+$/',$carried)) {
		echo '<font color="red"><b>ERROR: Carried over hours is not valid</b></font>';
		return false;
	}

	if(preg_match("/^[0-9]+(.[0-9]{2})?$/", $borrowed)) {
		//if(!preg_match('/^[0-9]+$/',$carried)) {
		echo '<font color="red"><b>ERROR: Borrowed hours is not valid</b></font>';
		return false;
	}

	if($hours<1) {
		echo '<font color="red"><b>ERROR:The number of hours per month are Invalid</b></font>';
		return false;
	}

	if($hours>720) {
		echo '<font color="red"><b>ERROR:The number of hours per month are Invalid</b></font>';
		return false;
	}

	if($termdate<$startcdate and $term=='1') {
		echo '<font color="red"><b>ERROR:Termination date cant be less than the contract start date</b></font>';
		return false;
	}

	if($m_type=='') {
		echo '<font color="red"><b>ERROR:Type of maintenance can not be empty</b></font>';
		return false;
	}


	if($startcdate==''||$endcdate=='') {
		echo' <font color="red"><b>ERROR:Neither Start date nor End date can be null</b></font>';
		return false;
	}

	$termdate = $termyear.'-'.'0'.$termmonth.'-'.$termday;
	//echo "select client from msi_mcontract where client = $client and contractid='$identity'";

	$ret_exists = sql::sql_run("select client from msi_mcontract where client = $client and contractid='$identity'");
	//echo $ret_exists."<br >";

	if(!$ret_exists){
		print("This is not a contract client please add the client first...");
		exit;
	}

	$ret_update = sql::sql_run("UPDATE msi_mcontract SET 
		`client` = $client,
		`hours` = $hours,
		`startdate` ='$startcdate',
		`enddate` ='$endcdate',
		`terminatingdate` ='$termdate',
		`terminating` ='$terminate',
		`primarytech` ='$primary', 
		`backuptech` ='$backup', 
		`server_hours` = '$server_hours',
		`server_techie` = '$server_techie',
		`type_maintenance` = '$m_type',
		`acc_manager` = '$acc_manager'
		WHERE `client` = $client AND `contractid`='$identity'
	");

	var_dump('-----ret_update-----');
	var_dump($ret_update);

	$getclientname=sql::sql_run("select company from msi_clients where id='$client'");
	$clientname = $getclientname[0][0];

	if(!isset($ret_update)){
		print "<b>Updated details for $clientname</b><p>";
		contract::maintenance(NULL,NULL);
	} else {
		print "<b>Something went wrong !</b>";
	}
}



function edit($client,$hours,$identity,$year2,$month2,$year3,$month3,$primary,$backup,$server_hours,$server_techie,$contract_month,$contract_year){
  if(!acl::has_access($_SESSION['id'],"contracts","edit")) return 1;

$primary=$_POST['primary'];
$backup=$_POST['backup'];
$server_hours=$_POST['server_hours'];
$server_techie=$_POST['server_techie'];
$hours = $_POST['hours'];
$client = $_POST['company'];
$termyear = $_POST['termyear'];
$termmonth = $_POST['termmonth'];
$year2 = $_POST['year2'];
$year3 = $_POST['year3'];
$month2 = $_POST['month2'];
$month3 = $_POST['month3'];
$identity = $_POST['identity'];

  if(!isset($contract_month)&&!isset($contract_year))
{
  $contract_year = date(Y);
  $contract_month = date(n);
}
  $next_month = $contract_month +1;
  if ($next_month > 12) { 
          $next_month = 1;
          $next_year = $contract_year +1;
          }       
  else {
          $next_year = $contract_year;
  }
  $prev_month = $contract_month - 1;
  if ($prev_month == 0){
          $prev_month = 12;
          $prev_year= $contract_year -1;
          }       
  else {
    $prev_year= $contract_year;
  }

//add leading 0 for months less than 10
if($contract_month < 10){
$strmonth = '0'.$contract_month;
}else{
$strmonth = $contract_month;
}
//end add leading
$moncalc=$contract_year.'-'.$strmonth.'-'.'01';
$fulldate=$contract_year.'-'.$strmonth;
$fulldate2=$contract_year.'-'.$strmonth.'-'.'00';
  
  $numdays=date("t", strtotime($year3 . "-" . $month3 . "-01"));
  $endcdate=$year3.'-'.'0'.$month3.'-'.$numdays;
  $startcdate=$year2.'-'.'0'.$month2.'-'.'01';
  if($month3<10)
  {
  $endcdate=$year3.'-'.'0'.$month3.'-'.'00';
  }
  else
  {
   $endcdate=$year3.'-'.$month3.'-'.'00';
  }
  if($month2<10)
  {

$startcdate=$year2.'-'.'0'.$month2.'-'.'00';
  }
  else
  {
   $startcdate=$year2.'-'.$month2.'-'.'00';
  }

  $months=explode("-",$startcdate);
  $months2=explode("-",$endcdate);
 // $carried = $_POST['carried'];
 // $borrowed = $_POST['borrowed'];
  
/*
//  if($startcdate!='')
//{
//  $months=explode("-",$startcdate);
//  $months2=explode("-",$endcdate);
//  $year=date("Y");
//  if($months[0]<$year|| $months2[0]<$year) {
//      ?><font color="red"><b>ERROR: You are not allowed to input last years contracts</b></font><?php;
//      return false;
//
//    }
}
  */
if($startcdate=='' || $endcdate=='') {
      ?><font color="red"><b>ERROR: Start date or end date cant be null</b></font><?php;
      return false;
    }

    if($hours>720) {
      ?><font color="red"><b>ERROR:The number of hours per month are Invalid</b></font><?php;
      return false;
    }
    /*if($hours<1) {
      ?><!-- <font color="red"><b>ERROR:The number of hours per month are Invalid</b></font>--><?php;
      return false;
    }*/

  if(!preg_match('/^[0-9]+$/',$hours)) {
      ?><font color="red"><b>ERROR: Contract hours are invalid</b></font><?php;
      return false;
    }
    
   if(preg_match("/^[0-9]+(.[0-9]{1})?$/", $carried)) { 
      //if(!preg_match('/^[0-9]+$/',$carried)) {
      ?><font color="red"><b>ERROR edit: Carried over hours are invalid</b></font><?php;
      return false;
   } 
   if(preg_match("/^[0-9]+(.[0-9]{1})?$/", $borrowed)) { 
   //if(!preg_match('/^[0-9]+$/',$carried)) {
      ?><font color="red"><b>ERROR edit: Borrowed hours are invalid</b></font><?php;
      return false;
    } 

    if($startcdate>$endcdate) {
      ?><font color="red"><b>ERROR: Start date cant be greater than end date</b></font><?php;
      return false;
    }
    /*
    $currentdate=date("Y-m-d");
    if($startcdate<$currentdate) {
      ?><font color="red"><b>ERROR: Start date cant be less than todays date</b></font><?php;
      return false;

    }
     */

    if($startcdate==''||$endcdate=='') {
      ?><font color="red"><b>ERROR:Neither Start date nor End date can be null</b></font><?php;
      return false;
}
//$ret_existscnt = sql::sql_run("select $client,contid,startdate,enddate from msi_contract where client = $client and ('$startcdate' between startdate and enddate or '$endcdate' between startdate and enddate or ('$startcdate'<startdate and '$endcdate.'>enddate)) and contid !=".$identity."");
//if($ret_existscnt)
//{
 //print"There is already a contract for the same client on the following date from ".$ret_existscnt[0][2].'  '.'to'.' '.$ret_existscnt[0][3];
//exit;
//}

$ret_exists = sql::sql_run("select client from msi_contract where client = $client and contid='$identity'");
if(!$ret_exists){
print("This is not a contract client please add the client first...");
exit;
}else{
}

//$ret_update = sql::sql_run("UPDATE msi_contract SET client = $client, hours = $hours,  primarytech ='$primary', backuptech = '$backup', server_hours = '$server_hours', server_techie = '$server_techie' where client = $client and contid= $identity
// ");

$ret_update = sql::sql_run("UPDATE msi_contract SET client = $client, hours = $hours, server_hours = '$server_hours' where client = $client and contid= $identity
		");

$getclientname=sql::sql_run("select company from msi_clients where id='$client'");
$clientname = $getclientname[0][0];
if(isset($ret_update)){
    print "<b>Something went wrong !</b>".mysqli_error();
    //contract::listall(NULL,NULL);
    } else {
    print "<div class='bolder'>Updated contract for   $clientname </div>";

    }
}

function delete($client){
 if(!acl::has_access($_SESSION['id'],"contracts","add")) return 1;

$result = sql::sql_run('update msi_contract set cancelled="cancelled" where client  = "'.$client.'"');
if(!isset($result)){
    print "<b>Deleted contract hours for client with id $client</b><p>";
    contract::listall(NULL,NULL);
    } else {
      print "<b>Something went wrong !</b>";
    }
}






function deletecont($identity,$company){
  if(!acl::has_access($_SESSION['id'],"contracts","add")) return 1;
  $result = sql::sql_run('delete from msi_contract where contid = "'.$identity.'"');
    if(!isset($result)){
      $getclientname=sql::sql_run("select company from msi_clients where id='$company'");
               $clientname=$getclientname[0][0];
           print "<b>Deleted contract hours for $clientname</b><p>";
                 contract::terminate(NULL,NULL);
                 } else {
      print "<b>Something went wrong !</b>";
    }
}


/////////////////////////////////////////////////////////////////
function maintenance_deletecont($identity,$company){
	if(!acl::has_access($_SESSION['id'],"contracts","add")) return 1;
	$result = sql::sql_run('delete from msi_mcontract where contractid = "'.$identity.'"');
	if(!isset($result)){
		$getclientname=sql::sql_run("select company from msi_clients where id='$company'");
		$clientname=$getclientname[0][0];
		print "<b>Deleted maintenance contract for $clientname</b><p>";
		contract::maintenance(NULL,NULL);
	} else {
		print "<b>Something went wrong !</b>";
	}
}
/////////////////////////////////////////////////////////////



  function export_client($id, $month, $year){
    	if(!acl::has_access($_SESSION['id'],"contract admin","read")) return 1;
    
    $sql = 'SELECT msi_clients.company,
       msi_billing.date,
       sum(ifnull(msi_billing.bill_hrs,0))as hours,
       msi_billing.nonbill_hrs,
       msi_billing.description,
       msi_billing.invoice,
       msi_users.username,
       msi_billing.id,
       msi_billing.type,
       msi_billing.invoiced_by,
       msi_billing.site,
       msi_billing.after_hour
    
      FROM msi_users, msi_billing,msi_clients
      WHERE msi_clients.id = msi_billing.client AND msi_billing.techie = msi_users.id AND msi_billing.client = '.$id.'
      AND month(msi_billing.date) = '.$month.' and year(msi_billing.date)= '.$year.'
	  And msi_billing.site = \'On-Site\'
	  or msi_billing.after_hour = \'On-Site\'
      group by msi_billing.after_hour,msi_billing.date
      ORDER BY msi_billing.date  ASC';
     
    
    	$results = sql::sql_run($sql);
    
    	// header("Content-Type:application/.xls");
     //header("Content-Disposition: attachment;filename=$results[0][0].'-'.$year.'-'.$month.xls");
     header("Content-Disposition: attachment;filename=$results[0][0].'-'.$year.'-'.$month.pdf");
    	// header("Pragma: no-cache");
    	// header("Expires: 0");
    	//header("Content-Type: application/x-msexcel");
    	header("Content-Type: application/pdf");
    	//header('Content-Disposition: attachment; filename="'.$results[0][0].'-'.$year.'-'.$month.'.xls"');
    	
    	print"<table width = '60%' >
          <tr><td><font size = '2' color = 'blue'>Mindspring Computing<br>
          Unit 5, Doncaster Office Park<br>
          Punters Way, Kenilworth.<br>
          Tel: (021) 657 1780<br>
          Fax: (021) 671 7599
          </font></td><td></td><td></td><td><img  src = 'http://www.mindspring.co.za/LOGO_x_small.jpg'  height ='69' width = '263' valign = 'top'/></td></tr></table><hr/>
          ";
    	print "<h2><CENTER>SUPPORT REPORT FOR &nbsp : &nbsp ".date("F Y", mktime(0, 0, 0, $month,1,$year))."</CENTER></h2>";
    	print "<h3><u><center>".$results[0][0]."</center></u></h3>";
    	print "<div>";
    	print "<table border = '1' width= '60%' >
          <th style ='background-color:silver' >Date </th>
          <th style ='background-color:silver' >Billable Hours</th>";
    	print "  <th style ='background-color:silver' > Time </th>";
    	print"  <th style ='background-color:silver' >&nbsp;&nbsp;&nbsp;Engineer&nbsp;&nbsp;&nbsp; </th>
          <th style ='background-color:silver' >Description  </th>";
    
    	if(is_array($results)){
    		foreach($results as $results){
    			$total += $results[2];
    
    			print"<tr>
    	 <td align= center width = '8%'>".$results[1]."</td>
    	 <td align = center width = 10%>".$results[2]."</td>";
    			print " <td align = center width = 10%>". $results[11]."</td>";
    			print " <td align = center width = 10%>".$results[6]."</td>";
    			print " <td align = center width = 10%>".$results[4]."</td>";
    			print" <tr>";
}
    	}
    	print "<tr style ='background-color:silver'><td align = center><b> Total </b></td>
    <td style ='background-color:silver' align = center><b>".$total."</b></td></tr>";
    	print "</table>";
    	print "</div>";
    	print "<p>&nbsp;</p>";
    
       $sql2 = 'SELECT msi_clients.company,
       msi_billing.date,
       sum(ifnull(msi_billing.bill_hrs,0))as hours,
       msi_billing.nonbill_hrs,
       msi_billing.description,
       msi_billing.invoice,
       msi_users.username,
       msi_billing.id,
       msi_billing.type,
       msi_billing.invoiced_by,
       msi_billing.site,
       msi_billing.after_hour
    
      FROM msi_users, msi_billing,msi_clients
      WHERE msi_clients.id = msi_billing.client AND msi_billing.techie = msi_users.id AND msi_billing.client = '.$id.'
      AND month(msi_billing.date) = '.$month.' and year(msi_billing.date)= '.$year.'
	  And msi_billing.site = \'Remote/Telephonic\'
	  or msi_billing.after_hour = \'Remote/Telephonic\'
      group by msi_billing.after_hour,msi_billing.date
      ORDER BY msi_billing.date  ASC';
     
    
    	$results2 = sql::sql_run($sql2);
    
    	//    header('Content-Disposition: attachment; filename="'.$results[0][0].'-'.$year.'-'.$month.'.xls"');
    	//    print"<table width = '60%' >
    	//    <tr><td><font size = '2' color = 'blue'>Mindspring Computing<br>
    	//    Unit 5, Doncaster Office Park<br>
    	//    Punters Way, Kenilworth.<br>
    	//    Tel: (021) 657 1780<br>
    	//    Fax: (021) 671 7599
    	//    </font></td><td></td><td></td><td><img  src = 'http://www.mindspring.co.za/LOGO_x_small.jpg'  height ='69' width = '263' valign = 'top'/></td></tr></table><hr/>
    	//    ";
    	print "<h2><CENTER>REMOTE REPORT FOR &nbsp : &nbsp ".date("F Y", mktime(0, 0, 0, $month,1,$year))."</CENTER></h2>";
    	print "<h3><u><center>".$results2[0][0]."</center></u></h3>";
    	print "<div>";
    	print "<table border = '1' width= '60%' >
        <th style ='background-color:silver' >Date </th>
        <th style ='background-color:silver' >Billable Hours</th>";
    	print "  <th style ='background-color:silver' > Time </th>";
    	print"  <th style ='background-color:silver' >&nbsp;&nbsp;&nbsp;Engineer&nbsp;&nbsp;&nbsp; </th>
          <th style ='background-color:silver' >Description  </th>";
    
    	if(is_array($results2)){
    		foreach($results2 as $results2){
    			$total2 += $results2[2];
    
    	 print"<tr>
    	 <td align= center width = '8%'>".$results2[1]."</td>
    	 <td align = center width = 10%>".$results2[2]."</td>";
    			print " <td align = center width = 10%>". $results2[11]."</td>";
    			print " <td align = center width = 10%>".$results2[6]."</td>";
    			print " <td align = center width = 10%>".$results2[4]."</td>";
    			print" <tr>";
    }
    	}
    	print "<tr style ='background-color:silver'><td align = center><b> Total </b></td>
        <td style ='background-color:silver' align = center><b>".$total2."</b></td></tr>";
    	print "</table>";
    	print "</div>"; }   
   }
  



function adhoc_clients(){
	?>

	<form name="addtier" action="index.php?module=tier&action=add&id=<?php print $result['company'];?>" method="POST" >
	
			<table>
			<tbody>
			<tr>
			<!-- Account Details -->
			<td valign="top" width="70%">
			<table class="box">
			<caption>Customer Details</caption>
			<tbody>
			<tr>
			<td>
			<label>Company:&nbsp;</label>
			</td>
			<td>
			<select name="company">
			<?php
			foreach ($results as $result) {
			?><option value="<?php
	                           print $result["id"];
	                            ?>"><?php
	                              print $result["company"];
	                            ?></option>
	                            <?php
	                          }
	                        ?>
	              </select>
			    </td>
			   </tr>
			    <tr>
	            <td>
	              <input type="submit" value="add" name="action">
	              <input type="submit" value="cancel" name="action">
	            </td>
	           </tr>
	            </tbody>
	         </table>
	        </form>
	        
                         
 <?php              
 }
 }
}

 
?>


<script type="text/javascript">

	function limitTextOnKeyUpDown(limitField, limitCount, limitNum) {
		if (limitField.value.length > limitNum) {
			limitField.value = limitField.value.substring(0, limitNum);
		} else {
			limitCount.value = limitNum - limitField.value.length;
		}
	}

</script>