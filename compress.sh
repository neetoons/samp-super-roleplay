git restore srp_db.ini 
commit=$(git rev-parse --short=8 HEAD)
zip -r super-roleplay-$commit.zip\
	libmariadb.dll\
	.git\
	.gitignore\
	log-core.dll\
	log-core.so\
	srp_db.ini\
	announce\
	announce.exe\
	server.cfg\
	samp-server.exe\
	samp03svr\
	pawno/*\
	plugins/*\
	db/*\
	filterscripts/*\
	scriptfiles/*\
	gamemodes/*\
	README.md
