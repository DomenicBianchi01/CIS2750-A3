<?php

	$result = array();

	//If the user is currently viewing all streams, then they cannot create a new post
	if ($_POST['stream'] != 'all') {
		
		exec('./a3 post.wpml '.$_POST['userID'], $result);
	}
	else {

		exec('./a3 blockPost.wpml '.$_POST['userID'], $result);
	}

	//Display webpage
	foreach($result as $text) {

		//Remove whitespace if the first character in the string is empty
		if ($text[0] == ' ') {

			$text = substr($text, 1);
		}

		//Add any values that will need to be passed between webpages
		if ($text == "</form>") {

			echo "<input type=\"hidden\" name=\"userID\" value=\"".$_POST['userID']."\"/>\n";
			echo "<input type=\"hidden\" name=\"stream\" value=\"".$_POST['stream']."\"/>\n";
			echo "<input type=\"hidden\" name=\"postCount\" value=\"".$_POST['postCount']."\"/>\n";
			echo "<input type=\"hidden\" name=\"maxCount\" value=\"".$_POST['maxCount']."\"/>\n";
			echo "<input type=\"hidden\" name=\"toggleMode\" value=\"".$_POST['toggleMode']."\"/>\n";
			echo "<input type=\"hidden\" name=\"timeStamp\" value=\"".$_POST['timeStamp']."\"/>\n";
			echo "<input type=\"hidden\" name=\"mode2\" value=\"2\"/>\n";
		}

		echo $text."\n";
	}
?>
