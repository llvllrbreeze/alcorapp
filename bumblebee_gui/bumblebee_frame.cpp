/////////////////////////////////////////////////////////////////////////////
// Name:        bumblebee_frame.cpp
// Purpose:     
// Author:      Andrea Carbone
// Modified by: 
// Created:     09/03/2007 01:01:27
// RCS-ID:      
// Copyright:   Alcor
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "bumblebee_frame.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
#include "gl_point_cloud_canvas.h"
////@end includes

#include "bumblebee_frame.h"

////@begin XPM images
/* XPM */
static char *beeico_small_xpm[] = {
/* columns rows colors chars-per-pixel */
"32 32 629 2",
"W@ c #724D11",
":+ c #7E6619",
"2O c #78641A",
"^+ c #846B1C",
"'O c #7E691C",
"rO c #7B651B",
"4X c #725F1A",
"}. c #6F5E1A",
"Z. c #695919",
"F+ c #846E1E",
"Io c #78641C",
"8X c #75631C",
"`  c #5D4C17",
"]O c #7E691E",
"q@ c #483614",
"G+ c #846E20",
"-o c #78641E",
"Q. c #7B651F",
"yo c #6F5B1D",
"B+ c #664F1C",
"sO c #7E6C23",
"*X c #7B6521",
"HX c #786421",
"qX c #726823",
"1o c #7B6B24",
"tX c #786723",
"D  c #63541F",
"5+ c #817026",
"Eo c #7B6B25",
"aX c #786423",
"fO c #7E6F27",
"Do c #937C29",
"c. c #7B6524",
"pX c #786424",
"c@ c #877228",
"T. c #7B6525",
">o c #453E1E",
"l. c #7E6927",
",. c #786427",
"w+ c #362D1D",
"R+ c #24211C",
"|+ c #27251E",
"[o c #AB9033",
"bo c #7E722E",
"s@ c #877830",
"qo c #9F8934",
"U+ c #847733",
"q+ c #66552B",
"/  c #7B682F",
"J. c #78672F",
"ZX c #7E7835",
"v+ c #847735",
".O c #726230",
"/o c #817635",
"*@ c #877B38",
"RX c #363329",
"P+ c #877B39",
"r. c #786734",
"PO c #847A3A",
"C  c #7E6C36",
"@. c #7B6B36",
"IO c #847A3B",
"Ao c #B7A042",
"v  c #7B6B37",
"{o c #BDA243",
"r  c #7B6B38",
"ao c #36362E",
"e  c #7B6B39",
"L  c #786A39",
"x  c #7B6B3A",
"H  c #726539",
"j  c #72653A",
"5  c #81703E",
"$  c #7B6B3D",
"*+ c #684809",
"QO c #6B4C0B",
"n@ c #7D6417",
"V. c #6B5816",
"6. c #655014",
"D+ c #836C1B",
",+ c #80681A",
"'. c #6E5917",
"0O c #715A18",
"Oo c #7A631A",
"[. c #715D19",
"3X c #6E5918",
"`O c #7D671C",
"^O c #7A601A",
"IX c #7A631B",
"D. c #6E5F1B",
"Z+ c #83691D",
"|. c #6B5E1C",
"N@ c #685119",
"f@ c #806B1F",
"h+ c #836F20",
"oX c #74641E",
"4@ c #867021",
"SO c #7D6A20",
"]o c #8F7622",
"pO c #7D6A21",
")X c #7A6921",
"H+ c #836F23",
"Fo c #6E5C20",
"q. c #6E5F21",
"<@ c #624C1D",
"kO c #7D6D25",
"v. c #776222",
"J+ c #837227",
"lO c #746123",
";@ c #382C19",
"l+ c #837228",
"1. c #776224",
"SX c #86792B",
"i+ c #715723",
"vo c #7D702A",
"n+ c #2F291D",
"JO c #80712C",
";. c #7D6A2A",
"I. c #7A6629",
"m+ c #26231D",
"9@ c #86762F",
"#+ c #29271F",
"<O c #80742F",
"=. c #7D6A2C",
"}o c #836F2E",
"H. c #77652C",
"gO c #807431",
"L+ c #867933",
"LO c #837836",
"u. c #7A6933",
"kX c #353128",
"hO c #837838",
"M  c #7D6D35",
"Mo c #807738",
"M@ c #564E30",
"O. c #7A6936",
" . c #776837",
"i  c #7D6D38",
"0+ c #867C3D",
">  c #776838",
"K  c #776839",
"k  c #746739",
"]X c #837E3F",
"Vo c #988B42",
"d  c #83723D",
"T@ c #76540A",
"Y@ c #79550A",
"L@ c #79580B",
"vO c #674609",
"R@ c #704F0C",
"   c None",
"3O c #735C13",
"S+ c #826A1A",
"NO c #796118",
"+@ c #856E1D",
"<+ c #7F691C",
"]. c #705B19",
"{. c #705E1A",
"0. c #6A5C1A",
"%O c #7C681D",
"@O c #79641C",
"cX c #735F1B",
"7X c #76631C",
"bO c #6A4D17",
"5X c #735F1C",
"(X c #79671E",
"bX c #76631D",
"{O c #7F6C20",
"iO c #7C681F",
"jo c #79671F",
"eX c #796D21",
"0X c #706420",
"C@ c #856B21",
"Ro c #7C6B22",
"JX c #796420",
"#@ c #857124",
"sX c #796421",
"k+ c #827025",
"Lo c #64511E",
"`. c #67551F",
"%X c #796422",
"WX c #645420",
"HO c #7F6F27",
"Y. c #796424",
"OX c #766625",
"v@ c #857128",
"n. c #675523",
"$X c #796426",
"3o c #7C6E2A",
"|X c #917E2E",
"C+ c #735926",
"t+ c #2E2A1E",
"(o c #7F722D",
"M+ c #25211D",
"zO c #645427",
"<X c #554624",
"+X c #79672B",
"!o c #7F722F",
"@X c #79672C",
"UO c #827633",
"K. c #79672F",
"'X c #7C7433",
"2X c #4F472A",
"~  c #7C6B31",
"&@ c #887B36",
"mo c #7F7535",
"E  c #7C6B32",
"e. c #766631",
"|  c #736532",
"%. c #7C6B33",
"XO c #52482D",
"No c #827938",
"X+ c #857A39",
"R  c #7C6B35",
"b  c #7C6B36",
"s  c #7F6F39",
"X. c #796A38",
"P  c #796A39",
"O  c #736539",
"h  c #70643A",
"1  c #7C6B3B",
"4  c #7F6F3D",
"[X c #8E8945",
"=  c #73683F",
"6o c #888448",
"7o c #949153",
"l@ c #6F530F",
"E@ c #754F11",
"s+ c #816819",
"(O c #7B6318",
"9. c #665917",
"y+ c #694E14",
"BO c #7B6319",
"OO c #78621A",
"NX c #78651B",
"do c #75611A",
"XX c #72631B",
".X c #6F621B",
" X c #6C611C",
"UX c #7B631B",
"MX c #78651C",
"xX c #725D1A",
"~+ c #846C1E",
"nO c #755B1A",
"gX c #7B631C",
"mX c #78651D",
"6X c #75611C",
"p+ c #81681E",
"eo c #816B1F",
"^X c #78651E",
"9X c #72631E",
"-X c #7B631E",
"zo c #7B6921",
"GX c #786520",
"FO c #7E6D23",
"oO c #6C581E",
")+ c #847226",
"dO c #7E6D25",
"^. c #7E6724",
"2o c #7B6C26",
"R. c #7B6625",
"r@ c #392F1B",
"6+ c #817129",
"&o c #5A4C21",
"iX c #786526",
"K+ c #84752B",
":O c #7E702B",
"jX c #514321",
"T+ c #2D281D",
"r+ c #24221C",
":@ c #2A271F",
"%@ c #877931",
"o@ c #755B29",
"f. c #7B692D",
"CX c #787130",
"So c #A89037",
"8@ c #877934",
"'+ c #877C37",
".+ c #847836",
"^o c #817736",
"$. c #7B6932",
"EX c #363128",
"#. c #7B6933",
"t. c #786833",
"$o c #36342A",
"po c #3F3A2B",
"]+ c #877C3A",
"I+ c #877C3B",
",  c #786837",
"g  c #6C6137",
"Co c #AB9743",
"c  c #7B6C3A",
"0  c #756739",
"+  c #75673A",
"@o c #6C5E39",
"9o c #AE9E4D",
"o  c #817448",
"F@ c #77570C",
"y@ c #6E4E0A",
"6O c #6B4D0A",
"&+ c #6B4D0C",
"D@ c #775A10",
"U@ c #7D5913",
"B. c #6B5615",
"d+ c #80691A",
"/O c #7A6118",
"A. c #685B18",
"]  c #625316",
"8O c #6B5015",
"C. c #6B5918",
"ZO c #7D681D",
"YX c #77601B",
"go c #77631C",
"2+ c #806C1F",
"AO c #7D681E",
"Uo c #7A671E",
"BX c #77661E",
"O@ c #866E20",
",o c #71611E",
"S  c #5F521C",
"N. c #68551C",
"aO c #7D6B22",
"dX c #7A6420",
"=O c #7D6B23",
")o c #7A6A23",
"=X c #7A6421",
"wX c #776C24",
"&X c #7A6422",
"5@ c #867427",
"x@ c #897227",
"w@ c #2F2718",
";O c #7D6E27",
"x. c #7D6825",
"<. c #776325",
"e@ c #2C261A",
"$@ c #86742A",
"|O c #80722A",
"YO c #7A6A28",
"k. c #7D6827",
" @ c #322B1C",
"`X c #7A6D2A",
"@+ c #47381F",
"$+ c #26241D",
"7+ c #83762F",
"4o c #7D712E",
"F  c #685B29",
">@ c #292820",
"(  c #7A672D",
"EO c #3B3424",
"!  c #7D6B2F",
"L. c #7A672E",
"Q  c #7D6B30",
"p. c #7A672F",
"}X c #988939",
"m@ c #746231",
"N  c #7D6B35",
"+. c #7A6A36",
"Jo c #5C5433",
"o. c #7A6A38",
"a  c #806F3A",
"<  c #7A6A39",
"q  c #77693B",
"#  c #7A6A3E",
"P@ c #76550A",
"B@ c #73540A",
"1@ c #6D4C0A",
"!O c #674709",
"7O c #70530C",
"cO c #6A4B0B",
";+ c #7C6317",
"7. c #645214",
">+ c #7F6719",
"wO c #765E17",
"'o c #7C6619",
"8. c #645516",
"eO c #796219",
"5. c #644F15",
"Z@ c #82681B",
"+O c #79621A",
"Po c #76611A",
"tO c #7C661C",
"oo c #79621B",
"/X c #79651C",
"@@ c #856F1F",
"1+ c #7F6A1E",
"yO c #7C661D",
":X c #79621C",
"~X c #76641D",
"4. c #614E18",
"[O c #7F6A1F",
"&O c #7C691F",
"LX c #79621D",
";o c #79651E",
"(. c #64521A",
"*O c #7C6921",
"3@ c #887023",
"qO c #6A571E",
"W+ c #644C1C",
" o c #857225",
"io c #5E501F",
"2. c #766123",
"wo c #8E7828",
"Wo c #7C6C27",
"#o c #52461F",
"z. c #7F6A26",
"E. c #7C6625",
"VX c #766A27",
"U. c #796526",
"/. c #675624",
"_+ c #85752C",
"!+ c #735A26",
"RO c #312C21",
">O c #7F7330",
"no c #7F7331",
"Ko c #403725",
"y. c #796833",
"n  c #7C6C35",
"9+ c #857B3A",
"t  c #7C6C36",
"TX c #675932",
"y  c #7C6C37",
"o+ c #857B3C",
"U  c #7C6C39",
"l  c #766738",
"A  c #82713B",
"{X c #978D43",
"%  c #7C6C3D",
"9  c #73663C",
".  c #796B41",
"6  c #857544",
"X  c #7C6F44",
"8o c #A39A54",
"Go c #7C6F4D",
"|o c #857856",
"9O c #8B7D5C",
"K@ c #7B5B0C",
"u+ c #694909",
"V@ c #6F4E0B",
"5O c #6C500C",
"S@ c #7E6213",
"H@ c #816318",
"'  c #604F15",
"g@ c #634A14",
"/+ c #846D1C",
"VO c #7B641A",
"g+ c #816C1D",
"QX c #725E1A",
"to c #7B641B",
"-+ c #7E651C",
"WO c #664E17",
"!X c #75621C",
"M. c #665118",
"ro c #7B641C",
"$O c #7B671E",
"(+ c #847021",
"Yo c #7B671F",
",X c #66541B",
"3+ c #816F22",
"DO c #7E6B21",
"<o c #7B6A21",
"~O c #6C501B",
"Ho c #6C591E",
"b. c #755F20",
"{  c #695B20",
"~. c #7E6822",
"j@ c #3C3118",
"4+ c #816F25",
"xo c #7B6A24",
"=+ c #6F541F",
"p@ c #877428",
"so c #57491F",
",@ c #332B1A",
"yX c #786626",
"h. c #7B6727",
"E+ c #302A1C",
"6@ c #87772D",
">. c #7B6729",
"-. c #7E6B2B",
"Y+ c #574624",
"z+ c #84762F",
"g. c #7B672B",
"2@ c #755C29",
"k@ c #4B3F25",
"[+ c #847631",
"++ c #72612C",
"KO c #817531",
"~o c #817533",
"_  c #7E6B30",
"d. c #7B6A30",
"3. c #78662F",
"s. c #7B6A31",
"1X c #393329",
"f  c #695E33",
"0o c #AB9841",
"Bo c #8A813E",
"p  c #7E6E39",
".. c #786938",
"J  c #756839",
"z  c #786939",
"w  c #78693A",
";  c #75683B",
"3  c #7E6E3D",
"@  c #78693C",
"&  c #817242",
"*  c #847646",
"G@ c #74540B",
"Q+ c #6B4B09",
"J@ c #7D5D10",
"MO c #775E16",
"A+ c #806719",
"_O c #7D661A",
"S. c #6B5D19",
"f+ c #806A1C",
"CO c #7D661B",
"#O c #7A651C",
"_o c #77641C",
"PX c #7A621B",
"*o c #74601B",
";X c #7A621C",
"nX c #77641D",
"vX c #74601C",
"-@ c #624E19",
".o c #7D691F",
"t@ c #68501A",
"F. c #71621F",
"}O c #806D22",
"!. c #7D6620",
"1O c #7D6C23",
"W. c #7A6521",
"_X c #7A6B24",
".@ c #5F4A1D",
"-O c #7D6C25",
"i@ c #897327",
"h@ c #503F1D",
"G. c #746326",
"}+ c #2F281B",
"Qo c #7D6F2A",
"N+ c #2C271C",
"). c #625124",
"ko c #4A4322",
"d@ c #86752D",
"%+ c #473C21",
":. c #7D692A",
"}  c #6E612A",
"P. c #7A682D",
"`+ c #867832",
"hX c #65552B",
")  c #7D692F",
"W  c #7D6C30",
"a. c #7A682F",
"O+ c #837735",
"i. c #7A6831",
"m  c #7D6C34",
"x+ c #867B3A",
"T  c #7D6C36",
"5o c #80793A",
"G  c #6E6135",
"c+ c #867B3C",
"u  c #7D6C38",
"I  c #7A6B39",
"7  c #6E643B",
"2  c #7D6C3C",
"8  c #71653D",
"uo c #746643",
"V+ c #6A490A",
"A@ c #7F6515",
"a+ c #7F6517",
")O c #7C6418",
"mO c #765C16",
"`o c #766219",
"[  c #645618",
" O c #705A19",
"+o c #7C641B",
"fo c #76621B",
"b@ c #826C1F",
"0@ c #735E1C",
"uO c #7C671E",
"Xo c #79631D",
"ho c #79661E",
"=o c #76621D",
"m. c #645019",
"xO c #6A551B",
"To c #76651F",
"fX c #79631E",
":o c #79661F",
"rX c #796920",
"zX c #6D591C",
"FX c #7C6A21",
"KX c #79631F",
"lo c #6D5C1E",
"j+ c #826F23",
"DX c #827225",
"GO c #7F6E25",
"co c #7C6D27",
"{+ c #5B4A21",
"j. c #7C6727",
"uX c #796627",
"a@ c #88772D",
"w. c #736429",
">X c #675726",
"e+ c #25231D",
"#X c #79662A",
"u@ c #765C28",
"z@ c #765C29",
"=@ c #857630",
"b+ c #5B4A26",
" + c #827530",
"7@ c #887A32",
"jO c #827531",
"AX c #857C33",
"*. c #7C6A2F",
"TO c #5E4E2A",
"&. c #7C6A30",
"8+ c #857935",
"^  c #7C6A31",
"V  c #7C6A32",
"%o c #343128",
"B  c #7C6A33",
",O c #827838",
"_. c #52472E",
"lX c #3A362B",
"Z  c #7F6E37",
"Y  c #7C6D39",
"Zo c #BBA346",
":  c #76683A",
"-  c #73673D",
"X@ c #6C4B09",
"I@ c #6F4F0C",
"4O c #6F550E",
/* pixels */
"          . X o                                                 ",
"        O + @ # $ % & *                                         ",
"      = - ; : > , < 1 2 3 4 5 6                                 ",
"    7 8 9 + 0 q w < e r t y u i p a s d                         ",
"  f g h j k 0 l z x c e v b n m m M N B V C Z A                 ",
"S D F G H k J K L P I r U Y i T R E W Q ! ~ ^ / ( ( ) _         ",
"` ' ] [ { } |  ...X.o.O.+.@.R #.$.%.V &.*.=.-.;.:.>.,.<.1.2.,.3.",
"4.5.6.7.8.9.0.q.w.e.r.t.y.u.i.p.a.s.d.f.g.>.h.j.k.l.z.x.c.v.b.n.",
"m.M.N.B.V.C.Z.A.S.D.F.G.H.J.K.K.L.P.( I.U.Y.Y.T.R.E.W.Q.!.~.^./.",
"(.)._.`.'.].[.{.}.|. X.XXXoXOX+X@X@X#XU.U.$XY.%X&X*X=X-X;X;X:X>X",
",X<X1X2X3X[.4X5X6X7X8X9X0XqXwXeXrXtXyXuXiXpXaXsXsXdXfX;X;XgX:XhX",
"  jXkXlXzXxXcXvXbXnXmXMXNXBXVXCXZXAXSXDXFXGXHXJXKXLXLXPXIXUXYXTX",
"  jXRXEXWXQXcX!X~X^X^X/X4X(X)X_X`X'X]X[X{X}X|X o.oXo:XooOo+o5X@o",
"  #o$o%o&o*o6X=o-o;o:omX>o,o<o1o2o3o4o5o6o7o8o9o0oqowoeorotoyouo",
"  iopoaosodofogo;ohojohokolozoxocovobonomoMoNoBoVoCoZoAoSoDoFoGo",
"  HoJoKoLoPofoIo;oUoYoYoTojoRoEoWoQo!o~o^o/o(o)o_o`o'o]o[o{o}o|o",
"   O.OXOoOOO+O@O#O$O%O&O&O*O=O-O;O:O>O~o^o,O<O1O2O3O4O5O6O7O8O9O",
"   O0OqOwOeOOorOtOyOuOiOpOaOsOdOfO:OgO/ohOhOjOkOlOzOxO5OcOvObO  ",
"    nOmOMONOBOVOCOZOZOAOSODOFOGOHOJOKOLOPOIOUOYOTOROEOWOQO!O~O  ",
"        ^O/O(O)O_O`O'O]O[O{O}OGOHO|O +.+X+o+O+++@+#+$+%+&+*+=+  ",
"            -+;+:+>+,+<+1+2+3+4+5+6+7+8+9+0+.+q+w+e+r+t+y+u+i+  ",
"                p+a+s+d+f+g+h+j+k+l+z+8+x+c+v+b+n+m+M+N+B+V+C+  ",
"                    Z+A+S+D+F+G+H+J+K+L+P+I+U+Y+T+m+R+E+W+Q+!+  ",
"                        ~+^+/+F+(+)+_+`+'+]+[+{+}+|+$+ @.@X@o@  ",
"                            O@+@@@#@$@%@&@*@=@-@;@:@>@,@<@1@2@  ",
"                                3@4@5@6@7@8@9@0@q@w@e@r@t@y@u@  ",
"                                    i@p@a@s@d@f@g@h@j@k@l@y@z@  ",
"                                        x@c@v@b@n@m@M@N@B@V@    ",
"                                            C@Z@A@S@D@F@G@V@    ",
"                                                H@J@K@L@P@I@    ",
"                                                    U@Y@T@R@    ",
"                                                        E@W@    "
};

