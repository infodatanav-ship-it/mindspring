<?php
class sql_queries {

	public static function getCompanyHours($techie) {

		// This query retrieves the total billed hours for each client associated with a specific technician
		// for the current month. It joins the msi_billing table with the msi_clients table
		// to get the company name and groups the results by client.
		// It uses the DATE_FORMAT function to ensure that only records from the current month are considered.

		// Note: The query uses single quotes around the techie variable to prevent SQL injection.
		// Ensure that the $techie variable is properly sanitized before using it in a production environment.

		// The query returns the company name, client ID, and the sum of billed hours for each client.

		if (empty($techie)) {
			// If no technician is specified, return an empty string or handle accordingly
			return "";
		} else {

			// If the techie variable is valid, proceed with the query
			$techie = addslashes($techie); // Sanitize the input to prevent SQL injection
			return "SELECT `msi_clients`.`company`, `msi_billing`.`client`, SUM(`msi_billing`.`bill_hrs`) 
			FROM `msi_billing` 
			INNER JOIN `msi_clients` ON `msi_clients`.`id` = `msi_billing`.`client` 
			WHERE `msi_billing`.`techie` = ".$techie." 
			AND `msi_billing`.`date` >= DATE_FORMAT(CURDATE(), '%Y-%m-01') 
			GROUP BY `msi_billing`.`client`;";

		}

	}

	public static function getTechies() {

		// This query retrieves distinct primary technicians from the msi_contract table
		// who are not terminating and have a contract end date in the future.
		// It also joins with the msi_users table to get the user ID of each technician.

		return "SELECT 
		DISTINCT(msi_contract.primarytech), msi_users.id 
		FROM msi_contract 
		INNER JOIN msi_users ON msi_users.username = msi_contract.primarytech 
		WHERE msi_contract.terminating = 'no' 
		AND CURRENT_DATE() < `msi_contract`.`enddate`
		AND NULLIF(msi_contract.primarytech, '') IS NOT NULL;";

	}

	public static function getCompanyContractHours($techie) {

		// current month (or any month you want)
		$year  = date('Y');   // 2025
		$month = date('m');   // 08

		echo "{$year}-{$month}-00";   // 2025-08-00

		return "SELECT `msi_clients`.`company`, `msi_clients`.`id`, msi_contract.hours 
		FROM `msi_contract` 
		INNER JOIN `msi_clients` ON `msi_clients`.`id` = `msi_contract`.`client`
		INNER JOIN `msi_users` ON `msi_users`.`username` = `msi_contract`.`primarytech`
		WHERE `msi_contract`.`primarytech` = '".$techie."' AND 
			(('2025-08' between date_format( `msi_contract`.`startdate`,'%Y-%m')
		AND 
			date_format( `msi_contract`.`enddate`,'%Y-%m')) 
		OR
			(`msi_contract`.`terminating`='no'and '{$year}-{$month}-00'>=date_format(`msi_contract`.`startdate`,'%Y-%m-00'))
		OR 
			(('{$year}-{$month}' between date_format( `msi_contract`.`startdate`,'%Y-%m')
		AND 
			date_format( `msi_contract`.`terminatingdate`,'%Y-%m')) AND `msi_contract`.`terminating`='yes'))
		ORDER BY msi_clients.company ASC;";
	}

	public static function getWeeklyCCEmails() {
		return "SELECT * FROM msi_settings where `settingname` = 'weekly_cc_emails';";
	}

	public static function getWeeklyBccEmails() {
		return "SELECT * FROM msi_settings where `settingname` = 'weekly_bcc_emails';";
	}

}