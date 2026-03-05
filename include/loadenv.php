<?php
class loadEnv {
	public static function loadEnv($path = __DIR__) {

		$envFile = $path . '/.env';

		if (!file_exists($envFile)) {
			return false;
		}

		$lines = file($envFile, FILE_IGNORE_NEW_LINES | FILE_SKIP_EMPTY_LINES);

		foreach ($lines as $line) {
			// Skip comments
			if (strpos(trim($line), '#') === 0) {
				continue;
			}
			
			// Split by equals sign
			list($name, $value) = explode('=', $line, 2);
			
			$name = trim($name);
			$value = trim($value);
			
			// Remove quotes if present
			if (preg_match('/^"(.*)"$/', $value, $matches)) {
				$value = $matches[1];
			} elseif (preg_match('/^\'(.*)\'$/', $value, $matches)) {
				$value = $matches[1];
			}
			
			// Set environment variable
			putenv("$name=$value");
			$_ENV[$name] = $value;
			$_SERVER[$name] = $value;

			var_dump($name . ' = ' . $value);

		}

		return true;

	}
}