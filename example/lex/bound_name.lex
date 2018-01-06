// Local or non local name can be referenced
$TOKEN {
	A  = / A / -> { $FRAG };
	C  = / C /;
	B  = / B{A}{C} /;
};
