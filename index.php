<?php session_start(); // Get the cookies set as soon as possible
// $Id: index.php,v 1.23 2009/05/25 07:55:52 etienne Exp $
ini_set('error_reporting', E_ALL & ~E_NOTICE & ~E_STRICT & ~E_DEPRECATED);
error_reporting(1); 
?>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
<head>
<meta name="generator" content="Bluefish" />
<meta name="author" content="Laurence Baldwin" />
<meta name="copyright" content="Mindspring Computing 2004" />
<meta name="ROBOTS" content="NOINDEX, NOFOLLOW" />
<link rel="stylesheet" type="text/css"  href="style.css" />

<title>Mindspring Intranet (<?php echo date('Y-m-d H:i:s'); ?>)</title>
</head>
<body>
<h1>Mindspring Intranet</h1>


<div class="main">
<?php
include("include/sql.php.inc");
//include("include/incident.php.inc");
# Commented out by Sandiswa - 2019 Apr 25 
//include("include/helpdesk.php.inc");
# Commented out by Sandiswa - 2019 Apr 25 
include("include/client.php.inc");
include("include/doc.php.inc");
//include("include/help.php.inc");
# Commented out by Sandiswa - 2019 Apr 25 
include("include/abook.php.inc");
//include("include/adslpass.php");
# Commented out by Sandiswa - 2019 Apr 25 
// Ill get back to these one day laurence
#include("include/domain.php.inc");
#include("include/website.php.inc");
# Commented out by Sandiswa - 2019 Apr 18 
# include("include/adsl.php.inc");
//include("include/password.php.inc");
include("include/main.php.inc");
include("include/acl.php.inc");
//include("include/dialup.php.inc");
# Commented out by Sandiswa - 2019 Apr 25 
include("include/billing.php.inc");
include("include/contract.php.inc");
include("include/admin.php.inc");
include("include/tier.php");
//include("include/adhoc_customers.php");
# Commented out by Sandiswa - 2019 Apr 25 
//include("include/new3.php");
# Commented out by Sandiswa - 2019 Apr 25 
include("include/reporting.php.inc");

$day = date('d');
$year = date('Y');
$month = date('n');
$hour = date('H');
$week = date('W'); // so that php and mysql both think the same way

if(isset($_REQUEST['type'])){ $type = $_REQUEST['type']; }
if(isset($_REQUEST['module'])){ $module = $_REQUEST['module']; }
if(isset($_REQUEST['client_id'])){ $client_id = $_REQUEST['client_id']; }
if(isset($_REQUEST['id'])){ $id = $_REQUEST['id']; }
if(isset($_POST['id'])){ $id = $_POST['id']; }
if(isset($_REQUEST['action'])){ $action = $_REQUEST['action']; }
if(isset($_REQUEST['confirm'])){ $confirm = $_REQUEST['confirm']; }
if(isset($_POST['search_string'])){ $search_string = $_POST['search_string']; }
if(isset($_REQUEST['search_string'])){ $search_string = $_REQUEST['search_string']; }
if(isset($_POST['incident'])){ $incident = $_POST['incident']; }
if(isset($_POST['sqltime'])){ $sqltime = $_POST['sqltime']; }
if(isset($_POST['submit'])){ $submit = $_POST['submit']; }
if(isset($_REQUEST['version'])){ $version = $_REQUEST['version']; }
if(isset($_REQUEST['server'])){ $server = $_REQUEST['server']; }
if(isset($_REQUEST['company'])){ $company = $_REQUEST['company']; }
if(isset($_REQUEST['lastname'])){ $lastname = $_REQUEST['lastname']; }
if(isset($_REQUEST['firstname'])){ $firstname = $_REQUEST['firstname']; }
if(isset($_REQUEST['telephone'])){ $telephone = $_REQUEST['telephone']; }
if(isset($_REQUEST['fax'])){ $fax = $_REQUEST['fax']; }
if(isset($_REQUEST['cellphone'])){ $cellphone = $_REQUEST['cellphone']; }
if(isset($_REQUEST['email'])){ $email = $_REQUEST['email']; }
if(isset($_REQUEST['saddress1'])){ $saddress1 = $_REQUEST['saddress1']; }
if(isset($_REQUEST['saddress2'])){ $saddress2 = $_REQUEST['saddress2']; }
if(isset($_REQUEST['szip'])){ $szip = $_REQUEST['szip']; }
if(isset($_REQUEST['paddress1'])){ $paddress1 = $_REQUEST['paddress1']; }
if(isset($_REQUEST['paddress2'])){ $paddress2 = $_REQUEST['paddress2']; }
if(isset($_REQUEST['pzip'])){ $pzip = $_REQUEST['pzip']; }
if(isset($_REQUEST['home'])){ $home = $_REQUEST['home']; }
if(isset($_REQUEST['cap_size'])){ $cap_size = $_REQUEST['cap_size']; }
if(isset($_REQUEST['password'])){ $password = $_REQUEST['password']; }
if(isset($_REQUEST['userid'])){ $userid = $_REQUEST['userid']; }
if(isset($_POST['userid'])){ $userid = $_POST['userid']; }
if(isset($_REQUEST['location'])){ $location = $_REQUEST['location']; }
if(isset($_REQUEST['customer'])){ $customer = $_REQUEST['customer']; }
if(isset($_REQUEST['line_speed'])){ $line_speed = $_REQUEST['line_speed']; }
if(isset($_REQUEST['number'])){ $number = $_REQUEST['number']; }
if(isset($_REQUEST['realm'])){ $realm = $_REQUEST['realm']; }
if(isset($_REQUEST['session_time'])){ $session_time = $_REQUEST['session_time']; }
if(isset($_REQUEST['throttle_size'])){ $throttle_size = $_REQUEST['throttle_size']; }
if(isset($_REQUEST['startcdate'])){ $startcdate = $_REQUEST['startcdate']; }
if(isset($_REQUEST['endcdate'])){ $endcdate = $_REQUEST['endcdate']; }
if(isset($_REQUEST['identity'])){ $identity = $_REQUEST['identity']; }
if(isset($_REQUEST['terminate'])){ $terminate = $_REQUEST['terminate']; }
if(isset($_REQUEST['color'])){ $color = $_REQUEST['color']; }
if(isset($_REQUEST['activetechie'])){ $activetechie = $_REQUEST['activetechie']; }
//for builiding the menu for editing and listing contracts
if(isset($_REQUEST['year2'])){ $year2 = $_REQUEST['year2']; }
if(isset($_REQUEST['month2'])){ $month2 = $_REQUEST['month2']; }

