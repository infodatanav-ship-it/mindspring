<?php
// $Id: doc.php.inc,v 1.6 2009/03/27 08:22:40 etienne Exp $

class doc {
    function upload_form(){


  if(!acl::has_access($_SESSION['id'],"documents admin","add")) return 1;
   $sql='select * from category';
   $results=sql::sql_run($sql);
   

     print "<span class=\"head\">Upload New Document</span>
            <table class=\"box\">
<form action=\"index.php?module=doc&action=upadd\" name=\"contadd\"method=\"POST\"  ENCTYPE=\"multipart/form-data\">
              <tr>
              <td align=\"right\"><b>Title:</b>
               </td>
                 <td>
        <input name=\"uptitle\" size=\"32px\">";

      print "

               </td>
	</tr>
<tr>
        <td align=\"right\"><b>Category:</b></td>
        <td> <select name=\"category\" readonly>";
                  foreach ($results as $result) {
                   print "<option value=\"$result[id]\">$result[category]</option>";
                 };
      print "
                 </select></td>
	</tr>

        <tr>
        <td align=\"top\"><b>Description:</b></td>
        <td><textarea name=\"updescription\" cols=70 rows=8>
</textarea></td>
	</tr>
<tr>
        <td align=\"right\"><b>Upload:</b></td>
        <td>
    <input type=\"file\" name=\"upfile\"  />

</td>
	</tr>
      </tr>
<tr><td colspan=\"2\" align=\"center\"></tr>
<tr><td colspan=\"2\" align=\"center\" ></tr>
<tr><td colspan=\"2\" align=\"center\"></tr>
<tr><td colspan=\"2\" align=\"center\" ></tr>
 	<tr><td colspan=\"2\" align=\"center\">
          <input type=\"submit\" value=\"Submit\" class=\"buttons\">
        </td></tr>
</from>
	</table>";
}


