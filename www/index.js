var numberOfActions = 6;

var type =  [];
var Qvalues =  [];
var ids =  { "A": [],
			 "E": [] };
var maxQ = 0;
var minQ = 0;
var intervalID;
var running = false;
var offset = 0;

function initialize(){
	type =  [];
	Qvalues =  [];
	ids =  { "A": [],
			 "E": [] };
	maxQ = 0;
	minQ = 0;
	intervalID = null;
	running = false;
	offset = 0;

	while (QAgents.childNodes.length > 3) {
    	QAgents.removeChild(QAgents.lastChild);
	}

	startButton.innerHTML = "Start";
	startButton.classList.remove("btn-warning");
  	startButton.classList.add("btn-success");
  	startButton.onclick = startLearning;
}


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
	Qvalues = [];
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

  	/*$(".platform").height($(".platform").width());
  	$(".platform").width($(".platform").width());*/
  	$(".platform").width($(".platform").height());
}

function actionType(x, y){
	if(x === 0 && y === 1){ return "left"; }
	if(x === 1 && y === 0){ return "up"; }
	if(x === 2 && y === 1){ return "right"; }
	if(x === 1 && y === 2){ return "down"; }
	if(x === 2 && y === 0){ return "destroy"; }
	if(x === 0 && y === 0){ return "build"; }
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

function getID(type){
	var index = 0
	if(type === "A"){
		for(; ids[type][index] && index < ids[type].length; index++){}
		ids[type][index] = index+1;
	} else if(type === "E"){
		for(; (ids[type][index] || !ids["A"][index]) && index < ids[type].length; index++){}
		ids[type][index] = index+1;
	}
	return ids[type][index];
}

function removeID(type, position){
	ids[type][position-1] = undefined;
}

function agentsNumber(){
	let i = 0;
	for(agent in ids["A"]){
		if(agent){ i++; }
	}
	return i;
}

function agentsIndex(id){
	let i = 1;
	for(agent in ids["A"]){
		if(id === ids.A[agent]){ return i; }
		if(agent){ i++; }
	}
}

function addRadio(){
	var agentNum = agentsNumber();
	var radio = document.createElement("input");
	radio.type = "radio"; radio.name = "QAgent";
	radio.addEventListener('click', function(){	offset = (agentNum-1) * numberOfActions; maxQ = 0; minQ = 0; });
	radio.id = "Agent" + agentNum + "radio";
	var label = document.createElement("label");
	label.innerHTML = "Agent" + agentNum;
	label.htmlFor = "Agent" + agentNum + "radio";
	QAgents.appendChild(radio);
	QAgents.appendChild(label);
	QAgents.appendChild(document.createElement("br"));
}

function removeRadio(){
	QAgents.removeChild(QAgents.lastElementChild);
	QAgents.removeChild(QAgents.lastElementChild);
	QAgents.removeChild(QAgents.lastElementChild);
}

function stepStatus(tableData){
	var parsedID = tableData.id.split("pTD")[1].split("_");
	var x = parsedID[0];
	var y = parsedID[1];

	if(hasClass(tableData, "emptyPlatform")){
		tableData.classList.remove("emptyPlatform");
	  	tableData.classList.add("wall");
	  	type[y][x] = "W";
	} else if(hasClass(tableData, "wall")){
		tableData.classList.remove("wall");
	  	tableData.classList.add("agent");
	  	type[y][x] = "A" + getID("A")
	  	addRadio();
	} else if(hasClass(tableData, "agent")){
		tableData.classList.remove("agent");
	  	tableData.classList.add("finish");
	  	removeID("A", Number(type[y][x].split("A")[1]));
	  	type[y][x] = "E" + getID("E");
	  	removeRadio();
	} else if(hasClass(tableData, "finish")){
		tableData.classList.remove("finish");
	  	tableData.classList.add("emptyPlatform");
	  	removeID("E", Number(type[y][x].split("E")[1]));
	  	type[y][x] = "P";
	}
}

function setTypeClass(id, typeClass){
	var element = document.getElementById(id);

	element.classList.remove("emptyPlatform");
	element.classList.remove("wall");
	element.classList.remove("agent");
	element.classList.remove("choosenAgent");
	element.classList.remove("finish");
	element.classList.add(typeClass);
}


function getTypeClass(type){
	switch(type[0]){
	case "P":
		return "emptyPlatform";
		break;
	case "W":
		return "wall";
		break;
	case "A":
		if(agentsIndex(Number(type.split("A")[1])) === Math.floor(offset / numberOfActions + 1)){
			return "choosenAgent";
		} else {
			return "agent";
		}
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

function actionMap(index){
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
		case 4:
			return "destroy";
			break;
		case 5:
			return "build";
			break;
	}
}

function rescaleQcolor(){
	var x = sizeX.value;
	var y = sizeY.value;

	for(let i = 0; i < numberOfActions; i++){
		for(let j = 0; j < x; j++){
			for(let k = 0; k < y; k++){
				id = "action" + actionMap(i) + x + '_' + y;
			}
		}
	}
}

function GreenValue(value){
	var rb;
	rb = 64 - Math.round((value/maxQ) * 64);
	a = value/maxQ;
	return 'rgba(' + rb + ',255,' + rb + ',' + a + ')';
}

function RedValue(value){
	var gb;
	gb = 64 - Math.round((value/minQ) * 64);
	a = value/minQ;
	return 'rgba(255,' + gb + ',' + gb + ',' + a + ')';
}

function refreshQvalues(){
	//console.log(Qvalues);
	for(let i = 0; i < Qvalues.length; i++){
		for(let j = 0; j < Qvalues[i].length; j++){
			for(let k = 0; k < numberOfActions; k++){
				id = "action" + actionMap(k) + j + "_" + i;
				if(Qvalues[i][j][offset+k] < 0){
					setColor(id, RedValue(Qvalues[i][j][offset+k]));
				} else {
					setColor(id, GreenValue(Qvalues[i][j][offset+k]));
				}
			}
		}
	}
}

function minmaxQ(){
	for(let i = 0; i < Qvalues.length; i++){
		for(let j = 0; j < Qvalues[i].length; j++){
			for(let k = 0; k < numberOfActions; k++){
				if(Qvalues[i][j][offset+k] > maxQ){
					maxQ = Qvalues[i][j][offset+k];
				}
				if(Qvalues[i][j][offset+k] < minQ){
					minQ = Qvalues[i][j][offset+k];
				}
			}
		}
	}
}

function refreshPitch(pitchData){
	if(!pitchData){ return; }

	for(let i = 0; i < pitchData.length; i++){
		var id = "pTD" + pitchData[i].x + "_" + pitchData[i].y;
		setTypeClass(id, getTypeClass(pitchData[i].type));
		Qvalues[pitchData[i].y][pitchData[i].x] = pitchData[i].Qvalues;
	}

	minmaxQ();
	refreshQvalues();
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
						//console.log(response);
						onSuccess(response);
					}
	})
}