if(isset($_REQUEST['year3'])){ $year3 = $_REQUEST['year3']; }
if(isset($_REQUEST['month3'])){ $month3 = $_REQUEST['month3']; }

if(isset($_REQUEST['termyear'])){ $termyear = $_REQUEST['termyear']; }
if(isset($_REQUEST['termmonth'])){ $termmonth = $_REQUEST['termmonth']; }
if(isset($_REQUEST['term'])){ $term = $_REQUEST['term']; }

//for uploading files
if(isset($_REQUEST['uptitle'])){ $uptitle = $_REQUEST['uptitle']; }
if(isset($_REQUEST['updescription'])){ $updescription = $_REQUEST['updescription']; }
//if(isset($_REQUEST['upfile'])){ $upfile = $_REQUEST['upfile']; }
if(isset($_FILES['upfile']['name'])){$upfile= $_FILES['upfile']['name'];}
 if(isset($_FILES['upfile']['tmp_name'])){$tmpname=$_FILES['upfile']['tmp_name'];}
if(isset($_FILES['upfile']['size'])){$filesize=$_FILES['upfile']['size'];}
if(isset($_FILES['upfile']['type'])){$filetype=$_FILES['upfile']['type'];}
if(isset($_FILES['upfile']['error'])){$errors=$_FILES['upfile']['error'];}
if(isset($_REQUEST['docpath'])){ $docpath = $_REQUEST['docpath']; }
if(isset($_REQUEST['category'])){ $category = $_REQUEST['category']; }

/*EPP: NEW Structure for faulty logic*/
/*ADSL Module: START*/

/*if(isset($_POST['ADSL_Username'])){ $ADSL_Username = $_POST['ADSL_Username']; }
if(isset($_POST['ADSL_Realm'])){ $ADSL_Realm = $_POST['ADSL_Realm']; }
if(isset($_POST['ADSL_Password'])){ $ADSL_Password = $_POST['ADSL_Password']; }
if(isset($_POST['ADSL_Contract'])){ $ADSL_Contract = $_POST['ADSL_Contract']; }
if(isset($_POST['ADSL_Adhoc'])){ $ADSL_Adhoc = $_POST['ADSL_Adhoc']; }
if(isset($_POST['ADSL_Session'])){ $ADSL_Session = $_POST['ADSL_Session']; }
if(isset($_POST['ADSL_Throttle'])){ $ADSL_Throttle = $_POST['ADSL_Throttle']; }
if(isset($_POST['ADSL_Name'])){ $ADSL_Name = $_POST['ADSL_Name']; }
if(isset($_POST['ADSL_Tel'])){ $ADSL_Tel = $_POST['ADSL_Tel']; }
if(isset($_POST['ADSL_Cell'])){ $ADSL_Cell = $_POST['ADSL_Cell']; }
if(isset($_POST['ADSL_Fax'])){ $ADSL_Fax = $_POST['ADSL_Fax']; }
if(isset($_POST['ADSL_Email'])){ $ADSL_Email = $_POST['ADSL_Email']; }
if(isset($_POST['ADSL_Provider'])) {$ADSL_Provider = $_POST['ADSL_Provider']; }
if(isset($_POST['ADSL_AcctType'])) {$ADSL_AcctType = $_POST['ADSL_AcctType']; }*/ # Commented out by Sandiswa - 2019 Apr 25 
/*ADSL Module: END*/
if(isset($_REQUEST['year'])){ $year = $_REQUEST['year']; }
if(isset($_REQUEST['month'])){ $month = $_REQUEST['month']; }
if(isset($_REQUEST['week'])){ $week = $_REQUEST['week']; }
if(isset($_REQUEST['day'])){ $day = $_REQUEST['day']; }
if(isset($_REQUEST['adsl_username'])){ $adsl_username = $_REQUEST['adsl_username']; }
if(isset($_POST['os'])){ $os = $_POST['os']; }
if(isset($_REQUEST['os'])){ $os = $_REQUEST['os']; }
if(isset($_POST['client'])){ $client = $_POST['client']; }
if(isset($_REQUEST['client'])){ $client = $_REQUEST['client']; }
if(isset($_POST['login'])){ $login = $_POST['login']; }
if(isset($_REQUEST['login'])){ $login = $_REQUEST['login']; }
if(isset($_POST['username'])){ $username = $_POST['username']; }
if(isset($_REQUEST['username'])){ $username = $_REQUEST['username'];}
if(isset($_POST['saix_type'])){ $saix_type = $_POST['saix_type']; }
if(isset($_REQUEST['saix_type'])){ $saix_type = $_REQUEST['saix_type'];}
if(isset($_REQUEST['notes'])){ $notes = $_REQUEST['notes'];}
if(isset($_REQUEST['active'])){ $active = $_REQUEST['active'];}
if(isset($_REQUEST['framed_ip_address'])){ $framed_ip_address = $_REQUEST['framed_ip_address'];}
if(isset($_REQUEST['date'])){ $date = $_REQUEST['date'];}

