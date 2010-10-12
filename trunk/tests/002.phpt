<?php
#echo confirm_concurrency_compiled(1);       
$c = new Concurrency();

$tmp = new ConcurrencyRequest();
$tmp->url = "http://loceeealhost/";
$r[] = $tmp;
/**
$tmp = new ConcurrencyRequest();
$tmp->url = "http://test.taobao.com/";
$r[] = $tmp;

$tmp = new ConcurrencyRequest();
$tmp->url = "http://s.taobao.com/";
$r[] = $tmp;

$tmp = new ConcurrencyRequest();
$tmp->url = "http://www.baidu.com/";
$r[] = $tmp;

$tmp = new ConcurrencyRequest();
$tmp->url = "http://www.google.com/";
$r[] = $tmp;


$tmp = new ConcurrencyRequest();
$tmp->url = "http://ss.taobao.com/";
$r[] = $tmp;
*/

foreach($r as $request) {
    $c->addRequest($request);
}



$c->request();
var_export($c);
echo "end---";
?>
