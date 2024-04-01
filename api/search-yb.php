<?php

	$song_title = "http://www.youtube.com/results?search_query=".$_GET['song_title'];
	$song_title = str_replace(' ', '+', $song_title);
	$limit = $_GET['limit'];
	$content = file_get_contents($song_title);

	$offset = 0;
	$counter = 0;
	$result = strpos($content, "href=\"/watch?v=", $offset);

	while($result != false)
	{
		$video_id = substr($content, $result + 15, 11);
		$title_var_1 = strpos($content, "dir=\"ltr\">", $result);
		$title_var_2 = strpos($content, "</a>", $title_var_1);
		$len = $title_var_2 - ($title_var_1 + 10);
		$titulo = substr($content, $title_var_1 + 10, $len);
		$titulo = html_entity_decode($titulo);
		$titulo = iconv("UTF-8", "WINDOWS-1252", strval($titulo));
	
		printf("|[%s||%s]|", $video_id, $titulo);

		$counter ++;
		$offset = $title_var_2 + 4;
		$result = strpos($content, "href=\"/watch?v=", $offset);
		if($counter >= $limit) break;
	}
	if($counter == 0) print("no");
		
?>