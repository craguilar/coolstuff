#!/usr/bin/perl -w 
use strict;
use Math::GMPz qw(:mpz);

my $x = Rmpz_init_set_str($ARGV[0],10);
my $y = Rmpz_init_set_str($ARGV[1],10);
my $z = Rmpz_init_set_str($ARGV[2],10);
my $res = Rmpz_init2(2048);
Rmpz_powm_sec($res, $x, $y, $z);
print "\n RESULTADO (DEC) \n";
Rmpz_out_str($res, 10);
print "\n RESULTADO (HEX) \n";
Rmpz_out_str($res, 16);
print "\n";