if(isset($_REQUEST['rate'])){ $rate = $_REQUEST['rate'];}
if(isset($_REQUEST['hrate'])){ $hrate = $_REQUEST['hrate'];}
if(isset($_REQUEST['type'])){ $type = $_REQUEST['type'];}
if(isset($_REQUEST['quote'])){ $quote = $_REQUEST['quote'];}
if(isset($_REQUEST['site'])){ $site = $_REQUEST['site'];}
if(isset($_REQUEST['travel'])){ $travel = $_REQUEST['travel'];}
if(isset($_REQUEST['bill_hrs'])){ $bill_hrs = $_REQUEST['bill_hrs'];}
if(isset($_REQUEST['nonbill_hrs'])){ $nonbill_hrs = $_REQUEST['nonbill_hrs'];}
if(isset($_REQUEST['description'])){ $description = $_REQUEST['description'];}
if(isset($_REQUEST['arrive_time'])){ $arrive_time = $_REQUEST['arrive_time'];}
if(isset($_REQUEST['depart_time'])){ $depart_time = $_REQUEST['depart_time'];}
if(isset($_REQUEST['delete'])){ $delete = $_REQUEST['delete'];}
if(isset($_REQUEST['Delete'])){ $delete = $_REQUEST['Delete'];}
if(isset($_REQUEST['hours'])){ $hours = $_REQUEST['hours'];}
if(isset($_REQUEST['module_id'])){ $module_id = $_REQUEST['module_id'];}
if(isset($_POST['add'])){ $add = $_POST['add'];}
if(isset($_POST['edit'])){ $edit = $_POST['edit'];}
if(isset($_POST['billing_post'])){ $billing_post = $_POST['billing_post'];}
if(isset($_POST['read'])){ $read = $_POST['read'];}
/* added "sort_column" for use with the abook::search function */
if(isset($_REQUEST['sort_column'])){$sort_column = $_REQUEST['sort_column'];}
/*add new fields for contract class */
if(isset($_REQUEST['contract_year'])){ $contract_year = $_REQUEST['contract_year'];}
if(isset($_REQUEST['contract_month'])){ $contract_month = $_REQUEST['contract_month'];}
if(isset($_REQUEST['primary'])){ $primary = $_REQUEST['primary'];}
if(isset($_REQUEST['backup'])){ $backup = $_REQUEST['backup'];}
if(isset($_REQUEST['server_hours'])){ $server_hours = $_REQUEST['server_hours'];}
if(isset($_REQUEST['server_techie'])){ $server_techie = $_REQUEST['server_techie'];}
if(isset($_REQUEST['acc_manager'])){ $acc_manager = $_REQUEST['acc_manager'];}
if(isset($_REQUEST['site'])) {$site = $_REQUEST['site']; }
if(isset($_REQUEST['afterhour'])) {$afterhour=$_REQUEST['afterhour']; }
if(isset($_REQUEST['effective'])) {$afterhour=$_REQUEST['effective']; }
if(isset($_REQUEST['techie'])) {$techie=$_REQUEST['techie']; }
if(isset($_REQUEST['m_type'])) {$m_type=$_REQUEST['m_type']; }
if(isset($_REQUEST['cid'])) {$cid=$_REQUEST['cid']; }
if(isset($_REQUEST['term'])) {$term=$_REQUEST['term']; }
if(isset($_REQUEST['billready'])) {$billready=$_REQUEST['billready']; }
//if(isset($_REQUEST['mailhours'])) {$mailhours=$_REQUEST['mailhours']; }



// If user has filled in the login form, check the password then log them in
if ($action == "login") {
acl::login($username,$password);
}

// Check that the user is logged in, if not send them to the login page
if( !acl::logged_in() || $action == "logout" ) {
session_destroy(); // just to be sure
$module = "login";
}

