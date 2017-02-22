var spawner = require('child_process').spawn,
    express = require('express'),
    readline = require('readline'),
    bodyParser = require('body-parser');

var app = express();

var proc;
var output;

//proc.stdin.end();
app.use(bodyParser.urlencoded())
app.post("/start", function(req, res){
	console.log("request:\t/start");
	proc = spawner('.\\bin\\MI.exe'),
	console.log("execute:\t.\\bin\\MI.exe");
	data = req.body;
	console.log(data);
	
	var rl = readline.createInterface({
	  input: proc.stdout
	}); rl.on('line', (line) => {
		console.log(line);
	  output += line + '\n';
	});
})

app.get("/next", function(req, res){
	var output = ''
	console.log("request: next");
	proc.stdin.write('n\n');
	//proc.stdin.end();

	console.log(output);
	res.end(output);
	output = '';
})

app.get("/a.js", function(req, res){
	//console.log("sent");
	res.sendFile('D:\\Suli\\2016-17(2.)\\Onlab\\onlab\\static\\a.js');
})

app.use(express.static('static'));
var server = app.listen(3000, function(){
	console.log('static');
})