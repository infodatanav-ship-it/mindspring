<?php
session_start();
?>
<html>
<head>
<title>Mindspring SMS Portal</title>
<meta name='author' value='Marius Pese' />
<SCRIPT LANGUAGE="JavaScript">
<!-- Original:  Ronnie T. Moore -->
<!-- Web Site:  The JavaScript Source -->
<!-- Begin
function textCounter(field, countfield, maxlimit) {
if (field.value.length > maxlimit) // if too long...trim it!
field.value = field.value.substring(0, maxlimit);
// otherwise, update 'characters left' counter
else 
countfield.value = maxlimit - field.value.length;
}
// End -->
</script>

<SCRIPT LANGUAGE="JavaScript">
<!--

<!-- Begin
function Check(chk)
{
if(document.myform.Check_ctr.checked==true){
for (i = 0; i < chk.length; i++)
chk[i].checked = true ;
}else{

for (i = 0; i < chk.length; i++)
chk[i].checked = false ;
}
}

// End -->
</script>

</head>
<body>
<br />
<?php
// Settings provided by Clickatell. Login to the website if ever outdated to retrieve new.
$user = "mindspring";
$password = "zur3360u39";
$api_id = "3206492";
$baseurl ="http://api.clickatell.com";
$to = "";
$from= "";
// End of clickatell settings
$myname=$_SESSION['username'];

$link = mysql_connect("localhost", "msi", "144455caregfdrrra")
		or die("Could not connect : " . mysql_error());
	mysql_select_db("msi") or die("Could not select database");
$sql=mysql_query('SELECT `name`, `number` FROM msi_sms_customers ORDER BY `name`');

$whoami=mysql_query('SELECT `Name`, `Number` FROM `msi_sms_users` WHERE `msi_sms_users`.`Name` LIKE "'.$myname.'"');
$hereiam=mysql_fetch_row($whoami);
$myname=$hereiam[0];
if ($myname=='') {die('You are not logged in! <a href="index.php">Back to the Intranet</a>');}
$from=$hereiam[1];
echo '<u>Hello '.$myname.', ';
?>
please select your recipients:</u><br />
<p style="font-size:smaller"><i>Every checked recipient will get a copy. <br />
Your cellnumber will be used as senderID.</i></p>

<form name="myform" action="sms_customers.php" method="post">


<?php
	$count=mysql_num_rows($sql);
	$halfcount=round($count/2); 
echo '<table>';	
	for ( $i=1; $i<=$count; $i++) { 
		$row=mysql_fetch_row($sql);
		if($i%2) {//odd
		$waleed=substr_replace ($row[1],'wally', 0, 0);
	        echo $waleed;   
		?>
			<tr><td><input type='checkbox' name='check_list' value='<?php echo $row[1]; ?>' /><?php echo $row[0].' ('.$row[1].')'; ?></td><td width=30px></td>
		<?php
		}else{//even
		?>
		<td><input type='checkbox' name='check_list' value='<?php echo $row[1]; ?>' /><?php echo $row[0].' ('.$row[1].')'; ?></td></tr>
		<?php
		}
	      }
?>
</table>



<input type="checkbox" name="Check_ctr" value="yes" onClick="Check(document.myform.check_list)"><b>Check All</b><br />


<br />
<textarea name=message wrap=physical cols=50 rows=4 onKeyDown="textCounter(this.form.message,this.form.remLen,160);" onKeyUp="textCounter(this.form.message,this.form.remLen,160);" onfocus="if(this.value==this.defaultValue){this.value='';}" onblur="if(this.value==''){this.value=this.defaultValue;}">Please enter your message here</textarea>
<br>
<input readonly type=text name=remLen size=3 maxlength=3 value="160"> characters left</font>
<br />
<br />
Alright, please <input type='submit' value='send this message' name='send'>

</form>

<?php

if (isset($_POST['send'])) 
{

$url = "$baseurl/http/auth?user=$user&password=$password&api_id=$api_id";
// do auth call
$ret = file($url);
// split our response. return string is on first line of the data returned
$sess = split(":",$ret[0]);
  if ($sess[0] == "OK") {
  $sess_id = trim($sess[1]); // remove any whitespace
  
$to=$_POST['check_list'];
$message=$_POST['message'];
$text = urlencode("$message");

	foreach ($to as &$value) 
	{
	if ($value=='') {break;}
	$url = "$baseurl/http/sendmsg?session_id=$sess_id&to=$value&text=$text&from=$from";
	echo $url;
	// do sendmsg call
	$ret = file($url);
	$send = split(":",$ret[0]);
		if ($send[0] == "ID") {
			echo "Message successfully sent to: ". $value."<br /><a href='http://intranet.mindspring.co.za/?'>Take me home</a>'";
		}else{
			echo "Send message failed: " . $send[0] . $send[1]."<br /><a href='http://intranet.mindspring.co.za/?'>Take me home</a>'";
		}
	}
	unset($value);

  }
}

?> 
</body>
</html>