?>

	<!--Scripts-->
	<!-- <script src="https://code.jquery.com/jquery-3.3.1.min.js"></script> -->
	<script type="text/javascript" src="https://cdn.jsdelivr.net/jquery/latest/jquery.min.js"></script>

	<script src="https://cdnjs.cloudflare.com/ajax/libs/popper.js/1.14.7/umd/popper.min.js" integrity="sha384-UO2eT0CpHqdSJQ6hJty5KVphtPhzWj9WO1clHTMGa3JDZwrnQq4sF86dIHNDz0W1" crossorigin="anonymous"></script>
	<script src="https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/js/bootstrap.min.js" integrity="sha384-JjSmVgyd0p3pXB1rRibZUAYoIIy6OrQ6VrjIEaFf/nJGzIxFDsf4x0xIM+B07jRM" crossorigin="anonymous"></script>


	<script type="text/javascript" src="https://cdn.jsdelivr.net/momentjs/latest/moment.min.js"></script>
	<script type="text/javascript" src="https://cdn.jsdelivr.net/npm/daterangepicker/daterangepicker.min.js"></script>
	<link rel="stylesheet" type="text/css" href="https://cdn.jsdelivr.net/npm/daterangepicker/daterangepicker.css" />

	<script src="./modal.js"></script>
	
	<script>
		$(function(){
			$('a').each(function() {
				if ($(this).prop('href') == window.location.href) {
					$(this).addClass('current');
				}
			});
		});
	</script>

  <script>
    $(document).ready(function() {
        $('#downloadBtn').click(function() {
            // Get table data
            let csvData = [];
            
            // Get headers
            let headers = [];
            $('#data-table thead th').each(function() {
                headers.push($(this).text().trim());
            });
            csvData.push(headers);
            
            // Get data rows
            $('#data-table tbody tr').each(function() {
                let row = [];
                $(this).find('td').each(function() {
                    row.push($(this).text().trim());
                });
                csvData.push(row);
            });
            
            // Convert to CSV format
            let csvContent = csvData.map(row => 
                row.map(cell => {
                    // Handle cells that contain commas, quotes, or newlines
                    if (typeof cell === 'string' && (cell.includes(',') || cell.includes('"') || cell.includes('\n'))) {
                        return '"' + cell.replace(/"/g, '""') + '"';
                    }
                    return cell;
                }).join(',')
            ).join('\n');
            
            // Add BOM for UTF-8 to handle special characters properly
            const blob = new Blob(['\uFEFF' + csvContent], { type: 'text/csv;charset=utf-8;' });
            
            // Create download link
            const link = document.createElement('a');
            const url = URL.createObjectURL(blob);
            
            // Set download attributes
            link.href = url;
            link.setAttribute('download', 'table-data.csv');
            
            // Append to body, click, and remove
            document.body.appendChild(link);
            link.click();
            document.body.removeChild(link);
            
            // Clean up the URL object
            URL.revokeObjectURL(url);
            
            // Optional: Show success message
            alert('Table data has been downloaded as CSV!');
        });
    });
  </script>
	
	<?php 
//<li><a href=\"index.php?module=password\">Passwords</a></li> Disabled password module sandiswa(5/17/2020)
//moved this it was showing the headers all the time, still cannot figure it out...
if(acl::logged_in()){

print "
<div class=\"pri-links\">
<li><a href=\"?\">Home</a></li>


<li><a href=\"index.php?module=abook\">Address</a></li>
<li><a href=\"?module=tier\">Tier List</a></li>
<li><a href=\"index.php?module=admin\">Admin</a></li><br>
<li><a href=\"index.php?module=doc\">Doc</a></li>
				
		
<li><a href=\"?module=contract\">Billing Management</a></li>
<li><a href=\"?module=billing\">Techies Billing</a></li>
<li><a href=\"?module=sms\">SMS</a></li>
<li><a href=\"?module=billing_reports&action=breakdown\">Reports</a></li>";
//print "<li><a href=\"?module=mailhours\">Mailing</a></li>		
	//	";
//<li><a href=\"?module=adhoc_customers\">Adhoc</a></li>
//<li><a href=\"index.php?module=dialup\">Dialups</a></li>
//<li><a href=\"index.php?module=adsl\">ADSL</a></li>
//commented menu items are modules no longer used
print "<li class = \"pri\" align = \"left\"><a  href=\"index.php?action=logout\">Logout</a></li>
</div>";

}



/*this is for the contract module, it has to be here so that it can dynamically update
check if this months hours have been updated/created, if not create them with last months hours and this months date
this check happens every time the view is displayed, but it's not expensive
*/
//commented out by  tich 2008 to cater for date ranges in contracts
/*

$sql_code = 'select * from msi_contract where month = substr(now() from 6 for 2) and year = substr(now() from 1 for 4)';
$ret_code = sql_run($sql_code);
if(!$ret_code){
 */
//add a new set of client hours for this month based on last months entries

//first we have to check for the possibility that this is the 1st month
/*
$sql_check_code = 'select substr(now() from 6 for 2)';
$ret_check_ret = sql_run($sql_check_code);
*/
/*
if($ret_check_ret[0][0] == '01')
{
$sql_code2 = 'insert into msi_contract select client, hours, substr(now() from 6 for 2), substr(now() from 1 for 4) from msi_contract where month = \'12\' and year = (cast(substr(now() from 1 for 4) as decimal)-1)';
*/
   /*check here for 1st month and adjust year one back or else the function will try to subtract one from the current month and get 0 and the wrong year */

/*
sql_run($sql_code2);
}else{
$sql_code2 = 'insert into msi_contract select client, hours, substr(now() from 6 for 2), substr(now() from 1 for 4) from msi_contract where month = (cast(substr(now() from 6 for 2) as decimal)-1) and year = substr(now() from 1 for 4)';
sql_run($sql_code2);
     }
	       }
  */

//end of check

//adsl::send_mail($month, "pramodhomelite@msp.co.za",$year,$day);
switch ($module) {

	case "login":
		acl::login_form();
	break;

	case "sms":
		require_once('include/sms.php');
	break;

	case "admin":
		admin::navigation();
	if($action == "add_user_form"){
		admin::add_user_form();
	} else if ($action == "add_user"){
		admin::add_user($username,$password, $rate,$hrate,$email);
	} else if ($action == "add_perm"){
		admin::add_perm($id,$module_id,$read,$add,$edit);
	} else if($action == "del_perm"){
		admin::del_perm($id);
	} else if($action == "edit_user_form"){
		admin::edit_user_form($id);
	} else if($action == "edit_user"){
		print "$password <- ";
		admin::edit_user($id,$username,$email,$password,$rate,$hrate,$color,$activetechie);
	} else if($action == "users"){
		admin::list_users();
	} else {
		admin::add_perm_form();
		admin::list_perm();
	}

	break;


    case "doc":
        doc::navigation();
		//doc::table();
        if($action=="add")
        {
            doc::upload_form();
        }
        else if($action=="list")
        {
          doc::table();
        }
        else if($action=="upadd")
        {
          doc::add($uptitle,$updescription,$upfile,$tmpname,$filesize,$filetype,$errors,$category);
        }
        else if($action=="admin")
        {
          doc::list_edit();
        }
        else if($action=="edit" && $delete!="Delete")
        {

          doc::edit_form($id);
        }
        else if($delete=="Delete")
        {

          doc::delete($id,$docpath);
        }
        else if($action=="categ")
        {

          doc::category();
        }
        else if($action=="edit_cat")
        {

          doc::edit_catform($id);
        }
        else if($action=="edit_category" && $delete!='Deletecat')
        {

          doc::edit_cat($id,$category);
        }

        else if($action=="docold")
        {

          doc::tableold();
        }
         else if($delete=="Deletecat")
        {
         doc::deletecat($id);

        }
        else if($action=="cat_add")
        {

          doc::cat_add();
        }
        else if($action=="addcategory")
        {

          doc::addcategory($category);
        }
        else if($action=="doc_edit")
        {
          doc::edit($id,$uptitle,$updescription,$docpath,$category);
        }
        else
        {
            doc::table();
        }
    break;

   /* case help:
		help::welcome();
    break;
    */ 
    # Commented out by Sandiswa - 2019 Apr 25 

	case "client":
		if ($action == "edit" && isset($client_id)){
			//display edit form
			client::edit_form($client_id);
		} else if ($action == "add" && isset($company)){

			var_dump('add client to db');

			$client = new client($client_id, $company, $lastname, $firstname, $telephone, $fax, $cellphone, $home, $email, $saddress1, $saddress2, 
						$szip, $paddress1, $paddress2, $pzip, $customer);
		} else if ($action == "add" && !isset($company)){
			client::add_form();
		} else if ($action == "update" && isset($client_id)){
			client::update($client_id, $company, $lastname, $firstname, $telephone, $fax, $cellphone, $home, $email, $saddress1, $saddress2,
						$szip, $paddress1, $paddress2, $pzip, $customer);
		} else if ($action == "delete" && isset($client_id)){
			client::delete($client_id);
		}  else if ($action == "remove" && isset($client_id)){
			client::remove($client_id);
		}   else if ($action == "view" && isset($client_id)){
			client::info($client_id);
		}

	break;

    case "billing":
      billing::navigation();
 
      if ( $action == "add_form" ){
        billing::add_form($client,$_SESSION['id'],$date,$rate,$bill_hrs,$nonbill_hrs,$description,$arrive_time,$depart_time,$type,$travel, $site, $afterhour,$quote,$m_type);
      } else
      if ( $action == "add" ){
	billing::add($client,$_SESSION['id'],$date,$rate,$bill_hrs,$nonbill_hrs,$description,$arrive_time,$depart_time,$type,$travel, $site, $afterhour,$quote,$m_type,$billready);
      } else
      if ( $action == "edit" ){
        if ($delete == "Delete" && is_numeric($id)){
	  billing::delete($id,$_SESSION['id']);
	} else {
	  billing::edit($client,$date,$rate,$bill_hrs,$nonbill_hrs,$description,$arrive_time,$depart_time,$type,$travel,$id,$afterhour,$quote,$site,$m_type,$billready);
	  if($billing_post=='true')
	 billing::invoice_report($year,$month);
	 else
	  billing::report_techie_month($_SESSION['username'],date("Y"),date("n"),$sort_column);
     }
     
	}else
		if ( $action == "edit_maintenance" ){  
      	
		          if ($delete == "Delete" && is_numeric($id))
		          { 
		           billing::delete_maintenance($id,$_SESSION['id']);
		         }

         else {
		billing::edit_maintenance($client,$date,$rate,$bill_hrs,$nonbill_hrs,$description,$arrive_time,$depart_time,$type,$travel,$id,$afterhour,$quote,$site,$m_type);
		if($billing_post=='true')
          	billing::invoice_report($year,$month);
			
		else 
		  billing::maintenance_report_techie_month($_SESSION['username'],date("Y"),date("n"));
		
		}

    } else
      if ( $action == "edit_form" ){
	billing::edit_form($id);
	} 
   else if ( $action == "maintenance_report_techie_month"){
   	billing::maintenance_report_techie_month($_SESSION['username'],$year,$month);
   }
	
 else if ( $action=="edit_maintenance_form"){
  	billing::edit_maintenance_form($id);
 }
	else if($action=="billing_edit"){
        billing::billing_edit($id);
	}else
      if ( $action == "report" ){
        billing::report_techie_month($_SESSION['username'],$year,$month,$sort_column);
      } else if ( $action == "contract_report"){
        billing::contract_report($year,$month);
      }
      else if ( $action == "maintenance_contract_client"){
      	billing::maintenance_contract_client($id,$year,$month,$type);
      }
      
      else if ( $action == "maintenance_contract_report"){
      	billing::maintenance_contract_report($year,$month);
      }
       
       else if ( $action == "invoice_report"){
	if(!isset($sort_column))
        billing::invoice_report($year,$month);
	else billing::invoice_report($year,$month,$sort_column);
      } else if ($action == "invoice" && isset($id)){
        billing::invoice($id);

      } else if($action == "invoice_confirm" && isset($id)){
        billing::invoice_confirm($id);
      } else if ($action == "contract_client" && isset($id)){
        billing::contract_client($id,$year,$month);
      } else if ($action == "uninvoiced"){

	       if(isset($sort_column)){
        billing::uninvoiced($sort_column);
	}else{
        billing::uninvoiced();
         }

      } else if ($action == "breakdown"){
       // billing::main_report($year,$month);
      } else if ($action == "weekly"){
       // billing::weekly_report($year,$week);
      }
      else if ($action == "remote_onsite_report"){
      	billing::remote_onsite_report($year,$month,$sort_column);
      }
     else if ($action == "techie_billing"){
     billing::techie_billing($techie,$year,$month,$sort_column);
     }
     else if ($action == "customer_summary"){
     billing::customer_summary($year,$month);
     }
    
       
       else {
      //  print '<table><tr>  <td valign="top">';
        billing::report_techie_month($_SESSION['username'],$year,$month,$sort_column);
	//print '</td>';

         /* basically I moved the code that shows a contract report summary from here to the report_techie_month function so that it will always show a summary */
   //      print '</tr></table>';
      }


    break;


    case "abook":

        if((isset($search_string)) && !(isset($sort_column))){
	abook::search($search_string);
	}else if((isset($search_string)) && (isset($sort_column))){
         abook::search($search_string,$sort_column);
	}else if((isset($sortcolumn)) && (!(isset($search_string)))){
        abook::search(NULL,$sortcolumn);
	}else if($action == "show_all"){
        abook::search($search_string,$sort_column,$action);
      }
      else if($action =="customer"){
        abook::search($search_string,$sort_column,$action);
      }
      else if($action =="non_customer"){
        abook::search($search_string,$sort_column,$action);
      }
      else{
        abook::form();
      }

    break;
/*
    case domain:
    	// add
    	//edit
    	//delete
    	// really delete
    	// list
    	domain::view();
    break;

    case website:
    	// add
    	//edit
    	//delete
    	// really delete
    	// list
    	website::view();
    break;
*/# Commented out by Sandiswa - 2019 Apr 25 
    case "reports":
	billing::main_report($year,$month);
    break;

    case "contract":
    contract::navigation();
      if($action == 'add_form'){
        contract::add_form();
      } 
      else if($action =='maintenance'){
      	contract::maintenance($contract_month,$contract_year);
      }
       else if($action =='maintenance_termination'){
       	contract::maintenance_termination($client,$hours,$identity,$year2,$month2,$year3,$month3,$termmonth,$termyear,$term,$primary,$backup,$server_hours,$server_techie,$m_type);
	
       }
  
   else if($action == 'add' && isset($company)){
        contract::add($company,$hours,$year2,$month2,$year3,$month3,$primary,$backup,$server_hours,$server_techie);
      } else if($action == 'edit_form'){
        contract::edit_form($id,$identity,$contract_month,$contract_year);
      }
      else if($action == 'termedit'){
        contract::terminatingedit_form($id,$identity,$contract_month,$contract_year);
       }
       else if($action == 'editmaintenance_form'){
       	contract::editmaintenance_form($id,$identity,$contract_month,$contract_year);
       }
       
       
      else if($action=='terminate') {
		 
	contract:: terminate($contract_month, $contract_year);

  }
  
else if($action=='audit') {

	contract:: audit($contract_month, $contract_year);

  }else if ( $action == "invoice_report"){
	if(!isset($sort_column))
        contract::invoice_report($year,$month);
	else contract::invoice_report($year,$month,$sort_column);
      } else if ($action == "invoice" && isset($id)){
        contract::invoice($id);

      }
  
 else if ($action == "invoice" && isset($id)){
        contract::invoice($id);

      } else if($action == "invoice_confirm" && isset($id)){
        //contract::invoice_confirm($id);
		billing::invoice_confirm($id);
      } else if ($action == "contract_client" && isset($id)){
        billing::contract_client($id,$year,$month);
      } else if ($action == "uninvoiced"){

	       if(isset($sort_column)){
        contract::uninvoiced($year,$month,$sort_column);
	}else{
        contract::uninvoiced($year,$month,$sort_column);
         }

      }

  else if($action=='term'&& $delete !='Delete') {

	contract::termination($company,$hours,$identity,$year2,$month2,$year3,$month3,$termmonth,$termyear,$term);

  }
  else if ($action=='term'&& $delete == "Delete" && isset($company)){

  contract::deletecont($identity,$company);
  }
  
   else if ($action=='maintenance_deletecont'&& $delete =='Delete'  && isset($company)) {
    contract::maintenance_deletecont($identity,$company);
   }

      else if($action == 'edit'){
        if ($delete == "delete" && isset($company)){
        contract::delete($company);
	} else {
        contract::edit($client,$hours,$identity,$year2,$month2,$year3,$month3,$primary,$backup,$server_hours,$server_techie,$contract_month,$contract_year);
	}
     } else {
          if(!isset($contract_year)&&!isset($contract_month))
{
       $contract_year = $year;
       $contract_month = $month;

      contract::listall($contract_month, $contract_year);
} else {

	contract::listall($contract_month, $contract_year);

  }
     }      

      break;
      
    /*  case adhoc_customers:
      	adhoc::navigation();
      	
      	if($action == 'add_form'){
      		adhoc::add_form();
      	} else if($action == 'add' && isset($company)){
      		adhoc::add($company,$hours);
      	} else if($action == 'edit_form'){
      		adhoc::edit_form($client);
      	}
      	 else if($action == 'edit'){
      		adhoc::edit($client,$hours);
      	}
      	
      	else {
      		adhoc::listall();
      	}
break;*/# Commented out by Sandiswa - 2019 Apr 25 

case contractadmin:
    contract::navigation();
     if($action == 'termedit'){
        contract::terminatingedit_form($id,$identity);
      }
      else if($action=='terminate') {

	contract:: terminate($contract_month, $contract_year,$sort_column);

  }
  else if ( $action == 'maintenance_termination' ){
  contract::editmaintenance_form($client,$hours,$identity,$year2,$month2,$year3,$month3,$termmonth,$termyear,$term,$primary,$backup,$server_hours,$server_techie);
  }

  else if($action=='term'&& $delete !='Delete') {

	contract:: termination($company,$hours,$identity,$year2,$month2,$year3,$month3,$termmonth,$termyear,$term,$effective);

  }
  else if ($action=='term'&& $delete == 'Delete' && isset($company)){

  contract::deletecont($identity);
  }
  else
  {
   contract:: terminate($contract_month, $contract_year);
  }
    break;

 /*   case adsl:
	    if($action == 'update'){
                //if((adsl::edit_confrim(blbbl))&&(adsl::edit()))
               if(adsl::adsl_edit(
                $confirm,
                $id,
                $ADSL_Password,
                $ADSL_Contract,
                $ADSL_Adhoc,
                $ADSL_Session,
                $ADSL_Throttle,
                $ADSL_Name,
                $ADSL_Tel,
                $ADSL_Cell,
                $ADSL_Fax,
                $ADSL_Email)) {
                    adsl::listall();
                }else{
                    adsl::edit_form($id);
                }
	      }else if ($action == 'edit'){
	        adsl::edit_form($id);
	      }else if ($action == 'delete' && isset($id)){
		if(adsl::delete(
                    $confirm,
                    $id)) {
                    adsl::listall();
                }else{
                    adsl::edit_form($id);
                }
	      }else if ($action == 'add_form'){
		adsl::add_form();
              } else if ($action == 'month_stats'){
	        adsl::month_stats($adsl_username,$month,$year);
              }else if ($action == 'adsl_summary'){
	        adsl::adsl_summary($adsl_account,$month,$year);
              } else if ($action == 'day_stats'){
	        adsl::day_stats($adsl_username,$month,$year,$day);
	      }else if ($action == 'add'){
                if(adsl::add(
                $company,
                $ADSL_Username,
                $ADSL_Realm,
                $ADSL_Password,
                $ADSL_Contract,
                $ADSL_Session,
                $ADSL_Name,
                $ADSL_Tel,
                $ADSL_Cell,
                $ADSL_Fax,
                $ADSL_Email,
                $ADSL_AcctType,
		  		$ADSL_Provider
                )) {
                    adsl::listall();
                }else{
                    adsl::add_form();
                }
	      }else{
		adsl::listall();
	      }
    break;
*/# Commented out by Sandiswa - 2019 Apr 25 
        case password:
          if($action == 'edit'){
            password::edit($id,$client,$server,$os,$login,$password);
            password::search_form();
          }
          else if ($action == 'edit_form'){
            password::edit_form($id);
          }
        else if ($action == 'delete' && isset($id)){
        //  	else if ($action == 'delete'){
            password::delete($id);
            password::search_form();
          }
          else if ($action == 'add_form'){
            password::add_form();
          }
          else if ($action == 'add'){
            password::add($client,$server,$os,$login,$password);
            password::search_form();
          }
	  else if ($action == 'listall'){
            password::listall();
	  }
	  else if ($action == 'search'){
            password::listall($search_string);
	  }
          else{
             password::search_form();
          }
     break;
       /* case ics:
        ics::showmenu();
        break;
       */ # Commented out by Sandiswa - 2019 Apr 25 
     
        case tier:
     if($action == 'edit'){
         tier::edit_form($client_id);
      }
         
     else
         if($action == 'update'){
        tier::update($id, $company, $lastname, $firstname, $telephone, $fax, $cellphone, $home,  $email, $saddress1, $saddress2, $szip, $paddress1, $paddress2, $pzip, $customer,$tier);
           }
    else if($action == 'add'){
         tier::add_form();
      }
         
         else {
          tier::listing();
         } 
          break;
		  
/*case dialup:
				if($action == 'add_form'){
                                        dialup::add_form();
                                }
				else if($action == 'add'){
                                        dialup::add($client,$userid,$password,$framed_ip_address,$active,$msi_userid);
                                        dialup::listall();
                                }

                                else if($action == 'edit'){
                                        dialup::edit($id,$userid,$password,$framed_ip_address,$active,$userid);
                                        dialup::listall();
                                }
                                else if($action == 'edit_form'){
                                        dialup::edit_form($id);
                                }
                                else if($action == 'delete'){
                                                dialup::delete($id);
                                                dialup::listall();
                                }
                                else{
                                                dialup::listall();
                                }
        break;*/# Commented out by Sandiswa - 2019 Apr 25 

    default:
    main::links();
    break;
    
    case billing_reports:
    	billing_reports::navigation();
    	if ($action == "breakdown"){
    		billing_reports::main_report($year,$month);
    	
    	} else if ($action == "weekly"){
    		billing_reports::weekly_report($year,$week);
    		
    	} else if ($action == "techie_billing_monthlygap"){
   			billing_reports::techie_billing_monthlygap($techie,$year,$month);
    	}
    	else if ($action == "remote_onsite_report"){
    		billing_reports::remote_onsite_report($year,$month,$sort_column);
    	}
    	else if ($action == "techie_billing"){
    		billing_reports::techie_billing($techie,$year,$month,$sort_column);
    	}
    	else if ($action == "monthly_gap"){
    		billing_reports::monthly_gap($techie,$year,$month,$sort_column);
        }
        else if ($action == "between_gaps"){
    		billing_reports::between_gaps($techie,$year,$month,$sort_column);
        }
    	else if ($action == "customer_summary"){
    		billing_reports::customer_summary($year,$month);
    	}
    	else if ($action == "maintenance_contract_report"){
    		billing_reports::maintenance_contract_report($year,$month);
    	}
    	else if ( $action == "maintenance_contract_client"){
    		billing_reports::maintenance_contract_client($id,$year,$month,$cid);
    	}
    	
    	else if($action = "bill_nonbill"){
    		billing_reports::bill_nonbill($year,$month);
    	}
    	else if($action = "obligated_hours"){
    		billing_reports::obligated_hours($year,$month,$sort_column);
    	}
    	
    	else {
    		billing_reports::main_report($year,$month);
    	}
    
    	break;
    	
    	case mailhours:
    		//mailing::mail();

}


?>

</div>
<p></p>
<p></p>
<p></p>
<p></p>
<p></p>
<p></p>
<div class="ad">Open Source Solutions for Open Minds&nbsp;</div>
<div class="footer">
	Mindspring Intranet<br>
	Copyright &copy; 2004-2007 Mindspring Computing<br>
	support@mindspring.co.za
</div>

<!-- Overlay -->
<div class="modal-overlay" style="display:none;"></div>

<!-- Modal -->
<div class="modal" style="display:none;">
	<div class="modal-content">
		<div class="modal-header">
			<div class="modal-title">Contract Hours Summary</div>
			<div class="modal-close-x">&times;</div>
		</div>
		<div class="modal-body">
			<div>
				<table style="width:100%; border-collapse: collapse; margin: 0 0 0 0;" class="table table-bordered modal-body-table">
					<form id="send-report-form">

						<!-- <div class="date-container">
							<div style="single-date">
								<label for="datefilter">Enter Start Date:</label>
								<br /><br />
								<input type="text" id="datefilter" name="datefilter" required>
								<br /><br />
							</div>
						</div> -->

						<div>
							<p>Please select a send type:</p>
							<input type="radio" id="normal" name="send_type" value="normal">
							<label for="normal">Normal Send</label><br>
							<input type="radio" id="testCC" name="send_type" value="testcc">
							<label for="testCC">Test Send (with CC)</label><br>  
							<input type="radio" id="testNoCC" name="send_type" value="testnocc">
							<label for="testNoCC">Test Send (without CC)</label><br><br>
						</div>

						<div>
							<label for="email-list">Enter email address:</label>
							<br /><br />
							<input type="text" id="email-list" name="email-list" value="" placeholder="Email Address" required>
							<br /><br />
						</div>

						<div>
							<button id="send-this-report" class="btn btn-secondary">Send Report</button>
						</div>

					</form>
				</table>
			</div>
		</div>
		<div class="modal-footer">
			<button class="btn btn-primary modal-close-button">Close</button>
		</div>
	</div>
</div>

</body>
</html>
