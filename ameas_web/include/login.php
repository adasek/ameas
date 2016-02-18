<?php
/*
 * Login-related stuff
 *
 * Author: Adam Benda <adam@adasek.cz>, 2016
*/

/*
  After including this, next variables are set:
  $logged: Am I a user? (true/false)
  $logged_user: My id
  $logged_email: My email
 */


session_start();

if ($_GET['w'] == "logout") {
    $_SESSION["login_id"] = "";
    $_SESSION["pass"] = "";
    header("Location:" . $BASEHREF . "");
    exit;
}

$logged = FALSE;
$logged_user = "";

if (!empty($_POST['login_email']) || !empty($_SESSION["logged_user"])) {

    if (!empty($_POST['login_email'])) {
        //Login from input form - according to email address
        $login = $mysqli->real_escape_string(strtolower($_POST['login_email']));

        //What is my id? Need to know to salt password properly.
        $select = "SELECT id FROM User WHERE email='$login'";
        $r = $mysqli->query($select)->fetch_array(MYSQLI_ASSOC);
        $u_id = $r['id'];

        $passhash = md5($_POST['login_pass'] . "_u=" . $u_id);
        echo $u_id . ":" . $passhash;
        $select = "SELECT * FROM User WHERE email='$login' AND password='$passhash'";
    } else {
        //Session strings should be safe
        $login_id = $_SESSION["logged_user"];
        $passhash = $_SESSION["pass"];
        $select = "SELECT * FROM User WHERE id='$login_id' AND password='$passhash'";
    }

    $loq = $mysqli->query($select);
    $lor = $loq->fetch_array();
    if ($loq->num_rows > 0) {
        $logged = TRUE;
        $logged_user = $lor['id'];
        $logged_email = $lor['email'];
    }

    if ($logged == TRUE && !empty($_POST['login_email'])) {
        //First (successful) login = we have to set into session
        $_SESSION["logged_user"] = $logged_user;
        $_SESSION["pass"] = $passhash;
        header("Location:" . $BASEHREF . "");
    }
}

if ($logged) {
    $tpl_logged = "<a style=\"font-size:70%\" href=\"" . $BASEHREF . "?w=logout\">Logout</a>";
}
?>
