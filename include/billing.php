<?php
// $Id: billing.php.inc,v 1.22 2009/04/09 13:47:17 etienne Exp $

?>
<script type='text/javascript' src='./include/jquery.js'></script>
	<link href='./include/calendar.css' rel='stylesheet' type='text/css'>
        <script type='text/javascript'>
            var getDatee = new Date();
            var monthe = getDatee.getMonth();
            var yeare = getDatee.getFullYear();
            var day = getDatee.getDate();

            function isEmpty(val){
               return (val === undefined || val == null || val.length <= 0) ? true : false;
            }
            
            function prev()
            {
            	monthe = monthe-1;
                if(monthe < 0)
        	{
        	    yeare = yeare-1;	
                    monthe = 11;
                }
                dispCal(monthe, yeare);
                return false;
            }
            
            function next()
            {
            	monthe = monthe+1;
                if(monthe > 11)
        	{
        	    yeare = yeare+1;	
                    monthe = 0;
                }
                dispCal(monthe, yeare);
                return false;
            }
            
            function daysInMonth(monthe, yeare)
            {
                return 32 - new Date(yeare, monthe, 32).getDate();
            }
            function getElementPosition(arrName,arrItem){
                for(var pos=0; pos<arrName.length; pos++ ){
                    if(arrName[pos]==arrItem){
                        return pos;
                    }
                }
            }
            
            function setVal(getDat){
                $('#sel').val(getDat);
                $('#calendar').hide();
            }
            
            function dispCal(mon,yea){
		var ar = new Array('Jan','Feb','Mar','Apr','May','Jun','Jul','Aug','Sep','Oct','Nov','Dec');
                var chkEmpty = isEmpty(mon);
                var n,days,calendar,startDate,newYea,setvale,i;
                if(chkEmpty != true){
                    mon = mon+1;
                    n = ar[mon-1];
                    n += " "+yea;
                    newYea = yea;
                    days = daysInMonth((mon-1),yea);
                    startDate = new Date(ar[mon-1]+" 1"+","+parseInt(yea));
                }else{
                    mon = getElementPosition(ar,ar[getDatee.getMonth()]);
                    n = ar[getDatee.getMonth()];
                    n += " "+yeare;
                    newYea = yeare;
                    days = daysInMonth(mon,yeare);
                    startDate = new Date(ar[mon]+" 1"+","+parseInt(yeare));
                }
                
                var startDay = startDate.getDay();
                var startDay1 = startDay;
                while(startDay> 0){
                   calendar += "<td></td>";  
                   startDay--;
                }                
                i = 1;
                while (i <= days){
                  if(startDay1 > 6){
                      startDay1 = 0;  
                      calendar += "</tr><tr>";  
                  }  
                  mon = monthe+1;
                  setvale = i+","+n;

                  if(mon < 10){
                  mon ="0" + mon; }
                  if(i < 10){
                	  i ="0" + i; }
		  if(i == day && newYea==yeare && mon==monthe){
		   // calendar +="<td class='thisday' onclick='setVal(\""+i+"-"+mon+"-"+newYea+"\")'>"+i+"</td>";
		    calendar +="<td class='thisday' onclick='setVal(\""+newYea+"-"+mon+"-"+i+"\")'>"+i+"</td>";
                  }else{  
                 //   calendar +="<td class='thismon' onclick='setVal(\""+i+"-"+mon+"-"+newYea+"\")'>"+i+"</td>";  
                    calendar +="<td class='thismon' onclick='setVal(\""+newYea+"-"+mon+"-"+i+"\")'>"+i+"</td>";  
                     
                  }
		  startDay1++;  
                  i++;  
                }
        //  calendar +="<td><a style='font-size: 9px; color: #eee; font-family: arial; text-decoration: none;' href='http://www.hscripts.com'>&copy;h</a></td>";   
		
                $('#calendar').css('display','block');
                $('#month').html(n);
                var test = "<tr class='weekdays'><td>Sun</td><td>Mon</td><td>Tue</td><td>Wed</td><td>Thu</td><td>Fri</td><td>Sat</td></tr>";  
                test += calendar;
		$('#dispDays').html(test);
            }
        </script>

   <script type="text/javascript">
    $(document).ready(function(){
        $('input[name="depart_time"]').ptTimeSelect();
    });
    $(document).ready(function(){
      $('input[name="arrive_time"]').ptTimeSelect();
   });

    
</script>

 <script language="javascript">
	   function calculatebilling(){
	  
		   var bill = document.getElementById("bill_hrs").value;
		   var location = document.getElementById("location").value;

        if(location == 'On-Site')
           {
              bills = Math.ceil(bill / 30) * 30; 
              bills = (bills/60).toFixed(2);
             }

    if(location == 'Remote/Telephonic')
             {
            	bills = Math.ceil(bill / 15) * 15;
                bills = (bills/60).toFixed(2);
             }
   if(location == 'Pramod-5mins')
             {
            	 bills = Math.ceil(bill / 5) * 5;
            	// alert(bills);
              bills = (bills/60).toFixed(2);
             }
 document.getElementById("bill_hrs").value = bills;
		   }
</script>   

<script language="javascript">
	   function calculatehours(){
	  
		   var arrive = document.getElementById("arrive_time").value;
		   var leave =document.getElementById("depart_time").value;


			  if(arrive == "24:00" || leave == "24:00" ){
				  alert("Arrival/Departure Time is invalid"); 
			  }
			   else{	  
		   var arrivetyme=arrive.split(':');
		   var leavetyme=leave.split(':');
		  
			   } 	
		  
		   var starttime =  parseInt(arrivetyme[0],10)*60+ parseInt(arrivetyme[1],10);
		 //  alert(starttime);
		   var stoptime = parseInt(leavetyme[0],10)*60+parseInt(leavetyme[1],10);
		  // alert(stoptime);
		   if(starttime == stoptime ){
				  alert("Arrival Time cannot be the same as Departure Time"); 
			  }
		   
		   else{
		   var minsdiff =  stoptime - starttime;
		   }
           if (minsdiff < 0 || starttime > stoptime){
        	   alert("Arrival Time must be less than Departure Time"); 
           }

		   else if(isNaN(starttime) == true || isNaN(stoptime) == true){
			    alert("Enter departure and arrival time in format (h:m)");
		   }
		   else { 
		   var totVal = String(100+Math.floor(minsdiff/60)).substr(1)+'.'+String(100+minsdiff%60).substr(1);
		   document.getElementById("traveling").value = totVal;
		   }

		   var split_total = document.getElementById("traveling").value;
		   var hour_mins = split_total.split('.');
		   var time =  parseInt(hour_mins[0],10)*60+ parseInt(hour_mins[1],10);

		
	     //alert(mins);
	  var location = document.getElementById("location").value;
            if(location == 'Pramod-5mins')
             {
            	time = Math.ceil(time / 5) * 5;
            	time = (time/60).toFixed(2);
              }

            if(location == 'Remote/Telephonic')
            {
            	time = Math.ceil( time / 15) * 15;
            	time = (time/60).toFixed(2);
            }

            if(location == 'On-Site')
            {
                if(totVal <= 1){
            	time = Math.ceil(time / 60) * 60; 
            	time = (time/60).toFixed(2);
             }
          //else{time = totVal;}
                else{
                time = Math.ceil(time / 30) * 30;
                time = (time/60).toFixed(2);}
               }

            else
            {
            	  document.getElementById("bill_hrs").value = '';
            }
		   
            document.getElementById("bill_hrs").value = time;
		   }
		   </script>

<script type = "text/javascript">
function getContract()
{
 
	 var select =  document.getElementById("type").value;
	 if(select == 'Maintenance'){
	 document.getElementById("m_type").style.visibility = "visible";
}
	 else {  document.getElementById("m_type").style.visibility = "hidden";}
  
  
}
function getClientname()
{
     var name =  document.getElementById("client").value;
     document.getElementById("clientname").value = name;
     window.location.href = "?module=billing&action=add_form&client=" + name; 
}

function fillDescription()
{
	 var desc =  document.getElementById("location").value;
	 if(desc == 'Pramod-5mins'){
		 document.getElementById("description").value = "IT and IS Consulting.";
       }
	 else {
		  document.getElementById("description").value = document.getElementById("deschidden").value;
		  }
}

function fillDescription2()
{
 	document.getElementById("deschidden").value = document.getElementById("description").value;	 
}


</script>	
	   	   
<?php 

class billing {

  function navigation(){
	print '<div class="sec-links"><ul>
	       <li><a href="?module=billing&action=report_techie_month">My Billing </a></li>
		   <li><a href="?module=billing&action=maintenance_report_techie_month" >Maintenance Billing</a></li>
		   <li><a href="?module=billing&action=techie_billing" >My Billing Detailed </a></li>
	       <li><a href="?module=billing&action=add_form" >Add</a></li>
           <li><a href="?module=billing&action=contract_report" >Contracts</a></li>
		
		'; 
	   print '</div>';
 }
  
