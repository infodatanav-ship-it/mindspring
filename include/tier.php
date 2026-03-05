<?php

class tier {

	function listing(){

		if(acl::has_access($_SESSION['id'],"tier list","edit")){

			$results = sql::sql_run("select company, firstname, lastname, telephone, cellphone, email,tier,id from msi_clients  where tier = 1 order by tier ");

			print"<p> &nbsp;</p>";

			print ' <div class="links">
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="?module=tier&action=add" style="color:black">>>Add Tier List<<</a>
				</div>

				<table class=\"client\" width = 1125>
				<tr class=\"head\">';

			print "<b><p><font color = 'red' size = '4'>&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;
			&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;
			&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;<u>Tier List 1</u></font></p></b> "; 

			print " <tr class=\"head\">";


			print "<td >Company</a></td><td >First Name</td><td >Last Name</td><td >Telephone</td><td>Cellphone</td><td >Email</td><td ></td></tr>";


			if(is_array($results )){	

				foreach ($results as $result) {
	   	

					$company = $result[0];

					$name = $result[1];

					$surname = $result[2];

					$telephone = $result[3];

					$cell = $result[4];

					$email = $result[5];

					$tier = $result[6];
 

					if ($tier == '1') { 
						$color='red';
					} else if ($tier == '2') { 
						$color='Chocolate';
					} else if ($tier == '3') {
						$color = 'Firebrick4';
					} else {
						$color = 'black';
					}

					$company= "<font color='".$color."'>$company</font>";
					$name = "<font color='".$color."'>$name</font>";
					$surname ="<font color='".$color."'>$surname</font>";
					$telephone = "<font color='".$color."'>$telephone</font>";
					$cell = "<font color='".$color."'>$cell</font>";
					$email = "<font color='".$color."'>$email</font>";
					$tier = "<font color='".$color."'>$tier</font>";

					print "<tr>
						<td class=\"client\">$company</td>
						<td class=\"client\">$name</td>
						<td class=\"client\">$surname</td>
						<td class=\"client\">$telephone</td>
						<td class=\"client\">$cell</td>
						<td class=\"client\">$email</td>
						<td class=\"client\"><a href=index.php?module=tier&action=edit&client_id=$result[7]>Edit</td>
						</tr>";
			}
		}
		print "</table>";
		$results = sql::sql_run("select  company, firstname, lastname, telephone, cellphone, email,tier,id from msi_clients where tier = 2 order by tier ");
		print ' <table class=\"client\" width = 1125>';

		print"<p> &nbsp;</p>";
print "<b><p><font color = 'blue' size = '4'>&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;<u>Tier List 2</u></font></p></b>
<tr class=\"head\">";

print "<td >Company</a></td><td>First Name</td><td >Last Name</td><td>Telephone</td><td>Cellphone</td><td>Email</td><td></td></tr>";

		if(is_array($results )){	
	   	foreach ($results as $result) {
	   	
	   	$company = $result[0];
	   	$name = $result[1];
	   	$surname = $result[2];
	   	$telephone = $result[3];
	   	$cell = $result[4];
	   	$email = $result[5];
	   	$tier = $result[6];

       if ($tier == '1') { 
		$color='red';
		 }else if ($tier == '2') { 
		$color='blue';
		 }else if ($tier == '3') {
		$color = 'Firebrick4';
		}else {
		$color = 'black';}
 
    	$company= "<font color='".$color."'>$company</font>";
      	$name = "<font color='".$color."'>$name</font>";
	 	$surname ="<font color='".$color."'>$surname</font>";
	   	$telephone = "<font color='".$color."'>$telephone</font>";
	  	$cell = "<font color='".$color."'>$cell</font>";
	 	$email = "<font color='".$color."'>$email</font>";
	 	$tier = "<font color='".$color."'>$tier</font>";

                    print "<tr>

                             <td class=\"client\">$company</td>
                             <td class=\"client\">$name</td>
                             <td class=\"client\">$surname</td>
                             <td class=\"client\">$telephone</td>
                             <td class=\"client\">$cell</td>
                             <td class=\"client\">$email</td>
                              <td class=\"client\"><a href=index.php?module=tier&action=edit&client_id=$result[7]>Edit</td>
                            </tr>";
	          }
		}
		  print "</table>";

 $results = sql::sql_run("select  company, firstname, lastname, telephone, cellphone, email,tier,id from msi_clients where tier = 3 order by tier ");

   print ' <table class=\"client\" width = 1125>
       <tr class=\"head\">';
  
  print"<p> &nbsp;</p>";
  print "<b><p><font color = 'black' size = '4'>&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;
  &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;
  &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;<u>Tier List 3</u></font></p></b>
  <tr class=\"head\">";

print "<td>Company</a></td><td>First Name</td><td>Last Name</td><td>Telephone</td><td>Cellphone</td><td>Email</td><td ></td></tr>";

		if(is_array($results )){	
	   	foreach ($results as $result) {
	   	
	   	$company = $result[0];
	   	$name = $result[1];
	   	$surname = $result[2];
	   	$telephone = $result[3];
	   	$cell = $result[4];
	   	$email = $result[5];
	   	$tier = $result[6];

       if ($tier == '1') { 
		$color='red';
		 }else if ($tier == '2') { 
		$color='Chocolate';
		 }else if ($tier == '3') {
		$color = 'black';
		}else {
		$color = 'black';}
  
    	$company= "<font color='".$color."'>$company</font>";
      	$name = "<font color='".$color."'>$name</font>";
	 	$surname ="<font color='".$color."'>$surname</font>";
	   	$telephone = "<font color='".$color."'>$telephone</font>";
	  	$cell = "<font color='".$color."'>$cell</font>";
	 	$email = "<font color='".$color."'>$email</font>";
	 	$tier = "<font color='".$color."'>$tier</font>";
	 	
                    print "<tr>

                             <td class=\"client\">$company</td>
                             <td class=\"client\">$name</td>
                             <td class=\"client\">$surname</td>
                             <td class=\"client\">$telephone</td>
                             <td class=\"client\">$cell</td>
                             <td class=\"client\">$email</td>
                              <td class=\"client\"><a href=index.php?module=tier&action=edit&client_id=$result[7]>Edit</td>
               </tr>";
	          }
		}
		  print "</table>";
}
else {

  $results = sql::sql_run("select  company, firstname, lastname, telephone, cellphone, email,tier from msi_clients 
                       where tier = 1                      
                       order by tier ");
  
print ' <table class=\"client\" width = 1125>
<tr class=\"head\">';
    
  print"<p> &nbsp;</p>";
 print "<b><p><font color = 'red' size = '4'>&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;
 &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;
 &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;<u>Tier List 1</u></font></p></b> "; 

 print " <tr class=\"head\">";

print "<td >Company</a></td><td >First Name</td><td >Last Name</td><td >Telephone</td><td>Cellphone</td><td >Email</td><td ></td></tr>";

		if(is_array($results )){	
	   	foreach ($results as $result) {
	   	
	   	$company = $result[0];
	   	$name = $result[1];
	   	$surname = $result[2];
	   	$telephone = $result[3];
	   	$cell = $result[4];
	   	$email = $result[5];
	   	$tier = $result[6];

       if ($tier == '1') { 
		   $color='red';
       } else if ($tier == '2') { 
		$color='Chocolate';
       }else if ($tier == '3') {
         $color = 'Firebrick4';
			}else {
			$color = 'black';}
 
    	$company= "<font color='".$color."'>$company</font>";
      	$name = "<font color='".$color."'>$name</font>";
	 	$surname ="<font color='".$color."'>$surname</font>";
	   	$telephone = "<font color='".$color."'>$telephone</font>";
	  	$cell = "<font color='".$color."'>$cell</font>";
	 	$email = "<font color='".$color."'>$email</font>";
	 	$tier = "<font color='".$color."'>$tier</font>";

                    print "<tr>

                             <td class=\"client\">$company</td>
                             <td class=\"client\">$name</td>
                             <td class=\"client\">$surname</td>
                             <td class=\"client\">$telephone</td>
                             <td class=\"client\">$cell</td>
                             <td class=\"client\">$email</td>
            </tr>";
	          }
		}
		  print "</table>";

$results = sql::sql_run("select  company, firstname, lastname, telephone, cellphone, email,tier from msi_clients 
                       where tier = 2                      
                       order by tier ");
  
print ' <table class=\"client\" width = 1125>
<tr class=\"head\">';
  
 print"<p> &nbsp;</p>";
 print "<b><p><font color = 'chocolate' size = '4'>&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;
 &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;
 &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;<u>Tier List 2</u></font></p></b> "; 

 print " <tr class=\"head\">";

print "<td >Company</a></td><td >First Name</td><td >Last Name</td><td >Telephone</td><td>Cellphone</td><td >Email</td><td ></td></tr>";

		if(is_array($results )){	
	   	foreach ($results as $result) {
	   	
	   	$company = $result[0];
	   	$name = $result[1];
	   	$surname = $result[2];
	   	$telephone = $result[3];
	   	$cell = $result[4];
	   	$email = $result[5];
	   	$tier = $result[6];
	   							
       if ($tier == '1') { 
			$color='red';
		 }else if ($tier == '2') { 
		$color='Chocolate';
		 }else if ($tier == '3') {
         $color = 'Firebrick4';
		 }else {
		$color = 'black';}
 
    	$company= "<font color='".$color."'>$company</font>";
      	$name = "<font color='".$color."'>$name</font>";
	 	$surname ="<font color='".$color."'>$surname</font>";
	   	$telephone = "<font color='".$color."'>$telephone</font>";
	  	$cell = "<font color='".$color."'>$cell</font>";
	 	$email = "<font color='".$color."'>$email</font>";
	 	$tier = "<font color='".$color."'>$tier</font>";
	   		   	
                    print "<tr>
                                  
                             <td class=\"client\">$company</td>
                             <td class=\"client\">$name</td>
                             <td class=\"client\">$surname</td>
                             <td class=\"client\">$telephone</td>
                             <td class=\"client\">$cell</td>
                             <td class=\"client\">$email</td>

</tr>";
	          }
		}
		  print "</table>";
  
$results = sql::sql_run("select  company, firstname, lastname, telephone, cellphone, email,tier from msi_clients 
                       where tier = 3                      
                       order by tier ");
  
print ' <table class=\"client\" width = 1125>
<tr class=\"head\">';
     
print"<p> &nbsp;</p>";
 print "<b><p><font color = 'Firebrick4' size = '4'>&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;
 &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;
 &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;<u>Tier List 3</u></font></p></b> "; 

 print " <tr class=\"head\">";

print "<td >Company</a></td><td >First Name</td><td >Last Name</td><td >Telephone</td><td>Cellphone</td><td >Email</td><td ></td></tr>";

		if(is_array($results )){	
	   	foreach ($results as $result) {
	   	
	   	$company = $result[0];
	   	$name = $result[1];
	   	$surname = $result[2];
	   	$telephone = $result[3];
	   	$cell = $result[4];
	   	$email = $result[5];
	    $tier = $result[6];

       if ($tier == '1') { 
		 $color='red';
		 }else if ($tier == '2') { 
		 $color='Chocolate';
		 }else if ($tier == '3') {
          $color = 'Firebrick4';
          }else {$color = 'black';}

    	$company= "<font color='".$color."'>$company</font>";
      	$name = "<font color='".$color."'>$name</font>";
	 	$surname ="<font color='".$color."'>$surname</font>";
	   	$telephone = "<font color='".$color."'>$telephone</font>";
	  	$cell = "<font color='".$color."'>$cell</font>";
	 	$email = "<font color='".$color."'>$email</font>";
	 	$tier = "<font color='".$color."'>$tier</font>";
	   	
                    print "<tr>
                             <td class=\"client\">$company</td>
                             <td class=\"client\">$name</td>
                             <td class=\"client\">$surname</td>
                             <td class=\"client\">$telephone</td>
                             <td class=\"client\">$cell</td>
                             <td class=\"client\">$email</td>       

</tr>";
	          }
		}
		  print "</table>";
 		  
}
}
 function edit_form($client_id){
        $result = sql::sql_run("select company, firstname, lastname, telephone, fax, cellphone, email, saddress1, saddress2, szip, paddress1, paddress2, pzip, home , customer,tier,id
          from msi_clients where id = \"$client_id\" limit 1");
        
     print "<span class=\"head\">Editing Tier List </span>
            <table class=\"box\">

            
            <tr>
                <td align=\"right\">
                  <form action=\"index.php?module=tier&action=update&id=$client_id\" method=\"POST\">
                  <b>Company Name: </b>
                </td>
                <td>
                  <input type=\"input\" name=\"company\" size=\"40\" value=\"".$result[0][0]."\" readonly='readonly'>
                </td>
              </tr>

              <tr>
                <td align=\"right\">    
                  <b>Firstname: </b>
                </td>
                <td>
                  <input type=\"input\" name=\"firstname\" size=\"40\" value=\"".$result[0][1]."\" readonly='readonly'>
                </td>
              </tr>
              <tr>
                <td align=\"right\">
                  <b>Lastname: </b>
                </td>
                <td>
                  <input type=\"input\" name=\"lastname\" size=\"40\" value=\"".$result[0][2]."\" readonly='readonly'>
                </td>
              </tr>
              <tr>
                <td align=\"right\">
                  <b>Work Telephone: </b>
                </td>
                <td>
                  <input type=\"input\" name=\"telephone\" size=\"40\" value=\"".$result[0][3]."\" readonly='readonly'>
                </td>
              </tr>
              <tr>
                <td align=\"right\">
                  <b>Fax: </b>
                </td>
                <td>
                  <input type=\"input\" name=\"fax\" size=\"40\" value=\"".$result[0][4]."\" readonly='readonly'>
                </td>
              </tr>
					<tr>
                <td align=\"right\">
                  <b>Cell: </b>
                </td>
                <td>
                  <input type=\"input\" name=\"cellphone\" size=\"40\" value=\"".$result[0][5]."\"readonly='readonly'>
                </td>
              </tr>
              <tr>
                <td align=\"right\">
                  <b>Home Telephone: </b>
                </td>
                <td>
                  <input type=\"input\" name=\"home\" size=\"40\" value=\"".$result[0][13]."\" readonly='readonly'>
                </td>
              </tr>
              <tr>
                <td align=\"right\">
                  <b>Email: </b>
                </td>
                <td>
                  <input type=\"input\" name=\"email\" size=\"40\" value=\"".$result[0][6]."\" readonly='readonly'>
                </td>
              </tr>
              <tr>
                <td align=\"right\" valign=\"top\">
                  <b>Street Address: </b>
                </td>
                <td>
                  <input type=\"input\" name=\"saddress1\" size=\"40\" value=\"".$result[0][7]."\" readonly='readonly'><br>
                  <input type=\"input\" name=\"saddress2\" size=\"40\" value=\"".$result[0][8]."\"readonly='readonly'><br>
                  <input type=\"input\" name=\"szip\" size=\"5\" value=\"".$result[0][9]."\" readonly='readonly'>
       			 </td>
              </tr>
              <tr>
                <td align=\"right\" valign=\"top\">       			
                  <b>Postal Address: </b>
                </td>
                <td>
                  <input type=\"input\" name=\"paddress1\" size=\"40\" value=\"".$result[0][10]."\" readonly='readonly'><br>
                  <input type=\"input\" name=\"paddress2\" size=\"40\" value=\"".$result[0][11]."\" readonly='readonly'><br>
                  <input type=\"input\" name=\"pzip\" size=\"5\" value=\"".$result[0][12]."\" readonly='readonly'><br>
                </td>
              </tr>
              
             
               <td align=\"right\" valign=\"top\">       			
                  <b>Tier: </b>
                </td>
                <td>
                  <input type=\"input\" name=\"tier\" size=\"5\" value=\"".$result[0][15]."\" ><br>
                </td>
              </tr>
	      
              <tr>
                <td align=\"right\">       			
                  <input type=\"submit\" value=\"update\" name=\"submit\">
                    <input type=\"submit\" value=\"cancel\" name=\"action\">
                </td>
              </tr>
              </form>
       </table>"; 
  
  }

 function update($id, $company, $lastname, $firstname, $telephone, $fax, $cellphone, $home,  $email, $saddress1, $saddress2, $szip, $paddress1, $paddress2, $pzip, $customer,$tier){
  //  if($customer == 'on'){ $customer = "1"; } else {$customer = "0";};
 
       if(isset($_POST['submit'])){
        $tier = $_POST['tier'];
        $to = "sandiswa@mindspring.co.za";
         $subject = "Customer Priority Notice";
          $message = "The tier list priority have been updated.
          <html>
        <head>
       <title>Customer Priority Notice</title>
       </head>
       <body>
       <font face = 'sans-serif'>
       <p>&nbsp;</p>
       <p>&nbsp;</p>
       <p>&nbsp;</p>
       <p>&nbsp;</p>
       <table width = '100%' >
       <tr><td><font size = '2' face ='sans-serif' color = 'blue'>Mindspring Computing<br>
       Unit 5, Doncaster Office Park<br>
       Punters Way, Kenilworth.<br>
       Tel: (021) 657 1780<br>
       Fax: (021) 671 7599
       </font></td></tr></table>
       ";
      

          $headers  = 'MIME-Version: 1.0' . "\r\n";
          $headers .= 'Content-type: text/html; charset=iso-8859-1'."\r\n";
          $headers  .= "From: support@mindspring.co.za" . "\r\n" ;
  
       }
    $sql = "update msi_clients set company = \"$company\" , lastname = \"$lastname\", firstname = \"$firstname\",
           telephone = \"$telephone\", fax = \"$fax\", cellphone = \"$cellphone\", home = \"$home\", email = \"$email\",
           saddress1 = \"$saddress1\", saddress2 = \"$saddress2\", szip = \"$szip\", paddress1 = \"$paddress1\",
           paddress2 = \"$paddress2\", pzip = \"$pzip\",customer =\"$customer\", tier = \"$tier\" where id = \"$id\" "; 
    
    $result = sql::sql_run($sql);
     if(!isset($result)){
       echo "<p></p>";
      print "<b>Updated Tier for ". $company."</b><br />";
      echo "<p></p>";
     mail($to,$subject,$message,$headers);
       tier::listing();
     }
     else {
    
       print "<b>Something went wrong.....".mysql_error(). "</b><br>I tried to run $sql";
     }
     
  }
  
  function add_form(){
  
    $results = sql::sql_run("
      SELECT
        `msi_clients`.`company`,
        `msi_clients`.`id`
      FROM
        `msi_clients`
      WHERE
        `msi_clients`.`customer`='1'
      ORDER BY
        `msi_clients`.`company` ASC
    ");
    
    if(!isset($results)) {
    echo '<b>SQL Error! Please contact the site administrator.</b>';
      return FALSE;      
    }
  
     print "<span class=\"head\">Editing Tier List </span>";
        // foreach ($results as $result) {
                      // $client_id= $result["id"];}
                           
        if(isset($_POST['action'])){
          if(isset($_POST['company'])){
            $id= $_POST["company"];
         
       // print "select company, firstname, lastname, telephone, fax, cellphone, email, saddress1, saddress2, szip, paddress1, paddress2, pzip, home , customer,tier,id
       //   from msi_clients where id = \"$id\" limit 1";
          $result = sql::sql_run("select company, firstname, lastname, telephone, fax, cellphone, email, saddress1, saddress2, szip, paddress1, paddress2, pzip, home , customer,tier,id
          from msi_clients where id = \"$id\" limit 1");

              print "<table class=\"box\">
                           <tbody>
                          <tr>
                <td align=\"right\">
                  <form action=\"index.php?module=tier&action=update&id=$id\" method=\"POST\">
                  <b>Company Name: </b>
                </td>
                <td>
                  <input type=\"input\" name=\"company\" size=\"40\" value=\"".$result[0][0]."\" readonly='readonly'>
                </td>
              </tr>
              <tr>
                <td align=\"right\">    
                  <b>Firstname: </b>
                </td>
                <td>
                  <input type=\"input\" name=\"firstname\" size=\"40\" value=\"".$result[0][1]."\" readonly='readonly'>
                </td>
              </tr>
              <tr>
                <td align=\"right\">
                  <b>Lastname: </b>
                </td>
                <td>
                  <input type=\"input\" name=\"lastname\" size=\"40\" value=\"".$result[0][2]."\" readonly='readonly'>
                </td>
              </tr>
              <tr>
                <td align=\"right\">
                  <b>Work Telephone: </b>
                </td>
                <td>
                  <input type=\"input\" name=\"telephone\" size=\"40\" value=\"".$result[0][3]."\" readonly='readonly'>
                </td>
              </tr>
              <tr>
                <td align=\"right\">
                  <b>Fax: </b>
                </td>
                <td>
                  <input type=\"input\" name=\"fax\" size=\"40\" value=\"".$result[0][4]."\" readonly='readonly'>
                </td>
              </tr>
					<tr>
                <td align=\"right\">
                  <b>Cell: </b>
                </td>
                <td>
                  <input type=\"input\" name=\"cellphone\" size=\"40\" value=\"".$result[0][5]."\"readonly='readonly'>
                </td>
              </tr>
              <tr>
                <td align=\"right\">
                  <b>Home Telephone: </b>
                </td>
                <td>
                  <input type=\"input\" name=\"home\" size=\"40\" value=\"".$result[0][13]."\" readonly='readonly'>
                </td>
              </tr>
              <tr>
                <td align=\"right\">
                  <b>Email: </b>
                </td>
                <td>
                  <input type=\"input\" name=\"email\" size=\"40\" value=\"".$result[0][6]."\" readonly='readonly'>
                </td>
              </tr>
              <tr>
                <td align=\"right\" valign=\"top\">
                  <b>Street Address: </b>
                </td>
                <td>
                  <input type=\"input\" name=\"saddress1\" size=\"40\" value=\"".$result[0][7]."\" readonly='readonly'><br>
                  <input type=\"input\" name=\"saddress2\" size=\"40\" value=\"".$result[0][8]."\"readonly='readonly'><br>
                  <input type=\"input\" name=\"szip\" size=\"5\" value=\"".$result[0][9]."\" readonly='readonly'>
       			 </td>
              </tr>
              <tr>
                <td align=\"right\" valign=\"top\">       			
                  <b>Postal Address: </b>
                </td>
                <td>
                  <input type=\"input\" name=\"paddress1\" size=\"40\" value=\"".$result[0][10]."\" readonly='readonly'><br>
                  <input type=\"input\" name=\"paddress2\" size=\"40\" value=\"".$result[0][11]."\" readonly='readonly'><br>
                  <input type=\"input\" name=\"pzip\" size=\"5\" value=\"".$result[0][12]."\" readonly='readonly'><br>
                </td>
              </tr>

               <td align=\"right\" valign=\"top\">       			
                  <b>Tier: </b>
                </td>
                <td>
                  <input type=\"input\" name=\"tier\" size=\"5\" value=\"".$result[0][15]."\" ><br>
                </td>
              </tr
              <tr>
                <td align=\"right\">       			
                  <input type=\"submit\" value=\"update\" name=\"submit\">
                   <input type=\"submit\" value=\"cancel\" name=\"action\">
                </td>
              </tr>
         
        </tbody>
      </table>
    </form>";
  
  }
        }else {
  ?> <form name="addtier" action="index.php?module=tier&action=add&id=<?php print $result['company'];?>" method="POST">
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
     		      <label>Company:&nbsp</label>
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
		  <?php 

 }
 }
}
?>