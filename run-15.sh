set -v
echo "== Q =="
time ./q tests/test15
time ./q tests/test15

echo "== Python =="
time node tests/test15.js
time node tests/test15.js

echo "== PHP =="
time php tests/test15.php
time php tests/test15.php
