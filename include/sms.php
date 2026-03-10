<?php
session_start();
// include('sql.php.inc');
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

$myname=$_SESSION['username'];

//$link = mysql_connect("localhost", "msi", "twrzTFdG6zZKf5jN")
//		or die("Could not connect : " . mysql_error());
//	mysql_select_db("msi") or die("Could not select database");
	
	
	
//  $link = mysqli_connect("intranetvm.mindspring.local", "msi_user", "hag!hd3@1zfa","msi");
		
// if (!$link) {
//     echo "Error: Unable to connect to MySQL." . PHP_EOL;
//     echo "Debugging errno: " . mysqli_connect_errno() . PHP_EOL;
//     echo "Debugging error: " . mysqli_connect_error() . PHP_EOL;
//     exit;
// }


$sql = sql::sql_run('SELECT `Name`, `Number` FROM msi_sms_users ORDER BY `Name`');
$hereiam = sql::sql_run('SELECT `Name`, `Number` FROM `msi_sms_users` WHERE `msi_sms_users`.`Name` LIKE "'.$myname.'"');

$myname=$hereiam[0]['Name'];
$from=$hereiam[0]['Number'];

var_dump($sql);
// $sms_users = $sql[0];
var_dump($sms_users);

foreach ($sql as $row) {
  var_dump($row);
  echo "<br />";
  echo "<br />";
}

echo "<h2>Mindspring SMS Portal</h2>";


echo "<u> Hello " . $myname . ", please select your recipients:</u><br />";
?>
<p style="font-size:smaller">
  <i>Every checked recipient will get a copy, if the custom field is set a copy will be sent to this number as well. <br />
  Your cellnumber will be used as senderID.</i><br />
<!---   Want to notify customers of internet / email problems? Click <a href="sms_customers.php">here</a>  --> </p>

<form name="mainform" action="include/sms.php" target="_self" method="post">
<?php

  // $count=mysqli_num_rows($sql);
	// $halfcount=round($count/2); 

  echo '<table>';


  var_dump( "start table" );
  // $i = 0;
  foreach ($sql[0] as $row) {



    var_dump($row);
    $replnum=substr_replace ($row['Number'],'0', 0, 2);
    $replnum=substr($replnum, 0, 3)." ".substr($replnum, 3, 3)." ".substr($replnum, 6, 4);

    echo "<tr><td><input type='checkbox' name='recipient[]' value='" . $row['Number']; "' />" . $row['Name'].' ('.$replnum.')' . "</td><td width='30px'></td>";

    // if($i%2) {//odd
      // echo "<tr><td><input type='checkbox' name='recipient[]' value='" . $row['Number']; "' />" . $row['Name'].' ('.$replnum.')' . "</td><td width='30px'></td>";
    // }else{//even
      // echo "<td><input type='checkbox' name='recipient[]' value='" . $row[1]; "' />" . $row[0].' ('.$replnum.')' . "</td></tr>";
    // }
    // $i++;
  }

?>
</table>
	
<input type='text' name='recipient[]' value=''> Custom number<br /><br />
<textarea name=message wrap=physical cols=50 rows=4 onKeyDown='textCounter(this.form.message,this.form.remLen,160);' onKeyUp='textCounter(this.form.message,this.form.remLen,160);' onfocus='if(this.value==this.defaultValue){this.value='';}' onblur='if(this.value==''){this.value=this.defaultValue;}'>Please enter your message here</textarea>
<br>
<input readonly type=text name=remLen size=3 maxlength=3 value='160'> characters left</font>
<br />
<br />
Alright, please <input type='submit' value='send this message' name='send'>

</form>


<?php

