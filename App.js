var spawner = require('child_process').spawn,
    express = require('express'),
    readline = require('readline'),
    bodyParser = require('body-parser');

var app = express();
var rl;
var proc;
var output = [];

function createInput(data){
	var x = data.pitch.type.length;
	var y = data.pitch.type[0].length;
	var input = x + ' ' + y + '\n';
	for(let i = 0; i < x; i++){
		for(let j = 0; j < y; j++){
			input += data.pitch.type[i][j] + ';' + data.pitch.reward[i][j] + ' ';
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

	proc.stdin.write('n\n');	
	rl = readline.createInterface({
	  input: proc.stdout
	}); rl.on('line', (line) => {
	  	if(line && line != 'start'){
		  	output.push(line);
			console.log("readed:" + line);
			console.log(output);
		}
	});

	res.sendStatus(200);
})

app.get("/next", function(req, res){
	console.log("request: next");
	var myOut = output[0] + output[1];
	console.log(myOut);
	output.splice(0, 2);
	res.end(myOut);
})

app.get("/index.js", function(req, res){
	//res.sendFile("D:\\Suli\\2016-17(2.)\\Onlab\\onlab\\static\\index.js");
	res.sendFile("C:\\Users\\Heavenly\\Desktop\\onlab\\www\\index.js");
})

app.get("/agent.jpg", function(req, res){
	//res.sendFile("D:\\Suli\\2016-17(2.)\\Onlab\\onlab\\static\\index.js");
	res.sendFile("C:\\Users\\Heavenly\\Desktop\\onlab\\www\\pictures\\agent.jpg");
})

app.use(express.static('www'));
var server = app.listen(3000, function(){
	console.log('www');
})