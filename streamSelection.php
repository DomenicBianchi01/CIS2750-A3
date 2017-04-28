<?php

	exec('./a3 streamSelection.wpml '.$_POST['userID'], $result);

	foreach($result as $text) {

		//Add any values that will need to be passed between webpages
		if ($text == "</form>") {

			echo "<input type=\"hidden\" name=\"userID\" value=\"".$_POST['userID']."\"/>\n";
			echo "<input type=\"hidden\" name=\"firstTime\" value=\"1\"/>\n";
		}

		//Check if there is an executable that now needs to be execute
		if (substr($text, 0, 13) == '---EXECUTE---') {

			$executable = substr($text, 15);

			//Create command and execute it
			$command = escapeshellcmd($executable.' "'.$_POST['userID'].'"');
			$result2 = shell_exec($command);

			//Split results line by line
			$list = explode(PHP_EOL, $result2);

			foreach($list as $text2) {

				//Add any values that will need to be passed between webpages
				if ($text2 == "</form>") {

					echo "<input type=\"hidden\" name=\"userID\" value=\"".$_POST['userID']."\"/>\n";
					echo "<input type=\"hidden\" name=\"firstTime\" value=\"1\"/>\n";
				}

				echo $text2."\n";
			}
		}
		else {

			echo $text."\n";
		}
	}
?>
