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
		data: { "a":"n", "b":"n" },
		dataType: 'json',
		success: function(data){
			console.log(data);
			nextValue = data;
		}
	});

	setInterval(function(){$.ajax({
		method: 'GET',
		url: '/next',
		dataType: 'json',
		success: function(data){
			console.log(data);
			phar.innerHTML=data;
		}
	})}, 3000);
	
});