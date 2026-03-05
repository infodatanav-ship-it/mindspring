<?php
require_once("dompdf_config.inc.php");
/*
$html =
  '<html><body>'.
  '<p>Put your html here, or generate it with your favourite '.
  'templating system.</p>'.
  '</body></html>';
 * */
 
$html='<html><head>
	<meta http-equiv="CONTENT-TYPE" content="text/html; charset=utf-8">

	<title></title><meta name="GENERATOR" content="OpenOffice.org 2.3  (Linux)">
	<meta name="AUTHOR" content="tichaona maunga">
	<meta name="CREATED" content="20090522;12240900">
	<meta name="CHANGEDBY" content="tichaona maunga">
	<meta name="CHANGED" content="20090522;15395100">
	<meta name="CHANGEDBY" content="tichaona maunga">
	<meta name="CHANGEDBY" content="tichaona maunga">
	<meta name="CHANGEDBY" content="tichaona maunga">
	<meta name="CHANGEDBY" content="tichaona maunga">
	<meta name="CHANGEDBY" content="tichaona maunga">
	<style type="text/css">
	<!--
		@page { size: 8.5in 11in; margin: 0.79in }
		P { margin-bottom: 0.08in }
        .reduce{ line-height:.1em;}
		TD P { margin-bottom: 0in }
        .table {
    font-size:inherit;
    border-collapse: collapse;
}

	-->
	</style></head>

<body dir="ltr" lang="en-US">
   <p class="reduce" style="margin-bottom: 0in;" align="justify"><img src="logo.jpg" name="graphics1" align="left" border="0" height="45" width="189"><br clear="left">
       <font face="Liberation Sans, sans-serif"><font size="2">Mindspring
Computing</font></font></p>
<p class="reduce" style="margin-bottom: 0in;" align="justify"><font face="Liberation Sans, sans-serif"><font size="2">Unit
5 Doncaster Office Park</font></font></p>
<p class="reduce" style="margin-bottom: 0in;" align="justify"><font face="Liberation Sans, sans-serif"><font size="2">Punters
way Kennilworth</font></font></p>
<p class="reduce" style="margin-bottom: 0in;" align="justify"><font face="Liberation Sans, sans-serif"><font size="2">P.o
box 46926 ,Glosderry,7702</font></font></p>
<p class="reduce" style="margin-bottom: 0in;" align="justify"><font face="Liberation Sans, sans-serif"><font size="2">Tel
(021) 657 1780</font></font></p>
<p class="reduce" style="margin-bottom: 0in;" align="justify"><font face="Liberation Sans, sans-serif"><font size="2">Fax
(021) 671 7599</font></font></p>
<p class="reduce" style="margin-bottom: 0in;" align="justify"><font face="Liberation Sans, sans-serif"><font size="2">Email
:<a href="mailto:info@mindspring.co.za">info@mindsprin</a></font></font><a href="mailto:info@mindspring.co.za"><font size="2">g.co.za</font></a></p>
<p style="margin-bottom: 0in;" align="justify"><br>
</p>
<table border="0" cellpadding="0" cellspacing="0" width="100%">
	<col width="128*">
	<col width="128*">
	<tbody><tr valign="top">
		<td width="50%">
			<table>
				
				<tr>
					<td id="subtotal" valign="top" nowrap="nowrap">
						Tichaona Maunga
						
					</td>
				</tr>
				<tr>
					<td id="subtotal" valign="top" nowrap="nowrap">
						Flat number 114 north drive
						
					</td>
				</tr>
				<tr>
					<td id="subtotal" valign="top">
						<p align="justify">bellville
						</p>
					</td>
				</tr>
				<tr>
					<td id="subtotal" valign="top">
						<p align="justify">7530
						</p>
					</td>
				</tr>
			</table>
			<p align="justify"><br>
			</p>
		</td>
		<td>
			<table align="right">
            <tbody><tr>
                <td>
                   2009/11/11
                </td>
            </tr>

            </tbody></table>



		</td>
	</tr>
</tbody></table>
<p style="margin-bottom: 0in;" align="justify"><br>
</p>
<table class="table" border="1" bordercolor="#000000" cellpadding="4" cellspacing="0" width="100%">
		<tr>
		<td colspan="2" valign="top" width="100%">
			<p align="center"><font face="Liberation Sans, sans-serif"><font size="4">Job description</font></font></p>
		</td>
	</tr>
	<tr>
		<td colspan="2" valign="top" width="100%">
			<p><!--description--><br>
			</p>
		</td>
	</tr>
	<tr valign="top">
		<td width="89%">
			<p align="justify"><font face="Liberation Sans, sans-serif"><font size="4">Total hours</font></font></p>
		</td>
		<td width="11%">
			<p><!--hours--><br>
			</p>
		</td>
	</tr>
</table>
<p style="margin-bottom: 0in;" align="justify"><font face="Liberation Sans, sans-serif"><font size="4">Comments:</font></font><br>
</p>
<p style="margin-bottom: 0in;" align="justify"><font face="Liberation Sans, sans-serif"><font size="4">......................................................................................................................................................................................</font></font><br>
</p>
<p style="margin-bottom: 0in;" align="justify"><font face="Liberation Sans, sans-serif"><font size="4">......................................................................................................................................................................................</font></font><br>
</p>
<p style="margin-bottom: 0in;" align="justify"><br>
</p>

<p style="margin-bottom: 0in;" align="justify"><font face="Liberation Sans, sans-serif"><font size="4">Technician:<!--tech--> </font></font>
</p>
<br><br>
<p style="margin-bottom: 0in;" align="justify"><font face="Liberation Sans, sans-serif"><font size="4">Signature.......................................................</font></font></p>
<p style="margin-bottom: 0in;" align="justify"><br>
</p>
<p style="margin-bottom: 0in;" align="justify"><font face="Liberation Sans, sans-serif"><font size="4">Client :<!--address1 --> </font></font>
</p>
<p style="margin-bottom: 0in;" align="justify"><br>
</p>
<p style="margin-bottom: 0in;" align="justify"><font face="Liberation Sans, sans-serif"><font size="4">Signature......................................................</font></font></p>

<div align="right"> <a href="javascript:void(0)" onclick="window.print()"><img alt="print page" src="content/images/printer.gif" border="0"></a></div>
</body></html>';

$dompdf = new DOMPDF();

$dompdf->load_html($html);
$dompdf->render();

$dompdf->stream("sample.pdf");
//$pdf_string = $dompdf->output();

 /*
$filepath =
"/var/www/html/intranet/documentation/guides/maunga.pdf";
    file_put_contents("".$filepath."", $pdf_string);
    echo $filepath;
  */
  
//$dompdf->stream("/var/www/html/sample.pdf");

?>
