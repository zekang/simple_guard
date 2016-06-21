<?php
echo __FILE__;
echo PHP_EOL;
for($i=1;$i<10;$i++){
	for($j=1;$j<=$i;$j++){
		echo $i.'*'.$j.'='.str_pad(($i*$j),2,' ',STR_PAD_LEFT).' ';
	}
	echo PHP_EOL;
}
