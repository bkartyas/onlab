/*var x = 5, y = 5;

for(i = 0; i < x; i++){
	for(j = 0; j < x; j++){
		let newDiv = document.createElement("div");
		newDiv.className += 'boreder:thin';
		//pitch.appendChild(newDiv);
	}	
}*/
$(document).ready(function(){

	var nextValue;
	
	$.ajax({
		method: 'POST',
		url: '/start',
		data: { "size": { "x": 2, "y": 2 },
				"parameters": [[ "P;1", "P;2" ],
			   				   [ "P;3", "P;4" ]] },
		dataType: 'json',
		success: function(data){
			console.log(data);
			nextValue = data;
		}
	});

	setTimeout(function(){$.ajax({
		method: 'GET',
		url: '/next',
		dataType: 'json',
		success: function(data){
			console.log(data);
			phar.innerHTML=data[0][0];
		}
	})}, 5000);

	/*setInterval(function(){$.ajax({
		method: 'GET',
		url: '/next',
		dataType: 'json',
		success: function(data){
			console.log(data);
			phar.innerHTML=data;
		}
	})}, 3000);*/
	
});