////@end XPM images

/*!
 * bumblebee_frame type definition
 */

IMPLEMENT_CLASS( bumblebee_frame, wxFrame )

/*!
 * bumblebee_frame event table definition
 */

BEGIN_EVENT_TABLE( bumblebee_frame, wxFrame )

////@begin bumblebee_frame event table entries
    EVT_CLOSE( bumblebee_frame::OnCloseWindow )

    EVT_BUTTON( ID_BUTTON_CLOSE_GUI, bumblebee_frame::OnButtonCloseGuiClick )

////@end bumblebee_frame event table entries

END_EVENT_TABLE()

/*!
 * bumblebee_frame constructors
 */

bumblebee_frame::bumblebee_frame( )
{
    Init();
}

bumblebee_frame::bumblebee_frame( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create( parent, id, caption, pos, size, style );
}

/*!
 * bumblebee_frame creator
 */

bool bumblebee_frame::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin bumblebee_frame creation
    wxFrame::Create( parent, id, caption, pos, size, style );

    CreateControls();
    SetIcon(GetIconResource(wxT("beeico_small.png")));
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end bumblebee_frame creation
    return true;
}

/*!
 * Member initialisation 
 */

void bumblebee_frame::Init()
{
////@begin bumblebee_frame member initialisation
////@end bumblebee_frame member initialisation
}
/*!
 * Control creation for bumblebee_frame
 */

