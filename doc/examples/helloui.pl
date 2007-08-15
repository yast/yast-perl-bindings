#! /usr/bin/perl -w
use YaST::YCP qw(:DATA :UI);
YaST::YCP::Import "UI";
YaST::YCP::init_ui ($ARGV[0] || "qt");

# http://forgeftp.novell.com/yast/doc/SL10.2/tdg/Book-UIReference.html

my $c = VBox(
    Label("Now we can call YaST UI from other languages!"),
    PushButton("&So What?")
    );

UI->OpenDialog($c);
UI->UserInput();
UI->CloseDialog();
