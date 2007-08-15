#! /usr/bin/perl -w
# this was the first example, seen on
# http://mvidner.blogspot.com/2007/06/yast-user-interface-library-useable.html
use YaST::YCP qw(:DATA);
YaST::YCP::Import "UI";
YaST::YCP::init_ui ($ARGV[0] || "qt");

my $c = Term("VBox",
    Term("Label", "Now we can call YaST UI from other languages!"),
    Term("PushButton", "&So What?")
    );

UI->OpenDialog($c);
UI->UserInput();
UI->CloseDialog();
