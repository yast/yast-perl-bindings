package simple1;

#BEGIN { $TYPEINFO{hello} = ["function", "void"]; }
BEGIN { $TYPEINFO{hello} = ["function", "string"]; }
sub hello
{
#    print "Hello, world\n";
    return "Hello, world";
}

1;
