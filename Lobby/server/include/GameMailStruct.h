#ifndef __GAME_MAIL_STRUCT_H__
#define __GAME_MAIL_STRUCT_H__

#include "PropertyMailState.h"
#include "DefinePropertyStruct.h"
#include "MailDefine.h"
#include "ShareStructDefine.h"

//---------------------------------------------------------------------------------------------------------
// mail [2/5/2018 Chief]
//---------------------------------------------------------------------------------------------------------
DefinePropertyArchive(BaseMail);
DefinePropertyArchive(AttachmentMail);
DefinePropertyArchive(ContentMail);
DefinePropertyArchive(SystemMail);

//---------------------------------------------------------------------------------------------------------
// FixedString<> = array[mailid, status] [2/5/2018 Chief]
//---------------------------------------------------------------------------------------------------------
#define PROPERTYS_MailConfirm(PROPERTY)	\
	PROPERTY(FixedString<MAX_MAILID_LIST>, MailIDList, " ")\

struct tagMailConfirm
{
	STRUCT_SHARESTRUCT_LIST(PROPERTYS_MailConfirm);
};


#endif // __GAME_MAIL_STRUCT_H__
