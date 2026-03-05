<?php
// $Id: adsl.php.inc,v 1.14 2009/03/27 08:22:40 etienne Exp $

// 20123Feb30-10:55-SS-Ver 1.17-sessions that have a START time in any given
//          month are included in that month's stats. 

// 20121Feb30-16h30-SS-Ver 1.16-Show Gb colums on ADSL total bandwidth

// 2012 Jan 30th, 11:35 am - Sandiswa Siga - Ver 1.15
// Added summary of usage by realm/ADSL provider, changed layout of ADSL a/c's

class adsl {
	
  
  function listall() {

  	if(!acl::has_access($_SESSION['id'],"adsl","read")) return 1;
    ?>  <!-- placed this div is below next to ADSL User Accounts(Sandiswa)-->
    <!--  <div class="links">
        <a href="?module=adsl&action=add_form" style="color:red">>>New ADSL Account<<</a>
    </div>--><?php
    
  
  //(2012-03Febr Sandiswa- changed the query to use acctStartTime to calulate bandwidth used for current month).
    
  
   $results = sql_run("
    SELECT
      `msi_clients`.`company`,
      `msi_adsl`.`id`,
      `msi_adsl`.`client`,
      CONCAT(`msi_adsl`.`username`,'@',`msi_adsl`.`realm`) AS username,
      `msi_adsl`.`cap_size`,
      `msi_adsl`.`throttle_size`,
      `msi_adsl`.`canceled`,
      IFNULL(adhoc.`adhoc`,0) AS adhoc,
      IFNULL(used.`total`,0) AS total,
      IFNULL(used.`total`,0)/((`msi_adsl`.`cap_size`+IFNULL(adhoc.`adhoc`,0))*(1024*1024*1024)/100) AS percent,
      msi_adsl.account
    FROM
      `msi_clients`,
      `msi_adsl`
      LEFT JOIN
        (
          SELECT
            `adsl_adhoc`.`id`,
            SUM(`adsl_adhoc`.`value`) AS adhoc
          FROM
            `adsl_adhoc`
          WHERE
            `adsl_adhoc`.`time` LIKE DATE_FORMAT(NOW(),'%Y-%m-%%')
          GROUP BY
            `adsl_adhoc`.`id`
        ) AS adhoc
      ON
        (`msi_adsl`.`id` = adhoc.`id`)
      LEFT JOIN
      (
        SELECT
          clean.`UserName`,
          SUM(clean.`AcctInputOctets`)+SUM(clean.`AcctOutputOctets`) AS total
        FROM
        (
          SELECT 
            `radacct`.`UserName`,
            MAX(`radacct`.`AcctInputOctets`) AS AcctInputOctets,
            MAX(`radacct`.`AcctOutputOctets`) AS AcctOutputOctets
          FROM 
            `radacct`
          WHERE
            UNIX_TIMESTAMP(`radacct`.`AcctStartTime`)  >=
            UNIX_TIMESTAMP(DATE_FORMAT(NOW(),'%Y-%m-01 00:00:00'))
          GROUP BY
            `radacct`.`AcctUniqueId`
        ) AS clean
        GROUP BY
          clean.`UserName`
      ) AS used 
    ON 
      (CONCAT(`msi_adsl`.`username`,'@',`msi_adsl`.`realm`) = used.`Username`)
    WHERE
      `msi_adsl`.`client` = `msi_clients`.`id`
    AND
    (
     	`msi_adsl`.`canceled` = '0000-00-00'
    OR
	`msi_adsl`.`canceled` >= DATE_FORMAT(NOW(),'%Y-%m-00')
    )	
    ORDER BY
      `msi_clients`.`company` ASC
    "); 

  
    $totalCap=0;
    $totalUsed=0;

  
  ?>
  <p></p>
  <span class="head">ADSL User Accounts <i>total: <?php
    print count($results);
  ?></i></span>
  <!-- this div is from top,(Sandiswa)-->
  <div class="links">
        <a href="?module=adsl&action=add_form" style="color:red">>>&nbsp;New ADSL Account<<</a>
    </div>
    <div class="links">
        <a href="?module=adsl&action=adsl_summary" style="color:red">>ADSL Account Summary<</a>
    </div>
  <table class="client" width = 1050>
    <tr class="head">
      <td align="center">
        Client
      </td>
      <td align="center">
        ADSL Account
      </td>
      <td align="center">
        Account Type
      </td>
      <td align="center" width = 80>
        Cap
      </td>      
      <td align="center" width = 80>
        Used
      </td>
      <td align="center">
        Status 
      </td>
    </tr>
  <?php
  foreach ($results as $result) {
    ?><tr>
      <td class="client">
        <a href="index.php?module=client&action=view&client_id=<?php
          print $result["client"];
          //removed underlines for links (Sandiswa)
        ?>" STYLE="text-decoration: none"><?php
          print $result["company"];
        ?></a>
      </td>
      <td class="client">
        <a href="index.php?module=adsl&action=edit&id=<?php
          print $result["id"];
           //removed underlines for links (Sandiswa)
        ?>" STYLE= "text-decoration: none"><?php
          print $result["username"];
        ?></a>
      </td>
	<td class="client" style = "font:smaller">
	<?php print $result["account"]; ?>
	</td>
      <td class="client" align="center">
        <?php
          if($result["adhoc"]) {
            print $result["cap_size"];
            ?>+<font color="green"><?php
            print $result["adhoc"];
            ?></font><?php
            print "=".($result["cap_size"]+$result["adhoc"]);
            $totalCap+=($result["cap_size"]+$result["adhoc"]);
          }else{
            print $result["cap_size"];
            $totalCap+=$result["cap_size"];
          }
        ?> Gb
      </td>
      <td class="client" align="center">
        <?php
          print number_format(($result["total"]/(1024*1024*1024)),2);
          $totalUsed+=number_format(($result["total"]/(1024*1024*1024)),0);
        ?> Gb
      </td>
      <td class="client" align="center">
        <?php
          if($result["canceled"]!='0000-00-00') {
            ?><font color="grey"><b><i>CANCELLED</i></b></font><?php
          }else if($result["percent"]>=100){
            ?><font color="red"><b>CAPPED</b></font><?php
          }else if(($result["cap_size"]+$result["adhoc"]-$result["throttle_size"])<=
                   ($result["total"]/(1024*1024*1024))) {
            ?><font color="#CD7F32"><b>THROTTLED</b></font><?php
          }else{
            ?><font color="green">OK</font><?php
          }
        ?>
      </td>
    </tr>
    <?php
    }
    ?>
    <tr>
	<td class="client"></td>
      <td class="client">
      </td>
      <td class="client">
        <b>Total</b>
      </td>
      <td class="client" align="center">
        <b><?php
          print $totalCap
        ?> Gb</b>
      </td>
      <td class="client" align="center">
        <b><?php
          print $totalUsed
        ?> Gb</b>
      </td>
      <td class="client" align="center">
        <b>
          <?php
            print number_format($totalUsed/($totalCap)*100,2);
          ?> %
        </b>
      </td>
    </tr>
    <?php
  }
  
  function adsl_edit(
    $confirm,
    $id,
    $Password,
    $Contract,
    $Adhoc,
    $Session,
    $Throttle,
    $Name,
    $Tel,
    $Cell,
    $Fax,
    $Email) {

    if(!acl::has_access($_SESSION['id'],"adsl","edit")) return 1;
    
    if(!preg_match('/^[0-9]+$/',$id)) {
      ?>
      <font color="red"><b>ERROR: ID is not valid</b></font><?php;
      adsl::listall();
      exit;
    }
    if(!preg_match('/^[a-z0-9]+$/',$Password)) {
      ?><font color="red"><b>ERROR: Password is not valid, expected 'a' to 'z', '0' to '9'</b></font><?php
      return FALSE;
    }
    if(!preg_match('/^[0-9]{1,2}$/',$Contract)) {
      ?><font color="red"><b>ERROR: Contract Cap is not valid, expected a value between 0 and 99</b></font><?php
      return FALSE;
    }
    if(!$Contract) {
      ?><font color="red"><b>ERROR: Contract Cap may not be 0</b></font><?php
      return FALSE;
    }    
    if(!preg_match('/^[0-9]?[0-9]?$/',$Adhoc)) {
      ?><font color="red"><b>ERROR: Adhoc Cap is not valid, expected a value between 0 and 99</b></font><?php
      return FALSE;
    }
//    if(!preg_match('/^[0-9]?[0-9]?$/',$Session)) {
//      ?><!-- <font color="red"><b>ERROR: Session Time is not valid, expected a value between 1 and 24</b></font>--><?php
//      return FALSE;      
//    }
    if(!preg_match('/^[0-9]{1,2}(\.{1}[0-9]{1,2})?$/',$Throttle)) {
      ?><font color="red"><b>ERROR: Throttle Size is not valid, expected a value between 0 and 99</b></font><?php
       return FALSE;      
    }    
    if(!preg_match('/^[a-zA-Z\.\ \-]*$/',$Name)) {
      ?><font color="red"><b>ERROR: Contact Name is not valid, expected 'A' to 'Z', 'a' to 'z' with spaces and a optional '.'</b></font><?php
      return FALSE;
    }    
    if(!preg_match('/^([0-9]{10})?$/',$Tel)) {
      ?><font color="red"><b>ERROR: Contact Tellephone number is not valid, expected '0' to '9' 10 digits or nothing</b></font><?php
      return FALSE;
    }
    if(!preg_match('/^([0-9]{10})?$/',$Cell)) {
      ?><font color="red"><b>ERROR: Contact Cellphone number is not valid, expected '0' to '9' for 10 digits or nothing</b></font><?php
      return FALSE;      
    }
    if(!preg_match('/^([0-9]{10})?$/',$Fax)) {
      ?><font color="red"><b>ERROR: Contact Fax number is not valid, expected '0' to '9' for 10 digits or nothing</b></font><?php
      return FALSE;
    }
    if(!preg_match('/^([a-z0-9\-_\+]+@[a-z0-9\-_]+(\.[a-z0-9\-_]+)+)?$/',$Email)) {
      ?><font color="red"><b>ERROR: Contact Email is not valid, format expected is 'user@domain.ltd'</b></font><?php
      return FALSE;
    }
    
    $result = sql_run(
      "SELECT
        `msi_adsl`.`canceled`
      FROM
        `msi_adsl`
      WHERE
        `msi_adsl`.`id`='$id'
      ");
    
    if(isset($result)) {
      if($result[0]["canceled"]!='0000-00-00') {
        ?><font color="red"><b>ERROR: Can not perform updates on CANCELLED Accounts.</b></font><?php
        return FALSE;
      }
    }else{
       ?><font color="red"><b>ERROR: SQL Lookup Failed! Please contact the site administrator.</b></font><?php
       return FALSE;
    }
    
    if(!isset($confirm)) {/*HACK: confirmation check*/
      $result = sql_run(
      "SELECT
        `msi_adsl`.`id`,
        `msi_adsl`.`username`,
        `msi_adsl`.`realm`,
        `msi_adsl`.`password`,
        `msi_adsl`.`cap_size`,
        `msi_adsl`.`session_time`,
        `msi_adsl`.`throttle_size`,
        `msi_adsl`.`name`,
        `msi_adsl`.`tel`,
        `msi_adsl`.`cell`,
        `msi_adsl`.`fax`,
        `msi_adsl`.`email`,
        `msi_adsl`.`canceled`,
        adhoc.`total`
      FROM
        `msi_adsl`,
        (
          SELECT
            SUM(`adsl_adhoc`.`value`) AS total
          FROM
            `adsl_adhoc`
          WHERE
            `adsl_adhoc`.`time` LIKE DATE_FORMAT(NOW(),'%Y-%m-%%')
          AND
            `adsl_adhoc`.`id` = '$id'
        ) AS adhoc
      WHERE
        `msi_adsl`.`id`='$id'
      LIMIT
        0,1");
    
      if(!isset($result)) {
        ?><font color="red"><b>ERROR: Could not find ADSL account (<?php print $id?>)! Please contact the site administrator.</b></font><?php
        adsl::listall();
        exit;
      }
      ?>
      <form name="adsl" action="index.php?module=adsl&action=update&userid=<?php print $result[0]["id"] ?>&confirm=no" method="POST">
      <table align="center">
	<tbody>
	  <tr>
	    <!-- Account Changes -->
	    <td valign="top">
	      <table class="box">
		<caption>Account Changes</caption>
		<tbody>
		  <tr>
		    <td>
     		      <label>Username:&nbsp</label>
		    </td>
		    <td>
		      <input type="text" value="<?php
                        print $result[0]["username"];
                        ?>" maxlength="24" readonly>&nbsp@
                      <input type="text" value="<?php
                        if($result[0]["realm"]=='dsl512msp.co.za') {
                          print "dsl512msp.co.za";
                        }else if($result[0]["realm"]=='lmsp.co.za') {
                          print "lmsp.co.za";
                        }else{
                          print "umsp.co.za";
                        }
                      ?>" readonly>
		    </td>
		  </tr>                  
                  <?php
                    if($result[0]["password"]!=$Password) {
                      ?>
                      <tr>
                        <td>
                          <label>Password:&nbsp</label>
                        </td>
                        <td>
                          Changed from '<font color="red"><b><?php
                            print $result[0]["password"];
                          ?></b></font>' to '<font color="green"><b><?php
                            print $Password;
                          ?>'</b></font>
                        </td>
                      </tr>
                      <?php
                    }
                  ?>
                  <?php
                    if($result[0]["cap_size"]!=$Contract) {
                      ?>
                      <tr>
                        <td>
                          <label>Contract Cap:&nbsp</label>
                        </td>
                        <td>
                          Changed from '<font color="red"><b><?php
                            print $result[0]["cap_size"];
                          ?></b></font>' to '<font color="greediten"><b><?php
                            print $Contract;
                          ?>'</b></font>
                        </td>
                      </tr>
                      <?php
                    }
                  ?>
                  <?php
                    if($Adhoc) {
                      ?>
                      <tr>
                        <td>
                          <label>Ad-Hoc Cap:&nbsp</label>
                        </td>
                        <td>
                          Add '<font color="green"><b><?php
                            print $Adhoc;
                          ?></b></font>' Gb
                        </td>
                      </tr>
                      <?php
                    }
                  ?>
                  <?php
                    if($result[0]["session_time"]!=$Session) {
                      ?>
                      <tr>
                        <td>
                          <label>Session Time:&nbsp</label>
                        </td>
                        <td>
                          Changed from '<font color="red"><b><?php
                            print $result[0]["session_time"];
                          ?></b></font>' to '<font color="green"><b><?php
                            print $Session;
                          ?>'</b></font>
                        </td>
                      </tr>
                      <?php
                    }
                  ?>
                  <?php
                    if($result[0]["throttle_size"]!=$Throttle) {
                      ?>
                      <tr>
                        <td>
                          <label>Throttle Size:&nbsp</label>
                        </td>
                        <td>
                          Changed from '<font color="red"><b><?php
                            print $result[0]["throttle_size"];
                          ?></b></font>' to '<font color="green"><b><?php
                            print $Throttle;
                          ?>'</b></font>
                        </td>
                      </tr>
                      <?php
                    }
                  ?>             
                </tbody>
              </table>
            </td>
          </tr>
          <tr>
            <td align="center">
              <label>Apply these changes?</label>
              <input type="hidden" value="<?php
                print $result[0]["id"];
              ?>" name=id>
              <input type="hidden" value="<?php
                print $Password;
              ?>" name=ADSL_Password>
              <input type="hidden" value="<?php
                print $Contract;
              ?>" name=ADSL_Contract>
              <input type="hidden" value="<?php
                print $Adhoc;
              ?>" name=ADSL_Adhoc>
              <input type="hidden" value="<?php
                print $Session;
              ?>" name=ADSL_Session>
              <input type="hidden" value="<?php
                print $Throttle;
              ?>" name=ADSL_Throttle>
              <input type="hidden" value="<?php
                print $Name;
              ?>" name=ADSL_Name>
              <input type="hidden" value="<?php
                print $Tel;
              ?>" name=ADSL_Tel>
              <input type="hidden" value="<?php
                print $Cell;
              ?>" name=ADSL_Cell>
              <input type="hidden" value="<?php
                print $Fax;
              ?>" name=ADSL_Fax>
              <input type="hidden" value="<?php
                print $Email;
              ?>" name=ADSL_Email>
            </td>
          </tr>
          <tr>
            <td align="center">
              <input type="submit" value="yes" name="confirm">
              <input type="submit" value="no" name="confirm">
            </td>
          </tr>
        </tbody>
      </table>   
      <?php
      exit;
    }else if($confirm != 'yes') {
      return FALSE;
    }
    
    $result = sql_run(
    "UPDATE
      `msi_adsl`
    SET
      `msi_adsl`.`password`='$Password',
      `msi_adsl`.`cap_size`='$Contract',
      `msi_adsl`.`session_time`='$Session',
      `msi_adsl`.`throttle_size`='$Throttle',
      `msi_adsl`.`name`='$Name',
      `msi_adsl`.`tel`='$Tel',
      `msi_adsl`.`cell`='$Cell',
      `msi_adsl`.`fax`='$Fax',
      `msi_adsl`.`email`='$Email',
      `msi_adsl`.`throttle_notify`='0'
    WHERE
      `msi_adsl`.`id`='$id'
    ");
    if(!isset($result)) {
      $result = sql_run(
        "SELECT
          `msi_adsl`.`username`,
          `msi_adsl`.`realm`
        FROM
          `msi_adsl`
        WHERE
          `msi_adsl`.`id`='$id'
        ");
      ?><b>Updated <?php print $result[0][username]."@".$result[0][realm]." ($id)"?></b><?php     
    } else {
      ?><font color="red"><b>ERROR: Update Failed! Please contact the site administrator.</b></font><?php
      return FALSE;
    }
    // ADHOC HERE

    if($Adhoc) {

$results=sql_run("SELECT MAX(`time`) AS maxtime FROM adsl_adhoc WHERE `id`= '$id'");
$mins_ago = date('Y-m-d h:i:s', strtotime("-5 minutes +6 hours"));
$mins_ahead = date('Y-m-d h:i:s', strtotime("+5 minutes +6 hours"));
foreach ($results as $result) {
	$lastadd=$result['maxtime']; 
		if (($lastadd>$mins_ago) && ($lastadd<$mins_ahead)) {?> <br><font color="red"><b>ERROR: Please wait 5 minutes before adding bandwidth again.</b></font><?php return FALSE;}
		else {print $lastadd; print $mins_ahead;}
} 


      $result = sql_run(
        "INSERT INTO
          `adsl_adhoc` (
          `time`,
          `id`,
          `value`,
          `user`
          )
          SELECT
            NOW(),
            '$id',
            '$Adhoc',
            CONCAT(
              `msi_users`.`mail`,
              '@".$_SERVER["REMOTE_ADDR"]."'
            )
          FROM
            `msi_users`
          WHERE
            `msi_users`.`id` = '".$_SESSION["id"]."'
        ");
        if(!isset($result)) {
          ?><br><font color="green"><b>Added '<?php print $Adhoc ?>' Gigs (Adhoc)</b></font><?php
        }else{
          ?><br><font color="red"><b>ERROR: Adding Adhoc Bandwidth Failed! Please contact the site administrator.</b></font><?php
          return FALSE;
        }
    }
    ?><br>The changes will become active when the database is rebuilt on the hour.<br><?php
    
    return TRUE;
  }
  
  function edit_form($id) {
    
    if(!acl::has_access($_SESSION['id'],"adsl","edit")) return 1;
    
    $result = sql_run(
    "SELECT
      `msi_adsl`.`id`,
      `msi_adsl`.`username`,
      `msi_adsl`.`realm`,
      `msi_adsl`.`password`,
      `msi_adsl`.`cap_size`,
      `msi_adsl`.`session_time`,
      `msi_adsl`.`throttle_size`,
      `msi_adsl`.`name`,
      `msi_adsl`.`tel`,
      `msi_adsl`.`cell`,
      `msi_adsl`.`account`,
      `msi_adsl`.`fax`,
      `msi_adsl`.`email`,
      `msi_adsl`.`canceled`,
      adhoc.`total`
    FROM
      `msi_adsl`,
      (
        SELECT
          SUM(`adsl_adhoc`.`value`) AS total
        FROM
          `adsl_adhoc`
        WHERE
          `adsl_adhoc`.`time` LIKE DATE_FORMAT(NOW(),'%Y-%m-%%')
        AND
          `adsl_adhoc`.`id` = '$id'
      ) AS adhoc
    WHERE
      `msi_adsl`.`id`='$id'
    LIMIT
      0,1");
    
    if(!isset($result)) {
      ?><font color="red"><b>ERROR: Could not find ADSL account (<?php print $id?>)! Please contact the site administrator.</b></font><?php
      adsl::listall();
      exit;
    }
    
    ?>
    <script type="text/javascript">
    <!--
      var keylist="abcdefghijklmnopqrstuvwxyz1234567890"
      var temp=''
      
      function GeneratePassword(object){
        temp=''
	for (i=0;i<8;i++)
	temp+=keylist.charAt(Math.floor(Math.random()*keylist.length))
	object.value=temp
      }
    -->
    </script> 
    
    <form name="adsl" action="index.php?module=adsl&action=update&userid=<?php print $result[0]["id"] ?>" method="POST">
      <table width="860">
	<tbody>
	  <tr>
	    <!-- Account Details -->
	    <td valign="top" width="70%">
	      <table class="box">
		<caption>Account Details</caption>
		<tbody>
		  <tr>
		    <td>
     		      <label>Username:&nbsp</label>
		    </td>
		    <td>
		      <input type="text" value="<?php
                        print $result[0]["username"];
                        ?>" maxlength="24" readonly>&nbsp@
                      <input type="text" value="<?php
                        if($result[0]["realm"]=='dsl512msp.co.za') {
                          print "dsl512msp.co.za";
                        }else if($result[0]["realm"]=='lmsp.co.za') {
				print "lmsp.co.za";
			} else if($result[0]["realm"]=='dslmsp.co.za') {
				print "dslmsp.co.za";
			} else if($result[0]["realm"]=='msp.co.za') {
				print "msp.co.za";
                        }else{
                          print "umsp.co.za";
                        }
                      ?>" readonly>
		    </td>
		  </tr>
		  <tr>
		    <td>
     		      <label>Password:&nbsp</label>
		    </td>
		    <td>
		      <input type="text" name="ADSL_Password" value="<?php
                      print $result[0]["password"]
                      ?>" maxlength="16" <?php
                      if($result[0]["canceled"]!='0000-00-00') {
                        print readonly;
                      }
                      ?>>&nbsp&nbsp&nbsp&nbsp
		      <input type="button" <?php
                      if($result[0]["canceled"]!='0000-00-00') {
                        print disabled;
                      }?> value="Generate" onClick="GeneratePassword(document.adsl.ADSL_Password)">
		    </td>
		  </tr>
		  <tr>
		    <td>
     		      <label>Contract Cap:&nbsp</label>
		    </td>
		    <td>
		      <input type="text" name="ADSL_Contract" value="<?php
                      print $result[0]["cap_size"]
                      ?>" maxlength="2" <?php
                      if($result[0]["canceled"]!='0000-00-00') {
                        print readonly;
                      }
                      ?>>
		      Gb
		    </td>
		  </tr>
		  <tr>
		    <td>
     		      <label>Ad-Hoc Cap:&nbsp</label>
		    </td>	    
		    <td>
		      <input type="text" name="ADSL_Adhoc" value="0"<?php
                      if($result[0]["canceled"]!='0000-00-00') {
                        print readonly  ;
                      }
                      ?> maxlength="2">
		      Gb&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp[ Total:&nbsp<font color="green"><?php
                        if($result[0]["total"]!='') {
                          print $result[0]["total"];
                        }else{
                          print 0;
                        }
                      ?></font> Gb ]
		    </td>
		  </tr>
		  <tr>
		    <td>
     		      <label>Effective Cap:&nbsp</label>
		    </td>	    
		    <td>
		      <input type="text" value="<?php
                        print $result[0]["total"]+$result[0]["cap_size"];
                      ?>" maxlength="4" readonly = "readonly" disabled="disabled">
		      Gb
		    </td>
		  </tr> 
		  
		  
		  <!-- added Account Type (sandiswa 28Dec2011) -->
		    <tr>
		    <td>
     		      <label>Account Type:&nbsp</label>
		    </td>	    
		    <td>
		      <input type="text" value="<?php
                       // print $result[0]["total"]+$result[0]["cap_size"];
                          print strtolower($result[0]["account"]);
                        
                      ?>" maxlength="10"  disabled="disabled" readonly="readonly"  />
		    
		    </td>
		  </tr> 
    
		  <tr>
		    <td>
     		      <label>Session Time:&nbsp</label>
		    </td>
		    <td>
		      <select name="ADSL_Session" <?php
                      if($result[0]["canceled"]!='0000-00-00') {
                        print disabled;
                      }
                      ?>>
			<option value="1"<?php
                    //    if($result[0]["session_time"]=='1') {
                    //      print " selected";
                    //    }
                        ?>>1</option>
			<option value="2"<?php
                        if(($result[0]["session_time"]<='2')&&($result[0]["session_time"]>'1')) {
                          print " selected";
                        }
                        ?>>2</option>
			<option value="4"<?php
                        if(($result[0]["session_time"]<='4')&&($result[0]["session_time"]>'2')) {
                          print " selected";
                        }
                        ?>>4</option>
			<option value="8"<?php
                        if(($result[0]["session_time"]<='8')&&($result[0]["session_time"]>'4')) {
                          print " selected";
                        }
                        ?>>8</option>
			<option value="12"<?php
                        if(($result[0]["session_time"]<='12')&&($result[0]["session_time"]>'8')) {
                          print " selected";
                        }
                        ?>>12</option>
			<option value="24"<?php
                        if(($result[0]["session_time"]<='24')&&($result[0]["session_time"]>'12')) {
                          print " selected";
                        }
                        ?>>24</option>
			
		      </select>
		      Hours
		    </td>
		  </tr>
		  <tr>
		    <td>
     		      <label>Throttle Size:&nbsp</label>
		    </td>
		    <td>
		      <select name="ADSL_Throttle" <?php
                      if($result[0]["canceled"]!='0000-00-00') {
                        print disabled;
                      }
                      ?>>
			<option value="0"<?php
                        if($result[0]["throttle_size"]=='0') {
                          print " selected";
                        }
                        ?>>0</option>
			<option value="0.05"<?php
                        if(($result[0]["throttle_size"]<='0.05')&&($result[0]["throttle_size"]>'0')) {
                          print " selected";
                        }
                        ?>>0.05</option>
                        <option value="0.1"<?php
                        if(($result[0]["throttle_size"]<='0.1')&&($result[0]["throttle_size"]>'0.05')) {
                          print " selected";
                        }
                        ?>>0.1</option>
			<option value="0.25"<?php
                        if(($result[0]["throttle_size"]<='0.25')&&($result[0]["throttle_size"]>'0.1')) {
                          print " selected";
                        }
                        ?>>0.25</option>
			<option value="0.5"<?php
                        if(($result[0]["throttle_size"]<='0.5')&&($result[0]["throttle_size"]>'0.25')) {
                          print " selected";
                        }
                        ?>>0.5</option>
			<option value="0.75"<?php
                        if(($result[0]["throttle_size"]<='0.75')&&($result[0]["throttle_size"]>'0.5')) {
                          print " selected";
                        }
                        ?>>0.75</option>
                        <?php
                          if($result[0]["cap_size"]>'1') {
                            print "<option value=\"1\"";
                            if(($result[0]["throttle_size"]<='1')&&($result[0]["throttle_size"]>'0.75')) {
                              print " selected";
                            }
                            print ">1</option>\n";
                          }
                          if($result[0]["cap_size"]>'2') {
                            print "<option value=\"2\"";
                            if(($result[0]["throttle_size"]<='2')&&($result[0]["throttle_size"]>'1')) {
                              print " selected";
                            }
                            print ">2</option>\n";
                          }
                          if($result[0]["cap_size"]>'5') {
                            print "<option value=\"5\"";
                            if(($result[0]["throttle_size"]<='5')&&($result[0]["throttle_size"]>'2')) {
                              print " selected";
                            }
                            print ">5</option>\n";
                          }
                          if($result[0]["cap_size"]>'10') {
                            print "<option value=\"10\"";
                            if(($result[0]["throttle_size"]<='10')&&($result[0]["throttle_size"]>'5')) {
                              print " selected";
                            }
                            print ">10</option>\n";
                          }
                        ?>
		      </select>
		      Gb
		    </td>
		  </tr>
		  <tr>
		    <td>
     		      <label>Status:&nbsp</label>
		    </td>
		    <td>
		      <label><?php
                      if($result[0]["canceled"]!='0000-00-00') {
                        ?><font color="red"><b>CANCELLED</b></font><?php;
                      }else{
                        adsl::adsl_status($result[0]["username"]."@".$result[0]["realm"]);
                      }
		      ?></label>
		      (<a href="index.php?module=adsl&action=month_stats&adsl_username=<?php
		      	print $result[0]["username"]."@".$result[0]["realm"];
		      ?>&year=<?php
		        print date(Y);
		      ?>&month=<?php
		        print date(n);
		      ?>">STATS</a> | <a href="http://<?php
			print $result[0]["username"]."@".$result[0]["realm"].":".$result[0]["password"];
                      ?>@userstats.adsl.saix.net"?>SAIX</a>)
		    </td>	  
		  </tr>
		</tbody>
	      </table>
	    </td>
	    <!-- Contact Details -->
	     <td valign="top" width="250">
	      <table class="box" width="95%">
		<caption>Contact Details</caption>
		<tbody>
		  <tr>
		    <td>
     		      <label>Name: </label>
		    </td>
		    <td>
		      <input type="text" name="ADSL_Name" value="<?php
                        print $result[0]["name"]
                      ?>" maxlength="64" <?php
                      if($result[0]["canceled"]!='0000-00-00') {
                        print readonly;
                      }?>>
		    </td>
		  </tr>
		  <tr>
		    <td>
     		      <label>Tel:&nbsp</label>
		    </td>
		    <td>
		      <input type="text" name="ADSL_Tel" value="<?php
                        print $result[0]["tel"]
                      ?>" maxlength="10" <?php
                      if($result[0]["canceled"]!='0000-00-00') {
                        print readonly;
                      }?>>
		    </td>
		  </tr>
		  <tr>
		    <td>
     		      <label>Cell:&nbsp</label>
		    </td>
		    <td>
		      <input type="text" name="ADSL_Cell" value="<?php
                        print $result[0]["cell"]
                      ?>" maxlength="10" <?php
                      if($result[0]["canceled"]!='0000-00-00') {
                        print readonly;
                      }?>>
		    </td>
		  </tr>
		  <tr>
		    <td>
     		      <label>Fax:&nbsp</label>
		    </td>
		    <td>
		      <input type="text" name="ADSL_Fax" value="<?php
                        print $result[0]["fax"]
                      ?>" maxlength="10" <?php
                      if($result[0]["canceled"]!='0000-00-00') {
                        print readonly;
                      }?>>
		    </td>
		  </tr>
		  <tr>
		    <td>
     		      <label>Email:&nbsp</label>
		    </td>
		    <td>
		      <input type="text" name="ADSL_Email" value="<?php
                        print $result[0]["email"]
                      ?>" maxlength="256" <?php
                      if($result[0]["canceled"]!='0000-00-00') {
                        print readonly;
                      }?>>
		    </td>
		  </tr>	
		</tbody>
	      </table>
	    </td>
	  </tr>
          <tr>
            <td>
              <input type="hidden" value="<?php
                print $result[0]["id"]
              ?>" name=id>
              <table width="100%">
                <tbody>
                  <tr>
                    <td>
                      <input type="submit" value="update" name="action" <?php
                        if($result[0]["canceled"]!='0000-00-00') {
                          print disabled;
                      }?>>
                    <!--</td>
                    <td align="right">-->
                      <input type="submit" value="delete" name="action" <?php
                        if($result[0]["canceled"]!='0000-00-00') {
                        print disabled;
                      }?>>
                    </td>
                  </tr>
                </tbody>
              </table>
            </td>
          </tr>
	  <tr>
   	    <!-- Account History -->
            <?php
              $result = sql_run(
              "SELECT
                `adsl_adhoc`.`time`,
                `adsl_adhoc`.`value`,
                `adsl_adhoc`.`user`
              FROM
                `adsl_adhoc`
              WHERE
                `adsl_adhoc`.`time` LIKE DATE_FORMAT(NOW(),'%Y-%m-%%')
              AND
                `adsl_adhoc`.`id` = '$id'                
              ORDER BY
                `adsl_adhoc`.`time` ASC");
            ?>
	    <td colspan=2>
	      <table class="box" width="100%">
		<caption>Account History</caption>
		<tbody>
		  <tr>
		    <td align="center">
		      <font size=1><b>Date</b></font>
		    </td>
		    <td align="center">
		      <font size=1><b>Modified By</b></font>
		    </td>
		    <td align="center">
		      <font size=1><b>Description</b></font>
		    </td>
		  </tr>
                  <?php
                  	if(is_array($result)){
                    foreach ($result as $i) {
                      ?>
                      <tr>
                        <td align="center">
                          <font size=0><?php print $i["time"] ?></font>
                        </td>
                        <td align="center">
                          <font size=0><?php print $i["user"] ?></font>
                        </td>
                        <td align="center">
                          <font size=0>Added <?php print $i["value"] ?> Ad-Hoc Gb</font>
                        </td>                        
                      </tr>
                      <?php
                    }
                  	}
                  ?>
		</tbody>
	      </table>
	    </td>
	  </tr>
 	</tbody>
      </table>
    </form>
    <?php
  }

  function delete(
    $confirm,
    $id) {
    
    if(!acl::has_access($_SESSION['id'],"adsl","add")) return 1;
    
    if(!isset($confirm)) {/*HACK: confirmation check*/
      ?>
      <form name="adsl" action="index.php?module=adsl&action=delete&userid=<?php print $id ?>&confirm=no" method="POST">
      <table align="center">
        <tbody>
          <tr>
            <td>
              <label>Are you sure you want to <font color="red"><b>DELETE</b></font> this account?</label>
            </td>
          </tr>
          <tr>
            <td>
             <input type="hidden" value="<?php
                print $id;
              ?>" name=id>
            </td>
          </tr>
          <tr>
            <td align="center">
              <input type="submit" value="yes" name="confirm">
              <input type="submit" value="no" name="confirm">
            </td>
          </tr>
        </tbody>
      </table>
      <?php
      exit;
    }else if($confirm != 'yes') {
      return FALSE;
    }
 
    $result = sql_run(
    "UPDATE
      `msi_adsl`
    SET
      `msi_adsl`.`canceled`=DATE_FORMAT(NOW(),'%Y-%m-00')
    WHERE
      `msi_adsl`.`id`='$id'
    ");
    
    if(!isset($result)) {
        $result = sql_run(
        "SELECT
          `msi_adsl`.`username`,
          `msi_adsl`.`realm`
        FROM
          `msi_adsl`
        WHERE
          `msi_adsl`.`id`='$id'
        ");
        
        ?><b>Cancelled <?php print $result[0][username]."@".$result[0][realm]." ($id)"?></b>
        <br>The changes will become active when the database is rebuilt on the hour.<?php
    } else {
      ?><b>Update Failed! Please contact the site administrator.</b><?php;
      adsl::listall();
      return FALSE;
    }
      
    return TRUE;
  }
  
  function add_form(){
  if(!acl::has_access($_SESSION['id'],"adsl","add")) return 1;
  
    $results = sql_run("
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
     ?><b>SQL Error! Please contact the site administrator.</b><?php;
      adsl::listall();
      return FALSE;      
    }
    
    ?>
    <script type="text/javascript">
    <!--
      var keylist="abcdefghijklmnopqrstuvwxyz1234567890"
      var temp=''
      
      function GeneratePassword(object){
        temp=''
	for (i=0;i<8;i++)
	temp+=keylist.charAt(Math.floor(Math.random()*keylist.length))
	object.value=temp
      }
      
      function SelectAcctType(){
      	removeAllOptions(document.adsl.AcctType);
      
      	if(document.adsl.ADSL_Provider.value == 'SAIX') {
      		addOption(document.adsl.ADSL_AcctType,"", "Please Select");
      		addOption(document.adsl.ADSL_AcctType,"LOCAL", "LOCAL");
      		addOption(document.adsl.ADSL_AcctType,"SHAPED", "SHAPED");
		addOption(document.adsl.ADSL_AcctType,"UNSHAPED", "UNSHAPED");
		addOption(document.adsl.ADSL_AcctType,"LITE", "LITE");
      	}
      
      	if(document.adsl.ADSL_Provider.value == 'CYBERSMART') {
      		addOption(document.adsl.ADSL_AcctType,"", "Please Select");
      		addOption(document.adsl.ADSL_AcctType,"UNSHAPED", "UNSHAPED");
      	}
      }
      
      function SelectRealms() {
      	removeAllOptions(document.adsl.ADSL_Realm);
      
      	if(document.adsl.ADSL_Provider.value == 'SAIX' && document.adsl.AcctType.value == 'SHAPED'){
      		addOption(document.adsl.ADSL_Realm,"dsl512msp.co.za", "dsl512msp.co.za");
      		addOption(document.adsl.ADSL_Realm,"msp.co.za", "msp.co.za");
      	}
      	if(document.adsl.ADSL_Provider.value == 'SAIX' && document.adsl.AcctType.value == 'UNSHAPED'){
      		addOption(document.adsl.ADSL_Realm,"umsp.co.za", "umsp.co.za");
      		addOption(document.adsl.ADSL_Realm,"msp.co.za", "msp.co.za");
      	}
      	if(document.adsl.ADSL_Provider.value == 'SAIX' && document.adsl.AcctType.value == 'LOCAL'){
      		addOption(document.adsl.ADSL_Realm,"msp.co.za", "msp.co.za");
	}
	if(document.adsl.ADSL_Provider.value == 'SAIX' && document.adsl.AcctType.value == 'LITE'){
		addOption(document.adsl.ADSL_Realm,"msp.co.za", "msp.co.za");
	}
      	if(document.adsl.ADSL_Provider.value == 'CYBERSMART'){
      		addOption(document.adsl.ADSL_Realm,"dslmsp.co.za", "dslmsp.co.za");
      	}
      }
      
      function removeAllOptions(selectbox)
      {
      	var i;
      	for(i=selectbox.options.length-1;i>=0;i--)
      	{
      		//selectbox.options.remove(i);
      		selectbox.remove(i);
      	}
      }
      
      
      function addOption(selectbox, value, text )
      {
      	var optn = document.createElement("OPTION");
      	optn.text = text;
      	optn.value = value;
      
      	selectbox.options.add(optn);
}
      
    -->
    </script>    
    <form name="adsl" action="index.php?module=adsl&action=add" method="POST">
      <table>
	<tbody>
	  <tr>
	    <!-- Account Details -->
	    <td valign="top" width="70%">
	      <table class="box">
		<caption>Account Details</caption>
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
		  			<tr><td><label>Provider and Account Type:</label></td><td><select name="ADSL_Provider" onChange="SelectAcctType();">
		  					<option value="">Please select</option>
		  					<option value="SAIX">SAIX</option>
		  					<option value="CYBERSMART">CYBERSMART</option>
		  				</select>
		  
		  					<select id="AcctType" name="ADSL_AcctType" onChange="SelectRealms();">
		  				</select>
			</td></tr> 
		  
		    <td>
     		      <label>Username:&nbsp</label>
		    </td>
		    <td>
		      <input type="text" name="ADSL_Username" maxlength="24">&nbsp@
                      <select name="ADSL_Realm">
                        <option value="dsl512msp.co.za" selected>
                          dsl512msp.co.za
                        </option>
                        <option value="lmsp.co.za">
                          lmsp.co.za
                        </option>
                        <option value="umsp.co.za">
                          umsp.co.za
                        </option>
                      </select>
		    </td>
		  </tr>
		  <tr>
		    <td>
     		      <label>Password:&nbsp</label>
		    </td>
		    <td>
		      <input type="text" name="ADSL_Password" maxlength="16" >&nbsp&nbsp&nbsp&nbsp
		      <input type="button" value="Generate" onClick="GeneratePassword(document.adsl.ADSL_Password)">
		    </td>
		  </tr>
		  <tr>
		    <td>
     		      <label>Contract Cap:&nbsp</label>
		    </td>
		    <td>
		      <input type="text" name="ADSL_Contract" maxlength="2">
		      Gb
		    </td>
		  </tr>
		  <tr>
		    <td>
     		      <label>Session Time:&nbsp</label>
		    </td>
		    <td>
		      <select name="ADSL_Session">
			<option value="1">1</option>
			<option value="2">2</option>
			<option value="4">4</option>
			<option value="8">8</option>
			<option value="12">12</option>
			<option value="24" selected>24</option>
		      </select>
		      Hours
		    </td>
		  </tr>
		</tbody>
	      </table>
	    </td>
	    <!-- Contact Details -->
	     <td valign="top" width="250">
	      <table class="box" width="95%">
		<caption>Contact Details</caption>
		<tbody>
		  <tr>
		    <td>
     		      <label>Name: </label>
		    </td>
		    <td>
		      <input type="text" name="ADSL_Name" maxlength="64">
		    </td>
		  </tr>
		  <tr>
		    <td>
     		      <label>Tel:&nbsp</label>
		    </td>
		    <td>
		      <input type="text" name="ADSL_Tel" maxlength="10">
		    </td>
		  </tr>
		  <tr>
		    <td>
     		      <label>Cell:&nbsp</label>
		    </td>
		    <td>
		      <input type="text" name="ADSL_Cell" maxlength="10">
		    </td>
		  </tr>
		  <tr>
		    <td>
     		      <label>Fax:&nbsp</label>
		    </td>
		    <td>
		      <input type="text" name="ADSL_Fax" maxlength="10">
		    </td>
		  </tr>
		  <tr>
		    <td>
     		      <label>Email:&nbsp</label>
		    </td>
		    <td>
		      <input type="text" name="ADSL_Email" maxlength="256">
		    </td>
		  </tr>	
		</tbody>
	      </table>
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

  function add(
    $Company,
    $Username,
    $Realm,
    $Password,
    $Contract,
    $Session,
    $Name,
    $Tel,
    $Cell,
    $Fax,
    $Email,
    $AcctType,
    $Provider
    
    ){

    	if(!acl::has_access($_SESSION['id'],"adsl","add")) return 1;
    
    if(!preg_match('/^[0-9]+$/',$Company)) {
      ?><font color="red"><b>ERROR: Company is not valid</b></font><?php;
      adsl::listall();
      exit;
    }
    if(!preg_match('/^[a-z0-9]+$/',$Username)) {
      ?><font color="red"><b>ERROR: Username is not valid, expected 'a' to 'z', '0' to '9'</b></font><?php
      return FALSE;
    }
    if(!preg_match('/^[a-z0-9\.]+$/',$Realm)) {
      ?><font color="red"><b>ERROR: Realm is not valid, expected 'a' to 'z', '0' to '9'</b></font><?php
      return FALSE;
    }    
    if(!preg_match('/^[a-z0-9]+$/',$Password)) {
      ?><font color="red"><b>ERROR: Password is not valid, expected 'a' to 'z', '0' to '9'</b></font><?php
      return FALSE;
    }
    if(!preg_match('/^[0-9]{1,2}$/',$Contract)) {
      ?><font color="red"><b>ERROR: Contract Cap is not valid, expected a value between 0 and 99</b></font><?php
      return FALSE;
    }
    if(!$Contract) {
      ?><font color="red"><b>ERROR: Contract Cap may not be 0</b></font><?php
      return FALSE;
    }
//    if(!preg_match('/^[0-9]?[0-9]?$/',$Session)) {
//      ?><!--<font color="red"><b>ERROR: Session Time is not valid, expected a value between 1 and 24</b></font>--><?php
//      return FALSE;      
//    }
    if(!preg_match('/^[a-zA-Z\.\ \-]*$/',$Name)) {
      ?><font color="red"><b>ERROR: Contact Name is not valid, expected 'A' to 'Z', 'a' to 'z' with spaces and a optional '.'</b></font><?php
      return FALSE;
    }    
    if(!preg_match('/^([0-9]{10})?$/',$Tel)) {
      ?><font color="red"><b>ERROR: Contact Tellephone number is not valid, expected '0' to '9' 10 digits or nothing</b></font><?php
      return FALSE;
    }
    if(!preg_match('/^([0-9]{10})?$/',$Cell)) {
      ?><font color="red"><b>ERROR: Contact Cellphone number is not valid, expected '0' to '9' for 10 digits or nothing</b></font><?php
      return FALSE;      
    }
    if(!preg_match('/^([0-9]{10})?$/',$Fax)) {
      ?><font color="red"><b>ERROR: Contact Fax number is not valid, expected '0' to '9' for 10 digits or nothing</b></font><?php
      return FALSE;
    }
    if(!preg_match('/^([a-z0-9\-_\+]+@[a-z0-9\-_]+(\.[a-z0-9\-_]+)+)?$/',$Email)) {
      ?><font color="red"><b>ERROR: Contact Email is not valid, format expected is 'user@domain.ltd'</b></font><?php
      return FALSE;
    }

    $result=sql_run("
      SELECT
        *
      FROM
        `msi_adsl`
      WHERE
        `msi_adsl`.`username`='$Username'
      AND
        `msi_adsl`.`realm`='$Realm'
    ");

    if(isset($result)) {
      ?><br><font color="red"><b>ERROR: Username already in use.</b></font><?php
      return FALSE;      
    }

    $result=sql_run("
      INSERT INTO
        `msi_adsl`
      SET
        `msi_adsl`.`client`='$Company',
        `msi_adsl`.`username`='$Username',
        `msi_adsl`.`realm`='$Realm',
        `msi_adsl`.`cap_size`='$Contract',
        `msi_adsl`.`password`='$Password',
        `msi_adsl`.`session_time`='$Session',
        `msi_adsl`.`throttle_size`='0.1',
        `msi_adsl`.`name`='$Name',
        `msi_adsl`.`tel`='$Tel',
        `msi_adsl`.`cell`='$Cell',
        `msi_adsl`.`fax`='$Fax',
        `msi_adsl`.`email`='$Email',
        `msi_adsl`.`account` = '$Provider $AcctType'
    ");
    /*if ($Realm=='msp.co.za' || $Realm=='dslmsp.co.za') {$result2=sql2_run("
    	INSERT INTO
    		`radcheck`
    	VALUES
    		('', '$Username@$Realm', 'Crypt-Password',    ':=', ENCRYPT('$Password')),
    		('', '$Username@$Realm', 'DSL-$AcctType-CAP', ':=', '$Contract'         )
    	");
    	$result3=sql2_run("
    	INSERT INTO
    		`radusergroup`
    	VALUES
    		('$Username@$Realm', 'DSL', '1');
    	");
    	
    }*/
    if(!isset($result)) {
      ?><br><font color="green"><b>Added New ADSL Account '<?php print $Username."@".$Realm ?>'</b></font><?php
    }else{
      ?><br><font color="red"><b>ERROR: Creating New ADSL Account! Please contact the site administrator.</b></font><?php
      return FALSE;
    }
    ?><br>The changes will become active when the database is rebuilt on the hour.<br><?php
    
    return TRUE; 
    
  }

  function adsl_status($Username){
    if(!acl::has_access($_SESSION['id'],"adsl","read")) return 1;
    
    
//(2012-03Febr Sandiswa- changed the query to use acctStartTime to calulate bandwidth used for current month).
    
    
$result=sql_run("
      SELECT
        IFNULL(sum.`total`,0) AS used,
        (IFNULL(adhoc.`adhoc`,0)+`msi_adsl`.`cap_size`)*(1024*1024*1024) AS cap,
        `msi_adsl`.`throttle_size`*(1024*1024*1024) AS throttle
      FROM
        `msi_adsl`,
        (
          SELECT
            SUM(clean.`AcctInputOctets`)+SUM(clean.`AcctOutputOctets`) AS total
          FROM
          (
            SELECT 
              MAX(`radacct`.`AcctInputOctets`) AS AcctInputOctets,
              MAX(`radacct`.`AcctOutputOctets`) AS AcctOutputOctets
            FROM 
              `radacct`
            WHERE
              `radacct`.`UserName`='$Username'
            AND
              UNIX_TIMESTAMP(`radacct`.`AcctStartTime`) >=
              UNIX_TIMESTAMP(DATE_FORMAT(NOW(),'%Y-%m-01 00:00:00'))
            GROUP BY
              `radacct`.`AcctUniqueId`
          ) AS clean
        ) AS sum,
        (
          SELECT
            SUM(`adsl_adhoc`.`value`) AS adhoc
          FROM
            `adsl_adhoc`,
            `msi_adsl`
          WHERE
            CONCAT(`msi_adsl`.`username`,'@',`msi_adsl`.`realm`)='$Username'
          AND
            `adsl_adhoc`.`id`=`msi_adsl`.`id`
          AND
            `adsl_adhoc`.`time` LIKE DATE_FORMAT(NOW(),'%Y-%m-%%')
        ) AS adhoc
      WHERE
        CONCAT(`msi_adsl`.`username`,'@',`msi_adsl`.`realm`)='$Username'
    ");
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    

    if($result[0]["used"]<=$result[0]["cap"]) {
      if($result[0]["used"]<($result[0]["cap"]-$result[0]["throttle"])) {
        ?><font color="green" ><b>OK</b></font><?php
      }else{
        ?><font color="#CD7F32"><b>THROTTLED</b></font><?php
      }
    }else{
      ?><font color="red"><b>CAPPED</b></font><?php      
    }
    ?>, <?php
    print number_format($result[0]["used"]/(1024*1024*1024),2);
    ?> Gb Used<?php
  }
    
  function month_stats($adsl_username,$month,$year){
  if(!acl::has_access($_SESSION['id'],"adsl","read")) return 1;
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
        print "<p></p>";
        print '<table><tr><td colspan="3" align="center">';
        print "<a href=?module=adsl&action=month_stats&adsl_username=$adsl_username&year=$prev_year&month=$prev_month>". date(F, mktime(0, 0, 0, $prev_month, 1, $prev_year)) ." $prev_year << Previous</a>&nbsp;&nbsp; ";
	    print " <b><a href=?module=adsl&action=month_stats&adsl_username=$adsl_username&year=$year&month=$month>". date(F, mktime(0, 0, 0, $month, 1, $year)) ." &nbsp;&nbsp;$year&nbsp;&nbsp;&nbsp;</a></b>";
	    print " <a href=?module=adsl&action=month_stats&adsl_username=$adsl_username&year=$next_year&month=$next_month>&nbsp;&nbsp;Next >> ". date(F, mktime(0, 0, 0, $next_month, 1, $next_year)) ." &nbsp;&nbsp;&nbsp;$next_year</a><p>";
	        print '</td></tr>
			    </table>';

     print "<span class=\"head\">ADSL Usage in <i>$month/$year</i> for $adsl_username <br> </span>
   
            <table  class=\"client\" width = 450 >
	    <tr class=\"head\" align ='center'><td align ='center'>Uploaded</td><td >Downloaded</td><td align ='center'>Total</td></tr>";
     
     $record = sql_run("SELECT 
          SUM(acctinputoctets),
          SUM(acctoutputoctets) ,
          SUM(acctinputoctets) + SUM(acctoutputoctets)
          FROM `radacct`
          WHERE MONTH (acctstarttime) = $month
	  AND  YEAR  (acctstarttime)  = $year
	  AND  username = \"$adsl_username\"");
           
   $uptotal = number_format($record[0][0],0,"."," ")." (". adsl::format_bytes($record[0][0]) .")";
    $downtotal = number_format($record[0][1],0,"."," ") ." (". adsl::format_bytes($record[0][1]) .")";
    $comtotal = number_format($record[0][2],0,"."," ") ." (". adsl::format_bytes($record[0][2]) .")";
     
    
     print "<tr align = 'right'><td class=\"client\">$uptotal</td><td class=\"client\">$downtotal</td><td class=\"client\">$comtotal</td></tr>
           </table><br>
          ";

     print "<table width = 415 class=\"client\">
	    <tr class=\"head\" ><td align ='center'  width = '50'>Day</td><td align ='center'  width = '100'>Uploaded</td><td align ='center' width = '100'>Downloaded</td><td align ='center'  width = '85'>Total (b)</td><td align ='center'  width = '70'>Total (Gb)</td></tr>";
	for ($i = 1; $i < 32; $i++){
	 
	 $result = sql_run("SELECT SUM( acctinputoctets ) + 
	            SUM( acctoutputoctets ) AS total
	             FROM `radacct`
                     WHERE MONTH(acctstarttime) = $month
                     and DAYOFMONTH(acctstarttime) = $i
		     and YEAR(acctstarttime) = $year
		     AND username = \"$adsl_username\"
                ");
         $total = $result[0][0];
         
           //removing the formating of GB, MB, KB and B on bandwidth used.(Sandiswa)
	 if(isset($total)) $total = number_format($total,0,"."," ");//." (". adsl::format_bytes($total) .")" ;
	 
	 
	 
	 if ($total == 0){
	  $totalgigs = " ";}
	  else{
	 $totalgigs = number_format($result[0][0] / 1024 /1024 /1024,2,"."," ") ." Gb";}
	 #$result = sql_run("SELECT format( SUM( acctoutputoctets ) / 1024 / 1024 ,2) AS total
	 $result = sql_run("SELECT SUM( acctoutputoctets ) AS total
	             FROM `radacct`
                     WHERE MONTH(acctstarttime) = $month
                     and DAYOFMONTH(acctstarttime) = $i
		     and YEAR(acctstarttime) = $year
		     AND username = \"$adsl_username\"
                ");
         $downloaded = $result[0][0];
         
         //removing the formating of GB, MB, KB and B on bandwidth used.(Sandiswa)
	 if(isset($downloaded)) $downloaded = number_format($downloaded,0,"."," ");//." (". adsl::format_bytes($downloaded) .")" ;
	 #$result = sql_run("SELECT format( SUM( acctinputoctets ) / 1024 / 1024,2) AS total
	 $result = sql_run("SELECT Sum(acctinputoctets) AS total
	             FROM `radacct`
                     WHERE MONTH(acctstarttime) = $month
                     and DAYOFMONTH(acctstarttime) = $i
		     and YEAR(acctstarttime) = $year
		     AND username = \"$adsl_username\"
                ");
         $uploaded = $result[0][0];
         
           //removing the formating of GB, MB, KB and B on bandwidth used.(Sandiswa)
	 if(isset($uploaded)) $uploaded = number_format($uploaded,0,"."," ");//." (". adsl::format_bytes($uploaded) .")" ;
	 
	print "<tr >
		<td class=\"client\" align = 'center'><a href=\"index.php?module=adsl&action=day_stats&adsl_username=$adsl_username&year=$year&month=$month&day=$i\" STYLE= 'text-decoration: none'>$i </a></td>
		<td class=\"client\" align = 'right'>$uploaded </td>
		<td class=\"client\" align = 'right'>$downloaded </td>";
	
	    
	   
		print "<td class=\"client\" align = 'right'><a href=\"index.php?module=adsl&action=day_stats&adsl_username=$adsl_username&year=$year&month=$month&day=$i\" STYLE= 'text-decoration: none'>$total </a></td>";
	
	  print "<td class=\"client\" align = 'right'><a href=\"index.php?module=adsl&action=day_stats&adsl_username=$adsl_username&year=$year&month=$month&day=$i\" STYLE= 'text-decoration: none'>$totalgigs </a></td>";
	    
	}
  	print '</table>';
  }

  
  //2011-Dec-21 Added AcctStopTime(Sandiswa)
 // Date was displayed as mm/yy/dd : changed to yy/mm/dd
  function day_stats($adsl_username,$month,$year,$day){
  if(!acl::has_access($_SESSION['id'],"adsl","read")) return 1;
    $results = sql_run("SELECT  AcctSessionId,  AcctUniqueId,  AcctStartTime, AcctInputOctets,  AcctOutputOctets, FramedIPAddress,  AcctStopTime
	             FROM `radacct`
                     WHERE MONTH(acctstarttime) = $month
                     and DAYOFMONTH(acctstarttime) = $day
		     and YEAR(acctstarttime) = $year
		     AND username = \"$adsl_username\"
                ");
    //Remove AcctSessionId,AcctUniqueId (sandiswa 28Dec2011)
    // <td class=\"client\" align=\"center\">$result[0]</td>
    // <td class=\"client\" align=\"center\">$result[1]</td>
	// <tr class=\"head\" align = 'center'><td>AcctSessionId</td><td>AcctUniqueId</td><td>AcctStartTime</td><td>AcctStopTime</td><td>AcctInputOctets</td><td>AcctOutputOctets</td><td>FramedIPAddress</td></tr>";
    
    print " <p></p>
    <span class=\"head\">$adsl_username<p></p> ADSL Usage for <i>$year/$month/$day</i></span><p></p>
            <table  class=\"client\" width =800>
	    <tr class=\"head\" align = 'center'><td>Start-Time</td><td>Stop-Time</td><td>Input-Octets</td><td>Output-Octets</td><td>IP-Address</td></tr>";
    
    foreach ($results as $result) {
    $input = number_format($result[3],0,".","  ");
    $output = number_format($result[4],0,".","  ");
      print "<tr>
   
             <td class=\"client\" align=\"center\">$result[2]</td>
             <td class=\"client\" align=\"center\">$result[6]</td>
             <td class=\"client\" align=\"right\">$input</td>
             <td class=\"client\" align=\"right\">$output</td>
             <td class=\"client\" align=\"right\">$result[5]</td>
	     </tr>";
      
         }
    
print '</table>';

  }
  
function adsl_summary($adsl_account,$month,$year){
  if(!acl::has_access($_SESSION['id'],"adsl","read")) return 1;
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
        print "<p></p>";
        print '<table><tr><td colspan="3" align="center">';
         print "<a href=?module=adsl&action=adsl_summary&adsl_account=$adsl_account&year=$prev_year&month=$prev_month> << Previous</a> &nbsp;&nbsp; ";
	     print " <b><a href=?module=adsl&action=adsl_summary&adsl_account=$adsl_account&year=$year&month=$month>". date(F, mktime(0, 0, 0, $month, 1, $year)) ." $year</a></b>&nbsp;&nbsp;";
	    print " <a href=?module=adsl&action=adsl_summary&adsl_account=$adsl_account&year=$next_year&month=$next_month>Next >></a><p>";
	        print '</td></tr>
			    </table>';

     print "<span class=\"head\">ADSL Account Summary <i> &nbsp;&nbsp;&nbsp;$month/$year</i> <br> </span>";
   
       print "   <table  class=\"client\" width = 400>
	   <tr class=\"head\"><td>Account Type</td><td>&nbsp;&nbsp;&nbspRealm&nbsp;&nbsp;&nbsp</td><td>&nbsp;&nbsp;&nbsp;Bandwidth Used</td></tr>";
       

         
       $cyberunshaped = sql_run("SELECT  Distinct(realm)
	                       FROM `radacct`
                            WHERE radacct.realm = 'dsl512msp.co.za' 
		                    OR radacct.realm = 'dslmsp.co.za' 
		                    ");
       
         foreach ($cyberunshaped as $result) {
             $realm = $result['realm'];
                if($realm =='dslmsp.co.za' ){
                 $account = 'CYBERSMART UNSHAPED';}
                 elseif($realm =='dsl512msp.co.za' ) { $account = 'SAIX SHAPED';}
        
           
             
       
       
       $bandwidth = sql_run("SELECT SUM( radacct.acctinputoctets ) +  SUM( radacct.acctoutputoctets ) AS total
	                       FROM `radacct`
                            WHERE MONTH(radacct.acctstarttime) = $month
		                    AND YEAR(radacct.acctstarttime) = $year
		                    AND radacct.realm = \"$realm\" 
            ");
       
     
        foreach($bandwidth as $bandwidth) {
          $total = $bandwidth['total'];

           $total2 =  adsl::format_bytes($total);
           
           
         

	 
	print "<tr >";
	//	 <td class=\"client\" align = 'center'><a href=\"index.php?module=adsl&action=day_stats&adsl_username=$adsl_username&year=$year&month=$month&day=$i\" STYLE= 'text-decoration: none'>$i </a></td>
	print "
		<td class=\"client\" align = 'left'> $account</td>
		<td class=\"client\" align = 'left'> $realm</td>
	    <td class=\"client\" align = 'right'>$total2 </td>";
	
	    
     }
         }
  
         

       
     $blended= sql_run("SELECT SUM(radacct.acctinputoctets)+ SUM(radacct.acctoutputoctets)as total,tableB.account,tableB.realm
                        FROM( SELECT CONCAT(msi_adsl.username,'@msp.co.za')as username, msi_adsl.realm,msi_adsl.account
                             FROM  msi_adsl
                             WHERE msi_adsl.realm = 'msp.co.za'
                             And(msi_adsl.account  = 'SAIX LOCAL'
                             OR msi_adsl.account = 'SAIX LITE'
                             OR msi_adsl.account = 'SAIX SHAPED'
                             OR msi_adsl.account = 'SAIX UNSHAPED')) as tableB,radacct
                             Where tableB.username = radacct.username
                             
                              AND MONTH(radacct.acctstarttime) = $month
                              AND YEAR(radacct.acctstarttime) = $year
                            Group by tableB.account");
     
  
     	if(is_array($blended)){ 
       foreach($blended as $blended){
       

       $tot= $blended['total'];
       $account =$blended['account']; 
        $realm = $blended['realm'];
     
   
        $total =  adsl::format_bytes($tot); 
  
  print "<tr >";

	print "
		<td class=\"client\" align = 'left'> $account</td>
		<td class=\"client\" align = 'left'> $realm</td>
	    <td class=\"client\" align = 'right'> $total</td>";
	       
         
}
     	}
   
	print '</table>';


}
  

  
  function format_bytes($bytes = 0){
   if ($bytes <= 1024){
     return "$bytes b";}
      else if ($bytes == null){
      	 return "b";
      	
    }
    else if ($bytes < 1048576){
      return number_format($bytes / 1024,2,"."," ") ." kb";
    } else if ($bytes < 512000000){
      return number_format($bytes / 1024 /1024,2,"."," ") ." Mb";
    } else {
      return number_format($bytes / 1024 /1024 /1024,2,"."," ") ." Gb";
    };
  }
  

}


?>

