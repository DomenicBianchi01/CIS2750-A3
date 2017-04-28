<?php

	if (empty($_POST['streamList'])) {

		$result = array();

		//If the add author button was pressed from the login page
		if (isset($_POST['fromLogin']) == '1') {

			exec('./a3 addauthorLogin.wpml '.$_POST['userID'], $result);

		}
		//If the add author button was pressed from the view stream page
		else {

			exec('./a3 addauthor.wpml '.$_POST['userID'], $result);
		}

		//Display page
		foreach($result as $text) {

			//Add any values that will need to be passed between webpages
			if ($text == "</form>") {

				echo "<input type=\"hidden\" name=\"userID\" value=\"".$_POST['userID']."\"/>\n";
				echo "<input type=\"hidden\" name=\"stream\" value=\"".$_POST['stream']."\"/>\n";
				echo "<input type=\"hidden\" name=\"postCount\" value=\"".$_POST['postCount']."\"/>\n";
				echo "<input type=\"hidden\" name=\"maxCount\" value=\"".$_POST['maxCount']."\"/>\n";
				echo "<input type=\"hidden\" name=\"toggleMode\" value=\"".$_POST['toggleMode']."\"/>\n";
				echo "<input type=\"hidden\" name=\"timeStamp\" value=\"".$_POST['timeStamp']."\"/>\n";
				echo "<input type=\"hidden\" name=\"fromLogin\" value=\"".$_POST['fromLogin']."\"/>\n";
			}

			echo $text."\n";
		}
	}
?>
