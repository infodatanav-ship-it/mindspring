$(document).ready(function() {

	$('input[name="datefilter"]').daterangepicker({
		autoUpdateInput: false,
		locale: {
			cancelLabel: 'Clear'
		}
	});

	$('input[name="datefilter"]').on('apply.daterangepicker', function(ev, picker) {
		$(this).val(picker.startDate.format('MM/DD/YYYY') + ' - ' + picker.endDate.format('MM/DD/YYYY'));
		console.log(picker.startDate.format('DD-MM-YYYY'));
		console.log(picker.endDate.format('DD-MM-YYYY'));
	});

	$('input[name="datefilter"]').on('cancel.daterangepicker', function(ev, picker) {
		$(this).val('');
	});

	 $('input[name="send_type"]').on('change', function () {

		console.log('Send Type:', $(this).val());

		if ( $(this).val() === 'normal' ) {
			$('#email-list').val('');
			$('#email-list').prop('disabled', $(this).attr('id') === 'normal');
		} else {
			$('#email-list').prop('disabled', false);
		}

	});

	$(document).on('click', '.send-report', function() {

		// Open modal trigger for sending report
		$('.modal-overlay, .modal').css('display', 'block');

		$('.modal-title').html('Send Report'); // Set the modal header
		$('.modal-body-table').empty(); // Clear previous content

		// disable input with id send_type
		$('#email-list').prop('disabled', true);

		// set radio button by id 
		// $('#normal').prop('checked', true);

	});


	// Handle form submission for sending report
	$('#send-this-report').on('click', function(e) {

		e.preventDefault(); // Prevent default form submission

		console.log('Form submitted');

		let emailAddress = $('#email-list').val();
		let sendTypeValue;

		// group identified by name attribute
		if ( $('input[name="send_type"]:checked').length === 0 ) {
			// none of the radios in the group are selected
			console.log('No option chosen yet.');
			alert('Please select the type of send.');
			return;

		} else {

			sendTypeValue = $('input[name="send_type"]:checked').val();
			console.log('Selected value:', sendTypeValue);

			if ( emailAddress === '' || emailAddress === null || emailAddress === undefined || emailAddress.length === 0 ) {
				console.log('empty email list');
				console.log(sendTypeValue);
				if ( sendTypeValue === 'normal' ) {
					console.log('send type is normal')
					console.log('email will be automatically set');
				} else {
					alert('Email address can only be empty if the send type is `normal`.');
					return;
				}
			} else {

				console.log('email list is not empty');
				console.log(emailAddress)

				if ( sendTypeValue === 'testcc' || sendTypeValue === 'testnocc' ) {
					var email = $('#email-list').val().trim();
					if (!isValidEmail(email)) {
						alert('Please enter a valid e-mail address.');
						return;
					} else {
						console.log('Valid email:', email);
					}
				}

			}

		}

		console.log('Send Type:', sendTypeValue);
		console.log('Email:', emailAddress);

		// Here you can add the logic to send the email
		let doAjax = true; // Set to true if you want to send the email via AJAX

		if (!doAjax) {

			alert('Email sending is disabled for this demo.');
			return; // Exit if AJAX is not enabled

		} else {

			console.log('Report sent to: ' + emailAddress + ' with send type: ' + sendTypeValue);

			// $.ajax({
			// 	url: './include/weekly_email_v2.php', // Replace with your API endpoint
			// 	type: 'POST',
			// 	data: { emailaddr: emailAddress, send_type: sendTypeValue },
			// 	success: function(response) {
			// 		console.log('Response:', response);
			// 		$('.modal-overlay, .modal').css('display', 'none'); // Close modal
			// 	},
			// 	error: function() {
			// 		alert('Error sending report');
			// 	}
			// });

		}


		

	});


	// Open modal trigger
	$('.displayClients').click(function() {

		// $('.modal-overlay, .modal').fadeIn();
		$('.modal-overlay, .modal').css('display', 'block');
		let techie = $(this).data('id'); // Get the data-id attribute
		// You can use this data-id to fetch or display specific content in the modal
		console.log('Techie ID:', techie);
		// For example, you might want to load content dynamically based on this ID
		$('#modal-techie-name').text(techie); // Set the techie name in the modal header

		// get url
		// let url = $(this).data('url'); // If you have a URL to load content from
		// console.log('URL:', url);

		let currentUrl = window.location.href;
		console.log('URL: ', currentUrl);

		let params = new URL(document.location.toString()).searchParams;
		let year = params.get("year");
		let month = params.get("month");

		console.log(year);
		console.log(month);

		if ( year ) {

			console.log('it exists...');

		} else {

			console.log('it does not exists...');

			const today = new Date();
			const year = today.getFullYear();
			const month = today.getMonth() + 1; // Convert to 1-indexed (1=Jan, 12=Dec)

			console.log(month);
			console.log(year);

			console.log({ year, month }); // e.g., { year: 2023, month: 10 }

		}

		console.log('year: ', year);
		console.log('month: ', month);

		const qdates = getFirstAndLastDayFormatted(year, month);
		console.log(qdates.firstDay); // "2023-02-01"
		console.log(qdates.lastDay);  // "2023-02-28"

		// Example data to display in the modal
		// This should be replaced with actual data fetching logic
		// Fetch data from the server using AJAX
		// Assuming you have an API endpoint that returns the required data

		$.ajax({
			url: 'include/getCompanyHours.php', // Replace with your API endpoint
			type: 'GET',
			data: { techie_id: techie, firstDay: qdates.firstDay, lastDay: qdates.lastDay },
			success: function(response) {
				console.log('Response:', response);

				// Process the response and update the modal content
				// For example, if the response is in JSON format, you can parse it
				let data = response;

				$('.modal-body-table').empty(); // Clear previous content
				let dialogHTML = ''; // Initialize row variable

				dialogHTML = `<tr>
					<td>Company</td>
					<td>Hours Assigned</td>
					<td>Hours Used</td>
				</tr>`;

				data.forEach(function (value, index) {

					// console.log('index', index, 'Company:', value.company, 'Hours:', value.hours);

					row = `<tr>
						<td>${value.company}</td>
						<td>${value.hours}</td>
						<td>${value.hours_used}</td>
					</tr>`;

					dialogHTML += row; // Append each row to the dialogHTML

				});

				$('.modal-body-table').append(dialogHTML);

			},
			error: function() {
				alert('Error fetching data');
			}
		});

	});
	
	// Modal click handler
	$('.modal').click(function(e) {
		// Prevent clicks inside modal from closing it
		e.stopPropagation();
	});
	
	// Overlay click handler
	$('.modal-overlay').click(function() {

		// Only close if clicking directly on overlay (not bubbling from modal)

		// $(this).find('input[type="text"], textarea').val('');
		// $(this).find('input[type="checkbox"], input[type="radio"]').prop('checked', false);

		$('.modal-overlay, .modal').css('display', 'none');
		$('#send-report-form')[0].reset();

	});
	
	// Close button handler
	$('.modal-close-button, .modal-close-x').click(function() {

		// $('#send-report-form')[0].reset();

		// $(this).find('input[type="text"], textarea').val('');
		// $(this).find('input[type="checkbox"], input[type="radio"]').prop('checked', false);

		// $('.modal').find('form')[0].reset();
		// $('.modal-overlay, .modal').fadeOut();

		// 1. Reset a text input to its original (HTML value="...") value
		$('#email-list').val( $('#email-list').prop('defaultValue') );

		// 2. Reset one radio button to its original (HTML checked) state
		$('#send_type').prop('checked', $('#send_type').prop('defaultChecked'));

		$('.modal-overlay, .modal').css('display', 'none');

	});
});

function isValidEmail(email) {
	// basic RFC-style pattern
	var pattern = /^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$/;
	return pattern.test(email);
}

function getFirstAndLastDayFormatted(year, month) {

	const firstDay = new Date(year, month - 1, 1);
	const lastDay = new Date(year, month, 0);
	
	return {
		firstDay: firstDay.toISOString().split('T')[0], // YYYY-MM-DD
		lastDay: lastDay.toISOString().split('T')[0]
	};
}
