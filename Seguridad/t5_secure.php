<?php

function verArchivo($file) {
    $ruta = __DIR__ . "/uploads/" . $file;

    if (!file_exists($ruta)) {
        echo "Archivo no encontrado.";
        return;
    }

    echo "<h3>Contenido del archivo:</h3>";
    echo "<pre>";
    
    // Leer archivo de forma segura
    echo htmlspecialchars(file_get_contents($ruta));
    
    echo "</pre>";
}

if (isset($_FILES['file'])) {

    // Verificar errores de subida
    if ($_FILES['file']['error'] !== UPLOAD_ERR_OK) {
        echo "Error al subir el archivo.";
        exit;
    }

    $tmp = $_FILES['file']['tmp_name'];
    $filename = basename($_FILES['file']['name']); // evita rutas maliciosas

    // Validar nombre (solo caracteres seguros)
    if (!preg_match('/^[a-zA-Z0-9._-]+$/', $filename)) {
        echo "Nombre de archivo no permitido.";
        exit;
    }

    // Validar extensión (ejemplo: solo txt)
    $ext = strtolower(pathinfo($filename, PATHINFO_EXTENSION));
    $permitidos = ['txt'];

    if (!in_array($ext, $permitidos)) {
        echo "Solo se permiten archivos .txt";
        exit;
    }

    // Crear carpeta si no existe
    $uploadDir = __DIR__ . "/uploads";
    if (!is_dir($uploadDir)) {
        mkdir($uploadDir, 0755, true);
    }

    // Evitar sobrescritura (agregar timestamp)
    $nuevoNombre = time() . "_" . $filename;
    $destino = $uploadDir . "/" . $nuevoNombre;

    // Mover archivo de forma segura
    if (!move_uploaded_file($tmp, $destino)) {
        echo "Error al mover el archivo.";
        exit;
    }

    // Mostrar contenido
    verArchivo($nuevoNombre);
}
?>

<!DOCTYPE html>
<html>
<body>
    <h2>Sube un archivo para ver su contenido</h2>
    <form method="POST" enctype="multipart/form-data">
        <input type="file" name="file" required>
        <button type="submit">Subir</button>
    </form>
</body>
</html>