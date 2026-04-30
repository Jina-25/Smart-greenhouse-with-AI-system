<?php
$conn = mysqli_connect("localhost", "root", "", "greenhouse");

if ($_SERVER['REQUEST_METHOD'] == 'POST') {
    $temp  = $_POST['temperature'];
    $hum   = $_POST['humidity'];
    $soil  = $_POST['soil_moisture'];
    $light = $_POST['light'];
    
    // This will update the first row it finds, no matter what its ID is
    mysqli_query($conn, "UPDATE current_plant SET 
        temperature='$temp', 
        humidity='$hum', 
        soil_moisture='$soil', 
        light='$light',
        timestamp=NOW()
        LIMIT 1");
    
    echo "OK";
}
?>