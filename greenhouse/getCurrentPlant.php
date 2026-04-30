<?php
$conn = mysqli_connect("localhost", "root", "", "greenhouse");

// This will fetch the first row it finds, no matter what its ID is
$result = mysqli_query($conn, "SELECT * FROM current_plant LIMIT 1");
$row = mysqli_fetch_assoc($result);

header('Content-Type: application/json');
echo json_encode($row);
?>