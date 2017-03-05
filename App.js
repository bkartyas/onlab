var spawner = require('child_process').spawn,
    express = require('express'),
    readline = require('readline'),
    bodyParser = require('body-parser');

var app = express();
var rl;
var proc;
var output = [];
var outputSize;

function createInput(data){
	outputSize = data.pitch.type.length;
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
			/*console.log("readed:" + line);
			console.log(output);*/
		}
	});

	res.sendStatus(200);
})

app.get("/next", function(req, res){
	console.log("request:\t/next");
	
	var myOut = '';
	if(output.length != 0){
		myOut += '{"pitch":[';
		for(let i = 0; i < outputSize; i++){
			myOut += '["';
			myOut += output[i].split(' ').join('","').slice(0, -2);
			myOut += '],';
		}
		myOut = myOut.slice(0, -1);
		output.splice(0, outputSize);
		myOut += ']}';
	} else {
		myOut = '{"status":"end"}';
	}

	console.log('Response:');
	console.log(myOut);
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