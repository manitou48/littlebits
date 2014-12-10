#!/usr/bin/perl
#  update DAC with value from ADC
# getADC returns 12 bits in 2 bytes, decimal
# setDAC nnnn     hex value 0000 to ff00

$adccmd = "/usr/local/lb/ADC/bin/getADC xx";
$daccmd = "/usr/local/lb/DAC/bin/setDAC ";

while(1) {
  @output = `$adccmd`;
  chomp @output;
#  print "$output[0] $output[1]\n";
  $x = sprintf("%02x00", $output[0]);
  system($daccmd . $x);
  sleep 1;
}
