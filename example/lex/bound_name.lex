// Local or non local name can be referenced

$TOKEN
{
    A  = / A / -> $FRAGMENT ;
    C  = / C / ;
    B  = / B{A}{C} / ;
};