function changeSpeed(){
	if(!running){ return; }
	clearInterval(intervalID);

	intervalID = setInterval(function(){
		queryNext(function(response){
				//console.log(response);
				if(response['status'] === 'end'){
					clearInterval(intervalID);
				} else {
					refreshPitch(response.pitch);
				}
			}
		)}, Number(speed.value)
	);
}

function startLearning(){
	data = readData();
	//console.log(data);
	$.ajax({
		method:   'POST',
		url: 	  '/start',
		data: 	  data,
		dataType: 'json',
		async: 	false,
	});

	running = true;
	resizePitch();
	startButton.innerHTML = "Stop";
	startButton.classList.remove("btn-success");
	startButton.classList.add("btn-warning");
	startButton.onclick = stopLearning;

	if(running){
		intervalID = setInterval(function(){
			queryNext(function(response){
						//console.log(response);
						if(response['status'] === 'end'){
							clearInterval(intervalID);
						} else {
							refreshPitch(response.pitch);
						}
					}
			)}, Number(speed.value)
		);
	} else {
		initialize();
	}
}

function stopLearning(){
	clearInterval(intervalID);

	$.ajax({
			method: 'GET',
			url: '/stop',
			dataType: 'json',
           	contentType: "application/json; charset=utf-8",
           	async: 	false
	})

	initialize();
	resizePitch();
}

$(document).ready(function(){
	resizePitch();
});