if (isset($_POST['send'])) 
{
		

//$url = "$baseurl/http/auth?user=$user&password=$password&api_id=$api_id";
///do auth call
//$ret = file($url);
// split our response. return string is on first line of the data returned
//$sess = split(":",$ret[0]);
// if ($sess[0] == "OK") {
//$sess_id = trim($sess[1]); // remove any whitespace
  

$message=$_POST['message'];
$text = urlencode($message);
$user = "mindspring";
$password = "jifejare";
//$api_id = "3206492";
//$baseurl ="http://www.mymobileapi.com";
$from= $hereiam[0]['Number'];
foreach($_POST['recipient'] as $value){
	
$data= array(
"Type"=> "sendparam", 
"Username" => $user,
"Password" => $password,
"live" => "true",
"numto" => $value,
"data1" => $message
) ; 

//This contains data that you will send to the server.
$data = http_build_query($data); //builds the post string ready for posting
echo do_post_request('https://www.mymobileapi.com/api5/http5.aspx', $data);
echo "<p>Message successfully sent to: ". $value."<br /><a href='http://dev.mindspring.co.za/?'>Take me home</a>'</p>";
/*unset($value);
}
else{
echo "<p>Send message failed: " . $value."<br /><a href='http://dev.mindspring.co.za/?'>Take me home</a>'</p>";
}	
unset($value);
}*/
}
}

//	foreach ($to as &$value) 
//	{
//	if ($value=='') {break;}
//	$url = "$baseurl/http/sendmsg?session_id=$sess_id&to=$value&text=$text&from=$from";
	// do sendmsg call
//	$ret = file($url);
//	$send = split(":",$ret[0]);
//		if ($send[0] == "ID") {
 function do_post_request($url, $data, $optional_headers = null)
  {
     $params = array('http' => array(
                 'method' => 'POST',
                  'content' => $data
              ));
     if ($optional_headers !== null) {
        $params['http']['header'] = $optional_headers;
     }
     $ctx = stream_context_create($params);
     $fp = @fopen($url, 'rb', false, $ctx);
     if (!$fp) {
        throw new Exception("Problem with $url, $php_errormsg");
        	
     }
     $response = @stream_get_contents($fp);
     if ($response === false) {
     	echo "responce = false";
        throw new Exception("Problem reading data from $url, $php_errormsg");
        
     }
     $response;
    return formatXmlString($response);
  }

function formatXmlString($xml) 
{  
	
  // add marker linefeeds to aid the pretty-tokeniser (adds a linefeed between all tag-end boundaries)
  $xml = preg_replace('/(>)(<)(\/*)/', "$1\n$2$3", $xml);
  	
  // now indent the tags
  $token      = strtok($xml, "\n");
  $result     = ''; // holds formatted version as it is built
  $pad        = 0; // initial indent
  $matches    = array(); // returns from preg_matches()
  if ($token === false){

  // scan each line and adjust indent based on opening/closing tags
  while ($token !== false) {
 
    // test for the various tag states
    
    // 1. open and closing tags on same line - no change
    if (preg_match('/.+<\/\w[^>]*>$/', $token, $matches)) : 
      $indent=0;
    // 2. closing tag - outdent now
    elseif (preg_match('/^<\/\w/', $token, $matches)) :
      $pad--;
    // 3. opening tag - don't pad this one, only subsequent tags
    elseif (preg_match('/^<\w[^>]*[^\/]>.*$/', $token, $matches)) :
      $indent=1;
    // 4. no indentation needed
    else :
      $indent = 0; 
    endif;
    	
    // pad the line with the required number of leading spaces
    $line    = str_pad($token, strlen($token)+$pad, ' ', STR_PAD_LEFT);
    $result .= $line . "\n"; // add to the cumulative result, with linefeed
    $token   = strtok("\n"); // get the next token
    $pad    += $indent; // update the pad size for subsequent lines    
  }
 
  return $result;

}
//}else{
//echo "Send message failed: " . $send[0] . $send[1]."<br /><a href='http://intranet.mindspring.co.za/?'>Take me home</a>'";
//unset($value);

}
 ?> 
</body>
</html> 


