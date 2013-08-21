#!/usr/bin/php
<?php
$root12 = pow(2,1/12);
for($i=3; $i<51; $i++){
	echo round((10500000/(440*pow($root12,$i)))).",";
	if(($i-2)%4 == 0)
		echo "\n";
}
?>
