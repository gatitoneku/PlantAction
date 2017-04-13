<?php
    $host = "localhost";
	$port = "5432";
	$dbname = "dbname";
	$user = "user";
	$password = "pass";
	$db = pg_connect( "host=$host port=$port dbname=$dbname user=$user password=$password" );

    session_start(); 
?>