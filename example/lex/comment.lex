$STATE
{
	INIT => $INITIAL,
	COMMENT,
};

$SKIP 
{
	( INIT, COMMENT )	BEG_COM	= / \(\* /, ( $BEGIN COMMENT );
	( COMMENT )			CHAR	= / .|\n /                    ;
	( COMMENT )			END_COM	= / \*\) /, ( $BEGIN INIT    );
};

$TOKEN
{
	( INIT ) INT = / int / -> { $IGCASE };
};
