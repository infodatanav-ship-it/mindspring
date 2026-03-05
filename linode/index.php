<?php
// linode.php


// --- Linode API Integration (Initial Fetch) ---
// IMPORTANT: Replace 'YOUR_LINODE_API_TOKEN' with your actual Linode API token.
// Consider storing this token securely, e.g., in an environment variable or a config file
// outside the web root, instead of directly in the code for production.
$linodeApiToken = 'df26e6bee748bdf803ff10ee6ef9bb5faf0d35a3e3ee0c6a3a34f4b31a703418'; // User provided token
$linodeApiUrl = 'https://api.linode.com/v4/domains';

$ch = curl_init();
curl_setopt($ch, CURLOPT_URL, $linodeApiUrl);
curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
curl_setopt($ch, CURLOPT_HTTPHEADER, [
    'Authorization: Bearer ' . $linodeApiToken,
    'Content-Type: application/json',
]);

$response = curl_exec($ch);

$domains = [];
$errorMessage = null;

if (curl_errno($ch)) {
    $errorMessage = "cURL Error: " . curl_error($ch);
} else {
    $data = json_decode($response, true);
    if (isset($data['errors'])) {
        $errorMessage = "Linode API Error: " . ($data['errors'][0]['reason'] ?? 'Unknown API error');
    } else {
        $domains = $data['data'] ?? [];
    }
}
curl_close($ch);
// --- End Linode API Integration (Initial Fetch) ---
?>
<!DOCTYPE html>
<html lang="en">

<head>

    <title>...:: Linode ::...</title>


    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.7.2/css/all.min.css" xintegrity="sha512-Evv84Mr4kqVGRNSgIGL/F/aIDqQb7xQ2vcrdIwxfjThSH8CSR7PBEakCr51Ck+w+/U6swU2Im1vVX0SVk9ABhg==" crossorigin="anonymous" referrerpolicy="no-referrer" />
    
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.0-alpha3/dist/css/bootstrap.min.css" rel="stylesheet"
        xintegrity="sha384-KK94CHFLLe+nY2dmCWGMq91rCGa5gtU4mk92HdvYe+M/SXH301p5ILy+dN9+nJOZ" crossorigin="anonymous">


    <link rel="stylesheet" type="text/css" href="https://cdn.datatables.net/2.0.8/css/dataTables.dataTables.min.css">
    <link rel="stylesheet" type="text/css" href="https://cdn.datatables.net/2.0.8/css/dataTables.bootstrap5.min.css">


    <style>
        .action-buttons button {
            margin-right: 5px;
            margin-bottom: 5px; /* For stacking on smaller screens */
        }
        .dataTables_wrapper .dataTables_paginate .paginate_button.current,
        .dataTables_wrapper .dataTables_paginate .paginate_button.current:hover {
            background-color: #0d6efd !important; /* Bootstrap primary blue */
            color: white !important;
            border-color: #0d6efd !important;
        }
    #viewRecordsModal .modal-dialog {
        max-width: 1220px !important; /* 1200px (modal-xl) + 20px */
    }
    </style>
