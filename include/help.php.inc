<?php
class help {

  function welcome() {
  print "<h2>Browser Buttons</h2>
  			<h2>HelpDesk Buttons</h2>
         Internet Explorer <a 
         href=\"javascript:q=(document.frames.length?'':document.selection.createRange().text);for(i=0;i<document.frames.length;i++){q=document.frames[i].document.selection.createRange().text;if(q!='')break;}if(q=='')void(q=prompt('Client Search',''));if(q)location.href='https://intranet.mindspring.co.za/beta/index.php?module=helpdesk&action=search&search_string='+escape(q)\">
Client Search</a><br>
FireFox <a href=\"javascript:q=document.getSelection();for(i=0;i<frames.length;i++){q=frames[i].document.getSelection();if(q)break;}if(!q)void(q=prompt('Client Name:',''));if(q)location.href='https://intranet.mindspring.co.za/beta/index.php?module=helpdesk&action=search&search_string='+escape(q)\">Client Search</a><br>";
  }
}

?>