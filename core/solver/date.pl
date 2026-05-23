#!/usr/bin/perl

chomp($date=`date`);

# inserting the date into WeICME.c

@ARGV="WeICME.c";
$^I=".old";
while(<>){
    s/You are using an executable made on.*/You are using an executable made on $date\\n");/g;
    print;
}

if(-f "frd.c"){
    @ARGV="frd.c";
    $^I=".old";
    while(<>){
        s/COMPILETIME.*/COMPILETIME       $date                    \\n\",p1);/g;
        print;
    }
}

system "rm -f WeICME.c.old";
system "rm -f frd.c.old";
