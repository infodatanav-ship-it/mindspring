#!/usr/bin/perl

my @vowels=("a", "e", "i", "o", "u");
my @consononts=("b","c","d","f","g","h","j","k","l","m","n","p","q","r","s","t","v","w","x","y","z");
my $password = "";

sub makerand {
	$cindex=int(rand(20));
	$vindex=int(rand(4));
}

for ($i=0;$i<4; $i++) {
	&makerand;
	$password = $password . @consononts[$cindex];
	$password = $password . @vowels[$vindex];
	# print "Vowel Index $vindex Consonont Index $cindex\n";
}

print "Content-type: text/html\n\n";
print "New password : $password \n\n";
print "<a href='index.php'>Back</a>\n";
exit;
