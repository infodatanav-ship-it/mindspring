<?php


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

    switch ($method) {
        case 'POST':
            curl_setopt($ch, CURLOPT_POST, true);
            curl_setopt($ch, CURLOPT_POSTFIELDS, json_encode($data));
            break;
        case 'PUT':
            curl_setopt($ch, CURLOPT_CUSTOMREQUEST, 'PUT');
            curl_setopt($ch, CURLOPT_POSTFIELDS, json_encode($data));
            break;
        case 'DELETE':
            curl_setopt($ch, CURLOPT_CUSTOMREQUEST, 'DELETE');
            break;
    }

    $response = curl_exec($ch);
    $http_code = curl_getinfo($ch, CURLINFO_HTTP_CODE);

    if (curl_errno($ch)) {
        return ['success' => false, 'message' => 'cURL Error: ' . curl_error($ch), 'http_code' => 0];
    }

    curl_close($ch);

    $responseData = json_decode($response, true);

    if ($http_code >= 200 && $http_code < 300) {
        return ['success' => true, 'message' => 'Operation successful.', 'data' => $responseData, 'http_code' => $http_code];
    } else {
        $errorReason = $responseData['errors'][0]['reason'] ?? 'Unknown API error.';
        return ['success' => false, 'message' => 'API Error (' . $http_code . '): ' . $errorReason, 'http_code' => $http_code];
    }
}

$action = $_POST['action'] ?? '';

