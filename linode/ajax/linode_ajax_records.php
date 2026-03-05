<?php
session_start();

// IMPORTANT: Replace 'YOUR_LINODE_API_TOKEN' with your actual Linode API token.
// For production, store this securely (e.g., environment variable, config file outside web root).
$linodeApiToken = 'df26e6bee748bdf803ff10ee6ef9bb5faf0d35a3e3ee0c6a3a34f4b31a703418'; // User provided token

header('Content-Type: application/json');

// Function to send cURL requests to Linode API
function callLinodeApi($method, $endpoint, $token, $data = null) {
    $ch = curl_init();
    $url = 'https://api.linode.com/v4/' . $endpoint;

    curl_setopt($ch, CURLOPT_URL, $url);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
    curl_setopt($ch, CURLOPT_HTTPHEADER, [
        'Authorization: Bearer ' . $token,
        'Content-Type: application/json',
    ]);

    $response = curl_exec($ch);
    $http_code = curl_getinfo($ch, CURLINFO_HTTP_CODE);
    curl_close($ch);

    $responseData = json_decode($response, true);

    if ($http_code >= 200 && $http_code < 300) {
        return ['success' => true, 'message' => 'Operation successful.', 'data' => $responseData, 'http_code' => $http_code];
    } else {
        $errorReason = $responseData['errors'][0]['reason'] ?? 'Unknown API error.';
        return ['success' => false, 'message' => 'API Error (' . $http_code . '): ' . $errorReason, 'http_code' => $http_code];
    }
}

$action = $_GET['action'] ?? ''; // Using GET for list action

switch ($action) {
    case 'listRecords':
        $domainId = $_GET['domain_id'] ?? '';

        if (empty($domainId)) {
            echo json_encode(['success' => false, 'message' => 'Domain ID is required to list records.']);
            exit;
        }

        $result = callLinodeApi('GET', 'domains/' . $domainId . '/records', $linodeApiToken);
        if ($result['success']) {
            echo json_encode(['success' => true, 'data' => $result['data']]);
        } else {
            echo json_encode(['success' => false, 'message' => $result['message']]);
        }
        break;

    default:
        echo json_encode(['success' => false, 'message' => 'Invalid action.']);
        break;
}
?>