 function add($client,$techie,$date,$rate,$bill_hrs,$nonbill_hrs,$description,$arrive_time,$depart_time,$type,$travel, $site, $afterhour,$quote,$m_type, $billready){
 if(!acl::has_access($_SESSION['id'],"billing","add")) return 1;
 
 
 
    if(!is_numeric($bill_hrs) && !is_numeric($nonbill_hrs)){
    	print '<div class="bolder">Hours need to be in numbers.</div>';
	$errors++;
    }
    if($bill_hrs <= 0 && $nonbill_hrs <= 0){
    	print '<div class="bolder">You need to bill some hours.</div>';
	$errors++;
    }
    // There is a company, as with techie you could double check against the client table
    if(!is_numeric($client)){
    	print '<div class="bolder">Please select a client.</div>';
	$errors++;
    }
    // check if quotation is number
   if(!is_numeric($quote)){
    	print '<div class="bolder">Quotation must be a number.</div>';
	$errors++;
    }
    // There is a techie, better method would be to double check in the users table where id = 
    if(!is_numeric($techie)){
    	print '<div class="bolder">Please select a company.</div>';
	$errors++;
    }
       
   $now = time();
    $bill_date_array =  explode("-",$date);
    $bill_date =  date(U, mktime(0, 0, 0, $bill_date_array[1], $bill_date_array[2], $bill_date_array[0]));
 
    if ($bill_date > $now && $_SESSION['username'] != 'anthea' && $_SESSION['username'] != 'waleed') {
    	print '<div class="bolder">You cannot bill for work in the future.</div>';
	$errors++;
    }  
    
   //if ( $bill_date > $now && $_SESSION['username'] != 'sandiswa'){
   // 	print '<div class="bolder">You cannot bill for work in the futuregjkgjkgljkgjkh.</div>';
	//$errors++;
  // }
    // the hourly rate is a number
    if (!is_numeric($rate)){
    	print '<div class="bolder">Hourly rate needs to be a valid amount, '.$rate.' is not valid .</div>';
	$errors++;
    }
    // the travel time is a number
    if (!is_numeric($travel)){
    	print '<div class="bolder">The travel time needs to be a valid amount of hours, '.$travel.' is not valid .</div>';
	$errors++;

    }    
    // type is set
    if ($type == ""){
    	print '<div class="bolder">You must select a type.</div>';
	$errors++;
    }
    
   if ($billready == ""){
  	$billready = "no";}
    	
    //	else {$billready = "no";
    
    	
    	//print '<div class="bolder">You must select a tickbox.</div>';
    	//print $billready;
    //	$errors++;
  //  }
    
   // $billready = $_POST['bill_ready'];
   // echo $billready;
  //if (strlen(ltrim($m_type)) < 2) {
  //  	print '<div class="bolder"> maintenance type is too short.</div>';
  //  	$errors++;
  //  }

 // the description is long enough
    if (strlen($description) < 12 ){
    	print '<div class="bolder">The description needs to be longer, please give more details.</div>';
	$errors++;
    }
    // arrive and depart times are valid 
    // will write this test when we know  how its going to be used.
    // if not valid call add_form again
    // type is set
    if ($site == ""){
    	print '<div class="bolder">You must select a location.</div>';
    	$errors++;
    }
    if ($errors >= 1 ){
      print "The form contains ".$errors." errors, please correct them. <p> ";
      billing::add_form($client,$techie,$date,$rate,$bill_hrs,$nonbill_hrs,$description,$arrive_time,$depart_time,$type,$travel,$quote,$site,$m_type,$billready);
         } 
   else {
   	
   	if ($type == 'Maintenance'){
  // echo "Select distinct client,type_maintenance from msi_mcontract where client = '$client' and terminating = 'no' and type_maintenance = '$m_type'";
   		$check = sql::sql_run("Select distinct client,type_maintenance from msi_mcontract where client = '$client' and terminating = 'no' and type_maintenance = '$m_type'");
    	$getname = sql::sql_run("Select company from msi_clients where id = '$client'");

if(!$check){
    echo  '<div class="bolder">'.$getname[0][0]." does not have a maintenance contract or the selected type...</div>";
    exit(billing::report_techie_month($_SESSION['username'],date("Y"),date("n")));}

    else{
    
    	foreach ($check as $check){
    	//	echo $check['type_maintenance'].":".$m_type.":<br />";
  
    if($check['type_maintenance']== $m_type){
   //	echo $check['type_maintenance'].":".$m_type.": is the same<br />";
      $sql = 'insert into msi_mbilling set  client = "'.$client.'" , techie = "'.$techie.'", date = "'.$date.'", rate = "'.$rate.'",
      bill_hrs = "'.$bill_hrs.'" , nonbill_hrs = "'.$nonbill_hrs.'" , description = "'.$description.'" , arrive_time = "'.$arrive_time.'",
      depart_time = "'.$depart_time.'", type = "'.$type.'",  travel = "'.$travel.'" ,online = curdate(), after_hour="'.$afterhour.'",quoteno="'.$quote.'", site="'.$site.'" , type_maintenance="'.$m_type.'"';

   	}
   	else {
   //	echo $check['type_maintenance'].":".$m_type.": is not the same<br />";
   		echo  '<div class="bolder">'.$getname[0][0]." does not have the selected type of maintenance contract.</div>";
   		exit(billing::report_techie_month($_SESSION['username'],date("Y"),date("n")));}
   	}
    }
   	}
  else {
  	          
      // else insert into db
      $sql = 'insert into msi_billing set  client = "'.$client.'" , techie = "'.$techie.'", date = "'.$date.'", rate = "'.$rate.'",
      bill_hrs = "'.$bill_hrs.'" , nonbill_hrs = "'.$nonbill_hrs.'" , description = "'.$description.'" , arrive_time = "'.$arrive_time.'",
      depart_time = "'.$depart_time.'", type = "'.$type.'",  travel = "'.$travel.'" ,online = curdate(), after_hour="'.$afterhour.'",quoteno="'.$quote.'", site="'.$site.'", bill_ready ="'.$billready.'"';

     }
     
 
     $result = sql::sql_run($sql);
      print '<i>Added billing for '.$_SESSION['username'].'.<br></i>';
      billing::report_techie_month($_SESSION['username'],date("Y"),date("n"));
      
      
    }
  }
  function invoice_confirm($id){

   $results = sql::sql_run('SELECT msi_clients.company, msi_billing.date, msi_billing.bill_hrs, msi_billing.nonbill_hrs, msi_billing.description, msi_billing.invoice
    ,msi_users.username, msi_billing.id, msi_billing.type, msi_billing.invoiced_by , msi_billing.online, msi_billing.rate, msi_billing.arrive_time, msi_billing.depart_time 
    FROM msi_users, msi_billing, msi_clients
    WHERE msi_clients.id = msi_billing.client AND msi_billing.techie = msi_users.id 
    and msi_billing.invoice = \'0000-00-00\' and msi_billing.id = '.$id);
 
    print "<span class=\"head\">Billing Invoice Table</span>
          <table class=\"client-link\">
	     <tr class=\"head\"><td class=\"client-link\"></td><td class=\"client-link\">Techie</td><td class=\"client-link\">Date (Work)</td><td class=\"client-link\">
Date (Online)</td><td class=\"client-link\">Bill Hours</td><td class=\"client-link\">Non Bill Hours</td>
<td class=\"client-link\">Arrival Time</td>
<td class=\"client-link\">Departure Time</td>
	     <td class=\"client-link\">Type</td><td class=\"client-link\">Rate</td><td class=\"client-link\">Description</td>
	     <td class=\"client-link\">Invoice</td></tr>";
    if(is_array($results )){
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
                              <td class=\"client\">$result[11]</td>
                              <td class=\"client\">$result[4]</td>";
                       print "<td class=\"client\">$result[5]</td>";
	          }
    }
    print "</table><br>";
    print "<p align=\"center\">Click on <a style=\"color:red\" href = \"?module=billing&action=invoice&id=$id\">Submit</a> to invoice this record.&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp<br>";
    print "Click on <a style=\"color:red\" href = \"?module=billing&action=uninvoiced\">Cancel</a> to return to previous screen.</p>";
}
     	
 function invoice($id){
 if(!acl::has_access($_SESSION['id'],"invoice","add")) return 1;

    $sql = 'update msi_billing set invoice = curdate() , invoiced_by = "'.$_SESSION['id'].'" where id = "'.$id.'"';
    $result = sql::sql_run($sql);
    print '<i>Invoice for '.$id.' by '.$_SESSION['username'].'.<br></i>';
    billing::uninvoiced();
  }
  
  function add_form($client = null ,$techie = null ,$date = null ,$rate = null ,$bill_hrs = null ,$nonbill_hrs = null ,$description = null ,
    $arrive_time = null ,$depart_time = null ,$type = null , $travel = null, $quote = null,$site = null,$date = null,$m_type = null,$billready = 'yes'){
 if(!acl::has_access($_SESSION['id'],"billing","add")) return 1;
  $results = sql::sql_run("SELECT DISTINCT company, id
    FROM `msi_clients`
    WHERE customer = \"1\"
    GROUP BY `company`");

  $results2 = sql::sql_run("select rate from msi_users where username = '".$_SESSION['username']."'");
  if($bill_hrs == null){ 
   	$bill_hrs = "0.0"; 
   } 
   
  if($quote == null){
  	$quote = "0000000";
  }

  	   
  if($nonbill_hrs == null) {
  //	$nonbill_hrs = "0.0"; 
  } 
  if($travel == null) { 
  	//$travel = "0.0";
   }
     if($arrive_time == null) {
     //	$arrive_time = "8:00"; 
     }
     if($depart_time == null) {
     	//$depart_time = "17:00";
      }
      if($m_type == null) {
     	$m_type = "";
     }

if(!$client == null) {
 
   $result3 = sql::sql_run("select company from `msi_clients` where id = $client");
 } else {
 	
 $result3[0][0] = null; 
}

if(!isset($_POST['date'])){
$date =date("Y-m-d");
}
else
{
$date = $_POST['date'];

//$date = date("Y-m-d", strtotime($_POST['date']));
}

  print '<span class="head">Add Billing</span>
    <table class="client"><tr><td class="client">
    <form method="POST" action="?module=billing&action=add"> 
      		<input type = "hidden" id = "clientname" name = "clientname" >
    <table>
      <tr>
        <td>Techie :</td>
	<td><input value="'.$_SESSION['username']. '" name="techie" tabindex=7 readonly></td>
      </tr>
      <tr>
        <td>Customer :</td>
	<td><select name="client" id = "client" onchange= "getClientname()">
     		<option value="'.$client.'">'.$result3[0][0].' </option>
  			
	';
	foreach ($results as $result) {
	print "<option value=\"$result[1]\">$result[0]</option>";
	       
	};
print '</td>
      </tr>';
    //  <tr>
     // <td>Date :</td>

//<td><input name="date" id = "datepicker" value="'.$date.'" size="10" tabindex=9></td>
  //   </tr>
print'    			
     <tr> 
    <td>Date :</td>  			
    <td><input name="date" id = "sel" onclick="dispCal()" value="'.$date.'" size="10" readonly="readonly">
    		<img src="./include/calendar.png" onclick="dispCal()" style="cursor: pointer; vertical-align: middle;" />
    		 <table class="calendar" id="calendar" border=0 cellpadding=0 cellspacing=0>
               <tr class="monthdisp">
   		      <td class="navigate" align="left"><img src="./include/previous.png" onclick="return prev()" /></td>
    		 
                <td align="center" id="month"></td>
                <td class="navigate" align="right"><img src="./include/next.png" onclick="return next()" /></td>
           </tr>
            <tr>
                <td colspan=3>
                    <table id="dispDays" border=0 cellpadding=4 cellspacing=4>                        
                    </table>                    
                </td>
            </tr>
        </table>			
</td>			
 </tr>			
<tr>
        <td>Hourly Rate :</td>
	<td><input name="rate" value="'.$results2[0][0].'" size="10" tabindex=10> <i>If you are not sure what the quoted amount was please set to 0.0</i>
	</td>
      </tr>
      <tr>
        <td>Type :</td>
	<td><select name="type" id = "type" onchange = "getContract()">';


//<option>'.$type.'</option>
// <option selected>Contract</option>
//	<option >Maintenance</option>
// <option>Ad Hoc</option>
//	  <option>QB Proposal</option>
//  <option>Mercury Proposal</option>    		
   $type = array('Ad Hoc','Contract','Maintenance','QB Proposal','Opal Proposal','Home User');
if( is_array( $type )) {
	foreach ($type as $type) {
		print '<option value="' . $type . '"';
	//	if ($type==$results[0]['type']) { print " selected"; }
		print ">" . $type . "</option>\n";
	}
}
print '</select>

	</td>
	</tr>';

echo'   
<tr>
        <td>Quote Number:</td>
	<td><input type=text name="quote" size="10" tabindex=12 value="'.$quote.'"></td>
      </tr>

    	 <tr id = "m_type" style= "visibility:hidden">
		
	      <td>Type of Maintenance: </td>';



if(isset($_GET[client])){
	$c = $_GET[client];
}
else {	
	$c = $client;
}
//echo "Select distinct type_maintenance from msi_mcontract where client ='$c'";

 $typem = sql::sql_run("Select distinct type_maintenance from msi_mcontract where client ='$c'");
 if($typem){
echo' <td><select name= "m_type" >';
 foreach($typem as $typem){
 echo' <option>'.$typem[0].'</option>
  ';
 }
  echo '  </select></td>';
  }
  else
  {
  	echo'<td><div class="bolder"> No maintenance contract found for '.$result3[0][0].'</div> </td>';
  }
echo ' </tr>	
<tr>
                          		
 <tr>                             		                            		
<td colspan =2><strong><font size =4px;>Billing</strong><br /></td>';
//<td><strong><font size =4px; >Travel</strong></td> 
print '
 </tr>                             		                            		
<tr>
                              		
	<td>Location</td>
	<td><select name="site" id= "location" onchange = "fillDescription()" tabindex=13>
        <option>'.$site.'</option>
		<option selected>On-Site</option>
		<option>Remote/Telephonic</option>
		<option>Pramod-5mins</option>
	</td>
		</tr>
          <tr>                    		
    <td>Arrival Time :</td>
	<td><input type=text name="arrive_time" id ="arrive_time" size="6" value="'.$arrive_time.'" tabindex=16 title ="Enter departure and arrival time in format (h:m)"><i>Enter departure and arrival time in format (h:m)</i></td>
                              		
	</tr>
   <tr>
   <td>Departure Time :</td>
	<td><input type=text name="depart_time" id ="depart_time" size="6" value="'.$depart_time.'" tabindex=17>
	&nbsp;&nbsp; <input type=button name="calculate" size="6" value="Calculate" onClick = "calculatehours()" tabindex=18>
	</td>		
	
      </tr>
<tr>
    <td>Billable Hours :</td>
     <td><input type=text name="bill_hrs" id ="bill_hrs" size="6" value="'.$bill_hrs.'" tabindex=14></td>';
    // &nbsp;&nbsp; <input type=button name="calculate2" size="6" value="Calculate" onClick = "calculatebilling()">
//
 //   <td>Departure Time :</td>
	//<td><input type=text name="depart_time" id ="depart_time" size="6" value="'.$depart_time.'" tabindex=17>
	//&nbsp;&nbsp; <input type=button name="calculate" size="6" value="Calculate" onClick = "calculatehours()" tabindex=18>
//	</td>		
	
  print'    </tr>
      <tr>
        <td>Non-Billable Hours :</td>
	<td><input type=text name="nonbill_hrs" size="6" value="'.$nonbill_hrs.'" tabindex=15></td>';
   
   //      <td>Travel Hours :</td>
  print'
	<td><input type=hidden name="travel" id ="traveling" size="6" value="'.$travel.'" tabindex=19>
 </td>
      </tr>
 
   </tr>  
      <tr>
	<td valign="top" >Time:</td>
	<td><input type="radio" name="afterhour" value="Office Hours" checked >Office Hours
	<input type="radio" name="afterhour" value="After Hours">After Hours/Saturday
	<input type="radio" name="afterhour" value="Sunday, Public Holiday">Sunday/ Public Holiday
	</td>
	</tr> ';
     // <tr>
     //   <td valign="top" >Description :</td>
	//<td><textarea name="description" id = "description" cols=70 rows=8 tabindex=20 wrap=virtual>'.$description.'</textarea></td>
	//</tr>

     print'  <tr>
        <td valign="top" >Description :</td>
	<td><textarea name="description" id="description" cols=70 rows=8 tabindex=20 wrap=virtual onchange= "fillDescription2()">'.$description.' </textarea> </td>
	<td><input type=hidden name="deschidden" id="deschidden" size="60" value="'.$description.'" tabindex=21></td>	
		</tr>

      <tr>
     <td colspan="2" align="center" align="center"> <b>Ready for Billing:</b> <input type="checkbox" name="billready" id = "billready" value='.$billready.' Checked tabindex=21> &nbsp;&nbsp;&nbsp;<input type="submit" value="Submit" tabindex=22></td>';
       //<td colspan="2" align="center"><input type="submit" value="Submit" tabindex=22></td>
  print '    </tr>
    </table>
  </td></tr></table></form>';
 }
  
  function delete($id,$techie_id){
  if(!acl::has_access($_SESSION['id'],"billing","add")) return 1;
   $result = sql::sql_run('delete from msi_billing where id = "'.$id.'" and techie = "'.$techie_id.'"'); 
   if(!isset($result)){
     print "<b>Deleted billing with ID of $id</b><p>";
      billing::report_techie_month($_SESSION['username'],date("Y"),date("n"));
   } else {
     print "<b>Something went wrong !</b>";
   }
  }
  
  //////////////////////////////////////////////////////////////////////////////
  function delete_maintenance($id,$techie_id){
  	if(!acl::has_access($_SESSION['id'],"billing","add")) return 1;
  	$result = sql::sql_run('delete from msi_mbilling where id = "'.$id.'" and techie = "'.$techie_id.'"');
  	if(!isset($result)){
  		print "<b>Deleted billing with ID of $id</b><p>";
  		billing::maintenance_report_techie_month($_SESSION['username'],date("Y"),date("n"));
  	} else {
  		print "<b>Something went wrong !</b>";
  	}
  }
  
  ////////////////////////////////////////////////////////////////////////////////

  function edit($client,$date,$rate,$bill_hrs,$nonbill_hrs,$description,$arrive_time,$depart_time,$type,$travel,$id,$afterhour,$quote,$site,$m_type,$billready){
  if(!acl::has_access($_SESSION['id'],"billing","edit")) return 1;
    $errors = 0;

    // do checks to make sure input is valid
    // some hours are billed
    if(!is_numeric($bill_hrs) && !is_numeric($nonbill_hrs)){
    	print '<div class="bolder">Hours needs to be in numbers.</div>';
	$errors++;
    }
    if($bill_hrs <= 0 && $nonbill_hrs <= 0){
    	print '<div class="bolder">You need to bill some hours.</div>';
	$errors++;
    }
    
    // There is a company, as with techie you could double check against the client table
    if(!is_numeric($client)){
    	print '<div class="bolder">Please select a client.</div>';
	$errors++;
    }
    
    // date is not in the future
    $now = time();
    $bill_date_array =  explode("-",$date);

    $bill_date =  date(U, mktime(0, 0, 0, $bill_date_array[1], $bill_date_array[2], $bill_date_array[0]));

    if ($bill_date > $now || !is_numeric($bill_date)){
    	print '<div class="bolder">You cannot bill for work in the future.</div>';
	$errors++;
    }
    // the hourly rate is a number
    if (!is_numeric($rate)){
    	print '<div class="bolder">Hourly rate needs to be a valid amount, '.$rate.' is not valid .</div>';
	$errors++;
    }
    // the travel time is a number
    if (!is_numeric($travel)){
    	print '<div class="bolder">The travel time needs to be a valid amount of hours, '.$rate.' is not valid .</div>';
	$errors++;
    }    
    // type is set
    if ($type == ""){
    	print '<div class="bolder">You must select a type.</div>';
	$errors++;
    }
    
    if ($site == ""){
    	print '<div class="bolder">You must select a location.</div>';
    	$errors++;
    }
    // the description is long enough
    if (strlen($description) < 12 ){
    	print '<div class="bolder">The description needs to be longer, please give more details.</div>';
	$errors++;
    }
   
    // arrive and depart times are valid 
    // will write this test when we know  how its going to be used.
    // if not valid call add_form again
  if(isset($billready)){
  	$billready= $_POST['billready'];}
  	
  	if($billready == ""){
  	$billready = 'no';}
  	else{$billready = 'yes';}

 
  
    if ($errors >= 1 ){
      print "The form contains $errors errors, please correct them.<p> ";
      billing::edit_form($id);
   // billing::add_form($client,$techie,$date,$rate,$bill_hrs,$nonbill_hrs,$description,$arrive_time,$depart_time,$type,$travel, $site, $afterhour,$quote);
    } else {
    
    	if($type == 'Maintenance'){
    		
    		if (strlen(ltrim($m_type)) < 2) {
    			print '<div class="bolder"> maintenance type is too short.</div>';
    			$errors++;
    		}
    		if ($errors >= 1 ){
    			print "The form contains $errors errors, please correct them.<p> ";
    			billing::edit_form($id);
    			// billing::add_form($client,$techie,$date,$rate,$bill_hrs,$nonbill_hrs,$description,$arrive_time,$depart_time,$type,$travel, $site, $afterhour,$quote);
    		}
    		else{
    		
    		$check = sql::sql_run("Select distinct client from msi_mcontract where client = '$client' and terminating = 'no' and type_maintenance ='$m_type'");
    		$getname = sql::sql_run("Select company from msi_clients where id = '$client'");
 
    		if(!$check){
    			echo '<div class="bolder">'.$getname[0][0]." does not have a maintenance contract or the selected type...</div>";
    			exit(billing::edit_form($id));
    			
    		}
    		else {
    		billing::add($client,$_SESSION['id'],$date,$rate,$bill_hrs,$nonbill_hrs,$description,$arrive_time,$depart_time,$type,$travel, $site, $afterhour,$quote,$m_type,$billready);
    		//delete billing entry
    		billing::delete($id,$_SESSION['id']);
    		//add entry to normal billing
    		?>
    		<script>
    		window.location.href = "?module=billing&action=report_techie_month";
    		</script>
    		<?php 
    		
    		}
    	}
    	}
     
     $sql = 'update msi_billing set  client = "'.$client.'" , date = "'.$date.'", rate = "'.$rate.'",
       bill_hrs = "'.$bill_hrs.'" , nonbill_hrs = "'.$nonbill_hrs.'" , description = "'.$description.'" , arrive_time = "'.$arrive_time.'",
        depart_time = "'.$depart_time.'", type = "'.$type.'", travel = "'.$travel.'",after_hour = "'.$afterhour.'",quoteno = "'.$quote.'",site = "'.$site.'",bill_ready = "'.$billready.'" where id = "'.$id.'"';

      $result = sql::sql_run($sql);
      print '<i>Edited billing for '.$date.' '.$id.' '.$_SESSION['username'].'.<br></i>';
    
   }
  
  }
  
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////  
  
  
  function edit_maintenance($client,$date,$rate,$bill_hrs,$nonbill_hrs,$description,$arrive_time,$depart_time,$type,$travel,$id,$afterhour,$quote,$site,$m_type){
  	if(!acl::has_access($_SESSION['id'],"billing","edit")) return 1;
  	$errors = 0;

  	if(!is_numeric($bill_hrs) && !is_numeric($nonbill_hrs)){
  		print '<div class="bolder">Hours needs to be in numbers.</div>';
  		$errors++;
  	}
  	if($bill_hrs <= 0 && $nonbill_hrs <= 0){
  		print '<div class="bolder">You need to bill some hours.</div>';
  		$errors++;
  	}
  
  	// There is a company, as with techie you could double check against the client table
  	if(!is_numeric($client)){
  		print '<div class="bolder">Please select a client.</div>';
  		$errors++;
  	}
  
  	// date is not in the future
  	$now = time();
  	$bill_date_array =  explode("-",$date);
  
  	$bill_date =  date(U, mktime(0, 0, 0, $bill_date_array[1], $bill_date_array[2], $bill_date_array[0]));
  
  	if ($bill_date > $now || !is_numeric($bill_date)){
  		print '<div class="bolder">You cannot bill for work in the future.</div>';
  		$errors++;
  	}
  	// the hourly rate is a number
  	if (!is_numeric($rate)){
  		print '<div class="bolder">Hourly rate needs to be a valid amount, '.$rate.' is not valid .</div>';
  		$errors++;
  	}
  	// the travel time is a number
  	if (!is_numeric($travel)){
  		print '<div class="bolder">The travel time needs to be a valid amount of hours, '.$travel.' is not valid .</div>';
  		$errors++;
  	}
  	// type is set
  	if ($type == ""){
  		print '<div class="bolder">You must select a type.</div>';
  		$errors++;
  	}
  
  	if ($site == ""){
  		print '<div class="bolder">You must select a location.</div>';
  		$errors++;
  	}
 
  	// the description is long enough
  	if (strlen($description) < 12 ){
  		print '<div class="bolder">The description needs to be longer, please give more details.</div>';
  		$errors++;
  	}
  	// arrive and depart times are valid
  	// will write this test when we know  how its going to be used.
  	// if not valid call add_form again
  
  	if ($errors >= 1 ){
  		print "The form contains $errors errors, please correct them.<p> ";
  		exit(billing::edit_maintenance_form($id));
  		// billing::add_form($client,$techie,$date,$rate,$bill_hrs,$nonbill_hrs,$description,$arrive_time,$depart_time,$type,$travel, $site, $afterhour,$quote);
  	} else {
  
     if($type == 'Maintenance'){
     	
     //	echo "Select distinct client,type_maintenance from msi_mcontract where client = '$client' and terminating = 'no' and type_maintenance = '$m_type'";
     	$check = sql::sql_run("Select distinct client,type_maintenance from msi_mcontract where client = '$client' and terminating = 'no' and type_maintenance = '$m_type'");
     	$getname = sql::sql_run("Select company from msi_clients where id = '$client'");
   
     	 
     	if(!$check){
     		echo  '<div class="bolder">'.$getname[0][0]." does not have a maintenance contract or the selected type ...</div>";
     		exit(billing::maintenance_report_techie_month($_SESSION['username'],date("Y"),date("n")));}
     	
     		else{
     			foreach ($check as $check){
     			
     	   if($check['type_maintenance']== $m_type){
     	
     	    // else insert into db
  		   $sql = 'update msi_mbilling set  client = "'.$client.'" , date = "'.$date.'", rate = "'.$rate.'",
            bill_hrs = "'.$bill_hrs.'" , nonbill_hrs = "'.$nonbill_hrs.'" , description = "'.$description.'" , arrive_time = "'.$arrive_time.'",
           depart_time = "'.$depart_time.'", type = "'.$type.'", travel = "'.$travel.'",after_hour = "'.$afterhour.'",quoteno = "'.$quote.'",site = "'.$site.'",type_maintenance = "'.$m_type.'" where id = "'.$id.'"';
  	        }
     			else {
     				echo  '<div class="bolder">'.$getname[0][0]." does not have the selected type of maintenance contract.</div>";
     				exit(billing::maintenance_report_techie_month($_SESSION['username'],date("Y"),date("n")));}
     			}
     			}
     			}
     			else {
           //delete maintainance contract entry
  			billing::delete_maintenance($id,$_SESSION['id']);
  			
  			//add entry to normal billing
  			billing::add($client,$_SESSION['id'],$date,$rate,$bill_hrs,$nonbill_hrs,$description,$arrive_time,$depart_time,$type,$travel, $site, $afterhour,$quote,$m_type);
   
  			?>
  			    		<script>
  			    		window.location.href = "?module=billing&action=maintenance_report_techie_month";
  			    		</script>
  			    		<?php 
  	
  		}
  		$result = sql::sql_run($sql);
  		print '<i>Edited billing for '.$date.' '.$id.' '.$_SESSION['username'].'.<br></i>';
  	
  	}
  }
 
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
  
function edit_form($id){
  	 
 if(!acl::has_access($_SESSION['id'],"billing","edit")) return 1;
  $results = sql::sql_run("SELECT * 
    FROM `msi_billing`
    WHERE id = \"$id\"
    ");

 if ($results[0]['invoice'] != "0000-00-00"){
    print "<b>This work with id $id has already been billed out on ".$results[0]['invoice']." </b>";
    return;
  }
  
  $result2 = sql::sql_run('select company from `msi_clients` where id = '.$results[0]['client'].' limit 1');
  $client_list = sql::sql_run("SELECT DISTINCT company, id
    FROM `msi_clients`
    WHERE customer = \"1\"
    GROUP BY `company`");

  print '
 <script type="text/javascript"><!--
				function submit_form (action) {
				if (action == \'delete\') {
				if (!confirm (\'You are about to delete this record!\'))
				return false;
				}
				
				}
				//--></script>
 
  <span class="head">Edit Billing</span>
    <table class="client"><tr><td class="client">
    <form method="POST" action="?module=billing&action=edit&id='.$id.'"> 
    <table>
      <tr>
        <td>Techie :</td>
	<td><input value="' .$_SESSION['username']. '" name="techie" tabindex = 7 readonly></td>
      </tr>
      <tr>
        <td>Customer :</td>
	<td><select name="client" tabindex = 7 ><option value="'.$results[0]['client'].'">'.$result2[0][0].'</option>
	';
	foreach ($client_list as $client_single) {
	       print "<option value=\"$client_single[1]\">$client_single[0]</option>";
	}; 
	print '</td>
      </tr>';
   //   <tr>
 //    <td>Date :</td>';
//  print '<td><input name="date" value="'.$results[0]['date'].'" size="10" id = "datepicker" ></td>';
	//print'  <td><input name="datepicker"  value="'.$results[0]['date'].'" size="10" id = "datepicker" >	</td>';
//print '  </tr>';
                    		
                    		
print'
     <tr>
    <td>Date :</td>
    <td><input name="date" id = "sel" onclick="dispCal()" value="'.$results[0]['date'].'" size="10" readonly="readonly">
    		<img src="./include/calendar.png" onclick="dispCal()" style="cursor: pointer; vertical-align: middle;" />
    		 <table class="calendar" id="calendar" border=0 cellpadding=0 cellspacing=0>
               <tr class="monthdisp">
   		      <td class="navigate" align="left"><img src="./include/previous.png" onclick="return prev()" /></td>
    
                <td align="center" id="month"></td>
                <td class="navigate" align="right"><img src="./include/next.png" onclick="return next()" /></td>
           </tr>
            <tr>
                <td colspan=3>
                    <table id="dispDays" style ="border=1" cellpadding=4 cellspacing=4>
                    </table>
                </td>
            </tr>
        </table>
</td>
 </tr>
                    		

      <tr>
        <td>Hourly Rate :</td>
	<td><input name="rate" value="'.$results[0]['rate'].'" size="10"> <i>If you are not sure what the quoted amount was please set to 0.0</i>
	</td>
      </tr>
      <tr>
        <td>Type :</td>
	<td><select name="type" id = "type" onchange = "getContract()">';

$type = array('Ad Hoc','Contract','Maintenance','QB Proposal','Opal Proposal','Home User');
if( is_array( $type )) {
	foreach ($type as $type) {
		print '<option value="' . $type . '"';
		if ($type==$results[0]['type']) { print " selected"; }
		print ">" . $type . "</option>\n";
	}
}
print "</select>\n";

print'	</td>
      </tr><tr>
        <td>Quote Number :</td>
	<td><input type=text name="quote" size="10" value="'.$results[0]['quoteno'].'"></td>
      </tr>';
  				
  		echo '<tr id = "m_type" style= "visibility:hidden">
<td>Type of Maintenance:</td>';
       $typem = sql::sql_run("Select distinct type_maintenance from msi_mcontract");

       echo' <td><select name= "m_type" >';

 foreach($typem as $typem){
   echo' <option value = "'.$typem['type_maintenance'].'"';
		if($typem['type_maintenance'] == $results[0]['type_maintenance']){ print " selected"; }
		print ">" . $typem['type_maintenance'] . "</option>\n";
  		}
  	
  	print "</select>\n";
  
  //     <td><input type=text name="m_type" size="" value="'.$m_type.' "></td>
  echo ' </td></tr> ';     			
 		
	//  <tr id = "m_type" style= "visibility:hidden">
    ///      <td>Type of Maintenance:</td>
     //    <td><input type=text name="m_type" size="" value="'.$m_type.' "></td>
    //    </tr>		      		
	       		
	       		
echo'     <tr>                             		
                              		
<td colspan =2><strong><font size =4px;>Billing</strong><br /></td>';
print' 
 </tr>                             		
                      		
<tr>
	<td>On /Off site</td>
	<td><select name="site" id = "location" onchange = "fillDescription()" >';


$location = array('On-Site','Remote/Telephonic','Pramod-5mins');
if( is_array( $location )) {
	foreach ($location as $location) {
		print '<option value="' . $location . '"';
		if ($location==$results[0]['site']) { print " selected"; }
		print ">" .$location. "</option>\n";
	}
}
print "</select>\n";
	print '</td>
    </tr>  
    <tr>			
   	<td>Arrival Time :</td>
	<td><input type=text name="arrive_time" id ="arrive_time" size="6" value="'.$results[0]['arrive_time'].'"><i>Enter departure and arrival time in format (h:m)</i></td>	
	</tr> 
     <tr> 			
    <td>Departure Time :</td>
	<td><input type=text name="depart_time" id ="depart_time" size="6" value="'.$results[0]['depart_time'].'">
     &nbsp;&nbsp;<input type=button name="calculate" size="6" value="Calculate" onClick= "calculatehours()"></td>			
    </tr> 			
		
    <tr>
        <td>Billable Hours :</td>
	<td><input type=text id = "bill_hrs" name="bill_hrs" size="6" value="'.$results[0]['bill_hrs'].'"></td>
</tr>
      <tr>
        <td>Non-Billable Hours :</td>
	<td><input type=text name="nonbill_hrs" size="6" value="'.$results[0]['nonbill_hrs'].'"></td>';
    		//<td>Travel Hours :</td>
   print '<td><input type=hidden name="travel" id ="traveling" size="6" value="'.$results[0]['travel'].'" >
    </td>
      </tr>
   <tr><td valign="top">Time:</td>
      <td>';
	$hours = array("Office Hours","After Hours","Sunday, Public Holiday");
	 for($i = 0; $i < 3; $i++){
    
	 if ($hours[$i] != $results[0]['after_hour']){
	    $checked = " ";}
	    else {
	     $checked = "checked";}
	 ?>
	<input type="radio" name="afterhour" value=<?php echo "'$hours[$i]'"; echo $checked;?>><?php echo str_replace(", " , "/",$hours[$i]);?>

	 <?php 
	 } 

	 if($results[0]['bill_ready']== 'yes'){
	 	$ready = 'checked';
	 
	 }
	 else { $ready = ' ';}
        //<tr>
    //    <td valign="top">Description :</td>
	//<td><textarea name="description" id= "description" cols=70 rows=8 wrap=virtual>'.$results[0]['description'].'</textarea></td>
    //  </tr>
//	 <tr>
   //    <td><textarea style="display:none;" name="deschidden" id ="deschidden" cols=70 rows=8 wrap=virtual>'.$results[0]['description'].'</textarea></td>
    
  //    </tr>

 
    print '</td></tr>
      

 <tr>
        <td valign="top">Description :</td>
	<td><textarea name="description" id ="description" cols=70 rows=8 wrap=virtual onchange= "fillDescription2()">'.$results[0]['description'].'</textarea></td>
      </tr>
        		
       <td><input type = "hidden" name="deschidden" id ="deschidden" size="60" value ="'.$results[0]['description'].'"</td>
    
   



       <tr>
       <td colspan="2" align="center">
 <input type="button" value="Back" name="back" onClick="javascript:history.go(-1)"/>&nbsp;&nbsp;&nbsp;
			 <b>Ready for Billing:</b> <input type="checkbox" name="billready" id = "billready" value= "'.$results[0]['bill_ready'].'". '.$ready.' > &nbsp;&nbsp;&nbsp;
			
			<input type="submit" value="Submit"><input type="submit" value="Delete" name="delete" onclick="submit_form (\'delete\')"></td>
			
		
      </tr>
    </table>
  </td></tr></table></form>';

  }


/////////////////////////////////////////////////////////////////////////////////////////////////////////
  function edit_maintenance_form($id){
  
  	if(!acl::has_access($_SESSION['id'],"billing","edit")) return 1;
   
  	$results = sql::sql_run("SELECT *
  			FROM `msi_mbilling`
  			WHERE id = \"$id\"
  			");

if ($results){
	//echo "No results found".$results[0]['invoice'];
  if ($results[0]['invoice'] != "0000-00-00"){
 	print "<b>This work with id $client has already been billed out on ".$results[0]['invoice']." </b>";
			return;
 	}
}
//else {echo  mysql_error();}
  
  	$result2 = sql::sql_run('select company from `msi_clients` where id = '.$results[0]['client'].' limit 1');
    $client_list = sql::sql_run("SELECT DISTINCT company, id
    FROM `msi_clients`
    WHERE customer = \"1\"
    GROUP BY `company`");

  print '
  	<script type="text/javascript"><!--
				function submit_form (action) {
				if (action == \'delete\') {
				if (!confirm (\'You are about to delete this record!\'))
				return false;
				}
  
				}
				//--></script>
  
  <span class="head">Edit Billing</span>
    <table class="client"><tr><td class="client">
    <form method="POST" action="?module=billing&action=edit_maintenance&id='.$id.'">
    <table>
      <tr>
        <td>Techie :</td>
	<td><input value="' .$_SESSION['username']. '" name="techie" tabindex = 7 readonly></td>
      </tr>
      <tr>
        <td>Customer :</td>
	<td><select name="client" tabindex = 7>
           		<option value="'.$results[0]['client'].'">'.$result2[0][0].'</option> ';
	foreach ($client_list as $client_single) {
  	print "<option value=\"$client_single[1]\">$client_single[0]</option>";
  	};
  	print '</td>
  	</tr>';
 // 	<tr>
  //    <td>Date :</td>';
 // print '<td><input name="date" value="'.$results[0]['date'].'" size="10" id = "datepicker" ></td>';
	//print'  <td><input name="datepicker"  value="'.$results[0]['date'].'" size="10" id = "datepicker" >	</td>';
 // 	print '  </tr>
                    		
                  		
  print'
     <tr>
    <td>Date :</td>
    <td><input name="date" id = "sel" onclick="dispCal()" value="'.$results[0]['date'].'" size="10" readonly="readonly">
    		<img src="./include/calendar.png" onclick="dispCal()" style="cursor: pointer; vertical-align: middle;" />
    		 <table class="calendar" id="calendar" border=0 cellpadding=0 cellspacing=0>
               <tr class="monthdisp">
   		      <td class="navigate" align="left"><img src="./include/previous.png" onclick="return prev()" /></td>
    
                <td align="center" id="month"></td>
                <td class="navigate" align="right"><img src="./include/next.png" onclick="return next()" /></td>
           </tr>
            <tr>
                <td colspan=3>
                    <table id="dispDays" style ="border=1" cellpadding=4 cellspacing=4>
                    </table>
                </td>
            </tr>
        </table>
</td>
 </tr>                  		
                    		
              		
    <tr>
  	<td>Hourly Rate :</td>
	<td><input name="rate" value="'.$results[0]['rate'].'" size="10"> <i>If you are not sure what the quoted amount was please set to 0.0</i>
	</td>
      </tr>
      <tr>
        <td>Type :</td>
	<td><select name="type" id = "type" onchange = "getContract()">';
  
$type = array('Ad Hoc','Contract','Maintenance','QB Proposal','Opal Proposal','Home User');
if( is_array( $type )) {
	foreach ($type as $type) {
  	print '<option value="' . $type . '"';
  		if ($type==$results[0]['type']) { print " selected"; }
  		print ">" . $type . "</option>\n";
  		}
  	}
  	print "</select>\n";
  
print'	</td>
  	</tr><tr>
  			<td>Quote Number :</td>
	<td><input type=text name="quote" size="10" value="'.$results[0]['quoteno'].'"></td>
      </tr>';

echo '<tr id = "m_type">
<td>Type of Maintenance:</td>';

       $typem = sql::sql_run("Select distinct type_maintenance from msi_mcontract where client = ".$results[0]['client']);
     
 echo' <td><select name= "m_type" >';
 foreach($typem as $typem){
 	
   echo' <option value = "'.$typem['type_maintenance'].'"';
		if($typem['type_maintenance'] == $results[0]['type_maintenance']){ print " selected"; }
		print ">" . $typem['type_maintenance'] . "</option>\n";
  		}
  	
  	print "</select>\n";

  //     <td><input type=text name="m_type" size="" value="'.$m_type.' "></td>
  echo ' </td></tr> ';     	  		
            	  		
// <tr id = "m_type">
  //			<td>Type of Maintenance:</td>
//	<td><input type=text name="m_type" size="10" value="'.$results[0]['type_maintenance'].'"></td>
  //    </tr>
            	         		
 echo'           		
  	   <tr>
       <td colspan =2><strong><font size =4px;>Billing</strong><br /></td>';
print'
 </tr>
  
<tr>
	<td>On /Off site</td>
	<td><select name="site" id = "location" onchange = "fillDescription()" >';
   
$location = array('On-Site','Remote/Telephonic','Pramod-5mins');
if( is_array( $location )) {
	foreach ($location as $location) {
		print '<option value="' . $location . '"';
  		if ($location==$results[0]['site']) { print " selected"; }
  		print ">" .$location. "</option>\n";
  		}
  		}
  		print "</select>\n";
  		print '</td>
  				</tr>
  				<tr>
  				<td>Arrival Time :</td>
  				<td><input type=text name="arrive_time" id ="arrive_time" size="6" value="'.$results[0]['arrive_time'].'"><i>Enter departure and arrival time in format (h:m)</i></td>
	</tr>
  
    <tr>
    <td>Departure Time :</td>
	<td><input type=text name="depart_time" id ="depart_time" size="6" value="'.$results[0]['depart_time'].'">
     &nbsp;&nbsp;<input type=button name="calculate" size="6" value="Calculate" onClick= "calculatehours()"></td>
    </tr>
  
    <tr>
        <td>Billable Hours :</td>
          <td><input type=text id = "bill_hrs" name="bill_hrs" size="6" value="'.$results[0]['bill_hrs'].'"></td>
</tr>
      <tr>
        <td>Non-Billable Hours :</td>
	<td><input type=text name="nonbill_hrs" size="6" value="'.$results[0]['nonbill_hrs'].'"></td>';
  				//<td>Travel Hours :</td>
   print '<td><input type=hidden name="travel" id ="traveling" size="6" value="'.$results[0]['travel'].'" >
    </td>
  				</tr>
   <tr><td valign="top">Time:</td>
  				<td>';
  				$hours = array("Office Hours","After Hours","Sunday, Public Holiday");
	 for($i = 0; $i < 3; $i++){
  
	 if ($hours[$i] != $results[0]['after_hour']){
	    $checked = " ";}
	    else {
  				$checked = "checked";}
  						?>
  	<input type="radio" name="afterhour" value=<?php echo "'$hours[$i]'"; echo $checked;?>><?php echo str_replace(", " , "/",$hours[$i]);?>
  
  	 <?php 
  	 }  
      print '</td></tr>
        <tr>
          <td valign="top">Description :</td>
  	<td><textarea name="description" id ="description" cols=70 rows=8 wrap=virtual>'.$results[0]['description'].'</textarea></td>
        </tr>
 <tr>
       <td><textarea style="display:none;" name="deschidden" id ="deschidden" cols=70 rows=8 wrap=virtual>'.$results[0]['description'].'</textarea></td>
    
      </tr>


         <tr>
         <td colspan="2" align="center"><input type="submit" value="Submit"><input type="submit" value="Delete" name="delete" onclick="submit_form (\'delete\')"></td>
        </tr>
      </table>
    </td></tr></table></form>';
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  

function billing_edit($id){

  if(!acl::has_access($_SESSION['id'],"billing","edit")) return 1;
  $results = sql::sql_run("SELECT * 
    FROM `msi_billing`
    WHERE id = \"$id\"
    ");

  $result2 = sql::sql_run('select company from `msi_clients` where id = '.$results[0]['client'].' limit 1');
  $client_list = sql::sql_run("SELECT DISTINCT company, id
    FROM `msi_clients`
    WHERE customer = \"1\"
    GROUP BY `company`");

  print '
 <script type="text/javascript"><!--
				function submit_form (action) {
				if (action == \'delete\') {
				if (!confirm (\'You are about to delete this record!\'))
				return false;
				}
				
				}
				//--></script>
 
  <span class="head">Add Billing</span>
    <table class="client"><tr><td class="client">
    <form method="POST" action="?module=billing&action=edit&id='.$id.'"> 
    <input type=\'hidden\' name=\'billing_post\' value=\'true\'>
    <table>
      <tr>
        <td>Techie :</td>
	<td><input value="' .$_SESSION['username']. '" name="techie" readonly></td>
      </tr>
      <tr>
        <td>Customer :</td>
	<td><select name="client"><option value="'.$results[0]['client'].'">'.$result2[0][0].'</option>
	';
	foreach ($client_list as $client_single) {
	       print "<option value=\"$client_single[1]\">$client_single[0]</option>";
	}; 
	print '</td>
      </tr>
      <tr>
      <td>Date :</td>
      <td><input name="date" value="'.$results[0]['date'].'" size="10"></td>
      </tr>
      <tr>
        <td>Hourly Rate :</td>
	<td><input name="rate" value="'.$results[0]['rate'].'" size="10"> <i>If you are not sure what the quoted amount was please set to 0.0</i>
	</td>
      </tr>
      <tr>
        <td>Type :</td>
	<td><select name="type">';
$type = array('Ad Hoc','Contract','QB Proposal','Opal Proposal','Home User');
if( is_array( $type )) {
	foreach ($type as $type) {
		print '<option value="' . $type . '"';
		if ($type==$results[0]['type']) { print " selected"; }
		print ">" . $type . "</option>\n";
	}
}
print "</select>\n";	
	print '</td>
      </tr>
       <tr>
        <td>Quote Number :</td>
	<td><input type=text name="quote" size="10" value="'.$results[0]['quoteno'].'"></td>
      </tr>
      <tr>
        <td>Billable Hours :</td>
	<td><input type=text name="bill_hrs" size="6" value="'.$results[0]['bill_hrs'].'"></td>
      </tr>
      <tr>
        <td>Non-Billable Hours :</td>
	<td><input type=text name="nonbill_hrs" size="6" value="'.$results[0]['nonbill_hrs'].'"></td>
      </tr>
	<tr>
	<td>On /Off site</td>
	<td><select name="site">';
	
	$site = array('On-Site','Remote/Telephonic','Pramod-5mins');
	if( is_array( $site )) {
		foreach ( $site as  $site) {
			print '<option value="' .  $site . '"';
			if ( $site==$results[0]['site']) { print " selected"; }
			print ">" .  $site . "</option>\n";
		}
	}
	print "</select>\n";
	print '</td></tr>			
         <tr>
        <td>Travel Hours :</td>
	<td><input type=text name="travel" size="6" value="'.$results[0]['travel'].'"></td>
      </tr>
      <tr>
        <td>Arrival Time :</td>
	<td><input type=text name="arrive_time" size="6" value="'.$results[0]['arrive_time'].'"></td>
      </tr>
      <tr>
        <td>Departure Time :</td>
	<td><input type=text name="depart_time" size="6" value="'.$results[0]['depart_time'].'"></td>
      </tr>
	
<tr><td valign="top">Time:</td>
      <td>';
	$hours = array("Office Hours","After Hours","Sunday, Public Holiday");
	 for($i = 0; $i < 3; $i++){
    
	 if ($hours[$i] != $results[0]['after_hour']){
	    $checked = " ";}
	    else {
	     $checked = "checked";}
	 ?>
	<input type="radio" name="afterhour" value=<?php echo "'$hours[$i]'"; echo $checked;?>><?php echo str_replace(", " , "/",$hours[$i]);?>
			
 <?php 
	 }     
//<td><input type="radio" name="afterhour" value="Office Hours" checked >Office Hours 
	//<input type="radio" name="afterhour" value="After Hours">After Hours/Saturday
	//<input type="radio" name="afterhour" value="Sunday, Public Holiday">Sunday/ Public Holiday
	//</td>
	print '</td></tr>
	 <tr>
        <td valign="top">Description :</td>
	<td><textarea name="description" cols=70 rows=8 wrap=virtual>'.$results[0]['description'].'</textarea></td>
      </tr>
<tr>
       <td colspan="2" align="center"><input type="submit" value="Submit"><input type="submit" value="Delete" name="delete" onclick="submit_form (\'delete\')"></td>
      </tr>
    </table>
  </td></tr></table></form>';
  }

 function report_client_summary($year,$month){
 if(!acl::has_access($_SESSION['id'],"billing admin","read")) return 1;
   $sql="SELECT msi_clients.company, sum( msi_billing.bill_hrs ) hours,msi_clients.id
   FROM `msi_clients` , msi_billing
   WHERE msi_billing.client = msi_clients.id AND
   MONTH ( msi_billing.date ) = $month AND year( msi_billing.date ) = $year
   GROUP BY msi_billing.client
   ORDER BY `hours` DESC";
  $results = sql::sql_run($sql); 
   print "<span class=\"head\">Client Summary 2</span>
	  				<table class=\"client\">
	  				<tr class=\"head\"><td>Client</td><td>Hours</td></tr>";
   	if(is_array($results)){
	   	foreach ($results as $result) {
                    print "<tr>
                             <td class=\"client\"> <a href = \"?module=billing&action=contract_client&id=$result[2]&year=$year&month=$month\">$result[0]</td>
                             <td class=\"client\">$result[1]</td>
                           </tr>";
	          }
   	}
		  print "</table>";
  }
  
  
  function report_client_summary2($year,$month){
  	if(!acl::has_access($_SESSION['id'],"billing admin","read")) return 1;
  	$sql="SELECT msi_clients.company, sum( msi_billing.bill_hrs ) hours
  	FROM `msi_clients` , msi_billing
  	WHERE msi_billing.client = msi_clients.id AND
  	MONTH ( msi_billing.date ) = $month AND year( msi_billing.date ) = $year
  	GROUP BY msi_billing.client
  	ORDER BY `hours` DESC";
  	$results = sql::sql_run($sql);
  	print "<span class=\"head\">Used Hours</span>
	  				<table class=\"client\">
	  				<tr class=\"head\"><td>Client</td><td>Total Hours Used</td></tr>";
  	if(is_array($results)){
  		foreach ($results as $result) {
  			print "<tr>
  			<td class=\"client\">$result[0]</td>
  			<td class=\"client\">$result[1]</td>
  			</tr>";
  		}
  	}
  	print "</table>";
  }

  function unused_client_summary($year,$month){
  	if(!acl::has_access($_SESSION['id'],"billing admin","read")) return 1;
  	
  	$fulldate=$year.'-'.$month;
  	$fulldate2=$year.'-'.$month.'-'.'00';

  	$sql="SELECT 
  	msi_clients.company,
  	sum( msi_billing.bill_hrs ) hours,
  	msi_contract.hours,
  	msi_contract.server_hours
  	
  	FROM `msi_clients` , msi_billing, msi_contract
  	WHERE msi_billing.client = msi_clients.id 
  	AND msi_contract.client = msi_clients.id
  	AND MONTH( msi_billing.date ) = $month AND year( msi_billing.date ) = $year
  	AND
	(
		('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
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
  
  	GROUP BY msi_billing.client
  	ORDER BY `hours` DESC";

  	$results = sql::sql_run($sql);
  	print "<span class=\"head\">Unsed Hours</span>
	  				<table class=\"client\">
	  				<tr class=\"head\"><td>Client</td><td>Contract Hours</td><td>Unused</td></tr>";
  	if(is_array($results)){
  		foreach ($results as $result) {
  			$tothours = $result[2] + $result[3];
  			$unused = $tothours - $result[1]; 
  			
  			if($unused < 0){
  				$unused = 0;
  			}
  			print "<tr>
  			<td class=\"client\">$result[0]</td>
  			<td class=\"client\">$tothours</td>
  			<td class=\"client\">$unused</td>
  			
  			</tr>";
  		}
  	}
  	print "</table>";
  }
   
  function report_techie_summary($year,$month){
  if(!acl::has_access($_SESSION['id'],"billing","read")) return 1;
  $sql="SELECT msi_users.username, sum( msi_billing.bill_hrs ) hours
   FROM msi_billing , msi_users
   WHERE msi_users.id = msi_billing.techie AND
   MONTH ( msi_billing.date ) = $month AND year( msi_billing.date ) = $year
   GROUP BY msi_billing.techie
   ORDER BY hours DESC";
  
   $results = sql::sql_run($sql); 
   print "<span class=\"head\">Techies</span>
	  				<table class=\"client\">
	  				<tr class=\"head\"><td>Name</td><td>Hours </td></tr>";
   	if(is_array($results)){
	   	foreach ($results as $result) {
                    print "<tr>
                             <td class=\"client\"><a href ='?module=billing_reports&action=techie_billing&year=$year&month=$month&techie=$result[0]'>".ucfirst($result[0])."</a></td>
                             <td class=\"client\">$result[1]</td>
                           </tr>";
	          }
   	}
		  print "</table>";
    }

  function report_month_summary($year,$month){
  if(!acl::has_access($_SESSION['id'],"billing admin","read")) return 1;
  $sql="SELECT msi_users.username, msi_billing.date, msi_clients.company, msi_billing.bill_hrs
    FROM msi_users , msi_billing, msi_clients
    WHERE msi_users.id = msi_billing.techie AND msi_billing.client = msi_clients.id AND
    MONTH (
    msi_billing.date
    ) = $month AND year( msi_billing.date ) = $year
    ORDER BY msi_billing.date DESC , msi_users.username DESC";
  $results = sql::sql_run($sql); 
  print "<span class=\"head\">Billing Table 2 </span>
	  				<table class=\"client\">
	  				<tr class=\"head\"><td>Name</td><td>Date</td><td>Client</td><td>Hours</td></tr>";
   	if(is_array($results)){
	   	foreach ($results as $result) {
                    print "<tr>
                             <td class=\"client\">".ucfirst($result[0])."</td> 
                             <td class=\"client\">$result[1]</td>
                             <td class=\"client\">$result[2]</td>
                             <td class=\"client\">$result[3]</td>
                           </tr>";
	          }
   	}
		  print "</table>";
  }
 
 function hours_total_month($year,$month){
      $sql="SELECT sum(bill_hrs) 
      FROM msi_billing
      WHERE month(date) = $month and year(date)= $year"; 	
      $result = sql::sql_run($sql);
    return $result[0][0];
  }
  
 function report_six_month_summary($year,$month){
 if(!acl::has_access($_SESSION['id'],"billing","read")) return 1;
    print '<span class="head">6 Month Summary</span><table class="client">
  <tr class="head"><td>Month</td><td>Hours</td></tr>';
    $month++;
    for ($i = 0; $i < 6; $i++){
      $month--;
      if ($month == 0 ) {
      	$month = 12;
	$year--;
      }
      $month_name = date(F, mktime(0, 0, 0, $month, 1, $year));
      $hours = billing::hours_total_month($year,$month);
      print "<tr><td class=\"client\">$month_name</td>
             <td class=\"client\">$hours</td>
	     </tr>";
    }
                  print "</table>";
  }

 /////////////////////////////////////////////////////////////////////////////////////////////////////////

  function maintenance_report_techie_month($techie,$year,$month){
  
  	if(!acl::has_access($_SESSION['id'],"billing","read")) return 1;
  	$sql = "SELECT msi_mbilling.date, msi_clients.company, msi_mbilling.bill_hrs, msi_mbilling.nonbill_hrs,
  	msi_mbilling.invoice, msi_mbilling.id, msi_mbilling.travel,msi_users.hrate,msi_mbilling.type,msi_mbilling.type_maintenance
  	FROM msi_mbilling, msi_clients, msi_users
  	WHERE msi_clients.id = msi_mbilling.client AND msi_mbilling.techie = msi_users.id AND msi_users.username = \"$techie\"
  	AND month(msi_mbilling.date) = \"$month\" and year(msi_mbilling.date) = \"$year\" order by msi_mbilling.date";
  
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
  	print "<a href=\"?module=billing&action=maintenance_report_techie_month&year=$prev_year&month=$prev_month\" style=\"color: #1A53FF\">". date(F, mktime(0, 0, 0, $prev_month, 1, $prev_year)) ." $prev_year << Previous</a> ";
  	print "&nbsp&nbsp";
  	print " <b><a href=?module=billing&action=maintenance_report_techie_month&year=$year&month=$month>". date(F, mktime(0, 0, 0, $month, 1, $year)) ." $year</a></b>";
  	print "&nbsp&nbsp";
  	print " <a href=\"?module=billing&action=maintenance_report_techie_month&year=$next_year&month=$next_month\" style=\"color: #1A53FF\">Next >> ". date(F, mktime(0, 0, 0, $next_month, 1, $next_year)) ." $next_year</a><p>";
  	print '</td></tr></table>';
  	
  	print '<table>';
  	print '<span class="head">'. date(F, mktime(0, 0, 0, $month, 1, $year)) .' '. $year .' Maintenance Summary</span>
    		
    <table class="client">
    <tr class="head"><td>Date</td>
     <td>Company </td>
     <td>Type of Maintenance</td>
	   <td>Billable</td>
	   <td>Non Billable</td>
	   <td>Travel Time</td>
	   <td>Invoiced</td>
	   <td></td>
	   </tr>';
  
  	$total_bill = 0;
  	$total_nonbill = 0;
  	if(is_array($results)){
  		foreach ($results as $results ){
  			//if($results[8] == 'Contract'){
  			$bill = $results[2];
  			$total_bill = $total_bill+ $results[2];
  			// }
  			$total_nonbill = $total_nonbill + $results[3];
  			$total_travel = $total_travel + $results[6];
  
  			print "<tr>
  			<td class=\"client\">$results[0]</td>
  			<td class=\"client\">$results[1]</td>
  			<td class=\"client\">$results[9]</td>
  			<td class=\"client\">$bill</td>
  			<td class=\"client\">$results[3]</td>
  			<td class=\"client\">$results[6]</td>
  			<td class=\"client\">$results[4]</td>";
  			if ($results[4] == "0000-00-00"){
  				print "<td class=\"client\"><a href=\"?module=billing&action=edit_maintenance_form&id=$results[5]\">Edit</a></td>";
  			} else {
  				print "<td class=\"client\"></td>";
  			}
  			print "</tr>";
  		}
  	}
  	print "<tr>
  	<td class=\"client\"></td>
  	<td class=\"client\"></td>
  	<td class=\"client\">Totals:</td>
  	<td class=\"client\">$total_bill</td>
  	<td class=\"client\">$total_nonbill</td>
  	<td class=\"client\">$total_travel</td>
  	<td class=\"client\"></td>
  	<td class=\"client\"></td>
  	</tr>";
  
  	print '</table></table>';
  }
  
 ////////////////////////////////////////////////////////////////////////////////////////////////////// 
 
 function report_techie_month($techie,$year,$month,$sort_column = null){

	if(!acl::has_access($_SESSION['id'],"billing","read")) return 1;

	$sql = "SELECT msi_billing.date, msi_clients.company, msi_billing.bill_hrs, msi_billing.nonbill_hrs,
     msi_billing.invoice, msi_billing.id, msi_billing.travel,msi_users.hrate,msi_billing.type
    FROM msi_billing, msi_clients, msi_users
    WHERE msi_clients.id = msi_billing.client AND msi_billing.techie = msi_users.id AND msi_users.username = \"$techie\"
    AND month(msi_billing.date) = \"$month\" and year(msi_billing.date) = \"$year\" order by msi_billing.date";

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
    print "<a href=\"?module=billing&action=report&year=$prev_year&month=$prev_month\" style=\"color: #1A53FF\">". date(F, mktime(0, 0, 0, $prev_month, 1, $prev_year)) ." $prev_year << Previous</a> ";
    print "&nbsp&nbsp";
    print " <b><a href=?module=billing&action=report&year=$year&month=$month>". date(F, mktime(0, 0, 0, $month, 1, $year)) ." $year</a></b>";
    print "&nbsp&nbsp";
    print " <a href=\"?module=billing&action=report&year=$next_year&month=$next_month\" style=\"color: #1A53FF\">Next >> ". date(F, mktime(0, 0, 0, $next_month, 1, $next_year)) ." $next_year</a><p>";
    print '</td></tr></table>';
                     		
   print '<table ><tr><td> 
   <span class="head">'. date(F, mktime(0, 0, 0, $month, 1, $year)) .' '. $year .' Invoices</span>
    <table class="client">                    		
    <tr class="head"><td>Date</td><td>Company </td>
	   <td>Billable</td>
	   <td>Non Billable</td>
	   <td>Travel Time</td>
	   <td>Invoiced</td>
	   <td></td>
	   </tr>';
    
    $total_bill = 0; 	   
    $total_nonbill = 0; 	   
    if(is_array($results)){
    foreach ($results as $results ){
       //if($results[8] == 'Contract'){
        $bill = $results[2];
       $total_bill = $total_bill+ $results[2];
      // }
    $total_nonbill = $total_nonbill + $results[3];
    $total_travel = $total_travel + $results[6];
  
	print "<tr>
	<td class=\"client\">$results[0]</td>
	<td class=\"client\">$results[1]</td>
	<td class=\"client\">$bill</td>
	<td class=\"client\">$results[3]</td>
	<td class=\"client\">$results[6]</td>
	<td class=\"client\">$results[4]</td>";
	if ($results[4] == "0000-00-00"){
	  print "<td class=\"client\"><a href=\"?module=billing&action=edit_form&id=$results[5]\">Edit</a></td>";
	} else {
	  print "<td class=\"client\"></td>";
	}
	print "</tr>";
    }
    }
	print "<tr>
	<td class=\"client\"></td>
	<td class=\"client\">Totals: </td>
	<td class=\"client\">$total_bill</td>
	<td class=\"client\">$total_nonbill</td>
	<td class=\"client\">$total_travel</td>
	<td class=\"client\"></td>
	<td class=\"client\"></td>
	</tr>";
    
    print '</table>
   		</td>';
   	//	<td>&nbsp;</td>
   	//	<td>';
   		
 //  billing::maintenance_report_techie_month($techie,$year,$month);
   		
  // 		print '</td>
   print '</tr>
   		</table>';

  //=====================================================================================================
     $strmonth = '';
     if($month < 10){
          $strmonth = '0'.$month;
}else{
	  $strmonth = $month;
}
$fulldate=$year.'-'.$strmonth;
$fulldate2=$year.'-'.$strmonth.'-'.'00';

if(!isset($sort_column)){
	$sort_column = "msi_clients.company";
}

     $sql="SELECT msi_clients.company,
                  msi_contract.hours,
		          msi_contract.server_hours,
                  msi_contract.client,
                  msi_clients.id,
                  msi_contract.startdate,
                  msi_contract.enddate,
		          msi_contract.primarytech,
		          msi_contract.backuptech,
		          msi_contract.effective,
		          msi_contract.server_techie,
		          msi_contract.notes,
                          msi_contract.acc_manager
		
         FROM     msi_contract, msi_clients
         WHERE    msi_clients.id = msi_contract.client
         AND      (('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
 AND date_format( msi_contract.enddate,'%Y-%m')) or
 (msi_contract.terminating='no'and '$fulldate2'>=date_format(msi_contract.startdate,'%Y-%m-00') )
or (('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
 AND date_format( msi_contract.terminatingdate,'%Y-%m'))
AND msi_contract.terminating='yes')) ORDER BY $sort_column ";

       $results2 = sql::sql_run($sql);
       print "<br><br><br>
        <table><tr><td>";
       print "<span class=\"head\">Contract Client Hours </span>
      
	                    <table class=\"client\">
	                    <tr class=\"head\">
	                    <td class=\"client-link\" style=\"border-left: 1px solid #FFFFFF\">
                        <a href=?module=billing&action=report_techie_month&year=$year&month=$month&sort_column=company>Company</a></td>
               
	                    <td class=\"client-link\" title = \"Contracted hours to be covered per month\" style=\"border-left: 1px solid #FFFFFF\">
	                    <a href=?module=billing&action=report_techie_month&year=$year&month=$month&sort_column=hours>Contracted Hours</a></td>
	                    <td title = \"Total hours that have been used in this month only\"style=\"border-left: 1px solid #FFFFFF\">Used</td>
	                    <td title = \"Hours that have not been covered from this month only\"style=\"border-left: 1px solid #FFFFFF\">Difference</td>
                        <td title = \"Hours left over in current month, taking into consideration carried over hours from previous monthand borrowed from hours from next month\"style=\"border-left: 1px solid #FFFFFF\">Effective</td>
                        <td class=\"client-link\" style=\"border-left: 1px solid #FFFFFF\">
                        <a href=?module=billing&action=report_techie_month&year=$year&month=$month&sort_column=primarytech>Main</a></td>
                        <td  style=\"border-left: 1px solid #FFFFFF\">Notes</td></tr>";
      // <td  style=\"border-left: 1px solid #FFFFFF\">To Carry Over</td>(TO CARRY OVER FIELD)
      
      // Commented out for now: BUSIEST TECHNICIAN PER CUSTOMER 
//<td style=\"border-left: 1px solid #FFFFFF\">Busiest</td>


        // <td class=\"client-link\" style=\"border-left: 1px solid #FFFFFF\">
        // <a href=?module=billing&action=report_techie_month&year=$year&month=$month&sort_column=backuptech>Backup</a></td>
        // <td  class=\"client-link\" style=\"border-left: 1px solid #FFFFFF\">
        // <a href=?module=billing&action=report_techie_month&year=$year&month=$month&sort_column=server_techie>Site Manager</a></td>
        // <td  class=\"client-link\" style=\"border-left: 1px solid #FFFFFF\">
        // <a href=?module=billing&action=report_techie_month&year=$year&month=$month&sort_column=acc_manager>Account Manager</a></td>

      foreach ($results2 as $result) {
         // echo $results[8]." ".$results[7]; 
	 /*	$subsql="SELECT 
			`username`
		FROM 
			`msi_users`
		WHERE id=(SELECT 
			`msi_billing`.`techie`
		FROM 
			`msi_billing` 
		WHERE 
			`msi_billing`.`type` = 'Contract' 
		AND 
			UNIX_TIMESTAMP(`msi_billing`.`date`) >= UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 3 MONTH)) 
		AND
			`msi_billing`.`client`=".$result['3']." 
		GROUP BY `msi_billing`.`techie`
		ORDER BY COUNT(*) DESC
		LIMIT 1
		) 
		";
		$subresults=sql_run($subsql);
		$mytechie=$subresults[0][0]; */
		$maintechie=ucfirst($result[7]);
		$backuptechie=ucfirst($result[8]);
		$available=$result[9];
		$siteman=ucfirst($result[10]);
                $acc_manager=ucfirst($result[12]);
	
	// GET COLOR FOR TECHIE
	/*$colorsql="SELECT `color` FROM `msi_users` WHERE `msi_users`.`username` LIKE '$maintechie'";
	$colorresults=sql_run($colorsql);
	$techiecolor=$colorresults[0][0];
		$maintechie="<font color='".$techiecolor."'>$maintechie</font>";
	$newcolorsql="SELECT `color` FROM `msi_users` WHERE `msi_users`.`username` LIKE '$backuptechie'";
	$newcolorresults=sql_run($newcolorsql);
	$newtechiecolor=$newcolorresults[0][0];
		$backuptechie="<font color='".$newtechiecolor."'>$backuptechie</font>";*/

		//$used = billing::client_contract_hours($result[3],$year,$month);
		
		$usedhours="SELECT sum(ifnull(bill_hrs,0)) 
      FROM msi_billing
      WHERE month(date) = $month and year(date)= $year
      AND client = \"$result[3]\" and upper(type) = 'CONTRACT' "; 
	//	echo 	$usedhours."<br />";	
    $usd = sql::sql_run($usedhours);
    $used = $usd[0][0];

              //20% of the cotract hrs
        $calccaryy=$result[1]*(1/5);
        // $prevmonth=$month-1;
                $prevused = billing::client_contract_hours($result[2],$year,$prev_month);
              
                if(!empty($prevused))
                {
                $prevdiff=$result[1]-$prevused;
                }
                 /*
                $prevmonth=$month-1;
                $prevused = billing::client_contract_hours($result[2],$year,$prevmonth);
                if(!empty($prevused))
                {
                $cforwad=$result[1]-$prevused;}*/
		
		$differenceHours = $result[1] + $result[2];
		$difference = $differenceHours - $used;
		//if ($difference < 0){
		//	$hoursOver = substr($difference, 1);
		//	$difference = "+" . "$hoursOver";
		//}
	 /*$difference = $result[1] - $used;*/

        if($difference==0 && $prevdiff>0)
                {
                  if($prevdiff>$calccaryy)
                    {
                        $cforwad=$calccaryy;
                    }
                    if($prevdiff<$calccaryy)
                    {
                        $cforwad=$prevdiff;
                    }

                }
        if($difference==0 and $prevdiff>0)
        {
        $total=$result[1].'+'. $cforwad;
        $difference=($total+$cforwad)-$result[1];
        }
        else
        {
         $total=$result[1];
        }

if ($_GET['month'] == date(n)) {$myday=date(d);}
else {$myday=30;}
if ($total > 0){
			$deviance=($used*100/$total)-($myday*100/30);}
			if($deviance > 20) { 
				$color='blue';
			}else if($deviance < (-20)) {
				$color='red';
			}else{
				$color='black';
			}

    $effecn = sql::sql_run("SELECT ifnull(msi_close_off.carried,0),ifnull(msi_close_off.borrowed,0)
	from msi_billing,msi_close_off

	where msi_close_off.client = msi_billing.client
	and  month(msi_billing.date) = $month 
	and year(msi_billing.date)= $year
		and  month(msi_close_off.closingmonth) = $month 
	and year(msi_close_off.closingmonth)= $year
     AND msi_billing.client = \"$result[3]\" and upper(msi_billing.type) = 'CONTRACT'");
      
       $effecpre = sql::sql_run("select ifnull(carried,0), ifnull(borrowed,0) from msi_close_off
                      where client = \"$result[3]\" 
                      and month(closingmonth) = $prev_month
                      and year(closingmonth) =  $prev_year");

 $borrnow =$effecn[0][1];
 $borrpre =$effecpre[0][1];
 $leftpre =$effecn[0][0];
   
    $sql4= "SELECT sum(ifnull(msi_close_off.writeoff,0)),msi_close_off.closingmonth,msi_contract.startdate 
           FROM msi_close_off,msi_contract 
           WHERE msi_close_off.client = msi_contract.client
           AND msi_close_off.closingmonth >= msi_contract.startdate 
           AND msi_close_off.closingmonth <= \"$fulldate2\"
           
           AND (('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
           AND date_format( msi_contract.enddate,'%Y-%m')) or
           (msi_contract.terminating='no'and '$fulldate2'>=date_format(msi_contract.startdate,'%Y-%m-00') )
           or (('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
           AND date_format( msi_contract.terminatingdate,'%Y-%m'))
           AND msi_contract.terminating='yes'))
           AND msi_close_off.client = \"$result[3]\" "; 

//echo $sql4;

       
      $write = sql::sql_run($sql4);
      $writeoff = $write[0][0];
   

 $total = "SELECT date_format(msi_contract.startdate,'%Y-%m-%01'), msi_contract.terminating
            FROM msi_contract
            WHERE 
            msi_contract.client = \"$result[3]\"
                  	
           AND (('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
 AND date_format( msi_contract.enddate,'%Y-%m')) or
 (msi_contract.terminating='no'and '$fulldate2'>=date_format(msi_contract.startdate,'%Y-%m-00') )
 or (('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
 AND date_format( msi_contract.terminatingdate,'%Y-%m'))
 AND msi_contract.terminating='yes'))";  

$contract = sql::sql_run($total);
$contract_date = $contract[0][0];
$num = cal_days_in_month(CAL_GREGORIAN, $strmonth, $year); // 31
$ddate=$year.'-'.$strmonth.'-'.$num;
$date1 = date(strtotime($contract_date));
$date2 = date(strtotime($ddate));
$contracttime  = $contract_date ;
$currentdate = $year."-".$strmonth."-".$num;
$d1 = strtotime($contracttime );
$d2 = strtotime($currentdate);
$min_date = min($d1, $d2);
$max_date = max($d1, $d2);
$t = 1;
while (($min_date = strtotime("+ 1 MONTH", $min_date)) <=  $max_date) {
$t++;

} 

$tothrs ="SELECT (ifnull(msi_contract.hours,0) + ifnull(msi_contract.server_hours,0))
                       
                       FROM 
                        msi_contract
                       
                          WHERE 
                  	      msi_contract.client =\"$result[3]\"
                  	      AND (('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
  AND date_format( msi_contract.enddate,'%Y-%m')) or
 (msi_contract.terminating='no'and '$fulldate2'>=date_format(msi_contract.startdate,'%Y-%m-00') )
 or (('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
 AND date_format( msi_contract.terminatingdate,'%Y-%m'))
 AND msi_contract.terminating='yes')) ";
 $hours =  sql::sql_run($tothrs);
 $allhours = $hours[0][0] * $t;
 $tothours = $hours[0][0] ;
 //echo  $currentdate ."<br />".$contracttime . $t."<br /><br />" ;
 
$effecthours ="SELECT ifnull(sum(msi_billing.bill_hrs),0),msi_billing.date, date_format( msi_contract.startdate,'%Y-%m'), msi_contract.enddate
FROM msi_contract,msi_clients,msi_billing
WHERE msi_clients.id = msi_contract.client 
AND msi_billing.client = msi_contract.client 
AND msi_billing.date >= msi_contract.startdate 
AND msi_billing.date <= '$ddate' 
AND msi_billing.client = \"$result[3]\" and upper(msi_billing.type) = 'CONTRACT'
AND (('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
AND date_format( msi_contract.enddate,'%Y-%m')) or
(msi_contract.terminating='no'and '$fulldate2'>=date_format(msi_contract.startdate,'%Y-%m-00') )
or(('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
AND date_format( msi_contract.terminatingdate,'%Y-%m'))
AND msi_contract.terminating='yes'))
AND msi_clients.customer = 1 "; 


$effecinvoice = "SELECT sum(msi_close_off.invoiced),sum(ifnull(msi_close_off.borrowed,0))
FROM msi_close_off,msi_contract 
  
WHERE msi_close_off.client  = \"$result[3]\"
AND msi_close_off.client = msi_contract.client
AND msi_close_off.closingmonth >= msi_contract.startdate 
AND msi_close_off.closingmonth <= '$ddate'
AND (('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
AND date_format( msi_contract.enddate,'%Y-%m')) or
(msi_contract.terminating='no'and '$fulldate2'>=date_format(msi_contract.startdate,'%Y-%m-00') )
or (('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
AND date_format( msi_contract.terminatingdate,'%Y-%m'))
AND msi_contract.terminating='yes'))
"; 

$efehours = sql::sql_run($effecthours); 
   $pinvoiced = sql::sql_run($effecinvoice); 
   $inveffec= $pinvoiced[0][0];
   $peffective = $efehours[0][0];

 $condate= "SELECT month(msi_contract.startdate), year(msi_contract.startdate)
                  FROM
                  msi_contract
                  WHERE 
                  msi_contract.client = \"$result[3]\" ";  
 
$invoicedcurrent = "SELECT sum(ifnull(msi_close_off.invoiced,0))
                   FROM msi_close_off, msi_contract
                   WHERE msi_close_off.closingmonth like \"%$fulldate%\"
                   AND msi_close_off.client = \"$result[3]\" 
                   AND msi_contract.client = msi_close_off.client
                   AND msi_close_off.closingmonth >= msi_contract.startdate";

$invoiced = sql::sql_run($invoicedcurrent);   
$invcd = $invoiced[0][0];

$ddate = sql::sql_run($condate);
$mdate = $ddate[0][0];
$ydate = $ddate[0][1];
$sdate = $ydate."-".$mdate;
$cdate = $year."-".$month;
$effectivenow = 0;

if ($sdate == $cdate){
$differenceprev = 0;
$extra = 0;


//$effectivenow  = $tothours - $used + $invcd + $borrnow ;
$effectivenow  = $tothours - $used + $invcd + $borrnow - $writeoff + $available;
//echo $effectivenow  ."= ".$tothours." - ".$used ."+ ".$invcd ."+ ".$borrnow ." + ". $available  ."- ".$writeoff."<br />";
}
else{

//$effectivenow  = $allhours - $peffective  +   $inveffec + $borrnow ;
$effectivenow  = $allhours - $peffective +  $inveffec + $borrnow - $writeoff + $available ;
//echo $effectivenow ." = ".$allhours." - ".$peffective ." +  ".$inveffec ." + ". $borrnow ."- ". $writeoff."<br />" ;

$differenceprev = $differenceprev ;
}
$bdate =  date('m',strtotime($cdate));
if($bdate > 10){
 $bdate = $year."-".$month;
 }
else {$bdate = $year."-0".$month;} 

if($bdate == date('Y-m')){
//$effectivenow  = $allhours - $peffective - $used +   $inveffec + $borrnow;

}
  if(($used + $borrnow +$borrowprev ) >= $tothours) {
    $carried = 0;
    }
    else
    {
      $carried = $unused; 
    }
$gr = $prev + $borrowprev;
 if($gr >= $tothours) {
 $leftprev = 0;
   
 }
$over = " (Over)";
//$over_contract = "<font color='red'>$over</font>";
//if($effectivenow < 0){$effectivenow = ($effectivenow * -1)."  ".$over_contract;} 
//$over_contract = "<font color='red'>$over</font>";
$effectivenow = number_format($effectivenow, 2);
if($effectivenow < 0){$effectivenow = ($effectivenow * -1)."  ".$over;}
elseif ( $effectivenow > 0  ){$effectivenow =$effectivenow." (Under)";}
           $difference = "<font color='".$color."'>$difference</font>";
			$used = "<font color='".$color."'>$used</font>";
			$effectivenow = "<font color='".$color."'>$effectivenow</font>";
                      print "<tr>
                      <td class=\"client\">$result[0]</td>";
                    //  <td class=\"client\" align=\"center\">$totalHours</td>";
                      print " <td class=\"client\" align=\"center\"> $tothours</td>
                      <td class=\"client\" align=\"center\">$used</td>
                      <td class=\"client\" align=\"center\">$difference</td>
                       <td class=\"client\" align=\"center\">$effectivenow</td>
			          <td class=\"client\" align=\"center\">$maintechie</td>
			          
			          <td style = \"width:300px\" class=\"client\" align=\"center\">$result[11]</td>
			         
			          </tr>";

              //<td class=\"client\" align=\"center\">$effective</td>";(TABLE DATA FOR EFFECTIVE)                   
              // Commented out for now BUSIEST TECHNICIAN PER CUSTOMER
              // <td class=\"client\" align=\"center\">$mytechie</td>		

              // <td class=\"client\" align=\"center\">$backuptechie</td>
              // <td class=\"client\" align=\"center\">$siteman</td>
              // <td class=\"client\" align=\"center\">$acc_manager</td>

}
       print "</table></td><td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td>";
       
//Support Hours Colour Legend
print "<br/>";
	
	print "<td valign = 'top'><table class=\"client\"padding=\"none\"><br />";
	print "<tr><span class=\"head\" align=\"right\">Legend</span></tr>";
	print "<tr><td class=\"client\"><font color=\"red\">Red</font></td><td class=\"client\">Support hours not yet used</td></tr>
	<tr><td class=\"client\"><font color=\"blue\">Blue</font></td><td class=\"client\">Support hours exceed total by more than 20%</td></tr>
	<tr><td class=\"client\">Black</td><td class=\"client\">Support hours within the range of 20% under or over</td></tr>";
    print "</table></td></tr></table>";
  //=====================================================================================================
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
       $sort_column = "invoice";
    
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
    AND msi_billing.invoice != "0000-00-00"
    AND month(msi_billing.date) = '.$month.' and year(msi_billing.date)= '.$year.' ORDER BY '.$sort_column);
/* removed exclusive contract reporting and made adhok entries viewable*/
/* this is the offending SQL code -> and upper(msi_billing.type) = \'CONTRACT\' */
    print '<table><tr><td colspan="3" align="center">';
    print "<a  href=?module=billing&action=invoice_report&year=$prev_year&month=$prev_month style=\"color: #1A53FF\">". date(F, mktime(0, 0, 0, $prev_month, 1, $prev_year)) ." $prev_year << Previous</a> ";
    print "&nbsp&nbsp";
    print " <b><a  href=?module=billing&action=invoice_report&year=$year&month=$month>". date(F, mktime(0, 0, 0, $month, 1, $year)) ." $year</a></b>";
    print "&nbsp&nbsp";   
    print " <a  href=?module=billing&action=invoice_report&year=$next_year&month=$next_month style=\"color: #1A53FF\">Next >> ". date(F, mktime(0, 0, 0, $next_month, 1, $next_year)) ." $next_year</a><p>";
    print '</td></tr></table>';

   print "<span class=\"head\">Billing Invoice Table</span>
         <table class=\"client-link\">
	     <tr class=\"head\"><td class=\"client-link\"><a href=\"?module=billing&sort_column=company&year=$year&month=$month&action=invoice_report\">Company</a></td><td class=\"client-link\"><a href=\"?module=billing&sort_column=username&year=$year&month=$month&action=invoice_report\">Techie</a></td><td class=\"client-link\"><a href=\"?module=billing&sort_column=date&year=$year&month=$month&action=invoice_report\">Date (Work)</a></td><td class=\"client-link\">
<a href=\"?module=billing&sort_column=online&year=$year&month=$month&action=invoice_report\">Date (Online)</a></td><td class=\"client-link\"><a href=\"?module=billing&sort_column=bill_hrs&year=$year&month=$month&action=invoice_report\">Bill Hours</a></td><td class=\"client-link\"><a href=\"?module=billing&sort_column=nonbill_hrs&year=$year&month=$month&action=invoice_report\">Non Bill Hours</a></td>
<td class=\"client-link\"><a href=\"?module=billing&sort_column=arrive&year=$year&month=$month&action=invoice_report\">Arrival Time</a></td>
<td class=\"client-link\"><a href=\"?module=billing&sort_column=depart&year=$year&month=$month&action=invoice_report\">Departure Time</a></td>
	     <td class=\"client-link\"><a href=\"?module=billing&sort_column=type&year=$year&month=$month&action=invoice_report\">Type</a></td>
	     <td class=\"client-link\"><a href=\"?module=billing&sort_column=type&year=$year&month=$month&action=invoice_report\">Quote No</a></td>
	     <td class=\"client-link\"><a href=\"?module=billing&sort_column=rate&year=$year&month=$month&action=invoice_report\">Rate</a></td><td class=\"client-link\"><a href=\"?module=billing&sort_column=rate&year=$year&month=$month&action=invoice_report\">Location</a></td><td class=\"client-link\"><a href=\"?module=billing&sort_column=rate&year=$year&month=$month&action=invoice_report\">Time</a></td><td class=\"client-link\"><a href=\"?module=billing&sort_column=description&year=$year&month=$month&action=invoice_report\">Description</a></td>
	     <td class=\"client-link\"><a href=\"?module=billing&sort_column=invoice&year=$year&month=$month&action=invoice_report\">Invoice</a></td>
         <td class=\"client-link\"><a href=\"?module=billing&sort_column=company&year=$year&month=$month&action=invoice_report\">edit</a></td>
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
                              print "<td class=\"client\"><a href=\"?module=billing&action=invoice_confirm&id=$result[7]\">$result[5]</a></td>";
	           } else {
                              print "<td class=\"client\">$result[5] (".billing::username($result[9]).")</td>";
		   }
                print "<td class=\"client\"><a href=\"?module=billing&action=billing_edit&id=$result[7]\">Edit</a></td>";
	         }
         	}
    print "</table>";

  }
function uninvoiced($sort_column = NULL){
 if(!acl::has_access($_SESSION['id'],"invoice","read")) return 1;

    if(!isset($sort_column)){
    $results = sql::sql_run('SELECT msi_clients.company, msi_billing.date, msi_billing.bill_hrs, msi_billing.nonbill_hrs, msi_billing.description, msi_billing.invoice
    , msi_users.username, msi_billing.id, msi_billing.type, msi_billing.invoiced_by , msi_billing.online, msi_billing.rate, msi_billing.arrive_time, msi_billing.depart_time,msi_billing.site,msi_billing.after_hour
    FROM msi_users, msi_billing, msi_clients
    WHERE msi_clients.id = msi_billing.client AND msi_billing.techie = msi_users.id
    AND INVOICE = "0000-00-00"
    ORDER BY `invoice` ASC');
}else{
 $results = sql::sql_run('SELECT msi_clients.company, msi_billing.date, msi_billing.bill_hrs, msi_billing.nonbill_hrs, msi_billing.description, msi_billing.invoice
    , msi_users.username, msi_billing.id, msi_billing.type, msi_billing.invoiced_by , msi_billing.online, msi_billing.rate, msi_billing.arrive_time, msi_billing.depart_time,msi_billing.site,msi_billing.after_hour
    FROM msi_users, msi_billing, msi_clients
    WHERE msi_clients.id = msi_billing.client AND msi_billing.techie = msi_users.id
    AND INVOICE = "0000-00-00"
    ORDER BY \''.$sort_column.'\'');
}

 print "<span class=\"head\">Still to Be Invoiced</span>
         <table class=\"client-link\">
         <tr class=\"head\"><td class=\"client-link\"><a href=\"?module=billing&sort_column=company&action=uninvoiced\">Company</a></td><td class=\"client-link\"><a href=\"?module=billing&sort_column=techie&action=uninvoiced\">Techie</a></td><td class=\"client-link\"><a href=\"?module=billing&sort_column=date&action=uninvoiced\">Date (Work)</a></td><td class=\"client-link\"><a href=\"?module=billing&sort_column=online&action=uninvoiced\">Date (Online)</a></td><td class=\"client-link\"><a href=\"?module=billing&sort_column=bill_hrs&action=uninvoiced\">Bill Hours</a></td><td class=\"client-link\"><a href=\"?module=billing&sort_column=nonbill_hrs&action=uninvoiced\">Non Bill Hours</a></td>
		 <td class=\"client-link\"><a href=\"?module=billing&sort_column=arrive_time&action=uninvoiced\">Arrival Time</a></td><td class=\"client-link\"><a href=\"?module=billing&sort_column=depart_time&action=uninvoiced\">Departure Time</a></td><td class=\"client-link\"><a href=\"?module=billing&sort_column=type&action=uninvoiced\">Type</a></td><td class=\"client-link\"><a href=\"?module=billing&sort_column=rate&action=uninvoiced\">Rate</a></td><td class=\"client-link\"><a href=\"?module=billing&sort_column=description&action=uninvoiced\">Location</a></td><td class=\"client-link\"><a href=\"?module=billing&sort_column=description&action=uninvoiced\">Time</a></td><td class=\"client-link\"><a href=\"?module=billing&sort_column=description&action=uninvoiced\">Description</a></td>
		 <td class=\"client-link\"
><a href=\"?module=billing&sort_column=invoice&action=uninvoiced\">Invoice</a></td></tr>";
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
                  <td class=\"client\">$result[11]</td>
                  <td class=\"client\">$result[14]</td>
                  <td class=\"client\">$result[15]</td>
                  <td class=\"client\">$result[4]</td>";

                   if ($result[5] == "0000-00-00"){
                              print "<td class=\"client\"><a href=\"?module=billing&action=invoice_confirm&id=$result[7]\">$result[5]</a></td>";
	           } else {
                              print "<td class=\"client\">$result[5] (".billing::username($result[9]).")</td></tr>";
		   }
	         }
    print "</table>";

}

 function contract_report($year,$month){
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
   	msi_contract.hours,
	msi_contract.server_hours,
	msi_contract.client,
    msi_clients.id,
    msi_contract.effective
FROM
	msi_contract,
	msi_clients
WHERE
	msi_clients.id = msi_contract.client
AND
	(('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
AND
	date_format( msi_contract.enddate,'%Y-%m'))
OR
	(msi_contract.terminating='no'and '$fulldate2'>=date_format(msi_contract.startdate,'%Y-%m-00') )
OR
	(('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
AND
	date_format( msi_contract.terminatingdate,'%Y-%m'))
AND
	msi_contract.terminating='yes'))
ORDER BY
	msi_clients.company";
//echo $sql;

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
    print "<a href=?module=billing&action=contract_report&year=$prev_year&month=$prev_month style=\"color: #1A53FF\">". date(F, mktime(0, 0, 0, $prev_month, 1, $prev_year)) ." $prev_year << Previous</a> ";
    print "&nbsp&nbsp";
    print " <b><a href=?module=billing&action=contract_report&year=$year&month=$month>". date(F, mktime(0, 0, 0, $month, 1, $year)) ." $year</a></b>";
    print "&nbsp&nbsp";    
    print " <a href=?module=billing&action=contract_report&year=$next_year&month=$next_month style=\"color: #1A53FF\">Next >> ". date(F, mktime(0, 0, 0, $next_month, 1, $next_year)) ." $next_year</a><p>";
    print '</td></tr>
           </table>';

    print "<i>For a monthly report including all the billing for a client please click on the client name.</i><p>";
         print "<br />";

/* added an extra column here for "Export" */

        print '<table><tr><td>';
    print "<span class=\"head\">Contract Client Hours</span>";

        print " <table class=\"client\">
                <tr class=\"head\"><td>Company</td>
                <td title=\"Contracted hours to be covered per month\" style=\"border-left: 1px solid #FFFFFF\">Contracted Hours</td>
                <td title=\"Hours that have not been covered from previous month only\"style=\"border-left: 1px solid #FFFFFF\">Left Previous</td> 
                <td title=\"Total hours that have been used in this month only\"style=\"border-left: 1px solid #FFFFFF\"> &nbsp;Used&nbsp; </td>
		        <td title=\"Contracted hours that have not been covered from this month only\"style=\"border-left: 1px solid #FFFFFF\">Left Current</td>
		        <td title=\"Hours left over in current month, taking into consideration carried over hours from previous month and borrowed from hours from next month\"style=\"border-left: 1px solid #FFFFFF\">Effective</td>";
		     //   <td style=\"border-left: 1px solid #FFFFFF\">Export</td>
		        print"</tr>";
     
                 foreach ($results as $result) {
            
        /* changed this so that used hours are only shown if specifically billed against the contract:
        ad hock billing will not update the used figure. The variable "$used" initially got it's value from "client_hours" 
        I updated it to get it's value from "client_contract_hours */

	$used = billing::client_contract_hours($result[3],$year,$month);
     //20% of the contract hrs
        $calccaryy=$result[1]*(1/5);
         $prevmonth=$month-1;
                $prevused = billing::client_contract_hours($result[2],$year,$prevmonth);
                if(!empty($prevused))
                {
                $prevdiff=$result[1]-$prevused;
                }
                                                      
	$differenceHours = $result[1] + $result[2];
	$difference = $differenceHours - $used;		

	 /*$difference = $result[1] - $used;*/
        if($difference==0 && $prevdiff>0)
                {

                   if($prevdiff>$calccaryy)
                    {
                        $cforwad=$calccaryy;
                    }
                    if($prevdiff<$calccaryy)
                    {
                        $cforwad=$prevdiff;
                    }

                }
        if($difference==0 and $prevdiff>0)
        {
        $total=$result[1].'+'. $cforwad;
        $difference=($total+$cforwad)-$result[1];
        }
        else
        {
         $total=$result[1];
        }

        /*
		  //$difference = $result[1] - $used;
                $prevmonth=$month-1;
                $prevused = billing::client_contract_hours($result[2],$year,$prevmonth);
                if(!empty($prevused))
                {
                $cforwad=$result[1]-$prevused;

                }
                $difference =($result[1]+ $cforwad) - $used;
        //$total=$result[1]+ $cforwad;
        if(!empty($prevused))
        {
        $total=$result[1].'+'. $cforwad;
        }
        else
        {
         $total=$result[1];
        }*/
        
   //     $available = $result[5];
      
if ($_GET['month'] == date(n)) {$myday=date(d);}
else {$myday=30;}
if($total > 0){
			$deviance=($used*100/$total)-($myday*100/30);}
			if($deviance > 20) { 
				$color='green';
			}else if($deviance < (-20)) {
				$color='red';
			}else{
				$color='black';
			}

			$supportHours = $result[1];
			$serverHours = $result[2];
			$totalHours = $supportHours + $serverHours;
		if($difference < 0 ){$difference = 0;}

	$difference = "<font color='".$color."'>$difference</font>";
			/* added extra column for "export" */
                        /* This is the place that Pramod modified on live geode 05-06-08 <td class=\"client\">
                        <a href=\"?module=billing&action=contract_client&id=$result[3]&year=$year&month=$month\">$result[0]</a> */
                     print "<tr>
                              <td class=\"client\"><a href=\"?module=billing&action=contract_client&id=$result[3]&year=$year&month=$month\" style = \"text-decoration:none\">$result[0]</a>
			      </td>";

    //calculating previous hours

$sql2 = "
SELECT
	msi_clients.company,
   	msi_contract.hours,
	msi_contract.server_hours,
	msi_contract.client,
    msi_clients.id,
    msi_contract.effective	
	
FROM
	msi_contract,
	msi_clients
WHERE
	msi_clients.id = msi_contract.client
	AND msi_clients.company = \"$result[0]\"
AND
	(('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
AND
	date_format( msi_contract.enddate,'%Y-%m'))
OR
	(msi_contract.terminating='no' and '$fulldate2'>=date_format(msi_contract.startdate,'%Y-%m-00') )
OR
	(('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
AND
	date_format( msi_contract.terminatingdate,'%Y-%m'))
AND
	msi_contract.terminating='yes'))
	ORDER BY
	msi_clients.company";

//echo $sql2;
$resul = sql::sql_run($sql2);
		
	foreach ($resul as $resul)	{
	
	$usedprev = billing::client_contract_hours($resul[3],$year,($month-1));
          //20% of the contract hrs
        $calccaryy2=$resul[1]*(1/5);
         $prevmonth=$month-1;
                $previousused = billing::client_contract_hours($resul[2],$year,$prevmonth);
                if(!empty($previousused))
                {
                $prevdifference=$resul[1]-$previousused;
                }
                                                      
	$differenceHoursprev = $resul[1] + $resul[2];
	$differenceprev = $differenceHoursprev - $usedprev;		

	 /*$difference = $result[1] - $used;*/
        if($differenceprev==0 && $prevdifferrence>0)
                {

                   if($prevdifference>$calccaryy2)
                    {
                        $cforwadprev=$calccaryy2;
                    }
                    if($prevdifference<$calccaryy2)
                    {
                        $cforwadprev=$prevdifference;
                    }

                }
        if($differenceprev==0 and $prevdifference>0)
        {
        $totalprev=$resul[1].'+'. $cforwadprev;
        $differenceprev=($totalprev+$cforwadprev)-$resul[1];
        }
        else
        {
         $totalprev=$resul[1];
        }
    
//echo $totalprev."  ";
//echo $differenceprev."</br>";
$effective = $totalHours + $differenceprev ;
$available = $resul[5];
//$toteff = $effective + $difference;

//echo $totalHours."+".$differenceprev."+".$difference." ".$toteff."</br>";
        /* //$difference = $result[1] - $used;
                $prevmonth=$month-1;
                $prevused = billing::client_contract_hours($result[2],$year,$prevmonth);
                if(!empty($prevused))
                {
                $cforwad=$result[1]-$prevused;

                }
                $difference =($result[1]+ $cforwad) - $used;
        //$total=$result[1]+ $cforwad;
        if(!empty($prevused))
        {
        $total=$result[1].'+'. $cforwad;
        }
        else
        {
         $total=$result[1];}*/
//calculating effective hours.


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
           
           AND msi_close_off.client = \"$resul[3]\" "; 
    
 //echo $sql4;
      
      $write = sql::sql_run($sql4);
      $writeoff = $write[0][0];

$effecn = sql::sql_run("select ifnull(carried,0), ifnull(borrowed,0) from msi_close_off
                      where client = \"$resul[3]\" 
                      and month(closingmonth) =$month
                      and year(closingmonth) = $year");

$effeprev = sql::sql_run("select ifnull(carried,0), ifnull(borrowed,0) from msi_close_off 
                      where client = \"$resul[3]\" 
                      and month(closingmonth) =$prev_month
                      and year(closingmonth) = $prev_year");

$borrnow = $effecn[0][1];
$borrprev = $effeprev[0][1];
if($differenceprev < 0){
$differenceprev = 0;}


//get contract start date.
$total = "SELECT date_format(msi_contract.startdate,'%Y-%m-%01'), msi_contract.terminating
          FROM msi_contract
          WHERE 
          msi_contract.client = \"$result[3]\"
         and (('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
 and date_format( msi_contract.enddate,'%Y-%m')) or
 (msi_contract.terminating='no'and '$fulldate2'>=date_format(msi_contract.startdate,'%Y-%m-00'))
 or ('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
 and date_format( msi_contract.terminatingdate,'%Y-%m') and terminating='yes') )
                  	
                 ";


$contract = sql::sql_run($total);
$contract_date = $contract[0][0];
$num = cal_days_in_month(CAL_GREGORIAN, $strmonth, $year); // 31
$ddate=$year.'-'.$strmonth.'-'.$num;
$date1 = date(strtotime($contract_date));
$date2 = date(strtotime($ddate));
//$diff = $date2 - $date1;
//$month = floor($difference / 86400 / 30 )+ 1;
//$months = round($difference / 86400 / 30 );

$contracttime  = $contract_date ;
$currentdate = $year."-".$strmonth."-".$num;

$d1 = strtotime($contracttime );
$d2 = strtotime($currentdate);
$min_date = min($d1, $d2);
$max_date = max($d1, $d2);
$t = 1;
while (($min_date = strtotime("+1 MONTH", $min_date)) <=  $max_date) {
 
$t++; 
} 
 //echo $contracttime." : ".$currentdate."<br />";
$tothrs ="SELECT (ifnull(msi_contract.hours,0) + ifnull(msi_contract.server_hours,0))
                       
                       FROM 
                       msi_contract
                       WHERE 
                  	   msi_contract.client =\"$result[3]\"
                  	   and (('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
 and date_format( msi_contract.enddate,'%Y-%m')) or
 (msi_contract.terminating='no'and '$fulldate2'>=date_format(msi_contract.startdate,'%Y-%m-00'))
 or ('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
 and date_format( msi_contract.terminatingdate,'%Y-%m') and terminating='yes') )";
$hours =  sql::sql_run($tothrs);
//$allhours = $hours[0][0] * $months;
$allhours = $hours[0][0] * $t;
$tothours = $hours[0][0] ;
//echo $t." :t <br />";
//echo  $currentdate ."<br />".$contracttime . $t."<br /><br />" ;

$effecthours ="SELECT ifnull(sum(msi_billing.bill_hrs),0),msi_billing.date, date_format( msi_contract.startdate,'%Y-%m'), msi_contract.enddate
FROM msi_contract,msi_clients,msi_billing
WHERE msi_clients.id = msi_contract.client 
AND msi_billing.client = msi_contract.client 
AND msi_billing.date >= msi_contract.startdate 
AND msi_billing.date <= '$ddate' 
AND msi_billing.client = \"$result[3]\" and upper(msi_billing.type) = 'CONTRACT'  
and (('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
and date_format( msi_contract.enddate,'%Y-%m')) or
(msi_contract.terminating='no'and '$fulldate2'>=date_format(msi_contract.startdate,'%Y-%m-00'))
or ('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
and date_format( msi_contract.terminatingdate,'%Y-%m') and terminating='yes') )
AND msi_clients.customer = 1 "; 
//echo $effecthours;

$effecinvoice = "SELECT sum(msi_close_off.invoiced),sum(ifnull(msi_close_off.borrowed,0))
FROM msi_close_off,msi_contract  
WHERE msi_close_off.client  = \"$result[3]\"
AND msi_close_off.client = msi_contract.client
AND msi_close_off.closingmonth >= msi_contract.startdate 
AND msi_close_off.closingmonth <= '$ddate'
and (('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
and date_format( msi_contract.enddate,'%Y-%m')) or
(msi_contract.terminating='no'and '$fulldate2'>=date_format(msi_contract.startdate,'%Y-%m-00'))
or ('$fulldate' between date_format( msi_contract.startdate,'%Y-%m')
and date_format( msi_contract.terminatingdate,'%Y-%m') and terminating='yes') )

";
   $efehours = sql::sql_run($effecthours); 
   $pinvoiced = sql::sql_run($effecinvoice); 
   $inveffec= $pinvoiced[0][0];
   $peffective = $efehours[0][0];

   $condate= "SELECT month(msi_contract.startdate), year(msi_contract.startdate)
                  FROM  msi_contract
                  WHERE msi_contract.client = \"$result[3]\"";  

	
   $invoicedcurrent = "SELECT sum(ifnull(msi_close_off.invoiced,0))
                   FROM msi_close_off, msi_contract
                   WHERE msi_close_off.closingmonth like \"%$fulldate%\"
                   AND msi_close_off.client = \"$result[3]\" 
                   AND msi_contract.client = msi_close_off.client
                   AND msi_close_off.closingmonth >= msi_contract.startdate";

$invoiced = sql::sql_run($invoicedcurrent);   
$invcd = $invoiced[0][0];
$ddate = sql::sql_run($condate);
$mdate = $ddate[0][0];
$ydate = $ddate[0][1];
$sdate = $ydate."-".$mdate;
$cdate = $year."-".$month;
$effectivenow = 0;

if ($sdate == $cdate){
$differenceprev = 0;
$extra = 0;
//$effectivenow  = $tothours - $used + $invcd + $borrnow ;
$effectivenow  = $tothours - $used + $invcd + $borrnow + $available - $writeoff;
//echo $effectivenow ." = ".$tothours." - ".$used." + ".$invcd." + ".$borrnow." + ".$available."  - ".$writeoff."<br />";
}

else{
$invcd = $invcd ;
//$effectivenow  = $allhours - $peffective +   $inveffec+ $borrnow ;
$effectivenow  = $allhours - $peffective +  $inveffec + $borrnow - $writeoff + $available ;
//echo $effectivenow ." 444444= ".$allhours." - ".$peffective." + ". $inveffec." + ". $borrnow." - ".$writeoff." + ".$available ."<br />";
$differenceprev = $differenceprev ;
}
$bdate =  date('m',strtotime($cdate));
if($bdate > 10){
 $bdate = $year."-".$month;
 }
else {$bdate = $year."-0".$month;} 
if($bdate == date('Y-m')){
//$effectivenow  = $allhours - $peffective - $used +   $inveffec  + $borrnow;
}

   if(($used + $borrnow +$borrowprev ) >= $totalHours) {
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
if($effectivenow < 0){$effectivenow = ($effectivenow * -1)."  ".$over_contract;}   

		$effecnow = "<font color='".$color."'>$effecnow</font>";
		$differenceprev = "<font color='".$color."'>$differenceprev</font>";
		$used = "<font color='".$color."'>$used</font>";
		$effective = "<font color='".$color."'>$effectivenow</font>";
		print "<td class=\"client\" align=\"center\">$totalHours</td>
        <td class=\"client\" align=\"center\">$differenceprev</td>
        <td class=\"client\" align=\"center\">$used</td>
        <td class=\"client\" align=\"center\">$difference</td>
        <td class=\"client\" align=\"center\">$effectivenow</td>";

	}
 //print"<td class=\"client\"><a href=\"export.php?module=billing&action=export_client&id=$result[3]&year=$year&month=$month\">(export)</a></td>
 
 print"</tr>";
	         }              
    print "</table>";
    print "</td><td>&nbsp;&nbsp;&nbsp;&nbsp;</td>";
    print "<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td>"; 
    print "<td valign = \"top\"><br />";
	print "<table width = '400' class=\"client\"padding=\"none\" >";
	print "<tr><span class=\"head\" align=\"right\">Fields Description</span></tr>";
	print "<tr><td class=\"client\"><font color=\"blue\">Contracted Hours</font></td><td class=\"client\">Contracted hours to be covered per month</td></tr>
	<tr><td width = '90' class=\"client\"><font color=\"blue\">Left Previous</font></td><td class=\"client\"> Hours that have not been covered from previous month only</td></tr>
	<tr><td class=\"client\"><font color=\"blue\">Used </font></td><td class=\"client\">Total hours that have been used in this month only</td></tr>
	<tr><td class=\"client\"><font color=\"blue\">Left Current</font></td><td class=\"client\"> Contracted hours that have not been covered from this month only</td></tr>
	<tr><td class=\"client\"><font color=\"blue\">Effective </font></td><td class=\"client\">Hours left over in current month, taking into consideration carried over hours from previous month and borrowed from hours from next month</td></tr>
	</tr>";
    print "</table><br>
    <p>&nbsp;</p>
    <table width = '400'class=\"client\"padding=\"none\">";
	print "<tr><span class=\"head\" align=\"right\">Legend</span></tr>";
	print "<tr><td class=\"client\"><font color=\"red\">Red</font></td><td class=\"client\">Support hours not yet used</td></tr>
	<tr><td class=\"client\"><font color=\"green\">Green</font></td><td class=\"client\">Support hours exceed total by more than 20%</td></tr>
	<tr><td class=\"client\">Black</td><td class=\"client\">Support hours within the range of 20% under or over</td></tr>";
    print "</table>"; 
    print "</td>
    
 </tr>
 </table>";  

  }
     function techie_contract_report($year,$month){
     if(!acl::has_access($_SESSION['id'],"billing","read")) return 1;
      $sql = 'SELECT msi_clients.company, msi_contract.hours, msi_contract.client, msi_clients.id
              FROM msi_contract, msi_clients
              WHERE msi_clients.id = msi_contract.client';
      $results = sql::sql_run($sql);
      print "<span class=\"head\">Contract Client Hours</span>
	         <table class=\"client\">
	         <tr class=\"head\"><td>Company</td><td>Contract Hours</td><td>Hours Used</td> <td>Difference</td></tr>";
       foreach ($results as $result) {
       
       $used = billing::client_contract_hours($result[2],$year,$month);
	   $difference = $result[1] - $used;
		    
                	//added a line here so difference would be positive
			//the variable is not used in a different context so we can change the actual "polarity'
if ($_GET['month'] == date(n)) {$myday=date(d);}
else {$myday=30;}
			$deviance=($used*100/$total)-($myday*100/30);
			if($deviance > 20) { 
				$color='green';
			}else if($deviance < (-20)) {
				$color='red';
			}else{
				$color='black';
			}
      
			$difference = "<font color='".$color."'>$difference</font>";
                print "<tr>
                       <td class=\"client\">$result[0]</td>
                       <td class=\"client\">$result[1]</td>
                       <td class=\"client\">$used </td>
                       <td class=\"client\">$difference</td>
                       </tr>";
       }
       print "</table>";
  }

 function client_hours($client,$year,$month){
 if(!acl::has_access($_SESSION['id'],"billing","read")) return 1;
    $sql="SELECT sum(bill_hrs) 
          FROM msi_billing
          WHERE month(date) = $month and year(date)= $year
          AND client = \"$client\""; 	
      $result = sql::sql_run($sql);
      return $result[0][0];
    }

 function client_contract_hours($client,$year,$month){
 if(!acl::has_access($_SESSION['id'],"billing","read")) return 1;
    $sql="SELECT sum(bill_hrs) 
      FROM msi_billing
      WHERE month(date) = $month and year(date)= $year
      AND client = \"$client\" and upper(type) = 'CONTRACT' "; 	
    $result = sql::sql_run($sql);
    return $result[0][0];
    }
    
  function username($id){
    $results = sql::sql_run("select username from msi_users where id = \"$id\"");
    return $results[0][0];
  }

 function contract_client($id,$year,$month){
 if(!acl::has_access($_SESSION['id'],"contracts","read")) return 1;

    $sql = 'SELECT msi_clients.company, msi_billing.date, msi_billing.bill_hrs, msi_billing.nonbill_hrs, msi_billing.description, msi_billing.invoice
    ,msi_users.username, msi_billing.id, msi_billing.type, msi_billing.invoiced_by 
    FROM msi_users, msi_billing, msi_clients
    WHERE msi_clients.id = msi_billing.client AND msi_billing.techie = msi_users.id AND msi_billing.client = '.$id.'
    AND month(msi_billing.date) = '.$month.' and year(msi_billing.date)= '.$year.'
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
    print "<a href=?module=billing&action=contract_client&id=$id&year=$prev_year&month=$prev_month style=\"color: #1A53FF\">". date(F, mktime(0, 0, 0, $prev_month, 1, $prev_year)) ." $prev_year << Previous</a> ";
    print "&nbsp&nbsp";
    print " <b><a href=?module=billing&action=contract_client&id=$id&year=$year&month=$month>". date(F, mktime(0, 0, 0, $month, 1, $year)) ." $year</a></b>";
    print "&nbsp&nbsp";   
    print " <a href=?module=billing&action=contract_client&id=$id&year=$next_year&month=$next_month style=\"color: #1A53FF\">Next >> ". date(F, mktime(0, 0, 0, $next_month, 1, $next_year)) ." $next_year</a><p>";
    print '</td></tr></table>';
    print "<span class=\"head\">Client Contract Report for ".$results[0][0]."</span>
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
 
  function export_client($id, $month, $year){
  	
  	if(!acl::has_access($_SESSION['id'],"billing admin","read")) return 1;
  
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
       msi_billing.after_hour
  
      FROM msi_users, msi_billing,msi_clients
      WHERE msi_clients.id = msi_billing.client AND msi_billing.techie = msi_users.id AND msi_billing.client = '.$id.'
      AND month(msi_billing.date) = '.$month.' and year(msi_billing.date)= '.$year.'
	  And (msi_billing.site = \'On-Site\'
	  or msi_billing.after_hour = \'On-Site\')
      group by msi_billing.after_hour,msi_billing.date
      ORDER BY msi_billing.date  ASC';
   //echo $sql."<br />";
  
  	$results = sql::sql_run($sql);

  	header('Content-Disposition: attachment; filename="'.$results[0][0].'-'.$year.'-'.$month.'.xls"');
  
  	// header("Content-Type:application/.xls");
  	//// header("Content-Disposition: attachment;filename=$results[0][0].'-'.$year.'-'.$month.xls");
  	// header("Pragma: no-cache");
  	// header("Expires: 0");
  
    // if($results){
  		 
  	//	header('Content-Disposition: attachment; filename="'.$results[0][0].'-'.$year.'-'.$month.'.xls"');
  		//header('Content-Disposition:attachment; filename="anthea.xls"');
  		print"<table width = '60%' >
          <tr><td><font size = '2' color = 'blue'>Mindspring Computing<br>
          Unit 5, Doncaster Office Park<br>
          Punters Way, Kenilworth.<br>
          Tel: (021) 657 1780<br>
          Fax: (021) 671 7599
          </font></td><td></td><td></td><td><img src = 'http://intranet.mindspring.co.za/Mindspring.jpeg.jpg'  height ='69' width = '263' valign = 'top'/></td></tr></table><hr/>
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
  //	}
  //	else {
  //		die("<h2>No Records Found for SUPPORT REPORT ... </h2>") ;
  //	}
  	 
  	$sql2 = 'SELECT msi_clients.company,
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
       msi_billing.after_hour
  
      FROM msi_users, msi_billing,msi_clients
      WHERE msi_clients.id = msi_billing.client AND msi_billing.techie = msi_users.id AND msi_billing.client = '.$id.'
      AND month(msi_billing.date) = '.$month.' and year(msi_billing.date)= '.$year.'
	  And (msi_billing.site = \'Remote/Telephonic\'
	  or msi_billing.after_hour = \'Remote/Telephonic\')
      group by msi_billing.after_hour,msi_billing.date
      ORDER BY msi_billing.date  ASC';
  	//echo $sql2."<br />";
  
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
  
  	if($results2){
  		print "<h2><CENTER>REMOTE REPORT FOR &nbsp : &nbsp ".date("F Y", mktime(0, 0, 0, $month,1,$year))."</CENTER></h2>";
  		print "<h3><u><center>".$results2[0][0]."</center></u></h3>";
  		print "<div>";
  		print "<table border = '1' width= '60%' >
          <th style ='background-color:silver' >Date </th>
          <th style ='background-color:silver' >Billable Hours</th>";
  		print "  <th style ='background-color:silver' > Time </th>";
  		print "  <th style ='background-color:silver' >&nbsp;&nbsp;&nbsp;Engineer&nbsp;&nbsp;&nbsp; </th>
          <th style ='background-color:silver'>Description  </th>";
  
  		if(is_array($results2)){
  			foreach($results2 as $results2){
  				$total2 += $results2[2];
         print  "<tr>
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
  		print "</div>";
  	}
  	else {
  		die("<h2> No Records Found for REMOTE REPORT ... </h2>") ;
  	}
  }
  //end
      

function used_client_pholiday($year,$month){
	if(!acl::has_access($_SESSION['id'],"billing admin","read")) return 1;
	 
	$fulldate=$year.'-'.$month;
	$fulldate2=$year.'-'.$month.'-'.'00';
	 $sqlhld="SELECT
	msi_clients.company,
	sum( msi_billing.bill_hrs ) hours 
	FROM `msi_clients` , msi_billing
	WHERE msi_billing.client = msi_clients.id
	AND MONTH( msi_billing.date ) = $month AND year( msi_billing.date ) = $year
	AND ((msi_billing.site = 'Sunday, Public Holiday')
	OR (msi_billing.after_hour = 'Sunday, Public Holiday'))
    GROUP BY msi_billing.client
	ORDER BY `hours` DESC";

					$resultshld = sql::sql_run($sqlhld);
					print "<span class=\"head\">Sunday/Public Holiday</span>
	  				<table class=\"client\">
	  				<tr class=\"head\"><td>Client</td><td>Total Hours Used</td></tr>";
  	if(is_array($resultshld)){
  	foreach ($resultshld as $resulthld) {

  	print "<tr>
  	<td class=\"client\">$resulthld[0]</td>
  	<td class=\"client\">$resulthld[1]</td>
</tr> ";
  	}
  	}
print "</table>";
  	}

function used_client_afterhours($year,$month){
  		if(!acl::has_access($_SESSION['id'],"billing admin","read")) return 1;
  	
  		$fulldate=$year.'-'.$month;
  		$fulldate2=$year.'-'.$month.'-'.'00';

  	$sqlaft="SELECT
  	msi_clients.company,
  	sum( msi_billing.bill_hrs ) hours
  	 
  	FROM `msi_clients` , msi_billing
  	WHERE msi_billing.client = msi_clients.id
  	AND MONTH( msi_billing.date ) = $month AND year( msi_billing.date ) = $year
  	AND ((msi_billing.site = 'After Hours')
  	OR (msi_billing.after_hour = 'After Hours'))
  	GROUP BY msi_billing.client
  	ORDER BY `hours` DESC";
  	 
  	$resultsaft = sql::sql_run($sqlaft);
  	print "<span class=\"head\">After Hours</span>
	  				<table class=\"client\">
	  				<tr class=\"head\"><td>Client</td><td>Total Hours Used</td></tr>";
  	if(is_array($resultsaft)){
  		foreach ($resultsaft as $resultaft) {
  				
  			print "<tr>
  			<td class=\"client\">$resultaft[0]</td>
  			<td class=\"client\">$resultaft[1]</td>
		</tr></td></tr>";
  		}
  	}
  	
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
  			print "<a href=\"?module=billing&action=techie_billing&year=$prev_year&month=$prev_month&techie=$techie\" style=\"color: #1A53FF\">". date(F, mktime(0, 0, 0, $prev_month, 1, $prev_year)) ." $prev_year << Previous</a> ";
  			print "&nbsp&nbsp";
  			print " <b><a href=?module=billing&action=techie_billing&year=$year&month=$month&techie=$techie>". date(F, mktime(0, 0, 0, $month, 1, $year)) ." $year</a></b>";
  			print "&nbsp&nbsp";
  			print " <a href=\"?module=billing&action=techie_billing&year=$next_year&month=$next_month&techie=$techie\" style=\"color: #1A53FF\">Next >> ". date(F, mktime(0, 0, 0, $next_month, 1, $next_year)) ." $next_year</a><p>";
  			print '</td>
 		</tr>
</table>';
  			print "<span class=\"head\"> Billing Report for ".ucwords($techie)."</span>";
  	
  			print" <table class=\"client\">
  			<tr class=\"head\">
  	
  			<td class=\"client-link\"><a href=\"?module=billing&sort_column=date&year=$year&month=$month&action=techie_billing&techie=$techie\">Date</a></td>
  			<td class=\"client-link\"><a href=\"?module=billing&sort_column=company&year=$year&month=$month&action=techie_billing&techie=$techie\">Client</a></td>
  			<td>Billable hours</td>
  			<td>Non Billable hours</td>
  			
  			<td>Arrival Time</td>
  			<td>Departure Time</td>
  			
  			<td class=\"client-link\"><a href=\"?module=billing&sort_column=type&year=$year&month=$month&action=techie_billing&techie=$techie\">Type</a></td>
  			<td>Description</td>
  			</tr>";
  	
  			if(is_array($t_billing)){
  				foreach ($t_billing as $t_billing) {
  	
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
  			}
  	
  			print "</table>";
  	
  	}
  	
                                    
}

?>