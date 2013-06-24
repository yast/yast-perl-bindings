package A::Nested;

BEGIN { $TYPEINFO{hello} = ["function", "string"]; }
sub hello
{
    return "Hello, world";
}

1;