</head>
<body>


                        <h2 class="mb-4">Domain Administration</h2>

                        <?php if ($errorMessage): ?>
                            <div class="alert alert-danger" role="alert">
                                Error fetching domains: <?php echo htmlspecialchars($errorMessage); ?>
                            </div>
                        <?php else: ?>
                            <button type="button" class="btn btn-primary mb-3" data-bs-toggle="modal" data-bs-target="#addDomainModal">
                                <i class="fas fa-plus"></i> Add New Domain
                            </button>
                            <table id="domainsTable" class="table table-striped table-hover" style="background-color:#FFF;">
                                <thead>
                                    <tr>
                                        <th>ID</th>
                                        <th>Domain</th>
                                        <th>Type</th>
                                        <th>Status</th>
                                        <th>Description</th>
                                        <th>Group</th>
                                        <th>Actions</th> </tr>
                                </thead>
                                <tbody>
                                    <?php foreach ($domains as $domain): ?>
                                    <tr>
                                        <td><?php echo htmlspecialchars($domain['id'] ?? 'N/A'); ?></td>
                                        <td><?php echo htmlspecialchars($domain['domain'] ?? 'N/A'); ?></td>
                                        <td><?php echo htmlspecialchars(ucfirst($domain['type'] ?? 'N/A')); ?></td>
                                        <td><?php echo htmlspecialchars(ucfirst($domain['status'] ?? 'N/A')); ?></td>
                                        <td><?php echo htmlspecialchars($domain['description'] ?? 'N/A'); ?></td>
                                        <td><?php echo htmlspecialchars($domain['group'] ?? 'N/A'); ?></td>
                                        <td class="action-buttons">
                                            <button class="btn btn-info btn-sm edit-domain-btn"
                                                    data-bs-toggle="modal" data-bs-target="#editDomainModal"
                                                    data-id="<?php echo htmlspecialchars($domain['id']); ?>"
                                                    data-domain="<?php echo htmlspecialchars($domain['domain']); ?>"
                                                    data-type="<?php echo htmlspecialchars($domain['type']); ?>"
                                                    data-description="<?php echo htmlspecialchars($domain['description']); ?>"
                                                    data-group="<?php echo htmlspecialchars($domain['group']); ?>">
                                                <i class="fas fa-edit"></i> 
                                            </button>
                                            <button class="btn btn-danger btn-sm delete-domain-btn"
                                                    data-bs-toggle="modal" data-bs-target="#deleteDomainModal"
                                                    data-id="<?php echo htmlspecialchars($domain['id']); ?>"
                                                    data-domain="<?php echo htmlspecialchars($domain['domain']); ?>">
                                                <i class="fas fa-trash"></i> 
                                            </button>
                                            <button class="btn btn-success btn-sm view-records-btn"
                                                    data-bs-toggle="modal" data-bs-target="#viewRecordsModal"
                                                    data-domain-id="<?php echo htmlspecialchars($domain['id']); ?>"
                                                    data-domain-name="<?php echo htmlspecialchars($domain['domain']); ?>">
                                                <i class="fas fa-list"></i> 
                                            </button>
                                        </td>
                                    </tr>
                                    <?php endforeach; ?>
                                </tbody>
                            </table>
                        <?php endif; ?>

                    </div>
                </div>
            </main>
        </div>
    </div>

    <div class="modal fade" id="addDomainModal" tabindex="-1" aria-labelledby="addDomainModalLabel" aria-hidden="true" style="color: #000">
        <div class="modal-dialog">
            <div class="modal-content">
                <div class="modal-header">
                    <h5 class="modal-title" id="addDomainModalLabel">Add New Domain</h5>
                    <button type="button" class="btn-close" data-bs-dismiss="modal" aria-label="Close"></button>
                </div>
                <form id="addDomainForm">
                    <div class="modal-body">
                        <div class="mb-3">
                            <label for="newDomainName" class="form-label">Domain Name</label>
                            <input type="text" class="form-control" id="newDomainName" name="domain" required>
                        </div>
                        <div class="mb-3">
                            <label for="newDomainType" class="form-label">Type</label>
                            <select class="form-select" id="newDomainType" name="type" required>
                                <option value="master">Master</option>
                                <option value="slave">Slave</option>
                            </select>
                        </div>
                        <div class="mb-3" id="soaEmailField" style="display: none;">
                            <label for="newSoaEmail" class="form-label">SOA Email (Required for Master)</label>
                            <input type="email" class="form-control" id="newSoaEmail" name="soa_email" placeholder="e.g., admin@example.com">
                        </div>
                        <div class="mb-3">
                            <label for="newDomainDescription" class="form-label">Description (Optional)</label>
                            <input type="text" class="form-control" id="newDomainDescription" name="description" maxlength="50">
                        </div>
                        <div class="mb-3">
                            <label for="newDomainGroup" class="form-label">Group (Optional)</label>
                            <input type="text" class="form-control" id="newDomainGroup" name="group" maxlength="253">
                        </div>
                        <div class="mb-3" id="masterIpsField" style="display: none;">
                            <label for="newMasterIps" class="form-label">Master IPs (Comma-separated for Slave)</label>
                            <input type="text" class="form-control" id="newMasterIps" name="master_ips" placeholder="e.g., 192.0.2.1,192.0.2.2">
                            <div class="form-text">Required for Slave domains.</div>
                        </div>
                    </div>
                    <div class="modal-footer">
                        <button type="button" class="btn btn-secondary" data-bs-dismiss="modal">Close</button>
                        <button type="submit" class="btn btn-primary">Add Domain</button>
                    </div>
                </form>
            </div>
        </div>
    </div>

    <div class="modal fade" id="editDomainModal" tabindex="-1" aria-labelledby="editDomainModalLabel" aria-hidden="true" style="color: #000">
        <div class="modal-dialog">
            <div class="modal-content">
                <div class="modal-header">
                    <h5 class="modal-title" id="editDomainModalLabel">Edit Domain</h5>
                    <button type="button" class="btn-close" data-bs-dismiss="modal" aria-label="Close"></button>
                </div>
                <form id="editDomainForm">
                    <div class="modal-body">
                        <input type="hidden" id="editDomainId" name="domain_id">
                        <div class="mb-3">
                            <label for="editDomainName" class="form-label">Domain Name</label>
                            <input type="text" class="form-control" id="editDomainName" name="domain" required readonly>
                        </div>
                        <div class="mb-3">
                            <label for="editDomainType" class="form-label">Type</label>
                            <select class="form-select" id="editDomainType" name="type" required>
                                <option value="master">Master</option>
                                <option value="slave">Slave</option>
                            </select>
                        </div>
                        <div class="mb-3" id="editSoaEmailField" style="display: none;">
                            <label for="editSoaEmail" class="form-label">SOA Email (Required for Master)</label>
                            <input type="email" class="form-control" id="editSoaEmail" name="soa_email" placeholder="e.g., admin@example.com">
                        </div>
                        <div class="mb-3">
                            <label for="editDomainDescription" class="form-label">Description (Optional)</label>
                            <input type="text" class="form-control" id="editDomainDescription" name="description" maxlength="50">
                        </div>
                        <div class="mb-3">
                            <label for="editDomainGroup" class="form-label">Group (Optional)</label>
                            <input type="text" class="form-control" id="editDomainGroup" name="group" maxlength="253">
                        </div>
                        <div class="mb-3" id="editMasterIpsField" style="display: none;">
                            <label for="editMasterIps" class="form-label">Master IPs (Comma-separated for Slave)</label>
                            <input type="text" class="form-control" id="editMasterIps" name="master_ips" placeholder="e.g., 192.0.2.1,192.0.2.2">
                            <div class="form-text">Required for Slave domains.</div>
                        </div>
                    </div>
                    <div class="modal-footer">
                        <button type="button" class="btn btn-secondary" data-bs-dismiss="modal">Close</button>
                        <button type="submit" class="btn btn-primary">Save Changes</button>
                    </div>
                </form>
            </div>
        </div>
    </div>

    <div class="modal fade" id="deleteDomainModal" tabindex="-1" aria-labelledby="deleteDomainModalLabel" aria-hidden="true" style="color: #000">
        <div class="modal-dialog">
            <div class="modal-content">
                <div class="modal-header">
                    <h5 class="modal-title" id="deleteDomainModalLabel">Confirm Delete Domain</h5>
                    <button type="button" class="btn-close" data-bs-dismiss="modal" aria-label="Close"></button>
                </div>
                <div class="modal-body">
                    Are you sure you want to delete domain <strong id="deleteDomainName"></strong> (ID: <span id="deleteDomainIdSpan"></span>)?
                    This action cannot be undone.
                    <input type="hidden" id="confirmDeleteDomainId">
                </div>
                <div class="modal-footer">
                    <button type="button" class="btn btn-secondary" data-bs-dismiss="modal">Cancel</button>
                    <button type="button" class="btn btn-danger" id="confirmDeleteDomainBtn">Delete Domain</button>
                </div>
            </div>
        </div>
    </div>

    <div class="modal fade" id="viewRecordsModal" tabindex="-1" aria-labelledby="viewRecordsModalLabel" aria-hidden="true" style="color: #000">
        <div class="modal-dialog modal-xl">
            <div class="modal-content">
                <div class="modal-header">
                    <h5 class="modal-title" id="viewRecordsModalLabel">DNS Records for <span id="recordsDomainName"></span></h5>
                    <button type="button" class="btn-close" data-bs-dismiss="modal" aria-label="Close"></button>
                </div>
                <div class="modal-body">
                    <input type="hidden" id="recordsDomainId">
                    <button type="button" class="btn btn-success btn-sm mb-3" data-bs-toggle="modal" data-bs-target="#addRecordModal" id="openAddRecordModalFromView">
                        <i class="fas fa-plus-circle"></i> Add New Record
                    </button>
                    <table id="recordsTable" class="table table-striped table-hover w-100" style="background-color:#FFF;">
                        <thead>
                            <tr>
                                <th>ID</th>
                                <th>Type</th>
                                <th>Name</th>
                                <th>Target</th>
                                <th>TTL</th>
                                <th>Priority/Weight/Port</th>
                                <th>Actions</th>
                            </tr>
                        </thead>
                        <tbody>
                            </tbody>
                    </table>
                    <div id="recordsLoadingError" class="alert alert-warning" style="display:none;"></div>
                </div>
                <div class="modal-footer">
                    <button type="button" class="btn btn-secondary" data-bs-dismiss="modal">Close</button>
                </div>
            </div>
        </div>
    </div>

    <div class="modal fade" id="addRecordModal" tabindex="-1" aria-labelledby="addRecordModalLabel" aria-hidden="true" style="color: #000">
        <div class="modal-dialog">
            <div class="modal-content">
                <div class="modal-header">
                    <h5 class="modal-title" id="addRecordModalLabel">Add DNS Record for <span id="addRecordDomainName"></span></h5>
                    <button type="button" class="btn-close" data-bs-dismiss="modal" aria-label="Close"></button>
                </div>
                <form id="addRecordForm">
                    <div class="modal-body">
                        <input type="hidden" id="addRecordDomainId" name="domain_id">
                        <div class="mb-3">
                            <label for="newRecordType" class="form-label">Record Type</label>
                            <select class="form-select" id="newRecordType" name="type" required>
                                <option value="A">A (Address)</option>
                                <option value="AAAA">AAAA (IPv6 Address)</option>
                                <option value="CNAME">CNAME (Canonical Name)</option>
                                <option value="MX">MX (Mail Exchange)</option>
                                <option value="NS">NS (Name Server)</option>
                                <option value="TXT">TXT (Text)</option>
                                <option value="SRV">SRV (Service)</option>
                                <option value="PTR">PTR (Pointer)</option>
                                <option value="CAA">CAA (Certificate Authority Authorization)</option>
                            </select>
                        </div>
                        <div class="mb-3">
                            <label for="newRecordName" class="form-label">Hostname (e.g., www, mail, @ for root)</label>
                            <input type="text" class="form-control" id="newRecordName" name="name" placeholder="@ for root domain">
                        </div>
                        <div class="mb-3">
                            <label for="newRecordTarget" class="form-label">Target (IP, Hostname, or Text)</label>
                            <input type="text" class="form-control" id="newRecordTarget" name="target" required>
                        </div>
                        <div class="mb-3">
                            <label for="newRecordTTL" class="form-label">TTL (Time To Live in seconds)</label>
                            <input type="number" class="form-control" id="newRecordTTL" name="ttl_sec" value="300" min="0">
                        </div>
                        <div class="mb-3" id="mxPriorityField" style="display: none;">
                            <label for="newRecordMXPriority" class="form-label">MX Priority</label>
                            <input type="number" class="form-control" id="newRecordMXPriority" name="mx_priority" value="10">
                        </div>
                        <div class="mb-3" id="srvFields" style="display: none;">
                            <div class="row">
                                <div class="col-4">
                                    <label for="newRecordSRVPriority" class="form-label">Priority</label>
                                    <input type="number" class="form-control" id="newRecordSRVPriority" name="srv_priority" value="0">
                                </div>
                                <div class="col-4">
                                    <label for="newRecordSRVWeight" class="form-label">Weight</label>
                                    <input type="number" class="form-control" id="newRecordSRVWeight" name="srv_weight" value="5">
                                </div>
                                <div class="col-4">
                                    <label for="newRecordSRVPort" class="form-label">Port</label>
                                    <input type="number" class="form-control" id="newRecordSRVPort" name="srv_port" value="80">
                                </div>
                            </div>
                        </div>
                    </div>
                    <div class="modal-footer">
                        <button type="button" class="btn btn-secondary" data-bs-dismiss="modal">Close</button>
                        <button type="submit" class="btn btn-primary">Add Record</button>
                    </div>
                </form>
            </div>
        </div>
    </div>

    <div class="modal fade" id="editRecordModal" tabindex="-1" aria-labelledby="editRecordModalLabel" aria-hidden="true" style="color: #000">
        <div class="modal-dialog">
            <div class="modal-content">
                <div class="modal-header">
                    <h5 class="modal-title" id="editRecordModalLabel">Edit DNS Record for <span id="editRecordDomainName"></span></h5>
                    <button type="button" class="btn-close" data-bs-dismiss="modal" aria-label="Close"></button>
                </div>
                <form id="editRecordForm">
                    <div class="modal-body">
                        <input type="hidden" id="editRecordDomainId" name="domain_id">
                        <input type="hidden" id="editRecordId" name="record_id">
                        <div class="mb-3">
                            <label for="editRecordType" class="form-label">Record Type</label>
                            <select class="form-select" id="editRecordType" name="type" required>
                                <option value="A">A (Address)</option>
                                <option value="AAAA">AAAA (IPv6 Address)</option>
                                <option value="CNAME">CNAME (Canonical Name)</option>
                                <option value="MX">MX (Mail Exchange)</option>
                                <option value="NS">NS (Name Server)</option>
                                <option value="TXT">TXT (Text)</option>
                                <option value="SRV">SRV (Service)</option>
                                <option value="PTR">PTR (Pointer)</option>
                                <option value="CAA">CAA (Certificate Authority Authorization)</option>
                            </select>
                        </div>
                        <div class="mb-3">
                            <label for="editRecordName" class="form-label">Hostname (e.g., www, mail, @ for root)</label>
                            <input type="text" class="form-control" id="editRecordName" name="name" placeholder="@ for root domain">
                        </div>
                        <div class="mb-3">
                            <label for="editRecordTarget" class="form-label">Target (IP, Hostname, or Text)</label>
                            <input type="text" class="form-control" id="editRecordTarget" name="target" required>
                        </div>
                        <div class="mb-3">
                            <label for="editRecordTTL" class="form-label">TTL (Time To Live in seconds)</label>
                            <input type="number" class="form-control" id="editRecordTTL" name="ttl_sec" min="0">
                        </div>
                        <div class="mb-3" id="editMxPriorityField" style="display: none;">
                            <label for="editRecordMXPriority" class="form-label">MX Priority</label>
                            <input type="number" class="form-control" id="editRecordMXPriority" name="mx_priority">
                        </div>
                        <div class="mb-3" id="editSrvFields" style="display: none;">
                            <div class="row">
                                <div class="col-4">
                                    <label for="editRecordSRVPriority" class="form-label">Priority</label>
                                    <input type="number" class="form-control" id="editRecordSRVPriority" name="srv_priority">
                                </div>
                                <div class="col-4">
                                    <label for="editRecordSRVWeight" class="form-label">Weight</label>
                                    <input type="number" class="form-control" id="editRecordSRVWeight" name="srv_weight">
                                </div>
                                <div class="col-4">
                                    <label for="editRecordSRVPort" class="form-label">Port</label>
                                    <input type="number" class="form-control" id="editRecordSRVPort" name="srv_port">
                                </div>
                            </div>
                        </div>
                    </div>
                    <div class="modal-footer">
                        <button type="button" class="btn btn-secondary" data-bs-dismiss="modal">Close</button>
                        <button type="submit" class="btn btn-primary">Save Changes</button>
                    </div>
                </form>
            </div>
        </div>
    </div>


    <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.3.0-alpha3/dist/js/bootstrap.bundle.min.js"
        xintegrity="sha384-ENjdO4Dr2bkBIFxQpeoTz1HIcje39Wm4jDKdf19U8gI4ddQ3GYNS7NTKfAdVQSZe"
        crossorigin="anonymous"></script>
    <script src="assets/js/script.js"></script>

    <script src="https://code.jquery.com/jquery-3.7.1.min.js" integrity="sha256-/JqT3SQfawRcv/BIHPThkBvs0OEvtFFmqPF/lYI/Cxo=" crossorigin="anonymous"></script>
    <script type="text/javascript" src="https://cdn.datatables.net/2.0.8/js/dataTables.min.js"></script>
    <script type="text/javascript" src="https://cdn.datatables.net/2.0.8/js/dataTables.bootstrap5.min.js"></script>

    <script>
        $(document).ready(function() {
            var domainsTable = $('#domainsTable').DataTable({
                "order": [[ 1, "asc" ]] // Order by Domain name initially
            });

            var recordsTable = null; // Initialize records DataTable as null

            // Helper function to refresh table data
            function refreshTable() {
                window.location.reload();
            }

            // Function to fetch and display records for a domain
            function fetchRecordsForDomain(domainId, domainName) {
                if (recordsTable) {
                    recordsTable.destroy(); // Destroy existing DataTable instance
                    recordsTable = null; // Reset
                }
                $('#recordsTable tbody').empty(); // Clear existing rows
                $('#recordsLoadingError').hide().text(''); // Hide previous errors

                $.ajax({
                    url: 'ajax/linode_ajax_records.php', // Assuming a new file for fetching records
                    type: 'GET',
                    data: { action: 'listRecords', domain_id: domainId },
                    dataType: 'json',
                    success: function(response) {
                        if (response.success) {
                            // Corrected: Access the actual records array from response.data.data
                            const records = response.data.data;
                            let recordsHtml = '';
                            // Ensure records is an array before iterating
                            if (Array.isArray(records)) {
                                records.forEach(record => {
                                    let priorityWeightPort = '';
                                    if (record.type === 'MX') {
                                        priorityWeightPort = `Priority: ${record.mx_priority ?? 'N/A'}`;
                                    } else if (record.type === 'SRV') {
                                        priorityWeightPort = `P: ${record.srv_priority ?? 'N/A'}, W: ${record.srv_weight ?? 'N/A'}, Port: ${record.srv_port ?? 'N/A'}`;
                                    }
                                    recordsHtml += `
                                        <tr>
                                            <td>${record.id}</td>
                                            <td>${record.type}</td>
                                            <td>${record.name === null ? '@' : record.name}</td>
                                            <td>${record.target}</td>
                                            <td>${record.ttl_sec}</td>
                                            <td>${priorityWeightPort}</td>
                                            <td>
                                                <button class="btn btn-sm btn-info edit-record-btn"
                                                        data-bs-toggle="modal" data-bs-target="#editRecordModal"
                                                        data-domain-id="${domainId}"
                                                        data-domain-name="${domainName}"
                                                        data-record-id="${record.id}"
                                                        data-type="${record.type}"
                                                        data-name="${record.name === null ? '@' : record.name}"
                                                        data-target="${record.target}"
                                                        data-ttl="${record.ttl_sec}"
                                                        data-mx-priority="${record.mx_priority ?? ''}"
                                                        data-srv-priority="${record.srv_priority ?? ''}"
                                                        data-srv-weight="${record.srv_weight ?? ''}"
                                                        data-srv-port="${record.srv_port ?? ''}">
                                                    <i class="fas fa-edit"></i> 
                                                </button>
                                                <button class="btn btn-sm btn-danger delete-record-btn"
                                                        data-bs-toggle="modal" data-bs-target="#deleteRecordModal"
                                                        data-domain-id="${domainId}"
                                                        data-domain-name="${domainName}"
                                                        data-record-id="${record.id}"
                                                        data-record-name="${record.name === null ? '@' : record.name}">
                                                    <i class="fas fa-trash"></i> 
                                                </button>
                                            </td>
                                        </tr>
                                    `;
                                });
                                $('#recordsTable tbody').html(recordsHtml);

                                recordsTable = $('#recordsTable').DataTable({
                                    "order": [[ 1, "asc" ]], // Order by Type initially
                                    "destroy": true // Allow reinitialization
                                });
                            } else {
                                console.error("Expected an array of records, but received:", records);
                                $('#recordsLoadingError').text('Unexpected data format for records.').show();
                            }
                        } else {
                            $('#recordsLoadingError').text('Error loading records: ' + response.message).show();
                        }
                    },
                    error: function(xhr, status, error) {
                        $('#recordsLoadingError').text('AJAX Error loading records: ' + xhr.responseText).show();
                    }
                });
            }

            // --- Add Domain Logic ---
            function toggleAddDomainSoaAndMasterIpsFields() {
                const domainType = $('#newDomainType').val();
                if (domainType === 'slave') {
                    $('#masterIpsField').show();
                    $('#newMasterIps').prop('required', true);
                    $('#soaEmailField').hide();
                    $('#newSoaEmail').prop('required', false).val(''); // Clear and un-require SOA for slave
                } else { // master
                    $('#masterIpsField').hide();
                    $('#newMasterIps').prop('required', false).val(''); // Clear and un-require master IPs for master
                    $('#soaEmailField').show();
                    $('#newSoaEmail').prop('required', true);
                }
            }

            $('#newDomainType').change(toggleAddDomainSoaAndMasterIpsFields);

            $('#addDomainForm').submit(function(e) {
                e.preventDefault();
                const formData = $(this).serializeArray();
                formData.push({name: 'action', value: 'addDomain'});

                $.ajax({
                    url: 'ajax/linode_ajax.php',
                    type: 'POST',
                    data: formData,
                    dataType: 'json',
                    success: function(response) {
                        if (response.success) {
                            alert('Domain added successfully: ' + response.message);
                            $('#addDomainModal').modal('hide');
                            refreshTable();
                        } else {
                            alert('Error adding domain: ' + response.message);
                        }
                    },
                    error: function(xhr, status, error) {
                        alert('AJAX Error: ' + xhr.responseText);
                    }
                });
            });

            // --- Edit Domain Logic ---
            function toggleEditDomainSoaAndMasterIpsFields() {
                const domainType = $('#editDomainType').val();
                if (domainType === 'slave') {
                    $('#editMasterIpsField').show();
                    $('#editMasterIps').prop('required', true);
                    $('#editSoaEmailField').hide();
                    $('#editSoaEmail').prop('required', false).val(''); // Clear and un-require SOA for slave
                } else { // master
                    $('#editMasterIpsField').hide();
                    $('#editMasterIps').prop('required', false).val(''); // Clear and un-require master IPs for master
                    $('#editSoaEmailField').show();
                    $('#editSoaEmail').prop('required', true);
                }
            }

            $('#domainsTable tbody').on('click', '.edit-domain-btn', function() {
                const id = $(this).data('id');
                const domain = $(this).data('domain');
                const type = $(this).data('type');
                const description = $(this).data('description');
                const group = $(this).data('group');
                // SOA email and Master IPs are not passed via data attributes for edit
                // as they might not be readily available on initial domain fetch.
                // For a complete solution, you'd fetch specific domain details via API here.

                $('#editDomainId').val(id);
                $('#editDomainName').val(domain);
                $('#editDomainType').val(type);
                $('#editDomainDescription').val(description);
                $('#editDomainGroup').val(group);

                // Trigger the visibility toggle for SOA and Master IPs
                // This will set requirements based on the loaded type
                toggleEditDomainSoaAndMasterIpsFields();
            });

            $('#editDomainType').change(toggleEditDomainSoaAndMasterIpsFields);


            $('#editDomainForm').submit(function(e) {
                e.preventDefault();
                const formData = $(this).serializeArray();
                formData.push({name: 'action', value: 'editDomain'});

                $.ajax({
                    url: 'ajax/linode_ajax.php',
                    type: 'POST', // Linode API uses PUT for update, but we'll send as POST to our PHP handler
                    data: formData,
                    dataType: 'json',
                    success: function(response) {
                        if (response.success) {
                            alert('Domain updated successfully: ' + response.message);
                            $('#editDomainModal').modal('hide');
                            refreshTable();
                        } else {
                            alert('Error updating domain: ' + response.message);
                        }
                    },
                    error: function(xhr, status, error) {
                        alert('AJAX Error: ' + xhr.responseText);
                    }
                });
            });

            // --- Delete Domain Logic ---
            $('#domainsTable tbody').on('click', '.delete-domain-btn', function() {
                const id = $(this).data('id');
                const domain = $(this).data('domain');
                $('#deleteDomainName').text(domain);
                $('#deleteDomainIdSpan').text(id);
                $('#confirmDeleteDomainId').val(id);
            });

            $('#confirmDeleteDomainBtn').click(function() {
                const domainId = $('#confirmDeleteDomainId').val();
                $.ajax({
                    url: 'ajax/linode_ajax.php',
                    type: 'POST', // Send as POST to our PHP handler
                    data: { action: 'deleteDomain', domain_id: domainId },
                    dataType: 'json',
                    success: function(response) {
                        if (response.success) {
                            alert('Domain deleted successfully: ' + response.message);
                            $('#deleteDomainModal').modal('hide');
                            refreshTable();
                        } else {
                            alert('Error deleting domain: ' + response.message);
                        }
                    },
                    error: function(xhr, status, error) {
                        alert('AJAX Error: ' + xhr.responseText);
                    }
                });
            });

            // --- View Records Logic ---
            $('#domainsTable tbody').on('click', '.view-records-btn', function() {
                const domainId = $(this).data('domain-id');
                const domainName = $(this).data('domain-name');
                $('#recordsDomainId').val(domainId);
                $('#recordsDomainName').text(domainName);
                $('#openAddRecordModalFromView').data('domain-id', domainId); // Pass domain_id to add record button
                $('#openAddRecordModalFromView').data('domain-name', domainName); // Pass domain_name to add record button

                fetchRecordsForDomain(domainId, domainName); // Call function to fetch records
            });


            // --- Add Record Logic (from View Records Modal) ---
            // This event listener ensures the add record modal gets the correct domain info
            $('#openAddRecordModalFromView').on('click', function() {
                const domainId = $(this).data('domain-id');
                const domainName = $(this).data('domain-name');
                $('#addRecordDomainId').val(domainId);
                $('#addRecordDomainName').text(domainName);
                // Reset form fields for new record
                $('#addRecordForm')[0].reset();
                $('#newRecordType').val('A'); // Default to A record
                $('#mxPriorityField').hide();
                $('#srvFields').hide();
            });

            $('#newRecordType').change(function() {
                const selectedType = $(this).val();
                $('#mxPriorityField').hide();
                $('#srvFields').hide();

                if (selectedType === 'MX') {
                    $('#mxPriorityField').show();
                } else if (selectedType === 'SRV') {
                    $('#srvFields').show();
                }
            });

            $('#addRecordForm').submit(function(e) {
                e.preventDefault();
                const formData = $(this).serializeArray();
                formData.push({name: 'action', value: 'addRecord'});

                $.ajax({
                    url: 'ajax/linode_ajax.php',
                    type: 'POST',
                    data: formData,
                    dataType: 'json',
                    success: function(response) {
                        if (response.success) {
                            alert('Record added successfully: ' + response.message);
                            $('#addRecordModal').modal('hide');
                            // Re-fetch records for the currently viewed domain after adding
                            const currentDomainId = $('#recordsDomainId').val();
                            const currentDomainName = $('#recordsDomainName').text();
                            fetchRecordsForDomain(currentDomainId, currentDomainName);
                        } else {
                            alert('Error adding record: ' + response.message);
                        }
                    },
                    error: function(xhr, status, error) {
                        alert('AJAX Error: ' + xhr.responseText);
                    }
                });
            });

            // --- Edit Record Logic ---
            function toggleEditRecordTypeSpecificFields() {
                const selectedType = $('#editRecordType').val();
                $('#editMxPriorityField').hide();
                $('#editSrvFields').hide();

                if (selectedType === 'MX') {
                    $('#editMxPriorityField').show();
                } else if (selectedType === 'SRV') {
                    $('#editSrvFields').show();
                }
            }

            // Listener for "Edit" button within the records table (in viewRecordsModal)
            $('#recordsTable tbody').on('click', '.edit-record-btn', function() {
                const domainId = $(this).data('domain-id');
                const domainName = $(this).data('domain-name');
                const recordId = $(this).data('record-id');
                const type = $(this).data('type');
                const name = $(this).data('name');
                const target = $(this).data('target');
                const ttl = $(this).data('ttl');
                const mxPriority = $(this).data('mx-priority');
                const srvPriority = $(this).data('srv-priority');
                const srvWeight = $(this).data('srv-weight');
                const srvPort = $(this).data('srv-port');

                $('#editRecordDomainId').val(domainId);
                $('#editRecordDomainName').text(domainName);
                $('#editRecordId').val(recordId);
                $('#editRecordType').val(type);
                $('#editRecordName').val(name);
                $('#editRecordTarget').val(target);
                $('#editRecordTTL').val(ttl);
                $('#editRecordMXPriority').val(mxPriority);
                $('#editRecordSRVPriority').val(srvPriority);
                $('#editRecordSRVWeight').val(srvWeight);
                $('#editRecordSRVPort').val(srvPort);

                toggleEditRecordTypeSpecificFields(); // Set initial visibility
            });

            // Listener for type change within editRecordModal
            $('#editRecordType').change(toggleEditRecordTypeSpecificFields);

            $('#editRecordForm').submit(function(e) {
                e.preventDefault();
                const formData = $(this).serializeArray();
                formData.push({name: 'action', value: 'editRecord'});

                $.ajax({
                    url: 'ajax/linode_ajax.php',
                    type: 'POST', // Send as POST to our PHP handler
                    data: formData,
                    dataType: 'json',
                    success: function(response) {
                        if (response.success) {
                            alert('Record updated successfully: ' + response.message);
                            $('#editRecordModal').modal('hide');
                            // Re-fetch records for the currently viewed domain after editing
                            const currentDomainId = $('#recordsDomainId').val();
                            const currentDomainName = $('#recordsDomainName').text();
                            fetchRecordsForDomain(currentDomainId, currentDomainName);
                        } else {
                            alert('Error updating record: ' + response.message);
                        }
                    },
                    error: function(xhr, status, error) {
                        alert('AJAX Error: ' + xhr.responseText);
                    }
                });
            });

            // --- Initial states for modals ---
            toggleAddDomainSoaAndMasterIpsFields(); // Set initial state for Add Domain modal
            // For edit domain modal, the fields are toggled when the edit button is clicked.
            // For add record modal, fields are toggled on type change.
            // For edit record modal, fields are toggled when edit record button is clicked.

        });
    </script>
</body>

</html>