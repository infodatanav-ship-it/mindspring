<?php 

require_once 'sql.php.inc'; // Ensure you have the sql.php file included for database operations
class msClasses {

	public static function getCompanyHours($tech_name, $firstDay, $lastDay) {

		$sql = "SELECT msi_clients.company, msi_contract.hours, msi_clients.id as client_id, msi_users.id as user_id FROM msi_contract INNER JOIN msi_clients on msi_contract.client = msi_clients.id INNER JOIN msi_users ON msi_users.username = msi_contract.primarytech WHERE primarytech = '".$tech_name."' AND terminating = 'no';";
		$results = sql::sql_run($sql); 

		if ($results) {
			$data = [];
			foreach ($results as $row) {
				$data[] = [
					'company' => $row['company'],
					'hours' => $row['hours'],
					'hours_used' => self::getCompanyHoursByTech($row['user_id'], $row['client_id'], $firstDay, $lastDay) // Assuming you want to initialize hours_used to 0
				];
			}
			return [
				'data' => $data, 
				'status' => 'success'
			];
		} else {
			return [
				'data' => ['error' => 'No data found'], 
				'status' => 'error'
			];
		}

	}

	public static function getCompanyHoursByTech($user_id, $client_id, $firstDay, $lastDay) {

		// return self::getCompanyHours($tech_name);

		$sql = "SELECT SUM(`msi_billing`.`bill_hrs`) as `hours_used`
		FROM `msi_billing`
		WHERE (`msi_billing`.`date` BETWEEN '".$firstDay."' AND '".$lastDay."') AND
		`msi_billing`.`type` = 'Contract' AND 
		`msi_billing`.`techie` = ".$user_id." AND
		`msi_billing`.`client` = ".$client_id." GROUP BY `msi_billing`.`client`, `msi_billing`.`techie`;";

		// Note: The date range '2025-07-01' to '2025-07-30' is hardcoded. Adjust as necessary.
		// Debugging line to check the SQL query

		$results = sql::sql_run($sql);

		// print_r($results[0]['hours_used']); // Debugging line to check the results
		if (!$results || empty($results)) {
			return 0; // Return 0 if no results found
		} else {
			return $results[0]['hours_used']; // Debugging line to check the results
		}

	}
}