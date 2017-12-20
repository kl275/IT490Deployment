<?php
require_once('path.inc');
require_once('get_host_info.inc');
require_once('rabbitMQLib.inc');


class FunctionLib
{
	private $conn;

// Attempts to connect to the database
// If the database does not exist log the error

	public function connect()
	{
		try
		{
		
			$this->conn = new PDO('mysql:host=localhost;dbname=users', 'root', 'password');

			return true;
		}
		catch(PDOException $e)
		{
			$message =  'Error: ' . $e->getMessage();
			//$client = new rabbitMQClient("logRabbitMQ.ini","logServer");			
			$client = new rabbitMQClient("testRabbitMQ.ini","testServer");      
			$request = array();
			$request['type'] = "log";
			$request['message'] = $message;
			$response = $client->publish($request);
		}
		
		return false;
	}

// Grabs Users firstname/lastname from registered table

	public function getUserInfo($email, $password)
	{
		$stmt = $this->conn->prepare('SELECT email, id FROM users WHERE email = :em AND password = :pw');
		$stmt->bindParam(':em', $email);
		$stmt->bindParam(':pw', $password);
		$stmt->setFetchMode(PDO::FETCH_ASSOC);
		$stmt->execute();
		
		$result = $stmt->fetch();
		echo "User id: " . $result['id'];		
		return $result;
	}
	
// Registers the User if the username 
// does not already exist in the database.

	public function registerUser($password, $firstname, $lastname, $email)
	{

		$database = mysql_connect ('localhost', 'root', 'password');
		mysql_select_db('users');
		$query = ("SELECT * FROM users where email = '$email'");
		$t = mysql_query($query);
		if(mysql_num_rows($t) > 0)
		{
			echo "Username already exist";
			return false;
		}
 		else
		{
			
			$this->conn = new PDO('mysql:host=localhost;dbname=users', 'root', 'password');
			$stmt = $this->conn->prepare("INSERT INTO users (password, fname, lname, email, id) VALUES (:pw, :fname, :lname, :em, NULL)");
			$stmt->bindParam(':pw', $password);
			$stmt->bindParam('fname', $firstname);
			$stmt->bindParam('lname', $lastname);
			$stmt->bindParam(':em', $email);

			return $stmt->execute();
			return true;
		}
	}
	
	
	public function __destruct()
	{
		$this->conn = null;
	}
}

?>
