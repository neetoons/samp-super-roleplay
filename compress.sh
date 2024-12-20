git restore srp_db.ini 
variable=$(git rev-parse --short=8 HEAD)
zip -r super-roleplay-2-$variable.zip\
	libmariadb.dll\
	.git\
	.gitignore\
	log-core.dll\
	log-core.so\
	srp_db.ini\
	announce\
	server.cfg\
	samp03svr\
	pawno/*\
	plugins/*\
	db/*\
	filterscripts/*\
	scriptfiles/*\
	gamemodes/*\
	samp-server.exe\
	README.md
