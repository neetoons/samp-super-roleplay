#if defined _INC_y_inline_bcrypt
	#endinput
#endif
#define _INC_y_inline_bcrypt

/*
Legal:
	Version: MPL 1.1
	
	The contents of this file are subject to the Mozilla Public License Version 
	1.1 the "License"; you may not use this file except in compliance with 
	the License. You may obtain a copy of the License at 
	http://www.mozilla.org/MPL/
	
	Software distributed under the License is distributed on an "AS IS" basis,
	WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
	for the specific language governing rights and limitations under the
	License.
	
	The Original Code is the YSI framework.
	
	The Initial Developer of the Original Code is Alex "Y_Less" Cole.
	Portions created by the Initial Developer are Copyright (c) 2022
	the Initial Developer. All Rights Reserved.

Contributors:
	Y_Less
	koolk
	JoeBullet/Google63
	g_aSlice/Slice
	Misiur
	samphunter
	tianmeta
	maddinat0r
	spacemud
	Crayder
	Dayvison
	Ahmad45123
	Zeex
	irinel1996
	Yiin-
	Chaprnks
	Konstantinos
	Masterchen09
	Southclaws
	PatchwerkQWER
	m0k1
	paulommu
	udan111
	Cheaterman

Thanks:
	JoeBullet/Google63 - Handy arbitrary ASM jump code using SCTRL.
	ZeeX - Very productive conversations.
	koolk - IsPlayerinAreaEx code.
	TheAlpha - Danish translation.
	breadfish - German translation.
	Fireburn - Dutch translation.
	yom - French translation.
	50p - Polish translation.
	Zamaroht - Spanish translation.
	Los - Portuguese translation.
	Dracoblue, sintax, mabako, Xtreme, other coders - Producing other modes for
		me to strive to better.
	Pixels^ - Running XScripters where the idea was born.
	Matite - Pestering me to release it and using it.

Very special thanks to:
	Thiadmer - PAWN, whose limits continue to amaze me!
	Kye/Kalcor - SA:MP.
	SA:MP Team past, present and future - SA:MP.

Optional plugins:
	Gamer_Z - GPS.
	Incognito - Streamer.
	Me - sscanf2, fixes2, Whirlpool.
*/

#include "..\..\YSI_Core\y_utils"
#include "..\..\YSI_Server\y_thirdpartyinclude\y3_indirection"
#include "..\y_bcrypt\y_bcrypt_natives"

static stock const
	YSI_gsc_Inline_BCryptCheckDone[] = "_Inline_BCryptCheckDone",
	YSI_gsc_Inline_BCryptHashDone[] = "_Inline_BCryptHashDone";

forward _Inline_BCryptCheckDone(Func:cb<i>, success);
forward _Inline_BCryptHashDone(Func:cb<si>, arg2);

public _Inline_BCryptCheckDone(Func:cb<i>, success)
{
	if (success == -1)
	{
		success = _YSI_BCryptIsEqualA();
	}
	@.cb(success);
	Indirect_Release(cb);
}

public _Inline_BCryptHashDone(Func:cb<si>, arg2)
{
	new
		hash[61],
		hashid = Indirect_GetMeta(cb);
	if (numargs() == 2)
	{
		// Secret internal hash ID.
		_YSI_BCryptGetHashB(arg2, hash, sizeof (hash));
	}
	else if (BCrypt_GetPlugin() == BCRYPT_PLUGIN_VERSION_LASSIR)
	{
		_YSI_BCryptGetHashA(hash);
	}
	else
	{
		// Can't be called if there's no valid plugin found.  So this is safe.
		_YSI_BCryptGetHashE(hash, sizeof (hash));
	}
	if (Indirect_GetTag(cb) == _:tagof (F@_@si:))
	{
		Indirect_Call(_:cb, tagof (F@_@si:), hash, hashid);
	}
	else
	{
		Indirect_Call(_:cb, tagof (F@_@s:), hash);
	}
	Indirect_Release(cb);
}

stock bool:BCrypt_CheckInline(const password[], const hash[], Func:cb<i>)
{
	switch (BCrypt_GetPlugin())
	{
	case BCRYPT_PLUGIN_VERSION_LASSIR:
		return _YSI_BCryptCheckA(password, hash, YSI_gsc_Inline_BCryptCheckDone, YSI_gcII, _:cb, -1);
	case BCRYPT_PLUGIN_VERSION_SREYAS_1, BCRYPT_PLUGIN_VERSION_SREYAS_2, BCRYPT_PLUGIN_VERSION_SREYAS_3, BCRYPT_PLUGIN_VERSION_SREYAS_4:
		return _YSI_BCryptCheckB(_:cb, YSI_gsc_Inline_BCryptCheckDone, password, hash);
	default:
		return false;
	}
}

stock BCrypt_HashInline(const password[], cost = 12, {F@_@s, F@_@si}:cb, tag = tagof (cb))
{
	++_YSI_gHashID;
	Indirect_SetTag(cb, tag);
	Indirect_SetMeta(cb, _YSI_gHashID);
	switch (BCrypt_GetPlugin())
	{
	case BCRYPT_PLUGIN_VERSION_LASSIR:
		return _YSI_BCryptHashA(password, cost, YSI_gsc_Inline_BCryptHashDone, YSI_gcI, _:cb) ? _YSI_gHashID : 0;
	case BCRYPT_PLUGIN_VERSION_SREYAS_1, BCRYPT_PLUGIN_VERSION_SREYAS_2, BCRYPT_PLUGIN_VERSION_SREYAS_3, BCRYPT_PLUGIN_VERSION_SREYAS_4:
		return _YSI_BCryptHashB(_:cb, YSI_gsc_Inline_BCryptHashDone, password, cost) ? _YSI_gHashID : 0;
	default:
		return 0;
	}
}

