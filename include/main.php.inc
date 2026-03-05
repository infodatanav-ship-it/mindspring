<?php
// $Id: main.php.inc,v 1.2 2008/01/22 07:39:43 laurence Exp $

class main {
  function links() {
    $results = sql::sql_run('SELECT DISTINCT category
    FROM `msi_links`
    ORDER BY `category` ASC');
    print '<table  class="box">';
    
    foreach ($results as $result){
      print '<tr class="heading"><td colspan="2">'. $result[0] .'</td></tr>';
      $results2 = sql::sql_run('SELECT *
      FROM `msi_links`
      WHERE category = "'. $result[0] .'"
      ORDER BY `title` ASC 
      ');
      
      foreach ($results2 as $result2) {
      print '<tr>
        <td class=client><a href="'. $result2['link'].'">'. $result2['title'] .'</a></td>
        <td class=client>'. $result2['description'] .'</td>
	</tr>';
      }
    }
    print '</table>';
  }
}

?>
