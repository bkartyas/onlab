var data =  { "pitch": { "type":   [], 
 			 			 "reward": [] }};

function resizePitch(){
	data.pitch.type = [];
	data.pitch.reward = [];
	var x = sizeX.value;
	var y = sizeY.value;

	while (pitch.firstChild) {
	    pitch.removeChild(pitch.firstChild);
	}

	for(i = 0; i < y; i++){
		data.pitch.type.push([]);
		data.pitch.reward.push([]);
		let newTR = document.createElement("tr");
		for(j = 0; j < x; j++){
			data.pitch.type[i].push("P");
			data.pitch.reward[i].push("0");
			newTR.appendChild(createTableData(j, i));
		}
		pitch.appendChild(newTR);	
	}

  	$("td").height($("td").width());
}

function createTableData(i, j){
	let newTD = document.createElement("td");
  	newTD.align = "center";
  	newTD.style["vertical-align"] = "middle";
	newTD.id = "pTD:" + i + ";" + j;
  	newTD.addEventListener("click", function(event) {
  		if(event.target.tagName === "TD"){
    		stepStatus(event.target);
    	}
	});

	let textBox = document.createElement("input");
	textBox.addEventListener("keydown", function(event) {	
		if (event.keyCode == 13) {
			var parsedID = event.target.id.split(":")[1].split(";");
			var x = parsedID[0];
			var y = parsedID[1];

			data.pitch.reward[y][x] = event.target.value;		
		}
	});
	textBox.style["font-size"] = "80%";
	textBox.size = "1";
	textBox.id = "pTB:" + i + ";" + j;
  	newTD.appendChild(textBox);
	newTD.style.backgroundColor = "white";

    //newTD.style.backgroundImage = "url('http://clipart-library.com/image_gallery/145213.jpg')";

	return newTD;
}

function stepStatus(tableData){
	var parsedID = tableData.id.split(":")[1].split(";");
	var x = parsedID[0];
	var y = parsedID[1];

	switch(tableData.style.backgroundColor){
	case "white":
		tableData.style.backgroundColor = "black";  
		data.pitch.type[y][x] = "W";
	break;
	case "black":
		tableData.style.backgroundColor = "blue";
		data.pitch.type[y][x] = "A";
	break;
	case "blue":
		tableData.style.backgroundColor = "yellow";         
		data.pitch.type[y][x] = "E";
	break;
	case "yellow":
		tableData.style.backgroundColor = "white";
		data.pitch.type[y][x] = "P";         
	break;
	default:
		tableData.style.backgroundColor = "white";         
		data.pitch.type[y][x] = "P";         
	}
}

$(document).ready(function(){

	resizePitch();
	var nextValue;
	
	
	startButton.addEventListener("click", function(){ 
		$.ajax({
			method: 'POST',
			url: '/start',
			data: data,
			dataType: 'json',
			success: function(data){
				console.log(data);
				nextValue = data;
			}
		});

		var intervalID = setInterval(function(){$.ajax({
			method: 'GET',
			url: '/next',
			dataType: 'json',
            contentType: "application/json; charset=utf-8",
			success: function(response){
						console.log(response);
						if(response['status'] === 'end'){
							clearInterval(intervalID);
						} else {
							var pitch = response.pitch;
							for(let i = 0; i < pitch.size; i++){
								for(let j = 0; j < pitch.size; j++){
									let field = pitch[i][j].split(";");
								}		
							}
						}
			}
	})}, 3000); });
	
});
