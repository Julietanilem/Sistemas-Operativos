<?php

function verArchivo($file) {
    echo "<h3>Contenido del archivo:</h3>";
    echo "<pre>";
    system("cat uploads/" . $file);  // vulnerable a inyección de comandos
    echo "</pre>";
}

if (isset($_FILES['file'])) {

    $filename = $_FILES['file']['name'];
    $tmp = $_FILES['file']['tmp_name'];

    // Crear carpeta uploads si no existe
    if (!is_dir("uploads")) {
        mkdir("uploads");
    }

    echo "<pre>";
    system("mv " . $tmp . " uploads/" . $filename); // vulnerable a inyección de comandos
    echo "</pre>";


    verArchivo($filename);
}
?>

<!DOCTYPE html>
<html>
<body>
    <h2>Sube un archivo para ver su contenido</h2>
    <form method="POST" enctype="multipart/form-data">
        <input type="file" name="file">
        <button type="submit">Subir</button>
    </form>
</body>
</html>
