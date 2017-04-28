<?php

	/*Domenic Bianchi
	CIS 2750 Assignment 3
	March 17, 2017
	This program displays a web interface to display posts from a signle or mutliple streams*/

	$counter = 0;

	//If a user was added/removed from a set of streams using the button on the login page
	if (!empty($_POST['streamList'])) {

		//Split the list of streams into array elements
		$result2 = explode(",",$_POST['streamList']);

		$commandLine = './addauthor ';

		//If the user is to be removed from the stream(s)
		if ($_POST['authorMode'] == ' Remove Author') {

			$commandLine = $commandLine.' '.'"-r" "';
		}
		else {

			$commandLine = $commandLine.'"" "';
		}

		//Create command that will be used to call the addauthor executable
		foreach ($result2 as $argument) {

			$commandLine = $commandLine.''.trim($argument).',';
		}

		if (!empty($_POST['userID2'])) {

			$commandLine = $commandLine.'" "'.$_POST['userID2'].'"';

			exec($commandLine, $result3);

			//Print any response messages to the browser (success, failure, etc)
			foreach ($result3 as $text) {

				echo $text."\n";
			}
		}
	}

	//Load web page content
	exec('./a3 index.wpml', $result);

	foreach($result as $text) {

		//Add any values that will need to be passed between webpages
		if ($text == "</form>") {

			echo "<input type=\"hidden\" name=\"fromLogin\" value=\"1\"/>\n";
		}

		echo $text."\n";
	}
?>


