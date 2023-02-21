<!DOCTYPE html>
<html lang="en" >
<head>
  <meta charset="UTF-8">
  <title>Webserv Project</title>
  <link href='https://fonts.googleapis.com/css?family=Open+Sans:400,800' rel='stylesheet' type='text/css'>
  <link rel="stylesheet" href="https://unpkg.com/@picocss/pico@latest/css/pico.min.css">

</head>
<body>
<main class="container">
  <div class="cc">
    <?php
      $target_dir = "/Users/soum/goinfre/webserv_1/webserv/tester_dir/put_test2/";
      $target_file = $target_dir.basename($_FILES["fileToUpload"]["name"]);
      if (move_uploaded_file($_FILES["fileToUpload"]["tmp_name"], $target_file))
      {
        echo '<div id="note">
            You can delete the file with:<br>
            curl -vX DELETE localhost:8080/uploads/'. htmlspecialchars(basename($_FILES["fileToUpload"]["name"])) .'
            </div>';

        echo '<h2>The file '. htmlspecialchars(basename($_FILES["fileToUpload"]["name"])). ' has been uploaded.</h2>';
      } else {
        echo '<h2>Sorry, there was an error uploading your file.</h2>';
      }
    ?>
  </div>
</main>
</body>
</html>
