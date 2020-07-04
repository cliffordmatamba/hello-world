<?php 

 session_start();
 
 if(!isset($_SESSION['name'])) {
   
    header("location:../include/login.php"); 
    }
 else {

?>

<?php include("../include/header.php");

include ("../include/config.php");

?>




 <div class="container-fluid" style="background-color:lavenderblush";>

          <h1 class="h1_index text-center">Full attendance can be seen below.</h1>
		   <br>
		    <a class="a_color" href="admin.php"><button type="button" class="btn btn-primary btn-lg">Admin Dashboard</button></a>

		   <br>
<h4 class="text-center">Overall attendance of sorted by date.</h4>


		
		<table class="table table-striped table-bordered">
   <thead>
      <tr>
        <th>EID</th>
        <th>Name</th>
		<th>Date</th>
		<th>Present/Abscent</th>
      </tr>
    </thead>
    <tbody>
		
		
 <?php 
//select database

$sql="SELECT * FROM `attendance_taken` ORDER BY id ASC";

$record = mysqli_query($con,$sql);


while($post=mysqli_fetch_assoc($record)) {
  
   ?>
   
   
      <tr>
        <td><?php echo $post['eid']; ?></td>
        <td><?php echo $post['name']; ?></td>
		<td><?php echo $post['date']; ?></td>
		<td><?php echo $post['attendance']; ?></td>
	   </tr>
              
      
<?php } ?>    

     </tbody>
    </table>
	</div>
   

<?php include("../include/footer.php"); ?>

 <?php } ?>
