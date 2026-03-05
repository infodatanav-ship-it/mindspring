<?php
require_once('mysql.class.php');
class billing {

public function add_form($client = null ,$techie = null ,$date = null ,$rate = null ,$bill_hrs = null ,$nonbill_hrs = null ,$description = null ,
    $arrive_time = null ,$depart_time = null ,$type = null , $travel = null){
if(!billing::instance()->has_access( $_SESSION['muser']['mid'],"billing","add")) return 1;


  if($bill_hrs == null) { $bill_hrs = "0.0"; }
  if($nonbill_hrs == null) { $nonbill_hrs = "0.0"; }
  if($travel == null) { $travel = "0.0"; }
  if(!$client == null) {


  $result3 =  mysql::instance()->run("

                `".mysql::instance()->map["MSICLIENTS"]["TABLE"]."`.
                `".mysql::instance()->map["MSICLIENTS"]["COMPANY"]."`
            FROM
                `".mysql::instance()->map["MSICLIENTS"]["TABLE"]."`
             WHERE
            `".mysql::instance()->map["MSICLIENTS"]["TABLE"]."`.
                `".mysql::instance()->map["MSICLIENTS"]["ID"]."`=
                 '".mysql_real_escape_string($client)."'

        "); 
  } else {
    $result3[0][mysql::instance()->map["MSICLIENTS"]["COMPANY"]] = null;
 }
 
 
  

include_once('content/billing.page.php');
return true;
 
  }
 function add($client,$techie,$date,$rate,$bill_hrs,$nonbill_hrs,$description,$arrive_time,$depart_time,$type,$travel, $site, $afterhour){
  if(!billing::instance()->has_access( $_SESSION['muser']['mid'],"billing","add")) return 1;
   
   $errors = 0;
      
    // do checks to make sure input is valid
    // some hours are billed
     
    if(!is_numeric($bill_hrs) && !is_numeric($nonbill_hrs)){
    	$billn="Hours need to be in numbers";
	$errors++;
    }
    if($bill_hrs <= 0 && $nonbill_hrs <= 0){
    	$billh="You need to bill some hours";
	$errors++;
    
    }

    // There is a company, as with techie you could double check against the client table
    if(!is_numeric($client)){
    	$cl="Please select a client";
	$errors++;

     
    }
    // There is a techie, better method would be to double check in the users table where id =
    if(!is_numeric($techie)){
    	$co="Please select a company";
	$errors++;

     
    }
    // date is not in the future
    $now = time();
    $bill_date_array =  explode("-",$date);
    $bill_date =  date(U, mktime(0, 0, 0, $bill_date_array[1], $bill_date_array[2], $bill_date_array[0]));
      
       if ($bill_date > $now || !is_numeric($bill_date)){
         $fu="You cannot bill for work in the future";
	$errors++;
    }
  
    // the hourly rate is a number
    
    if (!is_numeric($rate)){
    	$ra="Hourly rate needs to be a valid amount, $rate is not valid";
	$errors++;
    }
    // the travel time is a number
    if (!is_numeric($travel)){
    	$tr="The travel time needs to be a valid amount of hours, '$travel' is not valid";
	$errors++;

    }
    // type is set
    if ($type == ""){
    	//print '<div class="bolder">You must select a type.</div>';
        $ty="You must select a type";
	$errors++;
    }
    // the description is long enough
    if (strlen($description) < 12 ){
    	$dsc="The description needs to be longer, please give more details";
	$errors++;
    }
    // arrive and depart times are valid
    // will write this test when we know  how its going to be used.


    // if not valid call add_form again
   
    if ($errors >= 1 ){
     
    $er="The form contains $errors errors, please correct them";
   
      include_once('content/billing.page.php');
      return true;
      

    } else {


      // else insert into db

       mysql::instance('geode')->run("
                        INSERT INTO
                            msi_billing
                        (
                            client,
                            techie,
                            date,
                            rate,
                            bill_hrs,
                            nonbill_hrs,
                            description,
                            arrive_time,
                            depart_time,
                            type,
                            travel,
                            online,
			    after_hour,
			    site
                        )
                        VALUES(
                            '".mysql_real_escape_string($client)."',
                            '".mysql_real_escape_string($techie)."',
                            '".mysql_real_escape_string($date)."',
                            '".mysql_real_escape_string($rate)."',
                            '".mysql_real_escape_string($bill_hrs)."',
                            '".mysql_real_escape_string($nonbill_hrs)."',
                            '".mysql_real_escape_string($description)."',
                            '".mysql_real_escape_string($arrive_time)."',
                            '".mysql_real_escape_string($depart_time)."',
                            '".mysql_real_escape_string($type)."',
                            '".mysql_real_escape_string($travel)."',
                            curdate(),
			    '".$afterhour."',
			    '".$site."'
                        )
                    ");

      
     $mes='Added Billing for '.$_SESSION['muser']['musername'];
                   $year = date(Y);
                   $month = date(n);

                   $techie= $_SESSION['muser']['musername'];
                  billing::instance()->report_techie_month($techie,$year,$month,$mes);



     /*
     print "<script language='javascript'>
                 window.location.href='https://$_SERVER[HTTP_HOST]/?page=billing&action=new&success=1'
                   </script>";
     */
    }

  }

  function report_techie_month($techie,$year,$month,$mes=null){
 if(!billing::instance()->has_access( $_SESSION['muser']['mid'],"billing","read")) return 1;

 $results=mysql::instance('geode')->run("
                SELECT
                    msi_billing.date,
                    msi_clients.company,
                    msi_billing.bill_hrs,
                    msi_billing.nonbill_hrs,
                    msi_billing.invoice,
                    msi_billing.id,
                    msi_billing.travel
                FROM
                    msi_billing,
                    msi_clients,
                    msi_users
                WHERE
                    msi_clients.id = msi_billing.client
                AND
                    msi_billing.techie = msi_users.id
                AND
                    msi_users.username = '".mysql_real_escape_string($techie)."'
                AND
                    month(msi_billing.date) = '".mysql_real_escape_string($month)."'
                AND
                    year(msi_billing.date) =  '".mysql_real_escape_string($year)."'
                ORDER BY
                    msi_billing.date DESC
            ");

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

    print '<div id="display">';

    
billing::instance()->header();

    /*
           if(!empty($_GET[id]))
    {
    print '<i>Edited billing for '.$date.' '.$_GET[id].' '.$_SESSION['muser']['musername'].'.<br></i>';
    }
      */
      
      if(!empty($mes))
      {
   print"<fieldset>
<table>
<tr>
<td colspan=\"2\" align=\"center\" nowrap=\"nowrap\"><font color=\"green\">
$mes
</font>
</td>
</tr>
</table>
</fieldset>
</br>
";
      }
             

    print '<fieldset style="width:100%">';
    print '<table><tr><td colspan="3" align="center">';
    print "<a href=\"?page=billing&action=report&year=$prev_year&month=$prev_month\" style=\"color: #1A53FF\">". date(F, mktime(0, 0, 0, $prev_month, 1, $prev_year)) ." $prev_year << Previous</a> ";
print "&nbsp&nbsp";
    print " <b><a href=?page=billing&action=report&year=$year&month=$month>". date(F, mktime(0, 0, 0, $month, 1, $year)) ." $year</a></b>";
print "&nbsp&nbsp";
    print " <a href=\"?page=billing&action=report&year=$next_year&month=$next_month\" style=\"color: #1A53FF\">Next >> ". date(F, mktime(0, 0, 0, $next_month, 1, $next_year)) ." $next_year</a><p>";
    print '</td></tr><table>';

   print" <h3 id='he3'  style='padding-bottom:5px;  white-space: nowrap; text-align:left'>". date(F, mktime(0, 0, 0, $month, 1, $year)) .' '. $year .' Invoices</h3>

<table class="client">
           <tr  id="th" style="border-left:0.5px solid black"><td id="th">Date</td><td id="th">Company</td>
	   <td id="th" >Billable</td>
	   <td id="th">Non<br>Billable</td>
	   <td id="th">Travel<br>Time</td>
	   <td id="th">Invoiced</td>
	   <td id="th"></td>
       <td id="th"></td>
	   </tr>';

    $total_bill = 0;
    $total_nonbill = 0;

    foreach ($results as $result ){
    $total_bill = $total_bill + $result['bill_hrs'];
    $total_nonbill = $total_nonbill + $result['nonbill_hrs'];
    $total_travel = $total_travel + $result['travel'];
	print "<tr>
	<td id=\"ted\" nowrap=\"nowrap\">".$result['date']."</td>
	<td id=\"ted\" >".$result['company']."</td>
	<td id=\"ted\" nowrap=\"nowrap\">".$result['bill_hrs']."</td>
	<td id=\"ted\" nowrap=\"nowrap\">".$result['nonbill_hrs']."</td>
	<td id=\"ted\" nowrap=\"nowrap\">".$result['travel']."</td>
	<td id=\"ted\" nowrap=\"nowrap\">".$result['invoice']."</td>";
	if ($result['invoice'] == "0000-00-00"){
	  print "<td id=\"ted\"><font color=\"#0d1f65\"><a href=\"?page=billing&action=edit_form&id=".$result["id"]."\">Edit</a></font></td>";
	} else {
	  print "<td id=\"ted\"></td>";
	}
 //  if ($result[mysql::instance('geode')->map["MSIBILLING"]["INVOICE"]] == "0000-00-00"){
     print "<td id=\"ted\"><font color=\"#0d1f65\"><a href='modules/invoice.class.php?action=print&id=".$result["id"]."'>Print</a></font></td>";
 //   }
 //   else {
 //	  print "<td id=\"ted\"></td>";
 //	}
    print "</tr>";
    }
    
	print "<tr>
	<td id=\"ted\"></td>
	<td id=\"ted\">Totals:</td>
	<td id=\"ted\">$total_bill</td>
	<td id=\"ted\">$total_nonbill</td>
	<td id=\"ted\">$total_travel</td>
	<td id=\"ted\"></td>
	<td id=\"ted\"></td>
    <td id=\"ted\"></td>
	</tr>";

    print '</table>';
    print '</fieldset>';

  }
 public  function client_contract_hours($client,$year,$month){
   

   $bhours=mysql::instance('geode')->run("
                SELECT
                    sum(bill_hrs) AS billing
                FROM
                    msi_billing
                WHERE
                    month(date) = '$month'
                AND
                    year(date) = '$year'
                AND
                    client = '$client'
                AND
                    type ='Contract'    
		");
    $resultss= $bhours[0]["billing"];
    
   return  $resultss;
    }

 public function edit_form($id,$billn=null,$billh=null,$cl=null,$dh=null,$ra=null,$tr=null,$ty=null,$dsc=null,$er=null){
 //if(!acl::has_access($_SESSION['id'],"billing","edit")) return 1;

 $results=mysql::instance('geode')->run("
                SELECT
                   *
                FROM
                   msi_billing
                WHERE
                   id = '$id'
                    ");


  if ($results[0]['invoice'] != "0000-00-00"){
    print "<b>This work with id $id has already been billed out on ".$results[0]['invoice']." </b>";
    return;
  }

$result2= mysql::instance('geode')->run("
                SELECT
                   company
                FROM
                   msi_clients
                WHERE
                   id = '".$results[0]['client']."'
                LIMIT 1
                    ");

  /*
  $result2 = sql_run('select company from `msi_clients` where id = '.$results[0]['client'].' limit 1');
  */

  $client_list=mysql::instance('geode')->run("
                SELECT DISTINCT
                    company,
                    id
                FROM
                    msi_clients
                GROUP BY 
		    company
                    ");
     
print '<div id="display">';
billing::instance()->header();
if(!empty($_POST)) {
if($er>='1')
{
print "<fieldset style='width:100%'>
   <table>";
         if(!empty($billn))
         {
print"<tr>
     <td colspan=\"2\" align=\"center\" nowrap=\"nowrap\">
      <font color=\"red\"><b>$billn

      </b></font>
     </td>
    </tr>";
         }
    if(!empty($billh))
    {
   print "<tr>
     <td colspan=\"2\" align=\"center\" nowrap=\"nowrap\">
      <font color=\"red\"><b>$billh

      </b></font>
     </td>
    </tr>";
    }
    if(!empty($cl))
    {
   print"<tr>
     <td colspan=\"2\" align=\"center\" nowrap=\"nowrap\">
      <font color=\"red\"><b>$cl

      </b></font>
     </td>
    </tr>";
    }
    if(!empty($dh))
    {
   print" <tr>
     <td colspan=\"2\" align=\"center\" nowrap=\"nowrap\">
      <font color=\"red\"><b>$dh

      </b></font>
     </td>
    </tr>";
    }
    if(!empty($ra))
    {
print"<tr>
     <td colspan=\"2\" align=\"center\" nowrap=\"nowrap\">
      <font color=\"red\"><b>$ra

      </b></font>
     </td>
    </tr>";
    }
    if(!empty($tr))
    {
print"<tr>
     <td colspan=\"2\" align=\"center\" nowrap=\"nowrap\">
      <font color=\"red\"><b>$tr

      </b></font>
     </td>
    </tr>";
    }
    if(!empty($ty))
    {
print"<tr>
     <td colspan=\"2\" align=\"center\" nowrap=\"nowrap\">
      <font color=\"red\"><b>$ty

      </b></font>
     </td>
    </tr>";
    }
    if(!empty($dsc))
    {
print"<tr>
     <td colspan=\"2\" align=\"center\" nowrap=\"nowrap\">
      <font color=\"red\"><b>$dsc

      </b></font>
     </td>
    </tr>";
    }
    if($er>='1')
    {
print"<tr>
     <td colspan=\"2\" align=\"center\" nowrap=\"nowrap\">
      <font color=\"red\"><b>$er

      </b></font>
     </td>
    </tr>";
    }

  print"</table>
  </fieldset>
 <br/>";
}
}
print '<fieldset style="width:100%">';
print '<h3 id="he3"  style="padding-bottom:5px; text-align:left">Edit Billing</h3>
    <table class="client"><tr><td class="client">
    <form method="POST" action="?page=billing&action=edit&id='.$id.'">
    <table>
      <tr>
        <td nowrap="nowrap">Techie :</td>
	<td><input value="' .$_SESSION['user']['username']. '" name="techie" readonly></td>
      </tr>

      <tr>
        <td nowrap="nowrap">Customer :</td>
	<td><select name="client">
	';
	foreach ($client_list as $client_single) {
	       print "<option value=".$client_single['id']."";
           print "   ";
           if($client_single['company']==$result2[0]['company'])
           {
               print "selected";
           }
           print ">".$client_single['company']."</option>";
	};
	print '</td>
      </tr>
      <tr>
      <td nowrap="nowrap">Date :</td>
      <td><input name="date" value="'.$results[0]['date'].'" size="10"></td>
      </tr>
      <tr>
        <td nowrap="nowrap">Hourly Rate :</td>
	<td><input name="rate" value="'.$results[0]['rate'].'" size="10"> <i>If you are not sure what the quoted amount was please set to 0.0</i>
	</td>
      </tr>
      <tr>
        <td nowrap="nowrap">Type :</td>
	<td><select name="type">
	  <option>'.$results[0]['type'].'</option>
	  <option>Ad Hoc</option>
	  <option>Contract</option>
	  <option>QB Proposal</option>
	  <option>Opal Proposal</option>
	</td>
      </tr>
      <tr>
        <td nowrap="nowrap">Billable Hours :</td>
	<td><input type=text name="bill_hrs" size="6" value="'.$results[0]['bill_hrs'].'"></td>
      </tr>
      <tr>
        <td nowrap="nowrap">Non-Billable Hours :</td>
	<td><input type=text name="nonbill_hrs" size="6" value="'.$results[0]['nonbill_hrs'].'"></td>
      </tr>
      <tr>
        <td nowrap="nowrap">Travel Hours :</td>
	<td><input type=text name="travel" size="6" value="'.$results[0]['travel'].'"></td>
      </tr>
      <tr>
        <td nowrap="nowrap">Arrival Time :</td>
	<td><input type=text name="arrive_time" size="6" value="'.$results[0]['arrive_time'].'"></td>
      </tr>
      <tr>
        <td nowrap="nowrap">Departure Time :</td>
	<td><input type=text name="depart_time" size="6" value="'.$results[0]['depart_time'].'"></td>
      </tr>
      <tr>
        <td valign="top" nowrap="nowrap">Description :</td>
	<td><textarea name="description" cols=70 rows=8 wrap=virtual>'.$results[0]['description'].'</textarea></td>
      </tr>
      <tr>
       <td colspan="2" align="center"><input type="submit" name="edit" value="Submit"><input type="submit" value="Delete" name="delete"  onclick=\'return confirm("Are you sure you want to delete this item?")\'></td>
      </tr>
    </table>
  </td></tr></table></form>';
     
print '</fieldset>';
print '</div>';
  }

function edit($client,$date,$rate,$bill_hrs,$nonbill_hrs,$description,$arrive_time,$depart_time,$type,$travel,$id){
    if(!billing::instance()->has_access( $_SESSION['muser']['mid'],"billing","edit")) return 1;

   $errors = 0;
    

    if(!is_numeric($bill_hrs) && !is_numeric($nonbill_hrs)){
    	 $billn="Hours need to be in numbers";
	$errors++;
    }
    if($bill_hrs <= 0 && $nonbill_hrs <= 0){
    	$billh="You need to bill some hours";
	$errors++;

    }

    // There is a company, as with techie you could double check against the client table
    if(!is_numeric($client)){
    	 $cl="Please select a client";
	$errors++;


    }

    // date is not in the future
    $now = time();
    $bill_date_array =  explode("-",$date);
    $bill_date =  date(U, mktime(0, 0, 0, $bill_date_array[1], $bill_date_array[2], $bill_date_array[0]));
    if ($bill_date > $now || !is_numeric($bill_date)){
    	$dh="You cannot bill for work in the future";
	$errors++;
    }
   

     if (!is_numeric($rate)){
    	$ra="Hourly rate needs to be a valid amount, $rate is not valid";
	$errors++;
    }
    // the travel time is a number
    if (!is_numeric($travel)){
    	$tr="The travel time needs to be a valid amount of hours, '$travel' is not valid";
	$errors++;

    }
    // type is set
    if ($type == ""){
    	$ty="You must select a type";
	$errors++;
    }
    // the description is long enough
    if (strlen($description) < 12 ){
    	$dsc="The description needs to be longer, please give more details";
	$errors++;
    }

    // if not valid call add_form again

    if ($errors >= 1 ){
       $er="The form contains $errors errors, please correct them.<p> ";
       billing::instance()->edit_form($id,$billn,$billh,$cl,$dh,$ra,$tr,$ty,$dsc,$er);
     
    } else {
      // else insert into db



        mysql::instance('geode')->run("
                        UPDATE
                            msi_billing
                        SET
                            client      = '".mysql_real_escape_string($client)."',
                            date        = '".mysql_real_escape_string($date)."',
                            rate        = '".mysql_real_escape_string($rate)."',
                            bill_hrs    = '".mysql_real_escape_string($bill_hrs)."',
                            nonbill_hrs = '".mysql_real_escape_string($nonbill_hrs)."',
                            description = '".mysql_real_escape_string($description)."',
                            arrive_time = '".mysql_real_escape_string($arrive_time)."',
                            depart_time = '".mysql_real_escape_string($depart_time)."',
                            type        = '".mysql_real_escape_string($type)."',
                            travel      = '".mysql_real_escape_string($travel)."'
                        WHERE
                            id          = '".mysql_real_escape_string($id)."'
                        LIMIT 1
                    ");




  
     $mes='Edited billing for client with id '.$id.' on  '.$date.' by  '.$_SESSION['muser']['musername'];
                   $year = date(Y);
                   $month = date(n);
                                     
                   $techie= $_SESSION['muser']['musername'];
                  billing::instance()->report_techie_month($techie,$year,$month,$mes);


       /*

       print "<script language='javascript'>
                 window.location.href='https://$_SERVER[HTTP_HOST]/?page=billing&action=report&id=$id'
                   </script>";
*/
    }

  }


   public function client()
   {
       
 $results= mysql::instance('geode')->run("
            SELECT DISTINCT
                id,
                company
            FROM
                msi_clients
            WHERE
                customer = '1'
            ORDER BY
                company
            ");
       return $results;
   }
public function rate()
{
    $results2= mysql::instance('geode')->run("
            SELECT
                rate
            FROM
                msi_users
            WHERE
                username = '".mysql_real_escape_string($_SESSION['muser']['musername'])."'
            ");

  return $results2[0]['rate'];
}


  public function deletebill($id)
   {
        //if(!billing::instance()->has_access( $_SESSION['muser']['mid'],"billing","delete")) return 1;

        $company=mysql::instance('geode')->run("
		DELETE FROM
                    msi_billing
                WHERE
                    id = '$id'
                ");
                   $year = date(Y);
                   $month = date(n);

                   $techie= $_SESSION['muser']['musername'];
                   $mes='Deleted billing for '.$date.' '.$_GET[id].' '.$_SESSION['muser']['musername'];

                  billing::instance()->report_techie_month($techie,$year,$month,$mes);
       /*
       print "<script language='javascript'>
                 window.location.href='https://$_SERVER[HTTP_HOST]/?page=billing&action=report&id=$id'
                   </script>";
        */

   }
   function has_access($user,$module,$type){

  $mod=strtoupper($type);
  

      $result=mysql::instance('geode')->run("
                SELECT
                    msi_acl.id,
                    msi_module.name,
                    msi_acl.user
                FROM
                    msi_acl,
                    msi_module
                WHERE
                    msi_acl.module = msi_module.id
                AND
                    msi_acl.user = '$user'
                AND
                    msi_acl.$mod = '1'
                AND
                    (msi_module.name='all' 
		  OR
                    msi_module.name ='$module')
            ");

   
  if (count($result) > 0){
    return true;
  } else {
    print "<div id='display'>";
billing::instance()->header();
print "<fieldset style='width:100%'>
<table>
<tr>
<td colspan=\"2\" align=\"center\" nowrap=\"nowrap\"><font color=\"red\">
User ". $_SESSION['muser']['musername']." with id $user, does not have $type access to $module
</font>
</td>
</tr>

<tr>
<td colspan=\"2\" align=\"center\" ><font color=\"red\">
    If you require access to this part of the intranet please copy and paste the error message from above and email it to support@mindspring.co.za with a brief description.<br>
</font>
</td>
</tr>
</table>
</fieldset>
</div>";

    return false;
  }

  }

public function header()
{
    print '<fieldset>
  <table>
  <tr>
<td><b><font color="#0d1f65">
<a href="?page=billing&action=report">MY BILLING</A>
</font>
</b>
</td>

<td><b><font color="#0d1f65">
<a href="?page=billing&action=new">ADD BILL</A>
</font>
</b>
</td>
<td><b><font color="#0d1f65">
<a href="?page=billing&action=billhistory">BILLING HISTORY</A>
</font>
</b>
</td>
</tr>
</table>

   </fieldset>
<br>
</br>
<br>
</br>';
}

public function ginvoice($id)
{

 $results=mysql::instance('geode')->run("
                SELECT
                    *
                FROM
                    msi_billing
                WHERE
                    msi_billing.id = '$id'
                ");
 $result2= mysql::instance('geode')->run("
                SELECT
                    msi_clients.company,
                    msi_clients.saddress1,
                    msi_clients.saddress2,
                    msi_clients.paddress2
                FROM
                    msi_clients
                WHERE
                    msi_clients.id = '".$results[0]['client']."'
                LIMIT 1
                ");

    $dates =  explode("-",$results[0]['date']);
   $cdate=date('d F Y', mktime(0, 0, 0, $dates[1],$dates[2],$dates[0]));
    //$bill_date =  date(U, mktime(0, 0, 0, $bill_date_array[1], $bill_date_array[2], $bill_date_array[0]));
  
   
  
//print "<div id='display'>
//<fieldset>";
    
    $options=file_get_contents('content/invoice.html');
     $options=str_replace("<!--address1 -->",$result2[0]['company'], $options);
     $options=str_replace("<!--address2 -->",$result2[0]['saddress1'], $options);
     $options=str_replace("<!--address3 -->",$result2[0]['saddress2'], $options);
     $options=str_replace("<!--address4 -->",$result2[0]['paddress1'], $options);
     $options=str_replace("<!--description-->",$results[0]['description'], $options);
     $options=str_replace("<!--hours-->",$results[0]['bill_hrs'], $options);
     $options=str_replace("<!--date-->",$cdate, $options);
     $options=str_replace("<!--tech-->", ucwords(strtolower($_SESSION['muser']['musername'])), $options);
     $options=str_replace("<!--arrive-->",$results[0]['arrive_time'], $options);
     $options=str_replace("<!--depart-->",$results[0]['depart_time'], $options);
     $options=str_replace("<!--nonbill-->",$results[0]['nonbill_hrs'], $options);

    require_once("dompdf/dompdf_config.inc.php");
 $dompdf = new DOMPDF();
$dompdf->load_html($options);
$dompdf->render();

$dompdf->stream("invoice.pdf");

    // print_r($options);
    //include('content/invoice.php');
    

//print "
//</fieldset>
//</div>";

/*
 *
 *
require_once('modules/invoice.php');
//Instanciation of inherited class
$pdf=new PDF();
$pdf->AliasNbPages();
//$pdf->AddPage();
$pdf->SetFont('Times','',12);

$pd=$pdf->Output();
return $pd;
 *
 */
}
public function bill_history()
{
    print "<div id='display'>";
billing::instance()->header();
if(!empty($_POST))
{
  print '<fieldset >
   <table width="100%">
    <tr>
     <td colspan="2" align="center">
      <font color="red">';
      print $this->err["mesg"];

     print' </b></font>
     </td>
    </tr>
   </table>
  </fieldset>
 <br /> ';
 
   
}
   print' <fieldset style="width:100%">
 <h3 >Please select client to view billing history</h3>
    <table class="client"><tr><td class="client" >
<tr>
<td>
    <form action="?page=billing&action=viewhist" method="post" name="billhistory" onsubmit="return validhist()">
    <table>

      <tr>
        <td nowrap="nowrap">Customer :</td>
	<td><select name="client">';
     foreach ($results=billing::instance()->client() as $result) {
	      print"<option value=".$result['id'].">";
          print $result['company'];
          print "</option>";

	};

  print'  </select>
	</td>
      </tr>
<tr>
       <td colspan="2" align="center"><input type="submit" value="Submit"></td>
      </tr>

    </table>
    </form>
  </td></tr>

  </table>
  </fieldset>
</div>
';
}

public function selecthistory($id)
{
 $results=mysql::instance('geode')->run("
                SELECT
                    *
                FROM
                    msi_billing
                WHERE
                    client = '$id'
                ORDER BY 
		    date DESC 
		LIMIT 0,10
                ");
    if(count($results)==0)
        {
            $this->err["mesg"]="There is no record associated with the selected company!";
             
             billing::instance()->bill_history();
          
      die;
}
    


$result2= mysql::instance('geode')->run("
                SELECT
                    company
                FROM
                    msi_clients
                WHERE
                    id = '$id'
                LIMIT 1
                ");
   
   require_once('content/billhistory.page.php');
    
}

public function get_techie($id)
{
  $results=mysql::instance('geode')->run("
                SELECT
                    *
                FROM
                    msi_users
                WHERE
                    id = '$id'
                ");

    return $results[0]['username'];

}
 
  public static function instance() {

        static $instance;/*persistant instance varible*/

        if(!isset($instance)) {/*create class if 'instance' was not previously set*/

            $instance = new billing();

        }

        return $instance;/*return the class instance to the caller*/

    }//instance

    private function __construct() {

        $this->err=array(
            "no"      => TRUE,
            "mesg"    => 'Successful',
        );

    }//constructor

    public function __destruct() {

    }//deconstructor

    private $err;

}

?>
