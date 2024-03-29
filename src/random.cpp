// Copyright © 2022 Daylon Wilkins
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include <random.h>
#include <random>

std::random_device rd;
std::mt19937 mt(rd());
std::uniform_real_distribution<double> dist(0.0, std::nextafter(1.0, 2));

std::string preStrs[95] = {
	"Ba",
	"Be",
	"Bi",
	"Bo",
	"Bu",
	"Ca",
	"Ce",
	"Ci",
	"Co",
	"Cu",
	"Da",
	"De",
	"Di",
	"Do",
	"Du",
	"Fa",
	"Fe",
	"Fi",
	"Fo",
	"Fu",
	"Ga",
	"Ge",
	"Gi",
	"Go",
	"Gu",
	"Ha",
	"He",
	"Hi",
	"Ho",
	"Hu",
	"Ja",
	"Je",
	"Ji",
	"Jo",
	"Ju",
	"Ka",
	"Ke",
	"Ki",
	"Ko",
	"Ku",
	"La",
	"Le",
	"Li",
	"Lo",
	"Lu",
	"Ma",
	"Me",
	"Mi",
	"Mo",
	"Mu",
	"Na",
	"Ne",
	"Ni",
	"No",
	"Nu",
	"Pa",
	"Pe",
	"Pi",
	"Po",
	"Pu",
	"Ra",
	"Re",
	"Ri",
	"Ro",
	"Ru",
	"Sa",
	"Se",
	"Si",
	"So",
	"Su",
	"Ta",
	"Te",
	"Ti",
	"To",
	"Tu",
	"Va",
	"Ve",
	"Vi",
	"Vo",
	"Vu",
	"Wa",
	"We",
	"Wi",
	"Wo",
	"Wu",
	"Ya",
	"Ye",
	"Yi",
	"Yo",
	"Yu",
	"Za",
	"Ze",
	"Zi",
	"Zo",
	"Zu",
};

std::string midStrs[95] = {
	"ba",
	"be",
	"bi",
	"bo",
	"bu",
	"ca",
	"ce",
	"ci",
	"co",
	"cu",
	"da",
	"de",
	"di",
	"do",
	"du",
	"fa",
	"fe",
	"fi",
	"fo",
	"fu",
	"ga",
	"ge",
	"gi",
	"go",
	"gu",
	"ha",
	"he",
	"hi",
	"ho",
	"hu",
	"ja",
	"je",
	"ji",
	"jo",
	"ju",
	"ka",
	"ke",
	"ki",
	"ko",
	"ku",
	"la",
	"le",
	"li",
	"lo",
	"lu",
	"ma",
	"me",
	"mi",
	"mo",
	"mu",
	"na",
	"ne",
	"ni",
	"no",
	"nu",
	"pa",
	"pe",
	"pi",
	"po",
	"pu",
	"ra",
	"re",
	"ri",
	"ro",
	"ru",
	"sa",
	"se",
	"si",
	"so",
	"su",
	"ta",
	"te",
	"ti",
	"to",
	"tu",
	"va",
	"ve",
	"vi",
	"vo",
	"vu",
	"wa",
	"we",
	"wi",
	"wo",
	"wu",
	"ya",
	"ye",
	"yi",
	"yo",
	"yu",
	"za",
	"ze",
	"zi",
	"zo",
	"zu",
};

