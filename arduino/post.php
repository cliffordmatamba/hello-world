<
<?php

$servername = "localhost";

$dbname = "ferroknb_crypto";

$username = "ferroknb_cliff";

$password = "Juxby2019/";

$api_key_value = "tPmAT5Ab3j7F9";

$api_key= $name = $date = $attendance = $temp = $fmask = "";

if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $api_key = test_input($_POST["api_key"]);
    if($api_key == $api_key_value) {
        $sensor = test_input($_POST["name"]);
        $location = test_input($_POST["date"]);
        $value1 = test_input($_POST["attendance"]);
        $value2 = test_input($_POST["temp"]);
        $value3 = test_input($_POST["fmask"]);
        
        // Create connection
        $conn = new mysqli($servername, $username, $password, $dbname);
        // Check connection
        if ($conn->connect_error) {
            die("Connection failed: " . $conn->connect_error);
        } 
        
        $sql = "INSERT INTO attendance_taken (name, date, attendance , temp, fmask)
        VALUES ('" . $name . "', '" . $date . "', '" . $attendance . "', '" . $temp . "', '" . $fmask . "')";
        
        if ($conn->query($sql) === TRUE) {
            echo "New record created successfully";
        } 
        else {
            echo "Error: " . $sql . "<br>" . $conn->error;
        }
    
        $conn->close();
    }
    else {
        echo "Wrong API Key provided.";
    }

}
else {
    echo "No data posted with HTTP POST.";
}

function test_input($data) {
    $data = trim($data);
    $data = stripslashes($data);
    $data = htmlspecialchars($data);
    return $data;
}
