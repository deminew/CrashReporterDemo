<?php

	/*
	 * Author: Andreas Linde <mail@andreaslinde.de>
	 *
	 * Copyright (c) 2009 Andreas Linde & Kent Sutherland. All rights reserved.
	 * All rights reserved.
	 *
	 * Permission is hereby granted, free of charge, to any person
	 * obtaining a copy of this software and associated documentation
	 * files (the "Software"), to deal in the Software without
	 * restriction, including without limitation the rights to use,
	 * copy, modify, merge, publish, distribute, sublicense, and/or sell
	 * copies of the Software, and to permit persons to whom the
	 * Software is furnished to do so, subject to the following
	 * conditions:
	 *
	 * The above copyright notice and this permission notice shall be
	 * included in all copies or substantial portions of the Software.
	 *
	 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
	 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
	 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
	 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
	 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
	 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
	 * OTHER DEALINGS IN THE SOFTWARE.
	 */

//
// This part is responsible common used functions
//

function show_header($title)
{
    global $admintitle;
    
    echo '<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML  4.01//EN" "http://www.w3.org/TR/html4/strict.dtd">';
    echo '<html><head><title>'.$admintitle.' '.$title.'</title>';
    echo '<link rel="stylesheet" href="blueprint/screen.css" type="text/css" media="screen, projection"><link rel="stylesheet" href="blueprint/print.css" type="text/css" media="print"><!--[if IE]><link rel="stylesheet" href="blueprint/ie.css" type="text/css" media="screen, projection"><![endif]--><link rel="stylesheet" href="blueprint/plugins/buttons/screen.css" type="text/css" media="screen, projection">';
    echo '<link rel="stylesheet" type="text/css" href="layout.css">';
    echo '</head><body><div id="container" class="container prepend-top append-bottom">';
    echo '<h1>'.$admintitle.'</h1>';
}

function end_with_result($result)
{
	return '<html><body>'.$result.'</body</html>'; 
}

function init_database()
{
    global $server, $loginsql, $passsql, $base;

    $GLOBALS['link'] = mysql_connect($server, $loginsql, $passsql) or die(end_with_result('No database connection'));
    mysql_select_db($base) or die(end_with_result('No database connection'));
}


function parse_parameters($allowed_args)
{
    foreach(array_keys($_GET) as $k) {
        $temp = ",$k,";
        if(strpos($allowed_args,$temp) !== false) { $GLOBALS[$k] = $_GET[$k]; }
    }
}


function add_option($text, $value, $currentvalue)
{
    echo '<option value="'.$value.'"';
    if ($currentvalue == $value)
        echo " selected";
    echo '>'.$text.'</option>';
}

function create_link($text, $link, $button, $variables)
{
    $result = "";
    $result .= '<a href="'.$link;
    if (strpos($variables,'?') !== false)
    {
        $result .= $variables;
    } else {
        $result .= '?';
        $vars = explode(',', $variables);
        
        $i = 0;
        foreach(array_keys($vars) as $k) {
            if (strpos($vars[$k], '=') === false)
            {
                if (isset($GLOBALS[$vars[$k]]) && $GLOBALS[$vars[$k]] != "") {
                    if ($i > 0) {
                        $result .= '&';
                    }
                    $result .= $vars[$k].'='.$GLOBALS[$vars[$k]];
                }
            } else {
                if ($i > 0) {
                    $result .= '&';
                }
                $result .= $vars[$k];
            }
            $i++;
        }
    }
    $result .= '"';
    
    if ($button)
        $result .= ' class="button"';
    $result .= '>'.$text.'</a>';
    
    return $result;
}

function show_search($text, $currenttype)
{
    global $bundleidentifier, $version;
    
    echo '<form name="search" action="crashes.php" method="get">';
    echo '<input type="hidden" name="bundleidentifier" value="'.$bundleidentifier.'"/>';
    if ($version != "")
        echo '<input type="hidden" name="version" value="'.$version.'"/>';
    echo '<table><tr><th></th></tr>';
    echo '<tr><td style="text-align:right;vertical-align:right;"><input type="search" placeholder="Search this" name="search" size="25" maxlength="100"';
    if ($text != "" && $currenttype != "") echo ' value="'.$text.'"';
    echo '/> <select name="type">';
    add_option('ID', SEARCH_TYPE_ID, $currenttype);
    add_option('Description', SEARCH_TYPE_DESCRIPTION, $currenttype);
    add_option('Crashlog', SEARCH_TYPE_CRASHLOG, $currenttype);
    echo '</select>&nbsp;&nbsp;<button type="submit" class="button" style="float:right;">Search</button></td></tr>';
    echo '</table></form>';
}

function currentPageURL()
{
    $pageURL = 'http';
    if ($_SERVER["HTTPS"] == "on") {$pageURL .= "s";}
    $pageURL .= "://";
    if ($_SERVER["SERVER_PORT"] != "80") {
        $pageURL .= $_SERVER["SERVER_NAME"].":".$_SERVER["SERVER_PORT"].$_SERVER["REQUEST_URI"];
    } else {
        $pageURL .= $_SERVER["SERVER_NAME"].$_SERVER["REQUEST_URI"];
    }
    return $pageURL;
}

function create_issue($bundleidentifier, $issuelink)
{
    global $dbapptable, $createIssueTitle;
    
    $query = "SELECT issuetrackerurl FROM ".$dbapptable." WHERE bundleidentifier = '".$bundleidentifier."'";
    $result = mysql_query($query) or die(end_with_result('Error in SQL '.$query));

    $numrows = mysql_num_rows($result);
    if ($numrows > 0) {
        // get the status
        while ($row = mysql_fetch_row($result))
        {
            $issuetrackerurl = $row[0];
            $issuetrackerurl = str_replace('%subject%', urlencode($createIssueTitle), $issuetrackerurl);
            $issuetrackerurl = str_replace('%description%', urlencode('Crash Log: '.$issuelink), $issuetrackerurl);
            return "<a href='".$issuetrackerurl."' target='_new' class='button'>New issue</a>";
        }
    }
    
    return '';
}


?>
