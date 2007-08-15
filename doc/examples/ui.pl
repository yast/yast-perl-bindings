#! /usr/bin/perl -w
# Ported core/libyui/doc/examples/Label3.ycp
# - remove backticks:
# -- use helper functions instead of YCP terms
# -- use strings instead of YCP symbols (except in Value)

use YaST::YCP qw(:DATA :UI);
YaST::YCP::Import "UI";
YaST::YCP::init_ui ($ARGV[0] || "qt");

# Build dialog with one label, 4 Beatles buttons and an OK button.
UI->OpenDialog(
	       VBox(
		    Label("Select your favourite Beatle:"),
		    Label(id("beatle"), opt("outputField"), "   "),
		    HBox(
			 PushButton(id("john"),		"&John"),
			 PushButton(id("paul"),		"&Paul"),
			 PushButton(id("george"),	"&George"),
			 PushButton(id("ringo"),	"&Ringo")),
		    PushButton(id("ok"), "&OK")
		   )
	      );

# Wait for user input.
my $button;
# Input loop that only the OK button will leave.
# The 4 Beatles buttons will just propose a name.
do {
    # UserInput gives us the id, which is a YaST::YCP::Symbol
    $button = UI->UserInput()->value();

    if    ( $button eq "john" )		{ UI->ChangeWidget(id("beatle"), Symbol("Value"), "John Lennon"); }
    elsif ( $button eq "paul" )		{ UI->ChangeWidget(id("beatle"), Symbol("Value"), "Paul McCartney"); }
    elsif ( $button eq "george" )	{ UI->ChangeWidget(id("beatle"), Symbol("Value"), "George Harrison"); }
    elsif ( $button eq "ringo" )	{ UI->ChangeWidget(id("beatle"), Symbol("Value"), "Ringo Starr" ); }

    # Recalculate the layout - this is necessary since the label widget
    # doesn't recompute its size upon changing its value.
    UI->RecalcLayout();

} until ( $button eq "ok" );


# Retrieve the label's value.
# Perl note: Symbol is necessary for now
my $name = UI->QueryWidget(id("beatle"), Symbol("Value"));

# Close the dialog.
# Remember to read values from the dialog's widgets BEFORE closing it!
UI->CloseDialog();

# Pop up a new dialog to echo the input.
UI->OpenDialog(
	       VBox(
		    VSpacing(),
		    HBox(
			 Label("You selected:"),
			 Label(opt("outputField"), $name),
			 HSpacing()
			),
		    PushButton(opt("default"), "&OK")
		   )
	      );
UI->UserInput();
UI->CloseDialog();
