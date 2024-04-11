<?php
$servername = "localhost";
$dBUsername = "root";
$dBPassword = "";
$dBName = "jplearn";

$conn = mysqli_connect($servername, $dBUsername, $dBPassword, $dBName);
if (!$conn) 
{
    die("Connection failed: " . mysqli_connect_error());
}

if ($_SERVER['REQUEST_METHOD'] === 'POST') 
{
    // Takes raw data from the request
    $json = file_get_contents('php://input');
    // echo $json;

    // // Converts it into a PHP object
    $data = json_decode($json);

    if (json_last_error() == JSON_ERROR_NONE) {
        $device_id = $data->device_id;
        $type = $data->type;
        $value = $data->value;

        $created = date('Y-m-d H:i:s', strtotime(' + 4 hours'));

        $query = "INSERT INTO events (device_id, type, value, created) VALUES ('$device_id','$type','$value','$created');";

        if (mysqli_query($conn, $query)) {
            $response = ['response' => 'New data created successfully :)'];
            header("HTTP/1.1 201 OK");
        } else {
            $response = ['response' => "Error: " . $query . "<br>" . mysqli_error($conn)];
            header("HTTP/1.1 500 NOT OK");
        }

        header('Content-Type: application/json; charset=utf-8');
        echo json_encode($response);
    } else {
        echo "JSON Invalid";
    }
}

?>

// EXAMPLE-2 ye code basically ahmad logs se liya gaya hai.
<?php
/*************************************************************************************************
 *  Created By: Tauseef Ahmad
 *  Created On: 3 April, 2023
 *  
 *  YouTube Video: https://youtu.be/VEN5kgjEuh8
 *  My Channel: https://www.youtube.com/channel/UCOXYfOHgu-C-UfGyDcu5sYw/
 ***********************************************************************************************/
 
$hostname = "localhost"; 
$username = "root"; 
$password = ""; 
$database = "ENTER_DATABASE_NAME"; 

$conn = mysqli_connect($hostname, $username, $password, $database);

if (!$conn) { 
	die("Connection failed: " . mysqli_connect_error()); 
} 

echo "Database connection is OK<br>"; 

if(isset($_POST["temperature"]) && isset($_POST["humidity"])) {

	$t = $_POST["temperature"];
	$h = $_POST["humidity"];

	$sql = "INSERT INTO dht11 (temperature, humidity) VALUES (".$t.", ".$h.")"; 

	if (mysqli_query($conn, $sql)) { 
		echo "\nNew record created successfully"; 
	} else { 
		echo "Error: " . $sql . "<br>" . mysqli_error($conn); 
	}
}

?>