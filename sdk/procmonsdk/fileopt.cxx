
#include "pch.hpp"
#include "procmgr.hpp"
#include "fileopt.hpp"
#include "eventview.hpp"

#define IRP_MJ_CREATE                   0x00
#define IRP_MJ_CREATE_NAMED_PIPE        0x01
#define IRP_MJ_CLOSE                    0x02
#define IRP_MJ_READ                     0x03
#define IRP_MJ_WRITE                    0x04
#define IRP_MJ_QUERY_INFORMATION        0x05
#define IRP_MJ_SET_INFORMATION          0x06
#define IRP_MJ_QUERY_EA                 0x07
#define IRP_MJ_SET_EA                   0x08
#define IRP_MJ_FLUSH_BUFFERS            0x09
#define IRP_MJ_QUERY_VOLUME_INFORMATION 0x0a
#define IRP_MJ_SET_VOLUME_INFORMATION   0x0b
#define IRP_MJ_DIRECTORY_CONTROL        0x0c
#define IRP_MJ_FILE_SYSTEM_CONTROL      0x0d
#define IRP_MJ_DEVICE_CONTROL           0x0e
#define IRP_MJ_INTERNAL_DEVICE_CONTROL  0x0f
#define IRP_MJ_SHUTDOWN                 0x10
#define IRP_MJ_LOCK_CONTROL             0x11
#define IRP_MJ_CLEANUP                  0x12
#define IRP_MJ_CREATE_MAILSLOT          0x13
#define IRP_MJ_QUERY_SECURITY           0x14
#define IRP_MJ_SET_SECURITY             0x15
#define IRP_MJ_POWER                    0x16
#define IRP_MJ_SYSTEM_CONTROL           0x17
#define IRP_MJ_DEVICE_CHANGE            0x18
#define IRP_MJ_QUERY_QUOTA              0x19
#define IRP_MJ_SET_QUOTA                0x1a
#define IRP_MJ_PNP                      0x1b


CString CFileEvent::GetPath()
{
	PLOG_ENTRY pEntry = reinterpret_cast<PLOG_ENTRY>(getPreLog().GetBuffer());
	PLOG_FILE_OPT pFileOpt = TO_EVENT_DATA(PLOG_FILE_OPT, pEntry);

	CString strFileName;
	strFileName.Append(pFileOpt->Name, pFileOpt->NameLength);

	return strFileName;
}

CString CFileEvent::GetDetail()
{
	PLOG_ENTRY pEntry = reinterpret_cast<PLOG_ENTRY>(getPreLog().GetBuffer());
	PLOG_ENTRY pPostEntry = reinterpret_cast<PLOG_ENTRY>(getPostLog().GetBuffer());

	PLOG_FILE_OPT pFileOpt = TO_EVENT_DATA(PLOG_FILE_OPT, pEntry);
	UCHAR MajorFunction = pEntry->NotifyType - 20;
	CString strDetail;

	switch (MajorFunction)
	{
	case IRP_MJ_CREATE:
	{

		//
		// Set path
		//

		CString strTemp;

		PLOG_FILE_CREATE pCreateInfo = reinterpret_cast<PLOG_FILE_CREATE>(pFileOpt->Name + pFileOpt->NameLength);
		strTemp.Format(TEXT("DesiredAccess: 0x%x"), pCreateInfo->DesiredAccess);

		strDetail += strTemp;
		strDetail += TEXT("\r\n");

		strTemp.Format(TEXT("Io Status: 0x%x"), pPostEntry->Status);

		strDetail += strTemp;
		strDetail += TEXT("\r\n");

		//
		// TODO ShareMode. Disposition. Attributes. AllocationSize.
		//
		
	}
		break;
	default:
		strDetail = TEXT("TODO");
		break;
	}

	return strDetail;
}
