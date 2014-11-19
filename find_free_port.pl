#!/usr/bin/env perl
#
# Find and print a free port.
#

use strict;
use Socket;

socket(my $socket, PF_INET, SOCK_STREAM, 0) or die "$!\n";
bind($socket, pack_sockaddr_in(0, inet_aton('localhost'))) or die "$!\n";

my ($port, $addr) = sockaddr_in(getsockname($socket));

print "$port\n";
