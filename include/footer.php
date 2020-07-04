<footer>
  <div class="container">
      <div class="footer-box">
	  <?php 
	        date_default_timezone_set('Africa/Harare');
	           $date = date("Y-m-d");
               $newDate = date("d-m-Y", strtotime($date));
      ?>			   
               
        <p>Copyright &copy; <?php echo date("Y"); ?> Juxbyincorporated</p>
		</div>
  </div>
</footer>
</body>
</html>