<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.2.3/dist/css/bootstrap.min.css" rel="stylesheet">
    <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.2.3/dist/js/bootstrap.bundle.min.js"></script>
    <link rel="icon" type="image/x-icon" href="imgs/donkey.png">
    <title>Student Leave System - ระบบขออนุญาตหยุดเรียน</title>
</head>
<body>
    <nav class="navbar navbar-expand-sm bg-dark justify-content-center">
        <ul class="navbar-nav">
            <li class="nav-item">
            <a class="nav-link text-light" href="./index.php">State</a>
            </li>
            <li class="nav-item">
            <a class="nav-link text-light" href="./temp.php">Temperature</a>
            </li>
            <li class="nav-item">
            <a class="nav-link text-light" href="./humid.php">Humidity</a>
            </li>
            <li class="nav-item">
            <a class="nav-link text-light" href="./mois.php">Moisture</a>
            </li>
        </ul>
    </nav>

    <?php
        // Create connection
        $conn = new mysqli("localhost", "root", "", "minipj");
        // Check connection
        if ($conn->connect_error) 
        {
            die("Connection failed: " . $conn->connect_error);
        }

        echo '
            <div class="container-fluid p-5 bg-dark text-white">
                <h3>State</h3>
            </div>
        ';
        
        // ****************************************************
        // AutoState

        $sql = "SELECT * FROM autostate ORDER by id DESC LIMIT 20";
        $result = $conn->query($sql);

        echo '
            <div class="container mt-3">
                <h2>AutoState</h2>          
                <table class="table table-dark table-hover table-sm">
                    <thead>
                    <tr>
                        <th>ID</th>
                        <th>state</th>
                        <th>Date</th>
                    </tr>
                </thead>
        ';

        while($row = $result->fetch_assoc()) 
        {
            echo '
                    <tbody>
                    <tr>
                        <td>'.$row['id'].'</td>
                        <td>'.$row['state'].'</td>
                        <td>'.$row['date'].'</td>
                    </tr>
                    </tbody>

            ';
        }

        echo '
                </table>
            </div>
        ';

        // ****************************************************
        // FanState

        $sql = "SELECT * FROM fanstate ORDER by id DESC LIMIT 20";
        $result = $conn->query($sql);

        echo '
            <div class="container mt-3">
                <h2>FanState</h2>          
                <table class="table table-dark table-hover table-sm">
                    <thead>
                    <tr>
                        <th>ID</th>
                        <th>state</th>
                        <th>Date</th>
                    </tr>
                </thead>
        ';

        while($row = $result->fetch_assoc()) 
        {
            echo '
                    <tbody>
                    <tr>
                        <td>'.$row['id'].'</td>
                        <td>'.$row['state'].'</td>
                        <td>'.$row['date'].'</td>
                    </tr>
                    </tbody>

            ';
        }

        echo '
                </table>
            </div>
        ';

        // ****************************************************
        // FanState

        $sql = "SELECT * FROM pumpstate ORDER by id DESC LIMIT 20";
        $result = $conn->query($sql);

        echo '
            <div class="container mt-3">
                <h2>PumpState</h2>          
                <table class="table table-dark table-hover table-sm">
                    <thead>
                    <tr>
                        <th>ID</th>
                        <th>state</th>
                        <th>Date</th>
                    </tr>
                </thead>
        ';

        while($row = $result->fetch_assoc()) 
        {
            echo '
                    <tbody>
                    <tr>
                        <td>'.$row['id'].'</td>
                        <td>'.$row['state'].'</td>
                        <td>'.$row['date'].'</td>
                    </tr>
                    </tbody>

            ';
        }

        echo '
                </table>
            </div>
        ';

        $conn->close();
    ?>



</body>
</html>