     function navigation(){
         print '<div class="heading">
          <a href="?module=doc&action=list">List</a>:
          ';
if(acl::has_access($_SESSION['id'],"documents admin","read")){
     print'<a href="?module=doc&action=add">Upload Documents</a>';
     print"&nbsp:";
     print'<a href="?module=doc&action=admin">Documents Admin</a>';
     print"&nbsp:";
     print'<a href="?module=doc&action=categ">Categories</a>';
     print"&nbsp:";
     print'<a href="?module=doc&action=cat_add">Add Category</a>';
     }
          print'</div><p>';

}
function add($title,$description,$file,$tmpname,$filesize,$filetype,$errors,$category)
{

    if($title=='') {
   
	 echo "<font color='red'><b>ERROR:Title can't be null</b></font>";
	 
      return false;


    }

    if($description=='') {
    echo "<font color='red'><b>ERROR:Description can't be null</b></font>";
      return false;


    }
    if($file=='') {
      echo "<font color='red'><b>ERROR:You have to uplaod a document first</b></font>";
      return false;


    }
    

    $working_folder = "/var/www/html/intranet/documentation/guides/";
    $working_folder2="../documentation/guides/";
    //upload document
    $title=doc::cleansql($title);
    $description=doc::cleansql($description);
		@$fileName = $file;
		@$tmpName  = $tmpname;
		@$fileSize = $filesize;
		@$fileType = $filetype;
       $docpath=$working_folder2.$fileName;
       echo $docpath;
		if(move_uploaded_file ($tmpName, $working_folder.$fileName))
        {
         $sql ="insert into doctable(title,description,docpath,category) values('$title','$description','$docpath','$category')";
         $results = sql::sql_run($sql);
        Print "<font color=\"red\"><div style=\"font-color:red\">File successfully uploaded</div></font>";
        doc::table();
        }
        else
        {
        Print "<font color=\"red\"><div style=\"font-color:red\">There was an error uploading file</div></font>";
        doc::upload_form();
        }


       
}
     function table() {
    print "<span class=\"head\">Document List</span>";

    print '<table  class="client">
            <tr class="head"><td>Manuals</td><td></td></tr>
            <tr>
              <td  class="client"><a href="http://geode.mindspring.co.za/documentation/manuals/mrtg">MRTG Manual</a></td>
             <td  class="client">Multi Router Traffic Grapher online manual.</td>
       </tr>

        <tr>
                <td  class="client"><a href="http://geode.mindspring.co.za/documentation/manuals/php-man">PHP Manual</a></td>
                <td  class="client">The online manual for the php scripting language.</td>
        </tr>
        <tr>
                <td  class="client"><a href="http://geode.mindspring.co.za/documentation/manuals/mysql-man">mySQL Manual</a></td>
                <td  class="client">The online manual and admin guide for mySQL.</td>
        </tr>
        <tr>
                <td  class="client"><a href="http://geode.mindspring.co.za/documentation/manuals/rute.pdf">Rute eBook</a></td>
                <td  class="client">LINUX: Rute Users Tutorial and Exposition, by Mr Paul Sheer</td>
        </tr>

            </table><br>';


//this is a hack to update the date changed for documents
        if ($handle = opendir('./documentation/guides/')) {
        while (false !== ($file = readdir($handle))) {
        $i++;
        if ($file != '.' && $file != '..' ){

     // open file and get first line
          $filename = "./documentation/guides/$file";

         $datelast= date("Y-m-d", filectime($filename));
        $newfname='.'.$filename;
        $sql="update doctable set datecreated='$datelast' where docpath='$newfname'";
        sql::sql_run($sql);
        }}}
        
        $docsel="select id as i,category as b from category";
         $doc=sql::sql_run($docsel);
         
        
        
    print '<span class="head">In House Guides </span>
          <br></br>
         <br></br>
    			<table class="client">
            <tr class="head"><td>Title</td><td>Description</td></tr>';
         foreach($doc as $docs)
         {
          $sql=('SELECT doctable.title as a,doctable.description AS b,doctable.docpath as c,doctable.category as d FROM doctable  where category="'.$docs[i].'" order by datecreated desc');
$results=sql::sql_run($sql);
          /*
          $sql ="select title,description,docpath from doctable";
         $results = sql::sql_run($sql);*/
             if(!empty($results))
             {
         print'<tr class="head"><td>'.$docs[b].'</td><td></td></tr>';
             }
    foreach ($results as $result) {
        
        print '<tr><td class="client"><a href="'.$result[c].'">'.$result[a].'</a></td><td class="client" width=\"0px\" >'.$result[b].'</td></tr>';
     }
         }
    print '</table>';
  }

function list_edit() {
    if(!acl::has_access($_SESSION['id'],"documents admin","add")) return 1;

    print "<span class=\"head\">Document Administration</span>";
    print '<table  class="client">
            <tr class="head"><td>Manuals</td><td></td></tr>
            <tr>
              <td  class="client"><a href="http://geode.mindspring.co.za/documentation/manuals/mrtg">MRTG Manual</a></td>
             <td  class="client">Multi Router Traffic Grapher online manual.</td>
       </tr>

        <tr>
                <td  class="client"><a href="http://geode.mindspring.co.za/documentation/manuals/php-man">PHP Manual</a></td>
                <td  class="client">The online manual for the php scripting language.</td>
        </tr>
        <tr>
                <td  class="client"><a href="http://geode.mindspring.co.za/documentation/manuals/mysql-man">mySQL Manual</a></td>
                <td  class="client">The online manual and admin guide for mySQL.</td>
        </tr>
        <tr>
                <td  class="client"><a href="http://geode.mindspring.co.za/documentation/manuals/rute.pdf">Rute eBook</a></td>
                <td  class="client">LINUX: Rute Users Tutorial and Exposition, by Mr Paul Sheer</td>
        </tr>

            </table><br>';


         $docsel="select id as i,category as b from category";
         $doc=sql::sql_run($docsel);
    print '<span class="head">In House Guides </span>
             <br></br>
             <br></br>
    			<table class="client">
            <tr class="head"><td>Title</td><td>Description</td></tr>';
    foreach($doc as $docs)
         {
     $sql=('SELECT doctable.id as i,doctable.title as a,doctable.description AS b,doctable.docpath as c,doctable.category as d FROM doctable  where category="'.$docs[i].'" order by datecreated desc');
$results=sql::sql_run($sql);
         /*
         $sql ="select id,title,description,docpath from doctable";
         $results = sql::sql_run($sql);
*/
         if(!empty($results))
             {
         print'<tr class="head"><td>'.$docs[b].'</td><td></td></tr>';
             }
     foreach ($results as $result) {
         
          print '<tr><td class="client"><a href="?module=doc&action=edit&id='.$result[i].'" >'.$result[a].'</a></td><td class="client">'.$result[b].'</td></tr>';
     }
         }
    print '</table>';
  }

function edit_form($id){
if(!acl::has_access($_SESSION['id'],"documents admin","add")) return 1;

  if(!acl::has_access($_SESSION['id'],"contracts","add")) return 1;
$sql="select id,title,description,docpath,category from doctable where id='$id' ";
$results = sql::sql_run($sql);

$cat="select * from category";
$rescats=sql::sql_run($cat);

     print "<span class=\"head\">Edit Document</span>
            <table class=\"box\">
<form action=\"index.php?module=doc&action=doc_edit\" name=\"contadd\"method=\"POST\"  ENCTYPE=\"multipart/form-data\">
              <tr>
              <td align=\"right\"><b>Title:</b>
               </td>
                 <td>
        <input name=\"uptitle\" value=\"".$results[0][1]."\"  size=\"32px\"/>";

      print "

               </td>
	</tr>
<tr>
 <td align=\"right\"><b>Category:</b>
<td>

<select name=\"category\">";
   foreach ($rescats as $rescat) {
		print '<option value="' . $rescat[id]. '"';
		if ($rescat[id]==$results[0][4]) { print " selected"; }
		print ">" .$rescat[category]. "</option>\n";

}
	print "</select>\n
</td>
</tr>
        <tr>
        <td align=\"top\"><b>Description:</b></td>
        <td><textarea name=\"updescription\" cols=70 rows=8 >".$results[0][2]."
</textarea></td>
	</tr>
<input type=\"hidden\" value=\"".$results[0][0]."\" name=\"id\" />
<tr>
        <td align=\"right\"><b>DocPath:</b></td>
        <td>
    <input type=\"text\" name=\"docpath\" value=\"".$results[0][3]."\" size=\"32px\"s />

</td>
	</tr>

      </tr>
<tr><td colspan=\"2\" align=\"center\"></tr>
<tr><td colspan=\"2\" align=\"center\" ></tr>
<tr><td colspan=\"2\" align=\"center\"></tr>
<tr><td colspan=\"2\" align=\"center\" ></tr>
 	<tr><td colspan=\"2\" align=\"center\">
          <input type=\"submit\" value=\"Submit\" class=\"buttons\">
         <input type=\"submit\" value=\"Delete\" name=\"Delete\" onclick=\"return confirm('Are you sure you want to delete this item?')\" class=\"delete\">
        </td></tr>
</from>
	</table>";
}

function edit($id,$uptitle,$updescription,$docpath,$category){
    if(!acl::has_access($_SESSION['id'],"documents admin","edit")) return 1;
  



if($uptitle=='' || $updescription==''|| $docpath=='') {
     echo '<font color="red"><b>ERROR: Neither of the three fields can be null</b></font>';
      return false;


    }

$ret_update = sql::sql_run("update doctable set title = '$uptitle',description='$updescription',docpath='$docpath',category='$category' where id='$id' ");

if(!isset($ret_update)){
    print "<b>Record updated correctly</b><p>";
    doc::list_edit(NULL,NULL);
    } else {
      print "<b>Something went wrong !</b>";
    }
}
function delete($id,$docpath){
  if(!acl::has_access($_SESSION['id'],"documents admin","add")) return 1;
 
$docpath=str_replace('../','',$docpath);
$docpath='./'.$docpath;

fclose($docpath);
unlink($docpath);

$result = sql::sql_run('delete from doctable where id = "'.$id.'"');
if(!isset($result)){

    print "<b>Record Deleted </b><p>";
    doc::list_edit(NULL,NULL);
       } else {
      print "<b>Something went wrong !</b>";
    }

}

function cleansql($textline) {
	$textline = trim($textline);
	$textline = stripslashes($textline);
	$textline = str_replace("'","`",$textline);
	return $textline;
}
function category()
{
   
if(!acl::has_access($_SESSION['id'],"documents admin","add")) return 1;
    print '<span class="head">Categories </span>
    			<table class="client">
            <tr class="head"><td>Category</td></tr>';
          $sql ="select id,category from category";
         $results = sql::sql_run($sql);
         

     foreach ($results as $result) {
          print '<tr><td class="client"><a href="?module=doc&action=edit_cat&id='.$result[id].'">'.$result[category].'</a></td></tr>';
     }
    print '</table>';
  }

