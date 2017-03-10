var type =  [[]];
var maxQ;
var minQ;
var running = false;

function readData(){
	var x = sizeX.value;
	var y = sizeY.value;

	var data = { "pitch": { "type":   [],
 			 			    "reward": [] }};

	data.epoch = epoch.value;
	data.alpha = alpha.value;
	data.gamma = gamma.value;

	data.pitch.type = type;
	for(i = 0; i < y; i++){
		data.pitch.reward.push([]);
		for(j = 0; j < x; j++){
			var tb = "#pTB" + i + "_" + j;
			data.pitch.reward[i].push($(tb).val());
		}
	}

	return data;
}

function resizePitch(){
	type = [];
	var x = sizeX.value;
	var y = sizeY.value;

	while (pitch.firstChild) {
	    pitch.removeChild(pitch.firstChild);
	}

	for(i = 0; i < y; i++){
		type.push([]);
		let newTR = document.createElement("tr");
		for(j = 0; j < x; j++){
			type[i].push("P");
			newTR.appendChild(createTableData(j, i));
		}
		pitch.appendChild(newTR);
	}

  	$(".platform").height($(".platform").width());
  	$(".platform").width($(".platform").width());
}

function actionType(x, y){
	if(x === 0 && y === 1){ return "left"; }
	if(x === 1 && y === 0){ return "up"; }
	if(x === 2 && y === 1){ return "right"; }
	if(x === 1 && y === 2){ return "down"; }
}

function createTableData(x, y){
	let newTD = document.createElement("td");
	newTD.classList.add("platform");
	newTD.id = "pTD" + x + "_" + y;

	if(!running){
		newTD.addEventListener("click", function(event) {
				if(event.target.tagName === "TD"){
				stepStatus(event.target);
			}
		});

  		newTD.appendChild(createRewardInput());
	} else {
		let newTable = document.createElement("table");
		newTable.classList.add("action");
			for(let i = 0; i < 3; i++){
				let newTRin = document.createElement("tr");
				for(let j = 0; j < 3; j++){
					let newTD = document.createElement("td");
					newTD.id = "action" + actionType(j, i) + x + "_" + y;
					newTRin.appendChild(newTD);
				}
				newTable.appendChild(newTRin);
			}
			newTD.appendChild(newTable);
  	}

    //newTD.style.backgroundImage = "url('http://clipart-library.com/image_gallery/145213.jpg')";

	return newTD;
}

function createRewardInput(){
	let textBox = document.createElement("input");
	textBox.classList.add("reward");
	textBox.setAttribute("value", "10");
	textBox.id = "pTB" + i + "_" + j;

	return textBox;
}


/*function createUtilityView(){
	let newTable = document.createElement("table");
		newTable.classList.add("action");
			for(i = 0; i < 3; i++){
				let newTRin = document.createElement("tr");
				for(j = 0; j < 3; j++){
					let newTD = document.createElement("td");
					newTRin.appendChild(newTD);
				}
				newTable.appendChild(newTRin);
			}
	return newTable;
}*/

function stepStatus(tableData){
	var parsedID = tableData.id.split("pTD")[1].split("_");
	var x = parsedID[0];
	var y = parsedID[1];

	switch(tableData.style.backgroundColor){
	case "white":
		tableData.style.backgroundColor = "black";
		type[y][x] = "W";
		break;
	case "black":
		tableData.style.backgroundColor = "blue";
		type[y][x] = "A";
		break;
	case "blue":
		tableData.style.backgroundColor = "yellow";
		type[y][x] = "E";
		break;
	case "yellow":
		tableData.style.backgroundColor = "white";
		type[y][x] = "P";
		break;
	default:
		tableData.style.backgroundColor = "white";
		type[y][x] = "P";
	}
}

function getTypeColor(type){
	switch(type){
	case "P":
		return "white";
		break;
	case "W":
		return "black";
		break;
	case "A":
		return "blue";
		break;
	case "E":
		return "yellow";
		break;
	}
}

function setColor(id, color){
	var td = document.getElementById(id);
	td.style.backgroundColor = color;
}

function directionMap(index){
	switch(index){
		case 0:
			return "left";
			break;
		case 1:
			return "up";
			break;
		case 2:
			return "right";
			break;
		case 3:
			return "down";
			break;
	}
}

function GreenValue(value){
	rb = (((maxQ - minQ)/(value - minQ)) * 255);
	return 'rgb(' + rb + ',255,' + rb + ')';
}

function refreshPitch(pitchData){

	if(!maxQ && pitchData.length !== 0){ maxQ = pitchData[0].Qvalues[0]; };
	if(!minQ && pitchData.length !== 0){ minQ = pitchData[0].Qvalues[0]; };

	for(let i = 0; i < pitchData.length; i++){
		for(let j = 0; j < pitchData[i].Qvalues.length; j++){
			if(pitchData[i].Qvalues[j] > maxQ){ maxQ = pitchData[i].Qvalues[j]; }
			if(pitchData[i].Qvalues[j] > maxQ){ minQ = pitchData[i].Qvalues[j]; }
		}
	}

	for(let i = 0; i < pitchData.length; i++){
		var id = "pTD" + pitchData[i].x + "_" + pitchData[i].y;
		setColor(id, getTypeColor(pitchData[i].type));

		for(let j = 0; j < pitchData[i].Qvalues.length; j++){
			id = "action" + directionMap(j) + pitchData[i].x + "_" + pitchData[i].y;
			setColor(id, GreenValue(pitchData[i].Qvalues[j]));
		}
	}
}

function startLearning(){
	var nextValue;
		running = true;
		data = readData();
		console.log(data);
		resizePitch();
		$.ajax({
			method: 'POST',
			url: '/start',
			data: data,
			dataType: 'json'
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
							running = false;
						} else {
							refreshPitch(response.pitch);
						}
			}
	})}, 500);
}

$(document).ready(function(){
	resizePitch();
});
