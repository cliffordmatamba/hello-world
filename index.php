<?php

include ("/include/header.php");
include ("/include/config.php");

?>

<?php
date_default_timezone_set("Africa/Harare");


$date = date('m/d/Y h:i:s a', time());
echo $date;


?>
<div class="container-fluid jumbo">
 <h1 class="text-center">Smart School covid 19 project t</h1>
 <div class="mb-2">
 <h3 class="text-center ">A covid 19 IOT project prototype </h3>
 </div>
  <br>
  <br>
 <div class="container-fluid mx-auto">
 <div class="rows">
 
  <div class="col-sm-6 color_white col-sm-6-mod">
    <h3 class="text-center">Admin Dashboard</h3>
	<br>
     <a class="a_color" href="/admin/admin.php"><button type="button" class="btn btn-primary btn-lg">Admin Space</button></a>
  
  </div>
  <div class="col-sm-6 color_white col-sm-6-mod">
   <h3 class="text-center">User Dashboard</h3>
	<br>
    <a class="a_color" href="user/user.php"><button type="button" class="btn btn-primary btn-lg">User Space</button></a>
	 
  </div>
  
 </div>
 
 </div>
   
</div>



<?php

include ("/include/footer.php"); 

?>
