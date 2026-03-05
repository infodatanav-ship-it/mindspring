<?php
require_once 'msClass.php';							// Ensure you have the msClass.php file included for class definitions

$tech_name = $_GET['techie_id'];

// firstDay, lastDay

$firstDay = $_GET['firstDay'];
$lastDay = $_GET['lastDay'];
$return = msClasses::getCompanyHours($tech_name, $firstDay, $lastDay);

header('Content-Type: application/json');	// Set the content type to JSON
echo json_encode($return['data']);			// Return the data as JSON