  function edit_catform($id){
if(!acl::has_access($_SESSION['id'],"documents admin","add")) return 1;

  if(!acl::has_access($_SESSION['id'],"contracts","add")) return 1;


$cat="select id,category from category where id='$id'";
$rescats=sql::sql_run($cat);

     print "<span class=\"head\">Edit Category</span>
            <table class=\"box\">
<form action=\"index.php?module=doc&action=edit_category\" name=\"contadd\"method=\"POST\"  ENCTYPE=\"multipart/form-data\">
              <tr>
              <td align=\"right\"><b>Category:</b>
               </td>
                 <td>
        <input name=\"category\" value=\"".$rescats[0][1]."\"  size=\"32px\"/>";

      print "

               </td>
	</tr>

<input type=\"hidden\" value=\"".$rescats[0][0]."\" name=\"id\" />
<tr><td colspan=\"2\" align=\"center\"></tr>
<tr><td colspan=\"2\" align=\"center\" ></tr>
<tr><td colspan=\"2\" align=\"center\"></tr>
<tr><td colspan=\"2\" align=\"center\" ></tr>
 	<tr><td colspan=\"2\" align=\"center\">
          <input type=\"submit\" value=\"Submit\" class=\"buttons\">
         <input type=\"submit\" value=\"Deletecat\" name=\"Delete\" onclick=\"return confirm('Are you sure you want to delete this item?')\" class=\"delete\">
        </td></tr>
</from>
	</table>";
}

function edit_cat($id,$category){
    if(!acl::has_access($_SESSION['id'],"documents admin","add")) return 1;
  
if($category=='') {
     echo"<font color='red'><b>ERROR: Category can't be null</b></font>";
      return false;


    }

$ret_update = sql::sql_run("update category set category = '$category' where id='$id' ");

if(!isset($ret_update)){
    print "<b>Record updated correctly</b><p>";
    doc::category(NULL,NULL);
    } else {
      print "<b>Something went wrong !</b>";
    }
}
 function cat_add(){

if(!acl::has_access($_SESSION['id'],"documents admin","add")) return 1;
  
     print "<span class=\"head\">Add Category</span>
            <table class=\"box\">
<form action=\"index.php?module=doc&action=addcategory\" name=\"contadd\"method=\"POST\"  ENCTYPE=\"multipart/form-data\">
              <tr>
              <td align=\"right\"><b>Category:</b>
               </td>
                 <td>
        <input name=\"category\" value=\"\"  size=\"32px\"/>";

      print "

               </td>
	</tr>


<tr><td colspan=\"2\" align=\"center\"></tr>
<tr><td colspan=\"2\" align=\"center\" ></tr>
<tr><td colspan=\"2\" align=\"center\"></tr>
<tr><td colspan=\"2\" align=\"center\" ></tr>
 	<tr><td colspan=\"2\" align=\"center\">
          <input type=\"submit\" value=\"Submit\" class=\"buttons\">
                 </td></tr>
</from>
	</table>";
}

function addcategory($category){
  if(!acl::has_access($_SESSION['id'],"documents admin","add")) return 1;

if($category=='') {
      echo "<font color='red'><b>ERROR: Category can't be null</b></font>";
      return false;


    }

if(!preg_match('/^[a-zA-Z]{1,}/',$category))
{
	echo"<font color='red'><b>ERROR: Characters entered for category are invalid</b></font>";
      return false;
}


$ret_update = sql::sql_run("insert into category(category) values('$category')");

if(!isset($ret_update)){
    print "<b>Record inserted correctly</b><p>";
    doc::category(NULL,NULL);
    } else {
      print "<b>Something went wrong !</b>";
    }
}

function deletecat($id){
  if(!acl::has_access($_SESSION['id'],"documents admin","add")) return 1;
$ret_existscnt = sql::sql_run("select category from doctable where category='$id' ");
if($ret_existscnt)
{
 print"There is a record linked to this category therefore it can't be deleted";
exit;

}
$result = sql::sql_run('delete from category where id = "'.$id.'"');
if(!isset($result)){

    print "<b>Record Deleted </b><p>";
    doc::category(NULL,NULL);
       } else {
      print "<b>Something went wrong !</b>";
    }

}
//this function is to be removed later beacause it was meant to compare documents
function tableold() {
    print '<table  class="client">
            <tr class="head"><td>Manuals</td><td></td></tr>
            <tr>
              <td  class="client"><a href="http://geode.mindspring.co.za/documentation/manuals/mrtg">MRTG Manual</a></td>
             <td  class="client">Multi Router Traffic Grapher online manual.</td>
       </tr>

        <tr>
                <td  class="client"><a href="http://geode.mindspring.co.za/documentation/manuals/php-man">PHP Manual</a></td>
                <td  class="client">The online manual for the php scripting language.</td>
        </tr>
        <tr>
                <td  class="client"><a href="http://geode.mindspring.co.za/documentation/manuals/mysql-man">mySQL Manual</a></td>
                <td  class="client">The online manual and admin guide for mySQL.</td>
        </tr>
        <tr>
                <td  class="client"><a href="http://geode.mindspring.co.za/documentation/manuals/rute.pdf">Rute eBook</a></td>
                <td  class="client">LINUX: Rute Users Tutorial and Exposition, by Mr Paul Sheer</td>
        </tr>

            </table><br>';
    
    print '<span class="head">In House Guides </span>
    			<table class="client">
            <tr class="head"><td>Title</td><td>Description</td></tr>';
       if ($handle = opendir("./documentation/guides/")) {
           while (false !== ($file = readdir($handle))) {
        if ($file != '.' && $file != '..' ){
          // open file and get first line 
          $filename = "./documentation/guides/$file";

          $fp = fopen($filename, "r");

	  if((substr($filename,-3) == "doc") || (substr($filename,-3) == "pdf") )
		{
		   $blurb = "Extended description for this filetype is anavailable, please click on the title to download...";
		}else
		{
         	   $blurb = fgets($fp, 4096);
		}
          fclose($fp);
          // clean title
          $title = strtr($file,"-_","  ");
          $title = substr($title,0,-4);
          print '<tr><td class="client"><a href="http://geode.mindspring.co.za/documentation/guides/'.$file.'">'.$title.'</a></td>
		  <td class="client">'.$blurb.'</td></tr>';
        }
      }
      
    }
    print '</table>';
  }
}
//||(($filename[strlen($filename)-1]=='f')&&($filename[strlen($filename)-2]=='d')&&($filename[strlen($filename)-3]=='p'))

?>