60 60 *             constant SECS_PER_HOUR
\ SECS_PER_HOUR 24 *  constant SECS_PER_DAY

: ut2time ( unixtime -- second minute hour wday days )
    60 /mod \ seconds
    60 /mod \ minutes 
    24 /mod \ hours
    dup
    4 + 7 mod \ week day. 0 - Sunday, 1 - Monday, ... 6 - Satuday
    dup 0< if 7 + then
    swap \ days on top
;

: leap? ( year -- b ) 4 mod 0= ;

create -mon-yday
    31 c, 28 c,
    31 c, 30 c, 31 c,
    30 c, 31 c, 31 c,
    30 c, 31 c, 30 c,
    31 c,

: days2date ( days -- day month year )
    1970 \ Calculate year
    begin
        365 over leap? abs + \ Days per year
        ( days year dpy )
        rot ( year dpy days )
        over over ( year dpy days dpy days )
    <= while
        swap ( year days dpy ) - ( year days )
        swap 1+ ( days year )
    repeat
    nip
    \ Calculate month
    1 swap \ Month
    -mon-yday
    -rot
    begin
        2 pick c@ \ Day per month
        2 pick 2 = if 1+ then \ Leap feb correction
        over over
    >= while
        -
        rot 1+ rot 1+ rot \ Next month
    repeat
    drop 1+
    rot drop
    swap rot
;

: utc2msk ( ut_utc -- ut_msk )
    SECS_PER_HOUR 3 * + 
;

: ut2datetime ( unixtime -- second minute hour wday day month year )
    ut2time days2date
;

( datetime end: ) here hex.

\ Simple test
\ 1518512517 utc2msk ut2datetime cr . . . . . . . cr
\ : test cr 18500 17000 do i days2date . . . cr loop ;
\ test

