var type =  [];
var Qvalues =  [];
var maxQ;
var minQ;
var intervalID;
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
		Qvalues.push([]);
		let newTR = document.createElement("tr");
		for(j = 0; j < x; j++){
			type[i].push("P");
			Qvalues[i].push([0,0,0,0]);
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
	newTD.classList.add("emptyPlatform");
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

		startButton.innerHTML = "Stop";
		startButton.classList.remove("btn-success");
  		startButton.classList.add("btn-warning");
  		startButton.onclick = stopLearning;
  	}

    //newTD.style.backgroundImage = "url('http://clipart-library.com/image_gallery/145213.jpg')";

	return newTD;
}

function createRewardInput(){
	let textBox = document.createElement("input");
	textBox.classList.add("reward");
	textBox.setAttribute("value", "-10");
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

function hasClass(element, cls) {
    return (' ' + element.className + ' ').indexOf(' ' + cls + ' ') > -1;
}

function stepStatus(tableData){
	var parsedID = tableData.id.split("pTD")[1].split("_");
	var x = parsedID[0];
	var y = parsedID[1];

	if(hasClass(tableData, "emptyPlatform")){
		tableData.classList.remove("emptyPlatform");
	  	tableData.classList.add("wall");
	} else if(hasClass(tableData, "wall")){
		tableData.classList.remove("wall");
	  	tableData.classList.add("agent");
	} else if(hasClass(tableData, "agent")){
		tableData.classList.remove("agent");
	  	tableData.classList.add("finish");
	} else if(hasClass(tableData, "finish")){
		tableData.classList.remove("finish");
	  	tableData.classList.add("emptyPlatform");
	}
}

function setTypeClass(element, typeClass){
	element.classList.remove("emptyPlatform");
	element.classList.remove("wall");
	element.classList.remove("agent");
	element.classList.remove("finish");
	element.classList.add(typeClass);
}


function getTypeClass(type){
	switch(type){
	case "P":
		return "emptyPlatform";
		break;
	case "W":
		return "wall";
		break;
	case "A":
		return "agent";
		break;
	case "E":
		return "finish";
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

function rescaleQcolor(){
	var x = sizeX.value;
	var y = sizeY.value;

	for(let i = 0; i < 4; i++){
		for(let j = 0; j < x; j++){
			for(let k = 0; k < y; k++){
				id = "action" + directionMap(i) + x + '_' + y;
			}
		}
	}
}

function GreenValue(value){
	var rb;
	if(value === maxQ){
		rb = 255;
	} else {
		rb = ((1 - maxQ/value) * 255);
	}
	return 'rgb(' + rb + ',255,' + rb + ')';
}

function RedValue(value){
	var gb;
	if(value === minQ){
		gb = 255;
	} else {
		gb = ((1 - (minQ)/(minQ - value)) * 255);
	}
	return 'rgb(255,' + gb + ',' + gb + ')';
}

function refreshQvalues(){
	console.log(Qvalues);
	for(let i = 0; i < Qvalues.length; i++){
		for(let j = 0; j < Qvalues[i].length; j++){
			for(let k = 0; k < 4; k++){
				id = "action" + directionMap(k) + i + "_" + j;
				if(Qvalues[i][j][k] < 0){
					setColor(id, RedValue(Qvalues[i][j][k]));
				} else {
					setColor(id, GreenValue(Qvalues[i][j][k]));
				}
			}
		}
	}
}

function refreshPitch(pitchData){
	var oldMaxQ = maxQ;
	var oldMinQ = minQ;

	if(!maxQ && pitchData.length !== 0){ maxQ = Number(pitchData[0].Qvalues[0]); };
	if(!minQ && pitchData.length !== 0){ minQ = Number(pitchData[0].Qvalues[0]); };

	for(let i = 0; i < pitchData.length; i++){
		for(let j = 0; j < pitchData[i].Qvalues.length; j++){
			if(Number(pitchData[i].Qvalues[j]) > maxQ){ maxQ = Number(pitchData[i].Qvalues[j]); }
			if(Number(pitchData[i].Qvalues[j]) < minQ){ minQ = Number(pitchData[i].Qvalues[j]); }
		}
	}

	if(oldMaxQ !== maxQ || oldMinQ !== minQ){ rescaleQcolor(); }

	for(let i = 0; i < pitchData.length; i++){
		var id = "pTD" + pitchData[i].x + "_" + pitchData[i].y;
		setTypeClass(id, getTypeClass(pitchData[i].type));

		for(let j = 0; j < pitchData[i].Qvalues.length; j++){
			//id = "action" + directionMap(j) + pitchData[i].x + "_" + pitchData[i].y;

			Qvalues[pitchData[i].x][pitchData[i].y] = pitchData[i].Qvalues;

			refreshQvalues();
			//setColor(id, GreenValue(pitchData[i].Qvalues[j]));
		}
	}

	/*console.log("Q-s")
	console.log(Qvalues);*/
}

function queryNext(onSuccess){
	$.ajax({
			method: 'GET',
			url: '/next',
			dataType: 'json',
            contentType: "application/json; charset=utf-8",
			success: function(response){
						onSuccess(response);
					}
	})
}

function startLearning(){
	Qvalues = [];

	var nextValue;
		running = true;
		data = readData();
		console.log(data);
		$.ajax({
			method:   'POST',
			url: 	  '/start',
			data: 	  data,
			dataType: 'json',
			async: 	false
		});

		queryNext(function(res){
				resizePitch();
				console.log(res);
				if(res['status'] === 'end'){
					running = false;
				} else {
					refreshPitch(res.pitch);
				}
			}
		)

		intervalID = setInterval(function(){
			queryNext(function(response){
						console.log(response);
						if(response['status'] === 'end'){
							clearInterval(intervalID);
							running = false;
						} else {
							refreshPitch(response.pitch);
						}
					}
			)}, Number(speed.value)
		);
}

function stopLearning(){
	clearInterval(intervalID);

	$.ajax({
			method: 'GET',
			url: '/stop',
			dataType: 'json',
            contentType: "application/json; charset=utf-8"
	})

	running = false;
	resizePitch();
	startButton.innerHTML = "Start";
	startButton.classList.remove("btn-warning");
  	startButton.classList.add("btn-success");
  	startButton.onclick = startLearning;
}

$(document).ready(function(){
	resizePitch();
					console.log(type);console.log(Qvalues);
});