std::string postStr[855] = {
	"bad",
	"bed",
	"bid",
	"bod",
	"bud",
	"cad",
	"ced",
	"cid",
	"cod",
	"cud",
	"dad",
	"ded",
	"did",
	"dod",
	"dud",
	"fad",
	"fed",
	"fid",
	"fod",
	"fud",
	"gad",
	"ged",
	"gid",
	"god",
	"gud",
	"had",
	"hed",
	"hid",
	"hod",
	"hud",
	"jad",
	"jed",
	"jid",
	"jod",
	"jud",
	"kad",
	"ked",
	"kid",
	"kod",
	"kud",
	"lad",
	"led",
	"lid",
	"lod",
	"lud",
	"mad",
	"med",
	"mid",
	"mod",
	"mud",
	"nad",
	"ned",
	"nid",
	"nod",
	"nud",
	"pad",
	"ped",
	"pid",
	"pod",
	"pud",
	"rad",
	"red",
	"rid",
	"rod",
	"rud",
	"sad",
	"sed",
	"sid",
	"sod",
	"sud",
	"tad",
	"ted",
	"tid",
	"tod",
	"tud",
	"vad",
	"ved",
	"vid",
	"vod",
	"vud",
	"wad",
	"wed",
	"wid",
	"wod",
	"wud",
	"yad",
	"yed",
	"yid",
	"yod",
	"yud",
	"zad",
	"zed",
	"zid",
	"zod",
	"zud",
	"bag",
	"beg",
	"big",
	"bog",
	"bug",
	"cag",
	"ceg",
	"cig",
	"cog",
	"cug",
	"dag",
	"deg",
	"dig",
	"dog",
	"dug",
	"fag",
	"feg",
	"fig",
	"fog",
	"fug",
	"gag",
	"geg",
	"gig",
	"gog",
	"gug",
	"hag",
	"heg",
	"hig",
	"hog",
	"hug",
	"jag",
	"jeg",
	"jig",
	"jog",
	"jug",
	"kag",
	"keg",
	"kig",
	"kog",
	"kug",
	"lag",
	"leg",
	"lig",
	"log",
	"lug",
	"mag",
	"meg",
	"mig",
	"mog",
	"mug",
	"nag",
	"neg",
	"nig",
	"nog",
	"nug",
	"pag",
	"peg",
	"pig",
	"pog",
	"pug",
	"rag",
	"reg",
	"rig",
	"rog",
	"rug",
	"sag",
	"seg",
	"sig",
	"sog",
	"sug",
	"tag",
	"teg",
	"tig",
	"tog",
	"tug",
	"vag",
	"veg",
	"vig",
	"vog",
	"vug",
	"wag",
	"weg",
	"wig",
	"wog",
	"wug",
	"yag",
	"yeg",
	"yig",
	"yog",
	"yug",
	"zag",
	"zeg",
	"zig",
	"zog",
	"zug",
	"bam",
	"bem",
	"bim",
	"bom",
	"bum",
	"cam",
	"cem",
	"cim",
	"com",
	"cum",
	"dam",
	"dem",
	"dim",
	"dom",
	"dum",
	"fam",
	"fem",
	"fim",
	"fom",
	"fum",
	"gam",
	"gem",
	"gim",
	"gom",
	"gum",
	"ham",
	"hem",
	"him",
	"hom",
	"hum",
	"jam",
	"jem",
	"jim",
	"jom",
	"jum",
	"kam",
	"kem",
	"kim",
	"kom",
	"kum",
	"lam",
	"lem",
	"lim",
	"lom",
	"lum",
	"mam",
	"mem",
	"mim",
	"mom",
	"mum",
	"nam",
	"nem",
	"nim",
	"nom",
	"num",
	"pam",
	"pem",
	"pim",
	"pom",
	"pum",
	"ram",
	"rem",
	"rim",
	"rom",
	"rum",
	"sam",
	"sem",
	"sim",
	"som",
	"sum",
	"tam",
	"tem",
	"tim",
	"tom",
	"tum",
	"vam",
	"vem",
	"vim",
	"vom",
	"vum",
	"wam",
	"wem",
	"wim",
	"wom",
	"wum",
	"yam",
	"yem",
	"yim",
	"yom",
	"yum",
	"zam",
	"zem",
	"zim",
	"zom",
	"zum",
	"ban",
	"ben",
	"bin",
	"bon",
	"bun",
	"can",
	"cen",
	"cin",
	"con",
	"cun",
	"dan",
	"den",
	"din",
	"don",
	"dun",
	"fan",
	"fen",
	"fin",
	"fon",
	"fun",
	"gan",
	"gen",
	"gin",
	"gon",
	"gun",
	"han",
	"hen",
	"hin",
	"hon",
	"hun",
	"jan",
	"jen",
	"jin",
	"jon",
	"jun",
	"kan",
	"ken",
	"kin",
	"kon",
	"kun",
	"lan",
	"len",
	"lin",
	"lon",
	"lun",
	"man",
	"men",
	"min",
	"mon",
	"mun",
	"nan",
	"nen",
	"nin",
	"non",
	"nun",
	"pan",
	"pen",
	"pin",
	"pon",
	"pun",
	"ran",
	"ren",
	"rin",
	"ron",
	"run",
	"san",
	"sen",
	"sin",
	"son",
	"sun",
	"tan",
	"ten",
	"tin",
	"ton",
	"tun",
	"van",
	"ven",
	"vin",
	"von",
	"vun",
	"wan",
	"wen",
	"win",
	"won",
	"wun",
	"yan",
	"yen",
	"yin",
	"yon",
	"yun",
	"zan",
	"zen",
	"zin",
	"zon",
	"zun",
	"bap",
	"bep",
	"bip",
	"bop",
	"bup",
	"cap",
	"cep",
	"cip",
	"cop",
	"cup",
	"dap",
	"dep",
	"dip",
	"dop",
	"dup",
	"fap",
	"fep",
	"fip",
	"fop",
	"fup",
	"gap",
	"gep",
	"gip",
	"gop",
	"gup",
	"hap",
	"hep",
	"hip",
	"hop",
	"hup",
	"jap",
	"jep",
	"jip",
	"jop",
	"jup",
	"kap",
	"kep",
	"kip",
	"kop",
	"kup",
	"lap",
	"lep",
	"lip",
	"lop",
	"lup",
	"map",
	"mep",
	"mip",
	"mop",
	"mup",
	"nap",
	"nep",
	"nip",
	"nop",
	"nup",
	"pap",
	"pep",
	"pip",
	"pop",
	"pup",
	"rap",
	"rep",
	"rip",
	"rop",
	"rup",
	"sap",
	"sep",
	"sip",
	"sop",
	"sup",
	"tap",
	"tep",
	"tip",
	"top",
	"tup",
	"vap",
	"vep",
	"vip",
	"vop",
	"vup",
	"wap",
	"wep",
	"wip",
	"wop",
	"wup",
	"yap",
	"yep",
	"yip",
	"yop",
	"yup",
	"zap",
	"zep",
	"zip",
	"zop",
	"zup",
	"bar",
	"ber",
	"bir",
	"bor",
	"bur",
	"car",
	"cer",
	"cir",
	"cor",
	"cur",
	"dar",
	"der",
	"dir",
	"dor",
	"dur",
	"far",
	"fer",
	"fir",
	"for",
	"fur",
	"gar",
	"ger",
	"gir",
	"gor",
	"gur",
	"har",
	"her",
	"hir",
	"hor",
	"hur",
	"jar",
	"jer",
	"jir",
	"jor",
	"jur",
	"kar",
	"ker",
	"kir",
	"kor",
	"kur",
	"lar",
	"ler",
	"lir",
	"lor",
	"lur",
	"mar",
	"mer",
	"mir",
	"mor",
	"mur",
	"nar",
	"ner",
	"nir",
	"nor",
	"nur",
	"par",
	"per",
	"pir",
	"por",
	"pur",
	"rar",
	"rer",
	"rir",
	"ror",
	"rur",
	"sar",
	"ser",
	"sir",
	"sor",
	"sur",
	"tar",
	"ter",
	"tir",
	"tor",
	"tur",
	"var",
	"ver",
	"vir",
	"vor",
	"vur",
	"war",
	"wer",
	"wir",
	"wor",
	"wur",
	"yar",
	"yer",
	"yir",
	"yor",
	"yur",
	"zar",
	"zer",
	"zir",
	"zor",
	"zur",
	"bat",
	"bet",
	"bit",
	"bot",
	"but",
	"cat",
	"cet",
	"cit",
	"cot",
	"cut",
	"dat",
	"det",
	"dit",
	"dot",
	"dut",
	"fat",
	"fet",
	"fit",
	"fot",
	"fut",
	"gat",
	"get",
	"git",
	"got",
	"gut",
	"hat",
	"het",
	"hit",
	"hot",
	"hut",
	"jat",
	"jet",
	"jit",
	"jot",
	"jut",
	"kat",
	"ket",
	"kit",
	"kot",
	"kut",
	"lat",
	"let",
	"lit",
	"lot",
	"lut",
	"mat",
	"met",
	"mit",
	"mot",
	"mut",
	"nat",
	"net",
	"nit",
	"not",
	"nut",
	"pat",
	"pet",
	"pit",
	"pot",
	"put",
	"rat",
	"ret",
	"rit",
	"rot",
	"rut",
	"sat",
	"set",
	"sit",
	"sot",
	"sut",
	"tat",
	"tet",
	"tit",
	"tot",
	"tut",
	"vat",
	"vet",
	"vit",
	"vot",
	"vut",
	"wat",
	"wet",
	"wit",
	"wot",
	"wut",
	"yat",
	"yet",
	"yit",
	"yot",
	"yut",
	"zat",
	"zet",
	"zit",
	"zot",
	"zut",
	"baw",
	"bew",
	"biw",
	"bow",
	"buw",
	"caw",
	"cew",
	"ciw",
	"cow",
	"cuw",
	"daw",
	"dew",
	"diw",
	"dow",
	"duw",
	"faw",
	"few",
	"fiw",
	"fow",
	"fuw",
	"gaw",
	"gew",
	"giw",
	"gow",
	"guw",
	"haw",
	"hew",
	"hiw",
	"how",
	"huw",
	"jaw",
	"jew",
	"jiw",
	"jow",
	"juw",
	"kaw",
	"kew",
	"kiw",
	"kow",
	"kuw",
	"law",
	"lew",
	"liw",
	"low",
	"luw",
	"maw",
	"mew",
	"miw",
	"mow",
	"muw",
	"naw",
	"new",
	"niw",
	"now",
	"nuw",
	"paw",
	"pew",
	"piw",
	"pow",
	"puw",
	"raw",
	"rew",
	"riw",
	"row",
	"ruw",
	"saw",
	"sew",
	"siw",
	"sow",
	"suw",
	"taw",
	"tew",
	"tiw",
	"tow",
	"tuw",
	"vaw",
	"vew",
	"viw",
	"vow",
	"vuw",
	"waw",
	"wew",
	"wiw",
	"wow",
	"wuw",
	"yaw",
	"yew",
	"yiw",
	"yow",
	"yuw",
	"zaw",
	"zew",
	"ziw",
	"zow",
	"zuw",
	"bay",
	"bey",
	"biy",
	"boy",
	"buy",
	"cay",
	"cey",
	"ciy",
	"coy",
	"cuy",
	"day",
	"dey",
	"diy",
	"doy",
	"duy",
	"fay",
	"fey",
	"fiy",
	"foy",
	"fuy",
	"gay",
	"gey",
	"giy",
	"goy",
	"guy",
	"hay",
	"hey",
	"hiy",
	"hoy",
	"huy",
	"jay",
	"jey",
	"jiy",
	"joy",
	"juy",
	"kay",
	"key",
	"kiy",
	"koy",
	"kuy",
	"lay",
	"ley",
	"liy",
	"loy",
	"luy",
	"may",
	"mey",
	"miy",
	"moy",
	"muy",
	"nay",
	"ney",
	"niy",
	"noy",
	"nuy",
	"pay",
	"pey",
	"piy",
	"poy",
	"puy",
	"ray",
	"rey",
	"riy",
	"roy",
	"ruy",
	"say",
	"sey",
	"siy",
	"soy",
	"suy",
	"tay",
	"tey",
	"tiy",
	"toy",
	"tuy",
	"vay",
	"vey",
	"viy",
	"voy",
	"vuy",
	"way",
	"wey",
	"wiy",
	"woy",
	"wuy",
	"yay",
	"yey",
	"yiy",
	"yoy",
	"yuy",
	"zay",
	"zey",
	"ziy",
	"zoy",
	"zuy",
};

double rando::GetRandomValue() {
	return dist(mt);
}

float rando::GetRandomRange(float min, float max) {
	auto amount = (float)rando::GetRandomValue();
	return ((max - min) * amount) + min;
}

double rando::GetRandomRange(double min, double max) {
	auto amount = rando::GetRandomValue();
	return ((max - min) * amount) + min;
}

int rando::GetRandomRange(int min, int max) {
	auto amount = rando::GetRandomValue();
	return (int)round((max - min) * amount) + min;
}

size_t rando::GetRandomRange(size_t min, size_t max) {
	auto amount = rando::GetRandomValue();
	return (size_t)round((double)(max - min) * amount) + min;
}

std::string rando::GetPreString() {
	return preStrs[(int)(94.0 * rando::GetRandomValue())];
}

std::string rando::GetMidString() {
	return midStrs[(int)(94.0 * rando::GetRandomValue())];
}

std::string rando::GetPostString() {
	return postStr[(int)(854.0 * rando::GetRandomValue())];
}
