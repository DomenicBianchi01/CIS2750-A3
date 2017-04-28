<?php

	$mode = 1;
	$count = -1;
	$postCount = 0;
	$maxCount = 0;
	$currentCount = 0;
	$toggleMode = 1;
	$timeStamp = 0;

	//If the add author button was pressed at the addauthor web page has already been displayed and submitted
	if(isset($_POST['streamList'])) {

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

		$command = escapeshellcmd('./getReadPostNum.py "'.$_POST['stream'].'" "'.$_POST['userID'].'" "'.$timeStamp.'"');
		$postInfo = shell_exec($command);
		$list = explode(PHP_EOL, $postInfo);
		$_POST['maxCount'] = $list[1];
	}
	//If this is the first time this webpage is being loaded
	if (isset($_POST['firstTime'])) {

		date_default_timezone_set("America/Toronto"); 
		$timeStamp = date("YmdHis");

		//Determine how many posts are in the stream and how many have already been read
		$command = escapeshellcmd('./getReadPostNum.py "'.$_POST['stream'].'" "'.$_POST['userID'].'" "'.$timeStamp.'"');
		$postInfo = shell_exec($command);
		$list = explode(PHP_EOL, $postInfo);
		$currentCount = $list[0];
		$maxCount = $list[1];
		$toggleMode = 1;
	}
	else if (!isset($_POST['firstTime'])) {

		$currentCount = $_POST['postCount'];
		$maxCount = $_POST['maxCount'];
		$toggleMode = $_POST['toggleMode'];
		$timeStamp = $_POST['timeStamp'];
	}

	//If the post button was pressed and a post was submitted
	if (isset($_POST['messageContent'])) {

		$commandLine = './post "'.$_POST['userID'].'" "'.$_POST['stream'].'" "'.$_POST['messageContent']."\n".'"';
		exec($commandLine, $result);
	}
	//Go to the previous post
	if (isset($_POST['Previous_Page']) && $currentCount - 1 > -1) {

		$currentCount = $currentCount - 1;
	}
	//Go to the next post
	else if (isset($_POST['Next_Page']) && $currentCount < $maxCount) {

		$currentCount = $currentCount + 1;
	}
	//Mark all posts in the stream as read
	else if (isset($_POST['Mark_All'])) {

		$command = escapeshellcmd('./markAllPosts.py "'.$_POST['stream'].'" "'.$_POST['userID'].'" "'.$maxCount.'"');
		$postInfo = shell_exec($command);
	}
	//If the order of the posts should be changed
	else if (isset($_POST['Toggle_Order'])) {

		if ($toggleMode == 1) {

			$toggleMode = 2;
		}
		else {

			$toggleMode = 1;
		}

		$currentCount = 0;
	}
	//Check for new posts in the stream
	else if (isset($_POST['Check_For_New'])) {

		date_default_timezone_set("America/Toronto"); 
		$timeStamp = date("YmdHis");
		$command = escapeshellcmd('./getReadPostNum.py "'.$_POST['stream'].'" "'.$_POST['userID'].'" "'.$timeStamp.'"');
		$postInfo = shell_exec($command);
		$list = explode(PHP_EOL, $postInfo);

		
		$currentCount = $list[0];
		$maxCount = $list[1];
		$toggleMode = 1;
	}

	if ($toggleMode == 1) {

		echo "<p>Messages currently sorted by date</p>\n<hr>\n";
	}
	else {

		echo "<p>Messages currently sorted by name</p>\n<hr>\n";
	}

	//Load webpage
	exec('./a3 viewStream.wpml "'.$_POST['userID'].'" "'.$_POST['stream'].'" "'.$currentCount.'" "'.$toggleMode.'" "'.$maxCount.'" "'.$timeStamp.'"', $result);

	foreach($result as $text) {

		//Add any values that will need to be passed between webpages
		if ($text == "</form>") {

			echo "<input type=\"hidden\" name=\"userID\" value=\"".$_POST['userID']."\"/>\n";
			echo "<input type=\"hidden\" name=\"stream\" value=\"".$_POST['stream']."\"/>\n";
			echo "<input type=\"hidden\" name=\"postCount\" value=\"".$currentCount."\"/>\n";
			echo "<input type=\"hidden\" name=\"maxCount\" value=\"".$maxCount."\"/>\n";
			echo "<input type=\"hidden\" name=\"toggleMode\" value=\"".$toggleMode."\"/>\n";
			echo "<input type=\"hidden\" name=\"timeStamp\" value=\"".$timeStamp."\"/>\n";
		}
		//Run getPosts.py
		if (substr($text, 0, 13) == '---EXECUTE---') {

			$executable = substr($text, 15);

			//Create command and execute
			$command = escapeshellcmd($executable.' "'.$_POST['userID'].'" "'.$_POST['stream'].'" "'.$currentCount.'" "'.$toggleMode.'" "'.$maxCount.'" "'.$timeStamp.'"');
			$result2 = shell_exec($command);

			//Print post
			echo $result2;
			echo "</body>\n</html>";
		}
		else {

			echo $text."\n";
		}
	}
?>