void bumblebee_frame::CreateControls()
{    
////@begin bumblebee_frame content construction
    bumblebee_frame* itemFrame1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemFrame1->SetSizer(itemBoxSizer2);

    wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer3, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    point_cloud_view = new point_cloud_canvas( itemFrame1, ID_POINTCLOUD_CANVAS, wxDefaultPosition, wxSize(800, 600), wxNO_BORDER );
    itemBoxSizer3->Add(point_cloud_view, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton5 = new wxButton( itemFrame1, ID_BUTTON_CLOSE_GUI, _("Close"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer2->Add(itemButton5, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

////@end bumblebee_frame content construction

}

/*!
 * Should we show tooltips?
 */

bool bumblebee_frame::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap bumblebee_frame::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin bumblebee_frame bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end bumblebee_frame bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon bumblebee_frame::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin bumblebee_frame icon retrieval
    wxUnusedVar(name);
    if (name == _T("beeico_small.png"))
    {
        wxIcon icon(beeico_small_xpm);
        return icon;
    }
    return wxNullIcon;
////@end bumblebee_frame icon retrieval
}


//}
/*!
 * wxEVT_CLOSE_WINDOW event handler for ID_BUMBLEBEE_FRAME
 */

void bumblebee_frame::OnCloseWindow( wxCloseEvent& WXUNUSED(event) )
{
  point_cloud_view->Destroy();
  Destroy(); 
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON_CLOSE_GUI
 */

void bumblebee_frame::OnButtonCloseGuiClick( wxCommandEvent& event )
{
  wxCloseEvent dummy;
  OnCloseWindow(dummy);
}


