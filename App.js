var spawner = require('child_process').spawn,
    express = require('express'),
    readline = require('readline'),
    bodyParser = require('body-parser');

var app = express();

app.use(bodyParser.urlencoded());
app.use(bodyParser.json());

var rl;
var proc;
var output = [];
var outBefore;
var outputSize;

function log(message, data){
	console.log(message);
	if(!!data){
		console.log(data);
	}
}

function createInput(data){
	outputSize = data.pitch.type.length;
	var x = data.pitch.type.length;
	var y = data.pitch.type[0].length;
	var input = x + ' ' + y + '\n' + data.epoch + '\n' + data.alpha + '\n' + data.gamma + '\n';
	for(let i = 0; i < x; i++){
		for(let j = 0; j < y; j++){
			input += data.pitch.type[i][j] + ',' + data.pitch.reward[i][j] + ' ';
		}
		input += '\n';
	}

	return input;
}

app.post('/start', function(req, res){
	log('request:\t/start');
	proc = spawner(".\\bin\\MI.exe"),
	log('execute:\t.\\bin\\MI.exe', req.body);
	data = createInput(req.body);
	log('parsed:', data);
	proc.stdin.write(data);

	proc.stdin.write('n\n');
	rl = readline.createInterface({
	  input: proc.stdout
	}); rl.on('line', (line) => {
	  	if(line && line != 'start'){
		  	output.push(line);
		}
	});

	res.sendStatus(200);
})

function createOutput(){
	out = { "pitch": []};
	for(let i = 0; i < outputSize; i++){
		out.pitch.push([]);
		fields = output[i].split(' ');
		for(let j = 0; j < fields.length-1; j++){
			out.pitch[i].push(fields[j]);
		};
	}
	output.splice(0, outputSize);

	return out;
}

app.get("/next", function(req, res){
	log("request:\t/next");

	var myOut = { "pitch": []};
	if(output.length != 0){
		if(!outBefore){
			outBefore = createOutput();

			for(let i = 0; i < outputSize; i++){
				for(let j = 0; j < fields.length-1; j++){
					let values = outBefore.pitch[i][j].split(',');
					myOut.pitch.push({ "x": 	  j,
									   "y": 	  i,
									   "type": 	  values[0],
									   "reward":  values[1],
									   "Qvalues": values.slice(2, 6)});
				};
			}
		} else {
			out = createOutput();

			for(let i = 0; i < outputSize; i++){
				for(let j = 0; j < fields.length-1; j++){
					if(out.pitch[i][j] !== outBefore.pitch[i][j]){
						let values = out.pitch[i][j].split(',');
						myOut.pitch.push({ "x": 	  j,
										   "y": 	  i,
										   "type": 	  values[0],
										   "reward":  values[1],
										   "Qvalues": values.slice(2, 6)});
						outBefore.pitch[i][j] = out.pitch[i][j];
					}
				}
			}
		}
	} else {
		myOut.status = 'end';
	}

	//console.log(output);
	log('Response:', myOut);
	res.json(myOut);
	res.end();
})

app.use(express.static('www'));

var server = app.listen(3000, function(){
	log('www');
})