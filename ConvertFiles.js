var fs = require('fs'),
validFile = function (s) {
	return (/\.txt/).test(s)
},
searchRegex = /(\[|\])/g;

var i = 0;

var walk = function(dir) {
	fs.readdir(dir, function(err, list) {
		if (err) throw err;
		var pending = list.length;
		if (!pending) return;
		list.forEach(function(file) {
			file = dir + '/' + file;
			fs.stat(file, function(err, stat) {
				if (stat && stat.isDirectory()) {
					walk(file, function(err, res) {
						if (!--pending) return;
					});
				} else {
					if(validFile(file)) {
						fs.readFile(file, function(err, contents) {
							fs.writeFile(file, 
								contents.toString().replace(searchRegex, function(match, space) {
									console.log(file, ++i);
									return " "; 
								})
							);
						});
					}
					if (!--pending) return;
				}
			});
		});
	});
};

walk('./2x2/');







