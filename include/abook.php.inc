<?php
// $Id: abook.php.inc,v 1.7 2009/05/25 07:55:52 etienne Exp $

class abook {

 function navigation(){
	          print '<div class="sec-links"><ul>
		                      <li> <a href="?module=abook&action=show_all">Show All</a></li>
		                      <li> <a href="?module=client&action=add">Add User</a></li></ul>
                              <li> <a href="?module=abook&action=customer">Show Customers</a></li></ul>
                              <li> <a href="?module=abook&action=non_customer">Show Non Customers</a></li></ul>
	    
	               </div>';
  }  
 		  
  function form(){
   
/*If you are sure there is no client in the address book already <a href=\"?module=client&action=add\">add a client?</a> */
/*print "<span class=\"head\"> Address Book </span>"; */
	  abook::navigation();
	       print "<div class=\"box\">
	       If your looking for a customers contact details you are in the right place,<br>
	       if you need to know about clients accounts and servers try the help desk.<br>
	       Type any of the clients details in the box below, Ill search there names. addresses and numbers.<br><br>
	       	       
	       <form action=\"?module=abook\" method=\"POST\">
		 <input name=\"search_string\">
	         <input type=\"submit\" value=\"submit\">
	       </form>
	      
	       </div>";  
  }
  
  //modified search function can take one or two of the parameters, filling from the left the other params being null.
  function search($search_string=NULL, $sort_column=NULL,$action=NULL){
 

  if((!isset($search_string)) && (!isset($sort_column)) && ($action=="show_all"))
{
  
  //show all
      	$results = sql::sql_run("select id, company, firstname, lastname, telephone, cellphone, email from msi_clients");
}
else if((!isset($search_string)) && (!isset($sort_column)) && ($action=="customer"))
{
//for mindspring customers
$results = sql::sql_run("select id, company, firstname, lastname, telephone, cellphone, email from msi_clients where customer='1' ORDER BY company ASC");

}
else if((!isset($search_string)) && (!isset($sort_column)) && ($action=="non_customer"))
{
//for nonmindspring customers
$results = sql::sql_run("select id, company, firstname, lastname, telephone, cellphone, email from msi_clients where customer is null or customer='0'");

}

else if((isset($search_string)) && (!isset($sort_column)) && !empty($search_string)) {
 
//from search text box 
  	 $results = sql::sql_run("select id, company, firstname, lastname, telephone, cellphone, email
  																					 from msi_clients where company rlike \"$search_string\" or
  																					 firstname rlike \"$search_string\" or
  																					 lastname rlike \"$search_string\" or
  																					 saddress1 rlike \"$search_string\" or
  																					 saddress2 rlike \"$search_string\" or
  				 
	                                                                                 paddress1 rlike \"$search_string\" or
  																					 paddress2 rlike \"$search_string\" or
  																					 cellphone rlike \"$search_string\" or
  																				         email rlike \"$search_string\" or
  																					 telephone rlike \"$search_string\" order by company"); 
        

}else if((isset($search_string)) && (isset($sort_column)) && ($search_string != ''))
{               

      //sort by column with search string set

      $results = sql::sql_run("select id, company, firstname, lastname, telephone, cellphone, email
  																					 from msi_clients where company rlike \"$search_string\" or
  																					 firstname rlike \"$search_string\" or
  																					 lastname rlike \"$search_string\" or
  																					 saddress1 rlike \"$search_string\" or
  																					 saddress2 rlike \"$search_string\" or
  																				 	 paddress1 rlike \"$search_string\" or
  																					 paddress2 rlike \"$search_string\" or
  																					 cellphone rlike \"$search_string\" or
  																					 email rlike \"$search_string\" or
  																					 telephone rlike \"$search_string\" order by $sort_column"); 
         

} else if((isset($sort_column)) && ($search_string==''))
{

$results = sql::sql_run("select id, company, firstname, lastname, telephone, cellphone, email from msi_clients order by $sort_column");

}

else{
  

    $results = sql::sql_run("select id, company, firstname, lastname, telephone, cellphone, email from msi_clients"); }
  

//this applies to all above options
$result_count = count($results);

	/*

			 // Pass the required arguments to the client constructor
		 $ms_client = new client(
			$results[0][0], // id
			$results[0][1], // company
			$results[0][2], // firstname
			$results[0][3], // lastname
			$results[0][4], // telephone
			$results[0][5], // cellphone
			$results[0][6], // email
			// Add additional arguments as needed, or set to null if not available
			null, null, null, null, null, paddress1: null, paddress2: null, pzip: null, customer: null
		 );
		 $ms_client->info($results[0][0]);

	*/

	
	if($result_count == 1) {
		 client::info($results[0][0]);
	  } else if($result_count == 0){
	   	print "No results, maybe you want to <a href=\"?module=client&action=add\">add a client?</a><br>";
		  abook::form();
	  } else {
/* added links and vars for "search_string" and "sort_column" index.php uses these two variables, when set as the params for the abook::search function, which in turn determins the sql statement executed and the order of the resultset */
	  		print "<span class=\"head\">Search Results</span>


<table class=\"client\">
<tr class=\"head\">
<td class=\"client-link\">#</td>
<td class=\"client-link\"><a href=\"?module=abook&sort_column=company&search_string=$search_string\">Company</a></td><td class=\"client-link\"><a href=\"?module=abook&sort_column=firstname&search_string=$search_string\">First Name</a></td><td class=\"client-link\"><a href=\"?module=abook&sort_column=lastname&search_string=$search_string\">Last Name</a></td><td class=\"client-link\"><a href=\"?module=abook&sort_column=telephone&search_string=$search_string\">Telephone</a></td><td class=\"client-link\"><a href=\"?module=abook&sort_column=cellphone&search_string=$search_string\">Cellphone</a></td><td class=\"client-link\"><a href=\"?module=abook&sort_column=email&search_string=$search_string\">Email</a></td><td class=\"client-link\"></td></tr>";

 $n=0;
			
	   	foreach ($results as $result) {
            ++$n;
                    print "<tr>
                              <td class=\"client\">$n</td>
                             <td class=\"client\"><a href=index.php?module=client&action=view&client_id=$result[0]>$result[1]</a>
                             <td class=\"client\">$result[2]</td>
                             <td class=\"client\">$result[3]</td>
                             <td class=\"client\">$result[4]</td>
                             <td class=\"client\">$result[5]</td>
                             <td class=\"client\">$result[6]</td>
                             <td class=\"client\"><a href=index.php?module=client&action=view&client_id=$result[0]>View</a></td>
                             

</tr>";
	          }
		  print "</table><hr>";
	  }
  
  }
}

?>
