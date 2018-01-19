$TOKEN
{
    PLUS    = / "+"          /   ;
    MINUS   = / "-"          /   ;
    MUL     = / "*"          /   ;
    DIV     = / "/"          /   ;
    LPAREN  = / "("          /   ;
    RPAREN  = / ")"          /   ;
    NUM     = / [[:digit:]]+ /   ;
};

$SKIP
{
    SPACE   = / [ \n\t]+     /   ;
};
