<?php
class getDBDetails {
	public static function loadEnvIni($path) {
		if (!file_exists($path)) {
			return [];
		}
		
		$content = file_get_contents($path);
		$lines = explode("\n", $content);
		$config = [];
		
		foreach ($lines as $line) {
			$line = trim($line);
			if (!empty($line) && strpos($line, '=') !== false && strpos($line, '#') !== 0) {
				list($key, $value) = explode('=', $line, 2);
				$key = trim($key);
				$value = trim($value);
				
				// Remove quotes
				$value = trim($value, '"\'');
				
				$config[$key] = $value;
				putenv("$key=$value");
				$_ENV[$key] = $value;
			}
		}
		
		return $config;
	}

	public static function getDBCredentials() {

		$env = self::loadEnvIni(__DIR__ . '/.env');

		$db = $env['SERVER'];
		$db = $env['USER'];
		$db = $env['PASSWORD'];
		$db = $env['DATABASE'];



		return [
			'SERVER'   => $env['SERVER'],
			'USER'     => $env['USER'],
			'PASSWORD' => $env['PASSWORD'],
			'DATABASE' => $env['DATABASE'],
		];
	}

}

?>