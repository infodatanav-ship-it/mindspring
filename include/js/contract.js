
function getValues(){

	var extra = parseFloat(document.getElementById("ex").value);
	var borrowed =parseFloat(document.getElementById("brwd").value);
	var borrowedpre=parseFloat(document.getElementById("brwdpre").value);
	//var unusedpre=parseFloat(document.getElementById("unus").value);
	var unusedpre=parseFloat(document.getElementById("previous").value);
	var invpre=parseFloat(document.getElementById("invpre").value);
	var offpre=parseFloat(document.getElementById("off").value);
	var deduct=parseFloat(document.getElementById("deduct2").value);
	var unused =parseFloat(document.getElementById("carried").value);
	var effective = parseFloat(document.getElementById("ehours").value);

	if(isNaN(invpre) == "true"){
		invpre = 0;
	}
	if(isNaN(extra) == "true"){
		extra = 0;
	}
	if(isNaN(borrowedpre) == "true"){
		borrowedpre = 0;
	}
	if(isNaN(borrowed) == "true"){
		borrowed = 0;
	}
	if(isNaN(invpre) == "true"){
		invpre = 0;
	}
	if(isNaN(offpre) == "true"){
		offpre = 0;
	}
	if(isNaN(off) == "true"){
		off = 0;
	}

	if(isNaN(deduct) == "true"){
		deduct = 0;
	}
	if(isNaN(borrowed) == "true"){
		alert('enter a valid number');
	} 

	if (unusedpre < 0){
		unusedpre = 0;
	}

	var totalValue =(borrowed + unusedpre + deduct + unused)-(extra + borrowedpre );

	if(effective < 0){
		var totalValue3 = borrowed - (effective*-1);
	} else if (effective >= 0) {
		totalValue3 = effective - borrowed;
	} else { totalValue3 = 0;}

	if(totalValue3 < 0){
		totalValue3 = totalValue3 *(-1);
	} else {
		totalValue3 = 0;
	}

	var totalValue2 = borrowed + unusedpre + unused  - extra - borrowedpre ;


	if (totalValue < 0 ) {
		totalValue = totalValue*(-1);
	} else (totalValue = 0);

	if(isNaN(totalValue) == "true"){
		alert('enter a valid number');
	}
	//totalValue = parseFloat(totalValue).toFixed(2);
	//document.getElementById("invoiced").value = totalValue;
	totalValue3 = parseFloat(totalValue3).toFixed(2);
	document.getElementById("invoiced").value = totalValue3;

	var hours_left = 0 ;

	if (totalValue2 < 0 ) {
		totalValue2 = totalValue2*(-1);
		hours_left =  deduct - totalValue2;
		if (hours_left < 0) {
			hours_left = 0;
		}
	} else{
		hours_left =  deduct;
	}

	hours_left = parseFloat(hours_left).toFixed(2);
	document.getElementById("hours_left").value = hours_left;

	var prev = 0;
	if (unused > 0){
		prev = unused - (borrowedpre);
	}

	if(prev < 0){
		prev = 0;
	}

	prev = parseFloat(prev).toFixed(2);
	document.getElementById("previous").value = prev;
}
