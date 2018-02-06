$STATE
{
    ABC,
    DEF => $INITIAL,
};

$TOKEN
{
    ( DEF ) B = / A /, ( $BEGIN (ABC) );
    ( ABC ) C = / A /;
};
