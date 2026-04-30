<?php
$conn = mysqli_connect("localhost", "root", "", "greenhouse");

if ($_SERVER['REQUEST_METHOD'] == 'POST') {
    $plant_name = mysqli_real_escape_string($conn, $_POST['plant_name']);
    
    // This will update the first row it finds, no matter what its ID is
    mysqli_query($conn, "UPDATE current_plant SET 
        plant_name='$plant_name', 
        temperature=0, 
        humidity=0, 
        soil_moisture=0, 
        light=0 
        LIMIT 1");
    
    echo "OK";
}
?>