#!/usr/local/bin/perl
 
# for loop execution
$dummy;
for ( $a = 1000000; $a; $a = $a - 1) {
    $dummy = $a;
}
print $dummy