switch ($action) {
    case 'addDomain':
        $domain = $_POST['domain'] ?? '';
        $type = $_POST['type'] ?? '';
        $description = trim($_POST['description'] ?? ''); // Trim whitespace
        $group = trim($_POST['group'] ?? ''); // Trim whitespace for group as well
        $master_ips = $_POST['master_ips'] ?? '';
        $soa_email = trim($_POST['soa_email'] ?? ''); // New: Capture SOA email

        if (empty($domain) || empty($type)) {
            echo json_encode(['success' => false, 'message' => 'Domain name and type are required.']);
            exit;
        }

        // Validate description length if provided, or set to null if empty after trim
        if (!empty($description) && (strlen($description) < 1 || strlen($description) > 50)) {
            echo json_encode(['success' => false, 'message' => 'Description length must be between 1 and 50 characters.']);
            exit;
        }

        // Validate SOA email for master domains
        if ($type === 'master' && empty($soa_email)) {
            echo json_encode(['success' => false, 'message' => 'SOA Email is required for Master domains.']);
            exit;
        }

        $postData = [
            'domain' => $domain,
            'type' => $type,
            'description' => empty($description) ? null : $description, // Send null if empty
        ];

        // Add SOA email if type is master
        if ($type === 'master') {
            $postData['soa_email'] = $soa_email;
        }

        // Conditionally add group if it's not empty, and validate its length based on the error
        if (!empty($group)) {
            if (strlen($group) < 1 || strlen($group) > 253) {
                echo json_encode(['success' => false, 'message' => 'Group name length must be between 1 and 253 characters.']);
                exit;
            }
            $postData['group'] = $group;
        }


        if ($type === 'slave') {
            if (empty($master_ips)) {
                echo json_encode(['success' => false, 'message' => 'Master IPs are required for Slave domains.']);
                exit;
            }
            $postData['master_ips'] = array_map('trim', explode(',', $master_ips));
        }

        $result = callLinodeApi('POST', 'domains', $linodeApiToken, $postData);
        if ($result['success']) {
            echo json_encode(['success' => true, 'message' => 'Domain "' . $domain . '" created.', 'data' => $result['data']]);
        } else {
            echo json_encode(['success' => false, 'message' => $result['message']]);
        }
        break;

    case 'editDomain':
        $domainId = $_POST['domain_id'] ?? '';
        $domainName = $_POST['domain'] ?? ''; // Domain name is read-only but useful for messages
        $type = $_POST['type'] ?? '';
        $description = trim($_POST['description'] ?? ''); // Trim whitespace
        $group = trim($_POST['group'] ?? ''); // Trim whitespace for group as well
        $master_ips = $_POST['master_ips'] ?? '';
        $soa_email = trim($_POST['soa_email'] ?? ''); // New: Capture SOA email for edit, if needed

        if (empty($domainId) || empty($type)) {
            echo json_encode(['success' => false, 'message' => 'Domain ID and type are required for editing.']);
            exit;
        }

        // Validate description length if provided, or set to null if empty after trim
        if (!empty($description) && (strlen($description) < 1 || strlen($description) > 50)) {
            echo json_encode(['success' => false, 'message' => 'Description length must be between 1 and 50 characters.']);
            exit;
        }

        // Validate SOA email for master domains (for edit)
        if ($type === 'master' && empty($soa_email)) {
            echo json_encode(['success' => false, 'message' => 'SOA Email is required for Master domains.']);
            exit;
        }

        $putData = [
            'type' => $type, // Type can be changed
            'description' => empty($description) ? null : $description, // Send null if empty
        ];

        // Add SOA email if type is master (for edit)
        if ($type === 'master') {
            $putData['soa_email'] = $soa_email;
        }


        // Conditionally add group if it's not empty, and validate its length based on the error
        if (!empty($group)) {
            if (strlen($group) < 1 || strlen($group) > 253) {
                echo json_encode(['success' => false, 'message' => 'Group name length must be between 1 and 253 characters.']);
                exit;
            }
            $putData['group'] = $group;
        }

        // Linode API requires master_ips to be sent if type is slave, even if unchanged.
        // Or if changing from master to slave.
        if ($type === 'slave') {
            if (empty($master_ips)) {
                echo json_encode(['success' => false, 'message' => 'Master IPs are required for Slave domains.']);
                exit;
            }
            $putData['master_ips'] = array_map('trim', explode(',', $master_ips));
        } else {
             // If changing to master, ensure master_ips is not sent or is null.
             // Linode API docs don't explicitly say to send null, so omitting is safer.
        }


        $result = callLinodeApi('PUT', 'domains/' . $domainId, $linodeApiToken, $putData);
        if ($result['success']) {
            echo json_encode(['success' => true, 'message' => 'Domain "' . $domainName . '" updated.', 'data' => $result['data']]);
        } else {
            echo json_encode(['success' => false, 'message' => $result['message']]);
        }
        break;

    case 'deleteDomain':
        $domainId = $_POST['domain_id'] ?? '';

        if (empty($domainId)) {
            echo json_encode(['success' => false, 'message' => 'Domain ID is required for deletion.']);
            exit;
        }

        $result = callLinodeApi('DELETE', 'domains/' . $domainId, $linodeApiToken);
        if ($result['success']) {
            echo json_encode(['success' => true, 'message' => 'Domain (ID: ' . $domainId . ') deleted.']);
        } else {
            echo json_encode(['success' => false, 'message' => $result['message']]);
        }
        break;

    case 'addRecord':
        $domainId = $_POST['domain_id'] ?? '';
        $type = $_POST['type'] ?? '';
        $name = trim($_POST['name'] ?? ''); // Hostname - trim whitespace
        $target = $_POST['target'] ?? '';
        $ttl_sec = $_POST['ttl_sec'] ?? 300; // Default TTL

        if (empty($domainId) || empty($type) || empty($target)) {
            echo json_encode(['success' => false, 'message' => 'Domain ID, Record Type, and Target are required.']);
            exit;
        }

        $postData = [
            'type' => $type,
            'target' => $target,
            'ttl_sec' => (int)$ttl_sec,
        ];

        // Linode API expects 'name' for hostname. If empty or '@', use null for root.
        $postData['name'] = (empty($name) || $name === '@') ? null : $name;


        // Handle type-specific fields
        if ($type === 'MX') {
            $mx_priority = $_POST['mx_priority'] ?? 10;
            $postData['mx_priority'] = (int)$mx_priority;
        } elseif ($type === 'SRV') {
            $srv_priority = $_POST['srv_priority'] ?? 0;
            $srv_weight = $_POST['srv_weight'] ?? 5;
            $srv_port = $_POST['srv_port'] ?? 80;
            $postData['srv_priority'] = (int)$srv_priority;
            $postData['srv_weight'] = (int)$srv_weight;
            $postData['srv_port'] = (int)$srv_port;
            // For SRV, the 'name' field is usually _service._proto.name.domain.
            // Ensure `name` in `postData` is correctly formatted if required by Linode for SRV.
            // For simplicity, we are taking `name` as the hostname part. Linode API usually handles the full name.
        }

        $result = callLinodeApi('POST', 'domains/' . $domainId . '/records', $linodeApiToken, $postData);
        if ($result['success']) {
            echo json_encode(['success' => true, 'message' => 'DNS Record added for domain ID ' . $domainId . '.', 'data' => $result['data']]);
        } else {
            echo json_encode(['success' => false, 'message' => $result['message']]);
        }
        break;

    case 'editRecord':
        $domainId = $_POST['domain_id'] ?? '';
        $recordId = $_POST['record_id'] ?? '';
        $type = $_POST['type'] ?? '';
        $name = trim($_POST['name'] ?? '');
        $target = $_POST['target'] ?? '';
        $ttl_sec = $_POST['ttl_sec'] ?? 300; // Default TTL if not provided

        if (empty($domainId) || empty($recordId) || empty($type) || empty($target)) {
            echo json_encode(['success' => false, 'message' => 'Domain ID, Record ID, Record Type, and Target are required for editing.']);
            exit;
        }

        $putData = [
            'type' => $type, // Type is usually re-sent even if not changed
            'target' => $target,
            'ttl_sec' => (int)$ttl_sec,
        ];

        // Linode API expects 'name' for hostname. If empty or '@', use null for root.
        $putData['name'] = (empty($name) || $name === '@') ? null : $name;

        // Handle type-specific fields for update
        if ($type === 'MX') {
            $mx_priority = $_POST['mx_priority'] ?? 10;
            $putData['mx_priority'] = (int)$mx_priority;
        } elseif ($type === 'SRV') {
            $srv_priority = $_POST['srv_priority'] ?? 0;
            $srv_weight = $_POST['srv_weight'] ?? 5;
            $srv_port = $_POST['srv_port'] ?? 80;
            $putData['srv_priority'] = (int)$srv_priority;
            $putData['srv_weight'] = (int)$srv_weight;
            $putData['srv_port'] = (int)$srv_port;
        }

        $result = callLinodeApi('PUT', 'domains/' . $domainId . '/records/' . $recordId, $linodeApiToken, $putData);
        if ($result['success']) {
            echo json_encode(['success' => true, 'message' => 'DNS Record (ID: ' . $recordId . ') updated for domain ID ' . $domainId . '.', 'data' => $result['data']]);
        } else {
            echo json_encode(['success' => false, 'message' => $result['message']]);
        }
        break;


    default:
        echo json_encode(['success' => false, 'message' => 'Invalid action.']);
        break;
}

?>