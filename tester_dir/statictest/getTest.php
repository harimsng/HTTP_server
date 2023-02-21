
<!DOCTYPE html>
<html lang="en" >
<head>
  <meta charset="UTF-8">
  <title>Webserv Form</title>
    <link href='https://fonts.googleapis.com/css?family=Open+Sans:400,800' rel='stylesheet' type='text/css'>
    <link rel="stylesheet" href="https://unpkg.com/@picocss/pico@latest/css/pico.min.css">
</head>
<body>
<main class="container">
  <div>
    <h2>
    Welcome <?php $_GET["name"]; ?><br>
    Your email address is: <?php echo $_GET["email"]; ?><br>
    The sum of your numbers is <?php
      $sum = (int)$_GET["one"] + (int)$_GET["two"];
      echo $sum;
    ?><br>
  </h2>
    <a class="button-line" href="/">Go back Home</a>
  </div>
</main>
</body>
</html>
