<?php
header("Content-type: text/html; charset=utf-8");
require_once 'AipOcr.php';

// 你的 APPID AK SK
const APP_ID = '你的 App ID';
const API_KEY = '你的 Api key';
const SECRET_KEY = '你的 Secret Key';

$client = new AipOcr(APP_ID, API_KEY, SECRET_KEY);

$image = file_get_contents('code.jpg');
$client->basicGeneral($image);

// 调用通用文字识别, 图片参数为本地图片
$res = $client->basicGeneral($image);
// 打印结果 $res
var_dump($res);


?>