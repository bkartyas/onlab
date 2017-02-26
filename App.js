var spawner = require('child_process').spawn,
    express = require('express'),
    readline = require('readline'),
    bodyParser = require('body-parser');

var app = express();
var rl;
var proc;
var output = '[';

function cacheNext(){	
	output = '[';
	//proc.stdin.write('n\n');	
}

function createInput(data){
	var input = data['size']['x'] + ' ' + data['size']['y'] + '\n';
	for(let i = 0; i < data['size']['x']; i++){
		for(let j = 0; j < data['size']['y']; j++){
			input += data['parameters'][i][j] + ' ';
		}
		input += '\n';
	}

	return input;
}

app.use(bodyParser.urlencoded())
app.post('/start', function(req, res){
	console.log('request:\t/start');
	proc = spawner(".\\bin\\MI.exe"),
	console.log('execute:\t.\\bin\\MI.exe');

	data = createInput(req.body);
	console.log(data);
	proc.stdin.write(data);

	rl = readline.createInterface({
	  input: proc.stdout
	}); rl.on('line', (line) => {
	  	if(line && line != 'start'){
		  	output += '"' + line.trim().split(/[ ]+/).join('", "') + '", ';
			//console.log(line);
			console.log(output);
		}
	});

	cacheNext();
	res.sendStatus(200);
})

app.get("/next", function(req, res){
	console.log("request: next");
	output += ']';
	console.log(output);
	res.end(output);
	cacheNext();
})

app.get("/a.js", function(req, res){
	//console.log("sent");
	res.sendFile("D:\\Suli\\2016-17(2.)\\Onlab\\onlab\\static\\a.js");
})

app.use(express.static('static'));
var server = app.listen(3000, function(){
	console.log('static');
})