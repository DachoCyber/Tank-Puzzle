<?php
header('Content-Type: application/json');
header('Access-Control-Allow-Origin: *');

$method = $_SERVER['REQUEST_METHOD'];
if ($method !== 'GET' && $method !== 'POST') {
    http_response_code(405);
    die(json_encode(['error' => 'Only GET or POST methods are allowed']));
}

$file = 'data.json';


// Ako fajl ne postoji → napravi ga kao []
if (!file_exists($file)) {
    if (file_put_contents($file, '[]') === false) {
        http_response_code(500);
        die(json_encode(['error' => 'Cannot create data file']));
    }
}

if (!is_readable($file)) {
    http_response_code(403);
    die(json_encode(['error' => 'Cannot read score data']));
}

$json = file_get_contents($file);
if ($json === false) {
    http_response_code(500);
    die(json_encode(['error' => 'Failed to read data']));
}

// Ako je prazan ili nevalidan → []
$data = json_decode($json, true);
if (!is_array($data)) {
    $data = [];
}

// Sortiranje (najveći score prvi)
usort($data, function($a, $b) {
    return ($a['score'] ?? 0) - ($b['score'] ?? 0);
});

// Uzmi top 10
$data = array_slice($data, 0, 10);

// Reset indeksa i vrati
echo json_encode(array_values($data), JSON_PRETTY_PRINT);
?>