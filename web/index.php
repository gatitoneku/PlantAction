<!DOCTYPE html>
<html>
<head><title>PlantAction Login</title></head>  
<body>
<h1 style="font-family: Verdana">PlantAction</h1>
<h2 style="font-family: Verdana">Smart Plant Watering System</h2>
<form action="login.php" method="post"> 
  <div class="container">
    <label><b>Username:</b></label>
    <input type="text" placeholder="Enter Username" name="username" required><br>

    <label><b>Password:</b></label>
    <input type="password" placeholder="Enter Password" name="password" required><br>
      
    <input type="submit" id="login" value="Login" />
    <input type="checkbox" checked="checked"> Remember me
  </div><br>
    
    <!--PHP-->
<?php
require('config.inc.php');
if(!empty($_POST)) {

/* simple database login, will fix later */  
$result = pg_query_params($db, 'SELECT * FROM users WHERE username=$1 and password=$2', array($_POST['username'],$_POST['password']));

$rows = pg_num_rows($result);
    
if($rows!=0) {
    header('Location: http://localhost:8000/login.php' );
}
    else{
        echo "<p>Username and password do not match. Please contact admin@plantaction.me to register.</p>";
    }
}
?>
<!--end PHP-->
</form>
  
<!--_CSS_--->
<style>
/* Bordered form */
form {
    border: 3px solid #f1f1f1;
}

/* Full-width inputs */
input[type=text], input[type=password] {
    width: 10%;
    padding: 12px 20px;
    margin: 8px 0;
    display: inline-block;
    border: 1px solid #ccc;
    box-sizing: border-box;
}

/* Set a style for all buttons */
button {
    background-color: #4CAF50;
    color: white;
    padding: 14px 20px;
    margin: 8px 92;
    border: none;
    cursor: pointer;
    width: 10%;
}
p {
        color: red;
        font-family: "Verdana";
}

/* Extra style for the cancel button (red) */
.cancelbtn {
    width: auto;
    padding: 10px 18px;
    background-color: #f44336;
}

/* Add padding to containers */
.container {
    padding: 16px;
}

/* The "Forgot password" text */
span.psw {
    float: center;
    padding-top: 16px;
}

/* Change styles for span and cancel button on extra small screens */
@media screen and (max-width: 300px) {
    span.psw {
        display: block;
        float: none;
    }
    .cancelbtn {
        width: 100%;
    }
}    
</style>
    
</body>    
</html>