<?php

$action     = $_GET['action'];
$smil_file  =  'simple.smil';
$media_file = 'server.jpg';

if ($action == 'show_agent_string')
{
	$agent_file = 'agent.txt';
	file_put_contents($agent_file, $_SERVER['HTTP_USER_AGENT']);
	header('Last-Modified:'.gmdate('D, d M Y H:i:s', time()).' GMT', true, 200);
	header('Content-Length: '.filesize($agent_file));
	header("Content-Type: application/smil");
	header("Content-Description: File Transfer");
	header("Content-Disposition: attachment; filename=".basename($agent_file));
	readfile($agent_file);
	unlink($agent_file);
}
elseif ($action == 'get_smil_new')
{
	header('Last-Modified:Tue, 01 Mar 2016 14:14:14 GMT', true, 200);
	header('Content-Length: '.filesize($smil_file));
	header("Content-Type: application/smil");
	header("Content-Description: File Transfer");
	header("Content-Disposition: attachment; filename=".basename($smil_file));
	readfile($smil_file);
}
elseif ($action == 'get_smil_cached')
{
	header('Last-Modified:Tue, 01 Mar 2016 14:14:14 GMT', true, 304);
}
elseif ($action == 'get_smil_updated')
{
	header('Last-Modified:'.gmdate('D, d M Y H:i:s', time()+36000).' GMT', true, 200);
	header('Content-Length: '.filesize($smil_file));
	header("Content-Type: application/smil");
	header("Content-Description: File Transfer");
	header("Content-Disposition: attachment; filename=".basename($smil_file));
	readfile($smil_file);
}
elseif ($action == 'get_media_new')
{
	header('Last-Modified:Tue, 01 Mar 2016 14:14:14 GMT', true, 200);
   	header('Content-Length: '.filesize($media_file));
	header("Content-Type: image/jpeg");
	header("Content-Disposition: attachment; filename=".basename($media_file));
	readfile($media_file);
}
elseif ($action == 'get_media_cached')
{
	header('Last-Modified:Tue, 01 Mar 2016 14:14:14 GMT', true, 304);
}
elseif ($action == 'get_media_updated')
{
	header('Last-Modified:'.gmdate('D, d M Y H:i:s', time()+3600).' GMT', true, 200);
   	header('Content-Length: '.filesize($media_file));
	header("Content-Type: image/jpeg");
	header("Content-Disposition: attachment; filename=".basename($media_file));
	readfile($media_file);
}

?>
